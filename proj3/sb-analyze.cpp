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

void analyze_superBall(Superball *s, DisjointSetByRankWPC ds,
                       map<int, ScoreInfo> &answers) {
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

  DisjointSetByRankWPC ds(s->r * s->c);
  map<int, ScoreInfo> answers;

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
  analyze_superBall(s, ds, answers);
  cout << "Scoring sets:" << endl;
  for (map<int, ScoreInfo>::iterator answer = answers.begin();
       answer != answers.end(); answer++) {
    int j = answer->second.number;
    if (j >= s->mss) {
      int row = answer->second.location / s->c;
      int cal = answer->second.location % s->c;
      cout << "Size:" << setw(3) << answer->second.number
           << "  Char: " << answer->second.color << "  Scoring Cell: " << row
           << "," << cal << endl;
    }
  }
}