#include <iostream>
#include <string>
#include "helper.h"

using std::cout; using std::endl; using std::string;

int main() {
    Cube cube;
    string alg = cube.scramble();
    cout << "Algorithm: " << alg << endl;
    cout << cube << endl;
}