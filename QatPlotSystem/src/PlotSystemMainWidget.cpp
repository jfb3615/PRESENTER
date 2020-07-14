#include "QatPlotSystem/PlotSystemMainWidget.h"
#include "ui_PlotSystemMainWidget.h"
class PlotSystemMainWidget::Clockwork {
public:
  Ui::PlotSystemMainWidget ui;
};
PlotSystemMainWidget::PlotSystemMainWidget (QWidget *parent ):QWidget(parent),c(new Clockwork()) {
  c->ui.setupUi(this);
  c->ui.plotView->setBox(false);
}
PlotView *PlotSystemMainWidget::plotView() const {
  return c->ui.plotView;
}
PlotSystemMainWidget::~PlotSystemMainWidget () {
  delete c;
}

void PlotSystemMainWidget::setDimension(int w, int h) {
  if (w==0 && h==0) {
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(10);
    sizePolicy.setVerticalStretch(10);
    c->ui.plotView->setMinimumSize(600,600);
    c->ui.plotView->setMaximumSize(0xFFFFFF,0xFFFFFF);
    c->ui.plotView->setSizePolicy(sizePolicy);
  }
  else {
    c->ui.plotView->setFixedSize(w,h);
  }
}
