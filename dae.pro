TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = Core \
    DataReporting \
    Activity \
    IDAS_DAESolver \ 
    BONMIN_MINLPSolver \
#    IPOPT_NLPSolver \
	pyCore \
	pyActivity \
	pyDataReporting \
	pyIDAS \
	pyIPOPT \ 
	pyBONMIN
 
HEADERS += dae.h \
    dae_develop.h
