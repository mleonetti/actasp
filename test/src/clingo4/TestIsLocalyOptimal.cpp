
#include "unittest++/UnitTest++.h"

#include "../../../src/reasoners/IsNotLocallyOptimal.h"

#include <actasp/AspFluent.h>
#include <actasp/AnswerSet.h>

#include <iostream>

using namespace std;
using namespace actasp;


struct IsLocalyOptimalFixture {
  IsLocalyOptimalFixture() : actions(), goodPlans() {
    
    actions.insert(AspFluent("act1(0)"));
    actions.insert(AspFluent("act2(0)"));
    actions.insert(AspFluent("act3(0)"));



    list<AspFluent> plan1;
    plan1.push_back(AspFluent("act1(0)"));
    plan1.push_back(AspFluent("act1(1)"));

    list<AspFluent> plan2;
    plan2.push_back(AspFluent("act1(0)"));
    plan2.push_back(AspFluent("act2(1)"));
    plan2.push_back(AspFluent("act2(2)"));

    list<AspFluent> plan3;
    plan3.push_back(AspFluent("act1(0)"));
    plan3.push_back(AspFluent("act2(1)"));
    plan3.push_back(AspFluent("act3(2)"));

    goodPlans.insert(plan1);
    goodPlans.insert(plan2);
    goodPlans.insert(plan3);

  }
  
  ActionSet actions;
  set< list<AspFluent> > goodPlans;
    
};

TEST_FIXTURE(IsLocalyOptimalFixture, IsLocallyOptimal_cleanplan) {
  
  set<AspFluent> set1;
  set1.insert(AspFluent("act1(0)"));
  set1.insert(AspFluent("b(0)"));
  set1.insert(AspFluent("act2(1)"));
  set1.insert(AspFluent("c(1)"));
  
  AnswerSet aset1(set1.begin(), set1.end());
  
  list<AspFluent> plan;
  plan.push_back(AspFluent("act1(0)"));
  plan.push_back(AspFluent("act2(1)"));
  
  set< list<AspFluentRef>, LexComparator > emptySet;
  set< list<AspFluentRef>, LexComparator > badSet;
  IsNotLocallyOptimal tester(&emptySet,&badSet, actions,0,false);
  CHECK(equal(plan.begin(), plan.end(),tester.cleanPlan(aset1).begin()));
  
  set<AspFluent> set2;
  set2.insert(AspFluent("act1(0)"));
  set2.insert(AspFluent("act2(1)"));
  set2.insert(AspFluent("c(1)"));
  
  AnswerSet aset2(set2.begin(), set2.end());
  CHECK(equal(plan.begin(), plan.end(),tester.cleanPlan(aset2).begin()));
  
  set<AspFluent> set3;
  set3.insert(AspFluent("act1(0)"));
  set3.insert(AspFluent("act2(1)"));
  
  AnswerSet aset3(set3.begin(), set3.end());
  CHECK(equal(plan.begin(), plan.end(),tester.cleanPlan(aset3).begin()));
}


TEST_FIXTURE(IsLocalyOptimalFixture, IsLocalyOptimal_findFirstSuspiciousAction) {
  

  set< list<AspFluentRef>, LexComparator > badSet;
  
    set<list<AspFluentRef>, LexComparator > goodCopy;
  set<list<AspFluent>, LexComparator >::const_iterator gIt = goodPlans.begin();
  for(; gIt != goodPlans.end(); ++gIt)
    goodCopy.insert(list<AspFluentRef>(gIt->begin(), gIt->end()));
  
  IsNotLocallyOptimal tester(&goodCopy, &badSet, actions,2,true);
  
  list<AspFluent> bad1;
  bad1.push_back(AspFluent("act1(0)"));
  bad1.push_back(AspFluent("act2(1)"));
  bad1.push_back(AspFluent("act1(2)"));
  bad1.push_back(AspFluent("act2(3)"));
  
  list<AspFluentRef> badc(bad1.begin(),bad1.end());
  
  
  list<AspFluentRef>::const_iterator suspect = tester.findFirstSuspiciousAction(badc);
  
  CHECK(suspect == (++(++(badc.begin()))));
  
  list<AspFluent> bad2;
  bad2.push_back(AspFluent("act1(0)"));
  bad2.push_back(AspFluent("act3(1)"));
  bad2.push_back(AspFluent("act2(2)"));
  bad2.push_back(AspFluent("act3(3)"));
  
  badc.clear();
  badc.insert(badc.end(),bad2.begin(),bad2.end());
  
  
  suspect = tester.findFirstSuspiciousAction(badc);
  
  CHECK(suspect == (++(badc.begin())));  
  
    list<AspFluent> bad3;
  bad3.push_back(AspFluent("act2(0)"));
  bad3.push_back(AspFluent("act3(1)"));
  bad3.push_back(AspFluent("act2(2)"));
  bad3.push_back(AspFluent("act3(3)"));
  
    badc.clear();
  badc.insert(badc.end(),bad3.begin(),bad3.end());
  
  
  suspect = tester.findFirstSuspiciousAction(badc);
  
  CHECK(suspect == (badc.begin()));
  
}

TEST_FIXTURE(IsLocalyOptimalFixture, IsLocalyOptimal_operator) {

    set<AspFluent> good1;
  good1.insert(AspFluent("act1(0)"));
  good1.insert(AspFluent("act3(1)"));
  good1.insert(AspFluent("act3(2)"));
  good1.insert(AspFluent("act2(3)"));
  
  AnswerSet agood1(good1.begin(), good1.end());
  
  set< list<AspFluentRef>, LexComparator > badSet;
  
  set<list<AspFluentRef>, LexComparator > goodCopy;
  set<list<AspFluent>, LexComparator >::const_iterator gIt = goodPlans.begin();
  for(; gIt != goodPlans.end(); ++gIt)
    goodCopy.insert(list<AspFluentRef>(gIt->begin(), gIt->end()));
  
  
  IsNotLocallyOptimal tester(&goodCopy, &badSet, actions,2,true);
  
  CHECK(!tester(agood1));
  
  set<AspFluent> bad1;
  bad1.insert(AspFluent("act1(0)"));
  bad1.insert(AspFluent("act2(1)"));
  bad1.insert(AspFluent("act3(2)"));
  bad1.insert(AspFluent("act2(3)"));
  
  AnswerSet abad1(bad1.begin(), bad1.end());
  
  CHECK(tester(abad1));
  
  set<AspFluent> bad2;
  bad2.insert(AspFluent("act2(0)"));
  bad2.insert(AspFluent("act2(1)"));
  bad2.insert(AspFluent("act1(2)"));
  bad2.insert(AspFluent("act1(3)"));
  
  AnswerSet abad2(bad2.begin(), bad2.end());
  
  CHECK(tester(abad2));
  
  
}

TEST_FIXTURE(IsLocalyOptimalFixture, IsLocalyOptimal_hasLoop) {
  
  IsNotLocallyOptimal::PlanSet good,bad;
  
  IsNotLocallyOptimal isNotLocallyOptimal(&good,&bad,actions,0,false);
  
  set<AspFluent> plan1;
  plan1.insert(AspFluent("A(0)"));
  plan1.insert(AspFluent("act1(0)"));
  plan1.insert(AspFluent("B(1)"));
  plan1.insert(AspFluent("act2(1)"));
  plan1.insert(AspFluent("A(2)"));
  plan1.insert(AspFluent("act2(2)"));
  
  AnswerSet a1(plan1.begin(), plan1.end());
  
  CHECK(isNotLocallyOptimal(a1));
  
    set<AspFluent> plan2;
  plan2.insert(AspFluent("A(0)"));
  plan2.insert(AspFluent("act1(0)"));
  plan2.insert(AspFluent("B(1)"));
  plan2.insert(AspFluent("act2(1)"));
  plan2.insert(AspFluent("C(2)"));
  plan2.insert(AspFluent("act2(2)"));
  
  AnswerSet a2(plan2.begin(), plan2.end());
  
  CHECK(!isNotLocallyOptimal(a2));
  
}