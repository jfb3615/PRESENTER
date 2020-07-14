#include "QatSystemInterfaces/SystemInterface.h"
#include "QatDataAnalysis/HistogramManager.h"

class DASystem::Clockwork {
public:
  Hist1DSet       *hist1DSet;
  Hist2DSet       *hist2DSet;
  VisFunctionSet  *visFunctionSet;
  TableSet        *tableSet;
  QTreeWidget     *presenterTreeWidget;
  QTextEdit       *presenterMessageTextEdit;
  HistoLocator1D  *presenterH1Locator;
  HistoLocator2D  *presenterH2Locator;
  VisFunctionLocator *presenterVisFunctionLocator;
};

DASystem::DASystem():c(new Clockwork()){
  c->hist1DSet=NULL;
  c->hist2DSet=NULL;
  c->visFunctionSet=NULL;
  c->tableSet =NULL;
  c->presenterTreeWidget=NULL;
  c->presenterMessageTextEdit=NULL;
  c->presenterH1Locator=NULL;
  c->presenterH2Locator=NULL;
  c->presenterVisFunctionLocator=NULL;
}

DASystem::~DASystem() {
  delete c;
}

void DASystem::setHist1DSet(Hist1DSet * hist1DSet) {
  c->hist1DSet=hist1DSet;
}

void DASystem::setHist2DSet(Hist2DSet * hist2DSet) {
  c->hist2DSet=hist2DSet;
}


void DASystem::setTableSet(TableSet * tableSet) {
  c->tableSet=tableSet;
}

void DASystem::setVisFunctionSet(VisFunctionSet * visFunctionSet) {
  c->visFunctionSet = visFunctionSet;
}

const Hist1DSet *DASystem::hist1DSet() const {
  return c->hist1DSet;
}


const Hist2DSet *DASystem::hist2DSet() const {
  return c->hist2DSet;
}

const VisFunctionSet *DASystem::visFunctionSet() const {
  return c->visFunctionSet;
}

const TableSet *DASystem::tableSet() const {
  return c->tableSet;
}

QTreeWidget *DASystem::presenterTreeWidget() const {
  return c->presenterTreeWidget;
}

void DASystem::setPresenterTreeWidget (QTreeWidget *presenterTreeWidget) {
  c->presenterTreeWidget=presenterTreeWidget;
}

QTextEdit *DASystem::presenterMessageTextEdit()  const {
  return c->presenterMessageTextEdit;
}

void DASystem::setPresenterMessageTextEdit (QTextEdit *presenterMessageTextEdit) {
  c->presenterMessageTextEdit=presenterMessageTextEdit;
}


void DASystem::dblClicked1D(const Hist1D *){;}
void DASystem::dblClicked2D(const Hist2D *){;}
void DASystem::dblClickedMan(const HistogramManager*) {;}
void DASystem::dblClickedVisFunction(const VisFunction*) {;}


HistoLocator1D *DASystem::presenterH1Locator() const {
  return c->presenterH1Locator;
}

void DASystem::setPresenterH1Locator(HistoLocator1D *locator) {
  c->presenterH1Locator=locator;
}

HistoLocator2D *DASystem::presenterH2Locator() const {
  return c->presenterH2Locator;
}

void DASystem::setPresenterH2Locator(HistoLocator2D *locator) {
  c->presenterH2Locator=locator;
}

VisFunctionLocator *DASystem::presenterVisFunctionLocator() const {
  return c->presenterVisFunctionLocator;
}

void DASystem::setPresenterVisFunctionLocator(VisFunctionLocator *locator) {
  c->presenterVisFunctionLocator=locator;
}
