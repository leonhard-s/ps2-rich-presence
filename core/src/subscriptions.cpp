// Copyright 2024 Leonhard S.
#include <api/subscriptions.h>

#include <cstdint>

#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <dbg_census/stream/subscription_builder.h>

namespace {

QJsonObject qJsonObjectFromStdString(const std::string& str) {
    return QJsonDocument::fromJson(QString::fromStdString(str).toUtf8()).object();
}

} // anonymous namespace

namespace PresenceLib {

QJsonObject getDeathSubscription(int64_t character_id) {
    auto builder = dbg_census::stream::SubscriptionBuilder{ "Death" };
    builder.setCharacters({ character_id });
    return qJsonObjectFromStdString(builder.build());
}

QJsonObject getExperienceSubscription(int64_t character_id) {
    auto builder = dbg_census::stream::SubscriptionBuilder{ "GainExperience" };
    builder.setCharacters({ character_id });
    return qJsonObjectFromStdString(builder.build());
}

} // namespace PresenceLib
