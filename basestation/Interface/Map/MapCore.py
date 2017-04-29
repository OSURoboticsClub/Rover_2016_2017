"""
    This file contains the map page sub-class
"""

#####################################
# Imports
#####################################
# Python native imports
from PyQt5 import QtCore, QtWidgets, QtGui
import logging


#####################################
# About Class Definition
#####################################
class Map(QtCore.QObject):
    def __init__(self, main_window):
        super(Map, self).__init__()

        # ########## Reference to top level window ##########
        self.main_window = main_window  # type: QtWidgets.QMainWindow

        # ########## Get the settings instance ##########
        self.settings = QtCore.QSettings()

        # ########## Get the Pick And Plate instance of the logger ##########
        self.logger = logging.getLogger("RoverBaseStation")

        # ########## References to GUI Elements ##########
        self.maps_view = self.main_window.map_label  # type: QtWidgets.QLabel

        # ########## Class Variables ##########
        self.map_view_pixmap = QtGui.QPixmap("Resources/CachedMaps/mars_testing_site.png")

        self.maps_view.setPixmap(self.map_view_pixmap)

        # ########## Set defaults on GUI Elements ##########
        self.__load_settings()

    def __load_settings(self):
        self.logger.info("Map Interface Configured")
