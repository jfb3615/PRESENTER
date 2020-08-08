#include "QatPresenter/PresenterApplication.h"
#include <QApplication>
#include <QLocale>
#include <string>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <cstdlib>
int main(int argc, char ** argv) {
  //
  // Other locales may use "," rather than "." (decimal point). This
  // unfortunately prevents the interpretation of data on dials and
  // other widgets. 
  //
  QLocale::setDefault(QLocale(QLocale::English, QLocale::AnyCountry));
  QApplication app(argc,argv);
  app.addLibraryPath("/usr/local/share/qatplugins");
  PresenterApplication presenter;

  // Get Vis Dirs:
  char *visDir=getenv("PRESENTER_VIS_DIR");
  if (visDir) {
    char *thisDir=strtok(visDir,(char *)":");
    while (thisDir) {
      presenter.addVisFunctionDirectory(thisDir);
      thisDir=strtok(NULL, (char *) ":");
    }
  }
  else {
    presenter.addVisFunctionDirectory("/usr/local/share/");
  }

  presenter.readVisFunctions();

  if (argc==1) {
  }
  else if ((argc==2) && std::string(argv[1]).find(".pres")!=std::string::npos) {
    presenter.load(argv[1]);
  }
  else {
    for (int c=1;c<argc;c++) {
      presenter.open(argv[c]);
    }
  }
  presenter.show();
  return app.exec();
}
