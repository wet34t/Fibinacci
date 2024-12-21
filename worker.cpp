#include "worker.hpp"

Worker::Worker(std::function<void()> work) {
    work_ = std::move(work);
}

void Worker::DoWork() {
    work_();
}

void WorkQueue::Push(std::shared_ptr<Worker> w) {
    std::lock_guard<std::mutex> l(m);
    workerQueue_.emplace(w);
}

std::shared_ptr<Worker> WorkQueue::Pop() {
    std::unique_lock<std::mutex> l(m);
    if (!workerQueue_.size()) return nullptr;
    std::shared_ptr<Worker> w = workerQueue_.front();
    workerQueue_.pop();
    return w;
}

int WorkQueue::Size() {
    std::unique_lock<std::mutex> l(m);
    return workerQueue_.size();
}

WorkerThread::WorkerThread(std::shared_ptr<WorkQueue> workerQueue) :
workerQueue_(workerQueue),
work_(true) { }

void WorkerThread::DoWork() {
    std::thread t([&]{
        while (work_) {
            std::shared_ptr<Worker> w = workerQueue_->Pop();
            if (w) {
                w->DoWork();
            }
        }
    });
    thread_ = std::move(t);
}

void WorkerThread::Join() {
    work_ = false;
    thread_.join();
}

WorkManager::WorkManager() {
    workerQueue_ = std::make_shared<WorkQueue>();
}

WorkManager::~WorkManager() {
    while (workerQueue_->Size()) {
    }
    for (auto it = threads_.begin(); it != threads_.end(); it++) {
        (*it)->Join();
    }
    threads_.clear();
}

void WorkManager::AddWork(std::function<void()> work) {
    std::shared_ptr<Worker> w = std::make_shared<Worker>(std::move(work));
    workerQueue_->Push(w);
}

void WorkManager::DoWork() {
    for (int i=0; i<MAXWORKERS; i++) {
        std::unique_ptr<WorkerThread> t = std::make_unique<WorkerThread>(workerQueue_);
        t->DoWork();
        threads_.push_back(std::move(t));
    }
}

void WorkManager::Done() {
    while (workerQueue_->Size()) {
    }
    for (auto it = threads_.begin(); it != threads_.end(); it++) {
        (*it)->Join();
    }
    threads_.clear();
}
