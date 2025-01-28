// Challenge 02: Closest Numbers
// Name:  Eli Fisk

// Brief description:
// This code will take a given number of elements and find the pairs of elements
// that have the least difference between them. It will then print out that
// pair. If there are other pairs with that same least difference, they will be
// printed out as well.

#include <algorithm>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

// Main Execution

int main(int argc, char *argv[]) {
  int numOfElm = 0;

  // While input is given, continue.
  while (cin >> numOfElm) {
    int i = 0;
    int val = 0;
    vector<int> numbers;
    vector<int> answers;

    // Get all the values given so that they can be sorted.
    while (i < numOfElm) {
      cin >> val;
      numbers.push_back(val);
      i++;
    }
    sort(numbers.begin(), numbers.end());
    int minDif = 0;

    // Get a minimum difference to start with.
    minDif = numbers[1] - numbers[0];
    answers.push_back(numbers[0]);
    answers.push_back(numbers[1]);

    // Go through the list looking for any pair with a smaller difference.
    for (int j = 1; j < numOfElm - 1; j++) {
      int newDif = numbers[j + 1] - numbers[j];
      if (newDif < minDif) {
        minDif = newDif;
        // Reset list of pairs if a new min difference is found.
        answers.resize(0);
        answers.push_back(numbers[j]);
        answers.push_back(numbers[j + 1]);
      } else if (newDif == minDif) {
        answers.push_back(numbers[j]);
        answers.push_back(numbers[j + 1]);
      }
    }

    // Print.
    for (size_t i = 0; i < answers.size() - 1; i++) {
      cout << answers[i] << " ";
    }
    cout << answers[answers.size() - 1] << endl;
    answers.resize(0);
    numbers.resize(0);
  }

  return EXIT_SUCCESS;
}