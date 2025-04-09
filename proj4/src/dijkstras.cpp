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

void dijkstras(vector<Node> matrixMap, int start, int end, int size) {
  int current, lookAt = 0;
  vector<int> directions = {size, -1, 1, -size};
  multimap<int, int> mmap;
  mmap.insert(pair<int, int>(matrixMap[start].value, start));
  while (true) {
    if (mmap.size() == 0) {
      break;
    }
    current = mmap.begin()->second;
    mmap.erase(mmap.begin());
    if (matrixMap[current].visited == true) {
      continue;
    }
    for (int i = 0; i < 4; i++) {
      lookAt = current + directions[i];
      if (lookAt >= (size * size)) continue;
      if (lookAt % size <= 0) continue;
      if (lookAt <= -1) continue;

      if (matrixMap[lookAt].lengthToNode == 0 ||
          (matrixMap[lookAt].lengthToNode != 0 &&
           matrixMap[lookAt].lengthToNode >
               matrixMap[current].lengthToNode + matrixMap[current].value)) {
        mmap.insert(pair<int, int>(
            matrixMap[current].lengthToNode + matrixMap[current].value,
            lookAt));
        matrixMap[lookAt].lengthToNode =
            matrixMap[current].value + matrixMap[current].lengthToNode;
        matrixMap[lookAt].previous = current;
      }
    }
    matrixMap[current].visited = true;
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
  int nTypes, val, sizeY, sizeX, startX, startY, endX, endY = 0;
  while (cin >> nTypes) {
    map<char, int> moveTypes;
    char name;
    vector<Node> matrixMap;

    for (int i = 0; i < nTypes; i++) {
      cin >> name;
      cin >> val;
      moveTypes.insert({name, val});
    }
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
