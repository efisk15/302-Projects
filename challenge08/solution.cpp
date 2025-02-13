// Challenge 08: sequence alignment
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

enum TYPE {
  MATCH = 1,
  MISMATCH = -1,
  GAP = -1,
};

void printBoard(vector<vector<int>> matrix) {
  for (size_t i = 0; i < matrix.size(); i++) {
    for (size_t j = 0; j < matrix[0].size(); j++) {
      cout << setw(2) << matrix[i][j] << " ";
    }
    cout << endl;
  }
}

int main(int argc, char *argv[]) {
  string sequenceOne, sequenceTwo = " ";
  while (cin >> sequenceOne) {
    cin >> sequenceTwo;
    int stringSizeOne = sequenceOne.size();
    int stringSizeTwo = sequenceTwo.size();
    vector<int> expansion(sequenceOne.size() + 2, 0);
    vector<vector<int>> matrix(sequenceTwo.size() + 2, expansion);
    // matrix[row][cal]
    for (int i = 2; i < stringSizeOne + 2; i++) {
      matrix[0][i] = sequenceOne[i - 2];
    }
    for (int i = 2; i < stringSizeOne + 2; i++) {
      matrix[1][i] = (i - 1) * GAP;
    }
    for (int i = 2; i < stringSizeTwo + 2; i++) {
      matrix[i][0] = sequenceTwo[i - 2];
    }
    for (int i = 2; i < stringSizeTwo + 2; i++) {
      matrix[i][1] = (i - 1) * GAP;
    }
    for (int i = 2; i < stringSizeOne + 2; i++) {
      for (int j = 2; j < stringSizeTwo + 2; j++) {
        int addition = 0;
        if(matrix[0][j] == matrix[i][0]) {
          addition = 1;
        } else {
          addition = -1;
        }
        int diagonal = matrix[i - 1][j - 1] + addition;
        int left = matrix[i][j - 1] - 1;
        int up = matrix[i - 1][j] - 1;
        int cell = up;
        if(diagonal >= up) {
          cell = diagonal;
        }
        if(left >= cell) {
          cell = left;
        }
        matrix[i][j] = cell;
      }
    }
      cout << matrix[stringSizeOne + 1][stringSizeTwo + 1] << endl;
    }
    return 0;
  }
