#ifndef _PLOTSYSTEMMENU_H_
#define _PLOTSYSTEMMENU_H_
#include <QWidget>
namespace Ui {
  class PlotSystemMenu;
}

class PlotSystemMenu: public QWidget {
  
  Q_OBJECT
    
    public:

  // Constructor:
  PlotSystemMenu(QWidget *parent=NULL);
  
  // Destructor:
  ~PlotSystemMenu();

  bool autorange () const;
  void setAutorange (bool flag);

  bool normalize () const;
  void setNormalize (bool flag);

  bool variable () const;
  void setVariable (bool flag);
  
  bool square () const;
  void setSquare (bool flag);

  bool rect () const;
  void setRect (bool flag);

 signals:
 
  void normalizeHistos(bool flag);
  void reDimension(int w, int h);

  private slots:
  
  void SSNormalize(int);

  void setPlotDimensions(bool unused);

 private:

  class Clockwork;
  Clockwork *c;

};

#endif
