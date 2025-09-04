#include "LogicalPredicate.h"
#include <stdio.h>
#include <time.h>


/* Logical connectives and their c counterpart

   ¬   = !
   ^   = &&
   v   = ||
   ->  = (!x || y)
   <-> = x && y || !(x || y)
*/


// ConnectiveType helper functions
_Bool IsConnectiveAssociativityL2R(ConnectiveType cType) {
	switch (cType) {
	case Conjuntion:
		return true;
	case Disjunction:
		return true;
	default:
		return false;
	}
}


const char* GetConnectiveString(ConnectiveType cType) {
	switch (cType) {
	case Variable:
		return "";
	case Negation:
		return "!";
	case Conjuntion:
		return " ^ ";
	case Disjunction:
		return " v ";
	case Implication:
		return " -> ";
	case Equivalence:
		return " <-> ";
	}

	return "";
}


// LogicalPredicate helper functions (TODO: Implement negation)
void PredicatePrint(LogicalPredicate* predicate) {
	printf("(");

	if (predicate->LPredicate->Type < Conjuntion) printf("%s%c", GetConnectiveString(predicate->LPredicate->Type), predicate->LPredicate->VariableChar); else PredicatePrint(predicate->LPredicate);

	printf(GetConnectiveString(predicate->Type));

	if (predicate->RPredicate->Type < Conjuntion) printf("%s%c", GetConnectiveString(predicate->RPredicate->Type), predicate->RPredicate->VariableChar); else PredicatePrint(predicate->RPredicate);

	printf(")");
}

_Bool PredicateConjunction(LogicalPredicate* predicate) {
	_Bool leftOperand = predicate->LPredicate->Type == Variable ? *predicate->LPredicate->Variable : (predicate->LPredicate->Type == Negation ? !*predicate->LPredicate->Variable : predicate->LPredicate->Predicate(predicate->LPredicate));
	_Bool rightOperand = predicate->RPredicate->Type == Variable ? *predicate->RPredicate->Variable : (predicate->RPredicate->Type == Negation ? !*predicate->RPredicate->Variable : predicate->RPredicate->Predicate(predicate->RPredicate));

	return leftOperand && rightOperand;
}

_Bool PredicateDisjunction(LogicalPredicate* predicate) {
	_Bool leftOperand = predicate->LPredicate->Type == Variable ? *predicate->LPredicate->Variable : (predicate->LPredicate->Type == Negation ? !*predicate->LPredicate->Variable : predicate->LPredicate->Predicate(predicate->LPredicate));
	_Bool rightOperand = predicate->RPredicate->Type == Variable ? *predicate->RPredicate->Variable : (predicate->RPredicate->Type == Negation ? !*predicate->RPredicate->Variable : predicate->RPredicate->Predicate(predicate->RPredicate));

	return leftOperand || rightOperand;
}

_Bool PredicateImplication(LogicalPredicate* predicate) {
	_Bool leftOperand = predicate->LPredicate->Type == Variable ? *predicate->LPredicate->Variable : (predicate->LPredicate->Type == Negation ? !*predicate->LPredicate->Variable : predicate->LPredicate->Predicate(predicate->LPredicate));
	_Bool rightOperand = predicate->RPredicate->Type == Variable ? *predicate->RPredicate->Variable : (predicate->RPredicate->Type == Negation ? !*predicate->RPredicate->Variable : predicate->RPredicate->Predicate(predicate->RPredicate));

	return !leftOperand || rightOperand;
}

_Bool PredicateEquivalence(LogicalPredicate* predicate) {
	_Bool leftOperand = predicate->LPredicate->Type == Variable ? *predicate->LPredicate->Variable : (predicate->LPredicate->Type == Negation ? !*predicate->LPredicate->Variable : predicate->LPredicate->Predicate(predicate->LPredicate));
	_Bool rightOperand = predicate->RPredicate->Type == Variable ? *predicate->RPredicate->Variable : (predicate->RPredicate->Type == Negation ? !*predicate->RPredicate->Variable : predicate->RPredicate->Predicate(predicate->RPredicate));

	return leftOperand && rightOperand || !(leftOperand || rightOperand);
}


// LogicalPredicate helper functions
LogicalPredicate* GenerateVariablePredicates(int countVariables, _Bool* variables, char* variableChars) {
	LogicalPredicate* variablePredicates = (LogicalPredicate*)malloc(countVariables * sizeof(LogicalPredicate));

	if (variablePredicates == NULL) {
		printf("Unable to allocate %llu bytes for variable predicates\n", countVariables * sizeof(LogicalPredicate));
		return;
	}

	// Variable predicates generation
	// 1. Seed the pseudo RNG
	srand(time(NULL));

	// 2. Generate predicates with or without negation
	for (int i = 0; i < countVariables; i++) {
		variablePredicates[i] = CreateVariablePredicate(&variables[i], variableChars[i], rand() % 2);
	}

	return variablePredicates;
}

const char* GetBooleanChar(_Bool boolean) {
	return boolean ? "T" : "F";
}


// LogicalPredicate global functions
LogicalPredicate CreateVariablePredicate(_Bool* variable, char variableChar, _Bool negated) {
	return (LogicalPredicate) {
		.Type = negated ? Negation : Variable,
		.Variable = variable,
		.VariableChar = variableChar
	};
}


LogicalPredicate CreateLogicalPredicate(ConnectiveType connectiveType, LogicalPredicate* lPredicate, LogicalPredicate* rPredicate) {
	LogicalPredicate logicalPredicate = (LogicalPredicate) {
		.Type = connectiveType,
		.LPredicate = lPredicate,
		.RPredicate = rPredicate,
	};

	switch (connectiveType) {
	case Conjuntion:
		logicalPredicate.Predicate = &PredicateConjunction;
		break;
	case Disjunction:
		logicalPredicate.Predicate = &PredicateDisjunction;
		break;
	case Implication:
		logicalPredicate.Predicate = &PredicateImplication;
		break;
	case Equivalence:
		logicalPredicate.Predicate = &PredicateEquivalence;
		break;
	}
	
	return logicalPredicate;
}