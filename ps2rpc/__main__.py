"""Script-like client hook for the ps2rpc client.

This script supports no custom command line options and mostly serves
to launch the underlying Qt application.
"""

import sys
import asyncio
from typing import Optional

from PyQt6.QtWidgets import QApplication, QWidget, QPushButton

import auraxium

from ._typing_utils import asyncSlot, connect, QEventLoop


class Listener(QWidget):

    loop: Optional[asyncio.AbstractEventLoop]
    client: Optional[auraxium.Client]

    def __init__(self, loop: Optional[asyncio.AbstractEventLoop] = None):
        super().__init__()
        self.initUi()
        self.loop = loop or asyncio.get_event_loop()
        self.client = None

    def initUi(self):
        self.setWindowTitle('Push Test')
        self.send_btn = QPushButton('Send', self)
        connect(self.send_btn.clicked, self.send)

    @asyncSlot()
    async def send(self):
        if self.client is None:
            self.client = auraxium.Client()
        print(await self.client.request(auraxium.census.Query()))


if __name__ == '__main__':
    app = QApplication(sys.argv)
    loop = QEventLoop(app)
    asyncio.set_event_loop(loop)

    ex = Listener(loop)
    ex.show()
    with loop:
        loop.run_forever()
