#!/usr/bin/env python
Help = """Miniboard GUI Test Tool
From a SPECIFICATION.md file (supplied as the first command-line
argument), this program provides a GUI that can be used to get/set
all registers on the miniboard."""

import sys
import docparse #TODO: Add a try/catch and pop-up dialog.
from PyQt4.QtGui import *
from PyQt4.QtCore import *

def setup(window, spec_table, send_func, recv_fund):
	ww = QWidget(window)
	wlayout = QFormLayout()
	for r in spec_table:
		label = QLabel(r["name"])
		hl = QHBoxLayout()
		annotated_args = []
		for a,i in zip(r["argument"],range(0, len(r["argument"]))):
			print a
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
			subtitle = QLabel(a[1])
			subtitle.setFont(QFont("", 8))
			vl.addWidget(widget)
			vl.addWidget(subtitle)

			hl.addLayout(vl)
			#Generate read/write/update functions
			#Connect read/write/update functions
		rbtn = QToolButton()
		rbtn.setText("Read")
		wbtn = QToolButton()
		wbtn.setText("Write")
		if "r" not in r["rw"]:
			rbtn.setEnabled(False)
		if "w" not in r["rw"]:
			wbtn.setEnabled(False)	
		bvl = QVBoxLayout()
		hbvl = QHBoxLayout()
		hbvl.addWidget(rbtn)
		hbvl.addWidget(wbtn)
		bvl.addLayout(hbvl)
		bvl.addSpacing(15)
		hl.addStretch(1)
		hl.addLayout(bvl)
		wlayout.addRow(label, hl)
	wlayout.addRow(QLabel("Test"), QPushButton("Hello"))
	wlayout.addRow(QLabel("Test"), QPushButton("Hello"))
	wlayout.addRow(QLabel("Test"), QPushButton("Hello"))
	ww.setLayout(wlayout)
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
	
	setup(w, spec, None, None)
	
	w.show()
	sys.exit(app.exec_())
	
main()