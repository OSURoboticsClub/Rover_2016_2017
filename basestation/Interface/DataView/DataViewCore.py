"""
    This file contains the data view page sub-class
"""

#####################################
# Imports
#####################################
# Python native imports
from PyQt5 import QtCore, QtWidgets, QtGui
import logging

#####################################
# Global Variables
#####################################
JOYSTICK_AXIS_MIN = -144
JOYSTICK_AXIS_MAX = 144


#####################################
# About Class Definition
#####################################
class DataView(QtCore.QObject):
    def __init__(self, main_window):
        super(DataView, self).__init__()

        # ########## Reference to top level window ##########
        self.main_window = main_window  # type: QtWidgets.QMainWindow

        # ########## Get the settings instance ##########
        self.settings = QtCore.QSettings()

        # ########## Get the Pick And Plate instance of the logger ##########
        self.logger = logging.getLogger("RoverBaseStation")

        # ########## References to GUI Elements ##########
        self.left_y_lb = self.main_window.left_y_lb
        self.right_y_lb = self.main_window.right_y_lb

        self.pause_mode = self.main_window.pause_mode
        self.drive_mode = self.main_window.drive_mode

        # self.time_label = self.main_window.time_label
        # self.voltage_label = self.main_window.battery_voltage_label

        # ########## Class Variables ##########
        self.mutex = QtCore.QMutex()
        self.controller_states = None

        # ########## Make signal/slot connections ##########
        self.__connect_signals_to_slots()

        # ########## Set flags ##########
        self.xbox_connected = False
        self.frysky_connected = False

    def __connect_signals_to_slots(self):
        self.main_window.frsky_controller_class.controller_update_ready_signal.connect(self.on_controller_update_ready__slot)
        self.main_window.frsky_controller_class.controller_connection_aquired.connect(self.frysky_connected__slot)
        self.main_window.xbox_controller_class.controller_connection_aquired.connect(self.xbox_connected__slot)

    def __update_drive_percentages(self):
        if self.frysky_connected:
            left_percentage = round((self.controller_states["left_stick_y_axis"] / JOYSTICK_AXIS_MAX * 100), 2)
            right_percentage = round((self.controller_states["right_stick_y_axis"] / JOYSTICK_AXIS_MAX * 100), 2)

            pause_mode = self.controller_states["sc_state"]
            drive_mode = self.controller_states["se_state"]

            self.left_y_lb.setText(str(left_percentage) + "%")
            self.right_y_lb.setText(str(right_percentage) + "%")
            self.pause_mode.setText(str(pause_mode))
            self.drive_mode.setText(str(drive_mode))

    def on_update_other_gui_elements__slot(self, voltage, time):
        # self.time_label.setText(time)
        # self.voltage_label.setText(voltage)
        pass

    def on_controller_update_ready__slot(self, controller_states):
        self.controller_states = controller_states
        self.__update_drive_percentages()

    def xbox_connected__slot(self, connected):
        self.xbox_connected = connected

    def frysky_connected__slot(self, connected):
        self.frysky_connected = connected