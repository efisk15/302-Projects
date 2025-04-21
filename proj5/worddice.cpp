/* Project 5: Word Dice
 * File: worddice.cpp
 * Name: Eli Fisk.
 * Date: 4/21/2025
 *
 * Description:
 * This program will take a file of dice made of chars and a word and it will see if that word can
 * be spelled using the dice using network flow.
 */
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "graph.h"
using namespace std;

int main(int argc, char *argv[]) {
  // Read in dice and word file.
  ifstream diceInput(argv[1]);
  ifstream wordInput(argv[2]);
  string input;
  Graph *myGraph = new Graph();

  // Create the source node.
  myGraph->createNode(" ", SOURCE);
  // Create the Dice nodes.
  while (diceInput >> input) {
    myGraph->createNode(input, DICE);
  }

  // For each of the words in the word file, do the following.
  while (wordInput >> input) {
    // Create a node for each letter in the word.
    myGraph->createNode(input, WORD);
    // Create a sink node.
    myGraph->createNode(input, SINK);
    // Run the spellable function to see if the word can be spelled.
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
    // Delete the second half of the graph to restart.
    myGraph->deleteHalfGraph();
  }

  diceInput.close();
  wordInput.close();
  delete myGraph;
  return 0;
}
