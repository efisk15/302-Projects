// graph.h

#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
using namespace std;


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
struct Node {
  int id;
  NODE_TYPE type;
  
  int visited;
  vector<bool> letters;
  vector<Edge*> edges;
  Edge *backEdge; // What edge got me here.
  
};

class Graph {
  public:
    Graph();
    ~Graph();
    vector<Node*> nodes;    // All nodes
    vector<int> spellingIds;// Used to print the dice nodes more easily
    bool makeConnections();              // Function of interest.
    bool spellable(bool);        // Check if all residual edges are set to 1.
    void deleteHalfGraph(); // Delete the word Nodes, edges other than between source and dice, and sink;
    void createNode(string, NODE_TYPE);
    int numDice;
    void printGraph();

};

#endif