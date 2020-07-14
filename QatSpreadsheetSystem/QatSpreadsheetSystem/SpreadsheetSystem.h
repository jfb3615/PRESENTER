#ifndef _SPREADSHEETSYSTEM_H_
#define _SPREADSHEETSYSTEM_H_
#include <QatSystemInterfaces/SystemInterface.h>
#include <QWidget>
class HistogramManager;
class SpreadsheetSystem:public DASystem {
public:

  SpreadsheetSystem();

  virtual ~SpreadsheetSystem();
  
  virtual QString name() { return QString("SpreadsheetSystem");}

  virtual QWidget *mainWidget() const;

  virtual QWidget *toolWidget() const;

  virtual void update();

  virtual QByteArray saveState();

  virtual void restoreFromState (QByteArray ba); 

  virtual SpreadsheetSystem *clone() const;


 private:

  // It is illegal to copy or assign:
  SpreadsheetSystem(const SpreadsheetSystem &);
  SpreadsheetSystem & operator=(const SpreadsheetSystem &);


  // Internal:
  class Clockwork;
  Clockwork *c;
};

#endif
