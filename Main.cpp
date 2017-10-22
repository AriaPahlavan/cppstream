//
// Created by Aria Pahlavan on 10/20/17.
//
#include <iostream>
#include "VectorStream.h"
#include "Stream.h"

using namespace std;

int main() {

    vector<string> ints = {"", "one ", "two ", "three ", "four ", "five "};
    auto val = {1, 2, 3, 4, 5};

    auto func = [](int x) -> int { return 2 * x; };

    cout <<
    Stream::make<int>(val)
            .remove([](int x){ return x == 2; })
            .filter([](int x){ return x < 5; })
            .flatmap<string>([&ints](int x) { return ints[x]; })
            .reduce([](string a, string b){ return a + " " + b; }, "")
         << endl;




//            .reduce([](int a, int b) { return a + b; }, 0)

    return 0;
}