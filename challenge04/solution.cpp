/* Challenge 04: Graph Paths
 * File: solution.cpp
 * Name: Eli Fisk.
 * Date: 2/8/2025
 *
 * Description:
 * This program will take a series of NEDGES pairs of nodes and store them using an
 * adjacency list to store the relation between nodes. You can then ask it if there are
 * NPATHS specific paths and it will use a DFS traversal to check if each path exists or 
 * not. It will then print the result;
 */

#include <iostream>
#include <map>
#include <vector>
using namespace std;

// Custom Node struct.
struct NodeInfo {
  vector<char> edges;
  bool checked;
};

// Recursive function to check if achieve DFS checking.
bool check_children(map<char, NodeInfo> answers, char first, char second) {

  // Base case.
  if (answers[first].checked == true) {
    return false;
  }
  answers[first].checked = true;
  for (size_t i = 0; i < answers[first].edges.size(); i++) {
    if (answers[first].edges[i] == second) {
      return true;
    } else {
      if (check_children(answers, answers[first].edges[i], second)) {
        return true;
      }
    }
  }
  return false;
}

int main(int argc, char *argv[]) {
  int kth = 1;
  bool goAgain = false;
  int input;
  char first, second;

  while (true) {
    cin >> input;
    if (!input) {
      break;
    }
    if(goAgain) {
      cout << endl;
    }
    goAgain = true;
    
    // Use key = the node and value = edges it is connect to and whether or not it has
    // been checked.
    map<char, NodeInfo> answers;

    // Populate map with the NEDGES
    for (int i = 0; i < input; i++) {
      cin >> first;
      cin >> second;
      if (answers.find(first) != answers.end()) {
        answers[first].edges.push_back(second);
      } else {
        NodeInfo val;
        val.checked = false;
        val.edges.push_back(second);
        answers.insert({first, val});
      }
    }

    cin >> input;

    for (int i = 0; i < input; i++) {
      cin >> first;
      cin >> second;

      // Check it there is a valid path, print the result.
      if (check_children(answers, first, second)) {
        cout << "In Graph " << kth << " there is a path from " << first
             << " to " << second << endl;
      } else {
        cout << "In Graph " << kth << " there is no path from " << first
             << " to " << second << endl;
      }
    }
    kth++;
    input = 0;
  }
  return (0);
}
