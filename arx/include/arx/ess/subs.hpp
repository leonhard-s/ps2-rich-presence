// Copyright 2022 Leonhard S.

#ifndef ARX_ESS_SUBS_HPP
#define ARX_ESS_SUBS_HPP

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QJsonObject>

namespace arx
{

    /**
     * A subscription to the event streaming service (ESS).
     *
     * Subscriptions are used to generate the (un-)subscription payloads used
     * in the WebSocket connection. They can be thought of as factories for the
     * ESS subscription messages.
     */
    class Subscription
    {
    public:
        explicit Subscription(const QString &event_name);
        explicit Subscription(const QStringList &event_names);
        Subscription(const QString &event_name,
                     const QStringList &character_ids,
                     const QStringList &world_ids,
                     bool logical_and = false);
        Subscription(const QStringList &event_names,
                     const QStringList &character_ids,
                     const QStringList &world_ids,
                     bool logical_and = false);

        bool operator==(const Subscription &other) const;
        bool operator!=(const Subscription &other) const;

        QStringList getEventNames() const;
        QStringList getCharacterIds() const;
        QStringList getWorldIds() const;
        bool getLogicalAndFlag() const;

        QJsonObject buildSubscribePayload() const;
        QJsonObject buildUnsubscribePayload() const;
        static QJsonObject buildUnsubscribeAllPayload();

    private:
        QJsonObject buildBasicPayload() const;

        bool logical_and_;
        QStringList event_names_;
        QStringList character_ids_;
        QStringList world_ids_;
    };

} // namespace arx

#endif // ARX_ESS_SUBS_HPP
