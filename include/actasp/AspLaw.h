#ifndef actasp_AspLaw_h__guard
#define actasp_AspLaw_h__guard

#include <vector>

namespace actasp {

template<typename AtomType>
struct AspLaw {
  
  AspLaw() : head(), body() {}
  
  
  AspLaw& operator<< (AtomType fluent) throw (){
    body.push_back(fluent);
    return *this;
  }
  
  std::vector<AtomType> head;
  std::vector<AtomType> body;

};

}

#endif
