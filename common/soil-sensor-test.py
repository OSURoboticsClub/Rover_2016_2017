#!/usr/bin/env python
import sys
import math
import struct
import serial
import signal
import os
import time
import struct 

SerialPath="/dev/ttyUSB0"

class SoilSensor():
	def __init__(self, path):
		self.path = path
		self.__tty = serial.Serial(port=self.path,
		                           baudrate=9600,
	                               parity=serial.PARITY_NONE,
	                               stopbits=serial.STOPBITS_ONE,
	                               bytesize=serial.EIGHTBITS,
	                               timeout=0.2)
	def set_mode_rx(self):
		"""Set the transceiver to receive mode."""
		#Set DTR for receive mode, clear for transmit
		self.__tty.setDTR(True)
		
	def set_mode_tx(self):
		"""Set the transceiver to transmit mode."""
		self.__tty.setDTR(False)
		
	def send_command(self, addr_str, command_str):
		"""Send a command to the soil sensor."""
		self.set_mode_tx()
		time.sleep(.04)
		self.__tty.write(addr_str + command_str + "\r")
		self.__tty.flush()
		time.sleep(.05)
		self.__tty.write("\n")
		time.sleep(.005)
		self.set_mode_rx()
		reply = self.__tty.read(size=10000000)
		return reply
	
	def set_data(self, addr_str, command_str, data_str):
		"""Set data in the soil sensor."""
		self.set_mode_tx()
		time.sleep(.04)
		self.__tty.write(addr_str + command_str + "=" + data_str + "\r\n")
		self.__tty.flush()
		time.sleep(.05)
		self.__tty.write("\n")
		time.sleep(.005)
		self.set_mode_rx()
		reply = self.__tty.read(size=10000000)
		return reply
		
	def get_data(self, addr_str, command_str):
		"""Get data from the sensor, returning the data.
		   command_str is the two-character string."""
		self.set_mode_tx()
		time.sleep(.04)
		self.__tty.write(addr_str + command_str + "=?" + "\r")
		self.__tty.flush()
		time.sleep(.05)
		self.__tty.write("\n")
		time.sleep(.005)
		self.set_mode_rx()
		reply = self.__tty.read(size=10000000)
		return reply
	
	def get_measurement(self, addr_str):
		"""Take and return a soil measurement."""
		addr = addr_str
		self.send_command(addr, "TR")
		time.sleep(1)
		data = self.send_command(addr, "T3")
		print data
		data = data[3:]
		data = data.split(",")
		print "Raw Values (reading set 3):", data
		TempC = float(data[0])
		Moisture = float(data[2])
		Cond = float(data[4])
		PermR = float(data[6])
		PermI = float(data[8])
		Salinity = Cond * 6.4
		return {"TempC":TempC, "Moisture":Moisture, "Salinity":Salinity}
def main():
	s = SoilSensor(SerialPath)
	addr = s.get_data("///", "SN")[0:3]
	s.set_data(addr, "PE", "1")
	time.sleep(1)
	while True:
		print s.get_measurement(addr)
		print ""
		time.sleep(10)
		
	
if __name__ == "__main__":
	main()