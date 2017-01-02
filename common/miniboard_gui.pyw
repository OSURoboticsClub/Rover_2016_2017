#!/usr/bin/env python
Help = """Miniboard GUI Test Tool
From a SPECIFICATION.md file (supplied as the first command-line
argument), this program provides a GUI that can be used to get/set
all registers on the miniboard."""

import sys
import math
import docparse #TODO: Add a try/catch and pop-up dialog.
from PyQt4.QtGui import *
from PyQt4.QtCore import *
import struct
import serial
import signal
import os

signal.signal(signal.SIGINT, signal.SIG_DFL) #Make Ctrl-C quit the program

#TODO: Check lengths everywhere, for validation
#TODO: Add serial port selection
#TODO: Add big pause/unpause buttons
#TODO: Add auto-updating (1Hz?)
#TODO: Add scroll bar
SerialPortPath = "/dev/ttyACM0"

class MiniboardIO():
	"""Handles reading and writing from the miniboard."""
	path = SerialPortPath
	baud = 9600
	def __init__(self):
		os.system("stty -F %s -hupcl"%self.path)
	
	#DON'T USE DIRECTLY RIGHT NOW! Use writeread()
	def write(self, packet_contents):
		"""Write a packet to the miniboard, inserting start, end, and escape bytes
		   where necessary. """
		print "  send: ",
		packet_contents = "".join(packet_contents)
		packet_contents = packet_contents.replace(chr(0x02), "".join([chr(0x02), chr(0x02)]))
		packet_contents = packet_contents.replace(chr(0x01), "".join([chr(0x02), chr(0x01)]))
		packet_contents = packet_contents.replace(chr(0x03), "".join([chr(0x02), chr(0x03)]))
		packet_contents = chr(0x01) + packet_contents + chr(0x03)
		for c in packet_contents:
			print "0x%02X, "%ord(c),
		print "\n",
		self.__tty.write(packet_contents)
	
	#DON'T USE DIRECTLY RIGHT NOW! Use writeread()
	def read(self):
		"""Read a packet from the miniboard and return the contents (with
		   start, end, and escape bytes removed.
		   If the miniboard takes too long to reply, or if an error occurs,
		   an exception will be raised. TODO: Exception type."""
		print "  recv: ",
		reply = self.__tty.read(size=10000000)
		for c in reply:
			print "0x%02X, "%ord(c),
		print "\n",
		if reply[0] != chr(0x01):
			print "Error: missing start byte."
			return ""
		if reply[-1] != chr(0x03):
			print "Error: missing end byte."
			return ""
		reply = reply[1:-1]
		extracted = []
		esc = False
		for c in reply:
			if c == chr(0x02) and not esc:
				esc = True
				continue
			extracted.append(c)
			esc = False
		return "".join(extracted)
	
	def writeread(self, packet_contents):
		"""Write a packet to the miniboard and return the reply."""
		self.__tty = serial.Serial(port=self.path,
		                           baudrate=self.baud,
	                               parity=serial.PARITY_NONE,
	                               stopbits=serial.STOPBITS_ONE,
	                               bytesize=serial.EIGHTBITS,
	                               timeout=0.1)
		self.write(packet_contents)
		reply = self.read()
		self.__tty.close()
		return reply
	
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
		self.fmtcodes = {"u8":"<B", "i8":"<b", "u16":"<H", "i16":"<h", "u32":"<I", "i32":"<i", "i64":"<Q", "i64":"<q"}
	def writefunc(self):
		"""Return a function (due to pyqt weirdness) for writing this register to the miniboard."""
		def func():
			print "Write reg 0x%02X (%s): "%(self.reg["code"], self.reg["name"])
			p = [chr(self.reg["code"] | 0x00)]
			for a,w,i in zip(self.reg["argument"], self.widgets, range(0, len(self.widgets))):
				if a[0] == "*":
					self.widgets[i-1].setValue(len(str(w.text())))
			for a,w,i in zip(self.reg["argument"], self.widgets, range(0, len(self.widgets))):
				if a[0] != "*":
					value = w.value()
					p += list(struct.pack(self.fmtcodes[a[0]], value))
				else:
					p += str(w.text())
			reply = self.io.writeread(p)
		return func
		
	def readfunc(self):
		"""Return a function (due to pyqt weirdness) for reading this register from the miniboard."""
		def func():
			print "Read reg 0x%02X (%s): "%(self.reg["code"], self.reg["name"])
			p = [chr(self.reg["code"] | 0x80)]
			reply = self.io.writeread(p)
			if reply[0] != p[0]:
				print "Error: incorrect command code. Expected 0x%02X, got 0x%02X"%(ord(p[0]), ord(reply[0]))
			b = 1
			vs = []
			for a,w,i in zip(self.reg["argument"], self.widgets, range(0, len(self.widgets))):
				if a[0] != "*":
					s = struct.calcsize(self.fmtcodes[a[0]])
					value = struct.unpack(self.fmtcodes[a[0]], reply[b:b+s])[0]
					vs.append(value)
					b += s
				else:
					s = vs[i-1]
					value = reply[b:b+s]
					b+=s
				try:
					w.setValue(value)
				except:
					w.setText(value)
		return func
		
	def update(self):
		"""Re-process widget data after editing."""

def horizontalLine():
	#From http://stackoverflow.com/questions/5671354/how-to-programmatically-make-a-horizontal-line-in-qt
	line = QFrame()
	line.setFrameShape(QFrame.HLine)
	line.setFrameShadow(QFrame.Sunken)
	return line

def argtype_minval(argtype):
	"""Return the minimum value for a numerical argument type."""
	if argtype[0] == "u":
		return 0
	elif argtype[0]  == "i":
		return -(2**(int(argtype[1:])-1))
	else:
		return 0
	
def argtype_maxval(argtype):
	"""Return the minimum value for a numerical argument type."""
	if argtype[0] == "u":
		return 2**(int(argtype[1:])) - 1
	elif argtype[0] == "i":
		return 2**(int(argtype[1:])-1) - 1
	else:
		return 0

def argtype_minwidth(argtype):
	"""Return the minimum width of a spinbox for the given argument type."""
	m = 2.5 + math.ceil(math.log10(argtype_maxval(argtype)))
	return m * 9

class BigIntSpinBox(QAbstractSpinBox):
    """From http://stackoverflow.com/questions/26841036/pyqt4-spinbox-with-64bit-integers"""
    def __init__(self, parent=None):
        super(BigIntSpinBox, self).__init__(parent)

        self._singleStep = 1
        self._minimum = -18446744073709551616
        self._maximum = 18446744073709551615

        self.lineEdit = QLineEdit(self)

        rx = QRegExp("[1-9]\\d{0,20}")
        validator = QRegExpValidator(rx, self)

        self.lineEdit.setValidator(validator)
        self.setLineEdit(self.lineEdit)

    def value(self):
        try:
            return int(self.lineEdit.text())
        except:
            raise
            return 0

    def setValue(self, value):
        if self._valueInRange(value):
            self.lineEdit.setText(str(value))

    def stepBy(self, steps):
        self.setValue(self.value() + steps*self.singleStep())

    def stepEnabled(self):
        return self.StepUpEnabled | self.StepDownEnabled

    def setSingleStep(self, singleStep):
        assert isinstance(singleStep, int)
        # don't use negative values
        self._singleStep = abs(singleStep)

    def singleStep(self):
        return self._singleStep

    def minimum(self):
        return self._minimum

    def setMinimum(self, minimum):
        assert isinstance(minimum, int) or isinstance(minimum, long)
        self._minimum = minimum

    def maximum(self):
        return self._maximum

    def setMaximum(self, maximum):
        assert isinstance(maximum, int) or isinstance(maximum, long)
        self._maximum = maximum

    def _valueInRange(self, value):
        if value >= self.minimum() and value <= self.maximum():
            return True
        else:
            return False

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
				widget = BigIntSpinBox()
				if a[2] or "w" not in r["rw"]:
					widget.setEnabled(False)
				widget.setMinimum(argtype_minval(a[0]))
				widget.setMaximum(argtype_maxval(a[0]))
				widget.setMinimumSize(QSize(argtype_minwidth(a[0]), 0))
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
