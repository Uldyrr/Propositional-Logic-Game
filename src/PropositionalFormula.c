#include "PropositionalFormula.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// PropositionalFormula helper functions
void PropositionalFormulaPrint(PropositionalFormula* propositionalFormula) {
	PredicatePrint(propositionalFormula->Root);
}

void PropositionalFormulaPrintEvaluation(PropositionalFormula* propositionalFormula) {
	printf(GetBooleanChar(PropositionalFormulaEvaluate(propositionalFormula)));
}


// PropositionalFormula global functions
PropositionalFormula CreatePropositionalFormula(int countPredicates) {
	PropositionalFormula propositionalFormula = (PropositionalFormula){ .CountPredicates = countPredicates, };

	propositionalFormula.Predicates = (LogicalPredicate*)malloc(countPredicates * sizeof(LogicalPredicate));

	return propositionalFormula;
}


PropositionalFormula GeneratePropositionalFormula(int countVariables, LogicalPredicate* variablePredicates) { 
	int countPredicates = countVariables - 1;  // Note: We assume that the count of variables is >= 2
	int countFormulaSymbols = 2 * countVariables - 1;
	int* propositionalFormulaSymbols = (int*)malloc(countFormulaSymbols * sizeof(int));
	PropositionalFormula propositionalFormula = CreatePropositionalFormula(countPredicates);

	if (propositionalFormulaSymbols == NULL) {
		printf("Unable to allocate %llu bytes for propositional formula symbols\n", countFormulaSymbols * sizeof(int)); 
		return;
	}

	// Propositional formula generation
	// 1. Seed the pseudo RNG
	srand(time(NULL));

	// 2. Generate propositional formula symbols
	for (int i = 0; i < countFormulaSymbols; i++) {
		if (i % 2 == 0)
			propositionalFormulaSymbols[i] = rand() % countVariables;
		else
			propositionalFormulaSymbols[i] = PROPOSITONALFORMULA_GENERATION_CONNECTIVEOFFSET + Conjuntion + rand() % (Equivalence - Conjuntion + 1);

		//printf("%i ", propositionalFormulaSymbols[i]);
	}

	// 3. Cache count of unique variables
	for (int variableId = 0; variableId < countVariables; variableId++) {
		for (int i = 0; i < countFormulaSymbols; i++) {
			if (propositionalFormulaSymbols[i] == variableId) {
				propositionalFormula.CountVariables++;
				break;
			}
		}
	}

	//printf("\n");

	// 4. Process connectives until a singular root predicate has been made
	_Bool l2r;
	int iOffset, correctedI, isConnective, lOperandSIndex, rOperandSIndex;
	int countOfPredicates = 0;

	for (int connectiveType = Conjuntion; connectiveType <= Equivalence; connectiveType++) {
		l2r = IsConnectiveAssociativityL2R(connectiveType);
		iOffset = 0;
		
		//printf("\n");

		for (int i = 1; i < countFormulaSymbols; i += 2) {
			correctedI = l2r ? i + iOffset : countFormulaSymbols - 1 - i;
			isConnective = propositionalFormulaSymbols[correctedI] - PROPOSITONALFORMULA_GENERATION_CONNECTIVEOFFSET == connectiveType;

			if (isConnective == false) continue;

			iOffset -= 2;

			// -- Create new logical predicate at index correctedI - 1
			lOperandSIndex = propositionalFormulaSymbols[correctedI - 1];
			rOperandSIndex = propositionalFormulaSymbols[correctedI + 1];

			LogicalPredicate* lOperand = lOperandSIndex < PROPOSITONALFORMULA_GENERATION_PREDICATEEOFFSET ? &variablePredicates[lOperandSIndex] : &propositionalFormula.Predicates[lOperandSIndex - PROPOSITONALFORMULA_GENERATION_PREDICATEEOFFSET];
			LogicalPredicate* rOperand = rOperandSIndex < PROPOSITONALFORMULA_GENERATION_PREDICATEEOFFSET ? &variablePredicates[rOperandSIndex] : &propositionalFormula.Predicates[rOperandSIndex - PROPOSITONALFORMULA_GENERATION_PREDICATEEOFFSET];

			propositionalFormula.Predicates[countOfPredicates] = CreateLogicalPredicate(connectiveType, lOperand, rOperand);

			propositionalFormulaSymbols[correctedI - 1] = PROPOSITONALFORMULA_GENERATION_PREDICATEEOFFSET + countOfPredicates++;
			
			for (int j = correctedI; j < countFormulaSymbols - 2; j += 2) {
				propositionalFormulaSymbols[j] = propositionalFormulaSymbols[j + 2];
				propositionalFormulaSymbols[j + 1] = propositionalFormulaSymbols[j + 3];

				propositionalFormulaSymbols[j + 2] = -1;
				propositionalFormulaSymbols[j + 3] = -1;
			}
		}

		/*
		for (int i = 0; i < countFormulaSymbols; i++) {
			printf("%i ", propositionalFormulaSymbols[i]);
		}

		printf("\n");
		*/
	}

	propositionalFormula.Root = &propositionalFormula.Predicates[countOfPredicates - 1];

	//printf("\n");

	return propositionalFormula;
}


_Bool PropositionalFormulaEvaluate(PropositionalFormula* propositionalFormula) {
	return propositionalFormula->Root->Predicate(propositionalFormula->Root);
}
