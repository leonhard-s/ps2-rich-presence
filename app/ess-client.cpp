// Copyright 2022 Leonhard S.

#include "ess-client.hpp"

#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QtCore/QUrl>
#include <QtWebSockets/QWebSocket>

#include "arx.hpp"
#include "arx/ess.hpp"

namespace PresenceApp {

EssClient::EssClient(const QString& service_id, QObject* parent)
    : QObject{ parent }
    , service_id_{ service_id }
    , subscriptions_{}
{
    QObject::connect(&ws_, &QWebSocket::connected,
        this, &EssClient::onConnected);
    QObject::connect(&ws_, &QWebSocket::disconnected,
        this, &EssClient::onDisconnected);
    QObject::connect(&ws_, &QWebSocket::textMessageReceived,
        this, &EssClient::parseMessage);
}

bool EssClient::isConnected() const {
    return ws_.isValid() && ws_.state() == QAbstractSocket::SocketState::ConnectedState;
}

const QList<arx::Subscription>& EssClient::getSubscriptions() const {
    return subscriptions_;
}

void EssClient::connect() {
    const QUrl url = QUrl(QString::fromStdString(
        arx::getEndpointUrl(service_id_.toStdString())));
    ws_.open(url);
    qDebug() << "Connecting to" << url.toString();
}

void EssClient::disconnect() {
    ws_.close();
}

void EssClient::subscribe(const arx::Subscription subscription) {
    if (!subscriptions_.contains(subscription)) {
        subscriptions_.append(subscription);
        emit subscriptionAdded(subscription);
    }
    if (isConnected()) {
        auto msg = QString::fromStdString(
            subscription.buildSubscribeMessage());
        qDebug() << "Sending: " << msg;
        ws_.sendTextMessage(msg);
    }
}

void EssClient::unsubscribe(const arx::Subscription subscription) {
    if (subscriptions_.removeAll(subscription)) {
        emit subscriptionRemoved(subscription);
    }
    if (isConnected()) {
        ws_.sendTextMessage(QString::fromStdString(
            subscription.buildUnsubscribeMessage()));
    }
}

void EssClient::reconnect() {
    disconnect();
    connect();
}

void EssClient::onConnected() {
    auto timer = new QTimer(this);
    timer->setInterval(1000);
    timer->setSingleShot(true);
    QObject::connect(timer, &QTimer::timeout, [timer, this]() {
        qDebug() << "Timer expired, subscribing";
        timer->deleteLater();
        std::for_each(subscriptions_.begin(), subscriptions_.end(),
            [this](const auto& subscription) {
                subscribe(subscription);
            });
        });
    timer->start();
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

#if defined(_MSC_VER) && !defined(__clang__)
#   pragma warning(push)
#   pragma warning(disable : 4464)
#elif defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wreserved-identifier"
#endif

#include "moc_ess-client.cpp"

#if defined(_MSC_VER) && !defined(__clang__)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif
