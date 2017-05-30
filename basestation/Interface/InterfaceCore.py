"""
    This file contains the interface core sub-class
    This instantiates all the high level sub-classes for the interface
"""

#####################################
# Imports
#####################################
# Python native imports
from PyQt5 import QtCore, QtWidgets, QtWebEngine

# Custom imports
from Interface.LiveLogs.LiveLogsCore import LiveLogs
from PyQt5.QtQuick import QQuickView, QQuickItem
from PyQt5.QtCore import QUrl, QObject
from Interface.Map.MapCore import Map
from Interface.DataView.DataViewCore import DataView


#####################################
# Interface Class Definition
#####################################
class Interface(QtCore.QObject):
    def __init__(self, main_window):
        super(Interface, self).__init__()

        # ########## Reference to top level window ##########
        self.main_window = main_window  # type: QtWidgets.QMainWindow

        QtWebEngine.QtWebEngine.initialize()

        # ########## Instantiations of sub-classes ##########
        self.live_logs_class = LiveLogs(self.main_window)
        # self.maps_class = Map(self.main_window)
        self.data_view_class = DataView(self.main_window)


        # ########## References to GUI Elements ##########
        self.tab_widget = self.main_window.tab_widget  # type: QtWidgets.QTabWidget

        # ########## Set default interface parameters ##########
        # Always open to first tab on launch
        self.tab_widget.setCurrentIndex(0)

    def qml_clicked__slot(self):
        print("UI CLICKED")