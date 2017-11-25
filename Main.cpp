//
// Created by Aria Pahlavan on 10/20/17.
//
#include <iostream>
#include "Stream.h"
#include <experimental/optional>
#include <chrono>
#include <thread>

using namespace std::experimental;
using namespace std;

void premetiveWay(const vector<int> &vec);
void functionalWay(const vector<int> &vec);
void functionalWay2(const vector<int> &vec);

auto ellapsedTimeMs = [](clock_t start) { return "Time: " + to_string((std::clock() - start) / (double) (CLOCKS_PER_SEC / 1000)) + " ms"; };
auto ellapsedTime = [](clock_t start) { return "Time: " + to_string((std::clock() - start) / (double) (CLOCKS_PER_SEC / 1000 * 1000)) + " s"; };

int main() {
    vector<int> vec;

    srand(time(nullptr));

//  1000000000
    for (int index = 0; index < 100000; index++) {
        int random_integer = rand() % 10 + 0;
        vec.push_back(random_integer);
    }

    cout << endl;
    {
        auto start = std::clock();
        cout << "-----------------------------\n";
        premetiveWay(vec);
        std::cout << ellapsedTimeMs(start) << std::endl;
        std::cout << ellapsedTime(start) << std::endl;
        cout << "-----------------------------\n";

    }

    {
        auto start = std::clock();
        cout << "-----------------------------\n";
        functionalWay(vec);
        std::cout << ellapsedTimeMs(start) << std::endl;
        std::cout << ellapsedTime(start) << std::endl;
        cout << "-----------------------------\n";
    }

    {
        auto start = std::clock();
        cout << "-----------------------------\n";
        functionalWay2(vec);
        std::cout << ellapsedTimeMs(start) << std::endl;
        std::cout << ellapsedTime(start) << std::endl;
        cout << "-----------------------------\n";
    }


    return 0;
}

void busyWork(){
    unsigned int sleepTime = 5000;
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
}

void premetiveWay(const vector<int> &vec) {
    cout << "Primitive" << endl;
    auto sum = 0;

    for (auto val : vec) {
        if (val == 0) { continue; }             // remove zeros
        if (val % 2 == 0) { continue; }         // keep odds
        auto valStr = to_string(val);           // to string
//        cout << valStr << " ";                  // print
        auto valInt = stoi(valStr);             // to int
        sum += valInt;                          // sum up
    }
    busyWork();
    cout << endl << "result: " << sum << endl;  // get result
}

void functionalWay(const vector<int> &vec) {
    cout << "Functional 1" << endl;
    auto result = Stream::make<int>(vec)
            .remove([](int x) { return x < 1; })                  // remove zeros
            .filter([](int x) { return x % 2; })                  // keep odds
            .map<string>([](int x) { return to_string(x); })  // to string
//            .forEach([](string x) { cout << x << " "; })          // print
            .map<int>([](string x) { return stoi(x); })        // to int
            .reduce(plus<int>(), 0);                               // sum up

    busyWork();
    cout << endl << "result: " << result.get() << endl; // get result
}

void functionalWay2(const vector<int> &vec) {
    cout << "Functional 2" << endl;
    auto result = Stream::make2<int>(vec)
            .remove([](int x) { return x < 1; })                  // remove zeros
            .filter([](int x) { return x % 2; })                  // keep odds
            .flatmap<string>([](int x) { return to_string(x); })  // to string
//            .forEach([](string x) { cout << x << " "; })          // print
            .flatmap<int>([](string x) { return stoi(x); })        // to int
            .reduce(plus<int>(), 0);                               // sum up

    busyWork();
    cout << endl << "result: " << result << endl;
}