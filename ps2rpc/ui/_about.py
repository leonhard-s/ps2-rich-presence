"""UI and controller definition for the about widget."""

from typing import Optional

from PyQt6.QtCore import Qt
from PyQt6.QtWidgets import (QPushButton, QWidget, QDialog, QHBoxLayout,
                             QLabel, QVBoxLayout)

from .._qasync import connect

__all__ = [
    'AboutDialog',
]


class AboutDialog(QDialog):
    """About dialog widget & UI implementation."""

    def __init__(self, parent: Optional[QWidget] = None,
                 flags: Qt.WindowType = Qt.WindowType.Widget) -> None:
        super().__init__(parent=parent, flags=flags)
        self.setWindowTitle('About PlanetSide 2 Rich Presence')
        self.setWindowModality(Qt.WindowModality.WindowModal)
        # Create main content
        layout = QVBoxLayout()
        self.setLayout(layout)
        about_text = QLabel('Lorem ipsum dolor sit amet.')
        layout.addWidget(about_text)
        about_text.setWordWrap(True)
        about_text.setTextFormat(Qt.TextFormat.MarkdownText)
        # Add bottom bar
        bottom_bar = QHBoxLayout()
        layout.addLayout(bottom_bar)
        bottom_bar.addStretch()
        close_btn = QPushButton('Close')
        bottom_bar.addWidget(close_btn)
        # Connect signals
        connect(close_btn.clicked, self.accept)
