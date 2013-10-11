# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'editor_quantity.ui'
#
# Created: Fri Oct 11 01:38:30 2013
#      by: PyQt4 UI code generator 4.9.3
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

class Ui_EditorQuantity(object):
    def setupUi(self, EditorQuantity):
        EditorQuantity.setObjectName(_fromUtf8("EditorQuantity"))
        EditorQuantity.resize(250, 300)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.MinimumExpanding, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(EditorQuantity.sizePolicy().hasHeightForWidth())
        EditorQuantity.setSizePolicy(sizePolicy)
        EditorQuantity.setMinimumSize(QtCore.QSize(250, 200))
        EditorQuantity.setWindowTitle(_fromUtf8(""))
        EditorQuantity.setFrameShape(QtGui.QFrame.NoFrame)
        EditorQuantity.setFrameShadow(QtGui.QFrame.Raised)
        self.verticalLayout = QtGui.QVBoxLayout(EditorQuantity)
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName(_fromUtf8("horizontalLayout"))
        self.label = QtGui.QLabel(EditorQuantity)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.label.sizePolicy().hasHeightForWidth())
        self.label.setSizePolicy(sizePolicy)
        self.label.setMinimumSize(QtCore.QSize(50, 0))
        self.label.setObjectName(_fromUtf8("label"))
        self.horizontalLayout.addWidget(self.label)
        self.valueEdit = QtGui.QLineEdit(EditorQuantity)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(1)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.valueEdit.sizePolicy().hasHeightForWidth())
        self.valueEdit.setSizePolicy(sizePolicy)
        self.valueEdit.setMinimumSize(QtCore.QSize(100, 0))
        self.valueEdit.setObjectName(_fromUtf8("valueEdit"))
        self.horizontalLayout.addWidget(self.valueEdit)
        self.verticalLayout.addLayout(self.horizontalLayout)
        self.horizontalLayout_3 = QtGui.QHBoxLayout()
        self.horizontalLayout_3.setObjectName(_fromUtf8("horizontalLayout_3"))
        self.label_2 = QtGui.QLabel(EditorQuantity)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.label_2.sizePolicy().hasHeightForWidth())
        self.label_2.setSizePolicy(sizePolicy)
        self.label_2.setMinimumSize(QtCore.QSize(50, 0))
        self.label_2.setObjectName(_fromUtf8("label_2"))
        self.horizontalLayout_3.addWidget(self.label_2)
        self.unitsEdit = QtGui.QLineEdit(EditorQuantity)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(1)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.unitsEdit.sizePolicy().hasHeightForWidth())
        self.unitsEdit.setSizePolicy(sizePolicy)
        self.unitsEdit.setMinimumSize(QtCore.QSize(100, 0))
        self.unitsEdit.setObjectName(_fromUtf8("unitsEdit"))
        self.horizontalLayout_3.addWidget(self.unitsEdit)
        self.verticalLayout.addLayout(self.horizontalLayout_3)
        self.horizontalLayout_2 = QtGui.QHBoxLayout()
        self.horizontalLayout_2.setObjectName(_fromUtf8("horizontalLayout_2"))
        spacerItem = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_2.addItem(spacerItem)
        self.updateButton = QtGui.QPushButton(EditorQuantity)
        self.updateButton.setAutoDefault(True)
        self.updateButton.setDefault(True)
        self.updateButton.setObjectName(_fromUtf8("updateButton"))
        self.horizontalLayout_2.addWidget(self.updateButton)
        self.verticalLayout.addLayout(self.horizontalLayout_2)
        spacerItem1 = QtGui.QSpacerItem(20, 40, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        self.verticalLayout.addItem(spacerItem1)

        self.retranslateUi(EditorQuantity)
        QtCore.QMetaObject.connectSlotsByName(EditorQuantity)

    def retranslateUi(self, EditorQuantity):
        self.label.setText(QtGui.QApplication.translate("EditorQuantity", "Value", None, QtGui.QApplication.UnicodeUTF8))
        self.valueEdit.setToolTip(QtGui.QApplication.translate("EditorQuantity", "<html><head/><body><p>Insert the value (float).The format can be scientific. Examples:</p><pre style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:\'Courier New,courier\';\">1.02</span></pre><pre style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:\'Courier New,courier\';\">5.032E-3</span></pre></body></html>", None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setText(QtGui.QApplication.translate("EditorQuantity", "Units", None, QtGui.QApplication.UnicodeUTF8))
        self.unitsEdit.setToolTip(QtGui.QApplication.translate("EditorQuantity", "<html><head/><body><p>Insert an expression using daetools.pyUnits.unit objects and mathematical operators *, / and **. All base (m, kg, s, cd, A, K, mol) and derived (g, t, rad, sr, min, hour, day, Hz, N, J, W, C, Ohm, V, F, T, H, S, Wb, Pa, P, St, Bq, Gy, Sv, lx, lm, kat, knot, bar, b, Ci, R, rd, rem) SI units are allowed including the common prefixes (Y, Z, E, P, T, G, M, k, h, da, d, c, m, u, n, p, f, a, z, y). The expression should be a valid Python code. Examples: </p><p>km/kg (kilometres per kilogram)<br/>um / ns**2 (micrometres per nanoseconds squared) <br/>kJ / (kg*K) (kiloJoules per kilogram and Kelvin)<br/></p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
        self.updateButton.setToolTip(QtGui.QApplication.translate("EditorQuantity", "<html><head/><body><p>Update the value and units.</p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
        self.updateButton.setText(QtGui.QApplication.translate("EditorQuantity", "Update", None, QtGui.QApplication.UnicodeUTF8))

