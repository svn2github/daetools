"""********************************************************************************
                            plot2d.py
                 DAE Tools: pyDAE module, www.daetools.com
                 Copyright (C) Dragan Nikolic, 2016
***********************************************************************************
DAE Tools is free software; you can redistribute it and/or modify it under the
terms of the GNU General Public License version 3 as published by the Free Software
Foundation. DAE Tools is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with the
DAE Tools software; if not, see <http://www.gnu.org/licenses/>.
********************************************************************************"""
import os, sys, numpy, json
from os.path import join, realpath, dirname
from PyQt4 import QtCore, QtGui

# Matplotlib imports
import matplotlib
matplotlib.use('Qt4Agg')
matplotlib.rcParams.update({'figure.autolayout': True})
from matplotlib.figure import Figure
from matplotlib.backends.backend_qt4agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.backends.backend_qt4agg import NavigationToolbar2QT as NavigationToolbar
import matplotlib.animation as animation

# daetools imports
from daetools.pyDAE import *
from .choose_variable import daeChooseVariable, daeTableDialog
from .plot_options import *

images_dir = join(dirname(__file__), 'images')

class daePlot2dDefaults:
    def __init__(self, color='black', linewidth=0.5, linestyle='solid', marker='o', markersize=6, markerfacecolor='black', markeredgecolor='black'):
        self.color = color
        self.linewidth = linewidth
        self.linestyle = linestyle
        self.marker = marker
        self.markersize = markersize
        self.markerfacecolor = markerfacecolor
        self.markeredgecolor = markeredgecolor

    def to_dict(self):
        d = {}
        d['color']           = self.color
        d['linewidth']       = self.linewidth
        d['linestyle']       = self.linestyle
        d['marker']          = self.marker
        d['markersize']      = self.markersize
        d['markerfacecolor'] = self.markerfacecolor
        d['markeredgecolor'] = self.markeredgecolor
        return d

    @classmethod
    def from_dict(cls, d):
        pd = daePlot2dDefaults()
        pd.color            = d['color']
        pd.linewidth        = float(d['linewidth'])
        pd.linestyle        = d['linestyle']
        pd.marker           = d['marker']
        pd.markersize       = int(d['markersize'])
        pd.markerfacecolor  = d['markerfacecolor']
        pd.markeredgecolor  = d['markeredgecolor']
        return pd

class dae2DPlot(QtGui.QDialog):
    plotDefaults = [daePlot2dDefaults('black', 0.5, 'solid', 'o', 6, 'black', 'black'),
                    daePlot2dDefaults('blue',  0.5, 'solid', 's', 6, 'blue',  'black'),
                    daePlot2dDefaults('red',   0.5, 'solid', '^', 6, 'red',   'black'),
                    daePlot2dDefaults('green', 0.5, 'solid', 'p', 6, 'green', 'black'),
                    daePlot2dDefaults('c',     0.5, 'solid', 'h', 6, 'c',     'black'),
                    daePlot2dDefaults('m',     0.5, 'solid', '*', 6, 'm',     'black'),
                    daePlot2dDefaults('k',     0.5, 'solid', 'd', 6, 'k',     'black'),
                    daePlot2dDefaults('y',     0.5, 'solid', 'x', 6, 'y',     'black'),

                    daePlot2dDefaults('black', 0.5, 'dashed', 'o', 6, 'black', 'black'),
                    daePlot2dDefaults('blue',  0.5, 'dashed', 's', 6, 'blue',  'black'),
                    daePlot2dDefaults('red',   0.5, 'dashed', '^', 6, 'red',   'black'),
                    daePlot2dDefaults('green', 0.5, 'dashed', 'p', 6, 'green', 'black'),
                    daePlot2dDefaults('c',     0.5, 'dashed', 'h', 6, 'c',     'black'),
                    daePlot2dDefaults('m',     0.5, 'dashed', '*', 6, 'm',     'black'),
                    daePlot2dDefaults('k',     0.5, 'dashed', 'd', 6, 'k',     'black'),
                    daePlot2dDefaults('y',     0.5, 'dashed', 'x', 6, 'y',     'black'),

                    daePlot2dDefaults('black', 0.5, 'dotted', 'o', 6, 'black', 'black'),
                    daePlot2dDefaults('blue',  0.5, 'dotted', 's', 6, 'blue',  'black'),
                    daePlot2dDefaults('red',   0.5, 'dotted', '^', 6, 'red',   'black'),
                    daePlot2dDefaults('green', 0.5, 'dotted', 'p', 6, 'green', 'black'),
                    daePlot2dDefaults('c',     0.5, 'dotted', 'h', 6, 'c',     'black'),
                    daePlot2dDefaults('m',     0.5, 'dotted', '*', 6, 'm',     'black'),
                    daePlot2dDefaults('k',     0.5, 'dotted', 'd', 6, 'k',     'black'),
                    daePlot2dDefaults('y',     0.5, 'dotted', 'x', 6, 'y',     'black') ]

    def __init__(self, parent, tcpipServer, updateInterval = 0, animated = False):
        QtGui.QDialog.__init__(self, parent, QtCore.Qt.Window)
        self.setAttribute(QtCore.Qt.WA_DeleteOnClose)

        self.tcpipServer = tcpipServer

        self.legendOn = True
        self.gridOn   = True
        self.curves   = []
        self.funcAnimation = None
        self._isAnimating = False
        self._timer = None

        if animated == True:
            self.updateInterval = updateInterval
            self.plotType = daeChooseVariable.plot2DAnimated
        elif updateInterval == 0:
            self.updateInterval = 0
            self.plotType = daeChooseVariable.plot2D
        else:
            self.updateInterval = int(updateInterval)
            self.plotType = daeChooseVariable.plot2DAutoUpdated

        self.setWindowTitle("2D plot")
        self.setWindowIcon(QtGui.QIcon(join(images_dir, 'line-chart.png')))

        exit = QtGui.QAction(QtGui.QIcon(join(images_dir, 'close.png')), 'Exit', self)
        exit.setShortcut('Ctrl+Q')
        exit.setStatusTip('Exit application')
        self.connect(exit, QtCore.SIGNAL('triggered()'), self.close)

        export = QtGui.QAction(QtGui.QIcon(join(images_dir, 'template.png')), 'Export template', self)
        export.setShortcut('Ctrl+X')
        export.setStatusTip('Export template')
        self.connect(export, QtCore.SIGNAL('triggered()'), self.slotExportTemplate)

        properties = QtGui.QAction(QtGui.QIcon(join(images_dir, 'preferences.png')), 'Options', self)
        properties.setShortcut('Ctrl+P')
        properties.setStatusTip('Options')
        self.connect(properties, QtCore.SIGNAL('triggered()'), self.slotProperties)

        grid = QtGui.QAction(QtGui.QIcon(join(images_dir, 'grid.png')), 'Grid on/off', self)
        grid.setShortcut('Ctrl+G')
        grid.setStatusTip('Grid on/off')
        self.connect(grid, QtCore.SIGNAL('triggered()'), self.slotToggleGrid)

        legend = QtGui.QAction(QtGui.QIcon(join(images_dir, 'legend.png')), 'Legend on/off', self)
        legend.setShortcut('Ctrl+L')
        legend.setStatusTip('Legend on/off')
        self.connect(legend, QtCore.SIGNAL('triggered()'), self.slotToggleLegend)

        viewdata = QtGui.QAction(QtGui.QIcon(join(images_dir, 'data.png')), 'View tabular data', self)
        viewdata.setShortcut('Ctrl+D')
        viewdata.setStatusTip('View tabular data')
        self.connect(viewdata, QtCore.SIGNAL('triggered()'), self.slotViewTabularData)

        export_csv = QtGui.QAction(QtGui.QIcon(join(images_dir, 'csv.png')), 'Export CSV', self)
        export_csv.setShortcut('Ctrl+S')
        export_csv.setStatusTip('Export CSV')
        self.connect(export_csv, QtCore.SIGNAL('triggered()'), self.slotExportCSV)

        remove_line = QtGui.QAction(QtGui.QIcon(join(images_dir, 'remove.png')), 'Remove line', self)
        remove_line.setShortcut('Ctrl+R')
        remove_line.setStatusTip('Remove line')
        self.connect(remove_line, QtCore.SIGNAL('triggered()'), self.slotRemoveLine)

        new_line = QtGui.QAction(QtGui.QIcon(join(images_dir, 'add.png')), 'Add line', self)
        new_line.setShortcut('Ctrl+A')
        new_line.setStatusTip('Add line')
        self.connect(new_line, QtCore.SIGNAL('triggered()'), self.newCurve)

        play_animation = QtGui.QAction(QtGui.QIcon(join(images_dir, 'media-playback-start.png')), 'Start animation', self)
        play_animation.setShortcut('Ctrl+S')
        play_animation.setStatusTip('Start animation')
        self.connect(play_animation, QtCore.SIGNAL('triggered()'), self.playAnimation)
        self.play_animation = play_animation # save it

        stop_animation = QtGui.QAction(QtGui.QIcon(join(images_dir, 'media-playback-stop.png')), 'Stop animation', self)
        stop_animation.setShortcut('Ctrl+E')
        stop_animation.setStatusTip('Stop animation')
        self.connect(stop_animation, QtCore.SIGNAL('triggered()'), self.stopAnimation)
        self.stop_animation = stop_animation # save it

        export_video = QtGui.QAction(QtGui.QIcon(join(images_dir, 'save-video.png')), 'Export video/sequence of images', self)
        export_video.setShortcut('Ctrl+V')
        export_video.setStatusTip('Export video/sequence of images')
        self.connect(export_video, QtCore.SIGNAL('triggered()'), self.exportVideo)

        self.actions_to_disable = [export, viewdata, export_csv, grid, legend, new_line, remove_line, properties]

        self.toolbar_widget = QtGui.QWidget(self)
        layoutToolbar = QtGui.QVBoxLayout(self.toolbar_widget)
        layoutToolbar.setContentsMargins(0,0,0,0)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Fixed)
        self.toolbar_widget.setSizePolicy(sizePolicy)

        layoutPlot = QtGui.QVBoxLayout(self)
        layoutPlot.setContentsMargins(2,2,2,2)
        self.figure = Figure((7.5, 6), dpi=100, facecolor='white')#"#E5E5E5")
        self.canvas = FigureCanvas(self.figure)
        self.canvas.setParent(self)
        self.canvas.axes = self.figure.add_subplot(111)

        self.mpl_toolbar = NavigationToolbar(self.canvas, self.toolbar_widget, False)

        #self.mpl_toolbar.addSeparator()
        self.mpl_toolbar.addAction(export)
        self.mpl_toolbar.addAction(export_csv)
        self.mpl_toolbar.addAction(viewdata)
        self.mpl_toolbar.addSeparator()
        self.mpl_toolbar.addAction(grid)
        self.mpl_toolbar.addAction(legend)
        self.mpl_toolbar.addSeparator()
        self.mpl_toolbar.addAction(new_line)
        self.mpl_toolbar.addAction(remove_line)
        self.mpl_toolbar.addSeparator()
        self.mpl_toolbar.addAction(properties)
        #self.mpl_toolbar.addSeparator()
        #self.mpl_toolbar.addAction(exit)
        if self.plotType == daeChooseVariable.plot2DAnimated:
            self.mpl_toolbar.addSeparator()
            self.mpl_toolbar.addAction(play_animation)
            self.mpl_toolbar.addAction(stop_animation)
            self.mpl_toolbar.addAction(export_video)

        self.fp9  = matplotlib.font_manager.FontProperties(family='sans-serif', style='normal', variant='normal', weight='normal', size=9)
        self.fp10 = matplotlib.font_manager.FontProperties(family='sans-serif', style='normal', variant='normal', weight='normal', size=10)
        self.fp12 = matplotlib.font_manager.FontProperties(family='sans-serif', style='normal', variant='normal', weight='normal', size=12)

        self.textTime = self.figure.text(0.01, 0.01, '', fontproperties = self.fp10)

        self.xtransform = 1.0
        self.ytransform = 1.0

        for xlabel in self.canvas.axes.get_xticklabels():
            xlabel.set_fontproperties(self.fp10)
        for ylabel in self.canvas.axes.get_yticklabels():
            ylabel.set_fontproperties(self.fp10)

        layoutToolbar.addWidget(self.mpl_toolbar)
        layoutPlot.addWidget(self.canvas)
        layoutPlot.addWidget(self.toolbar_widget)

        if animated == False and self.updateInterval > 0:
            self._timer = QtCore.QTimer()
            QtCore.QObject.connect(self._timer, QtCore.SIGNAL('timeout()'), self.updateCurves)
            self._timer.start(self.updateInterval)

    def closeEvent(self, event):
        #print("dae2DPlot.closeEvent")
        if self.funcAnimation:
            self.funcAnimation.event_source.stop()

        if self._timer:
            self._timer.stop()

        return QtGui.QDialog.closeEvent(self, event)

    def updateCurves(self):
        try:
            for line, variable, domainIndexes, domainPoints, fun in self.curves:
                results = fun(variable, domainIndexes, domainPoints)
                if self.xtransform != 1:
                    xPoints = numpy.array(results[5])*self.xtransform
                else:
                    xPoints = results[5]

                if self.ytransform != 1:
                    yPoints = numpy.array(results[6])*self.ytransform
                else:
                    yPoints = results[6]

                currentTime = results[7]

                line.set_xdata(xPoints)
                line.set_ydata(yPoints)

                if self.textTime:
                    t = 'Time = {0} s'.format(currentTime)
                    self.textTime.set_text(t)

            self.reformatPlot()

        except Exception as e:
            print((str(e)))

    #@QtCore.pyqtSlot()
    def slotExportTemplate(self):
        try:
            curves = []
            template = {'curves':          curves,
                        'updateInterval' : self.updateInterval,
                        'xlabel' :         self.canvas.axes.get_xlabel(),
                        'xmin' :           self.canvas.axes.get_xlim()[0],
                        'xmax' :           self.canvas.axes.get_xlim()[1],
                        'xscale' :         self.canvas.axes.get_xscale(),
                        'xtransform':      1.0,
                        'ylabel' :         self.canvas.axes.get_ylabel(),
                        'ymin' :           self.canvas.axes.get_ylim()[0],
                        'ymax' :           self.canvas.axes.get_ylim()[1],
                        'yscale' :         self.canvas.axes.get_yscale(),
                        'ytransform':      1.0,
                        'legendOn' :       self.legendOn,
                        'gridOn' :         self.gridOn,
                        'plotTitle' :      self.canvas.axes.get_title(),
                        'windowTitle' :    str(self.windowTitle())
                       }

            for line, variable, domainIndexes, domainPoints, fun in self.curves:
                # variableName, indexes, points, linelabel, style = {linecolor, linewidth, linestyle, marker, markersize, markerfacecolor, markeredgecolor}
                style = daePlot2dDefaults(line.get_color(), line.get_linewidth(), line.get_linestyle(),
                                          line.get_marker(), line.get_markersize(), line.get_markerfacecolor(), line.get_markeredgecolor())
                curves.append((variable.Name, domainIndexes, domainPoints, line.get_label(), style.to_dict()))

            s = json.dumps(template, indent=2, sort_keys=True)

            filename = QtGui.QFileDialog.getSaveFileName(self, "Save 2D plot template", "template.pt", "Templates (*.pt)")
            if not filename:
                return

            f = open(filename, 'w')
            f.write(s)
            f.close()

        except Exception as e:
            print((str(e)))

    #@QtCore.pyqtSlot()
    def slotProperties(self):
        figure_edit(self.canvas, self)

    #@QtCore.pyqtSlot()
    def slotToggleLegend(self):
        self.legendOn = not self.legendOn
        self.reformatPlot()

    #@QtCore.pyqtSlot()
    def slotToggleGrid(self):
        self.gridOn = not self.gridOn
        self.reformatPlot()

    #@QtCore.pyqtSlot()
    def slotExportCSV(self):
        strInitialFilename = QtCore.QDir.current().path()
        strInitialFilename += "/untitled.csv";
        strExt = "Comma separated files (*.csv)"
        strCaption = "Save file"
        fileName = QtGui.QFileDialog.getSaveFileName(self, strCaption, strInitialFilename, strExt)
        if(fileName.isEmpty()):
            return

        datafile = open(str(fileName), 'w')
        lines = self.canvas.axes.get_lines()

        for line in lines:
            xlabel = self.canvas.axes.get_xlabel()
            ylabel = line.get_label()
            x = line.get_xdata()
            y = line.get_ydata()
            datafile.write('\"' + xlabel + '\",\"' + ylabel + '\"\n' )
            for i in range(0, len(x)):
                datafile.write('%.14e,%.14e\n' % (x[i], y[i]))
                #datafile.write(str(x[i]) + ',' + str(y[i]) + '\n')
            datafile.write('\n')

    #@QtCore.pyqtSlot()
    def slotViewTabularData(self):
        lines = self.canvas.axes.get_lines()

        tableDialog = daeTableDialog(self)
        tableDialog.setWindowTitle('Raw data')
        table = tableDialog.ui.tableWidget
        nr = 0
        ncol = len(lines)
        for line in lines:
            n = len(line.get_xdata())
            if nr < n:
                nr = n

        xlabel = self.canvas.axes.get_xlabel()
        table.setRowCount(nr)
        table.setColumnCount(ncol)
        horHeader = []
        verHeader = []
        for i, line in enumerate(lines):
            xlabel = self.canvas.axes.get_xlabel()
            ylabel = line.get_label()
            x = line.get_xdata()
            y = line.get_ydata()
            horHeader.append(ylabel)
            for k in range(0, len(x)):
                newItem = QtGui.QTableWidgetItem(str(y[k]))
                table.setItem(k, i, newItem)
            for k in range(0, len(x)):
                verHeader.append(str(x[k]))

        table.setHorizontalHeaderLabels(horHeader)
        table.setVerticalHeaderLabels(verHeader)
        table.resizeRowsToContents()
        tableDialog.exec_()

    #@QtCore.pyqtSlot()
    def slotRemoveLine(self):
        lines = self.canvas.axes.get_lines()
        items = []
        for line in lines:
            label = line.get_label()
            items.append(label)

        nameToRemove, ok = QtGui.QInputDialog.getItem(self, "Choose line to remove", "Lines:", items, 0, False)
        if ok:
            for i, line in enumerate(lines):
                label = line.get_label()
                if label == str(nameToRemove):
                    self.canvas.axes.lines.pop(i)
                    self.reformatPlot()
                    return

    def newFromTemplate(self, template):
        """
        template is a dictionary:
        {
           'curves' : [variableName, domainIndexes, domainPoints, lineTitle, style],
           'updateInterval' : float,
           'xlabel' : string,
           'xmin' : float,
           'xmax' : float,
           'xscale' : string [linear, log],
           'xtransform': float,
           'ylabel' : string,
           'ymin' : float,
           'ymax' : float,
           'yscale' : string [linear, log],
           'ytransform': float,
           'legendOn' : Bool,
           'gridOn' : Bool,
           'plotTitle' : string,
           'windowTitle' : string
        }
        """
        if len(self.tcpipServer.DataReceivers) == 0:
            return

        processes = {}
        for dataReceiver in self.tcpipServer.DataReceivers:
            processes[dataReceiver.Process.Name] = dataReceiver.Process

        if len(template) == 0:
            return False

        curves = template['curves']

        if 'xtransform' in template:
            self.xtransform = template['xtransform']
        if 'ytransform' in template:
            self.ytransform = template['ytransform']

        for i, curve in enumerate(curves):
            variableName  = curve[0]
            domainIndexes = curve[1]
            domainPoints  = curve[2]
            label         = None
            pd            = None
            if len(curve) > 3:
                label = curve[3]
            if len(curve) > 4:
                pd = daePlot2dDefaults.from_dict(curve[4])

            windowTitle     = "Select process for variable {0} (of {1})".format(i+1, len(curves))
            var_to_look_for = "Variable: {0}({1})".format(variableName, ','.join(domainPoints))
            items       = sorted(processes.keys())
            processName, ok = self.showSelectProcessDialog(windowTitle, var_to_look_for, items)
            if not ok:
                return False

            process = processes[str(processName)]
            for variable in process.Variables:
                if variableName == variable.Name:
                    variable, domainIndexes, domainPoints, xAxisLabel, yAxisLabel, xPoints, yPoints, currentTime = daeChooseVariable.get2DData(variable, domainIndexes, domainPoints)
                    self._addNewCurve(variable, domainIndexes, domainPoints, xAxisLabel, yAxisLabel, xPoints, yPoints, currentTime, label, pd)
                    break

        if 'xlabel' in template:
            self.canvas.axes.set_xlabel(template['xlabel'], fontproperties=self.fp12)
        if 'xmin' in template and 'xmax' in template:
            self.canvas.axes.set_xlim(float(template['xmin']), float(template['xmax']))
        if 'xscale' in template:
            self.canvas.axes.set_xscale(template['xscale'])

        if 'ylabel' in template:
            self.canvas.axes.set_ylabel(template['ylabel'], fontproperties=self.fp12)
        if 'ymin' in template and 'ymax' in template:
            self.canvas.axes.set_ylim(float(template['ymin']), float(template['ymax']))
        if 'yscale' in template:
            self.canvas.axes.set_yscale(template['yscale'])

        if 'gridOn' in template:
            self.gridOn = template['gridOn']
            self.canvas.axes.grid(self.gridOn)

        if 'legendOn' in template:
            self.legendOn = template['legendOn']
            if self.legendOn:
                self.canvas.axes.legend(loc = 0, prop=self.fp9, numpoints = 1, fancybox=True)
            else:
                self.canvas.axes.legend_ = None

        if 'plotTitle' in template:
            self.canvas.axes.set_title(template['plotTitle'])

        if 'windowTitle' in template:
            self.setWindowTitle(template['windowTitle'])

        #fmt = matplotlib.ticker.ScalarFormatter(useOffset = False)
        #fmt.set_scientific(False)
        #fmt.set_powerlimits((-3, 4))
        #self.canvas.axes.xaxis.set_major_formatter(fmt)
        #self.canvas.axes.yaxis.set_major_formatter(fmt)

        self.figure.tight_layout()
        self.canvas.draw()

        return True

    def showSelectProcessDialog(self, windowTitle, label, items):
        dlg = QtGui.QInputDialog(self)
        dlg.resize(500, 300)
        dlg.setWindowTitle(windowTitle)
        dlg.setLabelText(label)
        dlg.setComboBoxItems(items)

        dlg.setComboBoxEditable(False)
        dlg.setOption(QtGui.QInputDialog.UseListViewForComboBoxItems)

        if dlg.exec_() == QtGui.QDialog.Accepted:
            return str(dlg.textValue()), True
        else:
            return '', False

    def _updateFrame(self, frame):
        curve = self.curves[0]
        line    = curve[0]
        yPoints = curve[5]
        times   = curve[4]
        yData = yPoints[frame]
        line.set_ydata(yData)
        time = times[frame]
        self.canvas.axes.set_title('time = %f s' % time, fontproperties=self.fp10)

        if frame == len(times)-1: # the last frame
            for action in self.actions_to_disable:
                action.setEnabled(True)

            del self.funcAnimation
            self.funcAnimation = None

            self.play_animation.setIcon(QtGui.QIcon(join(images_dir, 'media-playback-start.png')))
            self.play_animation.setStatusTip('Start animation')
            self.play_animation.setText('Start animation')
            self._isAnimating = False

        return line,

    def _startAnimation(self):
        if len(self.curves) != 1:
            return

        # Set properties for the frame 0
        curve = self.curves[0]
        line    = curve[0]
        yPoints = curve[5]
        times   = curve[4]
        ymin = numpy.min(yPoints)
        ymax = numpy.max(yPoints)
        dy = 0.5 * (ymax-ymin)*0.05
        self.canvas.axes.set_ylim(ymin-dy, ymax+dy)
        self.canvas.axes.set_title('time = %f s' % times[0], fontproperties=self.fp10)

        frames = numpy.arange(1, len(times))
        self.funcAnimation = animation.FuncAnimation(self.figure,
                                                     self._updateFrame,
                                                     frames,
                                                     interval=self.updateInterval,
                                                     blit=False,
                                                     repeat=False)
        self.play_animation.setIcon(QtGui.QIcon(join(images_dir, 'media-playback-pause.png')))
        self.play_animation.setStatusTip('Pause animation')
        self.play_animation.setText('Pause animation')
        self._isAnimating = True
        #At the end do not call chow() nor save(), they will be ran by a caller

    #@QtCore.pyqtSlot()
    def playAnimation(self):
        if self.funcAnimation: # animation started - pause/resume it
            if self._isAnimating: # pause it
                for action in self.actions_to_disable:
                    action.setEnabled(True)
                self.funcAnimation.event_source.stop()
                self.play_animation.setIcon(QtGui.QIcon(join(images_dir, 'media-playback-start.png')))
                self.play_animation.setStatusTip('Start animation')
                self.play_animation.setText('Start animation')
                self._isAnimating = False
                self.canvas.draw()
            else: # restart it
                for action in self.actions_to_disable:
                    action.setEnabled(False)
                self.funcAnimation.event_source.start()
                self.play_animation.setIcon(QtGui.QIcon(join(images_dir, 'media-playback-pause.png')))
                self.play_animation.setStatusTip('Pause animation')
                self.play_animation.setText('Pause animation')
                self._isAnimating = True
                self.canvas.draw()

        else: # start animation
            for action in self.actions_to_disable:
                action.setEnabled(False)
            self._startAnimation()
            self.canvas.draw()

    #@QtCore.pyqtSlot()
    def stopAnimation(self):
        if self.funcAnimation: # animated started - stop it
            for action in self.actions_to_disable:
                action.setEnabled(True)

            self.funcAnimation.event_source.stop()
            del self.funcAnimation
            self.funcAnimation = None

            self.play_animation.setIcon(QtGui.QIcon(join(images_dir, 'media-playback-start.png')))
            self.play_animation.setStatusTip('Start animation')
            self.play_animation.setText('Start animation')
            self._isAnimating = False

        # Go back to frame 0
        self._updateFrame(0)
        self.canvas.draw()

    #@QtCore.pyqtSlot()
    def exportVideo(self):
        import save_video
        dlg = save_video.daeSavePlot2DVideo()
        for enc in sorted(animation.writers.list()):
            dlg.ui.comboEncoders.addItem(str(enc))
        dlg.ui.lineeditCodec.setText('')
        dlg.ui.lineeditFilename.setText(os.path.join(os.path.expanduser('~'), 'video.avi'))
        dlg.ui.spinFPS.setValue(10)
        dlg.ui.lineeditExtraArgs.setText(json.dumps([])) # ['-pix_fmt', 'yuv420p']
        dlg.ui.spinBitrate.setValue(-1)

        if dlg.exec_() != QtGui.QDialog.Accepted:
            return False

        filename   = str(dlg.ui.lineeditFilename.text())
        fps        = int(dlg.ui.spinFPS.value())
        encoder    = str(dlg.ui.comboEncoders.currentText())
        codec      = str(dlg.ui.lineeditCodec.text())
        bitrate    = int(dlg.ui.spinBitrate.value())
        extra_args = []
        try:
            extra_args = list(json.loads(str(dlg.ui.lineeditExtraArgs.text())))
        except:
            pass
        if bitrate == -1:
            bitrate = None
        if codec == '':
            codec = None
        if not extra_args:
            extra_args = None

        print('%s(fps = %s, codec = %s, bitrate = %s, extra_args = %s) -> %s' % (encoder, fps, codec, bitrate, extra_args, filename))

        # First stop the existing animation, if already started
        self.stopAnimation()
        Writer = animation.writers[encoder]
        writer = Writer(fps = fps, codec = codec, bitrate = bitrate, extra_args = extra_args)
        self._startAnimation()
        self.funcAnimation.save(filename, writer=writer)

    #@QtCore.pyqtSlot()
    def newCurve(self):
        processes = [dataReceiver.Process for dataReceiver in self.tcpipServer.DataReceivers]

        cv = daeChooseVariable(processes, self.plotType)
        cv.setWindowTitle('Choose variable for 2D plot')
        if cv.exec_() != QtGui.QDialog.Accepted:
            return False

        variable, domainIndexes, domainPoints, xAxisLabel, yAxisLabel, xPoints, yPoints, currentTime = cv.getPlot2DData()
        self._addNewCurve(variable, domainIndexes, domainPoints, xAxisLabel, yAxisLabel, xPoints, yPoints, currentTime, None, None)

        #fmt = matplotlib.ticker.ScalarFormatter(useOffset = False)
        #fmt.set_scientific(False)
        #fmt.set_powerlimits((-3, 4))
        #self.canvas.axes.xaxis.set_major_formatter(fmt)
        #self.canvas.axes.yaxis.set_major_formatter(fmt)

        return True

    #@QtCore.pyqtSlot()
    def newAnimatedCurve(self):
        processes = [dataReceiver.Process for dataReceiver in self.tcpipServer.DataReceivers]

        cv = daeChooseVariable(processes, self.plotType)
        cv.setWindowTitle('Choose variable for animated 2D plot')
        if cv.exec_() != QtGui.QDialog.Accepted:
            return False

        variable, domainIndexes, domainPoints, xAxisLabel, yAxisLabel, xPoints, yPoints, times = cv.getPlot2DAnimatedData()
        self._addNewAnimatedCurve(variable, domainIndexes, domainPoints, xAxisLabel, yAxisLabel, xPoints, yPoints, times, None, None)

        return True

    def _addNewAnimatedCurve(self, variable, domainIndexes, domainPoints, xAxisLabel, yAxisLabel, xPoints, yPoints_2D, times, label = None, pd = None):
        domains = '(' + ', '.join(domainPoints) + ')'

        if not label:
            label = variable.Name.replace("&", "").replace(";", "") + domains

        line = self.addLine(xAxisLabel, yAxisLabel, xPoints, yPoints_2D[0], label, pd)
        self.setWindowTitle(label)

        self.curves.append( (line, variable, domainIndexes, domainPoints, times, yPoints_2D) )

    def _addNewCurve(self, variable, domainIndexes, domainPoints, xAxisLabel, yAxisLabel, xPoints, yPoints, currentTime, label = None, pd = None):
        domains = "("
        for i in range(0, len(domainPoints)):
            if i != 0:
                domains += ", "
            domains += domainPoints[i]
        domains += ")"

        if not label:
            label = variable.Name.replace("&", "").replace(";", "") + domains

        line = self.addLine(xAxisLabel, yAxisLabel, xPoints, yPoints, label, pd)
        self.setWindowTitle(label)

        self.curves.append( (line, variable, domainIndexes, domainPoints, daeChooseVariable.get2DData) )

    def addLine(self, xAxisLabel, yAxisLabel, xPoints, yPoints, label, pd):
        no_lines = len(self.canvas.axes.get_lines())
        if not pd:
            n = no_lines % len(dae2DPlot.plotDefaults)
            pd = dae2DPlot.plotDefaults[n]

        xPoints_ = numpy.array(xPoints) * self.xtransform
        yPoints_ = numpy.array(yPoints) * self.ytransform

        line, = self.canvas.axes.plot(xPoints_, yPoints_, label=label, color=pd.color, linewidth=pd.linewidth, \
                                      linestyle=pd.linestyle, marker=pd.marker, markersize=pd.markersize, \
                                      markerfacecolor=pd.markerfacecolor, markeredgecolor=pd.markeredgecolor)

        if no_lines == 0: # why this?
            self.canvas.axes.set_xlabel(xAxisLabel, fontproperties=self.fp12)
            self.canvas.axes.set_ylabel(yAxisLabel, fontproperties=self.fp12)
            t = self.canvas.axes.xaxis.get_offset_text()
            t.set_fontproperties(self.fp10)
            t = self.canvas.axes.yaxis.get_offset_text()
            t.set_fontproperties(self.fp10)

        self.reformatPlot()
        return line

    def reformatPlot(self):
        lines = self.canvas.axes.get_lines()
        xmin = 1e20
        xmax = -1e20
        ymin = 1e20
        ymax = -1e20
        for line in lines:
            if numpy.min(line.get_xdata()) < xmin:
                xmin = numpy.min(line.get_xdata())
            if numpy.max(line.get_xdata()) > xmax:
                xmax = numpy.max(line.get_xdata())

            if numpy.min(line.get_ydata()) < ymin:
                ymin = numpy.min(line.get_ydata())
            if numpy.max(line.get_ydata()) > ymax:
                ymax = numpy.max(line.get_ydata())

        dx = (xmax - xmin) * 0.05
        dy = (ymax - ymin) * 0.05
        xmin -= dx
        xmax += dx
        ymin -= dy
        ymax += dy

        self.canvas.axes.set_xlim(xmin, xmax)
        self.canvas.axes.set_ylim(ymin, ymax)

        self.canvas.axes.grid(self.gridOn)

        if self.legendOn:
            self.canvas.axes.legend(loc = 0, prop=self.fp9, numpoints = 1, fancybox=True)
        else:
            self.canvas.axes.legend_ = None

        self.figure.tight_layout()
        self.canvas.draw()
