// Copyright 2022 Leonhard S.

#pragma once

#include <memory>

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>

#include "arx.hpp"
#include "arx/ess.hpp"

namespace dbg_census::stream {

class EssClient;

} // namespace dbg_census::stream

namespace PresenceApp {

/**
 * WebSocket client for the PS2 event streaming service.
 */
class EssClient : public QObject {
    Q_OBJECT

public:
    explicit EssClient(QString service_id, QObject* parent = nullptr);
    ~EssClient() override;
    EssClient(const EssClient& other) = delete;
    EssClient(EssClient&& other) noexcept = delete;

    EssClient& operator=(const EssClient& other) = delete;
    EssClient& operator=(EssClient&& other) noexcept = delete;

    bool isConnected() const;
    const QList<arx::Subscription>& getSubscriptions() const;

Q_SIGNALS:
    void connected();
    void disconnected();
    void messageReceived(QString message);
    void payloadReceived(QString event_name, arx::json_t payload);
    void subscriptionAdded(const arx::Subscription& subscription);
    void subscriptionRemoved(const arx::Subscription& subscription);
    void subscriptionsCleared();

public Q_SLOTS:
    void connect();
    void disconnect();
    void reconnect();
    void subscribe(const arx::Subscription& subscription);
    void unsubscribe(const arx::Subscription& subscription);

private:
    QString service_id_;
    QList<arx::Subscription> subscriptions_;
    std::unique_ptr<dbg_census::stream::EssClient> ess_client_;

    void onConnected();
    void onDisconnected();
    void parseMessage(const QString& message);
};

} // namespace PresenceApp
