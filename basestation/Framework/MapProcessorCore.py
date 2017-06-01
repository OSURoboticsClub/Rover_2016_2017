"""
    This file contains the live logs page sub-class
"""

#####################################
# Imports
#####################################
# Python native imports
from PyQt5 import QtCore, QtWidgets, QtGui
from Resources.Libraries.goompy import GooMPy
from PIL.ImageQt import ImageQt
import logging

#####################################
# Global Variables
#####################################
INITIAL_LATITUDE = 38.4064262
INITIAL_LONGITUDE = -110.794115
INITIAL_ZOOM = 18  # 15,16, and 18 18 seems best
MAPTYPE = 'satellite'

WIDTH = 2*640
HEIGHT = 640


#####################################
# Live Logs Class Definition
#####################################
class MapProcessor(QtCore.QThread):

    update_map_label_signal = QtCore.pyqtSignal()

    def __init__(self, main_window):
        super(MapProcessor, self).__init__()

        # ########## Reference to top level window ##########
        self.main_window = main_window  # type: QtWidgets.QMainWindow

        # ########## Get the settings instance ##########
        self.settings = QtCore.QSettings()

        # ########## Get the Pick And Plate instance of the logger ##########
        self.logger = logging.getLogger("RoverBaseStation")

        # ########## Thread Flags ##########
        self.run_thread_flag = True
        self.setup_goompy_flag = True

        # ########## References to GUI Elements ##########
        self.map_label = self.main_window.map_label  # type: QtWidgets.QLabel

        # ########## Class Variables ##########
        self.goompy = None
        self.pixmap = None

    def run(self):
        self.logger.debug("Map Processor Thread Starting...")

        while self.run_thread_flag:
            if self.setup_goompy_flag:
                self.__setup_goompy()
                self.setup_goompy_flag = False
            else:
                self.__show_map_plus_overlays()
            self.msleep(10)

        self.logger.debug("Map Processor Thread Stopping...")

    def __setup_goompy(self):
        self.goompy = GooMPy(WIDTH, HEIGHT, INITIAL_LATITUDE, INITIAL_LONGITUDE, INITIAL_ZOOM, MAPTYPE, 1500)

    def __show_map_plus_overlays(self):
        pil_image = self.goompy.getImage().convert("RGBA")

        qim = ImageQt(pil_image)
        self.pixmap = QtGui.QPixmap.fromImage(qim)

        self.update_map_label_signal.emit()

    def __on_map_label_update_requested__slot(self):
        self.map_label.setPixmap(self.pixmap)

    # noinspection PyUnresolvedReferences
    def connect_signals_to_slots__slot(self):
        self.main_window.kill_threads_signal.connect(self.on_kill_threads__slot)
        self.update_map_label_signal.connect(self.__on_map_label_update_requested__slot)

    def on_kill_threads__slot(self):
        self.run_thread_flag = False
