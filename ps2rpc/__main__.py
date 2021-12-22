"""Script-like client hook for the ps2rpc client.

This script supports no custom command line options and mostly serves
to launch the underlying Qt application.
"""

import sys
import asyncio
from typing import List

from PyQt6.QtWidgets import QApplication, QWidget, QPushButton

import auraxium
from qasync import QEventLoop, asyncSlot


class Listener(QWidget):

    loop = None
    client = None

    def __init__(self, loop=None):
        super().__init__()
        self.initUi()
        self.loop = loop or asyncio.get_event_loop()

    def initUi(self):
        self.setWindowTitle('Push Test')
        self.send_btn = QPushButton('Send', self)
        self.send_btn.clicked.connect(self.send)

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
