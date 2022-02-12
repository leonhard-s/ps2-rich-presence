// Copyright 2022 Leonhard S.

#include "gui/main-window.hpp"

#include <QtCore/QCoreApplication>
#include <QtCore/QDateTime>
#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

#include "gui/character-manager.hpp"
#include "gui/timeago.hpp"

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

        QObject::connect(characters_combo_box_,
                         &QComboBox::currentIndexChanged,
                         this,
                         &MainWindow::onCharacterChanged);
        QObject::connect(minimise_button_,
                         &QPushButton::clicked,
                         this,
                         &MainWindow::showMinimized);
    }

    void MainWindow::onCharacterChanged(int index)
    {
        if (index < 0)
        {
            return;
    }
        if (characters_combo_box_->count() <= 2)
        {
            resetCharacterComboBox();
        }
        if (index == characters_combo_box_->count() - 1)
        {
            // Get existing characters
            QStringList characters;
            for (int i = 0; i < characters_combo_box_->count() - 2; ++i)
            {
                characters.append(characters_combo_box_->itemText(i));
            }
            openCharacterManager(characters);
        }
        // TODO: Handle existing being selected
    }

    void MainWindow::openCharacterManager(const QStringList &characters)
    {
        auto dialog = new CharacterManager(this);
        // Add existing characters
        for (const auto &character : characters)
        {
            dialog->addCharacter(character);
        }
        // Show the dialog
        if (dialog->exec() == QDialog::DialogCode::Accepted)
        {
            // Update characters dropdown
            auto list = dialog->findChild<QListWidget *>();
            // Reset the dropdown
            resetCharacterComboBox();
            // Add characters from manager
            for (int i = 0; i < list->count(); ++i)
            {
                auto name = list->item(i)->text();
                // Offset of 2 because of the separator and "Manage" items
                auto index = characters_combo_box_->count() - 2;
                characters_combo_box_->insertItem(index, name);
            }
            // Select the topmost character if any were added
            if (list->count() > 0)
            {
                characters_combo_box_->setCurrentIndex(0);
            }
        }
        else
        {
            // Dialog was cancelled - if we had characters, select the first
            if (characters_combo_box_->count() > 2)
            {
                characters_combo_box_->setCurrentIndex(0);
            }
            else
            {
                // Otherwise, select nothing
                characters_combo_box_->setCurrentIndex(-1);
            }
        }
    }

    QString MainWindow::getProjectLink() const
    {
        return "https://github.com/leonhard-s/ps2-rich-presence/";
    }

    void MainWindow::setEventFrequency(double events_per_second)
    {
        if (events_per_second < 0.0)
        {
            event_frequency_->setText(tr("Unknown"));
        }
        else
        {
            event_frequency_->setText(QString::number(events_per_second, 'f', 2));
        }
    }

    void MainWindow::setEventLatency(int latency_ms)
    {
        if (latency_ms < 0)
        {
            latency_->setText(tr("Unknown"));
        }
        else
        {
            latency_->setText(tr("%1 ms").arg(latency_ms));
        }
    }

    void MainWindow::setLastPayload(bool never)
    {
        payload_ago_->setText(tr("Never"));
    }

    void MainWindow::setLastPayload(QDateTime timestamp)
    {
        payload_ago_->setText(getTimeAgo(timestamp));
    }

    void MainWindow::setLastPresence(bool never)
    {
        presence_ago_->setText(tr("Never"));
    }

    void MainWindow::setLastPresence(QDateTime timestamp)
    {
        presence_ago_->setText(getTimeAgo(timestamp));
    }

    void MainWindow::setStatus(const QString &status)
    {
        status_->setText(tr("Status: ") + status);
    }

    void MainWindow::setTrackingEnabled(bool enabled)
    {
        enable_button_->setText(enabled ? tr("Disable") : tr("Enable"));
    }

    void MainWindow::resetCharacterComboBox()
    {
        if (characters_combo_box_ == nullptr)
        {
            return;
        }
        characters_combo_box_->clear();
        characters_combo_box_->insertSeparator(0);
        characters_combo_box_->addItem(tr("Manage characters…"), QVariant(0));
        characters_combo_box_->setPlaceholderText(tr("Select a character"));
        characters_combo_box_->setCurrentIndex(-1);
    }

    void MainWindow::setupUi()
    {

        auto layout = new QVBoxLayout(this);

        // Characters label and combo box

        auto characters_layout = new QHBoxLayout();
        layout->addLayout(characters_layout);
        auto character_label = new QLabel(tr("Character:"), this);
        characters_layout->addWidget(character_label);
        characters_combo_box_ = new QComboBox(this);
        characters_layout->addWidget(characters_combo_box_);
        resetCharacterComboBox();
        characters_layout->setStretch(0, 1);
        characters_layout->setStretch(1, 2);

        // Status label

        layout->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Minimum,
                                              QSizePolicy::Expanding));
        status_ = new QLabel("", this);
        setStatus("");
        layout->addWidget(status_);
        layout->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Minimum,
                                              QSizePolicy::Expanding));

        // Statistics group

        auto statistics_group = new QGroupBox(tr("Statistics"), this);
        layout->addWidget(statistics_group);
        auto statistics_layout = new QGridLayout(statistics_group);

        auto latency_label = new QLabel(
            tr("Event stream latency") + ":", this);
        statistics_layout->addWidget(latency_label, 0, 0);
        latency_ = new QLabel("", this);
        setEventLatency(-1); // "Unknown"
        statistics_layout->addWidget(latency_, 0, 1);

        auto frequency_label = new QLabel(
            tr("Events per second") + ":", this);
        statistics_layout->addWidget(frequency_label, 1, 0);
        event_frequency_ = new QLabel("", this);
        setEventFrequency(-1.0); // "Unknown"
        statistics_layout->addWidget(event_frequency_, 1, 1);

        auto payload_ago_label = new QLabel(
            tr("Last event payload") + ":", this);
        statistics_layout->addWidget(payload_ago_label, 2, 0);
        payload_ago_ = new QLabel("", this);
        setLastPayload(false); // "Never"
        statistics_layout->addWidget(payload_ago_, 2, 1);

        auto presence_ago_label = new QLabel(
            tr("Last presence update") + ":", this);
        statistics_layout->addWidget(presence_ago_label, 3, 0);
        presence_ago_ = new QLabel("", this);
        setLastPresence(false); // "Never"
        statistics_layout->addWidget(presence_ago_, 3, 1);

        statistics_layout->setColumnMinimumWidth(0, 120);

        // Settings group

        auto settings_group = new QGroupBox(tr("Settings"), this);
        layout->addWidget(settings_group);
        auto settings_layout = new QVBoxLayout(settings_group);

        auto_track_ = new QCheckBox(
            tr("Automatically track characters on login"), this);
        settings_layout->addWidget(auto_track_);

        start_with_windows_ = new QCheckBox(tr("Start with Windows"), this);
        settings_layout->addWidget(start_with_windows_);

        minimise_to_tray_ = new QCheckBox(tr("Minimise to system tray"), this);
        settings_layout->addWidget(minimise_to_tray_);

        // Footer

        layout->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Minimum,
                                              QSizePolicy::Expanding));

        auto footer_labels_layout = new QHBoxLayout();
        layout->addLayout(footer_labels_layout);

        auto appinfo_string = QCoreApplication::applicationName() + " v" +
                              QCoreApplication::applicationVersion();
        auto appinfo_label = new QLabel(appinfo_string, this);
        footer_labels_layout->addWidget(appinfo_label);

        auto link = getProjectLink();
        auto title = tr("Project Repository");
        auto project_text = QString("[") + title + "]" + "(" + link + ")";
        auto repo_label = new QLabel(project_text, this);
        footer_labels_layout->addWidget(repo_label);
        repo_label->setTextFormat(Qt::TextFormat::MarkdownText);
        repo_label->setOpenExternalLinks(true);
        repo_label->setAlignment(Qt::AlignRight);

        auto footer_buttons_layout = new QHBoxLayout();
        layout->addLayout(footer_buttons_layout);

        enable_button_ = new QPushButton("", this);
        setTrackingEnabled(true);
        footer_buttons_layout->addWidget(enable_button_);
        minimise_button_ = new QPushButton(tr("Minimize"), this);
        minimise_button_->setChecked(true);
        footer_buttons_layout->addWidget(minimise_button_);
    }

} // namespace ps2rpc
