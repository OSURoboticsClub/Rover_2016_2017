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
			packet_data = list(cmd["code"] | 0x80)
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
	def __init__(self):
		self.make_signals()
		super().__init__()
		self.logger = logging.getLogger("MiniboardIO")
		os.system("stty -F %s -hupcl"%self.path)
		self.tty = serial.Serial(port=self.path,
		                         baudrate=self.baud,
	                             parity=serial.PARITY_NONE,
	                             stopbits=serial.STOPBITS_ONE,
	                             bytesize=serial.EIGHTBITS,
	                             timeout=0.005)
		self.reply = ""
	
	def send(self, databytes_list):
		"""Given a packet data string, turn it into a packet and send it to the rover."""
	
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
		reply = ""
		while True:
			reply.append(self.tty.read(size=10000000))
			#process packet
			self.msleep(10)

if __name__ == "__main__":
	#print(RoverCmdTable)
	m = MiniboardIO()
	write_battery_voltage(4, 3)
	#print(dir())
