// worddice.cpp
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "graph.h"
using namespace std;

int main(int argc, char *argv[]) {
  ifstream diceInput(argv[1]);
  ifstream wordInput(argv[2]);
  string input;
  Graph *myGraph = new Graph();
  myGraph->createNode(" ", SOURCE);
  while (diceInput >> input) {
    myGraph->createNode(input, DICE);
  }
  while (wordInput >> input) {
    myGraph->createNode(input, WORD);
    myGraph->createNode(input, SINK);
    if (myGraph->spellable()) {
      bool comma = false;
      for (size_t i = 0; i < myGraph->spellingIds.size(); i++) {
        if (comma) {
          cout << ",";
        }
        cout << myGraph->spellingIds[i] - 1;
        comma = true;
      }
      cout << ": ";
    } else {
      cout << "Cannot spell ";
    }
    cout << input << endl;
    myGraph->deleteHalfGraph();
  }

  diceInput.close();
  wordInput.close();
  delete myGraph;
  return 0;
}