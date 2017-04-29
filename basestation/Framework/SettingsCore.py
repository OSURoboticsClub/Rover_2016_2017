"""
    This file contains the Settings sub-class as part of the Framework Class
    This class handles initialization of system settings and handling defaults when no settings are found
"""

#####################################
# Imports
#####################################
# Python native imports
from PyQt5 import QtCore, QtWidgets
import os

# Custom imports

#####################################
# Global Variables
#####################################


#####################################
# PickAndPlateLogger Definition
#####################################
class Settings(QtCore.QObject):
    def __init__(self, main_window):
        super(Settings, self).__init__()

        # ########## Reference to highest level window ##########
        self.main_window = main_window  # type: QtWidgets.QMainWindow

        # ########## Set up settings for program ##########
        self.__setup_settings()

        # ########## Create Instance of settings ##########
        self.settings = QtCore.QSettings()

        # ########## Create Instance of settings ##########
        self.__set_hardcoded_settings()

    @staticmethod
    def __setup_settings():
        # noinspection PyCallByClass,PyTypeChecker,PyArgumentList
        QtCore.QCoreApplication.setOrganizationName("OSURC")
        # noinspection PyCallByClass,PyTypeChecker,PyArgumentList
        QtCore.QCoreApplication.setOrganizationDomain("http://osurobotics.club")
        # noinspection PyCallByClass,PyTypeChecker,PyArgumentList
        QtCore.QCoreApplication.setApplicationName("RoverBaseStation")

    def __set_hardcoded_settings(self):
        # Set the temporary directory used to store files while processing them
        app_data_dir = os.environ["APPDATA"]
        folder_name = "RoverBaseStation"
        full_path = app_data_dir + "\\" + folder_name
        self.settings.setValue("appdata_directory", full_path)
