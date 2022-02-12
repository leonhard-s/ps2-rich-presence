// Copyright 2022 Leonhard S.

#ifndef PS2RPC_GUI_MAINWINDOW_HPP
#define PS2RPC_GUI_MAINWINDOW_HPP

#include <QtCore/QObject>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

namespace ps2rpc
{

    class MainWindow : public QWidget
    {
        Q_OBJECT

    public:
        MainWindow();

    private:
        QString getProjectLink() const;
        void setEventFrequency(double events_per_second);
        void setEventLatency(int latency_ms);
        void setLastPayload(bool never);
        void setLastPayload(QDateTime timestamp);
        void setLastPresence(bool never);
        void setLastPresence(QDateTime timestamp);
        void setStatus(const QString &status);

        void resetCharacterComboBox();
        void setupUi();

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
        QPushButton *connect_button_;
        QPushButton *minimise_button_;
    };

} // namespace ps2rpc

#endif // PS2RPC_GUI_MAINWINDOW_HPP
