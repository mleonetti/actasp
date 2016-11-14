#include "unittest++/UnitTest++.h"

#include <actasp/reasoners/Reasoner.h>
#include <actasp/reasoners/Clingo3.h>
// #include <actasp/Action.h>
// #include <actasp/AspRule.h>
// #include <actasp/AspFluent.h>
// #include <actasp/action_utils.h>
// 
// #include "SimpleAction.h"
// 
// #include <map>
// #include <string>
// #include <list>
// #include <vector>
// #include <stdexcept>
// #include <algorithm>
// #include <iostream>
// #include <iterator>


struct ReasonerFixture {
  ReasonerFixture() {

    actasp::ActionSet actions;

    actions.insert(actasp::AspFluent("north",std::vector<std::string>()));
    actions.insert(actasp::AspFluent("east",std::vector<std::string>()));
    actions.insert(actasp::AspFluent("south",std::vector<std::string>()));
    actions.insert(actasp::AspFluent("west",std::vector<std::string>()));

    clingo3 = new actasp::Clingo3("n", "../test/queries/", "../test/griddomain/",actions);
    reasoner = new actasp::Reasoner(clingo3,36,actions);
    
    //set initial state to 2,0
    std::vector<actasp::AspFluent> initialObs;
    initialObs.push_back(actasp::AspFluent("pos(2,0,0)"));
    CHECK(reasoner->updateFluents(initialObs));

  }
  ~ReasonerFixture() {
    delete clingo3;
    delete reasoner;
  }

  actasp::Reasoner *reasoner;
  actasp::Clingo3 *clingo3;

};



TEST_FIXTURE(ReasonerFixture, updateCurrentStateTest) {

  //I'm initially in 2,1
  std::vector<actasp::AspFluent> initialObs;
  initialObs.push_back(actasp::AspFluent("pos(2,1,0)"));
  
  CHECK(reasoner->updateFluents(initialObs));

  //I'm initially not in 2,0
  std::vector<actasp::AspRule> stateTest1(1);
  stateTest1[0].body.push_back(actasp::AspFluent("not pos(2,0,0)"));

  CHECK(!(reasoner->currentStateQuery(stateTest1).isSatisfied()));
  
  std::vector<actasp::AspFluent> observations;
  observations.push_back(actasp::AspFluent("pos(2,0,0)"));
  
  CHECK(reasoner->updateFluents(observations));
  
  CHECK(reasoner->currentStateQuery(stateTest1).isSatisfied());
  
  //the obstacles that are always there are in fact there
  std::vector<actasp::AspRule> stateTest2(10);
  stateTest2[0].body.push_back(actasp::AspFluent("not obst(1,1,2,1,0)."));
  stateTest2[1].body.push_back(actasp::AspFluent("not obst(1,2,2,2,0)."));
  stateTest2[2].body.push_back(actasp::AspFluent("not obst(1,3,2,3,0)."));
  stateTest2[3].body.push_back(actasp::AspFluent("not obst(2,1,1,1,0)."));
  stateTest2[4].body.push_back(actasp::AspFluent("not obst(2,2,1,2,0)."));
  stateTest2[5].body.push_back(actasp::AspFluent("not obst(2,3,1,3,0)."));
  stateTest2[6].body.push_back(actasp::AspFluent("not obst(3,3,3,4,0)."));
  stateTest2[7].body.push_back(actasp::AspFluent("not obst(3,4,3,3,0)."));
  stateTest2[8].body.push_back(actasp::AspFluent("not obst(4,3,4,4,0)."));
  stateTest2[9].body.push_back(actasp::AspFluent("not obst(4,4,4,3,0)."));  
  
  CHECK(reasoner->currentStateQuery(stateTest2).isSatisfied());
}


TEST_FIXTURE(ReasonerFixture, planningTest) {

  std::vector<actasp::AspRule> goalFormula;
  actasp::AspRule goalRule;
  goalRule.body.push_back(actasp::AspFluent("not pos(3,4,n)"));
  goalFormula.push_back(goalRule);

  std::set<actasp::AspFluent> planFluents;
  planFluents.insert(actasp::AspFluent("north(1)"));
  planFluents.insert(actasp::AspFluent("north(2)"));
  planFluents.insert(actasp::AspFluent("north(3)"));
  planFluents.insert(actasp::AspFluent("north(4)"));
  planFluents.insert(actasp::AspFluent("east(5)"));
  
  actasp::AnswerSet correctPlan(planFluents.begin(), planFluents.end());

  actasp::AnswerSet plan = reasoner->computePlan(goalFormula);

  CHECK_EQUAL(5,plan.getFluents().size());
  
  CHECK(std::equal(plan.getFluents().begin(), plan.getFluents().end(), correctPlan.getFluents().begin())); //the plans are equal

}
/*
TEST_FIXTURE(ReasonerFixture, emptyPlanTest) {

  std::vector<actasp::AspRule> goalFormula;
  actasp::AspRule goalRule;
  goalRule.body.push_back(actasp::AspFluent("not pos(2,0,n)"));
  goalFormula.push_back(goalRule);

  actasp::AnswerSet plan = reasoner->computePlan(goalFormula);

  CHECK(plan.getFluents().empty());
}

TEST_FIXTURE(ReasonerFixture, planValidityTest) {
  

  std::vector<actasp::AspRule> goalFormula;
  actasp::AspRule goalRule;
  goalRule.body.push_back(actasp::AspFluent("not pos(3,4,n)"));
  goalFormula.push_back(goalRule);

  actasp::AnswerSet plan = reasoner->computePlan(goalFormula);

  CHECK(reasoner->isPlanValid(plan,goalFormula));

  std::vector<actasp::AspRule> wrongGoal;
  actasp::AspRule wrongGoalRule;
  wrongGoalRule.body.push_back(actasp::AspFluent("not pos(1,4,n)"));
  wrongGoal.push_back(wrongGoalRule);

   CHECK(!(reasoner->isPlanValid(plan,wrongGoal)));

}

TEST_FIXTURE(ReasonerFixture, currentStateTest) {

  //I am in 2,0
  //should be satisfied, it's the initial position
  std::vector<actasp::AspRule> stateTest1(1);
  stateTest1[0].head.push_back(actasp::AspFluent("pos(2,0,0)"));

  CHECK(reasoner->currentStateQuery(stateTest1).isSatisfied());
  
  //I am in 2,1 (note head)
  //should not be satisfied, I am in 2,0 and cannot be in two places
  //at the same time
  std::vector<actasp::AspRule> stateTest2(1);
  stateTest2[0].head.push_back(actasp::AspFluent("pos(2,1,0)"));
  
  CHECK(!(reasoner->currentStateQuery(stateTest2).isSatisfied()));
  
  //It is impossible that I am not in 2,1 (note body)
  //should not be satisfied, I am in 2,0 and cannot prove I am in 2,1
  std::vector<actasp::AspRule> stateTest3(1);
  stateTest3[0].body.push_back(actasp::AspFluent("not pos(2,1,0)"));
  
  CHECK(!(reasoner->currentStateQuery(stateTest3).isSatisfied()));
}

TEST_FIXTURE(ReasonerFixture, policyTest) {
  
  std::vector<actasp::AspRule> goalFormula;
    actasp::AspRule goalRule;
  goalRule.body.push_back(actasp::AspFluent("not pos(4,2,n)"));
  goalFormula.push_back(goalRule);
  
  actasp::PartialPolicy  *policy = reasoner->computePolicy(goalFormula,1);
  
  actasp::AnswerSet queryAnswer = reasoner->currentStateQuery(std::vector<actasp::AspRule>());
  std::set<actasp::AspFluent> state(queryAnswer.getFluents().begin(), queryAnswer.getFluents().end());
  actasp::ActionSet actions = policy->actions(state); 

  CHECK_EQUAL(2,actions.size());
  
  CHECK(  (actions.find(actasp::AspFluent("north(0)")) != actions.end()) && (actions.find(actasp::AspFluent("east(0)")) != actions.end()));
  

  goalFormula.clear();
  goalRule = actasp::AspRule();
  goalRule.body.push_back(actasp::AspFluent("not pos(2,4,n)"));
  goalFormula.push_back(goalRule);
  
  delete policy;
  policy = reasoner->computePolicy(goalFormula,1.5);
  actions = policy->actions(state); 
  
  CHECK_EQUAL(3,actions.size());
  
  CHECK(  (actions.find(actasp::AspFluent("north(0)")) != actions.end()) && 
          (actions.find(actasp::AspFluent("east(0)")) != actions.end()) &&
          (actions.find(actasp::AspFluent("west(0)")) != actions.end()));


  std::vector<actasp::AspFluent> anotherState;
  anotherState.push_back(actasp::AspFluent("pos(4,1,0)"));
  
  CHECK(reasoner->updateFluents(anotherState));

  goalFormula.clear();
  goalRule = actasp::AspRule();
  goalRule.body.push_back(actasp::AspFluent("not pos(4,2,n)"));
  goalFormula.push_back(goalRule);
  
  delete policy;
  policy = reasoner->computePolicy(goalFormula,1);
  queryAnswer = reasoner->currentStateQuery(std::vector<actasp::AspRule>());
  state.clear();
  state.insert(queryAnswer.getFluents().begin(), queryAnswer.getFluents().end());
  actions = policy->actions(state);
  
  
  CHECK_EQUAL(1,actions.size());
  
  CHECK(actions.find(actasp::AspFluent("north(0)")) != actions.end());
  
  delete policy;

}

struct PlanEqual {
  
  PlanEqual(const actasp::AnswerSet& myplan) : myplan(myplan) {}
  
  bool operator()(const actasp::AnswerSet& otherplan ) const {
    
    if(myplan.getFluents().size() != otherplan.getFluents().size())
      return false;
    
    return std::equal(myplan.getFluents().begin(), myplan.getFluents().end(), otherplan.getFluents().begin(), actasp::ActionEquality());
  }
  
  const actasp::AnswerSet& myplan;
};
 

TEST_FIXTURE(ReasonerFixture, allPlansTest) {
  
  std::vector<actasp::AspRule> goalFormula;
    actasp::AspRule goalRule;
  goalRule.body.push_back(actasp::AspFluent("not pos(2,4,n)"));
  goalFormula.push_back(goalRule);
  
  std::vector<actasp::AnswerSet> allPlans = reasoner->computeAllPlans(goalFormula,1.5);
  
  CHECK_EQUAL(8,allPlans.size());

  std::set<actasp::AspFluent> plan1;
  plan1.insert(actasp::AspFluent("north(1)"));
  plan1.insert(actasp::AspFluent("north(2)"));
  plan1.insert(actasp::AspFluent("north(3)"));
  plan1.insert(actasp::AspFluent("north(4)"));
  
  CHECK(find_if(allPlans.begin(),allPlans.end(),PlanEqual(actasp::AnswerSet(plan1.begin(), plan1.end()))) != allPlans.end());
  
   std::set<actasp::AspFluent> plan2;
  plan2.insert(actasp::AspFluent("north(1)"));
  plan2.insert(actasp::AspFluent("north(2)"));
  plan2.insert(actasp::AspFluent("east(3)"));
  plan2.insert(actasp::AspFluent("north(4)"));
  plan2.insert(actasp::AspFluent("west(5)"));
  plan2.insert(actasp::AspFluent("north(6)"));
  
  CHECK(find_if(allPlans.begin(),allPlans.end(),PlanEqual(actasp::AnswerSet(plan2.begin(), plan2.end()))) != allPlans.end());
  
  std::set<actasp::AspFluent> plan3;
  plan3.insert(actasp::AspFluent("north(1)"));
  plan3.insert(actasp::AspFluent("east(2)"));
  plan3.insert(actasp::AspFluent("north(3)"));
  plan3.insert(actasp::AspFluent("west(4)"));
  plan3.insert(actasp::AspFluent("north(5)"));
  plan3.insert(actasp::AspFluent("north(6)"));
  
  CHECK(find_if(allPlans.begin(),allPlans.end(),PlanEqual(actasp::AnswerSet(plan3.begin(), plan3.end()))) != allPlans.end());
  
  std::set<actasp::AspFluent> plan4;
  plan4.insert(actasp::AspFluent("east(1)"));
  plan4.insert(actasp::AspFluent("north(2)"));
  plan4.insert(actasp::AspFluent("north(3)"));
  plan4.insert(actasp::AspFluent("west(4)"));
  plan4.insert(actasp::AspFluent("north(5)"));
  plan4.insert(actasp::AspFluent("north(6)"));
  
  CHECK(find_if(allPlans.begin(),allPlans.end(),PlanEqual(actasp::AnswerSet(plan4.begin(), plan4.end()))) != allPlans.end());
  
  std::set<actasp::AspFluent> plan5;
  plan5.insert(actasp::AspFluent("north(1)"));
  plan5.insert(actasp::AspFluent("east(2)"));
  plan5.insert(actasp::AspFluent("north(3)"));
  plan5.insert(actasp::AspFluent("north(4)"));
  plan5.insert(actasp::AspFluent("west(5)"));
  plan5.insert(actasp::AspFluent("north(6)"));
  
  CHECK(find_if(allPlans.begin(),allPlans.end(),PlanEqual(actasp::AnswerSet(plan5.begin(), plan5.end()))) != allPlans.end());
  
  std::set<actasp::AspFluent> plan6;
  plan6.insert(actasp::AspFluent("east(1)"));
  plan6.insert(actasp::AspFluent("north(2)"));
  plan6.insert(actasp::AspFluent("north(3)"));
  plan6.insert(actasp::AspFluent("north(4)"));
  plan6.insert(actasp::AspFluent("west(5)"));
  plan6.insert(actasp::AspFluent("north(6)"));
  
  CHECK(find_if(allPlans.begin(),allPlans.end(),PlanEqual(actasp::AnswerSet(plan6.begin(), plan6.end()))) != allPlans.end());
  
  std::set<actasp::AspFluent> plan7;
  plan7.insert(actasp::AspFluent("west(1)"));
  plan7.insert(actasp::AspFluent("north(2)"));
  plan7.insert(actasp::AspFluent("north(3)"));
  plan7.insert(actasp::AspFluent("north(4)"));
  plan7.insert(actasp::AspFluent("north(5)"));
  plan7.insert(actasp::AspFluent("east(6)"));
  
  CHECK(find_if(allPlans.begin(),allPlans.end(),PlanEqual(actasp::AnswerSet(plan7.begin(), plan7.end()))) != allPlans.end()); 


  std::set<actasp::AspFluent> plan8;
  plan8.insert(actasp::AspFluent("east(1)"));
  plan8.insert(actasp::AspFluent("north(2)"));
  plan8.insert(actasp::AspFluent("west(3)"));
  plan8.insert(actasp::AspFluent("north(4)"));
  plan8.insert(actasp::AspFluent("north(5)"));
  plan8.insert(actasp::AspFluent("north(6)"));
  
  CHECK(find_if(allPlans.begin(),allPlans.end(),PlanEqual(actasp::AnswerSet(plan8.begin(), plan8.end()))) != allPlans.end()); 
  
  
  // checking longer plans
  
  allPlans = reasoner->computeAllPlans(goalFormula,2);
  std::set<actasp::AspFluent> plan9;
  plan9.insert(actasp::AspFluent("east(1)"));
  plan9.insert(actasp::AspFluent("west(2)"));
  plan9.insert(actasp::AspFluent("north(3)"));
  plan9.insert(actasp::AspFluent("north(4)"));
  plan9.insert(actasp::AspFluent("north(5)"));
  plan9.insert(actasp::AspFluent("east(6)"));
  plan9.insert(actasp::AspFluent("west(7)"));
  plan9.insert(actasp::AspFluent("north(8)"));
  
  CHECK(find_if(allPlans.begin(),allPlans.end(),PlanEqual(actasp::AnswerSet(plan9.begin(), plan9.end()))) == allPlans.end()); 

}

TEST_FIXTURE(ReasonerFixture, availableActionsTest) {
  
  //from 0,0 the only actions available must be north and east
  
  std::vector<actasp::AspFluent> state;
  state.push_back(actasp::AspFluent("pos(0,0,0)"));
  reasoner->updateFluents(state);
    
  actasp::ActionSet desired;
  desired.insert(actasp::AspFluent("north(1)"));
  desired.insert(actasp::AspFluent("east(1)"));
  
  actasp::ActionSet computed = reasoner->availableActions();
  
  actasp::ActionSet sym_diff;
  std::set_symmetric_difference(desired.begin(),desired.end(),computed.begin(),computed.end(),std::inserter(sym_diff,sym_diff.begin()));
  
  CHECK(sym_diff.empty());
  
  //from 2,0 the only actions available must be north, east, and west
  
  state.clear();
  state.push_back(actasp::AspFluent("pos(2,0,0)"));
  reasoner->updateFluents(state);
  
  desired.insert(actasp::AspFluent("west(1)"));
   
  computed = reasoner->availableActions();
  
  sym_diff.clear();
  std::set_symmetric_difference(desired.begin(),desired.end(),computed.begin(),computed.end(),std::inserter(sym_diff,sym_diff.begin()));
  
  CHECK(sym_diff.empty());
  
    //from 3,1 (not along the wall) all actions must be available
  
  state.clear();
  state.push_back(actasp::AspFluent("pos(3,1,0)"));
  reasoner->updateFluents(state);
    
  desired.insert(actasp::AspFluent("south(1)"));
  
  computed = reasoner->availableActions();

  
  sym_diff.clear();
  std::set_symmetric_difference(desired.begin(),desired.end(),computed.begin(),computed.end(),std::inserter(sym_diff,sym_diff.begin()));
  
  CHECK(sym_diff.empty());
  
}*/