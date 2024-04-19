// Copyright 2024 Leonhard S.
#pragma once

#include <memory>

#include <QObject>

// Forward declarations
class QJsonObject;
class QString;
class QThread;
namespace dbg_census::rest { class RestApiClient; }

namespace PresenceLib {

class AsyncRestClient : public QObject {
    Q_OBJECT

public:
    explicit AsyncRestClient(QObject* parent = nullptr);
    ~AsyncRestClient() override;

    void request(const QString& url);

signals:
    void requestFinished(const QJsonDocument& response);
    void requestFailed(const QString& error);

private:
    std::unique_ptr<dbg_census::rest::RestApiClient> rest_client_;
    std::unique_ptr<QThread> worker_thread_;

    Q_INVOKABLE void doRequest(const QString& url);
};

} // namespace PresenceLib

Q_DECLARE_METATYPE(PresenceLib::AsyncRestClient)
