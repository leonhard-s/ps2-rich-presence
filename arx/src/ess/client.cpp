// Copyright 2022 Leonhard S.

#include "arx/ess/client.hpp"

#include <string>

#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QtCore/QUrl>
#include <QtCore/QUrlQuery>
#include <QtWebSockets/QWebSocket>

#include "arx/types.hpp"
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
            auto payload = subscription.buildSubscribePayload();
            ws_.sendTextMessage(QString::fromStdString(payload.dump(4)));
        }
    }

    void EssClient::unsubscribe(const Subscription subscription)
    {
        subscriptions_.removeAll(subscription);
        if (isConnected())
        {
            auto payload = subscription.buildUnsubscribePayload();
            ws_.sendTextMessage(QString::fromStdString(payload.dump(4)));
        }
        emit subscriptionRemoved(subscription);
    }

    void EssClient::clearSubscriptions()
    {
        auto old_subs = subscriptions_;
        subscriptions_.clear();
        if (isConnected())
        {
            auto payload = Subscription::buildUnsubscribeAllPayload();
            ws_.sendTextMessage(QString::fromStdString(payload.dump(4)));
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
        auto json = json_object::parse(message.toStdString());
        // Ignore messages that are not for us
        auto it = json.find("service");
        if (it == json.end() || it->get<std::string>() != "event")
        {
            return;
        }
        // Dispatch
        if (json["type"] == "serviceMessage")
        {
            json_object payload = json["payload"];
            auto event_name = QString::fromStdString(
                payload["event_name"].get<std::string>());
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
