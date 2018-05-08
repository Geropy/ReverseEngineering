#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// 
// The first two inputs seem to be the size of the grid
// The third input seems to be the number of entities in the grid

// The 4 string inputs on each turn are either # or _
// This seems to indicate what is in the 4 moveable directions
// Then I'm given 2 integers for each entity, likely their position in the grid, since the numbers don't seem to exceed the grid size
// Then I'm given the choice of 5 outputs, which are movements in each direction or doing nothing

// B is doing nothing, since I always end with 2 when only doing B 

// The coordinates of the entities never change by more than 1 per turn, it seems like these things move 1 space per turn
// When outputting A, the string inputs change very turn until there is a # as the second input, then it never changes
// It feels as though I've hit a wall, and direction A corresponds to the second input
// THe fourht input is the only ones that's _ the whole time, it would make sense if this was the opposite direction of A

// A is second
// B is wait
// C is first
// D is third
// E is fourth

// Therefore, an initial plan is to move in the direction of a _ and not a #

// What do the entities have to do with it? My thought is that the game ends based on their position
// In the first test case, only 3 of the entities move
// When I stay still, the game ends when they reach 15 25 and 12 25. The third entity stays at 13 25 the whole time.
// When I A, the game ends when they reach 21 26 and 6 26. The thrid entity moves to 21 25 and just stays there.
// When I E, the game ends when they reach 21 22 and 6 24. The third entity moves to 6 25 and just stays there.

// I am the last entity on the list! The game ends when one of the others reach my position

// When just pacing back and forth between two squares, I end up with a higher score. Going back to the same square still gets me points, this isn't pacman.

// The movement of the enemies is deterministic, but depends on what I do, I should assume that they are coming for me in a somewhat-optimal way.

// This is definitely enough for me to start



int main()
{
    int firstInitInput;
    cin >> firstInitInput; cin.ignore();
	cerr << firstInitInput << endl;
    int secondInitInput;
    cin >> secondInitInput; cin.ignore();
	cerr << secondInitInput << endl;
    int numInputs;
    cin >> numInputs; cin.ignore();
	cerr << numInputs << endl;

	bool flip = false;

	vector<string> moves{ "A", "A", "C" };
	int idx = 0;

    // game loop
    while (1) {
        string firstInput;
        getline(cin, firstInput);
		cerr << firstInput << endl;

        string secondInput;
        getline(cin, secondInput);
		cerr << secondInput << endl;

        string thirdInput;
        getline(cin, thirdInput);
		cerr << thirdInput << endl;

        string fourthInput;
        getline(cin, fourthInput);
		cerr << fourthInput << endl;

        for (int i = 0; i < numInputs; i++) {
            int fifthInput;
            int sixthInput;
            cin >> fifthInput >> sixthInput; cin.ignore();
			cerr << fifthInput << " "<<sixthInput<< endl;

        }

		string answer;
		if (idx < moves.size())
		{
			answer = moves[idx];
		}
		else
		{
			answer = "E";
		}
		cout << answer << endl;
		idx++;
    }
}