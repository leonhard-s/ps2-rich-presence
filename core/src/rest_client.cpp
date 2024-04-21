// Copyright 2024 Leonhard S.
#include <api/rest_client.h>

#include <QJsonDocument>
#include <QString>
#include <QThread>
#include <dbg_census/rest/rest_api_client.h>
#include <moc_macros.h>

namespace PresenceLib {

AsyncRestClient::AsyncRestClient(QObject* parent)
    : QObject(parent)
    , rest_client_{ std::make_unique<dbg_census::rest::RestApiClient>() }
    , worker_thread_{ std::make_unique<QThread>() }
{
    moveToThread(worker_thread_.get());
    // NOTE: One class per call for now - not pretty, but works
    QObject::connect(worker_thread_.get(), &QThread::finished, this, &QObject::deleteLater);
    worker_thread_->start();
}

AsyncRestClient::~AsyncRestClient() {
    worker_thread_->quit();
    worker_thread_->wait();
}

void AsyncRestClient::request(const QString& url) {
    QMetaObject::invokeMethod(this, "doRequest", Qt::QueuedConnection, Q_ARG(QString, url));
}

void AsyncRestClient::doRequest(const QString& url) {
    auto response = rest_client_->request(url.toStdString());
    if (!response.has_value()) {
        emit requestFailed("Request failed");
        return;
    }
    const auto json_data = QString::fromStdString(response.value());
    emit requestFinished(QJsonDocument::fromJson(json_data.toUtf8()));
}

} // namespace PresenceLib

PUSH_MOC_WARNINGS_FILTER;
#include "api/moc_rest_client.cpp"
POP_MOC_WARNINGS_FILTER;
