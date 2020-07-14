#include "QatPresenter/PresenterTreeWidget.h"
PresenterTreeWidget::PresenterTreeWidget(QWidget *parent) :
  QTreeWidget(parent) {
}

PresenterTreeWidget::~PresenterTreeWidget() {
}

void PresenterTreeWidget::dropEvent(QDropEvent *event) {
  QAbstractItemView::dropEvent(event);
  emit itemDropped();
}
