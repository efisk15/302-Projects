// graph.h
/* Project 5: Word Dice
 * File: graph.h
 * Name: Eli Fisk.
 * Date: 4/21/2025
 *
 * Description:
 * This file holds the structs and the class for network flow.
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
using namespace std;

// Enum for node types
enum NODE_TYPE {
  SOURCE,
  WORD,
  SINK,
  DICE
};

// There will be two edges in between each node. e1's reverse points at e1r's edge.
struct Edge {
    class Node *to;
    class Node *from;
    Edge *reverse;
    int original; // Starting val: 1 -> e1r = opposite
    int residual; // Start val: 0 -> e1r = opposite

};

// Node struct
struct Node {
  int id;
  NODE_TYPE type;
  
  int visited;
  vector<bool> letters;
  vector<Edge*> edges;
  Edge *backEdge;
  
};

class Graph {
  public:
    Graph();
    vector<Node*> nodes;
    // Used to print the dice nodes more easily
    vector<int> spellingIds; 
    // Return true if there is a path to the sink.
    bool BFS();
    // Check if all residual edges are set to 1.
    bool spellable();
    // Delete the word Nodes, the sink, and the edges between them as well as the edges between the word and dice nodes.
    void deleteHalfGraph();
    // Create nodes from input.
    void createNode(string, NODE_TYPE);
    int numDice;
    // Keep track of the edges used in BFS.
    vector<Edge*> changedEdges;

};

#endif
