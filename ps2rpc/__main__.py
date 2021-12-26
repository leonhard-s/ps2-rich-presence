"""Script-like client hook for the ps2rpc client.

This script supports no custom command line options and mostly serves
to launch the underlying Qt application.
"""

import sys
import asyncio

from PyQt6.QtWidgets import QApplication

from ._listener import Listener
from ._qasync import QEventLoop

if __name__ == '__main__':
    # Create application
    app = QApplication(sys.argv)

    # Configure event loop
    loop = QEventLoop(app)
    asyncio.set_event_loop(loop)

    # Create listener object wrapping the ESS client
    listener = Listener(loop)

    # Run event loop
    with loop:
        loop.run_forever()
