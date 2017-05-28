#!/usr/bin/env python
Help = """Miniboard Test-Drive
From a SPECIFICATION.md file (supplied as the first command-line
argument), this program provides takes in joystick input and drives
the rover."""
import sys
import math
import docparse #TODO: Add a try/catch and pop-up dialog.
from PyQt4.QtGui import *
from PyQt4.QtCore import *
import struct
import serial
import signal
import os
import pygame
import time
import struct

signal.signal(signal.SIGINT, signal.SIG_DFL) #Make Ctrl-C quit the program

SerialPortPath = "/dev/ttyUSB0"

class MiniboardIO():
	"""Handles reading and writing from the miniboard."""
	path = SerialPortPath
	baud = 2400
	def __init__(self):
		os.system("stty -F %s -hupcl"%self.path)
		self.__tty = serial.Serial(port=self.path,
		                           baudrate=self.baud,
	                               parity=serial.PARITY_NONE,
	                               stopbits=serial.STOPBITS_ONE,
	                               bytesize=serial.EIGHTBITS,
	                               timeout=0.5)
	def calc_crc(self, body):
		body = [ord(b) for b in body]
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
	
	#DON'T USE DIRECTLY RIGHT NOW! Use writeread()
	def write(self, packet_contents):
		"""Write a packet to the miniboard, inserting the header first."""
		print "  send: ",
		packet_contents = "".join(packet_contents)
		plen = len(packet_contents) + 2
		crc = self.calc_crc(packet_contents)
		packet_contents = chr(0x01) + chr(plen)  + chr(crc & 0xFF) + chr(crc >> 8) + packet_contents
		for c in packet_contents:
			print "0x%02X, "%ord(c),
		print "\n",
		self.__tty.write(packet_contents)
	
	#DON'T USE DIRECTLY RIGHT NOW! Use writeread()
	def read(self):
		"""Read a packet from the miniboard and return the contents
		   (with header removed).
		   If the miniboard takes too long to reply, or if an error occurs,
		   an exception will be raised. TODO: Exception type."""
		print "  recv: ",
		reply = self.__tty.read(size=10000000)
		for c in reply:
			print "0x%02X, "%ord(c),
		print "\n",
		if len(reply) == 0:
			print "Error: no reply."
			return ""
		if reply[0] != chr(0x01):
			print "Error: missing start byte."
			return ""
		if len(reply) < 5:
			print "Error: no enough bytes."
			return ""
		if (len(reply) - 2) != ord(reply[1]):
			print "Error: length mismatch (header says %d, packet is %d)"%(ord(reply[1]), len(reply))
			return ""
		if self.calc_crc(reply[4:]) != (ord(reply[3])<<8 | ord(reply[2])):
			print "Error: CRC mismatch."
			return ""
		return "".join(reply[4:])
	
	def writeread(self, packet_contents):
		"""Write a packet to the miniboard and return the reply."""
		#self.__tty = serial.Serial(port=self.path,
		                           #baudrate=self.baud,
	                               #parity=serial.PARITY_NONE,
	                               #stopbits=serial.STOPBITS_ONE,
	                               #bytesize=serial.EIGHTBITS,
	                               #timeout=0.1)
		self.write(packet_contents)
		reply = self.read()
		#self.__tty.close()
		return reply
	
	def port_info(self):
		"""Return a tuple (serial_port_path, baud_rate)."""
		return (self.path, self.baud)
	
def horizontalLine():
	#From http://stackoverflow.com/questions/5671354/how-to-programmatically-make-a-horizontal-line-in-qt
	line = QFrame()
	line.setFrameShape(QFrame.HLine)
	line.setFrameShadow(QFrame.Sunken)
	return line

def setup(window, spec_table, io):
	ww = QWidget(window)
	window.setWindowTitle("Miniboard GUI Test Drive")
	
	
def get_specpath():
	"""Return the path to the specification file,
	   using a hardcoded default, command-line argument, pop-up dialog, etc."""
	return "SPECIFICATION.md"

#def main():
	#app = QApplication(sys.argv)
	#w = QMainWindow()
	#spec = None
	#with open(get_specpath(), "r") as f:
		#spec = docparse.extract_table(f.read());
	#io = MiniboardIO()
	#setup(w, spec, io)
	
	#w.show()
	#sys.exit(app.exec_())
	
def set_drive_power(io, left, right):
	"""Left and right are floats from -1 to 1."""
	l = int(127 * left)
	r = int(127 * right)
	s = "".join([struct.pack('b', i) for i in [0x10, l, l, l, r, r, r]])
	io.writeread(s) 
	
def set_swerve_mode(io, mode):
	if mode == 1 or mode == 2:
		print "Swerve mode: ", mode
	s = "".join([struct.pack('b', i) for i in [0x11, mode]])
	io.writeread(s) 
	
def get_joystick(joystick):
	"""Return a tuple (l,r) of motor powers from the joystick."""
	x = joystick.get_axis(0)
	y = joystick.get_axis(1)
	#print x, y
	#theta = math.atan2(y, x)
	#rad = math.sqrt(x**2 + y**2)
	#print theta, rad
	#if(rad > 1):
		#rad = 1
	#l=x 
	#r=-y
	return (x,y)
	
def main():
	with open(get_specpath(), "r") as f:
		spec = docparse.extract_table(f.read());
	io = MiniboardIO()
	pygame.init()
	pygame.joystick.init()
	joysticks = [pygame.joystick.Joystick(x) for x in range(pygame.joystick.get_count())]
	j = joysticks[0]
	print j.get_name()
	j.init()
	oldl = 0
	oldr = 0
	while(True):
		pygame.init()
		pygame.event.get()
		l,r = get_joystick(j)
		#print "Motor power L=", l, "R=", r
		if(oldl != l or oldr != r):
			set_drive_power(io, l, r)
		oldl = l
		oldr = r
		if int(j.get_button(6)) == 1:
			set_swerve_mode(io, 2)
		elif int(j.get_button(7)) == 1: 
			set_swerve_mode(io, 1)
		else:
			set_swerve_mode(io, 0)
		time.sleep(.02)
	
main()
