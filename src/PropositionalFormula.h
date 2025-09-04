#ifndef _PROPOSITIONALFORMULA_H

#include "LogicalPredicate.h"

#define PROPOSITONALFORMULA_GENERATION_CONNECTIVEOFFSET 100
#define PROPOSITONALFORMULA_GENERATION_PREDICATEEOFFSET 1000


/// <summary>
/// Stores the predicates contained within a propositional formula and provides the topmost root predicate.
/// </summary>
typedef struct PropositionalFormula {
	LogicalPredicate* Predicates;
	LogicalPredicate* Root;
	int CountPredicates;
	int CountVariables;
} PropositionalFormula;


// Propositional formula helper functions
void PropositionalFormulaPrint(PropositionalFormula* propositionalFormula);
void PropositionalFormulaPrintEvaluation(PropositionalFormula* propositionalFormula);


// Propositional formula global functions
PropositionalFormula CreatePropositionalFormula(int countPredicates);
PropositionalFormula GeneratePropositionalFormula(int countVariables, LogicalPredicate* variablePredicates);
_Bool PropositionalFormulaEvaluate(PropositionalFormula* propositionalFormula);

#endif _PROPOSITIONALFORMULA_H