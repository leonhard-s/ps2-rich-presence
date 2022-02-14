// Copyright 2022 Leonhard S.

#ifndef ARX_PAYLOAD_HPP
#define ARX_PAYLOAD_HPP

#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QString>

namespace arx
{

    bool isPayloadValid(const QString &collection, const QJsonObject &payload);

    bool isPayloadEmpty(const QString &collection, const QJsonObject &payload);

    QJsonObject payloadResultAsObject(const QString &collection,
                                      const QJsonObject &payload);

    QJsonArray payloadResultsAsArray(const QString &collection,
                                     const QJsonObject &payload);

} // namespace arx

#endif // ARX_PAYLOAD_HPP
