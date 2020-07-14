#include "QatSystemInterfaces/Serializer.h"
#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotStream.h"
#include "QatPlotting/PlotHist1DProperties.h"
#include "QatPlotting/PlotFunction1DProperties.h"
#include "QatPlotting/PlotHist2DProperties.h"
#include <QByteArray>
#include <QBuffer>
#include <QDataStream>
#include <QTextEdit>
#include <QMessageBox>
#include <stdexcept>
#include <iostream>
class Serializer::Clockwork {
public:
  QByteArray   byteArray;
  QBuffer     *buffer;
  QDataStream *state;
  QTreeWidget *treeWidget;
  QTreeWidgetItem *currentItem;
};

class Deserializer::Clockwork {
public:
  QByteArray byteArray;
  QBuffer    *buffer;
  QDataStream *state;
  unsigned int version;
  
  QTreeWidget     *treeWidget;
  QTreeWidgetItem *currentItem;
};


Serializer::Serializer (unsigned int version) :c(new Clockwork()){
  c->buffer = new QBuffer(&c->byteArray);
  c->buffer->open(QIODevice::WriteOnly);
  c->state = new QDataStream(c->buffer);
  c->treeWidget=NULL;
  c->currentItem=NULL;
  save(version);
}

Deserializer::Deserializer(const QByteArray &ba):c(new Clockwork()){
  c->byteArray=ba;
  c->buffer=new QBuffer(&c->byteArray);
  if (!c->buffer->open(QIODevice::ReadOnly)) {
    delete c->buffer;
    c->buffer=NULL;
    throw std::runtime_error("Cannot Deserialize!");
  }
  else {
    c->state=new QDataStream(c->buffer);
    restore(c->version);
  }
  c->treeWidget=NULL;
  c->currentItem=NULL;
}
  


Serializer::~Serializer () {

  c->buffer->close();
  delete c->state;
  delete c->buffer;

  delete c;
}

Deserializer::~Deserializer(){
  if (c->buffer) {
    if (!c->buffer->atEnd()) {
      std::cout << "Warning, buffer not at end" << std::endl;
    }
    c->buffer->close();
  }
  delete c->state;
  delete c->buffer;
  delete c;
}


unsigned int Deserializer::version() const{
  return c->version;
}

QByteArray Serializer::result() const {
  return c->byteArray;
}

//==================int=========================

void Serializer::save(unsigned int i) {
  (*c->state) << i;
}


void Deserializer::restore(unsigned int & i){
  (*c->state) >> i;
}

//==================int=========================
void Serializer::save(int i) {
  (*c->state) << i;
}

void Deserializer::restore(int & i){
  (*c->state) >> i;
}


//==================double=========================
void Serializer::save(double d) {
  (*c->state) << d;
}

void Deserializer::restore(double & d){
  (*c->state) >> d;
}

//==================bool=========================

void Serializer::save(bool i) {
  (*c->state) << i;
}

void Deserializer::restore(bool & b){
  (*c->state) >> b;
}

//==================std::string=========================

void Serializer::save(const std::string & name) {
  QString nameString=QString::fromStdString(name);
  (*c->state) << nameString;
}
void Deserializer::restore(std::string & name){

  QString nameString;;
  (*c->state) >> nameString;
  name = nameString.toStdString();

}

//==QTextEdit==//
void Serializer::save(QTextEdit *tE) {
  (*c->state) << tE->document()->toHtml();
}
void Deserializer::restore(QTextEdit *tE) {
  QString html;
  (*c->state) >> html;
  tE->clear();
  tE->append(html);
}

 
//==PlotView==/
void Serializer::save(PlotView *plotView) {
  (*c->state) << plotView->titleTextEdit()->document()->toHtml();
  (*c->state) << plotView->xLabelTextEdit()->document()->toHtml();
  (*c->state) << plotView->yLabelTextEdit()->document()->toHtml();
  (*c->state) << plotView->vLabelTextEdit()->document()->toHtml();
  (*c->state) << plotView->statTextEdit()->document()->toHtml();
  
  QRectF rect;
  if (plotView->rect()) (*c->state) << *plotView->rect();

  // Page 1:
  (*c->state) << plotView->isBox();
  (*c->state) << plotView->isLogX();
  (*c->state) << plotView->isLogY();
  (*c->state) << plotView->isGrid();
  (*c->state) << plotView->isXZero();
  (*c->state) << plotView->isYZero();
  (*c->state) << plotView->statBoxX();
  (*c->state) << plotView->statBoxY();


  (*c->state) << plotView->xAxisFont();
  (*c->state) << plotView->yAxisFont();

  (*c->state) << plotView->statBoxX();
  (*c->state) << plotView->statBoxY();
  (*c->state) << plotView->labelXSizePercentage();
  (*c->state) << plotView->labelYSizePercentage();
}

void Deserializer::restore(PlotView *plotView) {
  {
    QString html;
    (*c->state) >> html;
    plotView->titleTextEdit()->clear();
    plotView->titleTextEdit()->append(html);
  }
  {
    QString html;
    (*c->state) >> html;
    plotView->xLabelTextEdit()->clear();
    plotView->xLabelTextEdit()->append(html);
  }
  {
    QString html;
    (*c->state) >> html;
    plotView->yLabelTextEdit()->clear();
    plotView->yLabelTextEdit()->append(html);
  }
  {
    QString html;
    (*c->state) >> html;
    plotView->vLabelTextEdit()->clear();
    plotView->vLabelTextEdit()->append(html);
  }
  {
    QString html;
    (*c->state) >> html;
    plotView->statTextEdit()->clear();
    plotView->statTextEdit()->append(html);
  }

  {
    QRectF rect;
    (*c->state) >> rect;
    plotView->setRect(rect);
  }

  {
    bool flag;
    (*c->state) >> flag;
    plotView->setBox(flag);
  }

  {
    bool flag;
    (*c->state) >> flag;
    plotView->setLogX(flag);
  }

  {
    bool flag;
    (*c->state) >> flag;
    plotView->setLogY(flag);
  }

  {
    bool flag;
    (*c->state) >> flag;
    plotView->setGrid(flag);
  }

  {
    bool flag;
    (*c->state) >> flag;
    plotView->setXZero(flag);
  }

  {
    bool flag;
    (*c->state) >> flag;
    plotView->setYZero(flag);
  }


  {
    qreal x,y;
    (*c->state) >> x >> y;
    plotView->setLabelPos(x,y);
  }

  {
    QFont font;
    (*c->state) >> font;
    plotView->xAxisFont()=font;
  }


  {
    QFont font;
    (*c->state) >> font;
    plotView->yAxisFont()=font;
  }

  {
    qreal x,y, w, h;
    if (version()>=3) {
      (*c->state) >> x;
      (*c->state) >> y;
      (*c->state) >> w;
      (*c->state) >> h;
      plotView->setLabelPos(x,y);
      plotView->setLabelXSizePercentage(w);
      plotView->setLabelYSizePercentage(h);
    }
  }  

}


//==================QSpinBox=========================

void Serializer::save(QSpinBox *spinBox) {
  (*c->state) << spinBox->value();
}

void Deserializer::restore(QSpinBox *spinBox){
  int value;
  (*c->state) >> value;
  spinBox->setValue(value);
}

struct ItemRep {
  std::string s0;
  std::string s1;
  bool isSelected;
  bool isExpanded;
  int  childCount;
};

class MatchItem {
public:
  MatchItem(QTreeWidgetItem *item):item(item) {;}
  bool operator () (const ItemRep & rep) {
    return rep.s0==item->text(0).toStdString() && rep.s1==item->text(1).toStdString();
  }
private:
  QTreeWidgetItem *item;
};
    


//==================QTreeWidget=========================

void Serializer::save(QTreeWidget * treeWidget) {
  //Prepare:
  c->treeWidget  = treeWidget;
  c->currentItem = treeWidget->currentItem();

  // Go:
  save(treeWidget->headerItem()->text(0).toStdString());
  save(treeWidget->headerItem()->text(1).toStdString());
  QList<QTreeWidgetItem *> itemList=treeWidget->findItems("*", Qt::MatchWildcard);
  save (itemList.size());
  for (int i=0;i<itemList.size();i++) {
    save(itemList[i]);
  }

  // End:
  c->treeWidget  = NULL;
  c->currentItem = NULL;
}

void Deserializer::restore(QTreeWidget *treeWidget){
  //Prepare:
  c->treeWidget  = treeWidget;
  c->currentItem = treeWidget->currentItem();
  
  // Go:
  { std::string  s; restore(s)   ;   treeWidget->headerItem()->setText(0, s.c_str());};
  { std::string  s; restore(s)   ;   treeWidget->headerItem()->setText(1, s.c_str());};
  
  int size      ; restore(size);   


  for (int i=0;i<size;i++) {

    // Peak--------------
    std::string s0,s1;
    restore(s0);
    restore(s1);
    //--------------------
    bool mismatch=false;

    QList<QTreeWidgetItem *> items=treeWidget->findItems(s0.c_str(), Qt::MatchExactly, 0);
    QTreeWidgetItem *item = items.size()==1 ? items[0]:NULL;
    
    //    if (!item) {
    //  std::ostringstream warningStream;
    //  warningStream << "The input " << s0 << " has not been loaded or cannot be located.";
    //  QMessageBox::warning(0, 
    //			   "Present",
    //			   warningStream.str().c_str(),
    //			   QMessageBox::Ok);
    //}


    //   QTreeWidgetItem *item = treeWidget->topLevelItem(i);


    if (!item) mismatch=true;
    restore(item, mismatch,s0,s1);
    if (c->buffer->atEnd()) break;
    if (mismatch) i--;
    
  }

  // End:
  c->treeWidget  = NULL;
  c->currentItem = NULL;
}

//==================QTreeWidgetItem=========================

void Serializer::save(QTreeWidgetItem * treeWidgetItem) {
  save(treeWidgetItem->text(0).toStdString());
  save(treeWidgetItem->text(1).toStdString());
  save(treeWidgetItem->isSelected());
  save(treeWidgetItem->isExpanded());
  save(treeWidgetItem->childCount());
  for (int i=0;i<treeWidgetItem->childCount();i++) {
    save(treeWidgetItem->child(i));
  }
}

void Deserializer::restore(QTreeWidgetItem *item, bool & mismatch, 
			   const std::string & s0, 
			   const std::string & s1){
  ItemRep rep;
  //  restore(rep.s0);
  //  restore(rep.s1);
  rep.s0=s0;
  rep.s1=s1;

  restore(rep.isSelected);
  restore(rep.isExpanded);
  restore(rep.childCount);

  if (!mismatch && rep.s0!=item->text(0).toStdString()) {
    mismatch=1;
  }  
  if (!mismatch) {
    item->setExpanded(rep.isExpanded);
    item->setSelected(rep.isSelected);
  }

  for (int i=0;i<rep.childCount;i++) {

    std::string s0,s1; // shadowing input parameters.
    restore(s0);
    restore(s1);


    QTreeWidgetItem *next = NULL;
    if (!mismatch)  {
      
      //next = item->child(i);

      for (int c=0;c<item->childCount();c++) {
	QTreeWidgetItem *testItem=item->child(c);
	if (testItem->text(0).toStdString()==s0) {
	  next=testItem;
	  break;
	}
      }
	
      //QList<QTreeWidgetItem *> items=item->findItems(s0.c_str(), Qt::MatchExactly, 0);
      //next = items.size()==1 ? items[0]:NULL;
    

    }

    restore(next, mismatch,s0,s1);
  }
  
  
}


void Deserializer::restore  (PlotHist1DProperties & prop) {
  (*c->state) >> prop.pen;
  (*c->state) >> prop.brush;

  int plotStyle;
  (*c->state) >> plotStyle;
  prop.plotStyle= PlotHist1DProperties::PlotStyle(plotStyle);

  int symbolStyle;
  (*c->state) >> symbolStyle;
  prop.symbolStyle=PlotHist1DProperties::SymbolStyle(symbolStyle);
  (*c->state) >> prop.symbolSize;
  
}

void Deserializer::restore  (PlotFunction1DProperties & prop) {
  (*c->state) >> prop.pen;
  (*c->state) >> prop.brush;

}

void Deserializer::restore (PlotHist2DProperties & prop) {
  (*c->state) >> prop.pen;
  (*c->state) >> prop.brush;
}


void Serializer::save (const PlotHist1DProperties & prop) {
  (*c->state) << prop.pen;
  (*c->state) << prop.brush;
  (*c->state) << (int) prop.plotStyle;
  (*c->state) << (int) prop.symbolStyle;
  (*c->state) << prop.symbolSize;
  
}

void Serializer::save (const PlotFunction1DProperties & prop) {
  (*c->state) << prop.pen;
  (*c->state) << prop.brush;
}

void Serializer::save (const PlotHist2DProperties & prop) {
  (*c->state) << prop.pen;
  (*c->state) << prop.brush;
}

