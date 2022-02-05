"""UI and controller definition for the main window widget."""

from typing import Optional

from PyQt6.QtWidgets import (QGridLayout, QGroupBox, QInputDialog, QLabel,
                             QListWidget, QHBoxLayout, QListWidgetItem,
                             QMessageBox, QPushButton, QWidget, QVBoxLayout)
from PyQt6.QtCore import Qt

from ._about import AboutDialog
from .._qasync import connect, slot

__all__ = [
    'MainWindow',
]

_ALIGN_RIGHT = Qt.AlignmentFlag.AlignRight | Qt.AlignmentFlag.AlignVCenter


class _MainWindowUi(QWidget):
    """UI definition for the MainWindow class."""

    _REPO = 'https://github.com/leonhard-s/ps2-rich-presence'

    def __init__(self, parent: Optional[QWidget] = None,
                 flags: Qt.WindowType = Qt.WindowType.Widget) -> None:
        super().__init__(parent, flags)
        # Main column layout
        layout = QVBoxLayout()
        self.setLayout(layout)
        # Tracked characters
        tracked = QGroupBox('Tracked Characters')
        layout.addWidget(tracked)
        tracked.setFlat(True)
        tracked_layout = QVBoxLayout()
        tracked.setLayout(tracked_layout)
        self.tracked_characters = QListWidget()
        tracked_layout.addWidget(self.tracked_characters)
        self.tracked_characters.setAlternatingRowColors(True)
        tracked_controls = QHBoxLayout()
        tracked_layout.addLayout(tracked_controls)
        tracked_controls.addStretch()
        self.add_btn = QPushButton('Add')
        tracked_controls.addWidget(self.add_btn)
        self.remove_btn = QPushButton('Remove')
        tracked_controls.addWidget(self.remove_btn)
        # Activity information
        activity = QGroupBox('Activity Information')
        layout.addWidget(activity)
        activity.setFlat(True)
        activity_layout = QGridLayout()
        activity.setLayout(activity_layout)
        activity_layout.addWidget(QLabel('Tracked character'), 0, 0)
        self.tracked_char = QLabel('None')
        activity_layout.addWidget(self.tracked_char, 0, 1)
        self.tracked_char.setAlignment(_ALIGN_RIGHT)
        activity_layout.addWidget(QLabel('Event latency'), 1, 0)
        self.event_latency = QLabel('0 ms')
        activity_layout.addWidget(self.event_latency, 1, 1)
        self.event_latency.setAlignment(_ALIGN_RIGHT)
        activity_layout.addWidget(QLabel('Events per second'), 2, 0)
        self.events_per_second = QLabel('0')
        activity_layout.addWidget(self.events_per_second, 2, 1)
        self.events_per_second.setAlignment(_ALIGN_RIGHT)
        activity_layout.addWidget(QLabel('Most recent event'), 3, 0)
        self.most_recent_event = QLabel('None')
        activity_layout.addWidget(self.most_recent_event, 3, 1)
        self.most_recent_event.setAlignment(_ALIGN_RIGHT)
        activity_layout.addWidget(QLabel('Last status acquisition'), 4, 0)
        self.last_status_acquisition = QLabel('Never')
        activity_layout.addWidget(self.last_status_acquisition, 4, 1)
        self.last_status_acquisition.setAlignment(_ALIGN_RIGHT)
        activity_layout.addWidget(QLabel('Last status update'), 5, 0)
        self.last_status_update = QLabel('Never')
        activity_layout.addWidget(self.last_status_update, 5, 1)
        self.last_status_update.setAlignment(_ALIGN_RIGHT)
        # Footer
        footer = QHBoxLayout()
        layout.addLayout(footer)
        self.link_bug_report = QLabel(f'[Report a bug]({self._REPO}/issues)')
        self.link_bug_report.setOpenExternalLinks(True)
        footer.addWidget(self.link_bug_report)
        self.link_bug_report.setTextFormat(Qt.TextFormat.MarkdownText)
        self.link_bug_report.setOpenExternalLinks(True)
        self.link_github = QLabel(f'[GitHub]({self._REPO})')
        self.link_github.setOpenExternalLinks(True)
        footer.addWidget(self.link_github)
        self.link_github.setTextFormat(Qt.TextFormat.MarkdownText)
        self.link_github.setOpenExternalLinks(True)
        self.link_help = QLabel(f'[Help]({self._REPO}/wiki)')
        self.link_help.setOpenExternalLinks(True)
        footer.addWidget(self.link_help)
        self.link_help.setTextFormat(Qt.TextFormat.MarkdownText)
        self.link_help.setOpenExternalLinks(True)
        footer.addStretch()
        self.about_btn = QPushButton('About')
        footer.addWidget(self.about_btn)


class MainWindow(_MainWindowUi):
    """Main application window & UI implementation."""

    def __init__(self, parent: Optional[QWidget] = None,
                 flags: Qt.WindowType = Qt.WindowType.Widget) -> None:
        super().__init__(parent, flags)
        self.setWindowTitle('PS2 Rich Presence')
        self.resize(320, 420)
        self.setFixedSize(self.size())
        # Hook up signals
        connect(self.add_btn.clicked, self._add_character)
        connect(self.remove_btn.clicked, self._remove_character)
        connect(self.about_btn.clicked, self._show_about)

    @slot()
    def _add_character(self) -> None:
        """Input dialog for adding a character."""
        dlg = QInputDialog(self)
        dlg.setInputMode(QInputDialog.InputMode.TextInput)
        dlg.setInputMethodHints(Qt.InputMethodHint.ImhPreferLatin)
        dlg.setWindowTitle('Add character')
        dlg.setLabelText('Character to add')
        if not dlg.exec():
            return
        name = dlg.textValue().strip()
        if not name:
            return
        # Check character name length requirements
        if not 3 <= len(name) <= 32:
            QMessageBox.warning(self, 'Error', 'Character name must be '
                                'between 3 and 32 characters long')
            return
        # Check for invalid characters
        if not name.isalnum():
            QMessageBox.warning(
                self, 'Error', 'Character names must be alphanumeric')
        # Check for duplicate character name
        for index in range(self.tracked_characters.count()):
            item = self.tracked_characters.item(index)
            if item.text() == name:
                QMessageBox.warning(
                    self, 'Error', 'Character already added')
                return

        # TODO: Validate name against the API
        self.tracked_characters.addItem(QListWidgetItem(name))

    @slot()
    def _remove_character(self) -> None:
        """Input dialog for removing a character."""
        dlg = QInputDialog(self)
        dlg.setInputMode(QInputDialog.InputMode.TextInput)
        dlg.setInputMethodHints(Qt.InputMethodHint.ImhPreferLatin)
        dlg.setWindowTitle('Remove character')
        dlg.setLabelText('Character to remove')
        if not dlg.exec():
            return
        name = dlg.textValue().strip()
        if not name:
            return
        # Check character name length requirements
        for index in range(self.tracked_characters.count()):
            item = self.tracked_characters.item(index)
            if item.text() == name:
                self.tracked_characters.removeItemWidget(item)
                return
        else:
            QMessageBox.warning(
                self, 'Error', f'Character \"{name}\"not found')

    @slot()
    def _show_about(self) -> None:
        """Show the about dialog."""
        dlg = AboutDialog(self)
        dlg.show()
