// Copyright 2022 Leonhard S.

#include "gui/main-window.hpp"

#include <QtGui/QIcon>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

namespace ps2rpc
{

    MainWindow::MainWindow()
        : QWidget(nullptr)
    {
        // Configure the main window
        setFixedWidth(280);
        setFixedHeight(380);
        // Create GUI elements
        setupUi();
        // Disable maximise button on Windows
#ifdef Q_OS_WIN
        setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
#endif // Q_OS_WIN
    }

    void MainWindow::setupUi()
    {

        auto layout = new QVBoxLayout(this);

        // Characters label and combo box

        auto characters_layout = new QHBoxLayout();
        layout->addLayout(characters_layout);
        auto character_label = new QLabel("Character:", this);
        characters_layout->addWidget(character_label);
        characters_combo_box_ = new QComboBox(this);
        characters_layout->addWidget(characters_combo_box_);
        characters_layout->setStretch(0, 1);
        characters_layout->setStretch(1, 2);

        // DEBUG
        characters_combo_box_->addItem("Auroram");
        characters_combo_box_->addItem("Boram");
        characters_combo_box_->addItem("Celestis");
        characters_combo_box_->addItem("Doram");
        characters_combo_box_->addItem("Eiram");
        characters_combo_box_->insertSeparator(99); // Append
        characters_combo_box_->addItem("Manager characters…");

        // Status label

        layout->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Minimum,
                                              QSizePolicy::Expanding));
        status_ = new QLabel("Status: Waiting for login", this);
        layout->addWidget(status_);
        layout->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Minimum,
                                              QSizePolicy::Expanding));

        // Statistics group

        auto statistics_group = new QGroupBox("Statistics", this);
        layout->addWidget(statistics_group);
        auto statistics_layout = new QGridLayout(statistics_group);

        auto latency_label = new QLabel("Event stream latency:", this);
        statistics_layout->addWidget(latency_label, 0, 0);
        latency_ = new QLabel("51 ms", this);
        statistics_layout->addWidget(latency_, 0, 1);

        auto frequency_label = new QLabel("Events per second:", this);
        statistics_layout->addWidget(frequency_label, 1, 0);
        event_frequency_ = new QLabel("5.1", this);
        statistics_layout->addWidget(event_frequency_, 1, 1);

        auto payload_ago_label = new QLabel("Last event payload:", this);
        statistics_layout->addWidget(payload_ago_label, 2, 0);
        payload_ago_ = new QLabel("a few seconds ago", this);
        statistics_layout->addWidget(payload_ago_, 2, 1);

        auto presence_ago_label = new QLabel("Last presence update:", this);
        statistics_layout->addWidget(presence_ago_label, 3, 0);
        presence_ago_ = new QLabel("one minute ago", this);
        statistics_layout->addWidget(presence_ago_, 3, 1);

        statistics_layout->setColumnMinimumWidth(0, 120);

        // Settings group

        auto settings_group = new QGroupBox("Settings", this);
        layout->addWidget(settings_group);
        auto settings_layout = new QVBoxLayout(settings_group);

        auto_track_ = new QCheckBox(
            "Automatically track characters on login", this);
        settings_layout->addWidget(auto_track_);

        start_with_windows_ = new QCheckBox("Start with Windows", this);
        settings_layout->addWidget(start_with_windows_);

        minimise_to_tray_ = new QCheckBox("Minimise to system tray", this);
        settings_layout->addWidget(minimise_to_tray_);

        // Footer

        layout->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Minimum,
                                              QSizePolicy::Expanding));

        auto footer_labels_layout = new QHBoxLayout();
        layout->addLayout(footer_labels_layout);

        auto appinfo_label = new QLabel("PS2 Rich Presence v0.2", this);
        footer_labels_layout->addWidget(appinfo_label);
        auto repo_label = new QLabel("[Project Repository](#)", this);
        repo_label->setTextFormat(Qt::TextFormat::MarkdownText);
        repo_label->setOpenExternalLinks(true);
        repo_label->setAlignment(Qt::AlignRight);
        footer_labels_layout->addWidget(repo_label);

        auto footer_buttons_layout = new QHBoxLayout();
        layout->addLayout(footer_buttons_layout);

        connect_button_ = new QPushButton("Connect", this);
        footer_buttons_layout->addWidget(connect_button_);
        minimise_button_ = new QPushButton("Minimize", this);
        minimise_button_->setChecked(true);
        footer_buttons_layout->addWidget(minimise_button_);
    }

} // namespace ps2rpc
