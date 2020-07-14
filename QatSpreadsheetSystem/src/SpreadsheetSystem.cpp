#include "QatSpreadsheetSystem/SpreadsheetSystem.h"
#include "QatSpreadsheetSystem/SpreadsheetSystemMainWidget.h"
#include "QatSystemInterfaces/Serializer.h"
#include "QatDataAnalysis/HistogramManager.h"
#include <QObject>
#include <QTreeWidget>
#include <stdexcept>
#include <iostream>
#include "ui_SpreadsheetSystemMainWidget.h"

class SpreadsheetSystemMainWidget;
class SpreadsheetSystem::Clockwork {
public:
  SpreadsheetSystemMainWidget     *mainWidget;
  QWidget                         *toolWidget;
};

SpreadsheetSystem::SpreadsheetSystem ():c(new Clockwork()) {

  c->mainWidget=new SpreadsheetSystemMainWidget(NULL,this);
  c->toolWidget = new QWidget();

}

SpreadsheetSystem::~SpreadsheetSystem() {
  delete c;
}

QWidget *SpreadsheetSystem::mainWidget() const {
  return c->mainWidget;
}

QWidget *SpreadsheetSystem::toolWidget() const {
  return c->toolWidget;
}


void SpreadsheetSystem::update() {

  if (tableSet()->size()==1) {
    const Table *table=(*tableSet())[0];
    c->mainWidget->setTable(table);
  }
  else {
    c->mainWidget->setTable(NULL);
  }

  c->mainWidget->update();


}



QByteArray SpreadsheetSystem::saveState() {
  Serializer serializer(0); 
  serializer.save(c->mainWidget->ui().nRowsSpinBox);
  serializer.save(c->mainWidget->ui().positionSpinBox);
  //  serializer.save(presenterTreeWidget());
  return serializer.result();
}

void SpreadsheetSystem::restoreFromState (QByteArray ba){

  Deserializer state(ba);
  if (state.version()!=0) throw std::runtime_error ("Wrong version code in SpreadsheetSystem::restoreFromState");
  state.restore(c->mainWidget->ui().nRowsSpinBox);
  state.restore(c->mainWidget->ui().positionSpinBox);
  //state.restore(presenterTreeWidget());
}



//=====================Boilerplate======================//
SpreadsheetSystem *SpreadsheetSystem::clone() const {
  return new SpreadsheetSystem();
}


extern "C" SpreadsheetSystem *create_SpreadsheetSystem()  {
  return new SpreadsheetSystem();
}
