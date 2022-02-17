// Copyright 2022 Leonhard S.

#include "gui/character-manager.hpp"

#include <QtCore/QJsonObject>
#include <QtCore/QRegularExpression>
#include <QtCore/QScopedPointer>
#include <QtCore/QString>
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

#include <string>

#include "arx.hpp"
#include "ps2.hpp"

#include "appdata/serviceid.hpp"
#include "utils.hpp"

namespace ps2rpc
{

    CharacterManager::CharacterManager(QWidget *parent)
        : QDialog{parent}, manager_{new QNetworkAccessManager()}
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
        auto reply = manager_->get(QNetworkRequest(getCharacterInfoUrl(name)));
        QObject::connect(reply, &QNetworkReply::finished,
                         this, &CharacterManager::onCharacterInfoReceived);
        // Create temp character entry to show while waiting for reply
        auto item = new QListWidgetItem(tr("Loading '%1'…").arg(name));
        item->setData(Qt::UserRole, QVariant::fromValue(reply));
        list_->addItem(item);
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

    void CharacterManager::onCharacterInfoReceived()
    {
        // Get reply (cast to ScopedPointer to ensure it is deleted when this
        // function returns)
        QScopedPointer<QNetworkReply> reply(
            qobject_cast<QNetworkReply *>(sender()));
        // Remove temporary list entry
        for (int i = 0; i < list_->count(); i++)
        {
            if (list_->item(i)->data(Qt::UserRole).value<QNetworkReply *>() ==
                reply.data())
            {
                list_->takeItem(i);
                break;
            }
        }
        // Check for errors
        if (reply->error() != QNetworkReply::NetworkError::NoError)
        {
            QMessageBox::critical(this,
                                  tr("Character Manager"),
                                  tr("Failed to retrieve character info."),
                                  QMessageBox::Ok);
            return;
        }
        // Validate payload
        const QString collection = "character";
        auto payload = getJsonPayload(*reply);
        if (!arx::isPayloadValid(collection, payload))
        {
            QMessageBox::critical(this,
                                  tr("Character Manager"),
                                  tr("Invalid character info payload."),
                                  QMessageBox::Ok);
            return;
        }
        if (arx::isPayloadEmpty(collection, payload))
        {
            QMessageBox::critical(this,
                                  tr("Character Manager"),
                                  tr("Character does not exist."),
                                  QMessageBox::Ok);
            return;
        }
        auto data = arx::payloadResultAsObject(collection, payload);
        // Add character to list
        auto name = data["name"].toObject()["first"].toString();
        ps2::CharacterId id = data["character_id"].toString().toULongLong();
        auto profile_id = data["profile_id"].toString().toInt();
        auto faction_id = data["faction_id"].toString().toInt();
        auto world_data = data["world"].toObject();
        auto world_id = world_data["world_id"].toString().toInt();
        // Create character entry
        auto item = new QListWidgetItem(name);
        // TODO: Load character data into Qt::UserRole
        list_->addItem(item);
    }

    QUrl CharacterManager::getCharacterInfoUrl(const QString &character) const
    {
        auto name = character.toLower().toStdString();
        // Create API query
        arx::Query query("character", SERVICE_ID);
        query.addTerm(arx::SearchTerm("name.first_lower", name));
        auto join = arx::JoinData("characters_world");
        join.show.push_back("world_id");
        join.inject_at = "world";
        query.addJoin(join);
        query.setShow(
            {"character_id", "name.first", "faction_id", "profile_id"});
        // Build QUrl object
        return qUrlFromArxQuery(query);
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
