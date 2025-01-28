// Challenge 03: Palindrome Permutation
// Eli Fisk

// Brief Description:
// This program will take in a string of letters and determine if the string
// is a permutation of a palindrome. "Permutation of a palindrome" being a
// string that has 2 of each letter and can then have one extra letter. (Ex. "ciivc")

#include <algorithm>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <unordered_set>
using namespace std;
int main(int argc, char *argv[]) {
  string input;
  while (getline(cin, input)) {
    unordered_set<char> mySet;
    int count = 0;

    // Put chars from given word into the set and keep count of the chars.
    for (int i = 0; i < int(input.size()); i++) {
      if ((input[i] < 'z' && input[i] > 'a') ||
          (input[i] < 'Z' && input[i] > 'A')) {
        mySet.insert(input[i]);
        count++;
      }
    }
    // The set will only keep one of each char so 2 * mySet.size() should be no
    // more than 1 + total number of chars.
    if ((int(mySet.size()) * 2) > (count + 1)) {
      cout << "\"" << input << "\" is not a palindrome permutation" << endl;
    } else {
      cout << "\"" << input << "\" is a palindrome permutation" << endl;
    }
  }
  return (0);
}
