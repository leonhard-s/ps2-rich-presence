"""Script-like client hook for the ps2rpc client.

This script supports no custom command line options and mostly serves
to launch the underlying Qt application.
"""

import sys
import asyncio

from ._app import Application
from ._qasync import QEventLoop

if __name__ == '__main__':
    # Create application
    app = Application(sys.argv)
    # Create event loop
    loop = QEventLoop(app)
    asyncio.set_event_loop(loop)

    app.run(loop)

    # Run event loop
    with loop:
        loop.run_forever()
