#ifndef actasp_PartialPolicy_h__guard
#define actasp_PartialPolicy_h__guard

#include <actasp/AnswerSet.h>
#include <actasp/AspFluent.h>
#include <actasp/state_utils.h>

#include <set>
#include <map>
#include <stdexcept>

namespace actasp {
  
class PartialPolicy {
public:
  
  PartialPolicy(const ActionSet& actions);
  
	ActionSet actions(const std::set<AspFluent>& state) const throw();
	
	void merge(const AnswerSet& plan) throw(std::logic_error);
  void merge(const PartialPolicy& otherPolicy);
	
	bool empty()const throw();
	
private:
	std::map<std::set<AspFluent>, ActionSet, StateComparator<AspFluent> > policy;
  ActionSet allActions;
	
};
	
}

#endif
