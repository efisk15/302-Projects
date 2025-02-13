// Challenge 06: Repeated DNA sequences
#include <iostream>
#include <map>
using namespace std;
int main(int argc, char *argv[]) {
    string input;
    while(cin >> input) {
        int i = 0;
        map<string, bool> answers;
        while ((i + 8) < int(input.size())) {
            string val = input.substr(i, 9);
            if(answers.find(val) == answers.end()) {
                answers.insert({val, false});
            } else {
                answers[val] = true;
            }
            i++;
        }
        map<string, bool>::iterator answer;
        for(answer = answers.begin(); answer != answers.end(); ++answer) {
            if(answer->second) {
                cout << answer->first << endl;
            }
        }
        cout << -1 << endl;
    }
    return 0;
}
