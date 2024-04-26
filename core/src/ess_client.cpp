// Copyright 2024 Leonhard S.
#include <api/ess_client.h>

#include <utility>

#include <QJsonDocument>
#include <QJsonObject>

#include <dbg_census/stream/ess_client.h>
#include <dbg_census/stream/subscription_builder.h>
#include <moc_macros.h>

namespace {

std::string getMessageType(const QJsonObject& json) {
    auto it = json.find("type");
    if (it == json.end() || !it->isString()) {
        return "";
    }
    return it->toString().toStdString();
}

QJsonObject getPayload(const QJsonObject& json) {
    auto it = json.find("payload");
    if (it == json.end() || !it->isObject()) {
        return {};
    }
    auto payload = it->toObject();
    if (payload.find("event_name") == payload.end()) {
        return {};
    }
    return payload;
}

} // anonymous namespace

namespace PresenceLib {

EssClient::EssClient(QString service_id, QObject* parent)
    : QObject{ parent }
    , service_id_{ std::move(service_id) }
{}

EssClient::~EssClient() = default;

bool EssClient::isConnected() const {
    // TODO: Reconnect handling
    return ess_client_ != nullptr;
}

const QList<QJsonObject>& EssClient::getSubscriptions() const {
    return subscriptions_;
}

void EssClient::connect() {
    ess_client_ = std::make_unique<dbg_census::stream::EssClient>(
        service_id_.toStdString());

    // Hook up signals
    ess_client_->setOnReadyCallback([this]() {
        onConnected();
        });
    ess_client_->setOnDisconnectCallback([this]() {
        emit disconnected();
        });
    // TODO: Use payload callback
    ess_client_->setOnMessageCallback([this](const std::string& msg) {
        parseMessage(QString::fromStdString(msg));
        });

    qDebug() << "Connecting to ESS service";
    ess_client_->connect();
}

void EssClient::disconnect() {
    ess_client_->disconnect();
    ess_client_.reset();
}

void EssClient::subscribe(const QJsonObject& subscription) {
    if (!subscriptions_.contains(subscription)) {
        subscriptions_.append(subscription);
        emit subscriptionAdded(subscription);
    }
    if (isConnected()) {
        auto doc = QJsonDocument{ subscription };
        auto msg = doc.toJson().toStdString();
        qDebug() << "Sending: " << msg;
        ess_client_->send(msg);
    }
}

void EssClient::unsubscribe(const QJsonObject& subscription) {
    if (subscriptions_.removeAll(subscription) > 0) {
        emit subscriptionRemoved(subscription);
    }
    if (isConnected()) {
        auto doc = QJsonDocument{ subscription };
        auto msg = doc.toJson().toStdString();
        ess_client_->send(msg);
    }
}

void EssClient::reconnect() {
    disconnect();
    connect();
}

void EssClient::onConnected() {
    std::for_each(subscriptions_.begin(), subscriptions_.end(),
        [this](const auto& subscription) { subscribe(subscription); });
    emit connected();
}

void EssClient::onDisconnected() {
    emit disconnected();
}

void EssClient::parseMessage(const QString& message) {
    emit messageReceived(message);
    // Convert the text message to a JSON object
    auto json = QJsonDocument::fromJson(message.toUtf8()).object();

    // Ignore anything but event subscription messages
    if (getMessageType(json) != "serviceMessage") {
        return;
    }
    auto payload = getPayload(json);
    if (payload.empty()) {
        qWarning() << "Ignoring bad service message:" << message;
        return;
    }
    // Dispatch payload
    auto event_name = QString::fromStdString(
        payload["event_name"].toString().toStdString());
    emit payloadReceived(event_name, payload);
}

} // namespace PresenceLib

PUSH_MOC_WARNINGS_FILTER;
#include "api/moc_ess_client.cpp"
POP_MOC_WARNINGS_FILTER;
