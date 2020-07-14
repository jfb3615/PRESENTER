VERSION = 2.0.0

isEmpty(PREFIX) {
  PREFIX=/usr/local
}
  
isEmpty(QATSHAREDIR) {
  QATSHAREDIR=$$PREFIX/share
}

TEMPLATE = lib plugin
TARGET = QatSpreadsheetSystem
DEPENDPATH += .
INCLUDEPATH += . .. ../../QatSystemInterfaces 

DESTDIR=../../../plugins

CONFIG += plugin release c++17

LIBS += -L../../../lib  -lQatSystemInterfaces 
FORMS=SpreadsheetSystemMainWidget.ui

SOURCES += SpreadsheetSystem.cpp SpreadsheetSystemMainWidget.cpp 
HEADERS += ../QatSpreadsheetSystem/SpreadsheetSystem.h    ../QatSpreadsheetSystem/SpreadsheetSystemMainWidget.h

INSTALLS += target
target.path=$$QATSHAREDIR/qatplugins

mac {
  PKG_CONFIG_PATH += $$[QT_INSTALL_LIBS]/pkgconfig
  PKG_CONFIG = PKG_CONFIG_PATH=$$PKG_CONFIG_PATH pkg-config
}


QT += widgets 
CONFIG    += link_pkgconfig
PKGCONFIG += QatPlotWidgets
