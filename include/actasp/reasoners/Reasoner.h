
#ifndef actasp_Reasoner_h__guard
#define actasp_Reasoner_h__guard

#include <actasp/AspKR.h>

#include <actasp/AspFluent.h>

namespace actasp {

class QueryGenerator;
  
struct Reasoner : public AspKR {
  
  Reasoner(QueryGenerator *actualReasoner,unsigned int max_n, const ActionSet& allActions);
  
  ActionSet availableActions() const throw();
  
  AnswerSet currentStateQuery(const std::vector<actasp::AspRule>& query) const throw();
  
  bool updateFluents(const std::vector<actasp::AspFluent> &observations) throw();
  
  bool isPlanValid(const AnswerSet& plan, const std::vector<actasp::AspRule>& goal)  const throw();
  
  void resetCurrentState() throw();
  
  AnswerSet computePlan(const std::vector<actasp::AspRule>& goal) const throw ();
  
  std::vector< AnswerSet > computeAllPlans(const std::vector<actasp::AspRule>& goal, double suboptimality) const throw (std::logic_error);
  
  PartialPolicy computePolicy(const std::vector<actasp::AspRule>& goal, double suboptimality) const throw (std::logic_error);
  
  std::list< std::list<AspAtom> > query(const std::string &queryString, unsigned int timestep) const throw();
  
  void setMaxTimeStep(unsigned int max_n) throw() {
    this->max_n = max_n;
  }
  
  virtual ~Reasoner(){}

private:
  QueryGenerator *clingo;
  unsigned int max_n;
  ActionSet allActions;
};
  
}

#endif