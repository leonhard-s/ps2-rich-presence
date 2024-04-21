// Copyright 2022 Leonhard S.

#include "gui/character-manager.hpp"

#include <QtCore/QJsonDocument>
#include <QtCore/QRegularExpression>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QUrlQuery>
#include <QtGui/QRegularExpressionValidator>
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
#include <adopt_pointer.h>
#include <moc_macros.h>

#include "api/rest_client.h"
#include "appdata/service-id.hpp"
#include "game/character-info.hpp"
#include "utils.hpp"

namespace PresenceApp {

CharacterManager::CharacterManager(QWidget* parent
)
    : QDialog{ parent }
    , list_{ nullptr }
    , button_add_{ nullptr }
    , button_remove_{ nullptr }
    , button_close_{ nullptr }
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

void CharacterManager::addCharacter(const CharacterData& character) {
    if (list_ != nullptr) {
        auto* item = adopt_pointer(new QListWidgetItem(character.name_));
        item->setData(Qt::UserRole, QVariant::fromValue(character));
        list_->addItem(item);
    }
}

void CharacterManager::onAddButtonClicked() {
    auto dialog = std::unique_ptr<QDialog>(createCharacterNameInputDialog());
    if (dialog->exec() == QDialog::DialogCode::Rejected) {
        return;
    }
    auto name = dialog->findChild<QLineEdit*>()->text();
    // Ignore empty string
    if (name.isEmpty()) {
        return;
    }
    // Ignore duplicates
    for (int i = 0; i < list_->count(); ++i) {
        if (list_->item(i)->text() == name) {
            QMessageBox::information(
                this,
                tr("Character Manager"),
                tr("Character already exists."),
                QMessageBox::Ok);
            return;
        }
    }
    // Validate that this character exists
    auto* const client = adopt_pointer(new PresenceLib::AsyncRestClient(this));
    QObject::connect(client, &PresenceLib::AsyncRestClient::requestFinished,
        this, &CharacterManager::onCharacterInfoReceived);
    QObject::connect(client, &PresenceLib::AsyncRestClient::requestFinished,
        client, &QObject::deleteLater);
    client->request(getCharacterInfoUrl(name));
    is_loading_ = true;
    setEnabled(false);
}

void CharacterManager::onRemoveButtonClicked() {
    if (list_->currentRow() == -1 || list_->count() == 0) {
        // This button should already be disabled; this is just fallback
        button_remove_->setEnabled(false);
        return;
    }
    // Ask for confirmation
    auto msg = tr("Are you sure you want to remove the character '%1'?")
        .arg(list_->currentItem()->text());
    if (QMessageBox::question(
        this, tr("Remove Character"),
        msg, QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
        return;
    }
    // User has confirmed their intentions, remove the character
    list_->takeItem(list_->currentRow());
}

void CharacterManager::onCharacterSelected() {
    // Enable the remove button if a character is selected
    button_remove_->setEnabled(list_->currentRow() != -1);
}

void CharacterManager::onCharacterInfoReceived(const QJsonDocument& response) {
    if (is_loading_) {
        setEnabled(true);
        is_loading_ = false;
    }

    // Check for errors
    if (response.isEmpty()) {
        QMessageBox::critical(this,
            tr("Character Manager"),
            tr("Failed to retrieve character info."),
            QMessageBox::Ok);
        return;
    }
    // Validate payload
    const std::string collection = "character";
    auto payload = qtJsonToArxJson(response);
    if (arx::validatePayload(collection, payload) != 0) {
        QMessageBox::critical(this,
            tr("Character Manager"),
            tr("Invalid character info payload."),
            QMessageBox::Ok);
        return;
    }
    if (arx::isPayloadEmpty(collection, payload)) {
        QMessageBox::critical(this,
            tr("Character Manager"),
            tr("Character does not exist."),
            QMessageBox::Ok);
        return;
    }
    // HACK: Parse character data
    CharacterInfo temp;
    temp.handleCharacterInfoPayload(payload);
    const CharacterData info{ temp.getId(), temp.getName(), temp.getFaction(),
                       temp.getClass(), temp.getServer() };
    // Create character entry
    auto* item = adopt_pointer(new QListWidgetItem(info.name_));
    item->setData(Qt::UserRole, QVariant::fromValue(info));
    list_->addItem(item);
}

QString CharacterManager::getCharacterInfoUrl(const QString& character) {
    auto name = character.toLower().toStdString();
    // Create API query
    arx::Query query("character", SERVICE_ID);
    query.addTerm(arx::SearchTerm("name.first_lower", name));
    auto join = arx::JoinData("characters_world");
    join.show_.emplace_back("world_id");
    join.inject_at_ = "world";
    query.addJoin(join);
    query.setShow({ "character_id", "name.first", "faction_id", "profile_id" });
    // Build query string
    return qStringFromArxQuery(query);
}

CharacterData CharacterManager::parseCharacterPayload(
    const arx::json_t& payload) {
    // Set default/fallback values
    QString name = "N/A";
    arx::character_id_t id = 0;
    arx::faction_id_t faction_id = 0;
    arx::profile_id_t profile_id = 0;
    arx::world_id_t world_id = 0;
    ps2::Faction faction = ps2::Faction::NS;
    ps2::Class cls = ps2::Class::LightAssault;
    ps2::Server server = ps2::Server::Connery;
    // Get name object
    auto name_obj = payload["name"];
    if (name_obj.is_object()) {
        // Get name.first value
        auto name_val = name_obj["first"];
        if (name_val.is_string()) {
            name = QString::fromStdString(name_val.get<std::string>());
        }
        else {
            qWarning() << "Invalid type: \"name.first\" must be a string";
        }
    }
    else {
        qWarning() << "Invalid type: key \"name\" must be an object";
    }
    // Get character ID
    const auto& id_val = payload["character_id"];
    if (id_val.is_string()) {
        id = std::stoull(id_val.get<std::string>());
    }
    else {
        qWarning() << "Invalid type: \"character_id\" must be a string";
    }
    // Get faction
    const auto& faction_val = payload["faction_id"];
    if (faction_val.is_number_integer()) {
        faction_id = faction_val.get<arx::faction_id_t>();
    }
    else {
        qWarning() << "Invalid type: \"faction_id\" must be a number";
    }
    // Get profile
    const auto& profile_val = payload["profile_id"];
    if (profile_val.is_number_integer()) {
        profile_id = profile_val.get<arx::profile_id_t>();
    }
    else {
        qWarning() << "Invalid type: \"profile_id\" must be a number";
    }
    // Get world
    const auto& world_val = payload["world_id"];
    if (world_val.is_number_integer()) {
        world_id = world_val.get<arx::world_id_t>();
    }
    else {
        qWarning() << "Invalid type: \"world_id\" must be a number";
    }
    // Convert IDs to enum values
    if (ps2::faction_from_faction_id(faction_id, &faction) != 0) {
        qWarning() << "Unable to convert faction ID:" << faction_id;
    }
    if (ps2::class_from_profile_id(profile_id, &cls) != 0) {
        qWarning() << "Unable to create class from profile ID:"
            << profile_id;
    }
    if (ps2::server_from_world_id(world_id, &server) != 0) {
        qWarning() << "Unable to create server from world ID:" << world_id;
    }
    // Create character info
    return { id, name, faction, cls, server };
}

QDialog* CharacterManager::createCharacterNameInputDialog() {
    auto* dialog = adopt_pointer(new QDialog(this));
    dialog->setWindowTitle(tr("Add Character"));
    dialog->setMinimumSize(120, 80);
    auto* layout = adopt_pointer(new QVBoxLayout(dialog));

    auto* name_input = adopt_pointer(new QLineEdit(dialog));
    layout->addWidget(name_input);
    name_input->setPlaceholderText(tr("Enter character name"));
    name_input->setInputMethodHints(Qt::ImhNoPredictiveText);
    name_input->setValidator(
        adopt_pointer(new QRegularExpressionValidator(
            // Only accept between 3 and 32 alphanumerical characters
            QRegularExpression("[a-zA-Z0-9]{3,32}"), name_input)));

    auto* button_layout = adopt_pointer(new QHBoxLayout());
    layout->addLayout(button_layout);
    button_layout->addSpacerItem(adopt_pointer(
        new QSpacerItem(40, 20,
            QSizePolicy::Expanding,
            QSizePolicy::Minimum)));
    auto* button_ok = new QPushButton(tr("OK"), dialog);
    button_layout->addWidget(button_ok);
    button_ok->setDefault(true);
    auto* button_cancel = new QPushButton(tr("Cancel"), dialog);
    button_layout->addWidget(button_cancel);

    QObject::connect(name_input, &QLineEdit::returnPressed,
        dialog, &QDialog::accept);
    QObject::connect(button_ok, &QPushButton::clicked,
        dialog, &QDialog::accept);
    QObject::connect(button_cancel, &QPushButton::clicked,
        dialog, &QDialog::reject);
    return dialog;
}

void CharacterManager::setupUi() {
    auto* layout = adopt_pointer(new QVBoxLayout(this));

    // Instruction text
    auto* instructions = new QLabel(
        tr("Add characters below using their ingame name.\n"
            "\n"
            "You can drag character names to reorder them. Names higher in "
            "the list will be prioritised when auto-tracking is enabled in "
            "settings."),
        this);
    layout->addWidget(instructions);
    instructions->setWordWrap(true);

    // Characters list
    list_ = adopt_pointer(new QListWidget(this));
    layout->addWidget(list_);
    list_->setAlternatingRowColors(true);
    list_->setDragEnabled(true);
    list_->setDragDropMode(QAbstractItemView::InternalMove);

    // Buttons
    auto* button_layout = adopt_pointer(new QHBoxLayout());
    layout->addLayout(button_layout);

    button_add_ = adopt_pointer(new QPushButton(tr("Add"), this));
    button_layout->addWidget(button_add_);
    button_add_->setDefault(true);

    button_remove_ = adopt_pointer(new QPushButton(tr("Remove"), this));
    button_layout->addWidget(button_remove_);

    button_close_ = adopt_pointer(new QPushButton(tr("Confirm"), this));
    button_layout->addWidget(button_close_);
}

} // namespace PresenceApp

PUSH_MOC_WARNINGS_FILTER;
#include "moc_character-manager.cpp"
POP_MOC_WARNINGS_FILTER;
