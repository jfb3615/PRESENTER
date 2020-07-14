#ifndef _PRESENTERAPPLICATION_H_
#define _PRESENTERAPPLICATION_H_
#include "QatDataAnalysis/IOLoader.h"
#include <QMainWindow>
#include <string>
class HistogramManager;
class Table;
class QTreeWidgetItem;
class PresenterApplication: public QMainWindow {

  // This is a Q_OBJECT:
  Q_OBJECT

 public:

  // Constructor
  PresenterApplication(QWidget * parent=NULL);

  // Destructor
  ~PresenterApplication();

  // Open a file:
  void open(const std::string & pathname);

  // Save a configuration file:
  void save (const std::string & pathname);

  // Load a configuration file:
  void load (const std::string & pathname);

  // Load a system:
  void loadSystem (const std::string & systemName);

  // Add a directory for vis functions
  void addVisFunctionDirectory(const std::string & visFunctionDirectoryName);
  
  // Read in vis functions
  void readVisFunctions();

 private:
  
  // Copy and assign are illegal
  PresenterApplication & operator=(const PresenterApplication & );
  PresenterApplication (const PresenterApplication &);

  // Internals
  class Clockwork;
  Clockwork *c;

  public slots:

  // Quit the application:
  void quit();

  //  Load a system
  void loadSystem();

  //  Select a File for Open:
  void selectOpenFile();

  //  Close selected file:
  void selectCloseFile();

  //  Select a File for Load Configuration:
  void selectLoadFile();

  //  Select a File for Save Configuration:
  void selectSaveFile();

  //  Update the selection:
  void updateSelection();
 
  //  Respond to clicks on the tab b ar
  void raiseTabBarContextMenu (QWidget *, const QPoint & );

  //  Current selection changed
  void currentChanged(int i);

  // Change tables
  void refreshTables(HistogramManager *manager, const Table *oldTable, const Table *newTable);

  // Double click on an item in the tree:
  void itemDoubleClicked(QTreeWidgetItem *item, int column);

  // Unload files, systems, and input browser.
  void clear();

};
#endif
