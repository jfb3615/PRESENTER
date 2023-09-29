#include "QatSpreadsheetSystem/SpreadsheetSystemMainWidget.h"
#include "QatSpreadsheetSystem/SpreadsheetSystem.h"
#include "QatDataAnalysis/Table.h"
#include <QScrollBar>
#include "ui_SpreadsheetSystemMainWidget.h"
#include <iostream>
#include <sstream>
class SpreadsheetSystemMainWidget::Clockwork {
 public:
  SpreadsheetSystem *system;
  int position;
  int nRows;
  const Table *table;
  Ui::SpreadsheetSystemMainWidget ui;
};

Ui::SpreadsheetSystemMainWidget & SpreadsheetSystemMainWidget::ui() {
  return c->ui;
}


SpreadsheetSystemMainWidget::SpreadsheetSystemMainWidget (QWidget *parent,SpreadsheetSystem *system):
  QWidget(parent),
  c(new Clockwork()){
  c->ui.setupUi(this);
  QObject::connect(c->ui.previousButton,    SIGNAL(clicked()),                       this, SLOT(decrement()));
  QObject::connect(c->ui.nextButton,        SIGNAL(clicked()),                       this, SLOT(increment()));
  QObject::connect(c->ui.nRowsSpinBox,      SIGNAL(valueChanged(int)),               this, SLOT(changeNRows(int)));
  QObject::connect(c->ui.positionSpinBox,   SIGNAL(valueChanged(int)),               this, SLOT(changePosition(int)));
 
  c->nRows=c->ui.nRowsSpinBox->value();
  c->position=c->ui.positionSpinBox->value();
  c->system=system;
}

SpreadsheetSystemMainWidget::~SpreadsheetSystemMainWidget () {
  delete c;
}
void SpreadsheetSystemMainWidget::increment() {
  // Make double sure:
  if (!c->table) return;

  c->position += c->nRows;
  if (c->position>= int(c->table->numTuples())-c->nRows) c->position=c->table->numTuples()-c->nRows-1;
  c->ui.positionSpinBox->setValue(c->position);
}

void SpreadsheetSystemMainWidget::decrement() {
  // Make double sure:
  if (!c->table) return;

  c->position -= c->nRows;
  if (c->position<0) c->position=0;
  c->ui.positionSpinBox->setValue(c->position);
}

void SpreadsheetSystemMainWidget::changeNRows(int n) {
  // Make double sure:
  if (!c->table) return;

  c->nRows=n;
  if (c->position>=int(c->table->numTuples())-c->nRows) c->position=c->table->numTuples()-c->nRows-1;
  if (c->position<0) c->position=0;
  c->ui.positionSpinBox->setValue(c->position);
  update();
}

void SpreadsheetSystemMainWidget::changePosition(int p) {

  // Make double sure:
  if (!c->table) return;

  c->position=p;
  if (c->position>=int(c->table->numTuples())-c->nRows) c->position=c->table->numTuples()-c->nRows-1;
  if (c->position<0) c->position=0;
  c->ui.positionSpinBox->setValue(c->position);
  update();
}

const Table * SpreadsheetSystemMainWidget::table() const {
  return c->table;
}

void SpreadsheetSystemMainWidget::setTable (const Table * table) {

  c->ui.positionSpinBox->setEnabled(table);
  c->ui.nRowsSpinBox->setEnabled(table);
  c->ui.nextButton->setEnabled(table);
  c->ui.nextButton->setEnabled(table);
  if (table) c->ui.tableNameLabel->setText(table->name().c_str());
  else       c->ui.tableNameLabel->setText("");


  c->table=table;
}

int SpreadsheetSystemMainWidget::getNRows() const {
  return c->nRows;
}

int SpreadsheetSystemMainWidget::getPosition() const {
  return c->position;
}


void SpreadsheetSystemMainWidget::update() {
  
  int scrollPosition=c->ui.tableWidget->horizontalScrollBar()->value();
  c->ui.tableWidget->clear();
  c->ui.tableWidget->setRowCount(0);
  c->ui.tableWidget->setColumnCount(0);
  c->ui.tableWidget->update();

  if (!c->table) {
    return;
  }


  unsigned int startRow=getPosition();    
  unsigned int numRows =getNRows();   
  
  c->ui.tableWidget->setRowCount(numRows);
  c->ui.tableWidget->setColumnCount(c->table->numAttributes());
  

  for (int a=0;a<c->nRows;a++) {
    QTableWidgetItem *vh = new QTableWidgetItem();
    std::ostringstream tStream;
    tStream << a+startRow;
    vh->setText(tStream.str().c_str());
    c->ui.tableWidget->setVerticalHeaderItem(a,vh);
  }


  for (unsigned int n=0;n<c->table->numAttributes();n++) {
    const Attribute & attribute=c->table->attribute(n);
    
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setText(attribute.name().c_str());
    c->ui.tableWidget->setHorizontalHeaderItem(n,item);
    for (unsigned int a=0;a<numRows;a++) {
      if (a+startRow < c->table->numTuples()) {
	float  F=0;
	double D=0;
	int    I=0;
	unsigned int UI=0;
        size_t       UI64=0;
	
	if (attribute.type()==Attribute::FLOAT) {
	 
	  c->table->read(a+startRow, attribute.name(), F);
	  std::ostringstream output;
	  output << F;
	  QTableWidgetItem *item = new QTableWidgetItem();
	  item->setText(output.str().c_str());
	  c->ui.tableWidget->setItem(a, n, item);
	  
	}
	if (attribute.type()==Attribute::DOUBLE) {
	  c->table->read(a+startRow, attribute.name(), D);
	  std::ostringstream output;
	  output << D;
	  QTableWidgetItem *item = new QTableWidgetItem();
	  item->setText(output.str().c_str());
	  c->ui.tableWidget->setItem(a, n, item);
	  
	}
	else if (attribute.type()==Attribute::INT) {
	  c->table->read(a+startRow, attribute.name(), I);
	  std::ostringstream output;
	  output << I;
	  QTableWidgetItem *item = new QTableWidgetItem();
	  item->setText(output.str().c_str());
	  c->ui.tableWidget->setItem(a, n, item);	      
	  
	}
	else if (attribute.type()==Attribute::UINT) {
	  c->table->read(a+startRow, attribute.name(), UI);
	  std::ostringstream output;
	  output << UI;
	  QTableWidgetItem *item = new QTableWidgetItem();
	  item->setText(output.str().c_str());
	  c->ui.tableWidget->setItem(a, n, item);	      
	}
	else if (attribute.type()==Attribute::UINT64) {
	  c->table->read(a+startRow, attribute.name(), UI64);
	  std::ostringstream output;
	  output << UI64;
	  QTableWidgetItem *item = new QTableWidgetItem();
	  item->setText(output.str().c_str());
	  c->ui.tableWidget->setItem(a, n, item);	      
	}
      }
    }
  }

  for (unsigned int n=0;n<c->table->numAttributes();n++) {
    c->ui.tableWidget->resizeColumnToContents(n);
  }
  c->ui.tableWidget->horizontalScrollBar()->setValue(scrollPosition);

}

void SpreadsheetSystemMainWidget::edit (QTableWidgetItem* /*item*/) {
// Not yet implemented but needed to satisfy linking on MacOSX    
}

