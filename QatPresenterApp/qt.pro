VERSION = 2.0.0

isEmpty(PREFIX) {
  PREFIX=/usr/local
}
  
isEmpty(QATBINDIR) {
  QATBINDIR=$$PREFIX/bin
}

TEMPLATE = app
TARGET = present
DEPENDPATH += .
DESTDIR=../../bin
INCLUDEPATH += . ../QatPresenter ../QatSystemInterfaces
CONFIG += qt release c++17
LIBS += -L../../lib -lQatPresenter -lQatSystemInterfaces   


SOURCES += present.cpp

target.path=$$QATBINDIR

INSTALLS += target

CONFIG    += link_pkgconfig
PKGCONFIG += QatPlotWidgets

mac{
  PKG_CONFIG_PATH += $$[QT_INSTALL_LIBS]/pkgconfig
  PKG_CONFIG = PKG_CONFIG_PATH=$$PKG_CONFIG_PATH pkg-config
  CONFIG -= app_bundle
}

