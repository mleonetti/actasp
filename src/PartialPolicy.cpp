#include <actasp/PartialPolicy.h>

#include <actasp/Action.h>
#include <actasp/action_utils.h>

#include <algorithm>

using namespace std;

namespace actasp {
  
PartialPolicy::PartialPolicy(const ActionSet& actions) : policy(), allActions(actions) {}
	
ActionSet PartialPolicy::actions(const std::set<AspFluent>& state) const throw() {
  
	std::map<set<AspFluent>, ActionSet >::const_iterator acts = policy.find(state);

	if(acts != policy.end()) {
		return acts->second;
	}
	
	return ActionSet();
}

void PartialPolicy::merge(const AnswerSet& plan) throw(logic_error) {
  unsigned int planLength = plan.maxTimeStep();

  set<AspFluent> state = plan.getFluentsAtTime(0);
  
	//WARNING here we are assuming the action is attached to the next state.
	for (int timeStep = 1; timeStep <=planLength; ++timeStep) {
		
		set<AspFluent> stateWithAction = plan.getFluentsAtTime(timeStep);
    
    //find the action
    set<AspFluent>::iterator actionIt = find_if(stateWithAction.begin(),stateWithAction.end(),IsAnAction(allActions));
    
    if(actionIt == stateWithAction.end())
      throw logic_error("PartialPolicy: no action for some state");
    
    AspFluent action = *actionIt;
		
		//remove the action from there
		stateWithAction.erase(actionIt);
		
		ActionSet &stateActions = policy[state]; //creates an empty set if not present

		stateActions.insert(action);
		
		state = stateWithAction;
    
	}

}

struct MergeActions {
  MergeActions( std::map<std::set<AspFluent>, ActionSet, StateComparator<AspFluent> > &policy) : policy(policy) {}
 
 void operator()(const std::pair<set<AspFluent>, ActionSet >& stateActions) {
  
   map<set<AspFluent>, ActionSet >::iterator found = policy.find(stateActions.first);
   if(found == policy.end())
     policy.insert(stateActions);
   
   else {
     found->second.insert(stateActions.second.begin(),stateActions.second.end());
   }
     
   
 }
 
  std::map<std::set<AspFluent>, ActionSet, StateComparator<AspFluent> > &policy;
};

void PartialPolicy::merge(const PartialPolicy& otherPolicy) {
  
  set_union(otherPolicy.allActions.begin(),otherPolicy.allActions.end(),
                 allActions.begin(),allActions.end(),
                 inserter(allActions,allActions.begin()));
  
  for_each(otherPolicy.policy.begin(),otherPolicy.policy.end(),MergeActions(policy));
}

bool PartialPolicy::empty() const throw() {
	return policy.empty();
}



}
