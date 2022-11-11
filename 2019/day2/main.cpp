#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class IntCodeInterpreter {
public:
  // Parameters 
  int counter;
  vector<int> opList;

  // Methods
  IntCodeInterpreter(vector<int> &code) {
    counter = 0;
    opList = vector<int>(code);
  }

  bool stepCounter(int step = 4) {
    counter += step;
    return counter < opList.size();
  }

  int runCurrent() {
    // returns error code
    // 0: everything is fine
    // 1: termination state
    // 2: error
    int op = opList[counter];
    switch (op) {
      case 1:
        opList[opList[counter + 3]] = opList[opList[counter + 1]] + opList[opList[counter + 2]];
        stepCounter(4);
        break;
      case 2:
        opList[opList[counter + 3]] = opList[opList[counter + 1]] * opList[opList[counter + 2]];
        stepCounter(4);
        break;
      case 99:
        return 1;
      default:
        return 2;
    }
    return 0;
  }
};

std::ostream & operator<<(std::ostream & Str, IntCodeInterpreter const & v) { 
  // print something from v to str, e.g: Str << v.getX();
  for (int i = 0; i < v.opList.size(); i++) {
      cout << v.opList[i] << " ";
  }
  return Str;
}

vector<int> parseInput(string filename) {
  ifstream f(filename);
  string opStr;
  vector<int> result;
  while (getline(f, opStr, ',')) {
    result.push_back(stoi(opStr));
  }
  return result;
}

int part1() {
  vector<int> input = parseInput("input.txt");
  IntCodeInterpreter interp(input);
  interp.opList[1] = 12;
  interp.opList[2] = 2;
  //cout << "Start: " << interp << endl;
  while (!interp.runCurrent()) {};
  return interp.opList[0];
}

int part2() {
  vector<int> input = parseInput("input.txt");
  for (int noun = 0; noun < 100; noun++) {
    for (int verb = 0; verb < 100; verb++) {
      IntCodeInterpreter interp(input);
      interp.opList[1] = noun;
      interp.opList[2] = verb;
      while (!interp.runCurrent()) {};
      int output = interp.opList[0];
      if (output == 19690720) {
        return 100 * noun + verb;
      }
    }
  }
  return 0;
}

int main() {
  int answerPart1 = part1();
  cout << "Part 1: " << answerPart1 << endl;
  int answerPart2 = part2();
  cout << "Part 2: " << answerPart2 << endl;
  return 0;
}