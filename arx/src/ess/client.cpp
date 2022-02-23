// Copyright 2022 Leonhard S.

#include "arx/ess/client.hpp"

#include <QtCore/QDebug>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QtCore/QUrl>
#include <QtCore/QUrlQuery>
#include <QtWebSockets/QWebSocket>

#include "arx/ess/subs.hpp"

namespace arx
{

    EssClient::EssClient(const QString &service_id, const QString &environment, QObject *parent)
        : QObject{parent}, service_id_{service_id}, environment_{environment}, subscriptions_{}
    {
        QObject::connect(&ws_, &QWebSocket::connected, this, &EssClient::onConnected);
        QObject::connect(&ws_, &QWebSocket::disconnected, this, &EssClient::onDisconnected);
        QObject::connect(&ws_, &QWebSocket::textMessageReceived, this, &EssClient::parseMessage);
    }

    bool EssClient::isConnected() const
    {
        return ws_.isValid() && ws_.state() == QAbstractSocket::SocketState::ConnectedState;
    }

    const QList<Subscription> EssClient::getSubscriptions() const
    {
        return subscriptions_;
    }

    void EssClient::connect()
    {
        const QUrl url = getEssEndpointUrl(service_id_, environment_);
        ws_.open(url);
        // Print the URL to the console for debugging purposes.
        qDebug() << "Connecting to" << url.toString();
    }

    void EssClient::disconnect()
    {
        ws_.close();
    }

    void EssClient::reconnect()
    {
        if (ws_.isValid())
        {
            disconnect();
        }
        connect();
    }

    void EssClient::subscribe(const Subscription subscription)
    {
        subscriptions_.append(subscription);
        if (isConnected())
        {
            QJsonObject payload = subscription.buildSubscribePayload();
            ws_.sendTextMessage(QJsonDocument{payload}.toJson(QJsonDocument::Compact));
        }
    }

    void EssClient::unsubscribe(const Subscription subscription)
    {
        subscriptions_.removeAll(subscription);
        if (isConnected())
        {
            QJsonObject payload = subscription.buildUnsubscribePayload();
            ws_.sendTextMessage(QJsonDocument{payload}.toJson(QJsonDocument::Compact));
        }
        emit subscriptionRemoved(subscription);
    }

    void EssClient::clearSubscriptions()
    {
        auto old_subs = subscriptions_;
        subscriptions_.clear();
        if (isConnected())
        {
            QJsonObject payload = Subscription::buildUnsubscribeAllPayload();
            ws_.sendTextMessage(QJsonDocument{payload}.toJson(QJsonDocument::Compact));
        }
        for (auto const &subscription : old_subs)
        {
            emit subscriptionRemoved(subscription);
        }
        emit subscriptionsCleared();
    }

    void EssClient::onConnected()
    {
        emit connected();
        auto timer = new QTimer(this);
        timer->setInterval(100);
        timer->setSingleShot(true);
        QObject::connect(timer, &QTimer::timeout, [timer, this]()
                         {
                            timer->deleteLater();
                            for (const auto &subscription : subscriptions_)
                            {
                                subscribe(subscription);
                            } });
        timer->start();
    }

    void EssClient::onDisconnected()
    {
        emit disconnected();
    }

    void EssClient::parseMessage(const QString message)
    {
        // Forward raw payload to anyone who's asking
        emit messageReceived(message);
        // Convert the text message to a JSON object
        QJsonParseError *error = nullptr;
        QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8(), error);
        if (error != nullptr)
        {
            qDebug() << "Error parsing message: " << error->errorString();
            return;
        }
        QJsonObject json = doc.object();
        // Ignore messages that are not for us
        if (json["service"].toString() != "event")
        {
            return;
        }
        // Dispatch
        if (json["type"] == "serviceMessage")
        {
            QJsonObject payload = json["payload"].toObject();
            QString event_name = payload["event_name"].toString();
            emit payloadReceived(event_name, payload);
        }
        else if (json["type"] == "heartbeat")
        {
            emit heartbeatReceived(json);
        }
    }

    QUrl EssClient::getEssEndpointUrl(const QString &service_id, const QString &environment)
    {
        QUrl url;
        url.setScheme("wss");
        url.setHost("push.planetside2.com");
        url.setPath("/streaming");
        QUrlQuery query;
        query.addQueryItem("environment", environment);
        query.addQueryItem("service-id", service_id);
        url.setQuery(query);
        return url;
    }

} // namespace arx
