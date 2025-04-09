/* Challenge 08: sequence alignment
 * File: solution.cpp
 * Name: Eli Fisk.
 * Date: 4/9/2025
 *
 * Description:
 * This program will take two strings as DNA sequences and compare them using Dynamic programming and storing
 * their comparison values into a 2d matrix. The program will the return the value of the (m,n) cell in the matrix.
 */
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

// Enum for different cell types.
enum TYPE {
  MATCH = 1,
  MISMATCH = -1,
  GAP = -1,
};

int main(int argc, char *argv[]) {
  string sequenceOne, sequenceTwo = "";
  while (cin >> sequenceOne) {
    cin >> sequenceTwo;
    int stringSizeOne = sequenceOne.size();
    int stringSizeTwo = sequenceTwo.size();
    vector<int> expansion(sequenceOne.size() + 2, 0);
    vector<vector<int>> matrix(sequenceTwo.size() + 2, expansion);

    // Set up the edges of the matrix.
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
    for (int i = 2; i < stringSizeTwo + 2; i++) {
      for (int j = 2; j < stringSizeOne + 2; j++) {
        int addition = 0;

        // Check if the chars are the same.
        if (matrix[0][j] == matrix[i][0]) {
          addition = 1;
        } else {
          addition = -1;
        }

        // Find the value of cells in each direction.
        int diagonal = matrix[i - 1][j - 1] + addition;
        int left = matrix[i][j - 1] - 1;
        int up = matrix[i - 1][j] - 1;
        int cell = up;

        // Pick the largest value for the cell.
        if (diagonal >= up) {
          cell = diagonal;
        }
        if (left >= cell) {
          cell = left;
        }
        matrix[i][j] = cell;
      }
    }

    // Print the last cell.
    cout << matrix[matrix.size() - 1][matrix[0].size() - 1] << endl;
    break;
  }
  return 0;
}
