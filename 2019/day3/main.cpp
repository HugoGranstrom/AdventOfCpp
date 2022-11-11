#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>
#include <tuple>

using namespace std;

class Coordinate {
public:
  int x, y;
  Coordinate(int x_, int y_) {
    x = x_;
    y = y_;
  }
  int distTo(Coordinate other) {
    return abs(x - other.x) + abs(y - other.y);
  }
  int mag() {
    return abs(x) + abs(y);
  }
};

bool operator==(Coordinate a, Coordinate b) {
  return (a.x == b.x) && (a.y == b.y); 
}

std::ostream & operator<<(std::ostream & Str, Coordinate const & v) { 
  // print something from v to str, e.g: Str << v.getX();
  Str << "(x=" << v.x << ",y=" << v.y << ")" << endl;
  return Str;
}

Coordinate operator*(int x, Coordinate c) {
  return Coordinate(x*c.x, x*c.y);
}

Coordinate operator+(Coordinate c1, Coordinate c2) {
  return Coordinate(c1.x + c2.x, c1.y + c2.y);
}

class Path {
public:
  vector<Coordinate> path;

  void insert(Coordinate &coord) {
    path.push_back(coord);
  }

  bool isIn(Coordinate &coord) {
    for (int i = 0; i < path.size(); i++) {
      if (coord == path[i]) {
        return true;
      }
    }
    return false;
  }

  vector<Coordinate> findIntersections(Path &other) {
    vector<Coordinate> result;
    for (int i = 0; i < path.size(); i++) {
      for (int j = 0; j < other.path.size(); j++) {
        if (path[i] == other.path[j]) {
          result.push_back(path[i]);
        }
      }
    }
    return result;
  }
};

void parsePath(string &line, Path &path) {
  istringstream iss( line );
  Path path1;
  string op;
  Coordinate currentCoord(0, 0);
  while (getline(iss, op, ',')) {
    Coordinate dir(0,0);
    switch (op[0]) {
      case 'U':
        dir = Coordinate(0, 1);
        break;
      case 'D':
        dir = Coordinate(0, -1);
        break;
      case 'R':
        dir = Coordinate(1, 0);
        break;
      case 'L':
        dir = Coordinate(-1, 0);
        break;
      default:
        cout << "Something went wrong... " << op;
    }
    int stepSize = stoi(op.substr(1));
    // Step thorugh stepSize number of unit steps
    for (int i = 1; i <= stepSize; i++) {
      currentCoord = currentCoord + dir;
      path.insert(currentCoord);
    }
  }
}

tuple<int,int> part1And2() {
  fstream inputFile("input.txt");
  string line;
  Path path1, path2;

  getline(inputFile, line);
  parsePath(line, path1);

  getline(inputFile, line);
  parsePath(line, path2);

  vector<Coordinate> intersections = path1.findIntersections(path2);

  int minDist = 10000000;
  for (int i = 0; i < intersections.size(); i++) {
    int dist = intersections[i].mag();
    minDist = min(minDist, dist);
  }
  
  int minSteps = 100000000;
  for (int i = 0; i < intersections.size(); i++) {
    int steps = 0;
    for (int j = 0; j < path1.path.size(); j++) {
      if (intersections[i] == path1.path[j]) {
        steps += j + 1;
        break;
      }
    }
    for (int j = 0; j < path2.path.size(); j++) {
      if (intersections[i] == path2.path[j]) {
        steps += j + 1;
        break;
      }
    }

    minSteps = min(minSteps, steps);
  }


  return tuple<int,int> (minDist, minSteps);
}

int main() {
  int answerPart1, answerPart2;
  tie (answerPart1, answerPart2) = part1And2();
  cout << "Part 1: " << answerPart1 << endl;
  cout << "Part 2: " << answerPart2 << endl;
  return 0;
}