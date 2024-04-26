// Copyright 2024 Leonhard S.
#pragma once

#include <memory>

#include <QList>
#include <QJsonObject>
#include <QObject>
#include <QString>

namespace ax { class Subscription; }
namespace dbg_census::stream { class EssClient; }

namespace PresenceLib {

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
    const QList<QJsonObject>& getSubscriptions() const;

Q_SIGNALS:
    void connected();
    void disconnected();
    void messageReceived(QString message);
    void payloadReceived(QString event_name, const QJsonObject& payload);
    void subscriptionAdded(const QJsonObject& subscription);
    void subscriptionRemoved(const QJsonObject& subscription);
    void subscriptionsCleared();

public Q_SLOTS:
    void connect();
    void disconnect();
    void reconnect();
    void subscribe(const QJsonObject& subscription);
    void unsubscribe(const QJsonObject& subscription);

private:
    QString service_id_;
    QList<QJsonObject> subscriptions_;
    std::unique_ptr<dbg_census::stream::EssClient> ess_client_;

    void onConnected();
    void onDisconnected();
    void parseMessage(const QString& message);
};

} // namespace PresenceApp
