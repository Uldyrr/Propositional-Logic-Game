#include "src/PropositionalFormula.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>


char PropositionalVariablesChars[] = { 'Q',   'P',   'R',   'S', 'T', 'A', 'N', 'M'};
_Bool PropositionalVariables[] =     { false, false, false, false, false, false, false, false };
int CountPropositionalVariables = sizeof(PropositionalVariables);
LogicalPredicate* VariablePredicates;
PropositionalFormula GamePropositionalFormula;


_Bool* GetPropositionalVariable(char variable) {
	variable = toupper(variable);

	for (int i = 0; i < CountPropositionalVariables; i++) {
		if (PropositionalVariablesChars[i] == variable) return &PropositionalVariables[i];
	}

	return NULL;
}


void GetCharInput(char* msg, char* input) {
	printf("%s", msg);
	scanf_s("%c", input);
	fseek(stdin, 0, SEEK_END);
}


void NewGame() {
	// 1. Reset propositional variables and generate a new propositional formula
	for (int i = 0; i < CountPropositionalVariables; i++) {
		PropositionalVariables[i] = false;
	}

	GamePropositionalFormula = GeneratePropositionalFormula(CountPropositionalVariables, VariablePredicates);
}


void GameLoop() {
	char inputVariable;
	_Bool* inputPropositionalVariable;
	int countInput = 0;
	
	while (true) {
		// 2. Renderer current formula and the values of propositional variables
		system("cls");

		printf(countInput % 2 == 0 ? "Player 1's turn!" : "Player 2's turn!");

		printf("\n\n");

		PropositionalFormulaPrint(&GamePropositionalFormula);

		printf("\n\n| ");

		for (int i = 0; i < CountPropositionalVariables; i++) {
			printf("%c %s", PropositionalVariablesChars[i], i + 1 < CountPropositionalVariables ? "| " : "|\n|-");
		}

		for (int i = 0; i < CountPropositionalVariables; i++) {
			printf(i + 1 < CountPropositionalVariables ? "--|-" : "--|\n| ");
		}

		for (int i = 0; i < CountPropositionalVariables; i++) {
			printf("%s %s", GetBooleanChar(PropositionalVariables[i]), i + 1 < CountPropositionalVariables ? "| " : "|");
		}

		printf("\n\n");

		if (++countInput > CountPropositionalVariables) break;

		// 3. Get propositional variable input (If you don't write out the actual character, it is assumed you want one of the variables set to false)
		GetCharInput("Set variable to true: ", &inputVariable);

		if ((inputPropositionalVariable = GetPropositionalVariable(inputVariable)) != NULL) *inputPropositionalVariable = true;
	}

	PropositionalFormulaPrint(&GamePropositionalFormula);
	printf(" = ");
	PropositionalFormulaPrintEvaluation(&GamePropositionalFormula);

	printf("\n\n");

	printf(PropositionalFormulaEvaluate(&GamePropositionalFormula) ? "Player 1 Won!" : "Player 2 Won!");

	printf("\n\n");

	GetCharInput("Press enter to start a new game!", &inputVariable);
}


void Init() {
	VariablePredicates = GenerateVariablePredicates(CountPropositionalVariables, PropositionalVariables, PropositionalVariablesChars);
}


int main() {
	Init();

	while (true) {
		NewGame();
		GameLoop();
	}

	return 0;
}