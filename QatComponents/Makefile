prefix = /usr/local
targetdir = ../tmp

TMCPPFILES           =  $(shell find . -name "*.cpp")
MYCPPFILES           =  $(patsubst ./%,   %,   $(TMCPPFILES))
MYOFILES             =  $(patsubst %.cpp, %.o, $(MYCPPFILES))
DSOFILES             =  $(patsubst %.cpp, $(targetdir)/VisFunctions/%.so, $(MYCPPFILES))

override PKG_CONFIG_PATH= $(shell qtpaths --install-prefix)/lib/pkgconfig
override CXXFLAGS  +=  -I../.. `PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config QatPlotting --cflags`  -fPIC -std=c++17
override LDFLAGS   +=  `PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config QatPlotting --libs`  -fPIC 

%.cpp.%.o:
	$(CXX) -c  $< -o $@ $(CXXFLAGS) $(CPPFLAGS)

$(targetdir)/VisFunctions/%.so : %.o
	mkdir -p $(targetdir)/VisFunctions
	mkdir -p $(targetdir)/VisFunctions/`dirname $<`
	cp $< $(targetdir)/VisFunctions/`dirname $<`
	g++ -shared  $< $(LDFLAGS) -o $@
	rm -f $(targetdir)/VisFunctions/$<


all: $(DSOFILES)


install: all
	mkdir -p ../tmp
	mkdir -p $(targetdir)
	mkdir -p $(prefix)/share
	cp -r $(targetdir)/* $(prefix)/share
	mkdir -p $(prefix)/share/src
	mkdir -p $(prefix)/share/src/VisFunctions
	cp -r . $(prefix)/share/src/VisFunctions

uninstall:
	rm -r -f $(prefix)/share/VisFunctions
	rm -r -f $(prefix)/share/src/VisFunctions

clean:
	rm -r -f $(targetdir)/VisFunctions
	rm -r -f $(MYOFILES)
distclean:


bin:$(DSOFILES) 


