#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm> 

using namespace std;

int intPow(int base, int exp) {
  int result = 1;
  for (int i = 0; i < exp; i++) {
    result *= base;
  }
  return result;
}

class IntCodeInterpreter {
public:
  // Parameters 
  int counter, inputCounter;
  vector<int> opList;
  vector<int> input, output;

  // Methods
  IntCodeInterpreter(vector<int> &code, vector<int> input_) {
    counter = 0;
    inputCounter = 0;
    input = input_;
    opList = vector<int>(code);
  }

  int getValue(int offset, int mode) {
    // Get the value at counter + offset using mode.
    switch (mode) {
      case 0:
        return opList[opList[counter + offset]];
      case 1:
        return opList[counter + offset];
      default:
        cout << "Invalid mode encountered in getValue: " << mode << endl;
        return -1;
    }
  }

  vector<int> getModes(int op, int n) {
    // Return a vector with n modes
    vector<int> result;
    for (int i = 0; i < n; i++) {
      int temp = op / intPow(10, i+2); // 1234 -> 12
      temp = temp - 10 * (temp / 10); // 12 -> 2
      result.push_back(temp);
    }
    return result;
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
    int val1, val2, index;
    vector<int> modes;
    int op = opList[counter];
    int opCode = op - 100 * (op / 100); // get the two last digits
    switch (opCode) {
      case 1:
        modes = getModes(op, 2);
        opList[getValue(3, 1)] = getValue(1, modes[0]) + getValue(2, modes[1]);
        //opList[opList[counter + 3]] = opList[opList[counter + 1]] + opList[opList[counter + 2]];
        stepCounter(4);
        break;
      case 2:
        modes = getModes(op, 2);
        opList[getValue(3, 1)] = getValue(1, modes[0]) * getValue(2, modes[1]);
        //opList[opList[counter + 3]] = opList[opList[counter + 1]] * opList[opList[counter + 2]];
        stepCounter(4);
        break;
      case 3:
        opList[getValue(1, 1)] = input[inputCounter];
        inputCounter += 1;
        stepCounter(2);
        break;
      case 4:
        modes = getModes(op, 1);
        val1 = getValue(1, modes[0]);
        cout << "$ " << val1 << endl;
        output.push_back(val1);
        stepCounter(2);
        break;
      case 5:
        modes = getModes(op, 2);
        if (getValue(1, modes[0])) {
          counter = getValue(2, modes[1]);
        } else {
          stepCounter(3);
        }
        break;
      case 6:
        modes = getModes(op, 2);
        if (!getValue(1, modes[0])) {
          counter = getValue(2, modes[1]);
        } else {
          stepCounter(3);
        }
        break;
      case 7:
        modes = getModes(op, 2);
        val1 = getValue(1, modes[0]);
        val2 = getValue(2, modes[1]);
        index = getValue(3, 1);
        if (val1 < val2) {
          opList[index] = 1;
        } else {
          opList[index] = 0;
        }
        stepCounter(4);
        break;
      case 8:
        modes = getModes(op, 2);
        val1 = getValue(1, modes[0]);
        val2 = getValue(2, modes[1]);
        index = getValue(3, 1);
        if (val1 == val2) {
          opList[index] = 1;
        } else {
          opList[index] = 0;
        }
        stepCounter(4);
        break;
      case 99:
        return 1;
      default:
        return 2;
    }
    return 0;
  }

  void runAll() {
    int errorCode;
    while (!(errorCode = runCurrent())) {
      //cout << "Counter: " << counter << endl;
      for (int i = 0; i < opList.size(); i++) {
        //cout << opList[i] << " ";
      }
      //cout << endl;
    };
    if (errorCode != 1) {
      cout << "Error code returned!" << endl;
    }
  }
};

std::ostream & operator<<(std::ostream & Str, IntCodeInterpreter const & v) { 
  // print something from v to str, e.g: Str << v.getX();
  for (int i = 0; i < v.opList.size(); i++) {
      cout << v.opList[i] << " ";
  }
  return Str;
}

std::ostream & operator<<(std::ostream & Str, vector<int> const & v) { 
  // print something from v to str, e.g: Str << v.getX();
  cout << "{";
  for (int i = 0; i < v.size(); i++) {
      cout << v[i] << " ";
  }
  cout << "}";
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

void part1() {
  vector<int> input = parseInput("input.txt");
  vector<int> phases = {0, 1, 2, 3, 4};
  int maxOutput = 0;
  do {
    int inp = 0;
    for (int i = 0; i < phases.size(); i++) {
      IntCodeInterpreter interp(input, {phases[i], inp});
      interp.runAll();
      if (!interp.output.empty())
        inp = interp.output.back();
      else
        cout << "Error occured, output is empty!" << endl;
    }
    maxOutput = max(maxOutput, inp);
  } while (next_permutation(phases.begin(), phases.end()));

  cout << "Part 1: " << maxOutput << endl;
}

void part2() {
  vector<int> input = parseInput("inputTest.txt");
  vector<int> phases = {5, 6, 7, 8, 9};
  int maxOutput = 0;
  // it should return on input instead of on halt?
  do {
    int inp = 0;
    for (int i = 0; i < phases.size(); i++) {
      IntCodeInterpreter interp(input, {phases[i], inp});
      interp.runAll();
      if (!interp.output.empty())
        inp = interp.output.back();
      else
        cout << "Error occured, output is empty!" << endl;
    }
    maxOutput = max(maxOutput, inp);
  } while (next_permutation(phases.begin(), phases.end()));

  cout << "Part 2: " << maxOutput << endl;
}

int main() {
  part1();
  part2();
  return 0;
}