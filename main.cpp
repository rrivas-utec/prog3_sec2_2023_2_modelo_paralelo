#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <future>
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

void producer(promise<string> prm) {
    string msg = "Hola";
    prm.set_value(msg);
}

void consumer(future<string> fut) {
    cout << fut.get();
}

template <typename T, typename Callable, typename ...Params>
future<T> asincrono(Callable&& caller, Params&& ...params) {
    promise<T> prm;
    future<T> fut = prm.get_future();
    thread([&caller, prm](Params...params){
        auto value = caller(std::forward<Params...>(params...));
        prm.set_value(value);
    }, params...).detach();
    return fut;
}

void ejercicio_3() {
    promise<string> prm;
    auto fut = prm.get_future();

    jthread t_producer(producer, std::move(prm));
    jthread t_consumer(consumer, std::move(fut));

}

int multiplicar(int a, int b) {
    return a * b;
}

void ejercicio_4() {
    auto fut = async(multiplicar, 20, 30);
    cout << "Continue";
    this_thread::sleep_for(chrono::milliseconds(1000));
    auto resultado = fut.get();
    cout << resultado << endl;
}

void ejercicio_5() {
    int n = 10000000;
    vector<int> vd(n);
    random_device rd;
    uniform_int_distribution<int> ud(1, 100);
    for (auto &item: vd) item = ud(rd);
    // ----------START-------------
    auto start = chrono::high_resolution_clock::now();
    auto nt = thread::hardware_concurrency();
    vector<future<int>> vf(nt);
    auto range = n / nt;
    auto first = begin(vd);
    for (int i = 0; i < nt - 1; ++i) {
        auto last = next(first, range);
        vf[i] = async([first, last] {
            return *max_element(first, last);
        });
        first = last;
    }
    vf.back() = async([first, last=end(vd)] {
        return *max_element(first, last);
    });
    int max_value = vd.front();
    for (auto& f: vf)
        max_value = max(max_value, f.get());
    auto finish = chrono::high_resolution_clock::now();
    // ----------FINISH-------------
    cout << chrono::duration_cast<chrono::microseconds>(finish-start).count() << endl;
    cout << max_value << endl;
}



int main() {
//    ejercicio_1();
//    ejercicio_2();
//    ejercicio_3();
//    ejercicio_4();
    ejercicio_5();
    return 0;
}
