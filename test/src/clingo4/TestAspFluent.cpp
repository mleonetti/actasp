
#include "unittest++/UnitTest++.h"

#include <actasp/AspFluent.h>

using namespace std;
using namespace actasp;

TEST(AspFluentConstructor) {
	
	CHECK_THROW(AspFluent("Fluent(a,b,c"), invalid_argument);
	
	CHECK_THROW(AspFluent("Fluenta,b,c)"), invalid_argument);
	
	CHECK_THROW(AspFluent("Fluent"), invalid_argument);
	
	CHECK_THROW(AspFluent(""), invalid_argument);
	
}

TEST(AspFluentGetParameters) {
  
  AspFluent a("A(1,a,0)");
  
  vector<string> apra = a.getParameters();
  
  CHECK_EQUAL(2,apra.size());
  
  CHECK(apra[0] == "1" && apra[1] == "a");
  
  AspFluent b("B(1)");
  
  
  CHECK_EQUAL(0, b.getParameters().size()); 
}

TEST(AspFluentGetName) {
  
  AspFluent a("A(1,a,0)");
  
  CHECK_EQUAL("A",a.getName());
    
  AspFluent b("foo(1)");
  
  CHECK_EQUAL("foo",b.getName()); 
}

TEST(AspFluentComparison) {
	
	CHECK(AspFluent("a(a,b,0)") < AspFluent("b(a,b,0)"));
	
	CHECK(AspFluent("a(a,b,0)") < AspFluent("a(a,b,1)"));
	
	CHECK(!(AspFluent("a(a,b,1)") < AspFluent("a(a,b,0)")));
	
	CHECK(!(AspFluent("a(a,b,0)") < AspFluent("a(a,b,0)")));
	
	CHECK(!(AspFluent("b(a,b,0)") < AspFluent("a(a,b,0)")));
	
	CHECK(AspFluent("-a(a,b,0)") < AspFluent("a(a,b,0)"));
	
}