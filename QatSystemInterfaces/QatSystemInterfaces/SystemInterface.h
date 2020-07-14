#ifndef SYSTEMINTERFACE_H_
#define SYSTEMINTERFACE_H_
#include <QByteArray>
#include <QString>
#include <QObject>
class Hist1D;
class Hist2D;
class VisFunction;
class Table;
class QTreeWidget;
class QTreeWidgetItem;
class QTextEdit;
class HistogramManager;
class QWidget;
#include <vector>
#include <map>

typedef std::vector<const Hist1D *>      Hist1DSet;
typedef std::vector<const Hist2D *>      Hist2DSet;
typedef std::vector<const Table  *>      TableSet;
typedef std::vector<const VisFunction *> VisFunctionSet;
typedef std::map<QTreeWidgetItem *, const HistogramManager *> ConstHistManMap;
typedef std::map<QTreeWidgetItem *, HistogramManager *>       HistManMap;

#include "QatSystemInterfaces/HistoLocator.h"

class DASystem : public QObject{
  
  Q_OBJECT


 public:
  
  // Constructor:
  DASystem();
  
  // Destructor: 
  virtual ~DASystem();

  // Get the name of the system. 
  virtual QString name() { return QString("Base System");}

  // Get the main widget:
  virtual QWidget * mainWidget() const=0;

  // Tell the system to update:
  virtual void update() = 0;

  // Attribute some 1D histograms to this system:
  void setHist1DSet (Hist1DSet *);

  // Attribute some 2D histograms to this system:
  void setHist2DSet (Hist2DSet *);

  // Set the Vis function set:
  void setVisFunctionSet (VisFunctionSet *);

  // Attribute some 2D histograms to this system:
  void setTableSet (TableSet *);

  // Get the 1D Histograms:
  const Hist1DSet * hist1DSet() const;

  // Get the 2D Histograms:
  const Hist2DSet * hist2DSet() const;

  // Get the 1D Histograms:
  const VisFunctionSet * visFunctionSet() const;

  // Get the Tables:
  const TableSet * tableSet() const;

  // Save the state
  virtual QByteArray saveState()=0;

  // Restore the state
  virtual void restoreFromState (QByteArray ba)=0; 

  // Get the presenter's tree widget:
  QTreeWidget *presenterTreeWidget() const;

  // Set the presenter's tree widget:
  void setPresenterTreeWidget(QTreeWidget *);

  // Get the presenter's message text edit:
  QTextEdit *presenterMessageTextEdit() const;

  // Set the presenter's mesage text edit:
  void setPresenterMessageTextEdit(QTextEdit *);

  // Get the presenter's H1 Locator:
  HistoLocator1D *presenterH1Locator() const;

  // Set the presenter's H1 Locator:
  void setPresenterH1Locator(HistoLocator1D *);

  // Get the presenter's H2 Locator:
  HistoLocator2D *presenterH2Locator() const;

  // Set the presenter's H2 Locator:
  void setPresenterH2Locator(HistoLocator2D *);

  // Get the presenter's VisFunction Locator:
  VisFunctionLocator *presenterVisFunctionLocator() const;

  // Set the presenter's H2 Locator:
  void setPresenterVisFunctionLocator(VisFunctionLocator *);

  virtual DASystem *clone() const =0;

  virtual void dblClicked1D(const Hist1D *);
  virtual void dblClicked2D(const Hist2D *);
  virtual void dblClickedMan(const HistogramManager*);
  virtual void dblClickedVisFunction(const VisFunction *);

  // Return the tool box widget.
  virtual QWidget *toolWidget() const { return NULL;}


  // Save the state
  virtual QByteArray saveCache() { return saveState();}

  // Restore the state
  virtual void restoreFromCache (QByteArray ba) { restoreFromState(ba); }; 


 private:

  // Illegal operations:
  DASystem(const DASystem &);
  DASystem & operator=(const DASystem &);

  class Clockwork;
  Clockwork *c;
  
};

#endif     
   
