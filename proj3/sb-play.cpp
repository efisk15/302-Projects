// Testing: /home/jplank/cs302/Labs/Lab5/bin/sb-player 8 10 5 pbyrg bin/sb-play
// y y -

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

  if (argc != 9 && argc != 5) usage(NULL);

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

void analyze_superBall(Superball *s, DisjointSetByRankWPC ds,
                       map<int, ScoreInfo> &answers) {
  for (size_t i = 0; i < s->board.size(); i++) {
    if (s->board[i] != 46 && s->board[i] != 42) {
      if (int(i + s->c) < (s->c * s->r) && s->board[i] == s->board[i + s->c]) {
        ds.Union(ds.Find(i), ds.Find(i + s->c));
      }
      if (s->board[i] == s->board[i - 1] && i % s->c != 0) {
        ds.Union(ds.Find(i), ds.Find(i - 1));
      }
    }
  }
  for (size_t i = 0; i < s->board.size(); i++) {
    if (s->board[i] != 46 && s->board[i] != 42) {
      if (s->goals[i]) {
        ScoreInfo newVal;
        newVal.location = i;
        newVal.number = 0;
        newVal.color = s->board[i];

        answers.insert({ds.Find(i), newVal});
      }
    }
  }
  for (size_t i = 0; i < s->board.size(); i++) {
    int key = ds.Find(i);
    if (answers.find(key) != answers.end()) {
      answers[key].number++;
    }
  }
}

int main(int argc, char **argv) {
  Superball *s;

  s = new Superball(argc, argv);
  int boardSize = s->r * s->c;

  DisjointSetByRankWPC ds(boardSize);
  int score = 0;
  int highestMap = 0;
  int highestI = 0;
  int highestJ = 0;
  int baseI = 0;
  int baseJ = 0;

  map<int, ScoreInfo> answers;
  analyze_superBall(s, ds, answers);
  for (map<int, ScoreInfo>::iterator answer = answers.begin();
       answer != answers.end(); answer++) {
    int why = answer->second.number;
    if (why >= s->mss) {
      score = answer->second.location;
    }
  }
  if (score == 0) {
    for (int i = 0; i < boardSize; i++) {
      if (s->board[i] != 46 && s->board[i] != 42) {
        for (int j = i + 1; j < boardSize; j++) {
          if (s->board[j] != 46 && s->board[j] != 42) {
            if (s->board[i] != s->board[j]) {
              baseI = i;
              baseJ = j;
              int holdJ = s->board[j];
              s->board[j] = s->board[i];
              s->board[i] = holdJ;
              map<int, ScoreInfo> answers;
              // cout << "before" << endl;
              analyze_superBall(s, ds, answers);
              // cout << "after" << endl;
              s->board[i] = s->board[j];
              s->board[j] = holdJ;
              for (map<int, ScoreInfo>::iterator answer = answers.begin();
                   answer != answers.end(); answer++) {
                int why = answer->second.number;
                if (why > highestMap) {
                  highestMap = why;
                  highestI = i;
                  highestJ = j;
                }
              }
            }
          }
        }
      }
    }
  }

  if (score) {
    int row = score / s->c;
    int cal = score % s->c;
    cout << "SCORE " << row << " " << cal << endl;
  } else if (highestI && highestJ) {
    // cout << highestI << endl;
    int rowI = highestI / s->c;
    int calI = highestI % s->c;
    int rowJ = highestJ / s->c;
    int calJ = highestJ % s->c;

    cout << "SWAP " << rowI << " " << calI << " " << rowJ << " " << calJ
         << endl;
  } else {
    int rowI = baseI / s->c;
    int calI = baseI % s->c;
    int rowJ = baseJ / s->c;
    int calJ = baseJ % s->c;

    cout << "SWAP " << rowI << " " << calI << " " << rowJ << " " << calJ
         << endl;
  }
}