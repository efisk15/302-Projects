#include "graph.h"

#include <iostream>
#include <map>
#include <queue>
using namespace std;

Graph::Graph() {
  nodes.resize(0);
  spellingIds.resize(0);
  numDice = 0;
}

bool Graph::spellable() {
  while (BFS()) {
    Node* current;
    Node* pastCurrent;
    current = nodes[nodes.size() - 1];

    while (current->type != SOURCE) {
      pastCurrent = current;
      if (current->backEdge->original == 0) {
        current->backEdge->original = 1;
        current->backEdge->residual = 0;
        current->backEdge->reverse->original = 0;
        current->backEdge->reverse->residual = 1;
      } else {
        current->backEdge->original = 0;
        current->backEdge->residual = 1;
        current->backEdge->reverse->original = 1;
        current->backEdge->reverse->residual = 0;
      }
      current = current->backEdge->to;
      for (size_t j = 0; j < nodes.size(); j++) {
        nodes[j]->visited = false;
      }
      pastCurrent->visited = false;
    }
  };
  bool isScoreble = true;
  for (size_t i = 0; i < nodes[nodes.size() - 1]->edges.size(); i++) {
    if (nodes[nodes.size() - 1]->edges[i]->original != 1) {
      isScoreble = false;
    }
  }
  if (isScoreble) {
    for (size_t i = 0; i < nodes[nodes.size() - 1]->edges.size(); i++) {
      Node* current = nodes[nodes.size() - 1]->edges[i]->to;
      for (size_t j = 0; j < current->edges.size(); j++) {
        if (current->edges[j]->original == 1) {
          spellingIds.push_back(current->edges[j]->to->id);
          break;
        }
      }
    }
  }
  return isScoreble;
}
bool Graph::BFS() {
  Node* current = nodes[0];
  queue<Edge*> options;
  while (true) {
    current->visited = true;
    for (size_t i = 0; i < current->edges.size(); i++) {
      if (current->edges[i]->original == 1 &&
          current->edges[i]->to->visited != true) {
        options.push(current->edges[i]);
      }
    }
    if (options.size() == 0) {
      return false;
    }
    Edge* goTo = options.front();
    options.pop();

    goTo->to->backEdge = goTo->reverse;
    current = goTo->to;
    if (current->type == SINK) {
      return true;
    }
  }
  return false;
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

  for (size_t j = 0; j < nodes[0]->edges.size(); j++) {
    nodes[0]->edges[j]->original = 1;
    nodes[0]->edges[j]->residual = 0;
    nodes[0]->edges[j]->reverse->original = 0;
    nodes[0]->edges[j]->reverse->residual = 1;
  }
  spellingIds.resize(0);
  for (size_t j = 0; j < nodes.size(); j++) {
    nodes[j]->visited = false;
  }
}
void Graph::createNode(string input, NODE_TYPE type) {
  if (type == SOURCE) {
    Node* newNode = new Node();
    newNode->id = 0;
    newNode->type = type;
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
          backEdge->to = nodes[j];
          backEdge->from = newNode;
          edge->original = 1;
          edge->residual = 0;
          backEdge->original = 0;
          backEdge->residual = 1;
          edge->reverse = backEdge;
          backEdge->reverse = edge;
          nodes[j]->edges.push_back(edge);
          newNode->edges.push_back(backEdge);
        }
      }
      newNode->visited = false;
      nodes.push_back(newNode);
      startingId++;
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
    Edge* edgeBack = new Edge();
    edge->to = newNode;
    edge->from = nodes[0];
    edgeBack->from = newNode;
    edgeBack->to = nodes[0];
    edge->original = 1;
    edge->residual = 0;
    edgeBack->original = 0;
    edgeBack->residual = 1;
    edge->reverse = edgeBack;
    edgeBack->reverse = edge;
    nodes[0]->edges.push_back(edge);
    newNode->edges.push_back(edgeBack);
    numDice++;
    nodes.push_back(newNode);

  } else if (type == SINK) {
    Node* newNode = new Node();
    newNode->id = nodes.size();
    newNode->type = type;
    for (size_t j = numDice + 1; j < nodes.size(); j++) {
      Edge* toSink = new Edge();
      Edge* fromSink = new Edge();
      toSink->from = nodes[j];
      fromSink->from = newNode;
      toSink->to = newNode;
      fromSink->to = nodes[j];
      toSink->original = 1;
      toSink->residual = 0;
      fromSink->original = 0;
      fromSink->residual = 1;
      toSink->reverse = fromSink;
      fromSink->reverse = toSink;
      nodes[j]->edges.push_back(toSink);
      newNode->edges.push_back(fromSink);
    }
    nodes.push_back(newNode);

  }
}