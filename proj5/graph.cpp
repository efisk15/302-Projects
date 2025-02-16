#include "graph.h"

#include <iostream>
#include <map>
using namespace std;

Graph::Graph() {
  nodes.resize(0);
  spellingIds.resize(0);
  numDice = 0;
}

Graph::~Graph() {
  //   for (size_t i = 0; i < nodes.size(); i++) {
  //     for (size_t j = 0; j < nodes[i]->edges.size(); j++) {
  //       delete nodes[i]->edges[i];
  //     };
  //     delete nodes[i];
  //   }
}

bool Graph::spellable(bool print) {
  for (size_t i = numDice + 1; i < nodes.size() - 1; i++) {
    if (!nodes[i]->visited) {
      // cout << "basic" << endl;
      return false;
    }
  }
  if (print) {
    bool comma = false;
    for (size_t i = numDice + 1; i < nodes.size() - 1; i++) {
      if (comma) {
        cout << ",";
      }
      cout << nodes[i]->backEdge->to->id - 1;
      comma = true;
    }
    cout << ": ";
  }
  return true;
}
bool Graph::makeConnections() {
  // Look through all the letters
  // Make single connections:
  for (int i = 1; i < numDice + 1; i++) {
    if (nodes[i]->edges.size() == 1 && !nodes[i]->visited) {
      if (!nodes[i]->edges[0]->to->visited) {
        nodes[i]->edges[0]->reverse->residual = 1;
        nodes[i]->edges[0]->reverse->original = 0;
        nodes[i]->edges[0]->residual = 1;
        nodes[i]->edges[0]->original = 0;
        nodes[i]->edges[0]->to->visited = true;
        nodes[i]->edges[0]->from->visited = true;
        nodes[i]->edges[0]->to->backEdge = nodes[i]->edges[0]->reverse;
        nodes[i]->visited = true;
        break;
      }
    }
  }
  for (size_t i = numDice + 1; i < nodes.size() - 1; i++) {
    // Base case if char is not attached to anything.
    if (nodes[i]->edges.size() == 1) {
      // cout << "it be" << endl;
      return false;
    }

    // Look though all nodes with a certain number of edges,
    if (nodes[i]->edges.size() == 2 && !nodes[i]->visited) {
      // cout << " testing2: " << nodes[i]->id << endl;
      // cout << "ids part 2: " << nodes[i]->id << endl;
      if (!nodes[i]->edges[0]->to->visited) {
        // cout << "node id2:" << nodes[i]->id << "-> "
        //      << nodes[i]->edges[k]->to->id << endl;
        nodes[i]->edges[0]->reverse->residual = 1;
        nodes[i]->edges[0]->reverse->original = 0;
        nodes[i]->edges[0]->residual = 1;
        nodes[i]->edges[0]->original = 0;
        nodes[i]->edges[0]->to->visited = true;
        nodes[i]->edges[0]->from->visited = true;
        nodes[i]->backEdge = nodes[i]->edges[0];
        nodes[i]->visited = true;
      } else {
        // cout << "canceling" << endl;
        return false;
      }
    }
  }
  // printGraph();
  // cout << "/////////////////////////////////////////" << endl;

  // See if there are any dice connect to only one letter;
  int itr = 3;
  while (true) {
    // cout << 1 << endl;
    // For all chars that have itr edges;
    for (size_t i = numDice + 1; i < nodes.size() - 1; i++) {
      // cout << "id: " << nodes[i]->id << endl;
      if (int(nodes[i]->edges.size()) == itr && !nodes[i]->visited) {
        // cout << "id: " << nodes[i]->id << endl;
        // For all the chars's edges.
        multimap<int, int> answers;
        for (size_t j = 0; j < nodes[i]->edges.size() - 1; j++) {
          // Compare the num edges of the Dice the char is connect to. Take the
          // smallest number.
          if (!nodes[i]->edges[j]->to->visited) {
            int valid = 0;
            for(size_t k = 0; k < nodes[i]->edges[j]->to->edges.size();k++) {
              if(!nodes[i]->edges[j]->to->edges[k]->to->visited){
                valid++;
              }

            }
            answers.insert(
                pair<int, int>(valid, j));
          }
        }
        if (answers.size() == 0) {
          // cout << "cancel early" << endl;
          return false;
        }
        int number = answers.begin()->second;
        nodes[i]->edges[number]->reverse->residual = 1;
        nodes[i]->edges[number]->reverse->original = 0;
        nodes[i]->edges[number]->residual = 1;
        nodes[i]->edges[number]->original = 0;
        nodes[i]->edges[number]->to->visited = true;
        nodes[i]->edges[number]->from->visited = true;
        nodes[i]->backEdge = nodes[i]->edges[number];
        nodes[i]->visited = true;
      }
    }
    if (spellable(false)) {
      break;
    };
    itr++;
    // cout << "invalid" << endl;
    // return false;
  }
  return true;
}
void Graph::deleteHalfGraph() {
  for (size_t i = 1; i < nodes.size(); i++) {
    for (size_t j = 0; j < nodes[i]->edges.size(); j++) {
      if (nodes[i]->edges[j]->from->type == WORD ||
          nodes[i]->edges[j]->to->type == WORD) {
        delete nodes[i]->edges[j];
      }
    };
    if (nodes[i]->type != DICE) {
      delete nodes[i];
    } else {
      nodes[i]->edges.resize(0);
      nodes[i]->visited = false;
    }
  }
  nodes.resize(numDice + 1);
}

void Graph::printGraph() {
  for (size_t i = 0; i < nodes.size(); i++) {
    for (size_t j = 0; j < nodes[i]->edges.size(); j++) {
      if (nodes[i]->edges[j]->residual == 1) {
        cout << nodes[i]->edges[j]->from->id << "-Connected->"
             << nodes[i]->edges[j]->to->id << endl;
      } else {
        cout << nodes[i]->edges[j]->from->id << "->"
             << nodes[i]->edges[j]->to->id << endl;
      }
    }
  }
  for (size_t i = 0; i < nodes.size(); i++) {
    cout << nodes[i]->id << ": Visited: " << nodes[i]->visited << endl;
  }
}
void Graph::createNode(string input, NODE_TYPE type) {
  if (type == SOURCE) {
    Node* newNode = new Node();
    newNode->id = 0;
    newNode->type = type;
    // cout << "SOURCE created" << endl;
    nodes.push_back(newNode);

  } else if (type == WORD) {
    int startingId = numDice + 1;
    // Loop through letters
    for (size_t i = 0; i < input.size(); i++) {
      Node* newNode = new Node();
      newNode->id = startingId;
      newNode->type = type;
      newNode->letters.resize(26, 0);
      newNode->letters[input[i] - 'A'] = true;

      // Loop through Dice nodes
      for (int j = 1; j < numDice + 1; j++) {
        if (nodes[j]->letters[input[i] - 'A']) {
          Edge* edge = new Edge();
          Edge* backEdge = new Edge();
          edge->to = newNode;
          edge->from = nodes[j];
          edge->original = 1;
          edge->residual = 0;
          backEdge->to = nodes[j];
          backEdge->from = newNode;
          backEdge->original = 1;
          backEdge->residual = 0;
          edge->reverse = backEdge;
          backEdge->reverse = edge;
          nodes[j]->edges.push_back(edge);
          newNode->edges.push_back(backEdge);
        }
      }
      // cout << newNode->id << endl;
      newNode->visited = false;
      nodes.push_back(newNode);
      startingId++;
      // cout << "Word created" << endl;
    }

  } else if (type == DICE) {
    Node* newNode = new Node();
    int startingId = nodes.size();
    newNode->id = startingId;
    newNode->type = type;
    newNode->visited = false;
    newNode->letters.resize(26, 0);
    for (size_t i = 0; i < input.size(); i++) {
      newNode->letters[input[i] - 'A'] = true;
    }
    Edge* edge = new Edge();
    edge->to = newNode;
    edge->from = nodes[0];
    edge->original = 1;
    edge->residual = 0;
    nodes[0]->edges.push_back(edge);
    numDice++;
    // cout << "Dice created" << endl;
    nodes.push_back(newNode);

  } else if (type == SINK) {
    Node* newNode = new Node();
    newNode->id = nodes.size();
    newNode->type = type;
    for (size_t j = numDice + 1; j < nodes.size(); j++) {
      Edge* toSink = new Edge();
      toSink->from = nodes[j];
      toSink->to = newNode;
      toSink->original = 1;
      toSink->residual = 0;
      nodes[j]->edges.push_back(toSink);
    }
    nodes.push_back(newNode);

  } else {
    cout << "Please provide a type" << endl;
  }
}