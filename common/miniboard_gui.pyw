#!/usr/bin/env python
Help = """Miniboard GUI Test Tool
From a SPECIFICATION.md file (supplied as the first command-line
argument), this program provides a GUI that can be used to get/set
all registers on the miniboard."""

import sys
import docparse #TODO: Add a try/catch and pop-up dialog.
from PyQt4.QtGui import *
from PyQt4.QtCore import *
import struct

#TODO: Packet construction/parsing
#TODO: Set limits of spinboxes based on var size
#TODO: Parse options to create drop-down
#TODO: Handle variable-length stuff

SerialPort = "/dev/ttyACM0"

class MiniboardIO():
	"""Handles reading and writing from the miniboard."""
	path = SerialPort
	baud = 9600
	def write(self, packet_contents):
		"""Write a packet to the miniboard, inserting start, end, and escape bytes
		   where necessary. """
		print "Write: ",
		packet_contents = "".join(packet_contents)
		packet_contents = packet_contents.replace(chr(0x02), "".join([chr(0x02), chr(0x02)]))
		packet_contents = packet_contents.replace(chr(0x01), "".join([chr(0x02), chr(0x01)]))
		packet_contents = packet_contents.replace(chr(0x03), "".join([chr(0x02), chr(0x03)]))
		packet_contents = chr(0x01) + packet_contents + chr(0x03)
		for c in packet_contents:
			print "0x%02X, "%ord(c),
		print "\n",
	
	def read(self):
		"""Read a packet from the miniboard and return the contents (with
		   start, end, and escape bytes removed.
		   If the miniboard takes too long to reply, or if an error occurs,
		   an exception will be raised. TODO: Exception type."""
		return ""
	
	def port_info(self):
		"""Return a tuple (serial_port_path, baud_rate)."""
		return (self.path, self.baud)

class RegisterController():
	"""Handles reading/writing a single register and controlling that
	   register's UI widgets."""
	def setup(self, reg_dict, widgets, io):
		"""arg_row is the register specification from extract_table().
		   widgets is a list of the widgets corresponding to each register,
		    in the order they appear in the reg_dict.
		   io is the MiniboardIO class to use."""
		self.reg = reg_dict
		self.widgets = widgets
		self.io = io
		self.fmtcodes = {"u8":"<B", "i8":"<b", "u16":"<H", "i16":"<h", "u32":"<I", "i32":"<i"}
	def writefunc(self):
		"""Return a function (due to pyqt weirdness) for writing this register to the miniboard."""
		def func():
			print "Writing reg 0x%02X: "%self.reg["code"],
			p = [chr(self.reg["code"] | 0x00)]
			for a,w in zip(self.reg["argument"], self.widgets):
				if a[0] != "*":
					value = w.value()
					p += list(struct.pack(self.fmtcodes[a[0]], value))
				else:
					pass
					#TODO
			self.io.write(p)
		return func
		
	def readfunc(self):
		"""Return a function (due to pyqt weirdness) for reading this register from the miniboard."""
		def func():
			print "Reading reg 0x%02X: "%self.reg["code"]
			p = [chr(self.reg["code"] | 0x80)]
			self.io.write(p)
			#TODO: Read and parse
		return func
		
	def update(self):
		"""Re-process widget data after editing."""
		
		
def test():
	print "Test"

def horizontalLine():
	#From http://stackoverflow.com/questions/5671354/how-to-programmatically-make-a-horizontal-line-in-qt
	line = QFrame()
	line.setFrameShape(QFrame.HLine)
	line.setFrameShadow(QFrame.Sunken)
	return line

def setup(window, spec_table, io):
	ww = QWidget(window)
	flayout = QFormLayout()
	vlayout = QVBoxLayout()
	read_list = []
	write_list = []
	for r in spec_table:
		label = QLabel(r["name"])
		hl = QHBoxLayout()
		annotated_args = []
		control_widgets = []
		rc = RegisterController()
		for a,i in zip(r["argument"],range(0, len(r["argument"]))):
			a = [a[0], a[1], False] #Type, Name, Is_Size_Controller
			annotated_args.append(a)
			if a[0] == "*":
				annotated_args[i-1][2] = True
				
		for a in annotated_args:
			vl = QVBoxLayout()
			if a[0] == "*":
				widget = QLineEdit()
			else:
				widget = QSpinBox()
				if a[2]:
					widget.setEnabled(False)
			control_widgets.append(widget)
			subtitle = QLabel(a[1])
			subtitle.setFont(QFont("", 8))
			vl.addWidget(widget)
			vl.addWidget(subtitle)

			hl.addLayout(vl)
		rc.setup(r, control_widgets, io)
		rbtn = QToolButton()
		readfunc = rc.readfunc()
		writefunc = rc.writefunc()
		rbtn.setText("Read")
		rbtn.pressed.connect(readfunc)
		wbtn = QToolButton()
		wbtn.setText("Write")
		wbtn.pressed.connect(writefunc)
		if "r" not in r["rw"]:
			rbtn.setEnabled(False)
		else:
			read_list.append(readfunc)
		if "w" not in r["rw"]:
			wbtn.setEnabled(False)
		else:
			write_list.append(writefunc)
		bvl = QVBoxLayout()
		hbvl = QHBoxLayout()
		hbvl.addWidget(rbtn)
		hbvl.addWidget(wbtn)
		bvl.addLayout(hbvl)
		bvl.addSpacing(21)
		hl.addStretch(1)
		hl.addLayout(bvl)
		flayout.addRow(label, hl)
	gh = QHBoxLayout()
	gh.addWidget(QLabel("Port: %s    Baud: %d"%io.port_info()))
	gh.addStretch(1)	
	
	def read_all():
		for f in read_list:
			f()
			
	def write_all():
		for f in write_list:
			f()
	rbtn = QToolButton()
	rbtn.setText("Read All")
	rbtn.pressed.connect(read_all)
	wbtn = QToolButton()
	wbtn.setText("Write All")
	wbtn.pressed.connect(write_all)
	gh.addWidget(rbtn)
	gh.addWidget(wbtn)
	
	vlayout.addLayout(gh)
	vlayout.addWidget(horizontalLine())
	vlayout.addLayout(flayout)
	ww.setLayout(vlayout)
	window.setCentralWidget(ww)

def get_specpath():
	"""Return the path to the specification file,
	   using a hardcoded default, command-line argument, pop-up dialog, etc."""
	return "SPECIFICATION.md"

def main():
	app = QApplication(sys.argv)
	w = QMainWindow()
	spec = None
	with open(get_specpath(), "r") as f:
		spec = docparse.extract_table(f.read());
	io = MiniboardIO()
	setup(w, spec, io)
	
	w.show()
	sys.exit(app.exec_())
	
main()