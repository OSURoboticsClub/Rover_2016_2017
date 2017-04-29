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

#####################################
# Global Variables
#####################################
GAME_CONTROLLER_NAME = "FrSky FrSky Taranis Joystick"


#####################################
# Controller Class Definition
#####################################
class FreeSkyController(QtCore.QThread):

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
        self.broadcast_controller_data_flag = True

        # ########## Class Variables ##########
        self.gamepad = None  # type: GamePad

        self.controller_states = {
            "left_stick_x_axis": 0,
            "left_stick_y_axis": 0,

            "right_stick_x_axis": 0,
            "right_stick_y_axis": 0,

            "sf_state"
            "sg_state"
            "se_state"
            
            "ls_axis"
            "rs_axis"

            "dummy": 0
        }

        self.raw_mapping_to_class_mapping = {
            "ABS_RX": "left_stick_x_axis",
            "ABS_X": "left_stick_y_axis",
            "BTN_THUMBL": "left_stick_center_pressed",

            "ABS_Z": "right_stick_x_axis",
            "ABS_Y": "right_stick_y_axis",
            "BTN_THUMBR": "right_stick_center_pressed",

            "ABS_Z": "left_trigger_z_axis",
            "BTN_TL": "left_bumper_pressed",

            "ABS_RZ": "right_trigger_z_axis",
            "BTN_TR": "right_bumper_pressed",

            "ABS_HAT0X": "dpad_x",
            "ABS_HAT0Y": "dpad_y",

            "BTN_SELECT": "select_pressed",
            "BTN_START": "start_pressed",
            "BTN_HOME": "home_pressed",

            "BTN_SOUTH": "a_pressed",
            "BTN_EAST": "b_pressed",
            "BTN_NORTH": "x_pressed",
            "BTN_WEST": "y_pressed",

            "SYN_REPORT": "dummy"
        }


        # ########## Make signal/slot connections ##########
        self.__connect_signals_to_slots()

        # ########## Start Thread ##########
        self.start()

    def run(self):
        self.logger.debug("FreeSky Thread Starting...")

        while self.run_thread_flag:
            if self.setup_controller_flag:
                self.__setup_controller()
                self.setup_controller_flag = False
            if self.broadcast_controller_data_flag:
                self.__get_and_broadcast_controller_data()
            # self.msleep(50)

        self.logger.debug("FreeSky Thread Stopping...")

    # noinspection PyUnresolvedReferences
    def __connect_signals_to_slots(self):
        self.main_window.kill_threads_signal.connect(self.on_kill_threads__slot)

    def __setup_controller(self):
        for device in devices.gamepads:
            if device.name == GAME_CONTROLLER_NAME:
                self.gamepad = device
                return

    def __get_and_broadcast_controller_data(self):
        events = self.gamepad.read()

        for event in events:

            self.logger.debug(str(event.code) + " : " + str(event.state))
            # self.controller_states[self.raw_mapping_to_class_mapping[event.code]] = event.state
            self.controller_update_ready_signal.emit(self.controller_states)


    def on_kill_threads__slot(self):
        self.terminate()  # DON'T normally do this!!!!!
        self.run_thread_flag = False


