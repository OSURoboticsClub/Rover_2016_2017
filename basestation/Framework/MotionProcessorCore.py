"""
    This file contains the live logs page sub-class
"""

#####################################
# Imports
#####################################
# Python native imports
from PyQt5 import QtCore, QtWidgets
import logging
from Framework.MiniBoardIOCore import write_drive_motor_power, \
    read_drive_motor_power, write_pause

#####################################
# Global Variables
#####################################
DRIVE_MIN = -127
DRIVE_MAX = 127

#####################################
# Notes
#####################################
##### Xbox State Names
# "left_stick_x_axis"
# "left_stick_y_axis"
# "left_stick_center_pressed"
#
# "right_stick_x_axis"
# "right_stick_y_axis"
# "right_stick_center_pressed"
#
# "left_trigger_z_axis"
# "left_bumper_pressed"
#
# "right_trigger_z_axis"
# "right_bumper_pressed"
#
# "dpad_x"
# "dpad_y"
#
# "select_pressed"
# "start_pressed"
# "home_pressed"
#
# "a_pressed"
# "b_pressed"
# "x_pressed"
# "y_pressed"

##### Frsky State Names
# "left_stick_x_axis"
# "left_stick_y_axis"
#
# "right_stick_x_axis"
# "right_stick_y_axis"
#
# "sc_state"
# "sg_state"
# "se_state"
# "sa_state"
#
# "ls_axis"
# "rs_axis"
#
# "s1_axis"


#####################################
# Controller Class Definition
#####################################
class MotionProcessor(QtCore.QThread):
    send_miniboard_control_packet = QtCore.pyqtSignal(list)

    def __init__(self, main_window):
        super(MotionProcessor, self).__init__()

        # ########## Reference to top level window ##########
        self.main_window = main_window  # type: QtWidgets.QMainWindow

        # ########## Get the settings instance ##########
        self.settings = QtCore.QSettings()

        # ########## Get the Pick And Plate instance of the logger ##########
        self.logger = logging.getLogger("RoverBaseStation")

        # ########## Thread Flags ##########
        self.run_thread_flag = True
        self.wait_for_drive_response = False

        # ########## Class Variables ##########
        self.xbox_states = {}
        self.frsky_states = {}

    def run(self):
        self.logger.debug("Motion Processor Thread Starting...")

        # TODO: Switching out of drive state stops motion
        write_pause(self.send_miniboard_control_packet, 1)
        while self.run_thread_flag:
            if self.xbox_states and self.frsky_states:

                if self.frsky_states['sf_state']:                   # 1 is unpaused

                    if not self.frsky_states["sa_state"]:           # 0 is manual mode
                        if not self.frsky_states["se_state"]:       # 0 is drive mode
                            self.__drive_manual()
                        else:                                       # 1 is arm mode
                            self.__arm_manual()
                    else:                                           # 1 is auto mode
                        self.__drive_auto()
                else:                                               # 0 is paused
                    # TODO: STOP ALL MOTION IMMEDIATELY
                    # write_pause(self.send_miniboard_control_packet, 0)
                    self.__all_stop()

        self.logger.debug("Motion Processor Thread Stopping...")

    # noinspection PyUnresolvedReferences
    def connect_signals_to_slots__slot(self):
        self.main_window.xbox_controller_class.controller_update_ready_signal.connect(self.on_xbox_states_updated__slot)
        self.main_window.frsky_controller_class.controller_update_ready_signal.connect(self.on_frsky_states_updated__slot)

        self.send_miniboard_control_packet.connect(self.main_window.miniboard_class.append)

        self.main_window.miniboard_class.data_drive_motor_power.connect(self.on_drive_motor_power_response_received__slot)
        self.main_window.miniboard_class.ack_drive_motor_power.connect(self.on_drive_response_received__slot)

        self.main_window.kill_threads_signal.connect(self.on_kill_threads__slot)

    def __drive_manual(self):
        OFFSET = 127

        max_speed_scale_raw = self.frsky_states["s1_axis"] + OFFSET  # Range (min to max) 0 to 255
        left_stick_y_axis_raw = self.frsky_states["left_stick_y_axis"]  # Range (min to max) -127 to 127
        right_stick_y_axis_raw = self.frsky_states["right_stick_y_axis"]   # Range (min to max) -127 to 127

        scale_percentage = max_speed_scale_raw / 255

        left_scaled = (abs(left_stick_y_axis_raw) * scale_percentage)
        right_scaled = (abs(right_stick_y_axis_raw) * scale_percentage)

        if left_stick_y_axis_raw < 0:
            left_scaled = -left_scaled
        if right_stick_y_axis_raw < 0:
            right_scaled = -right_scaled

        left_scaled = int(self.clamp(left_scaled, DRIVE_MIN, DRIVE_MAX))
        right_scaled = int(self.clamp(right_scaled, DRIVE_MIN, DRIVE_MAX))


        self.wait_for_drive_response = True
        write_drive_motor_power(self.send_miniboard_control_packet, left_scaled, left_scaled, left_scaled, right_scaled, right_scaled, right_scaled)

        while self.wait_for_drive_response:
            self.msleep(1)


        # self.logger.debug(left_scaled)
        # self.logger.debug(right_scaled)

        # read_drive_motor_power(self.send_miniboard_control_packet)

    def __drive_auto(self):
        pass

    def __arm_manual(self):
        pass

    def __all_stop(self):
        pass

    def on_xbox_states_updated__slot(self, states):
        self.xbox_states = states

    def on_frsky_states_updated__slot(self, states):
        self.frsky_states = states

    def on_drive_motor_power_response_received__slot(self, sdict):
        # self.logger.debug(sdict)
        pass

    def on_drive_response_received__slot(self):
        self.wait_for_drive_response = False

    def on_kill_threads__slot(self):
        self.run_thread_flag = False

    @staticmethod
    def clamp(n, minn, maxn):
        return max(min(maxn, n), minn)

