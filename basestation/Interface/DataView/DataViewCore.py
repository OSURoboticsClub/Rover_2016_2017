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
JOYSTICK_AXIS_MIN = -32678
JOYSTICK_AXIS_MAX = 32767.0


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
        self.left_drive_lb = self.main_window.data_view_left_drive_label
        self.right_drive_lb = self.main_window.data_view_right_drive_label

        self.left_trigger_lb = self.main_window.left_trigger_label
        self.right_trigger_lb = self.main_window.right_trigger_label

        self.time_label = self.main_window.time_label
        self.voltage_label = self.main_window.battery_voltage_label

        # ########## Class Variables ##########
        self.mutex = QtCore.QMutex()
        self.controller_states = None

        # ########## Make signal/slot connections ##########
        self.__connect_signals_to_slots()

    def __connect_signals_to_slots(self):
        self.main_window.controller_class.controller_update_ready_signal.connect(self.on_controller_update_ready__slot)

    def __update_drive_percentages(self):
        left_percentage = round((self.controller_states["left_stick_y_axis"] / JOYSTICK_AXIS_MAX * 100), 2)
        right_percentage = round((self.controller_states["right_stick_y_axis"] / JOYSTICK_AXIS_MAX * 100), 2)

        left_value = self.controller_states["left_trigger_z_axis"]
        right_value = self.controller_states["right_trigger_z_axis"]

        self.left_drive_lb.setText(str(left_percentage) + "%")
        self.right_drive_lb.setText(str(right_percentage) + "%")

        self.left_trigger_lb.setText(str(left_value))
        self.right_trigger_lb.setText(str(right_value))

    def on_update_other_gui_elements__slot(self, voltage, time):
        self.time_label.setText(time)
        self.voltage_label.setText(voltage)

    def on_controller_update_ready__slot(self, controller_states):
        self.controller_states = controller_states
        self.__update_drive_percentages()
