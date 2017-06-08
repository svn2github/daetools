"""********************************************************************************
                             choose_variable.py
                 DAE Tools: pyDAE module, www.daetools.com
                 Copyright (C) Dragan Nikolic
***********************************************************************************
DAE Tools is free software; you can redistribute it and/or modify it under the
terms of the GNU General Public License version 3 as published by the Free Software
Foundation. DAE Tools is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with the
DAE Tools software; if not, see <http://www.gnu.org/licenses/>.
********************************************************************************"""
"""
October 2012 by Caleb Huttingh:
  - 3D plot bug fix when detecting free domains
"""
import sys, numpy
from os.path import join, realpath, dirname
from PyQt5 import QtCore, QtGui, QtWidgets
from daetools.pyDAE import *
from .choose_variable_ui import Ui_ChooseVariable
from .table_widget import Ui_tableWidgetDialog

python_major = sys.version_info[0]

class daeTableDialog(QtWidgets.QDialog):
    def __init__(self, parent):
        QtWidgets.QDialog.__init__(self, parent)
        self.ui = Ui_tableWidgetDialog()
        self.ui.setupUi(self)

images_dir = join(dirname(__file__), 'images')

class daeChooseVariable(QtWidgets.QDialog):

    (plot2D, plot2DAutoUpdated, plot2DAnimated, plot3D) = list(range(0, 4))
    FREE_DOMAIN = -1
    LAST_TIME   = -2

    def __init__(self, plotType):
        QtWidgets.QDialog.__init__(self)
        self.ui = Ui_ChooseVariable()
        self.ui.setupUi(self)

        self.setWindowIcon(QtGui.QIcon(join(images_dir, 'daetools-48x48.png')))

        self.variable = None
        self.domainIndexes   = []
        self.domainPoints    = []
        self.plotType        = plotType
        self.processes       = []

        self.hideAndClearAll()

        self.ui.treeWidget.itemSelectionChanged.connect(self.slotSelectionChanged)
        self.ui.timeComboBox.currentIndexChanged.connect(self.slotCurrentIndexChanged)
        self.ui.domain0ComboBox.currentIndexChanged.connect(self.slotCurrentIndexChanged)
        self.ui.domain1ComboBox.currentIndexChanged.connect(self.slotCurrentIndexChanged)
        self.ui.domain2ComboBox.currentIndexChanged.connect(self.slotCurrentIndexChanged)
        self.ui.domain3ComboBox.currentIndexChanged.connect(self.slotCurrentIndexChanged)
        self.ui.domain4ComboBox.currentIndexChanged.connect(self.slotCurrentIndexChanged)
        self.ui.domain5ComboBox.currentIndexChanged.connect(self.slotCurrentIndexChanged)
        self.ui.domain6ComboBox.currentIndexChanged.connect(self.slotCurrentIndexChanged)
        self.ui.domain7ComboBox.currentIndexChanged.connect(self.slotCurrentIndexChanged)

    def updateProcessesList(self, processes):
        for pnew in processes:
            found = False
            for pold in  self.processes:
                if pold.Name == pnew.Name:
                    found = True
                    break
            if not found:
                self.addProcess(pnew)

        self.processes = processes
       
    def addProcess(self, process):      
        rootItem = QtWidgets.QTreeWidgetItem(self.ui.treeWidget)
        rootItem.setText(0, process.Name)

        variables = process.Variables
        for var in variables:
            currentItem = rootItem
            names = var.Name.split(".")

            var_path = names[:-1]

            var_name = names[-1]
            var_name = var_name.replace("&", "")
            var_name = var_name.replace(";", "")

            # First find the parent QTreeWidgetItem
            for item in var_path:
                name = item.replace("&", "")
                name = name.replace(";", "")
                # All names in the path has to be enclosed with brackets [] to distinguish between variables and ports/models
                name = '[' + name + ']'

                found = False
                for c in range(0, currentItem.childCount()):
                    child = currentItem.child(c)
                    cname = child.text(0)
                    if name == cname:
                        found = True
                        currentItem = currentItem.child(c)
                        break

                if found == False:
                    currentItem = QtWidgets.QTreeWidgetItem(currentItem)
                    currentItem.setText(0, name)

            # Now we have the parent in the currentItem, so add the new item to it with the variable data
            varItem = QtWidgets.QTreeWidgetItem(currentItem)
            #varData = QtCore.QVariant(var)
            varItem.setText(0, var_name)
            varItem.setData(0, QtCore.Qt.UserRole, var) #varData)

    #@QtCore.pyqtSlot(int)
    def slotCurrentIndexChanged(self, index):
        self.domainIndexes = []
        if(self.ui.timeComboBox.isVisible()):
            i      = self.ui.timeComboBox.currentIndex()
            _index = self.ui.timeComboBox.itemData(i)
            if isinstance(_index, QtCore.QVariant):
                domain_index, ok = _index.toInt()
            else:
                domain_index = _index
            self.domainIndexes.append(domain_index)

        if(self.ui.domain0ComboBox.isVisible()):
            i      = self.ui.domain0ComboBox.currentIndex()
            _index = self.ui.domain0ComboBox.itemData(i)
            if isinstance(_index, QtCore.QVariant):
                domain_index, ok = _index.toInt()
            else:
                domain_index = _index
            self.domainIndexes.append(domain_index)

        if(self.ui.domain1ComboBox.isVisible()):
            i      = self.ui.domain1ComboBox.currentIndex()
            _index = self.ui.domain1ComboBox.itemData(i)
            if isinstance(_index, QtCore.QVariant):
                domain_index, ok = _index.toInt()
            else:
                domain_index = _index
            self.domainIndexes.append(domain_index)

        if(self.ui.domain2ComboBox.isVisible()):
            i      = self.ui.domain2ComboBox.currentIndex()
            _index = self.ui.domain2ComboBox.itemData(i)
            if isinstance(_index, QtCore.QVariant):
                domain_index, ok = _index.toInt()
            else:
                domain_index = _index
            self.domainIndexes.append(domain_index)

        if(self.ui.domain3ComboBox.isVisible()):
            i      = self.ui.domain3ComboBox.currentIndex()
            _index = self.ui.domain3ComboBox.itemData(i)
            if isinstance(_index, QtCore.QVariant):
                domain_index, ok = _index.toInt()
            else:
                domain_index = _index
            self.domainIndexes.append(domain_index)

        if(self.ui.domain4ComboBox.isVisible()):
            i      = self.ui.domain4ComboBox.currentIndex()
            _index = self.ui.domain4ComboBox.itemData(i)
            if isinstance(_index, QtCore.QVariant):
                domain_index, ok = _index.toInt()
            else:
                domain_index = _index
            self.domainIndexes.append(domain_index)

        if(self.ui.domain5ComboBox.isVisible()):
            i      = self.ui.domain5ComboBox.currentIndex()
            _index = self.ui.domain5ComboBox.itemData(i)
            if isinstance(_index, QtCore.QVariant):
                domain_index, ok = _index.toInt()
            else:
                domain_index = _index
            self.domainIndexes.append(domain_index)

        if(self.ui.domain6ComboBox.isVisible()):
            i      = self.ui.domain6ComboBox.currentIndex()
            _index = self.ui.domain6ComboBox.itemData(i)
            if isinstance(_index, QtCore.QVariant):
                domain_index, ok = _index.toInt()
            else:
                domain_index = _index
            self.domainIndexes.append(domain_index)

        if(self.ui.domain7ComboBox.isVisible()):
            i      = self.ui.domain7ComboBox.currentIndex()
            _index = self.ui.domain7ComboBox.itemData(i)
            if isinstance(_index, QtCore.QVariant):
                domain_index, ok = _index.toInt()
            else:
                domain_index = _index
            self.domainIndexes.append(domain_index)

        self.domainPoints = []
        if(self.ui.timeComboBox.isVisible()):
            self.domainPoints.append(str(self.ui.timeComboBox.currentText()))
        if(self.ui.domain0ComboBox.isVisible()):
            self.domainPoints.append(str(self.ui.domain0ComboBox.currentText()))
        if(self.ui.domain1ComboBox.isVisible()):
            self.domainPoints.append(str(self.ui.domain1ComboBox.currentText()))
        if(self.ui.domain2ComboBox.isVisible()):
            self.domainPoints.append(str(self.ui.domain2ComboBox.currentText()))
        if(self.ui.domain3ComboBox.isVisible()):
            self.domainPoints.append(str(self.ui.domain3ComboBox.currentText()))
        if(self.ui.domain4ComboBox.isVisible()):
            self.domainPoints.append(str(self.ui.domain4ComboBox.currentText()))
        if(self.ui.domain5ComboBox.isVisible()):
            self.domainPoints.append(str(self.ui.domain5ComboBox.currentText()))
        if(self.ui.domain6ComboBox.isVisible()):
            self.domainPoints.append(str(self.ui.domain6ComboBox.currentText()))
        if(self.ui.domain7ComboBox.isVisible()):
            self.domainPoints.append(str(self.ui.domain7ComboBox.currentText()))

        #print 'domainIndexes = {0}'.format(self.domainIndexes)
        #print 'domainPoints  = {0}'.format(self.domainPoints)

        freeDomains = 0
        for ind in self.domainIndexes:
            if ind == daeChooseVariable.FREE_DOMAIN: # if it is FREE
                freeDomains += 1

        if self.plotType == daeChooseVariable.plot2D:
            if freeDomains == 1:
                self.ui.buttonOk.setEnabled(True)
            else:
                self.ui.buttonOk.setEnabled(False)

        elif self.plotType == daeChooseVariable.plot2DAutoUpdated:
            if freeDomains == 1:
                self.ui.buttonOk.setEnabled(True)
            else:
                self.ui.buttonOk.setEnabled(False)

        elif self.plotType == daeChooseVariable.plot3D:
            if freeDomains == 2:
                self.ui.buttonOk.setEnabled(True)
            else:
                self.ui.buttonOk.setEnabled(False)

        elif self.plotType == daeChooseVariable.plot2DAnimated:
            if freeDomains == 2:
                if(self.ui.timeComboBox.isVisible()):
                    if(str(self.ui.timeComboBox.currentText()) != '*'):
                        self.ui.buttonOk.setEnabled(False)
                    else:
                        self.ui.buttonOk.setEnabled(True)
                else:
                    self.ui.buttonOk.setEnabled(False)
            else:
                self.ui.buttonOk.setEnabled(False)

        else:
            self.ui.buttonOk.setEnabled(False)

    def getPlot2DData(self):
        return daeChooseVariable.get2DData(self.variable, self.domainIndexes, self.domainPoints)

    @staticmethod
    def get2DData(variable, domainIndexes_, domainPoints_):
        # Achtung, achtung!!
        # It is important to get TimeValues first since the reporter
        # might add more values to the data receiver (in the meantime)
        # and the size of the xPoints and yPoints arrays will not match
        times   = variable.TimeValues
        values  = variable.Values
        domains = variable.Domains

        # deep copy domainIndexes because it is a data member and gets cleared if a new plot is added
        domainIndexes = list(domainIndexes_)
        domainPoints  = list(domainPoints_)

        xAxisLabel = ""

        # Remove html code marks ('&' and ';')
        yname = variable.Name
        yAxisLabel = yname.replace("&", "").replace(";", "") + " (%s)" % variable.Units
        xPoints = []

        noTimePoints = len(times)

        # x axis points
        # Only one domain is free
        for i in range(0, len(domainIndexes)):
            if domainIndexes[i] == daeChooseVariable.FREE_DOMAIN:
                if i == 0: # Time domain
                    xAxisLabel = "Time (s)"
                    xPoints.extend(times[0 : noTimePoints])

                else: # Some other domain
                    d = domains[i-1] # because Time is not in a domain list
                    names = d.Name.split(".")

                    # Remove html code marks ('&' and ';')
                    xname = names[len(names)-1]
                    xAxisLabel = xname.replace("&", "").replace(";", "") + " (%s)" % d.Units
                    if d.Type == eUnstructuredGrid:
                        xPoints.extend([i for i in range(d.NumberOfPoints)])
                    else:
                        xPoints.extend(d.Points[0 : d.NumberOfPoints])

                break

        # y axis points
        t = []
        for i in range(0, len(domainIndexes)):
            domainIndex = domainIndexes[i]

            if domainIndex == daeChooseVariable.FREE_DOMAIN:
                if i == 0: # Time points
                    t.append(slice(0, noTimePoints))

                else: # Other domain's points
                    d = domains[i-1]
                    t.append(slice(0, d.NumberOfPoints))

            elif domainIndex == daeChooseVariable.LAST_TIME:
                # Special case when time = "Last value" has been selected (animated plots)
                t.append(noTimePoints-1)

                # Update domain points with the last time
                domainPoints[i] = 'ct'

            else:
                t.append(domainIndex)

        yPoints = values[t] #.copy()

        #print(noTimePoints)
        #print(('Number of x points = {0}'.format(len(xPoints))))
        #print(('Number of y points = {0}'.format(len(yPoints))))
        #print times[-1]

        return variable, domainIndexes, domainPoints, xAxisLabel, yAxisLabel, xPoints, yPoints, times[-1]

    def getPlot2DAnimatedData(self):
        return daeChooseVariable.get2DAnimatedData(self.variable, self.domainIndexes, self.domainPoints)

    @staticmethod
    def get2DAnimatedData(variable, domainIndexes_, domainPoints_):
        # Achtung, achtung!!
        # It is important to get TimeValues first since the reporter
        # might add more values to the data receiver (in the meantime)
        # and the size of xPoints, yPoints and zPoints arrays will not match
        times   = variable.TimeValues
        values  = variable.Values
        domains = variable.Domains

        # deep copy domainIndexes because it is a data member and gets cleared if a new plot is added
        domainIndexes = list(domainIndexes_)
        domainPoints  = list(domainPoints_)

        timePoints = []
        xPoints = []
        yPoints = []
        xAxisLabel = ""
        yAxisLabel = ""

        # Remove html code marks ('&' and ';')
        yname = variable.Name
        yAxisLabel = yname.replace("&", "").replace(";", "") + " (%s)" % variable.Units

        # Find 2 domains that are FREE
        freeDomainIndexes = []
        for i in range(0, len(domainIndexes)):
            if domainIndexes[i] == daeChooseVariable.FREE_DOMAIN:
                freeDomainIndexes.append(i)
        if len(freeDomainIndexes) != 2:
            raise RuntimeError('Number of free domains is not 2')
        if domainIndexes[0] != daeChooseVariable.FREE_DOMAIN:
            raise RuntimeError('Time domain must be free for the animated 2D plot')

        noTimePoints = len(times)

        # Times
        timePoints.extend(times[0 : noTimePoints])

        # x axis points
        for i in range(0, len(domainIndexes)):
            if domainIndexes[i] == daeChooseVariable.FREE_DOMAIN:
                if i == 0: # Time domain
                    pass

                else: # Some other domain
                    d = domains[i-1] # because Time is not in a domain list
                    names = d.Name.split(".")

                    # Remove html code marks ('&' and ';')
                    xname = names[len(names)-1]
                    xAxisLabel = xname.replace("&", "").replace(";", "") + " (%s)" % d.Units
                    if d.Type == eUnstructuredGrid:
                        xPoints.extend([i for i in range(d.NumberOfPoints)])
                    else:
                        xPoints.extend(d.Points[0 : d.NumberOfPoints])

                    break

        # y axis points
        t = []
        for i in range(0, len(domainIndexes)):
            domainIndex = domainIndexes[i]

            if domainIndex == daeChooseVariable.FREE_DOMAIN:
                if i == 0: # Time points
                    t.append(slice(0, noTimePoints))

                else: # Other domain's points
                    d = domains[i-1]
                    t.append(slice(0, d.NumberOfPoints))

            else:
                t.append(domainIndex)

        yPoints = values[t] # This is 2D array!!

        #print('t = %s' % t)
        #print('yPoints = %s' % yPoints)
        #print('values = %s' % values)

        return variable, domainIndexes, domainPoints, xAxisLabel, yAxisLabel, xPoints, yPoints, timePoints

    def getPlot3DData(self):
        return daeChooseVariable.get3DData(self.variable, self.domainIndexes, self.domainPoints)

    @staticmethod
    def get3DData(variable, domainIndexes, domainPoints):
        # Achtung, achtung!!
        # It is important to get TimeValues first since the reporter
        # might add more values to the data receiver (in the meantime)
        # and the size of xPoints, yPoints and zPoints arrays will not match
        times   = variable.TimeValues
        values  = variable.Values
        domains = variable.Domains

        xPoints = []
        yPoints = []
        xAxisLabel = ""
        yAxisLabel = ""

        # Remove html code marks ('&' and ';')
        zname = variable.Name
        zAxisLabel = zname.replace("&", "").replace(";", "") + " (%s)" % variable.Units

        # Find 2 domains that are FREE
        freeDomainIndexes = []
        for i in range(0, len(domainIndexes)):
            if domainIndexes[i] == daeChooseVariable.FREE_DOMAIN:
                freeDomainIndexes.append(i)
        if len(freeDomainIndexes) != 2:
            raise RuntimeError('Number of free domains is not 2')

        noTimePoints = len(times)

        # x axis
        nd = freeDomainIndexes[0]
        if nd == 0: # Time domain
            xAxisLabel = "Time (s)"
            xPoints.extend(times[0 : noTimePoints])
            #for k in range(0, noTimePoints):
            #    xPoints.append(times[k])

        else: # Some other domain
            d = domains[nd-1] # because Time is not in a domain list
            names = d.Name.split(".")

            # Remove html code marks ('&' and ';')
            xname = names[len(names)-1]
            xAxisLabel = xname.replace("&", "").replace(";", "") + " (%s)" % d.Units

            xPoints.extend(d.Points[0 : d.NumberOfPoints])
            #for k in range(0, d.NumberOfPoints):
            #    xPoints.append(d[k])

        # y axis
        nd = freeDomainIndexes[1]
        if nd == 0: # Time domain
            yAxisLabel = "Time (s)"
            yPoints.extend(times[0 : noTimePoints])
            #for k in range(0, noTimePoints):
            #    yPoints.append(times[k])

        else: # Some other domain
            d = domains[nd-1] # because Time is not in a domain list
            names = d.Name.split(".")

            # Remove html code marks ('&' and ';')
            yname = names[len(names)-1]
            yAxisLabel = yname.replace("&", "").replace(";", "") + " (%s)" % d.Units

            yPoints.extend(d.Points[0 : d.NumberOfPoints])
            #for k in range(0, d.NumberOfPoints):
            #    yPoints.append(d[k])

        # z axis
        t = []
        for i in range(0, len(domainIndexes)):
            domainIndex = domainIndexes[i]

            if domainIndex == daeChooseVariable.FREE_DOMAIN:
                if i == 0: # Time domain
                    t.append(slice(0, noTimePoints))
                else:
                    d = domains[i-1] # because Time is not in a domain list
                    t.append(slice(0, d.NumberOfPoints))

            elif domainIndex == daeChooseVariable.LAST_TIME:
                t.append(noTimePoints-1)

                # Update domain points with the last time
                domainPoints[i] = 'ct'

            else:
                t.append(domainIndex)

        zPoints = values[t] #.copy()

        #print domainIndexes
        #print t
        #print xPoints
        #print yPoints
        #print zPoints

        return variable, domainIndexes, domainPoints, xAxisLabel, yAxisLabel, zAxisLabel, xPoints, yPoints, zPoints, times[-1]

    #@QtCore.pyqtSlot()
    def slotSelectionChanged(self):
        self.variable = None
        self.hideAndClearAll()

        items = self.ui.treeWidget.selectedItems()
        if len(items) != 1:
            return
        selItem = items[0]

        varData = selItem.data(0, QtCore.Qt.UserRole)
        if isinstance(varData, QtCore.QVariant):
            if varData.isNull():
                return
            var = varData.toPyObject()
        else:
            var = varData
        if var == None:
            return

        domains = var.Domains
        times   = var.TimeValues
        values  = var.Values

        self.variable = var

        if len(domains) == 0:
            self.enableControls(0)
            self.insertTimeValues(times)
        elif len(domains) == 1:
            self.enableControls(1)
            self.insertTimeValues(times)
            self.insertDomainValues(1, domains[0])
        elif len(domains) == 2:
            self.enableControls(2)
            self.insertTimeValues(times)
            self.insertDomainValues(1, domains[0])
            self.insertDomainValues(2, domains[1])
        elif len(domains) == 3:
            self.enableControls(3)
            self.insertTimeValues(times)
            self.insertDomainValues(1, domains[0])
            self.insertDomainValues(2, domains[1])
            self.insertDomainValues(3, domains[2])
        elif len(domains) == 4:
            self.enableControls(4)
            self.insertTimeValues(times)
            self.insertDomainValues(1, domains[0])
            self.insertDomainValues(2, domains[1])
            self.insertDomainValues(3, domains[2])
            self.insertDomainValues(4, domains[3])
        elif len(domains) == 5:
            self.enableControls(5)
            self.insertTimeValues(times)
            self.insertDomainValues(1, domains[0])
            self.insertDomainValues(2, domains[1])
            self.insertDomainValues(3, domains[2])
            self.insertDomainValues(4, domains[3])
            self.insertDomainValues(5, domains[4])
        elif len(domains) == 6:
            self.enableControls(6)
            self.insertTimeValues(times)
            self.insertDomainValues(1, domains[0])
            self.insertDomainValues(2, domains[1])
            self.insertDomainValues(3, domains[2])
            self.insertDomainValues(4, domains[3])
            self.insertDomainValues(5, domains[4])
            self.insertDomainValues(6, domains[5])
        elif len(domains) == 7:
            self.enableControls(7)
            self.insertTimeValues(times)
            self.insertDomainValues(1, domains[0])
            self.insertDomainValues(2, domains[1])
            self.insertDomainValues(3, domains[2])
            self.insertDomainValues(4, domains[3])
            self.insertDomainValues(5, domains[4])
            self.insertDomainValues(6, domains[5])
            self.insertDomainValues(7, domains[6])
        elif len(domains) == 8:
            self.enableControls(8)
            self.insertTimeValues(times)
            self.insertDomainValues(1, domains[0])
            self.insertDomainValues(2, domains[1])
            self.insertDomainValues(3, domains[2])
            self.insertDomainValues(4, domains[3])
            self.insertDomainValues(5, domains[4])
            self.insertDomainValues(6, domains[5])
            self.insertDomainValues(7, domains[6])
            self.insertDomainValues(8, domains[7])
        else:
            pass

    def insertTimeValues(self, times):
        label, comboBox = self.getComboBoxAndLabel(0)
        label.setText("Time")
        comboBox.addItem("*", daeChooseVariable.FREE_DOMAIN) #QtCore.QVariant(daeChooseVariable.FREE_DOMAIN))
        if self.plotType == daeChooseVariable.plot2DAutoUpdated:
            comboBox.addItem("Current time", daeChooseVariable.LAST_TIME) #QtCore.QVariant(daeChooseVariable.LAST_TIME))
        for i in range(0, len(times)):
            comboBox.addItem(str(times[i]), i) #QtCore.QVariant(i))

    def insertDomainValues(self, n, domain):
        label, comboBox = self.getComboBoxAndLabel(n)
        names = domain.Name.split(".")
        label.setText(names[len(names)-1])
        comboBox.addItem("*", daeChooseVariable.FREE_DOMAIN) #QtCore.QVariant(daeChooseVariable.FREE_DOMAIN))
        if domain.Type == eUnstructuredGrid:
            coordinates = domain.Coordinates
            for i in range(0, domain.NumberOfPoints):
                comboBox.addItem(str(tuple(coordinates[i])), i) #QtCore.QVariant(i))
        else:
            points = domain.Points
            for i in range(0, domain.NumberOfPoints):
                comboBox.addItem(str(points[i]), i) #QtCore.QVariant(i))

    def getComboBoxAndLabel(self, index):
        if(index == 0):
            label       = self.ui.timeLabel
            comboBox    = self.ui.timeComboBox
        elif(index == 1):
            label       = self.ui.domain0Label
            comboBox    = self.ui.domain0ComboBox
        elif(index == 2):
            label       = self.ui.domain1Label
            comboBox    = self.ui.domain1ComboBox
        elif(index == 3):
            label       = self.ui.domain2Label
            comboBox    = self.ui.domain2ComboBox
        elif(index == 4):
            label       = self.ui.domain3Label
            comboBox    = self.ui.domain3ComboBox
        elif(index == 5):
            label       = self.ui.domain4Label
            comboBox    = self.ui.domain4ComboBox
        elif(index == 6):
            label       = self.ui.domain5Label
            comboBox    = self.ui.domain5ComboBox
        elif(index == 7):
            label       = self.ui.domain6Label
            comboBox    = self.ui.domain6ComboBox
        elif(index == 8):
            label       = self.ui.domain7Label
            comboBox    = self.ui.domain7ComboBox
        else:
            return (None, None)

        return (label, comboBox)

    def enableControls(self, n):
        if(n == 0):
            self.ui.timeLabel.setVisible(True)
            self.ui.timeComboBox.setVisible(True)
        elif(n == 1):
            self.ui.timeLabel.setVisible(True)
            self.ui.timeComboBox.setVisible(True)
            self.ui.domain0Label.setVisible(True)
            self.ui.domain0ComboBox.setVisible(True)
        elif(n == 2):
            self.ui.timeLabel.setVisible(True)
            self.ui.timeComboBox.setVisible(True)
            self.ui.domain0Label.setVisible(True)
            self.ui.domain0ComboBox.setVisible(True)
            self.ui.domain1Label.setVisible(True)
            self.ui.domain1ComboBox.setVisible(True)
        elif(n == 3):
            self.ui.timeLabel.setVisible(True)
            self.ui.timeComboBox.setVisible(True)
            self.ui.domain0Label.setVisible(True)
            self.ui.domain0ComboBox.setVisible(True)
            self.ui.domain1Label.setVisible(True)
            self.ui.domain1ComboBox.setVisible(True)
            self.ui.domain2Label.setVisible(True)
            self.ui.domain2ComboBox.setVisible(True)
        elif(n == 4):
            self.ui.timeLabel.setVisible(True)
            self.ui.timeComboBox.setVisible(True)
            self.ui.domain0Label.setVisible(True)
            self.ui.domain0ComboBox.setVisible(True)
            self.ui.domain1Label.setVisible(True)
            self.ui.domain1ComboBox.setVisible(True)
            self.ui.domain2Label.setVisible(True)
            self.ui.domain2ComboBox.setVisible(True)
            self.ui.domain3Label.setVisible(True)
            self.ui.domain3ComboBox.setVisible(True)
        elif(n == 5):
            self.ui.timeLabel.setVisible(True)
            self.ui.timeComboBox.setVisible(True)
            self.ui.domain0Label.setVisible(True)
            self.ui.domain0ComboBox.setVisible(True)
            self.ui.domain1Label.setVisible(True)
            self.ui.domain1ComboBox.setVisible(True)
            self.ui.domain2Label.setVisible(True)
            self.ui.domain2ComboBox.setVisible(True)
            self.ui.domain3Label.setVisible(True)
            self.ui.domain3ComboBox.setVisible(True)
            self.ui.domain4Label.setVisible(True)
            self.ui.domain4ComboBox.setVisible(True)
        elif(n == 6):
            self.ui.timeLabel.setVisible(True)
            self.ui.timeComboBox.setVisible(True)
            self.ui.domain0Label.setVisible(True)
            self.ui.domain0ComboBox.setVisible(True)
            self.ui.domain1Label.setVisible(True)
            self.ui.domain1ComboBox.setVisible(True)
            self.ui.domain2Label.setVisible(True)
            self.ui.domain2ComboBox.setVisible(True)
            self.ui.domain3Label.setVisible(True)
            self.ui.domain3ComboBox.setVisible(True)
            self.ui.domain4Label.setVisible(True)
            self.ui.domain4ComboBox.setVisible(True)
            self.ui.domain5Label.setVisible(True)
            self.ui.domain5ComboBox.setVisible(True)
        elif(n == 7):
            self.ui.timeLabel.setVisible(True)
            self.ui.timeComboBox.setVisible(True)
            self.ui.domain0Label.setVisible(True)
            self.ui.domain0ComboBox.setVisible(True)
            self.ui.domain1Label.setVisible(True)
            self.ui.domain1ComboBox.setVisible(True)
            self.ui.domain2Label.setVisible(True)
            self.ui.domain2ComboBox.setVisible(True)
            self.ui.domain3Label.setVisible(True)
            self.ui.domain3ComboBox.setVisible(True)
            self.ui.domain4Label.setVisible(True)
            self.ui.domain4ComboBox.setVisible(True)
            self.ui.domain5Label.setVisible(True)
            self.ui.domain5ComboBox.setVisible(True)
            self.ui.domain6Label.setVisible(True)
            self.ui.domain6ComboBox.setVisible(True)
        elif(n == 8):
            self.ui.timeLabel.setVisible(True)
            self.ui.timeComboBox.setVisible(True)
            self.ui.domain0Label.setVisible(True)
            self.ui.domain0ComboBox.setVisible(True)
            self.ui.domain1Label.setVisible(True)
            self.ui.domain1ComboBox.setVisible(True)
            self.ui.domain2Label.setVisible(True)
            self.ui.domain2ComboBox.setVisible(True)
            self.ui.domain3Label.setVisible(True)
            self.ui.domain3ComboBox.setVisible(True)
            self.ui.domain4Label.setVisible(True)
            self.ui.domain4ComboBox.setVisible(True)
            self.ui.domain5Label.setVisible(True)
            self.ui.domain5ComboBox.setVisible(True)
            self.ui.domain6Label.setVisible(True)
            self.ui.domain6ComboBox.setVisible(True)
            self.ui.domain7Label.setVisible(True)
            self.ui.domain7ComboBox.setVisible(True)
        else:
            pass

    def hideAndClearAll(self):
        del self.domainIndexes[:]
        self.ui.buttonOk.setEnabled(False)

        self.ui.timeLabel.setVisible(False)
        self.ui.timeComboBox.setVisible(False)
        self.ui.domain0Label.setVisible(False)
        self.ui.domain0ComboBox.setVisible(False)
        self.ui.domain1Label.setVisible(False)
        self.ui.domain1ComboBox.setVisible(False)
        self.ui.domain2Label.setVisible(False)
        self.ui.domain2ComboBox.setVisible(False)
        self.ui.domain3Label.setVisible(False)
        self.ui.domain3ComboBox.setVisible(False)
        self.ui.domain4Label.setVisible(False)
        self.ui.domain4ComboBox.setVisible(False)
        self.ui.domain5Label.setVisible(False)
        self.ui.domain5ComboBox.setVisible(False)
        self.ui.domain6Label.setVisible(False)
        self.ui.domain6ComboBox.setVisible(False)
        self.ui.domain7Label.setVisible(False)
        self.ui.domain7ComboBox.setVisible(False)

        self.ui.timeComboBox.clear()
        self.ui.domain0ComboBox.clear()
        self.ui.domain1ComboBox.clear()
        self.ui.domain2ComboBox.clear()
        self.ui.domain3ComboBox.clear()
        self.ui.domain4ComboBox.clear()
        self.ui.domain5ComboBox.clear()
        self.ui.domain6ComboBox.clear()
        self.ui.domain7ComboBox.clear()


