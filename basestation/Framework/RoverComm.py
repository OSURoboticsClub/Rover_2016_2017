#!/usr/bin/env python3
"""
  This file contains the rover communication code.
"""
import sys
import os
import serial
from PyQt5 import QtCore, QtWidgets, QtGui
import logging
import struct
import signal
from io import StringIO,BytesIO
import enum

#Import docparse
docparsepath = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))) + "/common/"
sys.path.append(docparsepath)
import docparse
with open(docparsepath + "/SPECIFICATION.md") as p:
	RoverCmdTable = docparse.extract_table(p.read())

#Create read/write functions
def create_funcs(module_vars, cmd_table):
	"""Create functions of the form
	     read_<command_name>(signal)
	     write_<command_name>(signal, *args)
	   such as
	     read_pause(signal)
	     write_pause(signal, pause_state)
	   in the current module."""
	log = logging.getLogger("Miniboard Command Functions")
	def build_read_func(cmd):
		"""Return a function that takes a signal (connected to MiniboardIO's send())
		   and emits a string of packet data."""
		def f(signal):
			packet_data = [cmd["code"] | 0x80]
			signal.emit(packet_data)
		return f
	
	def build_write_func(cmd):
		"""Return a function that takes a signal (connected to MiniboardIO's send())
		   and emits a string of packet data."""
		fmtcodes = {"u8":"<B", "i8":"<b", "u16":"<H", "i16":"<h", "u32":"<I", "i32":"<i", "i64":"<Q", "i64":"<q"}
		def f(signal, *args):
			if len(args) != len(cmd["argument"]):
				log.error("Error calling write func for command %s: expected %d arguments, got %d"%(cmd["name"], len(args),len(cmd["arguments"])))
				return
			packet_data = [cmd["code"]]
			for a,v in zip(cmd["argument"], args):
				if a[0] == "*":
					packet_data += list(v)
				else:
					packet_data += list(struct.pack(fmtcodes[a[0]], v))
			signal.emit(packet_data)
		return f
	
	for c in cmd_table:
		module_vars["read_"+docparse.cannon_name(c["name"])] = build_read_func(c)
		module_vars["write_"+docparse.cannon_name(c["name"])] = build_write_func(c)
		
create_funcs(vars(), RoverCmdTable)

class MiniboardIO(QtCore.QThread):
	"""Handles reading and writing from the miniboard."""
	path = "/dev/ttyUSB0"
	baud = 9600
	cmd = RoverCmdTable
	on_kill_threads__slot = QtCore.pyqtSignal()
	def __init__(self, main_window):
		self.make_signals()
		super().__init__()
		self.main_window = main_window
		self.logger = logging.getLogger("MiniboardIO")
		os.system("stty -F %s -hupcl"%self.path)
		self.tty = serial.Serial(port=self.path,
		                         baudrate=self.baud,
	                             parity=serial.PARITY_NONE,
	                             stopbits=serial.STOPBITS_ONE,
	                             bytesize=serial.EIGHTBITS,
	                             timeout=0.005)
		self.reply = ""
		self.run_thread_flag = True
		self.connect_signals_to_slots()
		self.queue = []
		
	def calc_crc(self, body):
		remainder = 0xFFFF
		for i in range(0, len(body)):
			remainder ^= body[i] << 8
			remainder &= 0xFFFF
			for bit in reversed(range(1,9)):
				if remainder & 0x8000:
					remainder = (remainder << 1) ^ 0x1021
					remainder &= 0xFFFF
				else:
					remainder <<= 1
					remainder &= 0xFFFF
		return remainder
	
	def append(self, body_list):
		"""Add a command to the queue."""
		self.queue.append(body_list)
	
	def __send(self, body_list):
		"""Given a packet body list, turn it into a packet and send it to the rover."""
		packet_contents = body_list
		plen = len(packet_contents) + 2
		crc = self.calc_crc(packet_contents)
		packet_contents = [0x01] + [plen] + [crc & 0xFF] + [crc >> 8] + packet_contents
		self.tty.write(packet_contents)
		
	def make_signals(self):
		"""Create signals for received data from the rover.
		   Each signal has the name data_<canonical command name>,
		   such as data_battery_voltage.
		   Each signal emits a dictionary containing string keys of the
		   command's argument names."""
		for c in self.cmd:
			signame = "data_"+docparse.cannon_name(c["name"])
			setattr(self, signame, QtCore.pyqtSignal([dict]))
	 
	def run(self):
		"""Read from the serial port, recognize the command, and emit a signal."""
		reply = []
		fmtcodes = {"u8":"<B", "i8":"<b", "u16":"<H", "i16":"<h", "u32":"<I", "i32":"<i", "i64":"<Q", "i64":"<q"}
		while self.run_thread_flag:
			reply += list(self.tty.read(size=10000000))
			#process packet
			while len(reply) > 0:
				if reply[0] != 0x01:
					reply = reply[1:]
				else:
					break
			if len(reply) > 0: #Got start byte
				if len(reply) >= 5: #Got minimum complete packet
					if len(reply) >= (reply[1] + 2): #Got enough bytes for this packet
						if self.calc_crc(reply[4:]) == struct.unpack("<H", bytes(reply[2:4]))[0]: #CRC OK
							print("CRC OK")
							for c in reply:
								print("0x%02X, "%c, end="")
							print ("\n")
							if reply[0] & 0x80:
								print("read")
						reply = reply[(reply[1] + 2):]
			self.msleep(10)
	
	def connect_signals_to_slots(self):
		self.main_window.kill_threads_signal.connect(self.on_kill_threads__slot)
	
	def on_kill_threads__slot(self):
		self.run_thread_flag = False
        
class DemoThread(QtCore.QThread):
	on_kill_threads__slot = QtCore.pyqtSignal()
	send_packet = QtCore.pyqtSignal(list)
	def __init__(self, main_window):
		super().__init__()
		self.main_window = main_window
		self.connect_signals_to_slots()
	def run(self):
		while True:
			write_swerve_drive_state(self.send_packet, 3)
			self.msleep(1000)
			read_swerve_drive_state(self.send_packet)
			self.msleep(1000)

	def connect_signals_to_slots(self):
		self.main_window.kill_threads_signal.connect(self.on_kill_threads__slot)
		self.send_packet.connect(self.main_window.m.append)
        
class DemoWindow(QtWidgets.QMainWindow):
	kill_threads_signal = QtCore.pyqtSignal()
	def __init__(self):
		super().__init__()
		self.m = MiniboardIO(self)
		self.d = DemoThread(self)
		self.m.start()
		self.d.start()
		
if __name__ == "__main__":
	signal.signal(signal.SIGINT, signal.SIG_DFL)  # This allows the keyboard interrupt kill to work  properly
	application = QtWidgets.QApplication(sys.argv)  # Create the base qt gui application
	app_window = DemoWindow()  # Make a window in this application
	app_window.setWindowTitle("Demo Demo")  # Sets the window title
	app_window.show()  # Show the window in the application
	application.exec_()  # Execute launching of the application
	
