#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

int fuelCalculator(int mass, bool recurse) {
  int result = mass / 3 - 2;
  if (result <= 0) {
    return 0;
  }
  if (recurse) {
    result += fuelCalculator(result, recurse);
  }
  return result;
}

int part1() {
  fstream inputFile("input.txt");
  string line;

  int sum = 0;
  while (getline(inputFile, line)) {
    int val = stoi(line);
    sum += fuelCalculator(val, false);
  }
  
  inputFile.close();
  return sum;
}

int part2() {
  fstream inputFile("input.txt");
  string line;

  int sum = 0;
  while (getline(inputFile, line)) {
    int val = stoi(line);
    sum += fuelCalculator(val, true);
  }
  
  inputFile.close();
  return sum;
}

int main() {
  int part1Answer = part1();
  cout << "Part 1: " << part1Answer << endl;
  int part2Answer = part2();
  cout << "Part 2: " << part2Answer << endl;
  return 0;
}