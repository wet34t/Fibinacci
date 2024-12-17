#include <future>
#include "worker.hpp"

enum FibiType {
    fibiNormal,
    fibiDp,
    fibiThreads,
    fibiAsync,
    fibiWorker,
    fibiWorkerQueue
};

class Fibi {
protected:
    FibiType type_;
    static long CalcFibi(int n);

public:
    Fibi() :
    type_(fibiNormal) { }
    virtual ~Fibi() {}
    virtual void CalcFibiN(int n, long *result) const;
    FibiType GetType() const;
};

class FibiDp {
protected:
    std::vector<long> dp_;
    FibiType type_;
    long CalcFibi(int n);

public:
    FibiDp();
    virtual ~FibiDp() { }
    virtual void CalcFibiN(int n, long *result);
    FibiType GetType() const;
};

class FibiThreads : public Fibi {
private:
    void CalcFibiEven(int n, long *result) const;

public:
    FibiThreads() {
        type_ = fibiThreads;
    }
    virtual void CalcFibiN(int n, long *result) const;
};

class FibiAsync : public Fibi {
private:
    void CalcFibiEven(int n, long *result) const;

public:
    FibiAsync() {
        type_ = fibiAsync;
    }
    virtual void CalcFibiN(int n, long *result) const;
};

class FibiWorker : public Fibi {
public:
    FibiWorker() {
        type_ = fibiWorker;
    }
    virtual void CalcFibiN(int n, long *result) const;
};

class FibiWorkerQueue : public Fibi {
private:
    std::unique_ptr<WorkManager> manager_;

public:
    FibiWorkerQueue();
    virtual void CalcFibiN(int n, long *result) const;
};
