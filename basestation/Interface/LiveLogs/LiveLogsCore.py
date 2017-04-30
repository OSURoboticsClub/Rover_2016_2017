"""
    This file contains the live logs page sub-class
"""

#####################################
# Imports
#####################################
# Python native imports
from PyQt5 import QtCore, QtWidgets, QtGui
import logging


#####################################
# Live Logs Class Definition
#####################################
class LiveLogs(QtCore.QThread):

    text_ready_signal = QtCore.pyqtSignal(str)

    def __init__(self, main_window):
        super(LiveLogs, self).__init__()

        # ########## Reference to top level window ##########
        self.main_window = main_window  # type: QtWidgets.QMainWindow

        # ########## Get the settings instance ##########
        self.settings = QtCore.QSettings()

        # ########## Get the Pick And Plate instance of the logger ##########
        self.logger = logging.getLogger("RoverBaseStation")

        # ########## Thread Flags ##########
        self.run_thread_flag = True
        self.open_log_file_flag = True
        self.show_log_file_flag = True

        # ########## References to GUI Elements ##########
        self.live_log_tb = self.main_window.live_log_text_browser  # type: QtWidgets.QTextBrowser

        # ########## Class Variables ##########
        self.log_file_path = None
        self.log_file_reader = None
        self.log_file_prev_mtime = 0

    def run(self):
        self.logger.debug("Live Logs Thread Starting...")

        while self.run_thread_flag:
            if self.open_log_file_flag:
                self.__open_log_file()
                self.open_log_file_flag = False
            elif self.show_log_file_flag:
                self.__show_updated_log_file()
                self.msleep(250)

        self.logger.debug("Live Logs Thread Stopping...")

    # noinspection PyUnresolvedReferences
    def connect_signals_to_slots__slot(self):
        self.text_ready_signal.connect(self.live_log_tb.setText)
        self.live_log_tb.textChanged.connect(self.__on_move_cursor_needed__slot)

        self.main_window.kill_threads_signal.connect(self.on_kill_threads__slot)

    def __open_log_file(self):
        # Get the log file path
        appdata_base_directory = self.settings.value("appdata_directory", type=str)
        log_directory = appdata_base_directory + "/logs"
        self.log_file_path = log_directory + "/log.txt"

        # Open the class' reader for the file
        self.log_file_reader = open(self.log_file_path, 'r')

    def __show_updated_log_file(self):
        log_browser_string = ""

        # Seek back to the beginning of the file and read in the lines
        self.log_file_reader.seek(0)
        log_lines = self.log_file_reader.readlines()

        # Go through line by line and only add lines that are selected to be shown via the checkboxes
        for line in log_lines:
                log_browser_string += line

        # Display the text
        self.text_ready_signal.emit(log_browser_string)

    def __on_move_cursor_needed__slot(self):
        # Move the cursor to the end when the text browser text updates. This essentially scrolls constantly.
        self.live_log_tb.moveCursor(QtGui.QTextCursor.End)

    def on_kill_threads__slot(self):
        self.run_thread_flag = False
