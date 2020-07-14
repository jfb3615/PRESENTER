#ifndef _PLOTSYSTEMMAINWIDGET_H_
#define _PLOTSYSTEMMAINWIDGET_H_
#include <QWidget>
class  PlotView;
namespace Ui {
  class PlotSystemMainWidget;
}

class PlotSystemMainWidget: public QWidget {

  Q_OBJECT
    public:

  PlotSystemMainWidget(QWidget *parent=NULL);

  ~PlotSystemMainWidget();

  PlotView *plotView() const;


  public slots:
  void setDimension(int w, int h);


 private:

  class Clockwork;
  Clockwork *c;
  

};

#endif
