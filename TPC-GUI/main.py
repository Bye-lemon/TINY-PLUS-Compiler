import sys

from PyQt5 import QtWidgets
from TPC import TPC

if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    tpc = TPC()
    tpc.show()
    app.exec_()
