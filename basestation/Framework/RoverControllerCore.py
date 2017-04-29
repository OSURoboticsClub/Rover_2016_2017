"""
    This file contains the live logs page sub-class
"""

#####################################
# Imports
#####################################
# Python native imports
from PyQt5 import QtCore, QtWidgets, QtGui
import logging
from random import uniform
import datetime

#####################################
# Global Variables
#####################################


#####################################
# Controller Class Definition
#####################################
class RoverController(QtCore.QThread):

    update_other_gui_elements = QtCore.pyqtSignal(str, str)

    def __init__(self, main_window):
        super(RoverController, self).__init__()

        # ########## Reference to top level window ##########
        self.main_window = main_window  # type: QtWidgets.QMainWindow

        # ########## Get the settings instance ##########
        self.settings = QtCore.QSettings()

        # ########## Get the Pick And Plate instance of the logger ##########
        self.logger = logging.getLogger("RoverBaseStation")

        # ########## Thread Flags ##########
        self.run_thread_flag = True
        self.show_messages = True

        # ########## Class Variables ##########

        # ########## Make signal/slot connections ##########
        self.__connect_signals_to_slots()

        # ########## Start Thread ##########
        # self.start()

    def run(self):
        self.logger.debug("Rover controller Thread Starting...")

        while self.run_thread_flag:
            self.__create_and_broadcast_updates()
            self.msleep(1000)

        self.logger.debug("Rover controller Thread Stopping...")

    # noinspection PyUnresolvedReferences
    def __connect_signals_to_slots(self):
        self.update_other_gui_elements.connect(self.main_window.interface_class.data_view_class.on_update_other_gui_elements__slot)

        self.main_window.kill_threads_signal.connect(self.on_kill_threads__slot)

    def __create_and_broadcast_updates(self):
        value = round(26 + uniform(-4, 4), 2)
        self.logger.debug("Got rover heartbeat. Updating timestamp.")
        self.logger.debug("Got new rover voltage: " + str(value))
        self.update_other_gui_elements.emit(str(value) + "V", str(datetime.datetime.now()))

    def on_kill_threads__slot(self):
        self.run_thread_flag = False


