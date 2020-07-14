#ifndef _HISTOLOCATOR_H_
#define _HISTOLOCATOR_H_

class HistoLocator1D {
  
 public:

  // Constructor:
  HistoLocator1D(){};

  // Destructor:
  ~HistoLocator1D(){};

  // Insert
  void insert(const Hist1D *histo, std::string & name) {
    histoMap[histo]=name;
    nameMap [name ]=histo;
  }

  // Find forward
  const std::string *find  (const Hist1D *histo) const {
    std::map<const Hist1D *, std::string>::const_iterator h=histoMap.find(histo);
    return h==histoMap.end() ? NULL :  & (*h).second;
  }

  // Find reverse
  const Hist1D *find  (const std::string & name) const {
    std::map<std::string, const Hist1D *>::const_iterator h=nameMap.find(name);
    return h==nameMap.end() ? NULL :  (*h).second;
  }

 private:

  std::map<const Hist1D *, std::string>       histoMap;
  std::map<std::string,    const Hist1D *>    nameMap;

};


class HistoLocator2D {
  
 public:

  // Constructor:
  HistoLocator2D(){};

  // Destructor:
  ~HistoLocator2D(){};

  // Insert
  void insert(const Hist2D *histo, std::string & name) {
    histoMap[histo]=name;
    nameMap [name ]=histo;
  }

  // Find forward
  const std::string *find  (const Hist2D *histo) const {
    std::map<const Hist2D *, std::string>::const_iterator h=histoMap.find(histo);
    return h==histoMap.end() ? NULL :  & (*h).second;
  }

  // Find reverse
  const Hist2D *find  (const std::string & name) const {
    std::map<std::string, const Hist2D *>::const_iterator h=nameMap.find(name);
    return h==nameMap.end() ? NULL :  (*h).second;
  }

 private:

  std::map<const Hist2D *, std::string>       histoMap;
  std::map<std::string,    const Hist2D *>    nameMap;

};



class VisFunctionLocator {
  
 public:

  // Constructor:
  VisFunctionLocator(){};

  // Destructor:
  ~VisFunctionLocator(){};

  // Insert
  void insert(const VisFunction *vis, std::string & name) {
    visMap[vis]=name;
    nameMap [name ]=vis;
  }

  // Find forward
  const std::string *find  (const VisFunction *vis) const {
    std::map<const VisFunction *, std::string>::const_iterator h=visMap.find(vis);
    return h==visMap.end() ? NULL :  & (*h).second;
  }

  // Find reverse
  const VisFunction *find  (const std::string & name) const {
    std::map<std::string, const VisFunction *>::const_iterator h=nameMap.find(name);
    return h==nameMap.end() ? NULL :  (*h).second;
  }

 private:

  std::map<const VisFunction *, std::string>       visMap;
  std::map<std::string,    const VisFunction *>    nameMap;

};

#endif
