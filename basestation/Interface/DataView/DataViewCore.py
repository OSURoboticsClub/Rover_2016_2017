"""
    This file contains the data view page sub-class
"""

#####################################
# Imports
#####################################
# Python native imports
from PyQt5 import QtCore, QtWidgets, QtGui
import logging
from Framework.MiniBoardIOCore import write_drive_motor_power, read_drive_motor_power, write_pause, write_arm_motors

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
        self.miniboard_class = main_window.miniboard_class
        # ########## Get the settings instance ##########
        self.settings = QtCore.QSettings()

        # ########## Get the Pick And Plate instance of the logger ##########
        self.logger = logging.getLogger("RoverBaseStation")

        # ########## References to GUI Elements ##########
        self.left_y_lb = self.main_window.left_y_lb
        self.right_y_lb = self.main_window.right_y_lb

        self.pause_mode = self.main_window.pause_mode
        self.drive_mode = self.main_window.drive_mode

        self.left_motor_power = self.main_window.left_motor
        self.right_motor_power = self.main_window.right_motor

        self.battery_voltage = self.main_window.battery_voltage

        self.base_power = self.main_window.base
        self.bicep_power = self.main_window.bicep
        self.forearm_power = self.main_window.forearm
        self.pitch_power = self.main_window.pitch
        self.wrist_power = self.main_window.wrist

        # self.time_label = self.main_window.time_label
        # self.voltage_label = self.main_window.battery_voltage_label

        # ########## Class Variables ##########
        self.controller_states = None

        # ########## Make signal/slot connections ##########
        self.__connect_signals_to_slots()

        # ########## Set flags ##########
        self.xbox_connected = False
        self.frysky_connected = False

    def __connect_signals_to_slots(self):
        # These are incorrect as they don't show the scaled versions
        # self.main_window.frsky_controller_class.controller_update_ready_signal.connect(self.on_controller_update_ready__slot)
        # self.main_window.frsky_controller_class.controller_connection_aquired.connect(self.frysky_connected__slot)
        # self.main_window.xbox_controller_class.controller_connection_aquired.connect(self.xbox_connected__slot)

        # the data signals pass dictionaries to our slots after read requested
        self.main_window.miniboard_class.data_drive_motor_power.connect(self.__update_drive_power)
        self.main_window.miniboard_class.data_battery_voltage.connect(self.__update_battery_voltage)
        self.main_window.miniboard_class.data_arm_motors.connect(self.__update_arm_power)

    def __update_drive_percentages(self):
        if self.frysky_connected:
            left_percentage = round((self.controller_states["left_stick_y_axis"] / JOYSTICK_AXIS_MAX * 100), 2)
            right_percentage = round((self.controller_states["right_stick_y_axis"] / JOYSTICK_AXIS_MAX * 100), 2)

            self.left_y_lb.setText(str(left_percentage) + "%")
            self.right_y_lb.setText(str(right_percentage) + "%")

    def __update_modes(self):
        pause_mode = self.controller_states["sf_state"]
        drive_mode = self.controller_states["se_state"]

        self.pause_mode.setText(str(pause_mode))
        self.drive_mode.setText(str(drive_mode))

    def __update_drive_power(self, power_dict):
        self.left_motor_power.setText("F:%3d M:%3d B:%3d" % (
            power_dict["l_f_drive"],
            power_dict["l_m_drive"],
            power_dict["l_b_drive"],
        ))
        self.right_motor_power.setText("F:%3d M:%3d B:%3d" % (
            power_dict["r_f_drive"],
            power_dict["r_m_drive"],
            power_dict["r_b_drive"],
        ))

    def __update_battery_voltage(self, battery_dict):
        battery_visually_accurate = int(battery_dict["battery_voltage"]) / 1000
        self.battery_voltage.setText("%0.2f V" % battery_visually_accurate)

    def __update_arm_power(self, power_dict):
        self.base_power.setText("%d" % power_dict["arm_motor_1"])
        self.bicep_power.setText("%d" % power_dict["arm_motor_2"])
        self.forearm_power.setText("%d" % power_dict["arm_motor_3"])
        self.pitch_power.setText("%d" % power_dict["arm_motor_4"])
        self.wrist_power.setText("%d" % power_dict["arm_motor_5"])

    def on_update_other_gui_elements__slot(self, voltage, time):
        # self.time_label.setText(time)
        # self.voltage_label.setText(voltage)
        pass

    def on_controller_update_ready__slot(self, controller_states):
        self.controller_states = controller_states
        self.__update_drive_percentages()
        self.__update_modes()

    def xbox_connected__slot(self, connected):
        self.xbox_connected = connected

    def frysky_connected__slot(self, connected):
        self.frysky_connected = connected
