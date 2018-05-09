#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>
#include <map>
#include <chrono>

using namespace std;

/*
The first two inputs seem to be the size of the grid
The third input seems to be the number of entities in the grid

The 4 string inputs on each turn are either # or _
This seems to indicate what is in the 4 moveable directions
Then I'm given 2 integers for each entity, likely their position in the grid, since the numbers don't seem to exceed the grid size
Then I'm given the choice of 5 outputs, which are movements in each direction or doing nothing

B is doing nothing, since I always end with 2 when only doing B

The coordinates of the entities never change by more than 1 per turn, it seems like these things move 1 space per turn
When outputting A, the string inputs change very turn until there is a # as the second input, then it never changes
It feels as though I've hit a wall, and direction A corresponds to the second input
THe fourht input is the only ones that's _ the whole time, it would make sense if this was the opposite direction of A

A is second
B is wait
C is first
D is third
E is fourth

Therefore, an initial plan is to move in the direction of a _ and not a #

What do the entities have to do with it? My thought is that the game ends based on their position
In the first test case, only 3 of the entities move
When I stay still, the game ends when they reach 15 25 and 12 25. The third entity stays at 13 25 the whole time.
When I A, the game ends when they reach 21 26 and 6 26. The thrid entity moves to 21 25 and just stays there.
When I E, the game ends when they reach 21 22 and 6 24. The third entity moves to 6 25 and just stays there.

I am the last entity on the list! The game ends when one of the others reach my position

When just pacing back and forth between two squares a bit, I don't end up with a higher score. I only get points for moving to new squares. This is pacman!

The movement of the enemies is deterministic, but depends on what I do, I should assume that they are coming for me in a somewhat-optimal way.

This is definitely enough for me to start

I think I get bonus points about 100 bonus points for moving onto a bad guy, although it still ends the game.
You can go through the board, from first col to last col for example, like pacman

*/

enum Direction { UP, DOWN, LEFT, RIGHT, NONE };
enum SquareType { WALL, OPEN, UNKNOWN };

static clock_t start;

struct Move
{
	Direction dir;
	float score;

	Move(Direction dir, float score)
		: dir(dir)
		, score(score)
	{}

	Move()
		: dir(NONE)
		, score(0.0f)
	{}
};

struct Square
{
	SquareType type;
	bool visited;

	Square()
		: type(UNKNOWN)
		, visited(false)
	{}
};

int dist(int x1, int y1, int x2, int y2)
{
	return abs(x1 - x2) + abs(y1 - y2);
}


struct Board
{
	array<int, 2> myPos;
	vector<array<int, 2>> enemies;
	vector<vector<Square>> grid;
	float currentScore;

	Board(int rows, int cols)
		: myPos({ {-1, -1} })
		, currentScore(0.0f)
	{
		for (int i = 0; i < rows; i++)
		{
			grid.emplace_back(cols, Square());
		}
	}

	array<int, 2> getNewSquare(Direction dir, const array<int,2> & init)
	{

		switch (dir)
		{
		case UP:
			return { { init[0] > 0 ? init[0] - 1 : grid.size() - 1 , init[1] } };
		case DOWN:
			return { { init[0] < grid.size() - 1 ? init[0] + 1 : 0 , init[1] } };
		case LEFT:
			return { { init[0] , init[1] > 0 ? init[1] - 1 : grid[0].size() - 1 } };
		case RIGHT:
			return { { init[0] , init[1] < grid[0].size() - 1 ? init[1] + 1 : 0 } };
		default:
			return { {init[0], init[1]} };
		}

	}

	void setSquareType(Direction dir, SquareType type)
	{
		array<int, 2> newPos = getNewSquare(dir, myPos);	
		grid[newPos[0]][newPos[1]].type = type;
	}

	void setVisited()
	{
		Square& sq = grid[myPos[0]][myPos[1]];
		if (sq.visited = false)
		{
			currentScore += 2.0f;
			sq.visited = true;
		}
	}

	void setEnemySquareTypes()
	{
		for (auto& enemy : enemies)
		{
			grid[enemy[0]][enemy[1]].type = OPEN;
		}
	}

	int getScore(Direction dir)
	{
		int score = 0;
		array<int, 2> newPos = getNewSquare(dir, myPos);

		Square& newSquare = grid[newPos[0]][newPos[1]];
		if (newSquare.type == WALL) { return -100; }
		if (!newSquare.visited) { score += 5; }
		for (auto& enemy : enemies)
		{
			int distance = dist(newPos[0], newPos[1], enemy[0], enemy[1]);
			if (distance == 0)
			{
				score -= 10;
				break;
			}

			if (distance == 1)
			{
				score -= 15;
				break;
			}
		}

		return score;

	}

	Move getBestMove()
	{
		int depth = 1;
		Move bestMove ;
		while ((clock() - start) / (double)(CLOCKS_PER_SEC / 1000) < 20.0)
		{
			bestMove = dfs(depth);
			depth++;
			//cerr << depth << endl;
		}

		return bestMove;
	}

	Move dfs(int depth)
	{
		if (depth == 0)
		{
			//Score the position
			return Move(NONE, getHeuristicScore());
		}

		// For now, don't order the exploration
		// Leave out blocked moves
		array<int, 2> myInitPos = myPos;
		float initScore = currentScore;
		auto initEnemies = enemies;
		float maxScore = -9999.0f;
		Move bestMove;

		for (int i = 0; i < 4; i++)
		{
			Direction dir = (Direction)(i);
			array<int, 2> newPos = getNewSquare(dir, myPos);
			Square* square = &grid[newPos[0]][newPos[1]];
			bool currVisit = square->visited;
			if (square->type != WALL)
			{
				// Advance
				myPos = newPos;
				if (!square->visited)
				{
					currentScore += 2.0f;
					square->visited = true;
				}
				
				float score = advanceEnemies() ? dfs(depth - 1).score : getHeuristicScore();	

				if (score > maxScore)
				{
					maxScore = score;
					bestMove = Move(dir, score);
				}

				// Reset
				myPos = myInitPos;
				enemies = initEnemies;
				currentScore = initScore;
				square->visited = currVisit;
			}
		}

		// Test staying still
		float score = advanceEnemies() ? dfs(depth - 1).score : getHeuristicScore();
		if (score > maxScore)
		{
			maxScore = score;
			bestMove = Move(NONE, score);
		}

		enemies = initEnemies;

		return bestMove;
	}

	bool advanceEnemies()
	{
		// Advance enemies
		// Minimal enemy prediction for now
		for (auto& enemy : enemies)
		{
			// Assume every unknown square is open
			int minDist = dist(enemy[0], enemy[1], myPos[0], myPos[1]);
			if (minDist == 0)
			{
				// I've hit this enemy, I get 100 points, the enemy doesn't move, game over
				currentScore += 100.0f;
				return false;
			}
			array<int, 2> bestSquare = enemy;

			for (int i = 0; i < 4; i++)
			{
				array<int, 2> newSquare = getNewSquare((Direction)(i), enemy);
				if (grid[newSquare[0]][newSquare[1]].type != WALL)
				{
					int distance = dist(newSquare[0], newSquare[1], myPos[0], myPos[1]);
					if (distance == 0)
					{
						// The enemy hits me, game over
						return false;
					}
					if (distance < minDist)
					{
						bestSquare = newSquare;
						minDist = distance;
					}
				}
			}

			enemy = bestSquare;
		}

		return true;
	}

	float getHeuristicScore() const
	{
		// Obviously based on current score
		float score = currentScore;

		// If I share a square with an enemy, this is obviously a big loss
		// I want to be far from enemies
		int totalDist = 0;
		for (auto& enemy : enemies)
		{
			int distance = dist(myPos[0], myPos[1], enemy[0], enemy[1]);
			if (distance == 0)
			{
				score -= 100.0f;
				break;
			}

			else
			{
				totalDist += distance;
			}
		}

		score += (float)(totalDist) / 150.0f;
		return score;

	}

};


int main()
{
	map<Direction, string> outputMap;
	outputMap[DOWN] = "A";
	outputMap[UP] = "E";
	outputMap[NONE] = "B";
	outputMap[LEFT] = "C";
	outputMap[RIGHT] = "D";

	map<string, SquareType> inputMap;
	inputMap["#"] = WALL;
	inputMap["_"] = OPEN;

	int firstInitInput;
	cin >> firstInitInput; cin.ignore();
	cerr << firstInitInput << endl;
	int secondInitInput;
	cin >> secondInitInput; cin.ignore();
	cerr << secondInitInput << endl;
	int numInputs;
	cin >> numInputs; cin.ignore();
	cerr << numInputs << endl;

	Board board{ secondInitInput, firstInitInput};


	// game loop
	while (1) {
		start = clock();
		
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

		board.enemies.clear();
		for (int i = 0; i < numInputs; i++) {
			int fifthInput;
			int sixthInput;
			cin >> fifthInput >> sixthInput; cin.ignore();
			cerr << fifthInput << " " << sixthInput << endl;

			if (i < numInputs - 1)
			{
				board.enemies.push_back({ { fifthInput, sixthInput } });
			}

			else
			{
				board.myPos = { {fifthInput, sixthInput} };
			}
		}

		// First string is left
		// Second string is down
		// Third string is right
		// fourth string is up
		board.setSquareType(LEFT, inputMap[firstInput]);
		board.setSquareType(DOWN, inputMap[secondInput]);
		board.setSquareType(RIGHT, inputMap[thirdInput]);
		board.setSquareType(UP, inputMap[fourthInput]);

		board.setVisited();
		board.setEnemySquareTypes();

		Move bestMove = board.getBestMove();

		cout << outputMap[bestMove.dir] << endl;
	}
}