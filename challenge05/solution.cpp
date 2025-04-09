// Challenge 5: Minimum Spanning Tree

// Main Execution

#include <iostream>
#include <map>
#include <set>
#include <unordered_set>
#include <vector>
using namespace std;

struct Node {
  char origin;
  char next;
};
int main(int argc, char *argv[]) {
  int numN = 0;
  bool goAgain = false;
  while (cin >> numN) {
    if(goAgain) {
      cout << endl;
    }
    goAgain = true;
    int length = 0;
    vector<int> matrix;
    set<string> minSpanTree;
    unordered_set<char> visitedNodes;
    visitedNodes.insert('A');
    // Get input;
    int input;
    for (int i = 0; i < (numN * numN); i++) {
      cin >> input;
      matrix.push_back(input);
    }

    multimap<int, Node> mmap;
    int i = 0;
    while (true) {
      // Add the new set of nodes.
      for (int j = 0; j < numN; j++) {
        if (matrix[i * numN + j] > 0 &&
            visitedNodes.find(j + 65) == visitedNodes.end()) {
          Node val;
          val.origin = i + 65;
          val.next = j + 65;
          mmap.insert(pair<int, Node>(matrix[(i * numN) + j], val));
        }
      }

      // Remove outdated nodes.
      multimap<int, Node>::iterator itr;
      set<char> available;
      for (itr = mmap.begin(); itr != mmap.end();) {
        if (available.find(itr->second.next) == available.end()) {
          available.insert(itr->second.next);
          ++itr;
        } else {
          itr = mmap.erase(itr);
        }
      }
      available.clear();
      length += mmap.begin()->first;
      char why = mmap.begin()->second.origin;
      char why2 = mmap.begin()->second.next;
      
      string val;
      if (why < why2) {
        val.push_back(why);
        val.push_back(why2);
      } else {
        val.push_back(why2);
        val.push_back(why);
      }
      minSpanTree.insert(val);
      visitedNodes.insert(mmap.begin()->second.next);
      mmap.erase(mmap.begin());
      if (int(minSpanTree.size() + 1) == numN) {
        break;
      }
      i = why2 - 65;
    }
    cout << length << endl;
    set<string>::iterator itr;
    for (itr = minSpanTree.begin(); itr != minSpanTree.end(); ++itr) {
      cout << itr->data() << endl;
    }
  }

  return 0;
}
