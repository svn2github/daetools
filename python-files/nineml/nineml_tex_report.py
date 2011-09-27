#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
import nineml
from nineml.abstraction_layer.testing_utils import RecordValue, TestableComponent
from nineml.abstraction_layer import ComponentClass
from nineml.abstraction_layer.testing_utils import std_pynn_simulation
import os, sys, subprocess
from time import localtime, strftime, time
from daetools.pyDAE import *
from nineml_component_inspector import nineml_component_inspector

class texCommand:
    def __init__(self, startCommand, endCommand):
        self.startCommand = startCommand
        self.endCommand   = endCommand

    def __call__(self, content):
        return self.startCommand + content + self.endCommand

tex_itemize  = texCommand('\\begin{itemize}\n', '\\end{itemize}\n')
tex_verbatim = texCommand('\\begin{verbatim}', '\\end{verbatim} ')

def createLatexReport(inspector, tests, texTemplate, texOutputFile, find_files_dir = '.'):
    tf = open(texTemplate, 'r')
    template = ''.join(tf.readlines())
    tf.close()
    of = open(texOutputFile, 'w')
    content, tests_content = inspector.generateLatexReport(tests)

    comp_name = inspector.ninemlComponent.name.replace('_', '\\_')
    template  = template.replace('MODEL-NAME', comp_name)
    template  = template.replace('MODEL-SPECIFICATION', content)
    template  = template.replace('TESTS', tests_content)
    template  = template.replace('APPENDIXES', '')
    #template  = template.replace('FILES_PATH', find_files_dir)
    
    of.write(template)
    of.close()

def createPDF(texFile, outdir = None):
    # Run pdflatex twice because of the problems with the Table Of Contents (we need two passes)
    if outdir:
        res = os.system('/usr/bin/pdflatex -interaction=nonstopmode -output-directory {0} {1}'.format(outdir, texFile))
        res = os.system('/usr/bin/pdflatex -interaction=nonstopmode -output-directory {0} {1}'.format(outdir, texFile))
    else:
        res = os.system('/usr/bin/pdflatex -interaction=nonstopmode {0}'.format(texFile))
        res = os.system('/usr/bin/pdflatex -interaction=nonstopmode {0}'.format(texFile))
        
    return res

if __name__ == "__main__":
    component = 'hierachical_iaf_1coba'
    tex = component + '.tex'
    pdf = component + '.pdf'
    nineml_component = TestableComponent(component)()
    inspector = nineml_component_inspector()
    inspector.inspect(nineml_component)
    createLatexReport(inspector, [], 'nineml-tex-template.tex', tex)
    res = createPDF(tex)
    subprocess.call(['evince', pdf], shell=False)

