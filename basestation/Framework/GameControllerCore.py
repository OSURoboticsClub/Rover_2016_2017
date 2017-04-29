"""
    This file contains the live logs page sub-class
"""

#####################################
# Imports
#####################################
# Python native imports
from PyQt5 import QtCore, QtWidgets, QtGui
import logging
from inputs import get_gamepad

#####################################
# Global Variables
#####################################


#####################################
# Controller Class Definition
#####################################
class GameController(QtCore.QThread):

    controller_update_ready_signal = QtCore.pyqtSignal([dict])

    def __init__(self, main_window):
        super(GameController, self).__init__()

        # ########## Reference to top level window ##########
        self.main_window = main_window  # type: QtWidgets.QMainWindow

        # ########## Get the settings instance ##########
        self.settings = QtCore.QSettings()

        # ########## Get the Pick And Plate instance of the logger ##########
        self.logger = logging.getLogger("RoverBaseStation")

        # ########## Thread Flags ##########
        self.run_thread_flag = True
        self.setup_controller_flag = True
        self.broadcast_controller_data_flag = True

        # ########## Class Variables ##########
        self.controller_states = {
            "left_stick_y_axis": 0,
            "right_stick_y_axis": 0,
            "left_trigger_z_axis": 0,
            "right_trigger_z_axis": 0
        }

        # ########## Make signal/slot connections ##########
        self.__connect_signals_to_slots()

        # ########## Start Thread ##########
        # self.start()

    def run(self):
        self.logger.debug("Controller Thread Starting...")

        while self.run_thread_flag:
            if self.broadcast_controller_data_flag:
                self.__get_and_broadcast_controller_data()

        self.logger.debug("Controller Thread Stopping...")

    # noinspection PyUnresolvedReferences
    def __connect_signals_to_slots(self):
        self.main_window.kill_threads_signal.connect(self.on_kill_threads__slot)

    def __get_and_broadcast_controller_data(self):
        events = get_gamepad()

        for event in events:
            if event.code == "ABS_Y":
                self.controller_states["left_stick_y_axis"] = event.state
            elif event.code == "ABS_RY":
                self.controller_states["right_stick_y_axis"] = event.state
            elif event.code == "ABS_Z":
                self.controller_states["left_trigger_z_axis"] = event.state
            elif event.code == "ABS_RZ":
                self.controller_states["right_trigger_z_axis"] = event.state
            self.controller_update_ready_signal.emit(self.controller_states)

    def on_kill_threads__slot(self):
        self.terminate()  # DON'T normally do this!!!!!
        self.run_thread_flag = False


