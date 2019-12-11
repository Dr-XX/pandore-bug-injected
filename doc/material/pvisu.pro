win32 {
	CONFIG += windows
        DEFINES += STANDALONE
}

unix {
	MOC_DIR = .moc
	OBJECTS_DIR = .obj
}

macx{
	MOC_DIR = .moc
	OBJECTS_DIR = .obj
}

TEMPLATE = app

LIBS += -lpandore -L$(PANDOREHOME)/lib
INCLUDEPATH=$(PANDOREHOME)/include

CONFIG += qt thread release warn_off

HEADERS += $(PANDOREHOME)/src/viewers/Qt/imagevisu.h
HEADERS += $(PANDOREHOME)/src/viewers/Qt/imagemodel.h
SOURCES += $(PANDOREHOME)/src/viewers/Qt/imagemodel.cpp

HEADERS += $(PANDOREHOME)/src/viewers/Qt/viewer.h
HEADERS += $(PANDOREHOME)/src/viewers/Qt/canvasview.h
SOURCES += $(PANDOREHOME)/src/viewers/Qt/canvasview.cpp
HEADERS += $(PANDOREHOME)/src/viewers/Qt/valuesview.h
SOURCES += $(PANDOREHOME)/src/viewers/Qt/valuesview.cpp
HEADERS += $(PANDOREHOME)/src/viewers/Qt/histogramview.h
SOURCES += $(PANDOREHOME)/src/viewers/Qt/histogramview.cpp
HEADERS += $(PANDOREHOME)/src/viewers/Qt/profileview.h
SOURCES += $(PANDOREHOME)/src/viewers/Qt/profileview.cpp
HEADERS += $(PANDOREHOME)/src/viewers/Qt/propertiesview.h
SOURCES += $(PANDOREHOME)/src/viewers/Qt/propertiesview.cpp

HEADERS += $(PANDOREHOME)/src/viewers/Qt/controller.h
HEADERS += $(PANDOREHOME)/src/viewers/Qt/planeselector.h
SOURCES += $(PANDOREHOME)/src/viewers/Qt/planeselector.cpp
HEADERS += $(PANDOREHOME)/src/viewers/Qt/thresholdselector.h
SOURCES += $(PANDOREHOME)/src/viewers/Qt/thresholdselector.cpp
HEADERS += $(PANDOREHOME)/src/viewers/Qt/zoomselector.h
SOURCES += $(PANDOREHOME)/src/viewers/Qt/zoomselector.cpp
HEADERS += $(PANDOREHOME)/src/viewers/Qt/dynamicselector.h
SOURCES += $(PANDOREHOME)/src/viewers/Qt/dynamicselector.cpp
HEADERS += $(PANDOREHOME)/src/viewers/Qt/colorselector.h
SOURCES += $(PANDOREHOME)/src/viewers/Qt/colorselector.cpp
HEADERS += $(PANDOREHOME)/src/viewers/Qt/rotateselector.h
SOURCES += $(PANDOREHOME)/src/viewers/Qt/rotateselector.cpp

HEADERS += $(PANDOREHOME)/src/viewers/Qt/infoview.h
SOURCES += $(PANDOREHOME)/src/viewers/Qt/infoview.cpp

SOURCES += test.cc

TARGET = ./essai

DEFINES += MAIN
