#include "QatPresenter/PresenterApplication.h"
#include "QatDataAnalysis/IODriver.h"
#include "QatDataAnalysis/HistogramManager.h"
#include "QatSystemInterfaces/SystemInterface.h"
#include "QatSystemInterfaces/Serializer.h"
#include "QatSystemInterfaces/HistoLocator.h"
#include "ui_PresenterApplication.h"
#include "QatPlotWidgets/PlotHist1DDialog.h"

#include <QDir>
#include <QBuffer>
#include <QPluginLoader>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QMenu>
#include <QAction>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <libgen.h>


#include "QatPlotting/VisFunction.h"
#include <QDir>
#include <dlfcn.h>
#include <QStringList>
#include <QTreeWidget>

#include <unistd.h>

typedef void * (*CreationMethod) ();

inline std::string getDriver() {
  char *dn=getenv("QAT_IO_DRIVER");
  if (dn) {
    return std::string(dn);
  }
  else {
    return "HDF5Driver";
  }
}


// The I/O Loader
IOLoader loader;


class FunctionInsertion {

public:

  void insert (QTreeWidget *, 
	       const std::string & loadFileStem, 
	       std::map<QTreeWidgetItem *, VisFunction *> &,
	       VisFunctionLocator &);


private:

  void processDir(const std::string & dirName, 
		  QTreeWidgetItem *,
		  std::map<QTreeWidgetItem *,VisFunction *> &,
		  VisFunctionLocator &
		  ); 

};




class PresenterApplication::Clockwork {
public:
  Ui::PresenterApplication ui;
  const IODriver *driver;

  void handleManager(const HistogramManager *manager, QTreeWidgetItem *parent);
 
  void handleTable  (const Table            *table,   QTreeWidgetItem *parent);
  
  void purge(QTreeWidgetItem *, 
	     Hist1DSet & doomedH1Set,
	     Hist2DSet & doomedH2Set,
	     TableSet  & doomedTableSet);

  std::vector<DASystem *> system;
  Hist1DSet       selectedH1Set;
  Hist2DSet       selectedH2Set;
  TableSet        selectedTableSet;
  VisFunctionSet  selectedVisFunctionSet;
  std::map<QTreeWidgetItem *, const Hist1D *>     h1Map;
  std::map<QTreeWidgetItem *, const Hist2D *>     h2Map;
  std::map<QTreeWidgetItem *, const Table  *>     tbMap;

  std::map<QTreeWidgetItem *, VisFunction *>      visFunctionMap; 

  std::map<QTreeWidgetItem *, const HistogramManager *> managerMap;
  std::map<QWidget         *, QByteArray>         treeMap;
  std::map<QWidget         *, QByteArray>         sysStateMap;
  qint32 tab;
  std::string currentFilename;
  std::string currentDirectory;

  std::string attachFilename;
  std::string attachDirectory;

  HistoLocator1D histoLocator1;
  HistoLocator2D histoLocator2;
  VisFunctionLocator visFunctionLocator;

  std::map<std::string, CreationMethod> loadMap;

  std::vector<std::string> visFunctionDir;


};

void PresenterApplication::Clockwork::purge(QTreeWidgetItem *item,
					    Hist1DSet & doomedH1Set,
					    Hist2DSet & doomedH2Set,
					    TableSet  & doomedTableSet) {
  
  // h1Map cleanout
  {
    std::map<QTreeWidgetItem *, const Hist1D *>::iterator i=h1Map.find(item); 
    if (i!=h1Map.end()) {
      doomedH1Set.push_back((*i).second);
      h1Map.erase(i);
    }
  }


  // h2Map cleanout
  {
    std::map<QTreeWidgetItem *, const Hist2D *>::iterator i=h2Map.find(item); 
    if (i!=h2Map.end()) {
      doomedH2Set.push_back((*i).second);
      h2Map.erase(i);
    }
  }

  // tbMap cleanout
  {
    std::map<QTreeWidgetItem *, const Table *>::iterator i=tbMap.find(item); 
    if (i!=tbMap.end()) {
      doomedTableSet.push_back((*i).second);
      tbMap.erase(i);
    }
  }

  // managerMap cleanout
  {
    std::map<QTreeWidgetItem *, const HistogramManager *>::iterator i=managerMap.find(item); 
    if (i!=managerMap.end()) {
      managerMap.erase(i);
    }
  }

  for (int i=item->childCount()-1;i>=0; i--) {
    purge(item->child(i), doomedH1Set, doomedH2Set, doomedTableSet);
  }

  delete item;
}

void PresenterApplication::Clockwork::handleTable( const Table *table, QTreeWidgetItem *parent) {
  for (unsigned int a=0;a<table->numAttributes();a++) {
    const Attribute & attr = table->attribute(a);
    QStringList list;
    list << attr.name().c_str();
    list << attr.typeName().c_str();
    QTreeWidgetItem *item=new QTreeWidgetItem(list);
    item->setFlags(0); // not selectable or anything.. 
    parent->addChild(item);
  }
}


void PresenterApplication::Clockwork::handleManager (const HistogramManager *manager, QTreeWidgetItem *parent) {


  for (HistogramManager::DConstIterator d=manager->beginDir();d!=manager->endDir();d++) {
    QStringList list;
    list << (*d)->name().c_str();
    list << "Dir";
    QTreeWidgetItem *item=new QTreeWidgetItem(list);
    item->setFlags(Qt::ItemIsEnabled); // not selectable.
    parent->addChild(item);
    managerMap[item]=*d;
    handleManager(*d,item);
  }

  for (HistogramManager::H1ConstIterator h=manager->beginH1();h!=manager->endH1();h++) {
    QStringList list;
    list << (*h)->name().c_str();
    list << "H1";
    QTreeWidgetItem *item = new QTreeWidgetItem(list);
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);// | Qt::ItemIsDragEnabled);
    parent->addChild(item);
    h1Map[item]=*h;

    std::string name = (*h)->name();
    for (QTreeWidgetItem *m=parent;m!=NULL;m=m->parent()) {
      name = m->text(0).toStdString() + "/" + name;
    }
    histoLocator1.insert(*h,name);
  }

  for (HistogramManager::H2ConstIterator h=manager->beginH2();h!=manager->endH2();h++) {
    QStringList list;
    list << (*h)->name().c_str();
    list << "H2";
    QTreeWidgetItem *item = new QTreeWidgetItem(list);
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);// | Qt::ItemIsDragEnabled);
    parent->addChild(item);
    h2Map[item]=*h;


    std::string name = (*h)->name();
    for (QTreeWidgetItem *m=parent;m!=NULL;m=m->parent()) {
      name = m->text(0).toStdString() + "/" + name;
    }
    histoLocator2.insert(*h,name);

  }

  for (HistogramManager::TConstIterator h=manager->beginTable();h!=manager->endTable();h++) {
    QStringList list;
    list << (*h)->name().c_str();
    list << "Table";
    QTreeWidgetItem *item = new QTreeWidgetItem(list);
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);// | Qt::ItemIsDragEnabled);
    parent->addChild(item);
    handleTable(*h, item);
    tbMap[item]=*h;
  }

}



PresenterApplication::PresenterApplication (QWidget *parent)
  :QMainWindow(parent), c(new Clockwork())
{
  c->ui.setupUi(this);
  c->driver=loader.ioDriver(getDriver());
  if (!c->driver) {
    // Do not throw error at this point.
  }
  c->ui.treeWidget->setColumnWidth(0,200);
 

  connect (c->ui.actionQuit, SIGNAL(triggered()), this, SLOT(quit()));
  connect (c->ui.actionAttach, SIGNAL(triggered()), this, SLOT(selectOpenFile()));
  connect (c->ui.actionDetach, SIGNAL(triggered()), this, SLOT(selectCloseFile()));
  //  connect (c->ui.actionNew,  SIGNAL(triggered()), this, SLOT(selectNewFile()));
  connect (c->ui.actionLoad_Configuration, SIGNAL(triggered()), this, SLOT(selectLoadFile()));
  connect (c->ui.actionSave_Configuration, SIGNAL(triggered()), this, SLOT(selectSaveFile()));
  connect (c->ui.tabWidget,SIGNAL(contextMenu(QWidget *, const QPoint& )),
	   this,SLOT(raiseTabBarContextMenu(QWidget *, const QPoint &)));
  connect (c->ui.tabWidget, SIGNAL(currentChanged(int)), this, SLOT(currentChanged(int)));
  connect(c->ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(itemDoubleClicked(QTreeWidgetItem *, int)));
  // Look at the Plugin Directory and Populate the list of Plugins:
  
  QStringList libraryPath=QApplication::libraryPaths();
  for (int i=0;i<libraryPath.size();i++) {
    if (libraryPath.at(i).contains("qatplugins")) {
      QDir dir(libraryPath.at(i));
      QStringList entryList=dir.entryList();
      for (int e=0;e<entryList.size();e++) {
#ifndef __APPLE__
	if (entryList.at(e).contains(".so")) {
	  QString entry=entryList.at(e);
	  QString trunc0=entry.remove(0,6);   // Strip off "libQat"
	  QString suffix=".so";
	  QString system=trunc0.remove(suffix);
	  QString extraDotsIndicatingVersion=".";
	  if (system.indexOf(extraDotsIndicatingVersion)!=-1) continue;
	  QAction *action=c->ui.menuSystems->addAction(system, this, SLOT(loadSystem()));
	  action->setObjectName(system);
	}
#else 
	if (entryList.at(e).contains(".dylib")) {
	  QString entry=entryList.at(e);
	  QString trunc0=entry.remove(0,6);   // Strip off "libQat"
	  QString suffix=".dylib";
	  QString system=trunc0.remove(suffix);
	  QString extraDotsIndicatingVersion=".";
	  if (system.indexOf(extraDotsIndicatingVersion)!=-1) continue;
	  QAction *action=c->ui.menuSystems->addAction(system, this, SLOT(loadSystem()));
	  action->setObjectName(system);
	}
#endif
      }
    }
  }
  c->ui.tabWidget->removeTab(0);

  connect(c->ui.treeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(updateSelection()));
  connect(c->ui.treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem *,QTreeWidgetItem *)),   this, SLOT(updateSelection()));
  //  connect(c->ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(itemDoubleClicked(QTreeWidgetItem *, int)));
  c->tab=-1;


}

void PresenterApplication::readVisFunctions() {
  FunctionInsertion fs;
  for (unsigned int i=0;i<c->visFunctionDir.size();i++) {
    fs.insert(c->ui.treeWidget, c->visFunctionDir[i], c->visFunctionMap,c->visFunctionLocator);
  }
}

PresenterApplication::~PresenterApplication() {
  clear();
  std::map<QTreeWidgetItem *, VisFunction *>::iterator 
    begin=c->visFunctionMap.begin(),
    end=c->visFunctionMap.end(),
    current=begin;
  while(current!=end) {
    delete (*current).second;
    current++;
  }

 
  delete c;
}

void PresenterApplication::open(const std::string & pathname) {
  
  char fpath[1024];
  char *retChar=realpath(pathname.c_str(),fpath);
  if (!retChar) throw std::runtime_error ("Cannot find file " + pathname);

  const std::string fullpathname=fpath;
  const HistogramManager *manager = c->driver ? c->driver->openManager(fullpathname):NULL;
  if (!manager) {
    std::cerr << "Warning: PresenterApplication cannot open file " << pathname << std::endl; 
  }
  else {
    QStringList list;
    list << fullpathname.c_str();
    list << "File";

    QTreeWidgetItem *item = new QTreeWidgetItem(list);
    c->managerMap[item]=manager;
    c->ui.treeWidget->addTopLevelItem(item);
    c->handleManager(manager,item);
    c->ui.treeWidget->reset();
    c->attachFilename=fullpathname;
  }

}


void PresenterApplication::quit () {
  int button=QMessageBox::question(this, tr("Question"), tr("Really Quit?"), QMessageBox::No|QMessageBox::Yes, QMessageBox::Yes);
  if (button==int(QMessageBox::Yes))  qApp->quit();
}

void PresenterApplication::loadSystem () {
  QObject *senderObject = sender();
  if (senderObject) {
    
    // Get the name of the system from the sender:
    QString systemName = senderObject->objectName();

    loadSystem(systemName.toStdString());

  }
}

void PresenterApplication::loadSystem(const std::string & system) 
{

  QObject *senderObject = sender();

  //  Here is a message string for errors:
  std::ostringstream messageString;
    
  std::map<std::string, CreationMethod>::const_iterator lM =c->loadMap.find(system);
  CreationMethod cMethod=NULL;

  if (lM!=c->loadMap.end()) cMethod = (*lM).second;
  
  if (!cMethod) {
    QString systemName=system.c_str();
    // Get the library path from the core application:
    QStringList libraryPaths=QCoreApplication::instance()->libraryPaths();
    
    for (int i=0;i<libraryPaths.size();i++) {
      QString path=libraryPaths.at(i);
#ifndef __APPLE__
      QString libraryName = path + QString("/libQat") + systemName + QString(".so");
#else
      QString libraryName = path + QString("/libQat") + systemName + QString(".dylib");
#endif
      void *handle = dlopen(libraryName.toStdString().c_str(),RTLD_NOW);
      if (!handle) {
	std::cout << "Warning, dlopen fails on file: " << libraryName.toStdString() << std::endl;
	std::cout << dlerror() << std::endl;
	continue;
      }

      std::ostringstream createFunctionName;
      createFunctionName << "create_" << systemName.toStdString();
      union {CreationMethod func; void *data; } sym;
      sym.data=  dlsym(handle, createFunctionName.str().c_str());
      cMethod = sym.func;
      
      if (!cMethod) {
	std::cerr << "Error loading function " << createFunctionName.str() << std::endl;
	std::cerr << "From library " << libraryName.toStdString() << std::endl;
	continue;
      }
      
      if (cMethod) {
	c->loadMap[system]=cMethod;
	break;
      }
    }
  }
  if (cMethod) {
    {
      
      DASystem *system = (DASystem*) (*cMethod)();
      if (system) {
	messageString << "Success creating a " << system->name().toStdString() << std::endl;
	system->setHist1DSet(&c->selectedH1Set);
	system->setHist2DSet(&c->selectedH2Set);
	system->setTableSet (&c->selectedTableSet);
	system->setVisFunctionSet (&c->selectedVisFunctionSet);
	system->setPresenterTreeWidget(c->ui.treeWidget);
	system->setPresenterMessageTextEdit(c->ui.messageTextEdit);
	system->setPresenterH1Locator(&c->histoLocator1);
	system->setPresenterH2Locator(&c->histoLocator2);
	system->setPresenterVisFunctionLocator(&c->visFunctionLocator);
	c->system.push_back(system);
	
	system->toolWidget()->setParent(c->ui.toolBoxStackedWidget);
	c->ui.toolBoxStackedWidget->addWidget(system->toolWidget());
	system->toolWidget()->show();
	//Exp:
	c->ui.toolBoxStackedWidget->setEnabled(true);
	c->ui.toolBoxStackedWidget->show();
	c->ui.toolBoxStackedWidget->update();
	c->ui.toolBoxStackedWidget->setCurrentWidget(system->toolWidget());
	c->ui.tabWidget->blockSignals(true);
	c->ui.tabWidget->setCurrentIndex(c->ui.tabWidget->addTab(system->mainWidget(),system->name()));
	c->ui.tabWidget->blockSignals(false);

	c->tab=c->ui.tabWidget->currentIndex();
	//
	// This routine is called from (A) user request, or (B) config file restore.
	// In case (B) there is no sender, and no need to update either, since update
	// will follow...
	//
	if (senderObject) c->system.back()->update();
      }
      else {
	std::cerr << "Could not create system" << std::endl;
      }
    }
  }
  else {
    messageString << "Failure loading library" << std::endl;
  }
  c->ui.messageTextEdit->insertPlainText (messageString.str().c_str());
}


void PresenterApplication::selectOpenFile () {
  char dirbuff[1024];
  char *retChar=getcwd(dirbuff,1024);
  if (!retChar) throw std::runtime_error ("Cannot get working directory!");

  QFileDialog dialog(0);
  QStringList filters;
  filters.append("Root files (*.root)");
  filters.append("All files (*)");
  dialog.setNameFilters(filters);
  dialog.setDirectory(c->attachDirectory=="" ? dirbuff : c->attachDirectory.c_str());
  dialog.selectFile(c->attachFilename.c_str());
  dialog.setFileMode(QFileDialog::ExistingFile);
  if (dialog.exec()) {
    
    QStringList openFilenames=dialog.selectedFiles();
    if (openFilenames.count()!=1) return;
    QString openFileName = openFilenames[0];
    c->attachDirectory=dirname((char *) openFileName.toStdString().c_str());
    
    if (!openFileName.isNull()) {
      open(openFileName.toStdString());
    }
  }
}

void PresenterApplication::selectCloseFile () {
  // Get the list of selected top level items in the tree view.
  // Take those items out of the list..

  QList<QTreeWidgetItem *> selectedItems =c->ui.treeWidget->selectedItems();
  for (int i=0;i<selectedItems.count();i++) {
    if (!selectedItems[i]->parent()) {
      if (selectedItems[i]->text(1).toStdString()=="File") {

	std::ostringstream message;
	message << "Drop file " << selectedItems[i]->text(0).toStdString() << std::endl;
	c->ui.messageTextEdit->insertPlainText (message.str().c_str());

	// Block signals for a while:
	c->ui.treeWidget->blockSignals(true);
	int index = c->ui.treeWidget->indexOfTopLevelItem(selectedItems[i]);
	QTreeWidgetItem *removedItem=c->ui.treeWidget->takeTopLevelItem(index);

	// Purge:
	Hist1DSet doomedH1Set;
	Hist2DSet doomedH2Set;
	TableSet  doomedTableSet;
	c->purge(removedItem, doomedH1Set, doomedH2Set, doomedTableSet);

	// Close input file:
	const HistogramManager *manager=c->managerMap[removedItem];
	if (c->driver) c->driver->close(manager);


	// Turn signals back on.
							   
	c->ui.treeWidget->blockSignals(false);
	c->ui.treeWidget->update();
	updateSelection();
      }
    } 
  }
}


void PresenterApplication::selectLoadFile () {
  
  
  char dirbuff[1024];
  char *retChar=getcwd(dirbuff,1024);
  if (!retChar) throw std::runtime_error ("Cannot get working directory!");

  QFileDialog dialog(0);
  QStringList filters;
  filters.append("Presenter files (*.pres)");
  filters.append("All files (*)");
  dialog.setNameFilters(filters);
  dialog.setDirectory(c->currentDirectory=="" ? dirbuff : c->currentDirectory.c_str());
  dialog.selectFile(c->currentFilename.c_str());
  dialog.setFileMode(QFileDialog::ExistingFile);
  if (dialog.exec()) {
    
    QStringList loadFilenames=dialog.selectedFiles();
    if (loadFilenames.count()!=1) return;
    QString loadFileName = loadFilenames[0];
    c->currentDirectory=dirname((char *) loadFileName.toStdString().c_str());
    
    if (!loadFileName.isNull()) {
      clear();
      load(loadFileName.toStdString());
    }
  }
  
}

void PresenterApplication::selectSaveFile () {

  char dirbuff[1024];
  char * retChar=getcwd(dirbuff,1024);
  if (!retChar) throw std::runtime_error ("Cannot get working directory!");

  QFileDialog dialog(0);
  QStringList filters;
  filters.append("Presenter files (*.pres)");
  filters.append("All files (*)");
  dialog.setNameFilters(filters);
  dialog.setDirectory(c->currentDirectory=="" ? dirbuff : c->currentDirectory.c_str());
  dialog.selectFile(c->currentFilename.c_str());
  dialog.setAcceptMode(QFileDialog::AcceptSave);
  dialog.setConfirmOverwrite(true);
  if (dialog.exec()) {
    
    QStringList saveFilenames=dialog.selectedFiles();
    if (saveFilenames.count()!=1) return;
    QString saveFileName = saveFilenames[0];
    c->currentDirectory=dirname((char *) saveFileName.toStdString().c_str());
    
    if (!saveFileName.isNull()) {
      save(saveFileName.toStdString());
    }
  }
}




void PresenterApplication::updateSelection() {

  QList<QTreeWidgetItem *> selectedItems=c->ui.treeWidget->selectedItems();
  QTreeWidgetItem         *currentItem  =c->ui.treeWidget->currentItem();

  c->selectedH1Set.erase(c->selectedH1Set.begin(),c->selectedH1Set.end());
  c->selectedH2Set.erase(c->selectedH2Set.begin(),c->selectedH2Set.end());
  c->selectedTableSet.erase(c->selectedTableSet.begin(),c->selectedTableSet.end());
  c->selectedVisFunctionSet.erase(c->selectedVisFunctionSet.begin(),c->selectedVisFunctionSet.end());

  // Selected Items:
  for (int i=0;i<selectedItems.size();i++) {
    QTreeWidgetItem *item=selectedItems.at(i);
    // Locate all histograms:
    std::map<QTreeWidgetItem *, const Hist1D *>::const_iterator h1=c->h1Map.find(item);
    if (h1!=c->h1Map.end()) {
      const Hist1D *hist1D=(*h1).second;
      c->selectedH1Set.push_back(hist1D);
    }
    // Locate all scatterplots:
    std::map<QTreeWidgetItem *, const Hist2D *>::const_iterator h2=c->h2Map.find(item);
    if (h2!=c->h2Map.end()) {
      const Hist2D *hist2D=(*h2).second;
      c->selectedH2Set.push_back(hist2D);
    }
    // Locate all tables:
    std::map<QTreeWidgetItem *, const Table *>::const_iterator tb=c->tbMap.find(item);
    if (tb!=c->tbMap.end()) {
      const Table *table=(*tb).second;
      c->selectedTableSet.push_back(table);
    }
    // Locate all vis functions
    std::map<QTreeWidgetItem *, VisFunction *>::const_iterator vi=c->visFunctionMap.find(item);
    if (vi!=c->visFunctionMap.end()) {
      const VisFunction *visFunction=(*vi).second;
      c->selectedVisFunctionSet.push_back(visFunction);
    }
  }
  if (selectedItems.size()==0) {
    // Add the current item:
    if (!selectedItems.contains(currentItem))
      {
	// Locate all histograms:
	std::map<QTreeWidgetItem *, const Hist1D *>::const_iterator h1=c->h1Map.find(currentItem);
	if (h1!=c->h1Map.end()) {
	  const Hist1D *hist1D=(*h1).second;
	  c->selectedH1Set.push_back(hist1D);
	}
	// Locate all scatterplots:
	std::map<QTreeWidgetItem *, const Hist2D *>::const_iterator h2=c->h2Map.find(currentItem);
	if (h2!=c->h2Map.end()) {
	  const Hist2D *hist2D=(*h2).second;
	  c->selectedH2Set.push_back(hist2D);
	}
	// Locate all tables:
	std::map<QTreeWidgetItem *, const Table *>::const_iterator tb=c->tbMap.find(currentItem);
	if (tb!=c->tbMap.end()) {
	  const Table *table=(*tb).second;
	  c->selectedTableSet.push_back(table);
	}
	// Locate all vis functions
	std::map<QTreeWidgetItem *, VisFunction *>::const_iterator vi=c->visFunctionMap.find(currentItem);
	if (vi!=c->visFunctionMap.end()) {
	  const VisFunction *visFunction=(*vi).second;
	  c->selectedVisFunctionSet.push_back(visFunction);
	}
      }
  }
  
  // Issue a message:
  
  
  c->ui.messageTextEdit->insertPlainText ("Updating systems\n");
  QWidget *w = c->tab== -1 ? 0 : c->ui.tabWidget->widget(c->tab);
  for (unsigned int i=0;i<c->system.size();i++) {
    if (w && c->system[i]->mainWidget()==w) c->system[i]->update();
  }


}
 
void PresenterApplication::raiseTabBarContextMenu (QWidget *w, const QPoint &p) {
  QMenu menu(w);
  int index = c->ui.tabWidget->indexOf(w);

  QAction *cloneAction=menu.addAction("Clone");
  QAction *deleteAction=menu.addAction("Delete");
  QAction *renameAction=menu.addAction("Rename");
  
  QAction *selectedAction = menu.exec(p);
  if (!selectedAction) {
    return;
  }
  if (selectedAction==cloneAction) {
    for (unsigned int i=0;i<c->system.size();i++) {
      if (c->system[i]->mainWidget()==w) {
	std::ostringstream messageString;
	QByteArray byteArray=c->system[i]->saveState();
	DASystem *system = c->system[i]->clone();
	if (system) {
	  messageString << "Success cloning a " << system->name().toStdString() << std::endl;
	  system->setHist1DSet(&c->selectedH1Set);
	  system->setHist2DSet(&c->selectedH2Set);
	  system->setTableSet (&c->selectedTableSet);
	  system->setVisFunctionSet(&c->selectedVisFunctionSet);
	  system->setPresenterTreeWidget(c->ui.treeWidget);
	  system->setPresenterMessageTextEdit(c->ui.messageTextEdit);
	  system->setPresenterH1Locator(&c->histoLocator1);
	  system->setPresenterH2Locator(&c->histoLocator2);
	  system->setPresenterVisFunctionLocator(&c->visFunctionLocator);
	  c->system.push_back(system);

	  system->toolWidget()->setParent(c->ui.toolBoxStackedWidget);
	  c->ui.toolBoxStackedWidget->addWidget(system->toolWidget());
	  system->toolWidget()->show();
	//Exp:
	  c->ui.toolBoxStackedWidget->setEnabled(true);
	  c->ui.toolBoxStackedWidget->show();
	  c->ui.toolBoxStackedWidget->update();
	  c->ui.toolBoxStackedWidget->setCurrentWidget(system->toolWidget());

	  c->ui.tabWidget->setCurrentIndex(c->ui.tabWidget->addTab(system->mainWidget(),system->name()));

	  system->restoreFromState(byteArray);
	  system->update();
	}
	else {
	  messageString << "Failure cloning system" << std::endl;
	}
	c->ui.messageTextEdit->insertPlainText (messageString.str().c_str());
	break;
      }
    }
  }
  else if (selectedAction==deleteAction) {
    for (unsigned int i=0;i<c->system.size();i++) {
      if (c->system[i]->mainWidget()==w) {
	c->tab=-1;
	c->ui.tabWidget->removeTab(index);
	c->ui.toolBoxStackedWidget->removeWidget(c->system[i]->toolWidget());
	delete c->system[i];
	c->system.erase(c->system.begin()+i);
      }
    }
  }
  else if (selectedAction==renameAction) {
    bool ok;
    QString text = QInputDialog::getText( c->ui.tabWidget, "Rename tab","Rename tab '"+c->ui.tabWidget->tabText(index)+"' to:",
					  QLineEdit::Normal, c->ui.tabWidget->tabText(index), &ok );
    if (ok) c->ui.tabWidget->setTabText(index,text);

  }

}

void PresenterApplication::currentChanged(int newTab) {
  if (c->ui.tabWidget->count()==0) return;
  c->ui.tabWidget->setFocus();
  QObject::blockSignals(true);
  // Do stuff;
  
  if (c->tab!=-1) {
    // Save the state of the tab widget, in four easy steps. 

    // 1. Get the main widget.
    QWidget *w = c->ui.tabWidget->widget(c->tab);
    
    // 2. Get the system.
    DASystem *system=NULL;
    for (unsigned int i=0;i<c->system.size();i++) {
      if (w==c->system[i]->mainWidget()) {
	system=c->system[i];
	break;
      }
    }
    if (!system) {
      std::ostringstream stream;
      stream << "Cannot locate the system for tab widget " << c->tab ;
      throw std::runtime_error (stream.str());
    }

    // 3. Get the state of the tab widget.
    Serializer serializer(1);
    serializer.save(c->ui.tabWidget->tabText(c->tab).toStdString());
    serializer.save(c->ui.treeWidget);
    QByteArray byteArray=serializer.result();
   
    
    // 4. Connect the tab widget state to the system.
    c->treeMap[w]=byteArray;

    // 5. Save the state of the system, too:
    if (1) {
      if (system) {
	QByteArray systemByteArray=system->saveCache();
	c->sysStateMap[w]=systemByteArray;
      }
    }

  }
  
  // As for the new tab:  go through the presenter tree map and determine
  // see if the state is avaiable.  If so, set the state.  
  {
    // 1. Get the main widget.
    QWidget *w = c->ui.tabWidget->widget(newTab);

    DASystem *system=NULL;
    for (unsigned int i=0;i<c->system.size();i++) {
      if (w==c->system[i]->mainWidget()) {
	system=c->system[i];
	break;
      }
    }
 
    if (system) c->ui.toolBoxStackedWidget->setCurrentWidget(system->toolWidget());

    

    // 2. See if its state is in the map, if so restore.
    {
      std::map<QWidget         *, QByteArray>::iterator t=c->treeMap.find(w), end=c->treeMap.end();
      if (t!=end) {
	Deserializer state((*t).second);
	c->ui.treeWidget->blockSignals(true);
	if (state.version() >=1) {
	  std::string tName;
	  state.restore(tName);
	  QString tn(tName.c_str());
	  c->ui.tabWidget->setTabText(newTab,tn);
	}
	state.restore(c->ui.treeWidget);
	c->ui.treeWidget->blockSignals(false);
	if (state.version()>1) throw std::runtime_error ("Error restoring the state of the tree widget");
	
      }
    }
    
    // And restore the system as well:
    if (1) 
    {
      std::map<QWidget         *, QByteArray>::iterator t=c->sysStateMap.find(w), end=c->sysStateMap.end();
      if (t!=end) {
	if (system) {
	  system->restoreFromCache((*t).second);
	}
      }
    }
      

  }

  // End do stuff.  Old tab is now this tab.
  c->tab=newTab;
  QObject::blockSignals(false);
  updateSelection();
    
 
}

// Change tables
void PresenterApplication::refreshTables(HistogramManager *, const Table *oldTable, const Table *newTable) {

  std::map<QTreeWidgetItem *, const Table *>::const_iterator 
    begin=c->tbMap.begin(),
    end=  c->tbMap.end(),
    tb=begin;

  while (tb!=end) {
    const Table *table=(*tb).second;
    if (table==oldTable) {
      QTreeWidgetItem *item = (*tb).first;
      
      c->tbMap[item]=newTable;

      break;
    }
    tb++;
  } 
  
  updateSelection();

}


void PresenterApplication::itemDoubleClicked(QTreeWidgetItem *item, int /*column*/) {
  
  for (unsigned int i=0;i<c->system.size();i++) {
    if (c->system[i]->mainWidget()==c->ui.tabWidget->currentWidget()) {
      
      std::map<QTreeWidgetItem *, const Hist1D *>::const_iterator h1=c->h1Map.find(item);
      if (h1!=c->h1Map.end()) {
	const Hist1D *hist1D=(*h1).second;
	c->system[i]->dblClicked1D(hist1D);
      }
      std::map<QTreeWidgetItem *, const Hist2D *>::const_iterator h2=c->h2Map.find(item);
      if (h2!=c->h2Map.end()) {
	const Hist2D *hist2D=(*h2).second;
	c->system[i]->dblClicked2D(hist2D);
      }

      std::map<QTreeWidgetItem *, const HistogramManager *>::const_iterator dr=c->managerMap.find(item);
      if (dr!=c->managerMap.end()) {
	const HistogramManager *dir=(*dr).second;
	c->system[i]->dblClickedMan(dir);
      }
      std::map<QTreeWidgetItem *,  VisFunction *>::const_iterator v1=c->visFunctionMap.find(item);
      if (v1!=c->visFunctionMap.end()) {
	const VisFunction *vf=(*v1).second;
	c->system[i]->dblClickedVisFunction(vf);
      }

      if (item->flags() & Qt::ItemIsSelectable) item->setSelected(true);
    }
  }

}


void PresenterApplication::load( const std::string & pathname) {
  
  QString filename=pathname.c_str();

   if (filename.isEmpty()) {
     QMessageBox::critical(0, "Error - Empty file name provided",
                           "Empty file name provided.",QMessageBox::Ok,QMessageBox::Ok);
     return;
   }
   QFileInfo fi(filename);
   if (!fi.exists()) {
     QMessageBox::critical(0, "Error - file does not exists: "+filename,
                           "File does not exists: <i>"+filename+"</i>",QMessageBox::Ok,QMessageBox::Ok);
     return;
   }
   if (!fi.isReadable()) {
     QMessageBox::critical(0, "Error - file is not readable: "+filename,
                           "File is not readable: <i>"+filename+"</i>",QMessageBox::Ok,QMessageBox::Ok);
     return;
   }
   //open file
   QFile file(filename);
   if (!file.open(QIODevice::ReadOnly)) {
     QMessageBox::critical(0, "Error - problems opening file "+filename,
			   "Problems opening file: <i>"+filename+"</i>",QMessageBox::Ok,QMessageBox::Ok);
     return;
   }
  
   // move to selectload   clear();

   //Read:
   QString head,foot;
   QStringList inFileNames, systemNames;
   QList<QByteArray> systemStates;
   QList<QByteArray> systemTreeStates;
   qint32 tab;

   QByteArray byteArray64;
   QDataStream infile(&file);
   infile >> byteArray64;
 
   QByteArray byteArray = qUncompress(QByteArray::fromBase64(byteArray64));
 
   QBuffer buffer(&byteArray);
   buffer.open(QIODevice::ReadOnly);
   QDataStream in(&buffer);
   in >> head;
   in >> inFileNames;
   in >> systemNames;
   in >> systemStates;
   in >> systemTreeStates;
   in >> tab;
   in >> foot;
   buffer.close();

   // Check basic stuff.
   if (head!="This is an automatically generated config file for present. [cfg version 000]"
         ||foot!="This is the end of the automatically generated config file for present.") {
     QMessageBox::critical(0, "Error - file not in correct format: "+filename,
                           "File not in correct format: <i>"+filename+"</i>",QMessageBox::Ok,QMessageBox::Ok);
     return;
   }

   // Open all of the files:
   for (int i=0;i<inFileNames.count();i++) {
     try {
       open(inFileNames[i].toStdString());
     }
     catch (std::exception & e) {
     }
   }
   std::cout << "Loading systems...." << std::endl;
   // Start all of the systems:
   for (int i=0;i<systemNames.count();i++) {
     loadSystem(systemNames[i].toStdString());
   }
   // Restore all of the system trees to their states:
   for (int i=0;i<systemNames.count();i++) {
     c->treeMap[c->system[i]->mainWidget()]=systemTreeStates[i];
     Deserializer state(systemTreeStates[i]);
     c->ui.treeWidget->blockSignals(true);
     if (state.version() >=1) {
       std::string tName;
       state.restore(tName);
       QString tn(tName.c_str());
       c->ui.tabWidget->setTabText(i,tn);
     }

     state.restore(c->ui.treeWidget);
     c->ui.treeWidget->blockSignals(false);

     if (state.version()>1) throw std::runtime_error ("Error restoring the state of the tree widget");

   }
   std::cout << "Restoring systems..." << std::endl;
   // Restore all of the systems to their states:

   //--------------------------
   // Normally done when the tab changes. But if there is only one system, 
   // this is forced.
   if (systemNames.count()==1) updateSelection();
   //--------------------------

   for (int i=0;i<systemNames.count();i++) {
     c->ui.tabWidget->setCurrentIndex(i);
     c->system[i]->restoreFromState(systemStates[i]);
     //c->system[i]->update(); // normally done when the tab widget selection occurs..
   }
   c->ui.tabWidget->setCurrentIndex(tab);
   c->currentFilename=pathname;
}


void PresenterApplication::save( const std::string & pathname) {


  // Input Files:
  QStringList inFileNames;
  for (int i=0;i<c->ui.treeWidget->topLevelItemCount();i++) {
    QString file=c->ui.treeWidget->topLevelItem(i)->text(0);
    inFileNames.append( file ) ;
  }
  
  // Systems:
  QStringList systemNames;
  for (unsigned int i=0;i<c->system.size();i++) {
    systemNames.append(c->system[i]->name());
  }
  
  // Tree widget state:
  QList<QByteArray> systemTreeStates;
  for (unsigned int i=0;i<c->system.size();i++) {
    
    QWidget *W = c->system[i]->mainWidget();
    QWidget *w = c->tab==-1 ? NULL: c->ui.tabWidget->widget(c->tab);
    if (w==W) { // Means record now!  From presenting widget!
      Serializer serializer(1);
      serializer.save(c->ui.tabWidget->tabText(c->tab).toStdString());
      serializer.save(c->ui.treeWidget);
      QByteArray byteArray=serializer.result();      
      systemTreeStates.append(byteArray);
    }
    else {
      systemTreeStates.append(c->treeMap[W]);
    }
  }

  // System state:
  QList<QByteArray> systemStates;
  for (unsigned int i=0;i<c->system.size();i++) {
    systemStates.append(c->system[i]->saveState());
  }


  QByteArray byteArray;
  QBuffer buffer(&byteArray);
  buffer.open(QIODevice::WriteOnly);
  QDataStream out(&buffer);
  out<<QString("This is an automatically generated config file for present. [cfg version 000]" );
  out<< inFileNames;
  out<< systemNames;
  out<< systemStates;
  out<< systemTreeStates;
  out<< c->tab;
  out<<QString("This is the end of the automatically generated config file for present." );
  buffer.close();

  QFile file(pathname.c_str());
  if (!file.open(QIODevice::WriteOnly)) {
    c->ui.messageTextEdit->insertPlainText ("Cannot save configuration");  
    return;
  }
  QDataStream outfile(&file);
  outfile<<qCompress(byteArray).toBase64();

  c->currentFilename=pathname;
}

void PresenterApplication::clear() {

  c->ui.tabWidget->blockSignals(true);
  c->ui.treeWidget->clear();
  for (int i=c->ui.tabWidget->count()-1;i>=0;i--) c->ui.tabWidget->removeTab(i);
  c->ui.tabWidget->blockSignals(false);

  c->tab=-1;

  for (unsigned int i=0;i<c->system.size();i++) {
    delete c->system[i];
  }
  c->system.erase(c->system.begin(),c->system.end());
  c->selectedH1Set.erase(c->selectedH1Set.begin(),c->selectedH1Set.end());
  c->selectedH2Set.erase(c->selectedH2Set.begin(),c->selectedH2Set.end());
  c->selectedTableSet.erase(c->selectedTableSet.begin(),c->selectedTableSet.end());
  c->selectedVisFunctionSet.erase(c->selectedVisFunctionSet.begin(), c->selectedVisFunctionSet.end());
  c->h1Map.erase(c->h1Map.begin(),c->h1Map.end());
  c->h2Map.erase(c->h2Map.begin(),c->h2Map.end());
  c->tbMap.erase(c->tbMap.begin(),c->tbMap.end());
  c->treeMap.erase(c->treeMap.begin(),c->treeMap.end());


  {
    std::map<QTreeWidgetItem *, const HistogramManager *>::const_iterator 
      begin=c->managerMap.begin(),
      end=c->managerMap.end(),
      current=begin;
    while (current!=end) {
      const HistogramManager *manager=(*current).second;
      if (c->driver) c->driver->close(manager);
      current++;
    }
  }

		   
}

// Add a directory for vis functions
void PresenterApplication::addVisFunctionDirectory(const std::string & visFunctionDir) {
  c->visFunctionDir.push_back(visFunctionDir);
}






void FunctionInsertion::processDir(const std::string & dirName, 
				   QTreeWidgetItem *parentItem,
				   std::map<QTreeWidgetItem *, VisFunction *> &visFunction,
				   VisFunctionLocator &vfl) {

  QDir dir(dirName.c_str());
  if (dir.isReadable()) {
    QStringList subdirList = dir.entryList(QDir::Dirs);
    
    for (QStringList::Iterator subdir = ++subdirList.begin(); subdir !=  subdirList.end(); ++subdir) {
      if (!(*subdir).startsWith(".") && !(*subdir).contains("ti_files")) {
	QStringList shortList;
	shortList.push_back(*subdir);
        QTreeWidgetItem *item = new QTreeWidgetItem(parentItem, shortList,1);
	item->setFlags(Qt::ItemIsEnabled);
        processDir (std::string(dirName)+std::string("/") + (*subdir).toStdString(), item, visFunction,vfl);
      }
    }


    QDir d(dirName.c_str());
    d.setFilter( QDir::Files | QDir::Hidden | QDir::NoSymLinks );
    
    const QFileInfoList list = d.entryInfoList();

    
    QList<QFileInfo>::const_iterator it=list.begin();     // create list iterator
    
    while (it!=list.end()) {           // for each file...
      std::string selection  = (*it).fileName().toStdString();
      std::string basename   = (*it).baseName().toStdString();
      std::string extension  = (*it).suffix().toStdString();
      std::string path       = (*it).absolutePath().toStdString();
      std::string dir        = (*it).dir().dirName().toStdString();
      path += "/";
      path += basename;
      path += ".";
      path += extension;
      if (extension=="so") {
        void *handle = dlopen(path.c_str(),RTLD_NOW);
        if (!handle) {
          std::cout << "Warning, dlopen fails on file: " << path << std::endl;
          std::cout << dlerror() << std::endl;
          ++it; continue;
        }
        std::ostringstream createFunctionName;
        //createFunctionName << "_ZN" << basename.size() << basename << "6createEv";
        //createFunctionName << "create__" << basename.size() << basename << "SFv";
	createFunctionName << "create_"<< basename;
        CreationMethod createMethod;
	union { CreationMethod func; void* data; } sym;
	sym.data=  dlsym(handle, createFunctionName.str().c_str());
	createMethod = sym.func;
        if (!createMethod) {
          // We are hosed....
          std::cout << "Warning, cannot load function : " << basename << std::endl;
          std::cout << "That we try to load from file "   << path     << std::endl;
          std::cout << dlerror() << std::endl;
        }
        else {
          // Create this object on the spot.  .
          VisFunction *ff =  (VisFunction *) (*createMethod)();
          if (ff) {
	    std::string name=ff->name();
	    vfl.insert(ff,name);
	    QStringList list;
	    list << ff->name().c_str() ;
	    list << "Function";
	    QTreeWidgetItem *item = new QTreeWidgetItem(list);
	    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
	    parentItem->addChild(item);
	    visFunction[item]=ff;	
	  }
	  else {
	  }
        }
      }
      ++it; 
    }

  }

}


void FunctionInsertion::insert(QTreeWidget *fmw, const std::string & loadFileStem,  
			       std::map<QTreeWidgetItem *, VisFunction *> &visFunction,
			       VisFunctionLocator &vfl) {
  //
  //  In addition to the Histograms, also present the built in functions:
  //
  
  std::string loadFileName = loadFileStem+std::string("/VisFunctions");
  if (!QDir(loadFileName.c_str()).isReadable()) loadFileName="";


  if (loadFileName=="") {
    //   std::cerr << "Warning, cannot locate directory containing Visualization Functions." << std::endl;
   // We are hosed.
    // We are hosed. 
  }
  else {

    QStringList list;
    list << loadFileName.c_str();
  
    QTreeWidgetItem *library = new QTreeWidgetItem(list);
    library->setFlags(Qt::ItemIsEnabled);
    fmw->addTopLevelItem(library);

    //-------------------------------------------------//    
    // Into recursion land...--------------------------//
    processDir(loadFileName,library, visFunction,vfl); //
    //-------------------------------------------------//    
  }
  
}

 
