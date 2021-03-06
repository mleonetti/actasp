#include <iclingo>.
#program cumulative(n).

#const max_y = 5.
#const max_x = 5.


%%%%% Dynamics
 
%generate exactly one action, unless noop is set. This is used for planning.
1{north(I);east(I);west(I);south(I)}1 :- not noop(I), I>0, I=n-1.

%The planner cannot use noop, it must be manually set.
0{noop(I)}0 :- not noop(I), I>0, I=n-1.

%if noop is manually set, no other action can be taken. This is used when updating the current state through sensing.
0{north(I);east(I);west(I);south(I)}0 :- noop(I), I>0, I=n-1.



% north causes pos(X,Y+1) if pos(X,Y).
% nonexecutable north if pos(X,max_y).
% nonexecutable north if pos(X,Y,I), obst(X,Y,X,Y+1,I).

pos(X,Y+1,I) :- north(I), pos(X,Y,I-1), I>0, I=n-1.
:- north(I), pos(X,max_y,I-1).
:- north(I), pos(X,Y,I-1), obst(X,Y,X,Y+1,I-1).

pos(X+1,Y,I) :- east(I), pos(X,Y,I-1), I>0, I=n-1.
:- east(I), pos(max_x,Y,I-1).
:- east(I), pos(X,Y,I-1), obst(X,Y,X+1,Y,I-1).


pos(X-1,Y,I) :- west(I), pos(X,Y,I-1), I>0, I=n-1.
:- west(I), pos(0,0..max_y,I-1).
:- west(I), pos(X,Y,I-1), obst(X,Y,X-1,Y,I-1).

pos(X,Y-1,I) :- south(I),pos(X,Y,I-1), I>0, I=n-1.
:- south(I), pos(0..max_x,0,I-1).
:- south(I), pos(X,Y,I-1), obst(X,Y,X,Y-1,I-1).



%%%%% System

%%if you are somewhere, you are not anywyhere else
-pos(X,Y,I) :- pos(Z,K,I), Z != X, X=0..max_x, Y=0..max_y, I>=0, I=n-1.
-pos(X,Y,I) :- pos(Z,K,I), K != Y, X=0..max_x, Y=0..max_y, I>=0, I=n-1.

%% pos is inertial
pos(X,Y,I) :- pos(X,Y,I-1), not -pos(X,Y,I), I>0, I=n-1.


obst(X2,Y2,X1,Y1,I) :- obst(X1,Y1,X2,Y2,I).

%obstacles that are always there
obst(1,Y,2,Y,I) :- Y=1..3, I>=0, I=n-1.
obst(X,3,X,4,I) :- X=3..4, I>=0, I=n-1.

%if at some point you know about an obstacle, it doesn't go away
obst(X,Y,Z,K,I) :- obst(X,Y,Z,K,I-1), not -obst(X,Y,Z,K,I),  I>0, I=n-1.


%useless rule to suppress clingo's warning about -obst not beind defined.
%-obst will be set as an observation in the current state if necessary
-obst(0,0,0,1,I) :- I>=0, I=n-1.

%remember to hide non-fluents

%-pos is implied by pos, so it's useless
#show pos/3.
%#show obst/5.

#show north/1.
#show south/1.
#show east/1.
#show west/1.