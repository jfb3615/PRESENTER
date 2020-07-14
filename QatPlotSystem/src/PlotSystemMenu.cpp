#include "QatPlotSystem/PlotSystemMenu.h"
#include "ui_PlotSystemMenu.h"
#include <iostream>
class PlotSystemMenu::Clockwork {
public:
  Ui::PlotSystemMenu ui;
};
PlotSystemMenu::~PlotSystemMenu() {
  delete c;
}

PlotSystemMenu::PlotSystemMenu (QWidget *parent ):QWidget(parent),c(new Clockwork()) {
  c->ui.setupUi(this);
  connect(c->ui.normalizeCheckBox, SIGNAL(stateChanged(int)), this, SLOT(SSNormalize(int)));
  connect(c->ui.squareButton, SIGNAL(toggled(bool)), this, SLOT(setPlotDimensions(bool)));
  connect(c->ui.rectButton, SIGNAL(toggled(bool)), this, SLOT(setPlotDimensions(bool)));
  connect(c->ui.variableButton, SIGNAL(toggled(bool)), this, SLOT(setPlotDimensions(bool)));
}
bool PlotSystemMenu::autorange () const {
  return c->ui.autorangeCheckBox->isChecked();
}
void PlotSystemMenu::setAutorange(bool flag) {
  c->ui.autorangeCheckBox->setChecked(flag);
}

bool PlotSystemMenu::variable () const {
  return c->ui.variableButton->isChecked();
}
void PlotSystemMenu::setVariable(bool flag) {
  c->ui.variableButton->setChecked(flag);
}

bool PlotSystemMenu::square () const {
  return c->ui.squareButton->isChecked();
}
void PlotSystemMenu::setSquare(bool flag) {
  c->ui.squareButton->setChecked(flag);
}

bool PlotSystemMenu::rect () const {
  return c->ui.rectButton->isChecked();
}
void PlotSystemMenu::setRect(bool flag) {
  c->ui.rectButton->setChecked(flag);
}

bool PlotSystemMenu::normalize () const {
  return c->ui.normalizeCheckBox->isChecked();
}
void PlotSystemMenu::setNormalize(bool flag) {
  emit normalizeHistos(flag);
  c->ui.normalizeCheckBox->setChecked(flag);
}
void PlotSystemMenu::SSNormalize(int cbState) {
  if (cbState==Qt::Checked) {
    emit normalizeHistos(true);
  }
  emit normalizeHistos(false);
}
void PlotSystemMenu::setPlotDimensions(bool) {

  bool var    = c->ui.variableButton->isChecked(); 
  bool square = c->ui.squareButton->isChecked(); 
  bool rect   = c->ui.rectButton->isChecked();
  if (var) emit reDimension(0,0);
  if (square) emit reDimension(600,600);
  if (rect)   emit reDimension(800,600);

}
