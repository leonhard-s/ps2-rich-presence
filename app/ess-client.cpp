// Copyright 2022 Leonhard S.

#include "ess-client.hpp"

#include <utility>

#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QtCore/QUrl>

#include <dbg_census/stream/ess_client.h>

#include "arx.hpp"
#include "arx/ess.hpp"

#include "moc_macros.hpp"

namespace PresenceApp {

EssClient::EssClient(QString service_id, QObject* parent)
    : QObject{ parent }
    , service_id_{ std::move(service_id) }
{}

EssClient::~EssClient() = default;

bool EssClient::isConnected() const {
    // TODO: Reconnect handling
    return ess_client_ != nullptr;
}

const QList<arx::Subscription>& EssClient::getSubscriptions() const {
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

void EssClient::subscribe(const arx::Subscription& subscription) {
    if (!subscriptions_.contains(subscription)) {
        subscriptions_.append(subscription);
        emit subscriptionAdded(subscription);
    }
    if (isConnected()) {
        auto msg = subscription.buildSubscribeMessage();
        qDebug() << "Sending: " << msg;
        ess_client_->send(msg);
    }
}

void EssClient::unsubscribe(const arx::Subscription& subscription) {
    if (subscriptions_.removeAll(subscription) > 0) {
        emit subscriptionRemoved(subscription);
    }
    if (isConnected()) {
        ess_client_->send(subscription.buildUnsubscribeMessage());
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
    auto json = arx::json_t::parse(message.toStdString());
    // Ignore anything but event subscription messages
    if (arx::getMessageType(json) != arx::MessageType::SERVICE_MESSAGE) {
        return;
    }
    auto payload = arx::getPayload(json);
    if (payload.empty()) {
        qWarning() << "Ignoring bad service message:" << message;
        return;
    }
    // Dispatch payload
    auto event_name = QString::fromStdString(
        payload["event_name"].get<arx::json_string_t>());
    emit payloadReceived(event_name, payload);
}

} // namespace PresenceApp

PUSH_MOC_WARNINGS_FILTER;
#include "moc_ess-client.cpp"
POP_MOC_WARNINGS_FILTER;
