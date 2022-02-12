// Copyright 2022 Leonhard S.

#include "gui/character-manager.hpp"

#include <QtCore/QRegularExpression>
#include <QtCore/QScopedPointer>
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

    void CharacterManager::onAddButtonClicked()
    {
        QScopedPointer<QDialog> dialog(createCharacterNameInputDialog());
        if (dialog->exec() == QDialog::DialogCode::Rejected)
        {
            return;
        }
        // TODO: Validate that this character exists
        auto name = dialog->findChild<QLineEdit *>()->text();
        list_->addItem(name);
        emit characterAdded(list_->count() - 1, name);
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

        button_add_ = new QPushButton(tr("Add new"), this);
        button_layout->addWidget(button_add_);
        button_add_->setDefault(true);

        button_remove_ = new QPushButton(tr("Remove"), this);
        button_layout->addWidget(button_remove_);

        button_close_ = new QPushButton(tr("Close"), this);
        button_layout->addWidget(button_close_);
    }

} // namespace ps2rpc
