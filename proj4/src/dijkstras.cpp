// dijsktras.cpp

// Main Execution
#include <iostream>
#include <map>
#include <vector>
using namespace std;

struct Node {
  int value;
  int previous;
  int lengthToNode;
  bool visited;
};

bool compareNodes(vector<Node>& matrixMap, int current, int lookAt) {
  if (matrixMap[lookAt].visited == true) {
    if (matrixMap[lookAt].lengthToNode + matrixMap[lookAt].value <
        matrixMap[current].lengthToNode) {
      matrixMap[current].lengthToNode =
          matrixMap[lookAt].lengthToNode + matrixMap[lookAt].value;
      matrixMap[current].previous = lookAt;
    }
  } else {
    if (matrixMap[lookAt].lengthToNode == 0) {
      matrixMap[lookAt].lengthToNode =
          matrixMap[current].value + matrixMap[current].lengthToNode;
      matrixMap[lookAt].previous = current;
      return true;
    } else {
      if (matrixMap[lookAt].lengthToNode >
          matrixMap[current].value + matrixMap[current].lengthToNode) {
        matrixMap[lookAt].lengthToNode =
            matrixMap[current].value + matrixMap[current].lengthToNode;
        matrixMap[lookAt].previous = current;
      }
    }
  }
  return false;
}
void dijkstras(vector<Node> matrixMap, int start, int end, int size) {
  int current = start;
  matrixMap[start].lengthToNode = 0;
  matrixMap[start].visited = true;
  multimap<int, int> mmap;
  // int k = 0;
  mmap.insert(pair<int, int>(matrixMap[start].value, start));
  while (true) {
    mmap.erase(mmap.begin());
    int lookAt = 0;
    // check below
    if ((current + size) < (size * size)) {
      lookAt = current + size;
      if (compareNodes(matrixMap, current, lookAt)) {
        mmap.insert(pair<int, int>(matrixMap[lookAt].lengthToNode, lookAt));
      };
    }
    // check to the right
    if ((current + 1) % size > 0) {
      lookAt = current + 1;
      if (compareNodes(matrixMap, current, lookAt)) {
        mmap.insert(pair<int, int>(matrixMap[lookAt].lengthToNode, lookAt));
      };
    }
    // Check left: current - 1
    if (current % size != 0) {
      lookAt = current - 1;
      if (compareNodes(matrixMap, current, lookAt)) {
        mmap.insert(pair<int, int>(matrixMap[lookAt].lengthToNode, lookAt));
      };
    }
    // Check above
    if ((current - size) > -1) {
      lookAt = current - size;
      if (compareNodes(matrixMap, current, lookAt) == false) {
        mmap.insert(pair<int, int>(matrixMap[lookAt].lengthToNode, lookAt));
      };
    }
    matrixMap[current].visited = true;
    current = mmap.begin()->second;
    if (mmap.size() == 0) {
      break;
    }
  }

  int totalLength = matrixMap[end].lengthToNode;
  vector<int> answers;
  current = end;
  while (current != start) {
    answers.push_back(current);
    current = matrixMap[current].previous;
  }
  answers.push_back(start);
  cout << totalLength << endl;
  for (int i = answers.size() - 1; i > -1; i--) {
    cout << (answers[i] / size) << " " << (answers[i] % size) << endl;
  }
  cout << endl;
}

int main(int argc, char* argv[]) {
  int nTypes;
  while (cin >> nTypes) {
    map<char, int> moveTypes;
    int val;
    char name;
    vector<Node> matrixMap;

    for (int i = 0; i < nTypes; i++) {
      cin >> name;
      cin >> val;
      moveTypes.insert({name, val});
    }
    int sizeY;
    int sizeX;
    cin >> sizeY;
    cin >> sizeX;
    char mapSpot;
    for (int i = 0; i < sizeY * sizeX; i++) {
      cin >> mapSpot;
      Node val;
      val.visited = false;
      val.lengthToNode = 0;
      val.value = moveTypes[mapSpot];
      matrixMap.push_back(val);
    }
    int startX, startY, endX, endY = 0;
    cin >> startX;
    cin >> startY;
    cin >> endX;
    cin >> endY;
    int start = startX * sizeX + startY;
    int end = endX * sizeX + endY;
    dijkstras(matrixMap, start, end, sizeX);
  }
  return 0;
}
