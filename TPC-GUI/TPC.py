# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'TPC.ui'
#
# Created by: PyQt5 UI code generator 5.11.3
#
# WARNING! All changes made in this file will be lost!
import os

from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(800, 600)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.output = QtWidgets.QTextBrowser(self.centralwidget)
        self.output.setGeometry(QtCore.QRect(0, 411, 801, 141))
        self.output.setObjectName("output")
        self.console = QtWidgets.QLabel(self.centralwidget)
        self.console.setGeometry(QtCore.QRect(10, 390, 54, 12))
        font = QtGui.QFont()
        font.setFamily("Microsoft YaHei UI")
        font.setBold(True)
        font.setWeight(75)
        self.console.setFont(font)
        self.console.setObjectName("console")
        self.input = QtWidgets.QTextEdit(self.centralwidget)
        self.input.setGeometry(QtCore.QRect(0, 0, 801, 381))
        self.input.setObjectName("input")
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 800, 23))
        self.menubar.setObjectName("menubar")
        self.start = QtWidgets.QMenu(self.menubar)
        self.start.setObjectName("start")
        self.compile = QtWidgets.QMenu(self.menubar)
        self.compile.setObjectName("compile")
        self.menu_3 = QtWidgets.QMenu(self.menubar)
        self.menu_3.setObjectName("menu_3")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)
        self.newfile = QtWidgets.QAction(MainWindow)
        self.newfile.setShortcutContext(QtCore.Qt.WindowShortcut)
        self.newfile.setShortcutVisibleInContextMenu(True)
        self.newfile.setObjectName("newfile")
        self.openfile = QtWidgets.QAction(MainWindow)
        self.openfile.setObjectName("openfile")
        self.quit = QtWidgets.QAction(MainWindow)
        self.quit.setShortcutVisibleInContextMenu(True)
        self.quit.setObjectName("quit")
        self.compile_2 = QtWidgets.QAction(MainWindow)
        self.compile_2.setShortcutVisibleInContextMenu(True)
        self.compile_2.setObjectName("compile_2")
        self.execute = QtWidgets.QAction(MainWindow)
        self.execute.setShortcutVisibleInContextMenu(True)
        self.execute.setObjectName("execute")
        self.scan = QtWidgets.QAction(MainWindow)
        self.scan.setShortcutVisibleInContextMenu(True)
        self.scan.setObjectName("scan")
        self.parse = QtWidgets.QAction(MainWindow)
        self.parse.setShortcutVisibleInContextMenu(True)
        self.parse.setObjectName("parse")
        self.middle = QtWidgets.QAction(MainWindow)
        self.middle.setShortcutVisibleInContextMenu(True)
        self.middle.setObjectName("middle")
        self.description = QtWidgets.QAction(MainWindow)
        self.description.setShortcutVisibleInContextMenu(True)
        self.description.setObjectName("description")
        self.actionabout = QtWidgets.QAction(MainWindow)
        self.actionabout.setShortcutVisibleInContextMenu(True)
        self.actionabout.setObjectName("actionabout")
        self.savefile = QtWidgets.QAction(MainWindow)
        self.savefile.setShortcutVisibleInContextMenu(True)
        self.savefile.setObjectName("savefile")
        self.start.addAction(self.newfile)
        self.start.addAction(self.openfile)
        self.start.addAction(self.savefile)
        self.start.addSeparator()
        self.start.addAction(self.quit)
        self.compile.addAction(self.compile_2)
        self.compile.addAction(self.execute)
        self.compile.addSeparator()
        self.compile.addAction(self.scan)
        self.compile.addAction(self.parse)
        self.compile.addAction(self.middle)
        self.menu_3.addAction(self.description)
        self.menu_3.addSeparator()
        self.menu_3.addAction(self.actionabout)
        self.menubar.addAction(self.start.menuAction())
        self.menubar.addAction(self.compile.menuAction())
        self.menubar.addAction(self.menu_3.menuAction())

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)
        MainWindow.setTabOrder(self.input, self.output)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "TINY+ Language Compiler"))
        self.console.setText(_translate("MainWindow", "控制台"))
        self.start.setTitle(_translate("MainWindow", "开始"))
        self.compile.setTitle(_translate("MainWindow", "编译"))
        self.menu_3.setTitle(_translate("MainWindow", "帮助"))
        self.newfile.setText(_translate("MainWindow", "新建脚本文件"))
        self.newfile.setShortcut(_translate("MainWindow", "Ctrl+N"))
        self.openfile.setText(_translate("MainWindow", "打开脚本文件"))
        self.openfile.setShortcut(_translate("MainWindow", "Ctrl+O"))
        self.quit.setText(_translate("MainWindow", "退出程序"))
        self.quit.setIconText(_translate("MainWindow", "退出程序"))
        self.quit.setShortcut(_translate("MainWindow", "Ctrl+Q"))
        self.compile_2.setText(_translate("MainWindow", "编译"))
        self.compile_2.setShortcut(_translate("MainWindow", "F5"))
        self.execute.setText(_translate("MainWindow", "运行"))
        self.execute.setShortcut(_translate("MainWindow", "F6"))
        self.scan.setText(_translate("MainWindow", "词法分析结果"))
        self.scan.setShortcut(_translate("MainWindow", "F7"))
        self.parse.setText(_translate("MainWindow", "语法分析结果"))
        self.parse.setShortcut(_translate("MainWindow", "F8"))
        self.middle.setText(_translate("MainWindow", "查看中间代码"))
        self.middle.setShortcut(_translate("MainWindow", "F9"))
        self.description.setText(_translate("MainWindow", "TINY+ 语言描述"))
        self.description.setShortcut(_translate("MainWindow", "Ctrl+H"))
        self.actionabout.setText(_translate("MainWindow", "关于"))
        self.actionabout.setShortcut(_translate("MainWindow", "Ctrl+A"))
        self.savefile.setText(_translate("MainWindow", "保存脚本文件"))
        self.savefile.setShortcut(_translate("MainWindow", "Ctrl+S"))


class TPC(QtWidgets.QMainWindow, Ui_MainWindow):
    def __init__(self):
        super(TPC, self).__init__()
        self.setupUi(self)
        self.filename = ""
        self.openfile.triggered.connect(self.fileopen)
        self.savefile.triggered.connect(self.filesave)
        self.newfile.triggered.connect(self.filenew)
        self.compile_2.triggered.connect(self.filecompile)
        self.scan.triggered.connect(self.showscan)
        self.filenew()
        self.input.setFont(QtGui.QFont("Ubuntu Mono", 12))

    def fileopen(self):
        filename, filetype = QtWidgets.QFileDialog.getOpenFileName(self, "打开脚本文件", "C:", "TINY+ Script (*.TNY)")
        if filename:
            try:
                file = QtCore.QFile(filename)
                if not file.open(QtCore.QIODevice.ReadOnly):
                    raise IOError(file.errorString())
                istream = QtCore.QTextStream(file)
                self.input.setPlainText(istream.readAll())
                self.filename = filename
            except EnvironmentError as e:
                QtWidgets.QMessageBox.warning(self, "脚本 {0} 加载失败".format(filename))
            finally:
                if file is not None:
                    file.close()
            self.setWindowTitle("TINY+ Language Compiler " + QtCore.QFileInfo(file).fileName())

    def filenew(self):
        self.filename = "Untitled-0"
        self.input.clear()
        self.setWindowTitle("TINY+ Language Compiler " + self.filename)

    def filesave(self):
        if self.filename.startswith("Untitled"):
            self.filename = QtWidgets.QFileDialog.getSaveFileName(self, "保存脚本文件", self.filename, "TINY+ files (*.TNY)")[0]
        with open(self.filename, 'w') as file:
            file.write(self.input.toPlainText())
        self.output.setText("文件保存成功！")
        self.setWindowTitle("TINY+ Language Compiler " + QtCore.QFileInfo(self.filename).fileName())

    def filecompile(self):
        self.filesave()
        script = "TPC " + self.filename
        result = os.popen(script)
        self.output.setText(result.read())

    def showscan(self):
        filename = self.filename.replace('.TNY', '.scan')
        oscan = open(filename, "r")
        self.output.setPlainText(oscan.read())
        oscan.close()


