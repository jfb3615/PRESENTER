VERSION = 2.0.0

isEmpty(PREFIX) {
  PREFIX=/usr/local
}
  
isEmpty(QATLIBDIR) {
  QATLIBDIR=$$PREFIX/lib
}

TEMPLATE = lib dll
TARGET = QatSystemInterfaces
DEPENDPATH += .
INCLUDEPATH += . .. 
DESTDIR=../../../lib
CONFIG += qt release c++17

# Input
HEADERS += ../QatSystemInterfaces/*.h
SOURCES += SystemInterface.cpp Serializer.cpp

target.path=$$QATLIBDIR
INSTALLS += target

mac {
  PKG_CONFIG_PATH += $$[QT_INSTALL_LIBS]/pkgconfig
  PKG_CONFIG = PKG_CONFIG_PATH=$$PKG_CONFIG_PATH pkg-config
}

CONFIG    += link_pkgconfig
PKGCONFIG += QatPlotWidgets
