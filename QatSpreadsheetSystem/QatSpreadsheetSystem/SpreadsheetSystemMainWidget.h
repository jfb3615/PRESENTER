#ifndef _SPREADSHEETSYSTEMMAINWIDGET_H_
#define _SPREADSHEETSYSTEMMAINWIDGET_H_
class Table;
class QTableWidgetItem;
class SpreadsheetSystem;
#include <QWidget>
namespace Ui {
  class SpreadsheetSystemMainWidget;
}

class SpreadsheetSystemMainWidget: public QWidget {

  Q_OBJECT

  public:

  // Constructor:
  SpreadsheetSystemMainWidget(QWidget *parent=NULL,SpreadsheetSystem *system=NULL);

  // Destructor:
  ~SpreadsheetSystemMainWidget();

  int getPosition() const;
  int getNRows() const;

  const Table * table() const;
  void setTable (const Table * table); 

  Ui::SpreadsheetSystemMainWidget & ui();

  public slots:

  void increment();
  void decrement();
  void changeNRows(int i);
  void changePosition(int i);
  void update();
  void edit (QTableWidgetItem *item);

 private:

  class Clockwork;
  Clockwork *c;

};

#endif
