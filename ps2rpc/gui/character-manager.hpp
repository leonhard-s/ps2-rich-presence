// Copyright 2022 Leonhard S.

#ifndef PS2RPC_GUI_CHARACTER_MANAGER_HPP
#define PS2RPC_GUI_CHARACTER_MANAGER_HPP

#include <QtCore/QObject>
#include <QtWidgets/QDialog>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>

namespace ps2rpc
{

    class CharacterManager : public QDialog
    {
        Q_OBJECT

    public:
        explicit CharacterManager(QWidget *parent = nullptr);

    private:
        void setupUi();

        QListWidget *list_;
        QPushButton *button_add_;
        QPushButton *button_remove_;
        QPushButton *button_close_;
    };

} // namespace ps2pc

#endif // PS2RPC_GUI_CHARACTER_MANAGER_HPP
