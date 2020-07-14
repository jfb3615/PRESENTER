VERSION = 2.0.0

isEmpty(PREFIX) {
  PREFIX=/usr/local
}
  
isEmpty(QATLIBDIR) {
  QATLIBDIR=$$PREFIX/lib
}
 
TEMPLATE = lib dll
TARGET = QatPresenter
DEPENDPATH += .
DESTDIR=../../../lib
LIBS        += -L$(DESTDIR) -lQatSystemInterfaces 
INCLUDEPATH += . .. ../../QatSystemInterfaces
CONFIG += qt release c++17

# Mysterious problem:  when the compilation unit PresenterApplication.cpp is
# optimized, presenter fails to load the RootDriver.  No explanation. Next
# line is the workaround. 
QMAKE_CXXFLAGS_RELEASE -= -O2

# Input
HEADERS += ../QatPresenter/*.h
SOURCES += PresenterTreeWidget.cpp QatTabBar.cpp QatTabWidget.cpp PresenterApplication.cpp
FORMS   += PresenterApplication.ui

target.path=$$QATLIBDIR
INSTALLS += target

mac {
  PKG_CONFIG_PATH += $$[QT_INSTALL_LIBS]/pkgconfig
  PKG_CONFIG = PKG_CONFIG_PATH=$$PKG_CONFIG_PATH pkg-config
}

QT        += widgets
CONFIG    += link_pkgconfig
PKGCONFIG += QatPlotWidgets
