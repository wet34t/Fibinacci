#include <time.h>
#include <iostream>
#include "fibi.hpp"

using namespace std;

struct logger_t {
    mutex m;
    ostream &out_;

    logger_t(ostream &out) :
    out_(out) { }

    template <typename T>
    logger_t &operator<<(T message) {
        lock_guard<mutex> l(m);
        out_ << message;
        return *this;
    }
};

struct logger_t logger(cout);

template <typename T>
void printArray(T arr, int size) {
        for (int i=0; i<size; i++) {
                logger << arr[i] << " ";
        }
        logger << "\n";
}

void printType(FibiType t) {
    switch (t) {
        case 0:
            logger << "Fibi\n";
            break;
        case 1:
            logger << "FibiDp\n";
            break;
        case 2:
            logger << "FibiAsync\n";
            break;
        case 3:
            logger << "FibiThreads\n";
            break;
        case 4:
            logger << "FibiWorker\n";
            break;
        case 5:
            logger << "FibiWorkerQueue\n";
            break;
        default:
            break;
    }
}

template <typename T>
void calculateFibi(T *calc, int n) {
        long *results = new long[n];
        time_t now = time(0);
        logger << "Starting for type ";
    printType(calc->GetType());
        calc->CalcFibiN(n, results);
        printArray(results, n);
        logger << "Done, took " << time(0)-now << " for type ";
    printType(calc->GetType());
        delete [] results;
}

int main() {
        Fibi *fibi = new Fibi();
        Fibi *fibiAsync = new FibiAsync();
        Fibi *fibiThreads = new FibiThreads();
    Fibi *fibiWorker = new FibiWorker();
    Fibi *fibiWorkerQueue = new FibiWorkerQueue();
    FibiDp *fibiDp = new FibiDp();
        int n = 49;

    auto f1 = std::async(&calculateFibi<FibiDp>, fibiDp, n);
        auto f2 = std::async(&calculateFibi<Fibi>, fibiAsync, n);
        auto f3 = std::async(&calculateFibi<Fibi>, fibiThreads, n);
    auto f4 = std::async(&calculateFibi<Fibi>, fibiWorker, n);
    auto f5 = std::async(&calculateFibi<Fibi>, fibiWorkerQueue, n);
    calculateFibi<Fibi>(fibi, n);

        f1.get();
        f2.get();
        f3.get();
    f4.get();
    f5.get();

        delete fibi;
        delete fibiAsync;
    delete fibiThreads;
    delete fibiWorker;
    delete fibiWorkerQueue;
    delete fibiDp;
}
