#include "fibi.hpp"

long Fibi::CalcFibi(int n) {
        if (n <= 0) return 0;
        else if (n == 1) return 1;
        else if (n == 2) return 1;
        else return CalcFibi(n-1) + CalcFibi(n-2);
}

void Fibi::CalcFibiN(int n, long *result) const {
        for (int i=1; i<=n; i++) {
                result[i-1] = CalcFibi(i);
        }
}

FibiType Fibi::GetType() const {
        return type_;
}

FibiDp::FibiDp() :
type_(fibiDp) {
        dp_.push_back(0);
        dp_.push_back(1);
        dp_.push_back(1);
}

long FibiDp::CalcFibi(int n) {
        if (n < dp_.size()) {
                return dp_[n];
        } else {
                for (int i=dp_.size(); i<=n; i++) {
                        dp_.push_back(dp_[i-1]+dp_[i-2]);
                }
                return dp_[n];
        }
}

void FibiDp::CalcFibiN(int n, long *result) {
        for (int i=1; i<=n; i++) {
                result[i-1] = CalcFibi(i);
        }
}

FibiType FibiDp::GetType() const {
        return type_;
}


void FibiThreads::CalcFibiEven(int n, long *result) const {
        for (int i=2; i<=n; i+=2) {
                result[i-1] = CalcFibi(i);
        }
}

void FibiThreads::CalcFibiN(int n, long *result) const {
        std::thread t(&CalcFibiEven, this, n, result);

        for (int i=1; i<=n; i+=2) {
                result[i-1] = CalcFibi(i);
        }
        t.join();
}

void FibiAsync::CalcFibiEven(int n, long *result) const {
        for (int i=2; i<=n; i+=2) {
                result[i-1] = CalcFibi(i);
        }
}

void FibiAsync::CalcFibiN(int n, long *result) const {
        auto future = std::async(&CalcFibiEven, this, n, result);

        for (int i=1; i<=n; i+=2) {
                result[i-1] = CalcFibi(i);
        }

        future.get();
}

void FibiWorker::CalcFibiN(int n, long *result) const {
        for (int i=1; i<=n; i++) {
                auto work = [this, &result, i] {
                        result[i-1] = CalcFibi(i);
                };
                Worker *w = new Worker(work);
                w->DoWork();
                delete w;
        }
}

FibiWorkerQueue::FibiWorkerQueue() {
        type_ = fibiWorkerQueue;
        manager_ = std::make_unique<WorkManager>();
}

void FibiWorkerQueue::CalcFibiN(int n, long *result) const {
        manager_->DoWork();
        for (int i=1; i<=n; i++) {
                auto work = [this, &result](int j) {
                        result[j-1] = CalcFibi(j);
                };
                manager_->AddWork(std::bind(work, i));
        }
        manager_->Done();
}