# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'Simulator.ui'
#
# Created: Thu Oct  6 15:55:33 2011
#      by: PyQt4 UI code generator 4.7.3
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_SimulatorDialog(object):
    def setupUi(self, SimulatorDialog):
        SimulatorDialog.setObjectName("SimulatorDialog")
        SimulatorDialog.resize(606, 580)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap("images/py.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        SimulatorDialog.setWindowIcon(icon)
        self.verticalLayout_2 = QtGui.QVBoxLayout(SimulatorDialog)
        self.verticalLayout_2.setObjectName("verticalLayout_2")
        self.formLayout = QtGui.QFormLayout()
        self.formLayout.setFieldGrowthPolicy(QtGui.QFormLayout.AllNonFixedFieldsGrow)
        self.formLayout.setObjectName("formLayout")
        self.simulationLabel = QtGui.QLabel(SimulatorDialog)
        self.simulationLabel.setObjectName("simulationLabel")
        self.formLayout.setWidget(0, QtGui.QFormLayout.LabelRole, self.simulationLabel)
        self.SimulationLineEdit = QtGui.QLineEdit(SimulatorDialog)
        self.SimulationLineEdit.setEnabled(False)
        self.SimulationLineEdit.setObjectName("SimulationLineEdit")
        self.formLayout.setWidget(0, QtGui.QFormLayout.FieldRole, self.SimulationLineEdit)
        self.DataReporterTCPIPAddressLabel = QtGui.QLabel(SimulatorDialog)
        self.DataReporterTCPIPAddressLabel.setObjectName("DataReporterTCPIPAddressLabel")
        self.formLayout.setWidget(1, QtGui.QFormLayout.LabelRole, self.DataReporterTCPIPAddressLabel)
        self.DataReporterTCPIPAddressLineEdit = QtGui.QLineEdit(SimulatorDialog)
        self.DataReporterTCPIPAddressLineEdit.setText("")
        self.DataReporterTCPIPAddressLineEdit.setObjectName("DataReporterTCPIPAddressLineEdit")
        self.formLayout.setWidget(1, QtGui.QFormLayout.FieldRole, self.DataReporterTCPIPAddressLineEdit)
        self.MINLPSolverLabel = QtGui.QLabel(SimulatorDialog)
        self.MINLPSolverLabel.setObjectName("MINLPSolverLabel")
        self.formLayout.setWidget(2, QtGui.QFormLayout.LabelRole, self.MINLPSolverLabel)
        self.MINLPSolverComboBox = QtGui.QComboBox(SimulatorDialog)
        self.MINLPSolverComboBox.setObjectName("MINLPSolverComboBox")
        self.MINLPSolverComboBox.addItem("")
        self.MINLPSolverComboBox.addItem("")
        self.MINLPSolverComboBox.addItem("")
        self.formLayout.setWidget(2, QtGui.QFormLayout.FieldRole, self.MINLPSolverComboBox)
        self.DAESolverLabel = QtGui.QLabel(SimulatorDialog)
        self.DAESolverLabel.setObjectName("DAESolverLabel")
        self.formLayout.setWidget(3, QtGui.QFormLayout.LabelRole, self.DAESolverLabel)
        self.DAESolverComboBox = QtGui.QComboBox(SimulatorDialog)
        self.DAESolverComboBox.setObjectName("DAESolverComboBox")
        self.DAESolverComboBox.addItem("")
        self.formLayout.setWidget(3, QtGui.QFormLayout.FieldRole, self.DAESolverComboBox)
        self.LASolverLabel = QtGui.QLabel(SimulatorDialog)
        self.LASolverLabel.setObjectName("LASolverLabel")
        self.formLayout.setWidget(4, QtGui.QFormLayout.LabelRole, self.LASolverLabel)
        self.LASolverComboBox = QtGui.QComboBox(SimulatorDialog)
        self.LASolverComboBox.setObjectName("LASolverComboBox")
        self.LASolverComboBox.addItem("")
        self.LASolverComboBox.addItem("")
        self.LASolverComboBox.addItem("")
        self.LASolverComboBox.addItem("")
        self.LASolverComboBox.addItem("")
        self.LASolverComboBox.addItem("")
        self.LASolverComboBox.addItem("")
        self.LASolverComboBox.addItem("")
        self.LASolverComboBox.addItem("")
        self.LASolverComboBox.addItem("")
        self.LASolverComboBox.addItem("")
        self.LASolverComboBox.addItem("")
        self.LASolverComboBox.addItem("")
        self.LASolverComboBox.addItem("")
        self.LASolverComboBox.addItem("")
        self.formLayout.setWidget(4, QtGui.QFormLayout.FieldRole, self.LASolverComboBox)
        self.TimeHorizonLabel = QtGui.QLabel(SimulatorDialog)
        self.TimeHorizonLabel.setObjectName("TimeHorizonLabel")
        self.formLayout.setWidget(5, QtGui.QFormLayout.LabelRole, self.TimeHorizonLabel)
        self.TimeHorizonDoubleSpinBox = QtGui.QDoubleSpinBox(SimulatorDialog)
        self.TimeHorizonDoubleSpinBox.setMaximum(1000000000.0)
        self.TimeHorizonDoubleSpinBox.setObjectName("TimeHorizonDoubleSpinBox")
        self.formLayout.setWidget(5, QtGui.QFormLayout.FieldRole, self.TimeHorizonDoubleSpinBox)
        self.ReportingIntervalLabel = QtGui.QLabel(SimulatorDialog)
        self.ReportingIntervalLabel.setObjectName("ReportingIntervalLabel")
        self.formLayout.setWidget(6, QtGui.QFormLayout.LabelRole, self.ReportingIntervalLabel)
        self.ReportingIntervalDoubleSpinBox = QtGui.QDoubleSpinBox(SimulatorDialog)
        self.ReportingIntervalDoubleSpinBox.setMaximum(1000000000.0)
        self.ReportingIntervalDoubleSpinBox.setObjectName("ReportingIntervalDoubleSpinBox")
        self.formLayout.setWidget(6, QtGui.QFormLayout.FieldRole, self.ReportingIntervalDoubleSpinBox)
        self.verticalLayout_2.addLayout(self.formLayout)
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName("horizontalLayout")
        spacerItem = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout.addItem(spacerItem)
        self.ExportButton = QtGui.QPushButton(SimulatorDialog)
        self.ExportButton.setEnabled(True)
        self.ExportButton.setObjectName("ExportButton")
        self.horizontalLayout.addWidget(self.ExportButton)
        self.MatrixButton = QtGui.QPushButton(SimulatorDialog)
        self.MatrixButton.setEnabled(True)
        self.MatrixButton.setObjectName("MatrixButton")
        self.horizontalLayout.addWidget(self.MatrixButton)
        self.RunButton = QtGui.QPushButton(SimulatorDialog)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.RunButton.sizePolicy().hasHeightForWidth())
        self.RunButton.setSizePolicy(sizePolicy)
        self.RunButton.setDefault(True)
        self.RunButton.setObjectName("RunButton")
        self.horizontalLayout.addWidget(self.RunButton)
        self.PauseButton = QtGui.QPushButton(SimulatorDialog)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.PauseButton.sizePolicy().hasHeightForWidth())
        self.PauseButton.setSizePolicy(sizePolicy)
        self.PauseButton.setObjectName("PauseButton")
        self.horizontalLayout.addWidget(self.PauseButton)
        self.ResumeButton = QtGui.QPushButton(SimulatorDialog)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.ResumeButton.sizePolicy().hasHeightForWidth())
        self.ResumeButton.setSizePolicy(sizePolicy)
        self.ResumeButton.setCheckable(False)
        self.ResumeButton.setChecked(False)
        self.ResumeButton.setObjectName("ResumeButton")
        self.horizontalLayout.addWidget(self.ResumeButton)
        self.verticalLayout_2.addLayout(self.horizontalLayout)
        self.verticalLayout = QtGui.QVBoxLayout()
        self.verticalLayout.setObjectName("verticalLayout")
        self.textEdit = QtGui.QTextEdit(SimulatorDialog)
        self.textEdit.setEnabled(True)
        self.textEdit.setMinimumSize(QtCore.QSize(510, 180))
        font = QtGui.QFont()
        font.setFamily("Monospace")
        font.setPointSize(9)
        self.textEdit.setFont(font)
        self.textEdit.setHorizontalScrollBarPolicy(QtCore.Qt.ScrollBarAsNeeded)
        self.textEdit.setUndoRedoEnabled(False)
        self.textEdit.setLineWrapMode(QtGui.QTextEdit.NoWrap)
        self.textEdit.setReadOnly(True)
        self.textEdit.setHtml("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Monospace\'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:\'Courier New\';\"></p></body></html>")
        self.textEdit.setObjectName("textEdit")
        self.verticalLayout.addWidget(self.textEdit)
        self.verticalLayout_2.addLayout(self.verticalLayout)
        self.horizontalLayout_3 = QtGui.QHBoxLayout()
        self.horizontalLayout_3.setObjectName("horizontalLayout_3")
        self.progressBar = QtGui.QProgressBar(SimulatorDialog)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.progressBar.sizePolicy().hasHeightForWidth())
        self.progressBar.setSizePolicy(sizePolicy)
        self.progressBar.setMinimumSize(QtCore.QSize(150, 0))
        self.progressBar.setProperty("value", 50)
        self.progressBar.setInvertedAppearance(False)
        self.progressBar.setObjectName("progressBar")
        self.horizontalLayout_3.addWidget(self.progressBar)
        self.progressLabel = QtGui.QLabel(SimulatorDialog)
        self.progressLabel.setObjectName("progressLabel")
        self.horizontalLayout_3.addWidget(self.progressLabel)
        self.verticalLayout_2.addLayout(self.horizontalLayout_3)

        self.retranslateUi(SimulatorDialog)
        QtCore.QMetaObject.connectSlotsByName(SimulatorDialog)

    def retranslateUi(self, SimulatorDialog):
        SimulatorDialog.setWindowTitle(QtGui.QApplication.translate("SimulatorDialog", "DAE Tools Simulator", None, QtGui.QApplication.UnicodeUTF8))
        self.simulationLabel.setText(QtGui.QApplication.translate("SimulatorDialog", "Simulation", None, QtGui.QApplication.UnicodeUTF8))
        self.DataReporterTCPIPAddressLabel.setText(QtGui.QApplication.translate("SimulatorDialog", "Data Reporter (TCP/IP : port)", None, QtGui.QApplication.UnicodeUTF8))
        self.MINLPSolverLabel.setText(QtGui.QApplication.translate("SimulatorDialog", "(MI)NLP Solver", None, QtGui.QApplication.UnicodeUTF8))
        self.MINLPSolverComboBox.setItemText(0, QtGui.QApplication.translate("SimulatorDialog", "IPOPT NLP", None, QtGui.QApplication.UnicodeUTF8))
        self.MINLPSolverComboBox.setItemText(1, QtGui.QApplication.translate("SimulatorDialog", "NLOPT NLP", None, QtGui.QApplication.UnicodeUTF8))
        self.MINLPSolverComboBox.setItemText(2, QtGui.QApplication.translate("SimulatorDialog", "BONMIN MINLP", None, QtGui.QApplication.UnicodeUTF8))
        self.DAESolverLabel.setText(QtGui.QApplication.translate("SimulatorDialog", "DAE Solver", None, QtGui.QApplication.UnicodeUTF8))
        self.DAESolverComboBox.setItemText(0, QtGui.QApplication.translate("SimulatorDialog", "Sundials IDAS", None, QtGui.QApplication.UnicodeUTF8))
        self.LASolverLabel.setText(QtGui.QApplication.translate("SimulatorDialog", "LA Solver", None, QtGui.QApplication.UnicodeUTF8))
        self.LASolverComboBox.setItemText(0, QtGui.QApplication.translate("SimulatorDialog", "Sundials LU (dense, sequential)", None, QtGui.QApplication.UnicodeUTF8))
        self.LASolverComboBox.setItemText(1, QtGui.QApplication.translate("SimulatorDialog", "Trilinos Amesos - KLU (sparse, sequential, direct)", None, QtGui.QApplication.UnicodeUTF8))
        self.LASolverComboBox.setItemText(2, QtGui.QApplication.translate("SimulatorDialog", "Trilinos Amesos - SuperLU (sparse, sequential, direct)", None, QtGui.QApplication.UnicodeUTF8))
        self.LASolverComboBox.setItemText(3, QtGui.QApplication.translate("SimulatorDialog", "Trilinos Amesos - Umfpack (sparse, sequential, direct)", None, QtGui.QApplication.UnicodeUTF8))
        self.LASolverComboBox.setItemText(4, QtGui.QApplication.translate("SimulatorDialog", "Trilinos Amesos - Lapack (dense, sequential, direct)", None, QtGui.QApplication.UnicodeUTF8))
        self.LASolverComboBox.setItemText(5, QtGui.QApplication.translate("SimulatorDialog", "Trilinos AztecOO - Krylov (sparse, sequential, iterative)", None, QtGui.QApplication.UnicodeUTF8))
        self.LASolverComboBox.setItemText(6, QtGui.QApplication.translate("SimulatorDialog", "Intel Pardiso (sparse, OpenMP, direct)", None, QtGui.QApplication.UnicodeUTF8))
        self.LASolverComboBox.setItemText(7, QtGui.QApplication.translate("SimulatorDialog", "Intel MKL Lapack (dense, OpenMP, direct)", None, QtGui.QApplication.UnicodeUTF8))
        self.LASolverComboBox.setItemText(8, QtGui.QApplication.translate("SimulatorDialog", "AMD ACML Lapack (dense, OpenMP, direct)", None, QtGui.QApplication.UnicodeUTF8))
        self.LASolverComboBox.setItemText(9, QtGui.QApplication.translate("SimulatorDialog", "Lapack (dense, sequential, direct)", None, QtGui.QApplication.UnicodeUTF8))
        self.LASolverComboBox.setItemText(10, QtGui.QApplication.translate("SimulatorDialog", "Magma Lapack (dense, CUDA, direct)", None, QtGui.QApplication.UnicodeUTF8))
        self.LASolverComboBox.setItemText(11, QtGui.QApplication.translate("SimulatorDialog", "SuperLU (sparse, sequential, direct)", None, QtGui.QApplication.UnicodeUTF8))
        self.LASolverComboBox.setItemText(12, QtGui.QApplication.translate("SimulatorDialog", "SuperLU_MT (sparse, POSIX threads, direct)", None, QtGui.QApplication.UnicodeUTF8))
        self.LASolverComboBox.setItemText(13, QtGui.QApplication.translate("SimulatorDialog", "SuperLU_CUDA (sparse, CUDA, direct)", None, QtGui.QApplication.UnicodeUTF8))
        self.LASolverComboBox.setItemText(14, QtGui.QApplication.translate("SimulatorDialog", "CUSP (sparse, CUDA, iterative)", None, QtGui.QApplication.UnicodeUTF8))
        self.TimeHorizonLabel.setText(QtGui.QApplication.translate("SimulatorDialog", "Time Horizon, s", None, QtGui.QApplication.UnicodeUTF8))
        self.ReportingIntervalLabel.setText(QtGui.QApplication.translate("SimulatorDialog", "Reporting Interval, s", None, QtGui.QApplication.UnicodeUTF8))
        self.ExportButton.setText(QtGui.QApplication.translate("SimulatorDialog", "Export Matrix...", None, QtGui.QApplication.UnicodeUTF8))
        self.MatrixButton.setText(QtGui.QApplication.translate("SimulatorDialog", "Matrix Preview...", None, QtGui.QApplication.UnicodeUTF8))
        self.RunButton.setText(QtGui.QApplication.translate("SimulatorDialog", "Start", None, QtGui.QApplication.UnicodeUTF8))
        self.PauseButton.setText(QtGui.QApplication.translate("SimulatorDialog", "Pause", None, QtGui.QApplication.UnicodeUTF8))
        self.ResumeButton.setText(QtGui.QApplication.translate("SimulatorDialog", "Resume", None, QtGui.QApplication.UnicodeUTF8))
        self.progressLabel.setText(QtGui.QApplication.translate("SimulatorDialog", "ETA: 01:06:15", None, QtGui.QApplication.UnicodeUTF8))

