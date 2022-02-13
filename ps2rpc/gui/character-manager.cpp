// Copyright 2022 Leonhard S.

#include "gui/character-manager.hpp"

#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QRegularExpression>
#include <QtCore/QScopedPointer>
#include <QtCore/QUrl>
#include <QtCore/QUrlQuery>
#include <QtGui/QRegularExpressionValidator>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

#include "appdata/serviceid.hpp"

namespace ps2rpc
{

    CharacterManager::CharacterManager(QWidget *parent)
        : QDialog(parent)
    {
        // Configure the modal dialog
        setWindowTitle(tr("Manage Characters"));
        setFixedWidth(280);
        setFixedHeight(320);
        setModal(true);
        // Create GUI elements
        setupUi();

        // Disable the remove button while no character is selected
        button_remove_->setEnabled(false);

        // Signals
        QObject::connect(button_add_, &QPushButton::clicked,
                         this, &CharacterManager::onAddButtonClicked);
        QObject::connect(button_remove_, &QPushButton::clicked,
                         this, &CharacterManager::onRemoveButtonClicked);
        QObject::connect(button_close_, &QPushButton::clicked,
                         this, &CharacterManager::accept);
        QObject::connect(list_, &QListWidget::itemSelectionChanged,
                         this, &CharacterManager::onCharacterSelected);
    }

    void CharacterManager::addCharacter(const QString &character)
    {
        if (list_ != nullptr)
        {
            list_->addItem(character);
            emit characterAdded(list_->count() - 1, character);
        }
    }

    void CharacterManager::onAddButtonClicked()
    {
        QScopedPointer<QDialog> dialog(createCharacterNameInputDialog());
        if (dialog->exec() == QDialog::DialogCode::Rejected)
        {
            return;
        }
        auto name = dialog->findChild<QLineEdit *>()->text();
        // Ignore empty string
        if (name.isEmpty())
        {
            return;
        }
        // Ignore duplicates
        for (int i = 0; i < list_->count(); ++i)
        {
            if (list_->item(i)->text() == name)
            {
                QMessageBox::information(this,
                                         tr("Character Manager"),
                                         tr("Character already exists."),
                                         QMessageBox::Ok);
                return;
            }
        }
        // Validate that this character exists
        auto manager = new QNetworkAccessManager();
        auto reply = manager->get(QNetworkRequest(getCharacterInfoUrl(name)));
        QObject::connect(reply, &QNetworkReply::finished,
                         [this, name, manager, reply]()
                         {
                             if (reply->error() != QNetworkReply::NetworkError::NoError)
                             {
                                 // TODO: Unable to verify that char exists
                                 return;
                             }
                             // Validate payload
                             auto data = reply->readAll();
                             auto json = QJsonDocument::fromJson(data);
                             if (json["character_list"].toArray().count() == 0)
                             {

                                 QMessageBox::information(this,
                                                          tr("Character Manager"),
                                                          tr("Character does not exist."),
                                                          QMessageBox::Ok);
                                 return;
                             }
                             // TODO: Store character info
                             list_->addItem(name);
                             emit characterAdded(list_->count() - 1, name);
                             delete manager;
                         });
    }

    void CharacterManager::onRemoveButtonClicked()
    {
        if (list_->currentRow() == -1 || list_->count() == 0)
        {
            // This button should already be disabled; this is just fallback
            button_remove_->setEnabled(false);
            return;
        }
        // Ask for confirmation
        auto msg = tr("Are you sure you want to remove the character '%1'?")
                       .arg(list_->currentItem()->text());
        if (QMessageBox::question(
                this, tr("Remove Character"),
                msg, QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        {
            return;
        }
        // User has confirmed their intentions, remove the character
        list_->takeItem(list_->currentRow());
    }

    void CharacterManager::onCharacterSelected()
    {
        // Enable the remove button if a character is selected
        button_remove_->setEnabled(list_->currentRow() != -1);
    }

    QUrl CharacterManager::getCharacterInfoUrl(const QString &character) const
    {
        auto service_id = QString(SERVICE_ID);
        QUrl url;
        url.setScheme("https");
        url.setHost("census.daybreakgames.com");
        url.setPath("/" + service_id + "/get/ps2:v2/character");
        QUrlQuery query;
        query.addQueryItem("name.first_lower", character.toLower());
        query.addQueryItem("c:join", "characters_world^show:world_id^inject_at:world");
        query.addQueryItem("c:show", "character_id,name.first,faction_id,profile_id");
        url.setQuery(query);
        return url;
    }

    QDialog *CharacterManager::createCharacterNameInputDialog()
    {
        auto dialog = new QDialog(this);
        dialog->setWindowTitle(tr("Add Character"));
        dialog->setMinimumSize(120, 80);
        auto layout = new QVBoxLayout(dialog);

        auto name_input = new QLineEdit(dialog);
        layout->addWidget(name_input);
        name_input->setPlaceholderText(tr("Enter character name"));
        name_input->setInputMethodHints(Qt::ImhNoPredictiveText);
        name_input->setValidator(
            new QRegularExpressionValidator(
                // Only accept between 3 and 32 alphanumerical characters
                QRegularExpression("[a-zA-Z0-9]{3,32}"), name_input));

        auto button_layout = new QHBoxLayout();
        layout->addLayout(button_layout);
        button_layout->addSpacerItem(new QSpacerItem(40, 20,
                                                     QSizePolicy::Expanding,
                                                     QSizePolicy::Minimum));
        auto button_ok = new QPushButton(tr("OK"), dialog);
        button_layout->addWidget(button_ok);
        button_ok->setDefault(true);
        auto button_cancel = new QPushButton(tr("Cancel"), dialog);
        button_layout->addWidget(button_cancel);

        QObject::connect(name_input, &QLineEdit::returnPressed,
                         dialog, &QDialog::accept);
        QObject::connect(button_ok, &QPushButton::clicked,
                         dialog, &QDialog::accept);
        QObject::connect(button_cancel, &QPushButton::clicked,
                         dialog, &QDialog::reject);
        return dialog;
    }

    void CharacterManager::setupUi()
    {
        auto layout = new QVBoxLayout(this);

        // Instruction text
        auto instructions = new QLabel(
            tr("Add characters below using their ingame name.\n"
               "\n"
               "You can drag character names to reorder them. Names higher in "
               "the list will be prioritised when auto-tracking is enabled in "
               "settings."),
            this);
        layout->addWidget(instructions);
        instructions->setWordWrap(true);

        // Characters list
        list_ = new QListWidget(this);
        layout->addWidget(list_);
        list_->setAlternatingRowColors(true);
        list_->setDragEnabled(true);
        list_->setDragDropMode(QAbstractItemView::InternalMove);

        // Buttons
        auto button_layout = new QHBoxLayout();
        layout->addLayout(button_layout);

        button_add_ = new QPushButton(tr("Add"), this);
        button_layout->addWidget(button_add_);
        button_add_->setDefault(true);

        button_remove_ = new QPushButton(tr("Remove"), this);
        button_layout->addWidget(button_remove_);

        button_close_ = new QPushButton(tr("Confirm"), this);
        button_layout->addWidget(button_close_);
    }

} // namespace ps2rpc
