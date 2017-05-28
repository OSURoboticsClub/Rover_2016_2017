"""
    This file calls read voltage, and read drive motor power
"""

#####################################
# Imports
#####################################
# Python native imports
from PyQt5 import QtCore, QtWidgets
import logging
from Framework.MiniBoardIOCore import read_drive_motor_power, read_battery_voltage, read_arm_motors
import time


class ReadUpdater(QtCore.QThread):
    send_miniboard_control_packet = QtCore.pyqtSignal(list)

    def __init__(self, main_window):
        super(ReadUpdater, self).__init__()

        # ########## Reference to top level window ##########
        self.main_window = main_window  # type: QtWidgets.QMainWindow

        # ########## Get the settings instance ##########
        self.settings = QtCore.QSettings()

        # ########## Get the Pick And Plate instance of the logger ##########
        self.logger = logging.getLogger("RoverBaseStation")

        # ########## Some Flags ##########
        self.run_thread_flag = True

        # ########## Class variables ##########
        self.data_last_seen = time.time()

    def run(self):
        self.logger.debug("Read Updater Thread Starting...")

        while self.run_thread_flag:
            #needs signal to communincate effectively with the rover
            read_drive_motor_power(self.send_miniboard_control_packet)
            read_battery_voltage(self.send_miniboard_control_packet)
            read_arm_motors(self.send_miniboard_control_packet)
            self.msleep(1000)

        self.logger.debug("Read Updater Thread Stopping...")


    def connect_signals_to_slots__slot(self):
        self.send_miniboard_control_packet.connect(self.main_window.miniboard_class.append)

        self.main_window.kill_threads_signal.connect(self.on_kill_threads__slot)

    def on_kill_threads__slot(self):
        self.run_thread_flag = False
