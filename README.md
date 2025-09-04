# Propositional-Logic-Game
A 2-player game with fixed size randomized propositional formulas which follows precedence and associativity.

Player 1 always starts first and has the end goal of making the propositional formula evaluate to true (T).<br/>
Player 2 starts second and has the end goal of making the evaluation become false (F).

Every player starts their turn by inputting either "y" or "n" depending on if they want to set a variable to either true or false.<br/>
They must follow up their decision by inputting the character of the propositional variable they want to update (This "locks" the variable to that value).<br/>
All of the propositional variables start as false, and can be either locked to false or true. Your turn can also be skipped if there is invalid input. <br/>

Refer to the comments in "main.c" when building the program.
