//
// Created by Aria Pahlavan on 10/20/17.
//
#include <iostream>
#include "Stream.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace std;

int main() {

    vector<int> val;
    srand (time(NULL));

    for (int i = 0; i < 100000; ++i) {
        i = rand() % 500 + 1;
        val.emplace_back(i);
    }


    cout <<
         Stream::make<int>(val)
                 .remove([](int x) { return x == 2; })
                 .filter([](int x){ return x<5; })
                 .flatmap<string>([](int x) { return std::to_string(x); })
                 .reduce([](string a, string b) { return a + " " + b; }, "{")
                 .get()
         <<
            " }"
         << endl;

    cout << "------------" << endl;


    return 0;
}