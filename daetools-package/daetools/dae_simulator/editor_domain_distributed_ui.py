# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'editor_domain_distributed.ui'
#
# Created: Tue Oct  8 16:25:30 2013
#      by: PyQt4 UI code generator 4.9.3
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

class Ui_EditorDistributedDomain(object):
    def setupUi(self, EditorDistributedDomain):
        EditorDistributedDomain.setObjectName(_fromUtf8("EditorDistributedDomain"))
        EditorDistributedDomain.resize(250, 300)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.MinimumExpanding, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(EditorDistributedDomain.sizePolicy().hasHeightForWidth())
        EditorDistributedDomain.setSizePolicy(sizePolicy)
        EditorDistributedDomain.setMinimumSize(QtCore.QSize(250, 200))
        EditorDistributedDomain.setWindowTitle(_fromUtf8(""))
        EditorDistributedDomain.setFrameShape(QtGui.QFrame.NoFrame)
        EditorDistributedDomain.setFrameShadow(QtGui.QFrame.Raised)
        self.verticalLayout = QtGui.QVBoxLayout(EditorDistributedDomain)
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName(_fromUtf8("horizontalLayout"))
        self.label = QtGui.QLabel(EditorDistributedDomain)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.label.sizePolicy().hasHeightForWidth())
        self.label.setSizePolicy(sizePolicy)
        self.label.setMinimumSize(QtCore.QSize(100, 0))
        self.label.setObjectName(_fromUtf8("label"))
        self.horizontalLayout.addWidget(self.label)
        self.discrMethodEdit = QtGui.QLineEdit(EditorDistributedDomain)
        self.discrMethodEdit.setEnabled(False)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(1)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.discrMethodEdit.sizePolicy().hasHeightForWidth())
        self.discrMethodEdit.setSizePolicy(sizePolicy)
        self.discrMethodEdit.setMinimumSize(QtCore.QSize(100, 0))
        self.discrMethodEdit.setObjectName(_fromUtf8("discrMethodEdit"))
        self.horizontalLayout.addWidget(self.discrMethodEdit)
        self.verticalLayout.addLayout(self.horizontalLayout)
        self.horizontalLayout_3 = QtGui.QHBoxLayout()
        self.horizontalLayout_3.setObjectName(_fromUtf8("horizontalLayout_3"))
        self.label_2 = QtGui.QLabel(EditorDistributedDomain)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.label_2.sizePolicy().hasHeightForWidth())
        self.label_2.setSizePolicy(sizePolicy)
        self.label_2.setMinimumSize(QtCore.QSize(100, 0))
        self.label_2.setObjectName(_fromUtf8("label_2"))
        self.horizontalLayout_3.addWidget(self.label_2)
        self.orderEdit = QtGui.QLineEdit(EditorDistributedDomain)
        self.orderEdit.setEnabled(False)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(1)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.orderEdit.sizePolicy().hasHeightForWidth())
        self.orderEdit.setSizePolicy(sizePolicy)
        self.orderEdit.setMinimumSize(QtCore.QSize(100, 0))
        self.orderEdit.setObjectName(_fromUtf8("orderEdit"))
        self.horizontalLayout_3.addWidget(self.orderEdit)
        self.verticalLayout.addLayout(self.horizontalLayout_3)
        self.horizontalLayout_4 = QtGui.QHBoxLayout()
        self.horizontalLayout_4.setObjectName(_fromUtf8("horizontalLayout_4"))
        self.label_3 = QtGui.QLabel(EditorDistributedDomain)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.label_3.sizePolicy().hasHeightForWidth())
        self.label_3.setSizePolicy(sizePolicy)
        self.label_3.setMinimumSize(QtCore.QSize(100, 0))
        self.label_3.setObjectName(_fromUtf8("label_3"))
        self.horizontalLayout_4.addWidget(self.label_3)
        self.numberOfIntervalsEdit = QtGui.QLineEdit(EditorDistributedDomain)
        self.numberOfIntervalsEdit.setEnabled(False)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(1)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.numberOfIntervalsEdit.sizePolicy().hasHeightForWidth())
        self.numberOfIntervalsEdit.setSizePolicy(sizePolicy)
        self.numberOfIntervalsEdit.setMinimumSize(QtCore.QSize(100, 0))
        self.numberOfIntervalsEdit.setObjectName(_fromUtf8("numberOfIntervalsEdit"))
        self.horizontalLayout_4.addWidget(self.numberOfIntervalsEdit)
        self.verticalLayout.addLayout(self.horizontalLayout_4)
        self.horizontalLayout_5 = QtGui.QHBoxLayout()
        self.horizontalLayout_5.setObjectName(_fromUtf8("horizontalLayout_5"))
        self.label_4 = QtGui.QLabel(EditorDistributedDomain)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.label_4.sizePolicy().hasHeightForWidth())
        self.label_4.setSizePolicy(sizePolicy)
        self.label_4.setMinimumSize(QtCore.QSize(100, 0))
        self.label_4.setObjectName(_fromUtf8("label_4"))
        self.horizontalLayout_5.addWidget(self.label_4)
        self.lowerBoundEdit = QtGui.QLineEdit(EditorDistributedDomain)
        self.lowerBoundEdit.setEnabled(True)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(1)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.lowerBoundEdit.sizePolicy().hasHeightForWidth())
        self.lowerBoundEdit.setSizePolicy(sizePolicy)
        self.lowerBoundEdit.setMinimumSize(QtCore.QSize(100, 0))
        self.lowerBoundEdit.setObjectName(_fromUtf8("lowerBoundEdit"))
        self.horizontalLayout_5.addWidget(self.lowerBoundEdit)
        self.verticalLayout.addLayout(self.horizontalLayout_5)
        self.horizontalLayout_6 = QtGui.QHBoxLayout()
        self.horizontalLayout_6.setObjectName(_fromUtf8("horizontalLayout_6"))
        self.label_5 = QtGui.QLabel(EditorDistributedDomain)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.label_5.sizePolicy().hasHeightForWidth())
        self.label_5.setSizePolicy(sizePolicy)
        self.label_5.setMinimumSize(QtCore.QSize(100, 0))
        self.label_5.setObjectName(_fromUtf8("label_5"))
        self.horizontalLayout_6.addWidget(self.label_5)
        self.upperBoundEdit = QtGui.QLineEdit(EditorDistributedDomain)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(1)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.upperBoundEdit.sizePolicy().hasHeightForWidth())
        self.upperBoundEdit.setSizePolicy(sizePolicy)
        self.upperBoundEdit.setMinimumSize(QtCore.QSize(100, 0))
        self.upperBoundEdit.setObjectName(_fromUtf8("upperBoundEdit"))
        self.horizontalLayout_6.addWidget(self.upperBoundEdit)
        self.verticalLayout.addLayout(self.horizontalLayout_6)
        self.horizontalLayout_7 = QtGui.QHBoxLayout()
        self.horizontalLayout_7.setObjectName(_fromUtf8("horizontalLayout_7"))
        self.label_6 = QtGui.QLabel(EditorDistributedDomain)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.label_6.sizePolicy().hasHeightForWidth())
        self.label_6.setSizePolicy(sizePolicy)
        self.label_6.setMinimumSize(QtCore.QSize(100, 0))
        self.label_6.setObjectName(_fromUtf8("label_6"))
        self.horizontalLayout_7.addWidget(self.label_6)
        self.unitsEdit = QtGui.QLineEdit(EditorDistributedDomain)
        self.unitsEdit.setEnabled(False)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(1)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.unitsEdit.sizePolicy().hasHeightForWidth())
        self.unitsEdit.setSizePolicy(sizePolicy)
        self.unitsEdit.setMinimumSize(QtCore.QSize(100, 0))
        self.unitsEdit.setObjectName(_fromUtf8("unitsEdit"))
        self.horizontalLayout_7.addWidget(self.unitsEdit)
        self.verticalLayout.addLayout(self.horizontalLayout_7)
        self.horizontalLayout_2 = QtGui.QHBoxLayout()
        self.horizontalLayout_2.setObjectName(_fromUtf8("horizontalLayout_2"))
        spacerItem = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_2.addItem(spacerItem)
        self.updateButton = QtGui.QPushButton(EditorDistributedDomain)
        self.updateButton.setAutoDefault(True)
        self.updateButton.setDefault(True)
        self.updateButton.setObjectName(_fromUtf8("updateButton"))
        self.horizontalLayout_2.addWidget(self.updateButton)
        self.verticalLayout.addLayout(self.horizontalLayout_2)
        spacerItem1 = QtGui.QSpacerItem(20, 40, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        self.verticalLayout.addItem(spacerItem1)

        self.retranslateUi(EditorDistributedDomain)
        QtCore.QMetaObject.connectSlotsByName(EditorDistributedDomain)

    def retranslateUi(self, EditorDistributedDomain):
        self.label.setText(QtGui.QApplication.translate("EditorDistributedDomain", "Discr. Method", None, QtGui.QApplication.UnicodeUTF8))
        self.discrMethodEdit.setToolTip(QtGui.QApplication.translate("EditorDistributedDomain", "<html><head/><body><p>Insert the floating point value.The acceptable value is a valid Python float expression. Examples:</p>\n"
"<pre>1.02</pre>\n"
"<pre>5.032E-3</pre>\n"
"</body></html>", None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setText(QtGui.QApplication.translate("EditorDistributedDomain", "Order", None, QtGui.QApplication.UnicodeUTF8))
        self.orderEdit.setToolTip(QtGui.QApplication.translate("EditorDistributedDomain", "<html><head/><body><p>Insert the unit expressions using daetools.pyUnits.unit objects using mathematical operators *, / and **. All base and derived SI units are allowed including the common prefixes (more information at <a href=\"https://en.wikipedia.org/wiki/International_System_of_Units#Units_and_prefixes\"><span style=\" text-decoration: underline; color:#0000ff;\">https://en.wikipedia.org/wiki/International_System_of_Units#Units_and_prefixes</span></a>). The expression entered is a valid Python code. Examples:</p>\n"
"<pre>m/kg</pre>\n"
"<pre>m / s**2</pre>\n"
"<pre>J / (kg*K)</pre>\n"
"</body></html>", None, QtGui.QApplication.UnicodeUTF8))
        self.label_3.setText(QtGui.QApplication.translate("EditorDistributedDomain", "No. Intervals", None, QtGui.QApplication.UnicodeUTF8))
        self.numberOfIntervalsEdit.setToolTip(QtGui.QApplication.translate("EditorDistributedDomain", "<html><head/><body><p>Insert the unit expressions using daetools.pyUnits.unit objects using mathematical operators *, / and **. All base and derived SI units are allowed including the common prefixes (more information at <a href=\"https://en.wikipedia.org/wiki/International_System_of_Units#Units_and_prefixes\"><span style=\" text-decoration: underline; color:#0000ff;\">https://en.wikipedia.org/wiki/International_System_of_Units#Units_and_prefixes</span></a>). The expression entered is a valid Python code. Examples:</p>\n"
"<pre>m/kg</pre>\n"
"<pre>m / s**2</pre>\n"
"<pre>J / (kg*K)</pre>\n"
"</body></html>", None, QtGui.QApplication.UnicodeUTF8))
        self.label_4.setText(QtGui.QApplication.translate("EditorDistributedDomain", "Lower Bound", None, QtGui.QApplication.UnicodeUTF8))
        self.lowerBoundEdit.setToolTip(QtGui.QApplication.translate("EditorDistributedDomain", "<html><head/><body><p>Insert the unit expressions using daetools.pyUnits.unit objects using mathematical operators *, / and **. All base and derived SI units are allowed including the common prefixes (more information at <a href=\"https://en.wikipedia.org/wiki/International_System_of_Units#Units_and_prefixes\"><span style=\" text-decoration: underline; color:#0000ff;\">https://en.wikipedia.org/wiki/International_System_of_Units#Units_and_prefixes</span></a>). The expression entered is a valid Python code. Examples:</p>\n"
"<pre>m/kg</pre>\n"
"<pre>m / s**2</pre>\n"
"<pre>J / (kg*K)</pre>\n"
"</body></html>", None, QtGui.QApplication.UnicodeUTF8))
        self.label_5.setText(QtGui.QApplication.translate("EditorDistributedDomain", "Upper Bound", None, QtGui.QApplication.UnicodeUTF8))
        self.upperBoundEdit.setToolTip(QtGui.QApplication.translate("EditorDistributedDomain", "<html><head/><body><p>Insert the unit expressions using daetools.pyUnits.unit objects using mathematical operators *, / and **. All base and derived SI units are allowed including the common prefixes (more information at <a href=\"https://en.wikipedia.org/wiki/International_System_of_Units#Units_and_prefixes\"><span style=\" text-decoration: underline; color:#0000ff;\">https://en.wikipedia.org/wiki/International_System_of_Units#Units_and_prefixes</span></a>). The expression entered is a valid Python code. Examples:</p>\n"
"<pre>m/kg</pre>\n"
"<pre>m / s**2</pre>\n"
"<pre>J / (kg*K)</pre>\n"
"</body></html>", None, QtGui.QApplication.UnicodeUTF8))
        self.label_6.setText(QtGui.QApplication.translate("EditorDistributedDomain", "Units", None, QtGui.QApplication.UnicodeUTF8))
        self.unitsEdit.setToolTip(QtGui.QApplication.translate("EditorDistributedDomain", "<html><head/><body><p>Insert the unit expressions using daetools.pyUnits.unit objects using mathematical operators *, / and **. All base and derived SI units are allowed including the common prefixes (more information at <a href=\"https://en.wikipedia.org/wiki/International_System_of_Units#Units_and_prefixes\"><span style=\" text-decoration: underline; color:#0000ff;\">https://en.wikipedia.org/wiki/International_System_of_Units#Units_and_prefixes</span></a>). The expression entered is a valid Python code. Examples:</p>\n"
"<pre>m/kg</pre>\n"
"<pre>m / s**2</pre>\n"
"<pre>J / (kg*K)</pre>\n"
"</body></html>", None, QtGui.QApplication.UnicodeUTF8))
        self.updateButton.setToolTip(QtGui.QApplication.translate("EditorDistributedDomain", "<html><head/><body><p>Update the value.</p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
        self.updateButton.setText(QtGui.QApplication.translate("EditorDistributedDomain", "Update", None, QtGui.QApplication.UnicodeUTF8))

