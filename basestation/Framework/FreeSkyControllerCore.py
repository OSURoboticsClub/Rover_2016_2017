"""
    This file contains the live logs page sub-class
"""

#####################################
# Imports
#####################################
# Python native imports
from PyQt5 import QtCore, QtWidgets, QtGui
import logging
from inputs import devices, GamePad
import time

#####################################
# Global Variables
#####################################
GAME_CONTROLLER_NAME = "FrSky FrSky Taranis Joystick"
CONTROLLER_DATA_UPDATE_FREQUENCY = 50  # Times per second


#####################################
# Controller Class Definition
#####################################
class FreeSkyController(QtCore.QThread):

    # ########## Signals ##########
    controller_connection_aquired = QtCore.pyqtSignal(bool)
    controller_update_ready_signal = QtCore.pyqtSignal([dict])

    def __init__(self, main_window):
        super(FreeSkyController, self).__init__()

        # ########## Reference to top level window ##########
        self.main_window = main_window  # type: QtWidgets.QMainWindow

        # ########## Get the settings instance ##########
        self.settings = QtCore.QSettings()

        # ########## Get the Pick And Plate instance of the logger ##########
        self.logger = logging.getLogger("RoverBaseStation")

        # ########## Thread Flags ##########
        self.run_thread_flag = True
        self.setup_controller_flag = True
        self.data_acquisition_and_broadcast_flag = True
        self.controller_aquired = False

        # ########## Class Variables ##########
        self.gamepad = None  # type: GamePad

        self.controller_states = {
            "left_stick_x_axis": 0,
            "left_stick_y_axis": 0,

            "right_stick_x_axis": 0,
            "right_stick_y_axis": 0,

            "sc_state": 0,
            "sf_state": 0,
            "sg_state": 0,
            "se_state": 0,
            "sa_state": 0,

            "ls_axis": 0,
            "rs_axis": 0,

            "s1_axis": 0,
            "s2_axis": 0
        }

        self.raw_mapping_to_class_mapping = {
            "ABS_RX": "left_stick_x_axis",
            "ABS_X": "left_stick_y_axis",


            "ABS_Z": "right_stick_x_axis",
            "ABS_Y": "right_stick_y_axis",


            "BTN_C": "sc_state",
            "BTN_Z": "sf_state",
            "BTN_TL": "sg_state",
            "BTN_WEST": "se_state",
            "BTN_SOUTH": "sa_state",

            "ABS_RY": "ls_axis",
            "ABS_RUDDER": "rs_axis",

            "ABS_RZ": "s1_axis",
            "ABS_THROTTLE" : "s2_axis"
        }

        self.last_time = time.time()

    def run(self):
        self.logger.debug("FreeSky Thread Starting...")

        while self.run_thread_flag:
            if self.setup_controller_flag:
                self.controller_aquired = self.__setup_controller()
                self.setup_controller_flag = False
            if self.data_acquisition_and_broadcast_flag:
                self.__get_controller_data()
                self.__broadcast_if_ready()

        self.logger.debug("FreeSky Thread Stopping...")

    # noinspection PyUnresolvedReferences
    def connect_signals_to_slots__slot(self):
        self.main_window.kill_threads_signal.connect(self.on_kill_threads__slot)

    def __setup_controller(self):
        for device in devices.gamepads:
            if device.name == GAME_CONTROLLER_NAME:
                self.gamepad = device
                self.controller_connection_aquired.emit(True)
                return True
        self.logger.info("FrySky Failed to Connect")
        self.controller_connection_aquired.emit(False)
        return False
    def __get_controller_data(self):
        if self.controller_aquired:
            events = self.gamepad.read()

            for event in events:
                if event.code in self.raw_mapping_to_class_mapping:
                    self.controller_states[self.raw_mapping_to_class_mapping[event.code]] = event.state

                # if event.code not in self.raw_mapping_to_class_mapping and event.code != "SYN_REPORT":
                #     self.logger.debug(str(event.code) + " : " + str(event.state))
                #
                # if event.code == "ABS_RUDDER":
                #     self.logger.debug(str(event.code) + " : " + str(event.state))

    def __broadcast_if_ready(self):
        current_time = time.time()

        if (current_time - self.last_time) > (1/CONTROLLER_DATA_UPDATE_FREQUENCY):
            self.controller_update_ready_signal.emit(self.controller_states)
            self.last_time = current_time
            # self.logger.debug(self.controller_states)

    def on_kill_threads__slot(self):
        self.terminate()  # DON'T normally do this, but fine in this instance
        self.run_thread_flag = False


