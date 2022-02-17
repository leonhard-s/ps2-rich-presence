// Copyright 2022 Leonhard S.

#include "arx/payload.hpp"

#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QString>

namespace
{

    QString getResultListName(const QString &collection)
    {
        return collection + "_list";
    }

} // namespace

namespace arx
{

    bool isPayloadValid(const QString &collection, const QJsonObject &payload)
    {
        if (payload.contains("error") || payload.contains("errorCode"))
        {
            qDebug() << "Payload for collection" << collection
                     << "contains error message:" << payload;
            return false;
        }
        // Ensure the given payload contains the required keys.
        if (!payload.contains("returned"))
        {
            qDebug() << "Payload for collection" << collection
                     << "does not contain required key 'returned'.";
            return false;
        }
        if (!payload.contains(getResultListName(collection)))
        {
            qDebug() << "Payload for collection" << collection
                     << "does not contain results list";
            return false;
        }
        return true;
    }

    bool isPayloadEmpty(const QString &collection, const QJsonObject &payload)
    {
        if (payload["returned"].toInt() == 0)
        {
            return true;
        }
        auto key = getResultListName(collection);
        if (payload.contains(key))
        {
            return payload[key].toArray().isEmpty();
        }
        return false;
    }

    QJsonObject payloadResultAsObject(const QString &collection,
                                      const QJsonObject &payload)
    {
        auto key = getResultListName(collection);
        return payload[key].toArray()[0].toObject();
    }

    QJsonArray payloadResultsAsArray(const QString &collection,
                                     const QJsonObject &payload)
    {
        auto key = getResultListName(collection);
        return payload[key].toArray();
    }

} // namespace arx
