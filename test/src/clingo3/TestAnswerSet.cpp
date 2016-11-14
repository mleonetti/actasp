
#include "unittest++/UnitTest++.h"

#include "SimpleAction.h"

#include <actasp/AnswerSet.h>
#include <actasp/AspFluent.h>


#include <vector>
#include <algorithm>

using namespace std;
using namespace actasp;

TEST(AnswerSetContains) {

	std::set<actasp::AspFluent> fluents;
	
	fluents.insert(AspFluent("A(alice,bob,0)"));
	fluents.insert(AspFluent("B(alice,bob,0)"));
	fluents.insert(AspFluent("-A(carol,bob,0)"));
	
	AnswerSet answer(fluents.begin(), fluents.end());
	
	CHECK(answer.contains(AspFluent("B(alice,bob,0)")));
	CHECK(!answer.contains(AspFluent("B(alice,bob,1)")));
	CHECK(answer.contains(AspFluent("-A(carol,bob,0)")));
	CHECK(!answer.contains(AspFluent("A(carol,bob,0)")));
	
}

TEST(AnswerSetInstantiateActions) {
	
	std::map<std::string, actasp::Action *> actionMap;
	actionMap.insert(std::make_pair(std::string("north"), new SimpleAction("north")));
	actionMap.insert(std::make_pair(std::string("east"), new SimpleAction("east")));
	
	std::set<actasp::AspFluent> fluents;
	fluents.insert(AspFluent("north(param,1)"));
	fluents.insert(AspFluent("east(param,2)"));
	
	AnswerSet answer(fluents.begin(), fluents.end());
	
	list<Action*> plan = answer.instantiateActions(actionMap);
	
	bool correct = plan.front()->getName() == "north";
	delete plan.front();
	plan.pop_front();
	correct = correct && plan.front()->getName() == "east";
	delete plan.front();
	plan.pop_front();
	
	correct = correct && plan.empty();
	
	CHECK(correct);
	
	//exceptional cases
	//there's a hole in the plan
	fluents.insert(AspFluent("east(param,4)"));
	answer = AnswerSet(fluents.begin(), fluents.end());
	
	CHECK_THROW(answer.instantiateActions(actionMap), logic_error);
	
	std::map<std::string, actasp::Action *>::iterator actMapIt = actionMap.begin();
	for(; actMapIt != actionMap.end(); ++actMapIt)
		delete (actMapIt->second);
}

TEST(AnswerSetGetFluentsAtTime) {
	std::set<actasp::AspFluent> fluents;
	fluents.insert(AspFluent("north(param,0)"));
	fluents.insert(AspFluent("east(param,1)"));
	fluents.insert(AspFluent("pos(param,1)"));
	fluents.insert(AspFluent("-pos(param2,1)"));
	fluents.insert(AspFluent("east(param,2)"));
	
	AnswerSet answer(fluents.begin(), fluents.end());
	
	std::set<actasp::AspFluent> selected = answer.getFluentsAtTime(1);
	
	CHECK_EQUAL(3, selected.size());
	
	CHECK(std::equal(selected.begin(),selected.end(),++(fluents.begin())));
	
	CHECK_EQUAL(0, answer.getFluentsAtTime(4).size());
	
}

