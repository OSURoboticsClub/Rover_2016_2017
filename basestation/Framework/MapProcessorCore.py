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
from PIL import ImageDraw
import logging

#####################################
# Global Variables
#####################################
INITIAL_LATITUDE = 38.4064262
INITIAL_LONGITUDE = -110.794115
INITIAL_ZOOM = 18  # 15,16, and 18 18 seems best
MAPTYPE = 'satellite'

POSSIBLE_ZOOM_LEVELS = [18]

WIDTH = 2 * 640
HEIGHT = 640

HAB_X_OFFSET = 82
HAB_Y_OFFSET = -9


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
        self.pixmap = QtGui.QPixmap("Resources/Images/waiting.png")

        self.gps_valid = None
        self.lat = None
        self.lon = None
        self.alt = None  # {'gps_pos_valid': 1, 'altitude': 21052, 'longitude': -668740150, 'latitude': 229797144}

        self.gps_track_valid = None
        self.gps_speed = None
        self.gps_heading = None

        self.current_zoom_index = POSSIBLE_ZOOM_LEVELS.index(INITIAL_ZOOM)
        self.last_zoom_level_index = self.current_zoom_index
        self.num_zoom_levels = len(POSSIBLE_ZOOM_LEVELS)

        # ########## Gui Event Variables ##########
        self.x_drag = 0
        self.y_drag = 0

    def run(self):
        self.logger.debug("Map Processor Thread Starting...")
        self.update_map_label_signal.emit()

        while self.run_thread_flag:
            if self.setup_goompy_flag:
                self.__setup_goompy()
                self.setup_goompy_flag = False
            else:
                self.__add_map_point_from_gps()
                self.__show_map_plus_overlays()
            self.msleep(1)

        self.logger.debug("Map Processor Thread Stopping...")

    def __setup_goompy(self):
        self.goompy = GooMPy(WIDTH, HEIGHT, INITIAL_LATITUDE, INITIAL_LONGITUDE, INITIAL_ZOOM, MAPTYPE, 1500)
        self.goompy.move(HAB_X_OFFSET, HAB_Y_OFFSET)

    def __show_map_plus_overlays(self):

        if self.last_zoom_level_index != self.current_zoom_index:
            self.logger.debug("Changed to zoom level: " + str(POSSIBLE_ZOOM_LEVELS[self.current_zoom_index]))
            self.goompy.useZoom(POSSIBLE_ZOOM_LEVELS[self.current_zoom_index])
            self.last_zoom_level_index = self.current_zoom_index

        pil_image = self.goompy.getImage().convert("RGBA")

        self.__draw_circle_at_point(pil_image, self.goompy.width // 2, self.goompy.height // 2, 3)

        qim = ImageQt(pil_image)
        self.pixmap = QtGui.QPixmap.fromImage(qim)

        self.update_map_label_signal.emit()

    def __add_map_point_from_gps(self):
        self.logger.debug("Test")

    def __draw_circle_at_point(self, image, x, y, r):
        draw = ImageDraw.Draw(image)
        draw.ellipse((x - r, y - r, x + r, y + r), fill=(255, 0, 0, 255))

    def __on_map_label_update_requested__slot(self):
        self.map_label.setPixmap(self.pixmap)

    def on_gps_coordinates_updated__slot(self, sdict):
        self.gps_valid = sdict["gps_pos_valid"]
        self.lat = (sdict["latitude"] * 10 ** -5) / 60
        self.lon = (sdict["longitude"] * 10 ** -5) / 60
        self.alt = (sdict["altitude"] / 10)

        if self.gps_valid:
            self.logger.debug("LAT: " + str(self.lat) + " | LON: " + str(self.lon) + " | ALT: " + str(self.alt))
        else:
            self.logger.debug("Waiting for GPS lock")

    def on_gps_track_updated__slot(self, sdict):
        self.gps_track_valid = sdict["gps_track_valid"]
        self.gps_speed = sdict["gps_speed"]
        self.gps_heading = sdict["gps_heading"]

        if self.gps_track_valid:
            self.logger.debug("SPEED: " + str(self.gps_speed) + " | HEADING: " + str(self.gps_heading))
        else:
            pass

    def __mouse_released_on_map_event(self, _):
        self.x_drag = -1
        self.y_drag = -1

    def __mouse_scrolled_on_map(self, event):
        if self.goompy:
            if event.angleDelta().y() > 0:
                self.current_zoom_index += 1
            else:
                self.current_zoom_index -= 1

            self.current_zoom_index = self.clamp(self.current_zoom_index, 0, self.num_zoom_levels - 1)

    def __mouse_moved_on_map_label(self, event):
        if self.x_drag != -1 and self.y_drag != -1 and self.goompy:
            buttons = event.buttons()
            if buttons == QtCore.Qt.LeftButton:
                x_pos = event.pos().x()
                y_pos = event.pos().y()
                dx = x_pos - self.x_drag
                dy = y_pos - self.y_drag
                if dy < 300 and dx < 300:
                    # self.logger.debug(str(dx) + " : " + str(dy))
                    self.goompy.move(-dx, -dy)
                    self.x_drag = x_pos
                    self.y_drag = y_pos
                else:
                    self.x_drag = event.pos().x()
                    self.y_drag = event.pos().y()
        else:
            self.x_drag = event.pos().x()
            self.y_drag = event.pos().y()

    # noinspection PyUnresolvedReferences
    def connect_signals_to_slots__slot(self):
        self.main_window.kill_threads_signal.connect(self.on_kill_threads__slot)
        self.update_map_label_signal.connect(self.__on_map_label_update_requested__slot)
        self.main_window.miniboard_class.data_gps_position.connect(self.on_gps_coordinates_updated__slot)
        self.main_window.miniboard_class.data_gps_track.connect(self.on_gps_track_updated__slot)

        self.map_label.mouseReleaseEvent = self.__mouse_released_on_map_event
        self.map_label.mouseMoveEvent = self.__mouse_moved_on_map_label
        self.map_label.wheelEvent = self.__mouse_scrolled_on_map

    def on_kill_threads__slot(self):
        self.run_thread_flag = False

    @staticmethod
    def clamp(n, minn, maxn):
        return max(min(maxn, n), minn)
