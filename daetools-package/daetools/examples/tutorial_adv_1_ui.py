# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'tutorial_adv_1.ui'
#
# Created by: PyQt5 UI code generator 5.7
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_InteractiveRunDialog(object):
    def setupUi(self, InteractiveRunDialog):
        InteractiveRunDialog.setObjectName("InteractiveRunDialog")
        InteractiveRunDialog.setWindowModality(QtCore.Qt.WindowModal)
        InteractiveRunDialog.resize(729, 424)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(InteractiveRunDialog.sizePolicy().hasHeightForWidth())
        InteractiveRunDialog.setSizePolicy(sizePolicy)
        InteractiveRunDialog.setMinimumSize(QtCore.QSize(0, 0))
        InteractiveRunDialog.setMaximumSize(QtCore.QSize(1000, 1000))
        InteractiveRunDialog.setWindowTitle("")
        InteractiveRunDialog.setSizeGripEnabled(True)
        InteractiveRunDialog.setModal(True)
        self.verticalLayout_3 = QtWidgets.QVBoxLayout(InteractiveRunDialog)
        self.verticalLayout_3.setObjectName("verticalLayout_3")
        self.horizontalLayout_2 = QtWidgets.QHBoxLayout()
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.controlsLayout = QtWidgets.QVBoxLayout()
        self.controlsLayout.setContentsMargins(4, 4, 4, 4)
        self.controlsLayout.setObjectName("controlsLayout")
        self.horizontalLayout_6 = QtWidgets.QHBoxLayout()
        self.horizontalLayout_6.setContentsMargins(-1, -1, -1, 0)
        self.horizontalLayout_6.setObjectName("horizontalLayout_6")
        self.label_6 = QtWidgets.QLabel(InteractiveRunDialog)
        self.label_6.setMinimumSize(QtCore.QSize(150, 0))
        self.label_6.setObjectName("label_6")
        self.horizontalLayout_6.addWidget(self.label_6)
        self.powerSpinBox = QtWidgets.QDoubleSpinBox(InteractiveRunDialog)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.MinimumExpanding, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.powerSpinBox.sizePolicy().hasHeightForWidth())
        self.powerSpinBox.setSizePolicy(sizePolicy)
        self.powerSpinBox.setMinimumSize(QtCore.QSize(100, 0))
        self.powerSpinBox.setDecimals(1)
        self.powerSpinBox.setMaximum(5000.0)
        self.powerSpinBox.setSingleStep(100.0)
        self.powerSpinBox.setProperty("value", 1000.0)
        self.powerSpinBox.setObjectName("powerSpinBox")
        self.horizontalLayout_6.addWidget(self.powerSpinBox)
        self.label_7 = QtWidgets.QLabel(InteractiveRunDialog)
        self.label_7.setMinimumSize(QtCore.QSize(20, 0))
        self.label_7.setObjectName("label_7")
        self.horizontalLayout_6.addWidget(self.label_7)
        self.controlsLayout.addLayout(self.horizontalLayout_6)
        self.horizontalLayout_5 = QtWidgets.QHBoxLayout()
        self.horizontalLayout_5.setContentsMargins(-1, -1, -1, 0)
        self.horizontalLayout_5.setObjectName("horizontalLayout_5")
        self.label_3 = QtWidgets.QLabel(InteractiveRunDialog)
        self.label_3.setMinimumSize(QtCore.QSize(150, 0))
        self.label_3.setObjectName("label_3")
        self.horizontalLayout_5.addWidget(self.label_3)
        self.reportingIntervalSpinBox = QtWidgets.QDoubleSpinBox(InteractiveRunDialog)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.MinimumExpanding, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.reportingIntervalSpinBox.sizePolicy().hasHeightForWidth())
        self.reportingIntervalSpinBox.setSizePolicy(sizePolicy)
        self.reportingIntervalSpinBox.setMinimumSize(QtCore.QSize(100, 0))
        self.reportingIntervalSpinBox.setMaximum(1000.0)
        self.reportingIntervalSpinBox.setObjectName("reportingIntervalSpinBox")
        self.horizontalLayout_5.addWidget(self.reportingIntervalSpinBox)
        self.label_4 = QtWidgets.QLabel(InteractiveRunDialog)
        self.label_4.setMinimumSize(QtCore.QSize(20, 0))
        self.label_4.setObjectName("label_4")
        self.horizontalLayout_5.addWidget(self.label_4)
        self.controlsLayout.addLayout(self.horizontalLayout_5)
        self.horizontalLayout = QtWidgets.QHBoxLayout()
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.label = QtWidgets.QLabel(InteractiveRunDialog)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.label.sizePolicy().hasHeightForWidth())
        self.label.setSizePolicy(sizePolicy)
        self.label.setMinimumSize(QtCore.QSize(150, 0))
        self.label.setObjectName("label")
        self.horizontalLayout.addWidget(self.label)
        self.intervalSpinBox = QtWidgets.QDoubleSpinBox(InteractiveRunDialog)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.intervalSpinBox.sizePolicy().hasHeightForWidth())
        self.intervalSpinBox.setSizePolicy(sizePolicy)
        self.intervalSpinBox.setMinimumSize(QtCore.QSize(100, 0))
        self.intervalSpinBox.setMinimum(0.1)
        self.intervalSpinBox.setMaximum(10000.0)
        self.intervalSpinBox.setProperty("value", 100.0)
        self.intervalSpinBox.setObjectName("intervalSpinBox")
        self.horizontalLayout.addWidget(self.intervalSpinBox)
        self.label_5 = QtWidgets.QLabel(InteractiveRunDialog)
        self.label_5.setMinimumSize(QtCore.QSize(20, 0))
        self.label_5.setObjectName("label_5")
        self.horizontalLayout.addWidget(self.label_5)
        self.controlsLayout.addLayout(self.horizontalLayout)
        self.horizontalLayout_4 = QtWidgets.QHBoxLayout()
        self.horizontalLayout_4.setSizeConstraint(QtWidgets.QLayout.SetDefaultConstraint)
        self.horizontalLayout_4.setContentsMargins(0, -1, -1, 0)
        self.horizontalLayout_4.setObjectName("horizontalLayout_4")
        self.label_2 = QtWidgets.QLabel(InteractiveRunDialog)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.label_2.sizePolicy().hasHeightForWidth())
        self.label_2.setSizePolicy(sizePolicy)
        self.label_2.setMinimumSize(QtCore.QSize(150, 0))
        self.label_2.setObjectName("label_2")
        self.horizontalLayout_4.addWidget(self.label_2)
        self.currentTimeEdit = QtWidgets.QLineEdit(InteractiveRunDialog)
        self.currentTimeEdit.setEnabled(False)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.currentTimeEdit.sizePolicy().hasHeightForWidth())
        self.currentTimeEdit.setSizePolicy(sizePolicy)
        self.currentTimeEdit.setMinimumSize(QtCore.QSize(40, 0))
        self.currentTimeEdit.setFrame(False)
        self.currentTimeEdit.setObjectName("currentTimeEdit")
        self.horizontalLayout_4.addWidget(self.currentTimeEdit)
        self.controlsLayout.addLayout(self.horizontalLayout_4)
        self.horizontalLayout_3 = QtWidgets.QHBoxLayout()
        self.horizontalLayout_3.setSizeConstraint(QtWidgets.QLayout.SetFixedSize)
        self.horizontalLayout_3.setContentsMargins(6, 6, 6, 6)
        self.horizontalLayout_3.setObjectName("horizontalLayout_3")
        spacerItem = QtWidgets.QSpacerItem(40, 20, QtWidgets.QSizePolicy.MinimumExpanding, QtWidgets.QSizePolicy.Minimum)
        self.horizontalLayout_3.addItem(spacerItem)
        self.runButton = QtWidgets.QPushButton(InteractiveRunDialog)
        self.runButton.setMinimumSize(QtCore.QSize(30, 0))
        self.runButton.setBaseSize(QtCore.QSize(40, 0))
        self.runButton.setObjectName("runButton")
        self.horizontalLayout_3.addWidget(self.runButton)
        self.controlsLayout.addLayout(self.horizontalLayout_3)
        spacerItem1 = QtWidgets.QSpacerItem(20, 40, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Expanding)
        self.controlsLayout.addItem(spacerItem1)
        self.horizontalLayout_2.addLayout(self.controlsLayout)
        self.plotLayout = QtWidgets.QVBoxLayout()
        self.plotLayout.setContentsMargins(0, 0, 0, 0)
        self.plotLayout.setObjectName("plotLayout")
        self.frame = QtWidgets.QFrame(InteractiveRunDialog)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.MinimumExpanding, QtWidgets.QSizePolicy.Minimum)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.frame.sizePolicy().hasHeightForWidth())
        self.frame.setSizePolicy(sizePolicy)
        self.frame.setMinimumSize(QtCore.QSize(510, 410))
        self.frame.setFrameShape(QtWidgets.QFrame.NoFrame)
        self.frame.setFrameShadow(QtWidgets.QFrame.Plain)
        self.frame.setObjectName("frame")
        self.plotLayout.addWidget(self.frame)
        self.horizontalLayout_2.addLayout(self.plotLayout)
        self.verticalLayout_3.addLayout(self.horizontalLayout_2)

        self.retranslateUi(InteractiveRunDialog)
        QtCore.QMetaObject.connectSlotsByName(InteractiveRunDialog)

    def retranslateUi(self, InteractiveRunDialog):
        _translate = QtCore.QCoreApplication.translate
        self.label_6.setText(_translate("InteractiveRunDialog", "Input power (Qin):"))
        self.label_7.setText(_translate("InteractiveRunDialog", "W"))
        self.label_3.setText(_translate("InteractiveRunDialog", "Reporting Interval:"))
        self.label_4.setText(_translate("InteractiveRunDialog", "s"))
        self.label.setText(_translate("InteractiveRunDialog", "Integrate for:"))
        self.label_5.setText(_translate("InteractiveRunDialog", "s"))
        self.label_2.setText(_translate("InteractiveRunDialog", "Current time:"))
        self.currentTimeEdit.setText(_translate("InteractiveRunDialog", "0.0 s"))
        self.runButton.setText(_translate("InteractiveRunDialog", "Run"))

