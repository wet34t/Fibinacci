#include <functional>
#include <queue>
#include <vector>
#include <mutex>
#include <thread>
#include <memory>

class Worker {
private:
    std::function<void()> work_;

public:
    Worker(std::function<void()> work);
    void DoWork();
};

class WorkQueue {
private:
    std::queue<std::shared_ptr<Worker>> workerQueue_;
    std::mutex m;

public:
    void Push(std::shared_ptr<Worker> w);
    std::shared_ptr<Worker> Pop();
    int Size();
};

class WorkerThread {
    std::shared_ptr<WorkQueue> workerQueue_;
    bool work_;
    std::thread thread_;

public:
    WorkerThread(std::shared_ptr<WorkQueue> workerQueue);
    void DoWork();
    void Join();
};

class WorkManager {
private:
    std::shared_ptr<WorkQueue> workerQueue_;
    std::vector<std::unique_ptr<WorkerThread>> threads_;
    const int MAXWORKERS = 5;

public:
    WorkManager();
    ~WorkManager();
    void AddWork(std::function<void()> work);
    void DoWork();
    void Done();
};
