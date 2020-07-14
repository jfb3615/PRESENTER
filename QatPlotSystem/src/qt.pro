VERSION = 2.0.0

isEmpty(PREFIX) {
  PREFIX=/usr/local
}
  
isEmpty(QATSHAREDIR) {
  QATSHAREDIR=$$PREFIX/share
}

TEMPLATE = lib plugin
TARGET = QatPlotSystem
DEPENDPATH += . /usr/local/include /usr/include
INCLUDEPATH += . .. ../../QatSystemInterfaces 

DESTDIR=../../../plugins

CONFIG += plugin release c++17

LIBS += -L../../../lib  -lQatSystemInterfaces 
FORMS=PlotSystemMainWidget.ui PlotSystemMenu.ui

SOURCES += PlotSystem.cpp PlotSystemMainWidget.cpp PlotSystemMenu.cpp
HEADERS += ../QatPlotSystem/PlotSystem.h    ../QatPlotSystem/PlotSystemMainWidget.h ../QatPlotSystem/PlotSystemMenu.h

INSTALLS += target
target.path=$$QATSHAREDIR/qatplugins

mac {
  PKG_CONFIG_PATH += $$[QT_INSTALL_LIBS]/pkgconfig
  PKG_CONFIG = PKG_CONFIG_PATH=$$PKG_CONFIG_PATH pkg-config
}

QT += widgets
CONFIG    += link_pkgconfig
PKGCONFIG += QatPlotWidgets
