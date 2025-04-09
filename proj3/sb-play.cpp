/* Project 3 of COSC 302 at University of Tennessee Knoxville.
 * File: sb-play.cpp
 * Written by Eli Fisk.
 * Date: 2/7/2025
 *
 * Description:
 * This program can be used to analyze a board from the game Super Ball, and
 * then print out the next best move for the game given the current board. It
 * takes the boards as standard input and then prints out either a SWAP move or
 * a SCORE move.
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
  // fprintf(stderr, "%s\n", argv[4]);

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

  s = new Superball(argc, argv);
  int boardSize = s->r * s->c;

  DisjointSetByRankWPC ds(boardSize);

  // Variables for holding score assessments.
  int score = 0;
  int highestMap = 0;
  int highestI = -1;
  int highestJ = -1;
  int highestScoreMap = 0;
  int highestScoreI = -1;
  int highestScoreJ = -1;
  int baseI = 0;
  int baseJ = 0;

  map<int, ScoreInfo> answers;

  // First check if there is a scoreble set, if so, score it.
  analyze_superBall(s, ds, answers);
  for (map<int, ScoreInfo>::iterator answer = answers.begin();
       answer != answers.end(); answer++) {
    if (answer->second.number >= s->mss) {
      score = answer->second.location;
      if (score) {
        int row = score / s->c;
        int cal = score % s->c;
        cout << "SCORE " << row << " " << cal << endl;
        return 0;
      }
    }
    // Set minimum set scores for comparisons later.
    if (answer->second.location != 0) {
      highestScoreMap += answer->second.number;
    } else {
      highestMap += answer->second.number;
    }
  }
  if (score > -1) {
    // Test every SWAP combination possible.
    for (int i = 0; i < boardSize; i++) {
      if (s->board[i] != 46 && s->board[i] != 42) {
        for (int j = i + 1; j < boardSize; j++) {
          if (s->board[j] != 46 && s->board[j] != 42) {
            if (s->board[i] != s->board[j]) {
              // Default if none of the cells are touching.
              baseI = i;
              baseJ = j;

              // Swap 2 cells to then analyze new board.
              int holdJ = s->board[j];
              s->board[j] = s->board[i];
              s->board[i] = holdJ;
              map<int, ScoreInfo> answers;
              analyze_superBall(s, ds, answers);

              // Swap cells back to original locations.
              s->board[i] = s->board[j];
              s->board[j] = holdJ;
              int totalScorePoints = 0;
              int totalNormPoints = 0;

              // Each new board will get 2 scores. The scores will be:
              // Sizes of all non-scoreble sets added together or
              // (Sizes of all scoreble sets)^2.
              // Keep track of the boards that have the highest of each.
              for (map<int, ScoreInfo>::iterator answer = answers.begin();
                   answer != answers.end(); answer++) {
                if (answer->second.location != 0) {
                  totalScorePoints +=
                      answer->second.number * answer->second.number;
                }
                if (answer->second.location == 0) {
                  totalNormPoints += answer->second.number;
                }
              }
              if (totalScorePoints >= highestScoreMap) {
                highestScoreMap = totalScorePoints;
                highestScoreI = i;
                highestScoreJ = j;
              }
              if (totalNormPoints >= highestMap) {
                highestMap = totalNormPoints;
                highestI = i;
                highestJ = j;
              }
            }
          }
        }
      }
    }
  }
  int scoreJ, scoreI;

  // Swap to get the highest scoring sets if possible.
  if (highestScoreI > -1 && highestScoreJ > -1) {
    scoreI = highestScoreI;
    scoreJ = highestScoreJ;

    // Use the swap with the highest none scoreble sets for a backup.
  } else if (highestI > -1 && highestJ > -1) {
    scoreI = highestI;
    scoreJ = highestJ;

    // Just swap to cells for a backup to the backup.
  } else {
    scoreI = baseI;
    scoreJ = baseJ;
  }
  int rowI = scoreI / s->c;
  int calI = scoreI % s->c;
  int rowJ = scoreJ / s->c;
  int calJ = scoreJ % s->c;
  cout << "SWAP " << rowI << " " << calI << " " << rowJ << " " << calJ << endl;
}
