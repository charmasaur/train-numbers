#include <iostream>
#include <algorithm>
#include <cstdio>
#include <list>
#include <set>
#include <cstring>
#include <stack>
#include <cassert>
using namespace std;

#define NUM_DIGS 4
#define PLACEHOLDER (1 << 31)
#define TARGET 10

class Binary {
  public:
    virtual bool works(int a, int b) = 0;
    virtual int doIt(int a, int b) = 0;
};

class Plus : public Binary {
  public:
    bool works(int a, int b) { return true; }
    int doIt(int a, int b) { return a + b; }
};

class Minus : public Binary {
  public:
    bool works(int a, int b) { return true; }
    int doIt(int a, int b) { return a - b; }
};

class Times : public Binary {
  public:
    bool works(int a, int b) { return true; }
    int doIt(int a, int b) { return a * b; }
};

class Divide : public Binary {
  public:
    bool works(int a, int b) { return b != 0 && a % b == 0; }
    int doIt(int a, int b) { return a/b; }
};

class Power : public Binary {
  public:
    bool works(int a, int b) {
      if (b < 0) {
        return false;
      }
      if (a > 1 && b > 10) {
        return false;
      }
      return true;
    }
    int doIt(int a, int b) {
      int ret = 1;
      for (int i = 0; i < b; i++) {
        ret *= a;
      }
      return ret;
    }
};

list<Binary*> binaries;
stack<set<int>*> dem;

void init(bool exp) {
  binaries.push_back(new Plus());
  binaries.push_back(new Minus());
  binaries.push_back(new Times());
  binaries.push_back(new Divide());
  if (exp) {
    binaries.push_back(new Power());
  }
}

bool check(int* thingos) {
  // See whether it's valid.
  int ct = 0;
  for (int i = 0; i < 2 * NUM_DIGS - 1; ++i) {
    if (thingos[i] == PLACEHOLDER) {
      --ct;
    } else {
      ++ct;
    }
    if (ct == 0) {
      return false;
    }
  }
  if (ct != 1) {
    return false;
  }

  for (int i = 0; i < 2 * NUM_DIGS - 1; ++i) {
    if (thingos[i] == PLACEHOLDER) {
      // Pop the last two and apply operation.
      set<int>* aset = dem.top();
      dem.pop();
      set<int>* bset = dem.top();
      dem.pop();
      set<int>* newset = new set<int>();
      for (set<int>::iterator a = aset->begin(); a != aset->end(); ++a) {
        for (set<int>::iterator b = bset->begin(); b != bset->end(); ++b) {
          for(list<Binary*>::iterator o = binaries.begin(); o != binaries.end(); ++o) {
            if ((*o)->works(*a, *b)) {
              newset->insert((*o)->doIt(*a, *b));
            }
          }
        }
      }
      // Push the result.
      dem.push(newset);
      delete aset;
      delete bset;
    } else {
      dem.push(new set<int>(thingos + i, thingos + i + 1));
    }
  }
  assert(!dem.empty());
  set<int>* res = dem.top();
  dem.pop();
  assert(dem.empty());
  bool works = false;
  for (set<int>::iterator a = res->begin(); a != res->end(); ++a) {
    if (*a == TARGET) {
      works = true;
      break;
    }
  }
  delete res;
  return works;
}

bool can_do_it(int* digs) {
  int thingos[2 * NUM_DIGS - 1];
  for (int i = 0; i < NUM_DIGS; ++i) {
    thingos[i] = digs[i];
  }
  for (int i = NUM_DIGS; i < 2 * NUM_DIGS - 1; ++i) {
    thingos[i] = PLACEHOLDER;
  }

  sort(thingos, thingos + 2 * NUM_DIGS - 1);
  do {
    if (check(thingos)) {
      return true;
    }
  } while (next_permutation(thingos, thingos + 2 * NUM_DIGS - 1));
  return false;
}

int main(int argc, char ** argv) {
  if (argc < 2) {
    cout << "Usage: foo number [-e]" << endl;
    return 0;
  }
  if (strlen(argv[1]) != NUM_DIGS) {
    cout << "number must be " << NUM_DIGS << " digits" << endl;
    return 0;
  }
  bool exp = false;
  if (argc > 2) {
    if (strcmp(argv[2], "-e") == 0) {
      exp = true;
    } else {
      cout << "Unrecognised switch: " << argv[2] << endl;
      return 0;
    }
  }

  init(exp);

  int digs[NUM_DIGS];
  for (int i = 0; i < NUM_DIGS; ++i) {
    digs[i] = argv[1][i] - '0';
  }
  cout << (can_do_it(digs) ? "1" : "0") << endl;
  return 0;
}
