#This is a dummy job! I use this qt file just to get system configuration.

TEMPLATE = lib dll
TARGET =  XXXXXX
DESTDIR = YYYYYY
DEPENDPATH += .
INCLUDEPATH += . ..  /usr/local/include

LIBS += -L/usr/local/lib -lQatPlotWidgets -lQatPlotting -lQatDataModeling  -lQatDataAnalysis -lQatGenericFunctions -lgsl -lgslcblas -ldl

CONFIG += qt debug 
QT     += widgets

# Input
SOURCES += XXXXXX.cc

mac {
  CONFIG -= app_bundle
  CONFIG += c++11
  INCLUDEPATH += /usr/local/include/eigen3
}

unix {
  QMAKE_DEL_FILE=rm -rf
  QMAKE_DISTCLEAN += ../local
  INCLUDEPATH += /usr/include/eigen3
  QMAKE_CXXFLAGS += -std=c++11
}
