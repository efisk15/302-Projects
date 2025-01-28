// Challenge 01: Rotating Arrays; null solution

// Name: Eli Fisk

// Description: This program will take a number of elements, n, in a list and rotate them
// rotate them a given number of times in a given direction. It will complete this in O(n)
// time. It will then print the new output to the command line.

#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main(int argc, char *argv[]) {
  int totalNum = 0;
  int rotations = 0;
  char direction = ' ';
  vector<int> numbers;

  // While there is input to get, place input in variables.
  while (cin >> totalNum) {
    cin >> rotations;
    cin >> direction;

    // Make sure the vector is the right size so that you can index into it.
    numbers.resize(totalNum, 0);
    int value;

    // Go value by value and place each one in it's final index based on given
    // direction and rotations.
    for (int i = 0; i < totalNum; i++) {
      cin >> value;
      int position = 0;
      if (direction == 'L') {
        position = i - rotations % totalNum;
        if (position < 0) {
          position += totalNum;
        }
      } else {
        position = (rotations + i) % totalNum;
      }
      numbers[position] = value;
    }

    // Print the values.
    for (int i = 0; i < totalNum - 1; i++) {
      cout << numbers[i] << " ";
    }
    cout << numbers[totalNum - 1] << endl;
  }
  return 0;
}