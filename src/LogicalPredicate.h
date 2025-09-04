#ifndef _LOGICALPREDICATE_H

#include <stdbool.h>
#include <stdlib.h>


// Enum ConnectiveType
/// <summary>
/// The logical connectives. The lower the enum value, the higher the precedence
/// </summary>
typedef enum ConnectiveType {
	Variable = 0,
	Negation,
	Conjuntion,
	Disjunction,
	Implication,
	Equivalence
} ConnectiveType;


// ConnectiveType helper functions
_Bool IsConnectiveAssociativityL2R(ConnectiveType cType);
const char* GetConnectiveString(ConnectiveType cType);


// Struct LogicalPredicate
/// <summary>
/// High-level data structure used to create a branching tree of predicates to evaluate
/// </summary>
typedef struct LogicalPredicate LogicalPredicate;

struct LogicalPredicate {
	ConnectiveType Type;
	_Bool* Variable; 
	char VariableChar;
	LogicalPredicate* LPredicate;
	LogicalPredicate* RPredicate;
	_Bool (*Predicate)(LogicalPredicate*);
};


// LogicalPredicate helper functions
_Bool PredicateConjunction(LogicalPredicate* predicate);
_Bool PredicateDisjunction(LogicalPredicate* predicate);
_Bool PredicateImplication(LogicalPredicate* predicate);
_Bool PredicateEquivalence(LogicalPredicate* predicate);
void PredicatePrint(LogicalPredicate* predicate);
const char* GetBooleanChar(_Bool boolean);

LogicalPredicate* GenerateVariablePredicates(int countVariables, _Bool* variables, char* variableChars);


// LogicalPredicate global functions
LogicalPredicate CreateVariablePredicate(_Bool* variable, char variableChar, _Bool negated);
LogicalPredicate CreateLogicalPredicate(ConnectiveType connectType, LogicalPredicate* lPredicate, LogicalPredicate* rPredicate);


#endif _LOGICALPREDICATE_H