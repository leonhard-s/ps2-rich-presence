// Copyright 2022 Leonhard S.

#pragma once

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtWebSockets/QWebSocket>

#include "arx.hpp"
#include "arx/ess.hpp"

namespace ps2rpc {

/**
 * WebSocket client for the PS2 event streaming service.
 */
class EssClient: public QObject
{
    Q_OBJECT

public:
    explicit EssClient(const QString& service_id,
        QObject* parent = nullptr);

    bool isConnected() const;
    const QList<arx::Subscription>& getSubscriptions() const;

Q_SIGNALS:
    void connected();
    void disconnected();
    void messageReceived(QString message);
    void payloadReceived(QString event_name, arx::json_t payload);
    void subscriptionAdded(const arx::Subscription subscription);
    void subscriptionRemoved(const arx::Subscription subscription);
    void subscriptionsCleared();

public Q_SLOTS:
    void connect();
    void disconnect();
    void reconnect();
    void subscribe(const arx::Subscription subscription);
    void unsubscribe(const arx::Subscription subscription);

private Q_SLOTS:
    void onConnected();
    void onDisconnected();
    void parseMessage(const QString& message);

private:
    QString service_id_;
    QList<arx::Subscription> subscriptions_;
    QWebSocket ws_;
};

} // namespace ps2rpc
