#ifndef _PRESENTERTREEWIDGET_H_
#define _PRESENTERTREEWIDGET_H_
#include <QTreeWidget>
class PresenterTreeWidget: public QTreeWidget {
  Q_OBJECT

    public:

  PresenterTreeWidget(QWidget *parent=0);

  ~PresenterTreeWidget();

 protected:

  virtual void dropEvent(QDropEvent *event);

 signals:

  void itemDropped();

};
#endif
