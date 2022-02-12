// Copyright 2022 Leonhard S.

#include "gui/character-manager.hpp"

#include <QtCore/QScopedPointer>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
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

        // Signals
        QObject::connect(button_close_, &QPushButton::clicked,
                         this, &CharacterManager::accept);
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
