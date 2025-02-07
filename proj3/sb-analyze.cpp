/* Project 3 of COSC 302 at University of Tennessee Knoxville.
 * File: sb-analyze.cpp
 * Written by Eli Fisk.
 * Date: 2/6/2025
 *
 * Description:
 * This program can be used to analyze a board from the game Super Ball. It
 * takes the boards as standard input and then prints out the score-able sets
 * and the location of a cell in the set that you can use to score each set
 * with.
 *
 * Citations:
 * Used the lab's Superball object to be able to read in the board from standard
 * input. Also used the lab's disjointed sets object to create disjointed sets.
 */

#include <cstdio>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

#include "disjoint.h"

using namespace std;

class Superball {
 public:
  Superball(int argc, char **argv);
  int r;
  int c;
  int mss;
  int empty;
  vector<int> board;
  vector<int> goals;
  vector<int> colors;
};

// Custom struct to hold disjointed set info.
struct ScoreInfo {
  int number;
  int location;
  char color;
};

void usage(const char *s) {
  fprintf(stderr, "usage: sb-analyze rows cols min-score-size colors\n");
  if (s != NULL) fprintf(stderr, "%s\n", s);
  exit(1);
}

Superball::Superball(int argc, char **argv) {
  int i, j;
  string s;

  if (argc != 5) usage(NULL);

  if (sscanf(argv[1], "%d", &r) == 0 || r <= 0) usage("Bad rows");
  if (sscanf(argv[2], "%d", &c) == 0 || c <= 0) usage("Bad cols");
  if (sscanf(argv[3], "%d", &mss) == 0 || mss <= 0) usage("Bad min-score-size");

  colors.resize(256, 0);

  for (i = 0; i < (int)strlen(argv[4]); i++) {
    if (!isalpha(argv[4][i])) usage("Colors must be distinct letters");
    if (!islower(argv[4][i])) usage("Colors must be lowercase letters");
    if (colors[argv[4][i]] != 0) usage("Duplicate color");
    colors[argv[4][i]] = 2 + i;
    colors[toupper(argv[4][i])] = 2 + i;
  }

  board.resize(r * c);
  goals.resize(r * c, 0);

  empty = 0;

  for (i = 0; i < r; i++) {
    if (!(cin >> s)) {
      fprintf(stderr, "Bad board: not enough rows on standard input\n");
      exit(1);
    }
    if ((int)s.size() != c) {
      fprintf(stderr, "Bad board on row %d - wrong number of characters.\n", i);
      exit(1);
    }
    for (j = 0; j < c; j++) {
      if (s[j] != '*' && s[j] != '.' && colors[s[j]] == 0) {
        fprintf(stderr, "Bad board row %d - bad character %c.\n", i, s[j]);
        exit(1);
      }
      board[i * c + j] = s[j];
      if (board[i * c + j] == '.') empty++;
      if (board[i * c + j] == '*') empty++;
      if (isupper(board[i * c + j]) || board[i * c + j] == '*') {
        goals[i * c + j] = 1;
        board[i * c + j] = tolower(board[i * c + j]);
      }
    }
  }
}

// Function to analyze the board. takes in a map of custom struct and updates it
// to be returned.
void analyze_superBall(Superball *s, DisjointSetByRankWPC ds,
                       map<int, ScoreInfo> &answers) {
  // Go through the board and create unions to make disjointed sets.
  for (size_t i = 0; i < s->board.size(); i++) {
    if (s->board[i] != 46 && s->board[i] != 42) {
      // Check if the cell below the current cell is the same as the current
      // cell.
      if (int(i + s->c) < (s->c * s->r) && s->board[i] == s->board[i + s->c]) {
        ds.Union(ds.Find(i), ds.Find(i + s->c));
      }
      // Check if the cell behind the current cell is the same as the current
      // cell.
      if (s->board[i] == s->board[i - 1] && i % s->c != 0) {
        ds.Union(ds.Find(i), ds.Find(i - 1));
      }
    }
  }
  // Add each disjointed set to a map with custom struct.
  for (size_t i = 0; i < s->board.size(); i++) {
    if (s->board[i] != 46 && s->board[i] != 42) {
      int key = ds.Find(i);

      // If set already added, increment the set's size.
      if (answers.find(key) != answers.end()) {
        answers[key].number++;

        // If it doesn't exist, add a new element.
      } else {
        ScoreInfo newVal;
        newVal.color = s->board[i];
        newVal.number = 1;
        newVal.location = 0;
        answers.insert({ds.Find(i), newVal});
      }

      // If the current cell is scoreble, set the element's location
      if (s->goals[i]) {
        answers[key].location = i;
      }
    }
  }
}

int main(int argc, char **argv) {
  Superball *s;
  // Make a new Superball object.
  s = new Superball(argc, argv);

  DisjointSetByRankWPC ds(s->r * s->c);

  // Create map to hold all disjointed sets.
  map<int, ScoreInfo> answers;
  analyze_superBall(s, ds, answers);
  cout << "Scoring sets:" << endl;

  // Go through map and print the sets that are scoreble.
  for (map<int, ScoreInfo>::iterator answer = answers.begin();
       answer != answers.end(); answer++) {
    int j = answer->second.number;
    if (j >= s->mss && answer->second.location) {
      int row = answer->second.location / s->c;
      int cal = answer->second.location % s->c;
      cout << "Size:" << setw(3) << answer->second.number
           << "  Char: " << answer->second.color << "  Scoring Cell: " << row
           << "," << cal << endl;
    }
  }
}
