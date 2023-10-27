#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
using namespace std;

void ejercicio_1() {
    int n = 10000000;
    vector<int> vd(n);
    random_device rd;
    uniform_int_distribution<int> ud(1, 100);
    for (auto& item: vd) item = ud(rd);
    // ----------START-------------
    auto start = chrono::high_resolution_clock::now();
    auto it = max_element(begin(vd), end(vd));
    auto finish = chrono::high_resolution_clock::now();
    // ----------FINISH-------------
    cout << chrono::duration_cast<chrono::microseconds>(finish-start).count() << endl;
    cout << *it << endl;
}

void buscar_max(int& result, vector<int>::iterator first, vector<int>::iterator last) {
    auto it = max_element(first, last);
    result = *it;
}

void ejercicio_2() {
    int n = 10000000;
    vector<int> vd(n);
    random_device rd;
    uniform_int_distribution<int> ud(1, 100);
    for (auto &item: vd) item = ud(rd);
    // ----------START-------------
    auto start = chrono::high_resolution_clock::now();
    auto nt = thread::hardware_concurrency();
    vector<jthread> vt(nt);
    vector<int> vr(nt);
    auto range = n / nt;
    auto first = begin(vd);
    for (int i = 0; i < nt - 1; ++i) {
        auto last = next(first, range);
        vt[i] = jthread(buscar_max, ref(vr[i]), first, last);
        first = last;
    }
    vt.back() = jthread(buscar_max, ref(vr.back()), first, end(vd));

    for (auto& t: vt) t.join();
    auto it = max_element(begin(vr), end(vr));
    auto finish = chrono::high_resolution_clock::now();
    // ----------FINISH-------------
    cout << chrono::duration_cast<chrono::microseconds>(finish-start).count() << endl;
    cout << *it << endl;
}
int main() {
    ejercicio_1();
    ejercicio_2();
    return 0;
}
