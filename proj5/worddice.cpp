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
    // cout << input << endl;
    myGraph->createNode(input, DICE);
  }
  while (wordInput >> input) {
    myGraph->createNode(input, WORD);
    myGraph->createNode(input, SINK);
    // cout << input << endl;
    if (!myGraph->makeConnections()) {
      cout << "Cannot spell " << input << endl;
      
    } else {
      myGraph->spellable(true);
      cout << input << endl;
    }
    // myGraph->printGraph();
    myGraph->deleteHalfGraph();
  }
  // myGraph->printGraph();

  diceInput.close();
  wordInput.close();
  delete myGraph;
  return 0;
}
