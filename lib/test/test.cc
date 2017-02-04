#include "iitracer.h"
#include <atomic>
#include <thread>
#include <vector>
#include <algorithm>
#include <iostream>


using namespace std;

void bar() {
    static atomic<int> count {10000};
    if (--count > 0)
        bar();
}

void foo() {
    int x;
    for(x = 0; x < 4; x++)
        bar ();
}

int main (int argc, char *argv[]) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    vector<thread> v;
    for(int i=0; i<3; ++i)
        v.emplace_back(foo);

    foo();
    bar();

    for_each(v.begin(), v.end(), [](thread& f) { f.join(); });

    std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " microseconds" << std::endl;

    SaveTraceData("tmptestbuf.wtf-trace");

    return 0;
}