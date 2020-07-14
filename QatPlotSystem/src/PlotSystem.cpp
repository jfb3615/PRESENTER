#include <QObject>
#include "QatPlotSystem/PlotSystem.h"
#include "QatPlotSystem/PlotSystemMainWidget.h"
#include "QatPlotSystem/PlotSystemMenu.h"
#include "QatPlotting/PlotHist1D.h"
#include "QatPlotting/PlotHist2D.h"
#include "QatPlotting/PlotFunction1D.h"
#include "QatPlotting/PlotWave1D.h"
#include "QatPlotting/PlotOrbit.h"
#include "QatPlotting/VisFunction.h"
#include "QatSystemInterfaces/Serializer.h"
#include "QatDataAnalysis/Hist1D.h"
#include "QatDataAnalysis/Hist2D.h"
#include "QatDataAnalysis/HistogramManager.h"
#include "QatPlotWidgets/PlotFunction1DDialog.h"
#include "QatPlotWidgets/PlotOrbitDialog.h"
#include "QatPlotWidgets/PlotHist1DDialog.h"
#include "QatPlotWidgets/PlotHist2DDialog.h"
#include "QatPlotWidgets/PlotDirDialog.h"
#include "QatPlotWidgets/PlotView.h"
#include <QTextEdit>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <map>
class PlotSystemMainWidget;
class PlotSystem::Clockwork {
public:
  PlotSystemMainWidget *plotSystemMainWidget;
  PlotSystemMenu       *plotSystemToolWidget;

  std::map<const Hist1D *, PlotHist1D *> plotHist1DMap;
  std::map<const Hist1D *, PlotHist1D *> plotHist1DMapNormalized;
  std::map<const Hist2D *, PlotHist2D *> plotHist2DMap;
  // The following is used for PlotFunction1D and also for PlotOrbit:
  std::multimap<const VisFunction  *, Plotable *>   visFunctionMap;

  PlotHist1DDialog                      *dialog1;
  PlotHist2DDialog                      *dialog2;
  PlotDirDialog                         *dialogDir;
  PlotFunction1DDialog                  *dialogVis;
  PlotOrbitDialog                       *dialogOrb;
  std::vector<Hist1D *>                  normalizedHistograms;

  void setProperties(const Hist1D *h, PlotHist1DProperties & prop) {

    {
      std::map<const Hist1D *, PlotHist1D *>::const_iterator x=plotHist1DMap.find(h);
      if (x!=plotHist1DMap.end()) (*x).second->setProperties(prop);
    }
    {
      std::map<const Hist1D *, PlotHist1D *>::const_iterator x=plotHist1DMapNormalized.find(h);
      if (x!=plotHist1DMapNormalized.end()) (*x).second->setProperties(prop);
    }
  }
  PlotFunction1D *makePlot(const VisFunction *visFunction, unsigned int i) {
    PlotFunction1D *newPlot=NULL;
    if (visFunction->getFunction(i)->dimensionality()==1) {
      newPlot=
	visFunction->getDomainRestriction(i) ? 
	new PlotFunction1D(*visFunction->getFunction(i), *visFunction->getDomainRestriction(i)):
	new PlotFunction1D(*visFunction->getFunction(i));
      PlotFunction1D::Properties prop;
      prop.pen.setWidth(3);
      newPlot->setProperties(prop);
    }
    if (visFunction->getFunction(i)->dimensionality()==2) {
      newPlot=
	visFunction->getDomainRestriction(i) ? 
	new PlotWave1D(*visFunction->getFunction(i), *visFunction->getDomainRestriction(i)):
	new PlotWave1D(*visFunction->getFunction(i));
      PlotWave1D::Properties prop;
      prop.pen.setWidth(3);
      newPlot->setProperties(prop);
    }
    return newPlot;
  }
  PlotOrbit *makeOrbit(const VisFunction *visFunction) {
    PlotOrbit* newPlot=new PlotOrbit(*visFunction->getFunction(0),
				     *visFunction->getFunction(1),
				     0, 1);
    PlotOrbit::Properties prop;
    prop.pen.setWidth(3);
    newPlot->setProperties(prop);
    return newPlot;
  }
};


PlotSystem::PlotSystem ():c(new Clockwork()) {
  QRectF rect;

  c->plotSystemMainWidget=new PlotSystemMainWidget();
  c->plotSystemToolWidget=new PlotSystemMenu();
  c->dialog1=NULL;
  c->dialog2=NULL;
  c->dialogDir=NULL;
  c->dialogVis=NULL;
  c->dialogOrb=NULL;
  connect (c->plotSystemToolWidget, SIGNAL(normalizeHistos(bool)), this, SLOT(setNormalize(bool)));
  connect (c->plotSystemToolWidget, SIGNAL(reDimension(int, int)), c->plotSystemMainWidget, SLOT(setDimension(int, int)));
}

PlotSystem::~PlotSystem() {
  for (std::map<const Hist1D *, PlotHist1D *>::iterator m=c->plotHist1DMap.begin();m!=c->plotHist1DMap.end();m++) {
    delete (*m).second;
  }
  for (std::map<const Hist1D *, PlotHist1D *>::iterator m=c->plotHist1DMapNormalized.begin();m!=c->plotHist1DMapNormalized.end();m++) {
    delete (*m).second;
  }
  for (std::map<const VisFunction *, Plotable *>::iterator m=c->visFunctionMap.begin();m!=c->visFunctionMap.end();m++) {
    delete (*m).second;
  }
  for (unsigned int n=0;n<c->normalizedHistograms.size();n++) delete c->normalizedHistograms[n];
  delete c;
}

QWidget *PlotSystem::mainWidget() const {
  return c->plotSystemMainWidget;
}

QWidget *PlotSystem::toolWidget() const {
  return c->plotSystemToolWidget;
}

void PlotSystem::update() {

  // Clear the main widget:
  PlotView *plotView=c->plotSystemMainWidget->plotView();
  plotView->clear();

  // Handle the 1D Plots:

  QRectF r;
  bool plotsSelected=false;

  for (unsigned int i=0;i<hist1DSet()->size();i++) {
    
    plotsSelected=true;
    const Hist1D *hist1D=(*hist1DSet())[i];
    {
      std::ostringstream message;
      message << "Presenting 1D Histogram: " << hist1D->name() 
	      << " min=" << hist1D->min() 
	      << " max=" << hist1D->max()
	      << " nBins=" << hist1D->nBins() 
	      << " minContents=" << hist1D->minContents() 
	      << " maxContents=" << hist1D->maxContents() 
	      << " mean="        << hist1D->mean()
	      << " rms="    << sqrt(hist1D->variance()) 
	      << " under/over="    << hist1D->underflow() <<"/" << hist1D->overflow()  
	      << " sum="    << hist1D->sum()
	
	      << std::endl; 
      presenterMessageTextEdit()->insertPlainText (message.str().c_str());
    }
    {
      PlotHist1D *plotHist1DStd=NULL, *plotHist1DNormalized=NULL;
      std::map<const Hist1D *, PlotHist1D *>::const_iterator h=c->plotHist1DMap.find(hist1D);
      if (h==c->plotHist1DMap.end()) {

	// Put the basic plot:
	plotHist1DStd=new PlotHist1D(*hist1D);
	c->plotHist1DMap[hist1D]=plotHist1DStd;

	// Put the normalized plot:
	Hist1D normalizedHist=*hist1D;
	if (normalizedHist.sum()!=0) normalizedHist *= (1/normalizedHist.sum());
	normalizedHist *= (1/normalizedHist.binWidth());
	c->normalizedHistograms.push_back(new Hist1D(normalizedHist));
	plotHist1DNormalized=new PlotHist1D(*c->normalizedHistograms.back());
	c->plotHist1DMapNormalized[hist1D]=plotHist1DNormalized;

	PlotHist1D::Properties prop;
	prop.pen.setWidth(3);
	plotHist1DStd->setProperties(prop);
	plotHist1DNormalized->setProperties(prop);
	
      }
      else {
	plotHist1DStd = (*h).second;
	std::map<const Hist1D *, PlotHist1D *>::const_iterator i=c->plotHist1DMapNormalized.find(hist1D);
	plotHist1DNormalized = (*i).second;
	plotHist1DNormalized->setProperties(plotHist1DStd->properties());
      }
      PlotHist1D *plotHist1D = c->plotSystemToolWidget->normalize() ? plotHist1DNormalized : plotHist1DStd;

      
      if (!plotHist1D) throw std::runtime_error("Error locating 1D Plot");
      r |= plotHist1D->rectHint();
      plotView->add(plotHist1D);
    }
  }
  

  // Handle the 2D plots:
  for (unsigned int i=0;i<hist2DSet()->size();i++) {
    plotsSelected=true;
    const Hist2D *hist2D=(*hist2DSet())[i];
    PlotHist2D *plotHist2D=NULL;

    {
      std::ostringstream message;
      message << "Presenting 2D Histogram: " << hist2D->name() << std::endl;
      presenterMessageTextEdit()->insertPlainText (message.str().c_str());
    }

    std::map<const Hist2D *, PlotHist2D *>::const_iterator h=c->plotHist2DMap.find(hist2D);
    if (h==c->plotHist2DMap.end()) {
      plotHist2D=new PlotHist2D(*hist2D);
      PlotHist2D::Properties prop;
      prop.pen.setWidth(3);
      plotHist2D->setProperties(prop);
      c->plotHist2DMap[hist2D]=plotHist2D;
    }
    else {
      plotHist2D = (*h).second;
    }

    if (!plotHist2D) throw std::runtime_error("Error locating 2D Plot");
    r |= plotHist2D->rectHint();
    plotView->add(plotHist2D);

  }

  // Handle the Function Plots
  for (unsigned int i=0;i<visFunctionSet()->size();i++) {
    plotsSelected=true;
    const VisFunction *visFunction=(*visFunctionSet())[i];
    PlotFunction1D *plotFunction1D=NULL;
    PlotOrbit      *plotOrbit     =NULL;

    {
      std::ostringstream message;
      message << "Presenting Vis Function: " << visFunction->name() << std::endl;
      presenterMessageTextEdit()->insertPlainText (message.str().c_str());
    }

    //  std::map<const VisFunction *, PlotFunction1D *>::const_iterator h=c->visFunctionMap.find(visFunction);
    std::pair< 
    std::map<const VisFunction *, Plotable *>::const_iterator,
    std::map<const VisFunction *, Plotable *>::const_iterator 
    > ret=c->visFunctionMap.equal_range(visFunction);
    if (ret.first==ret.second) {
      if (visFunction->getMode()==VisFunction::NORMAL) {
	for (unsigned int i=0;i<visFunction->getNumFunctions();i++) {
	  plotFunction1D = c->makePlot(visFunction,i);
	  c->visFunctionMap.insert(std::make_pair(visFunction, plotFunction1D));
	  r |= visFunction->rectHint();
	  plotView->add(plotFunction1D);
	}
      }
      else if (visFunction->getMode()==VisFunction::ORBIT) {
	plotOrbit=c->makeOrbit(visFunction);
	c->visFunctionMap.insert(std::make_pair(visFunction, plotOrbit));
	plotView->add(plotOrbit);
	r |= visFunction->rectHint();
      }
    }
    else {
      std::map<const VisFunction *, Plotable *>::const_iterator 
	begin=ret.first,
	end=ret.second,
	h=begin;
      while (h!=end) {
	
	plotFunction1D = dynamic_cast<PlotFunction1D *> ((*h).second);
	if (plotFunction1D) {
	  r |= visFunction->rectHint();
	  plotView->add(plotFunction1D);
	}
	plotOrbit = dynamic_cast<PlotOrbit *> ((*h).second);
	if (plotOrbit) {
	  r |= visFunction->rectHint();
	  plotView->add(plotOrbit);
	}
	h++;
      }
    }


  }

  if (plotView->isLogY() || plotView->isLogX()) c->plotSystemToolWidget->setAutorange(false);
  if (plotsSelected && c->plotSystemToolWidget->autorange() ) plotView->setRect(r);
  plotView->update();
}

QByteArray PlotSystem::saveState() {
  Serializer serializer(3); 
  serializer.save((bool) c->plotSystemToolWidget->autorange());
  serializer.save((bool) c->plotSystemToolWidget->normalize());
  serializer.save(c->plotSystemMainWidget->plotView());

  {
    unsigned int sz=c->plotHist1DMap.size();
    serializer.save(sz);
    std::map<const Hist1D *, PlotHist1D *>::iterator 
    begin=c->plotHist1DMap.begin(),
    end  =c->plotHist1DMap.end(), p=begin;
  
    while (p!=end) {
      PlotHist1D::Properties prop=(*p).second->properties();
      const std::string *fullyQualifiedName = presenterH1Locator()->find((*p).first);
      if (fullyQualifiedName) {
	serializer.save(*fullyQualifiedName);
	serializer.save(prop);
      }
      else {
	serializer.save("Unknown");
	serializer.save(prop);
      }
      p++;
    }
  }

  {
    unsigned int sz=c->plotHist2DMap.size();
    serializer.save(sz);
    std::map<const Hist2D *, PlotHist2D *>::iterator 
    begin=c->plotHist2DMap.begin(),
    end  =c->plotHist2DMap.end(), p=begin;
  
    while (p!=end) {
      PlotHist2D::Properties prop=(*p).second->properties();
      const std::string *fullyQualifiedName = presenterH2Locator()->find((*p).first);
      if (fullyQualifiedName) {
	serializer.save(*fullyQualifiedName);
	serializer.save(prop);
      }
      else {
	serializer.save("Unknown");
	serializer.save(prop);
      }
      p++;
    }
  }

  {

    unsigned int sz=c->visFunctionMap.size();
    serializer.save(sz);
    std::map<const VisFunction *, Plotable *>::iterator 
    begin=c->visFunctionMap.begin(),
    end  =c->visFunctionMap.end(), p=begin;
  
    while (p!=end) {
      const VisFunction * vis = (*p).first;
      PlotFunction1D *pFunction=dynamic_cast<PlotFunction1D *> ((*p).second);
      if (pFunction) {
	PlotFunction1D::Properties prop=pFunction->properties();
	const std::string *fullyQualifiedName = presenterVisFunctionLocator()->find((*p).first);
	if (fullyQualifiedName) {
	  serializer.save(*fullyQualifiedName);
	  serializer.save(prop);
	}
	else {
	  serializer.save("Unknown");
	  serializer.save(prop);
	}
	serializer.save(vis->getNumParameters());
	for (unsigned int i=0;i<vis->getNumParameters();i++) {
	  serializer.save(vis->getParameter(i)->getValue());
	}
      }
      p++;
    }
  }

  {
    serializer.save((bool) c->plotSystemToolWidget->variable());
    serializer.save((bool) c->plotSystemToolWidget->square());
    serializer.save((bool) c->plotSystemToolWidget->rect());
  }
    
  return serializer.result();
}

void PlotSystem::restoreFromState (QByteArray ba){
  Deserializer state(ba);
  if (state.version()>3) throw std::runtime_error ("Wrong version code in PlotSystem::restoreFromState");
  bool autorange;
  state.restore(autorange);
  
  bool normalize=false;
  if (state.version() >=1) {
    state.restore(normalize);
  }


  state.restore(c->plotSystemMainWidget->plotView());

  c->plotSystemToolWidget->setAutorange(autorange);
  c->plotSystemToolWidget->setNormalize(normalize);
  
  {
    int num;
    state.restore(num);
    for (int i=0;i<num;i++) {
      std::string histoName;
      PlotHist1D::Properties prop;
      state.restore(histoName);
      state.restore(prop);

      const Hist1D * hist = presenterH1Locator()->find(histoName);
      if (hist) {
	std::map<const Hist1D *, PlotHist1D *>::const_iterator p = c->plotHist1DMap.find(hist);
	if (p!=c->plotHist1DMap.end()) {
	  (*p).second->setProperties(prop);
	}
      }
    }
  }

  {
    int num;
    state.restore(num);
    for (int i=0;i<num;i++) {
      std::string histoName;
      PlotHist2D::Properties prop;
      state.restore(histoName);
      state.restore(prop);

      const Hist2D * hist = presenterH2Locator()->find(histoName);
      if (hist) {
	std::map<const Hist2D *, PlotHist2D *>::const_iterator p = c->plotHist2DMap.find(hist);
	if (p!=c->plotHist2DMap.end()) {
	  (*p).second->setProperties(prop);
	}
      }


    }
  }

  if (state.version() >=2) {
    {
      int num;
      state.restore(num);
      for (int i=0;i<num;i++) {
	std::string fcnName;
	PlotFunction1D::Properties prop;
	state.restore(fcnName);
	state.restore(prop);
	
	const VisFunction * vis = presenterVisFunctionLocator()->find(fcnName);
	if (vis) {
	  std::map<const VisFunction *, Plotable *>::const_iterator p = c->visFunctionMap.find(vis);
	  if (p!=c->visFunctionMap.end()) {
	    PlotFunction1D *pFunction=dynamic_cast<PlotFunction1D *>((*p).second);
	    if (pFunction) pFunction->setProperties(prop);
	  }
	}
	unsigned int npar;
	state.restore(npar);
	for (unsigned int i=0;i<npar;i++) {
	  double v;
	  state.restore(v);
	  if (vis) vis->getParameter(i)->setValue(v);
	}
      }
    }
  }
  if (state.version()>=3) {
      bool var;
      state.restore(var);
      bool square;
      state.restore(square);
      bool rect;
      state.restore(rect);
      c->plotSystemToolWidget->setVariable(var);
      c->plotSystemToolWidget->setSquare(square);
      c->plotSystemToolWidget->setRect(rect);
      
  }

}




void PlotSystem::dblClicked1D(const Hist1D *hist1D) {

  // Make a dialog;

  if (!c->dialog1) {
    c->dialog1=new PlotHist1DDialog(presenterTreeWidget());
    connect(c->dialog1, SIGNAL(updateHisto()), this, SLOT(callUpdate()));
  }

  PlotHist1D *plotHist1D=NULL;
  std::map<const Hist1D *, PlotHist1D *>::const_iterator h=c->plotHist1DMap.find(hist1D);
  if (h==c->plotHist1DMap.end()) {
    plotHist1D=new PlotHist1D(*hist1D);
    PlotHist1D::Properties prop;
    prop.pen.setWidth(3);
    plotHist1D->setProperties(prop);
    c->plotHist1DMap[hist1D]=plotHist1D;
  }
  else {
    plotHist1D = (*h).second;
  }
  c->dialog1->synchTo(plotHist1D);
  c->dialog1->setWindowTitle(hist1D->name().c_str());
  c->dialog1->show();

}

void PlotSystem::callUpdate() {
  update();
}
void PlotSystem::dblClickedVisFunction (const VisFunction *vf) {
  // Make a dialog;

  if (!c->dialogVis) {
    c->dialogVis=new PlotFunction1DDialog(presenterTreeWidget());
    connect(c->dialogVis, SIGNAL(updateFunction()), this, SLOT(callUpdate()));
  }

  if (!c->dialogOrb) {
    c->dialogOrb=new PlotOrbitDialog(presenterTreeWidget());
    connect(c->dialogOrb, SIGNAL(updateFunction()), this, SLOT(callUpdate()));
  }

  PlotFunction1D *plotFunction1D=NULL;
  PlotOrbit      *plotOrbit     =NULL;
  //  std::map<const VisFunction *, PlotFunction1D *>::const_iterator h=c->visFunctionMap.find(vf);
  std::pair< 
    std::map<const VisFunction *, Plotable *>::const_iterator,
    std::map<const VisFunction *, Plotable *>::const_iterator 
    > ret=c->visFunctionMap.equal_range(vf);
  
  if (ret.first==ret.second) {

    if (vf->getMode()==VisFunction::NORMAL) {
      for (unsigned int i=0;i<vf->getNumFunctions();i++) {
	plotFunction1D = c->makePlot(vf,i);
	c->visFunctionMap.insert(std::make_pair(vf,plotFunction1D));
      }
    }
    else if (vf->getMode()==VisFunction::ORBIT) {
      plotOrbit = c->makeOrbit(vf);
      c->visFunctionMap.insert(std::make_pair(vf,plotOrbit));
    }
    
    
    // MAKE ALL PLOT FUNCTIONS
  }
  else {
    std::map<const VisFunction *, Plotable *>::const_iterator 
      begin=ret.first,
      end=ret.second,
      h=begin;
    while (h!=end) {
      PlotFunction1D *pFunction=dynamic_cast<PlotFunction1D *> ((*h).second);
      if (pFunction) {
	plotFunction1D = pFunction;
	break; // GET ANY OLD PLOT FUNCTION
      }
      h++;
    }
  }
  c->dialogVis->synchTo(plotFunction1D);
  c->dialogVis->clearParameters();

  for (unsigned int i=0;i<vf->getNumParameters();i++) {
    c->dialogVis->addParameter(vf->getParameter(i));
  }

  c->dialogVis->setWindowTitle(vf->name().c_str());
  c->dialogVis->show();
}
void PlotSystem::dblClicked2D (const Hist2D *hist2D) {
  // Make a dialog;

  if (!c->dialog2) {
    c->dialog2=new PlotHist2DDialog(presenterTreeWidget());
    connect(c->dialog2, SIGNAL(updateHisto()), this, SLOT(callUpdate()));
  }

  PlotHist2D *plotHist2D=NULL;
  std::map<const Hist2D *, PlotHist2D *>::const_iterator h=c->plotHist2DMap.find(hist2D);
  if (h==c->plotHist2DMap.end()) {
    plotHist2D=new PlotHist2D(*hist2D);
    PlotHist2D::Properties prop;
    prop.pen.setWidth(3);
    plotHist2D->setProperties(prop);
    c->plotHist2DMap[hist2D]=plotHist2D;
  }
  else {
    plotHist2D = (*h).second;
  }
  c->dialog2->synchTo(plotHist2D);
  c->dialog2->setWindowTitle(hist2D->name().c_str());
  c->dialog2->show();
}
void PlotSystem::dblClickedMan(const HistogramManager *dir) {

  if (!c->dialogDir) {
    c->dialogDir=new PlotDirDialog(presenterTreeWidget());
    connect(c->dialogDir, SIGNAL(updateManager()), this, SLOT(callUpdate()));
  }
  // 
  // First pass:   create plots (if necessary) and make collection.
  // 
  std::vector<PlotHist1D*> p1DSet;
  std::vector<PlotHist2D*> p2DSet;
  // 2D Histograms:
  for (HistogramManager::H1ConstIterator h1=dir->beginH1();h1!=dir->endH1();h1++) {

    const Hist1D *hist1D=*h1;
    PlotHist1D *plotHist1D=NULL;
    std::map<const Hist1D *, PlotHist1D *>::const_iterator h=c->plotHist1DMap.find(hist1D);
    if (h==c->plotHist1DMap.end()) {
      plotHist1D=new PlotHist1D(*hist1D);
      PlotHist1D::Properties prop;
      prop.pen.setWidth(3);
      plotHist1D->setProperties(prop);
      c->plotHist1DMap[hist1D]=plotHist1D;
    }
    else {
      plotHist1D = (*h).second;
    }
    p1DSet.push_back(plotHist1D);
  }
  // 2D Histograms:
  for (HistogramManager::H2ConstIterator h2=dir->beginH2();h2!=dir->endH2();h2++) {

    const Hist2D *hist2D=*h2;
    PlotHist2D *plotHist2D=NULL;
    std::map<const Hist2D *, PlotHist2D *>::const_iterator h=c->plotHist2DMap.find(hist2D);
    if (h==c->plotHist2DMap.end()) {
      plotHist2D=new PlotHist2D(*hist2D);
      PlotHist2D::Properties prop;
      prop.pen.setWidth(3);
      plotHist2D->setProperties(prop);
      c->plotHist2DMap[hist2D]=plotHist2D;
    }
    else {
      plotHist2D = (*h).second;
    }
    p2DSet.push_back(plotHist2D);
  }

  c->dialogDir->synchTo(p1DSet, p2DSet);
  c->dialogDir->setWindowTitle(dir->name().c_str());
  c->dialogDir->show();
}

void PlotSystem::setNormalize(bool) {
  std::ostringstream message;
  message << "Normalizing 1D histograms " << std::endl; 
  presenterMessageTextEdit()->insertPlainText (message.str().c_str());
  update();
}





QByteArray PlotSystem::saveCache() {
  Serializer serializer(0); 
  {
    unsigned int sz=c->visFunctionMap.size();
    serializer.save(sz);
    std::map<const VisFunction *, Plotable *>::iterator 
    begin=c->visFunctionMap.begin(),
    end  =c->visFunctionMap.end(), p=begin;
  
    while (p!=end) {
      const VisFunction * vis = (*p).first;
      const std::string *fullyQualifiedName = presenterVisFunctionLocator()->find((*p).first);
      if (fullyQualifiedName) {
	serializer.save(*fullyQualifiedName);
      }
      else {
	serializer.save("Unknown");
      }
      serializer.save(vis->getNumParameters());
      for (unsigned int i=0;i<vis->getNumParameters();i++) {
	serializer.save(vis->getParameter(i)->getValue());
      }
      p++;
    }
  }


  return serializer.result();
}

void PlotSystem::restoreFromCache (QByteArray ba){
  Deserializer state(ba);
  if (state.version()>0) throw std::runtime_error ("Wrong version code in PlotSystem::restoreFromCache");
  {
    int num;
    state.restore(num);
    for (int i=0;i<num;i++) {
      std::string fcnName;
      state.restore(fcnName);
      const VisFunction * vis = presenterVisFunctionLocator()->find(fcnName);
      if (vis) {
	unsigned int npar;
	state.restore(npar);
	for (unsigned int i=0;i<npar;i++) {
	  double v;
	  state.restore(v);
	  if (vis) vis->getParameter(i)->setValue(v);
	}
      }
    }
  }
}






//=====================Boilerplate======================//
PlotSystem *PlotSystem::clone() const {
  return new PlotSystem();
}


extern "C" PlotSystem *create_PlotSystem()  {
  return new PlotSystem();
}
