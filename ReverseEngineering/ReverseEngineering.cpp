#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main()
{
    int firstInitInput;
    cin >> firstInitInput; cin.ignore();
    int secondInitInput;
    cin >> secondInitInput; cin.ignore();
    int thirdInitInput;
    cin >> thirdInitInput; cin.ignore();

    // game loop
    while (1) {
        string firstInput;
        getline(cin, firstInput);
        string secondInput;
        getline(cin, secondInput);
        string thirdInput;
        getline(cin, thirdInput);
        string fourthInput;
        getline(cin, fourthInput);
        for (int i = 0; i < thirdInitInput; i++) {
            int fifthInput;
            int sixthInput;
            cin >> fifthInput >> sixthInput; cin.ignore();
        }

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;

        cout << "A, B, C, D or E" << endl;
    }
}