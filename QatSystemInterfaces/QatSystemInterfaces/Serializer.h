#ifndef _SERIALIZER_H_
#define _SERIALIZER_H_
#include <QByteArray>
#include <QSpinBox>
#include <QTreeWidget>
#include <QTextEdit>
#include <string>
class PlotHist1DProperties;
class PlotHist2DProperties;
class PlotFunction1DProperties;

class PlotView;
// This is a helper class for Serialization.  The design
// is taken from VP1 (Kittelmann & al.)

class Serializer {

 public:
  
  // Constructor:
  Serializer(unsigned int version);
 

  // Destructor:
  ~Serializer();

  // Get the result:
  QByteArray result() const;

  // Save methods, builtin types:
  void save(bool);
  void save(int);
  void save(unsigned int);
  void save(double);
  void save(const std::string & name);
  
  // Save methods, Widgets
  void save(QSpinBox *);
  void save(QTreeWidget *);
  void save(QTreeWidgetItem *);
  void save(PlotView *);
  void save(const PlotHist1DProperties & prop);
  void save(const PlotHist2DProperties & prop);
  void save(const PlotFunction1DProperties & prop);
  void save(QTextEdit *);

 private:


  class Clockwork;
  Clockwork *c;

};

class Deserializer {

public:

  // Constructor:
  Deserializer(const QByteArray &);
  

  // Destructor:
  ~Deserializer();

  // Access Version:
  unsigned int version() const;

  // Restore methods, builtin types:
  void restore(bool &);
  void restore(int &);
  void restore(unsigned int &);
  void restore(double &);
  void restore(std::string & name);
  
  // Restore methods, Widgets
  void restore(QSpinBox *);
  void restore(QTreeWidget *);
  void restore(QTreeWidgetItem *, bool &mismatch, const std::string & s0, const std::string & s1);
  void restore(PlotView *);
  void restore(PlotHist1DProperties     & prop);
  void restore(PlotHist2DProperties     & prop);
  void restore(PlotFunction1DProperties & prop);
  void restore(QTextEdit *);

private:

  class Clockwork;
  Clockwork *c;
};

#endif
