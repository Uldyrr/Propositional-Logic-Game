#include "src/PropositionalFormula.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

//#define MACOS  // Define this macro if you need system("clear") instead of system("cls")

/*
	Before building this program, note this:

	- Enter as many boolean values as you'd like in "PropositionalVariables" (they are always reset to false before a new game).

	- The program will associate any propositional variable at a given index in "PropositionalVariables" with the values 
	stored at the same index in "PropositionalVariablesChars" and "PropositionalVariablesLocked". 
	Give the i-th propositional variable a char value at the i-th index of "PropositionalVariablesChars"

	- You do not need to initialize "PropositionalVariablesLocked" as the program creates it at runtime.
*/


_Bool PropositionalVariables[] = { false, false, false, false, false };
char PropositionalVariablesChars[] = {'Q', 'P', 'R', 'S', 'T'};
_Bool* PropositionalVariablesLocked;
int CountPropositionalVariables = sizeof(PropositionalVariables);
LogicalPredicate* VariablePredicates;
PropositionalFormula GamePropositionalFormula;


_Bool* GetPropositionalVariable(char variable) {
	variable = toupper(variable);

	for (int i = 0; i < CountPropositionalVariables; i++) {
		if (PropositionalVariablesChars[i] == variable && PropositionalVariablesLocked[i] == false) {
			PropositionalVariablesLocked[i] = true;

			return &PropositionalVariables[i];
		}
	}

	return NULL;
}


void GetBoolInput(char* msg, _Bool* input) {
	char inputChar;
	printf("%s", msg);
	scanf_s("%c", &inputChar);
	fseek(stdin, 0, SEEK_END);

	*input = toupper(inputChar) == 'Y';
}


void GetCharInput(char* msg, char* input) {
	printf("%s", msg);
	scanf_s("%c", input);
	fseek(stdin, 0, SEEK_END);
}


void NewGame() {
	// Reset propositional variables and generate a new propositional formula
	for (int i = 0; i < CountPropositionalVariables; i++) {
		PropositionalVariables[i] = false;
		PropositionalVariablesLocked[i] = false;
	}

	GamePropositionalFormula = GeneratePropositionalFormula(CountPropositionalVariables, VariablePredicates);
}


void GameLoop() {
	_Bool inputBool;
	char inputVariable;
	_Bool* inputPropositionalVariable;
	int countInput = 0;
	
	while (true) {
		// 1. Renderer current formula and the values of propositional variables
#ifdef MACOS
		system("clear");
#else
		system("cls");
#endif 

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
			printf("%s %s", PropositionalVariablesLocked[i] ? GetBooleanChar(PropositionalVariables[i]) : " ", i + 1 < CountPropositionalVariables ? "| " : "|");
		}

		printf("\n\n");

		if (++countInput > GamePropositionalFormula.CountVariables) break;

		// 2. Get propositional variable input
		GetBoolInput("Set to true? (y/n): ", &inputBool);
		GetCharInput("Set variable to true: ", &inputVariable);

		if ((inputPropositionalVariable = GetPropositionalVariable(inputVariable)) != NULL) *inputPropositionalVariable = inputBool;
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
	PropositionalVariablesLocked = (_Bool*)malloc(CountPropositionalVariables * sizeof(_Bool));
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
