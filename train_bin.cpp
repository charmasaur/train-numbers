#include <iostream>
#include <algorithm>
#include <cstdio>
#include <list>
#include <set>
#include <cstring>
#include <stack>
#include <cassert>
#include <cmath>
using namespace std;

#define TARGET 10
#define EPSILON 0.000001

class Binary {
  public:
    virtual bool works(float a, float b) = 0;
    virtual float doIt(float a, float b) = 0;
};

class Plus : public Binary {
  public:
    bool works(float a, float b) { return true; }
    float doIt(float a, float b) { return a + b; }
};

class Minus : public Binary {
  public:
    bool works(float a, float b) { return true; }
    float doIt(float a, float b) { return a - b; }
};

class Times : public Binary {
  public:
    bool works(float a, float b) { return true; }
    float doIt(float a, float b) { return a * b; }
};

class Divide : public Binary {
  public:
    bool works(float a, float b) { return abs(b) > EPSILON; }
    float doIt(float a, float b) { return a/b; }
};

class Power : public Binary {
  public:
    bool works(float a, float b) { return true; }
    float doIt(float a, float b) { return pow(a, b); }
};

class Reverse : public Binary {
  private:
    Binary *original;
  public:
    Reverse(Binary *o) : original(o) {}
    bool works(float a, float b) { return original->works(b, a); }
    float doIt(float a, float b) { return original->doIt(b, a); }
};

list<Binary*> binaries;

void init(bool exp) {
  binaries.push_back(new Plus());
  binaries.push_back(new Minus());
  binaries.push_back(new Reverse(new Minus()));
  binaries.push_back(new Times());
  binaries.push_back(new Divide());
  binaries.push_back(new Reverse(new Divide()));
  if (exp) {
    binaries.push_back(new Power());
    binaries.push_back(new Reverse(new Power()));
  }
}

bool check(list<float>::iterator begin, list<float>::iterator end) {
  if (next(begin) == end) {
    return *begin == TARGET;
  }
  for (list<float>::iterator i = begin; i != end; ++i) {
    for (list<float>::iterator j = next(i); j != end; ++j) {
      for(list<Binary*>::iterator o = binaries.begin(); o != binaries.end(); ++o) {
        if (!(*o)->works(*i, *j)) {
          continue;
        }
        float res = (*o)->doIt(*i, *j);
        float oldi = *i;
        float oldj = *j;
        *i = *begin;
        *j = res;
        if (check(next(begin), end)) {
          return true;
        }
        *i = oldi;
        *j = oldj;
      }
    }
  }
  return false;
}

int main(int argc, char ** argv) {
  if (argc < 2) {
    cout << "Usage: foo number [-e]" << endl;
    return 1;
  }
  bool exp = false;
  if (argc > 2) {
    if (strcmp(argv[2], "-e") == 0) {
      exp = true;
    } else {
      cout << "Unrecognised switch: " << argv[2] << endl;
      return 1;
    }
  }

  init(exp);

  list<float> digs;
  for (unsigned int i = 0; i < strlen(argv[1]); ++i) {
    int dig = argv[1][i] - '0';
    if (dig < 0 || dig >= 10) {
      cout << "Digits must all be digits" << endl;
      return 1;
    }
    digs.push_back(argv[1][i] - '0');
  }
  cout << (check(digs.begin(), digs.end()) ? "1" : "0") << endl;
  return 0;
}
