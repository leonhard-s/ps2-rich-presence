// Copyright 2022 Leonhard S.

#ifndef ARX_ESS_CLIENT_HPP
#define ARX_ESS_CLIENT_HPP

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtWebSockets/QWebSocket>

#include "arx/types.hpp"
#include "arx/ess/subs.hpp"

namespace arx
{

    /**
     * WebSocket client for the PS2 event streaming service.
     *
     * Event subscriptions can be added via the "subscribe" and "unsubscrube"
     * methods. If the WebSocket is running when the subscription is added, the
     * subscription will be sent immediately. Otherwise, the subscription will
     * be sent when the WebSocket is (re)connected.
     */
    class EssClient : public QObject
    {
        Q_OBJECT

    public:
        explicit EssClient(const QString &service_id,
                           const QString &environment = "ps2",
                           QObject *parent = nullptr);

        bool isConnected() const;
        const QList<Subscription> getSubscriptions() const;

    Q_SIGNALS:
        void connected();
        void disconnected();
        void heartbeatReceived(json_object data);
        void messageReceived(QString message);
        void payloadReceived(QString event_name, json_object payload);
        void subscriptionAdded(Subscription subscription);
        void subscriptionRemoved(Subscription subscription);
        void subscriptionsCleared();

    public Q_SLOTS:
        void connect();
        void disconnect();
        void reconnect();
        void subscribe(const Subscription subscription);
        void unsubscribe(const Subscription subscription);
        void clearSubscriptions();

    private Q_SLOTS:
        void onConnected();
        void onDisconnected();
        void parseMessage(const QString message);

    private:
        static QUrl getEssEndpointUrl(const QString &service_id,
                                      const QString &environment);

        QString service_id_;
        QString environment_;
        QList<Subscription> subscriptions_;
        QWebSocket ws_;
    };

} // namespace arx

#endif // ARX_ESS_CLIENT_HPP
