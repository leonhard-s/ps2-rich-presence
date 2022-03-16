// Copyright 2022 Leonhard S.

#ifndef PS2RPC_GUI_MAINWINDOW_HPP
#define PS2RPC_GUI_MAINWINDOW_HPP

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QScopedPointer>
#include <QtCore/QTimer>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

#include "game/character-info.hpp"
#include "core.hpp"

namespace ps2rpc
{

    class MainWindow : public QWidget
    {
        Q_OBJECT

    public:
        MainWindow();

        bool isPresenceEnabled() const;
        bool isTrackerRunning() const;

        void saveConfig();
        void loadConfig();

    public slots:
        void startTracking(const CharacterData &character);
        void stopTracking();

    private Q_SLOTS:
        void onCharacterChanged(int index);
        void onEventPayloadReceived();
        void onLastSeenTimerExpired();

    private:
        void openCharacterManager(const QList<CharacterData> &characters = {});
        void updateEventLatency();
        void updateEventFrequency();
        void updateLastSeenLabels();

        QString getProjectLink() const;
        void setEventFrequency(double events_per_second);
        void setEventLatency(int latency_ms);
        void setLastPayload(bool never);
        void setLastPayload(QDateTime timestamp);
        void setLastPresence(bool never);
        void setLastPresence(QDateTime timestamp);
        void setStatus(const QString &status);
        void setTrackingEnabled(bool enabled);

        void resetCharacterComboBox();
        void setupUi();

        QScopedPointer<RichPresenceApp> app_;
        QScopedPointer<QTimer> last_seen_timer_;

        // GUI elements
        QComboBox *characters_combo_box_;
        QLabel *status_;
        QLabel *latency_;
        QLabel *event_frequency_;
        QLabel *payload_ago_;
        QLabel *presence_ago_;
        QCheckBox *auto_track_;
        QCheckBox *start_with_windows_;
        QCheckBox *minimise_to_tray_;
        QPushButton *enable_button_;
        QPushButton *minimise_button_;
    };

} // namespace ps2rpc

#endif // PS2RPC_GUI_MAINWINDOW_HPP
