#ifndef _PLOTSYSTEM_H_
#define _PLOTSYSTEM_H_
#include <QatSystemInterfaces/SystemInterface.h>
#include <QWidget>
class PlotSystem:public DASystem {

  Q_OBJECT

public:

  PlotSystem();

  virtual ~PlotSystem();
  
  virtual QString name() { return QString("PlotSystem");}

  virtual QWidget *mainWidget() const;

  virtual QWidget *toolWidget() const;

  virtual void update();

  virtual QByteArray saveState();

  virtual void restoreFromState (QByteArray ba); 

  virtual QByteArray saveCache();

  virtual void restoreFromCache (QByteArray ba); 

  virtual PlotSystem *clone() const;

  virtual void dblClicked1D(const Hist1D *);
  virtual void dblClicked2D (const Hist2D *);
  virtual void dblClickedMan(const HistogramManager *);
  virtual void dblClickedVisFunction(const VisFunction *);

  public slots:
  
  void callUpdate();
  void setNormalize(bool);

 private:

  // It is illegal to copy or assign:
  PlotSystem(const PlotSystem &);
  PlotSystem & operator=(const PlotSystem &);


  // Internal:
  class Clockwork;
  Clockwork *c;
};

#endif
