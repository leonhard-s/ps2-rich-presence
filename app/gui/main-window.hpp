// Copyright 2022 Leonhard S.

#pragma once

#include <memory>

#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QList>
#include <QObject>
#include <QPushButton>
#include <QTimer>
#include <QWidget>

#include "game/character-info.hpp"
#include "core.hpp"

namespace PresenceApp {

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow();
    MainWindow(const MainWindow& other) = delete;
    MainWindow(MainWindow&& other) noexcept = delete;

    MainWindow& operator=(const MainWindow& other) = delete;
    MainWindow& operator=(MainWindow&& other) noexcept = delete;

    bool isPresenceEnabled() const;
    bool isTrackerRunning() const;

    void saveConfig();
    void loadConfig();

public slots:
    void startTracking(const CharacterData& character);
    void stopTracking();

private Q_SLOTS:
    void onCharacterChanged(int index);
    void onEventPayloadReceived();
    void onLastSeenTimerExpired();

private:
    void openCharacterManager(const QList<CharacterData>& characters = {});
    void updateEventLatency();
    void updateEventFrequency();
    void updateLastSeenLabels();

    static QString getProjectLink();
    void setEventFrequency(double events_per_second);
    void setEventLatency(qint32 latency_ms);
    void setLastPayload(const QDateTime& timestamp);
    void setLastPresence(const QDateTime& timestamp);
    void setStatus(const QString& status);
    void setTrackingEnabled(bool enabled);

    void resetCharacterComboBox();
    void setupUi();

    std::unique_ptr<RichPresenceApp> app_;
    std::unique_ptr<QTimer> last_seen_timer_;

    // GUI elements
    QComboBox* characters_combo_box_;
    QLabel* status_;
    QLabel* latency_;
    QLabel* event_frequency_;
    QLabel* payload_ago_;
    QLabel* presence_ago_;
    QCheckBox* auto_track_;
    QCheckBox* start_with_windows_;
    QCheckBox* minimise_to_tray_;
    QPushButton* enable_button_;
    QPushButton* minimise_button_;
};

} // namespace PresenceApp
