
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <functional>
#include <thread>
#include <list>
#include <mutex>
#include <condition_variable>

using namespace std;

class student{
    private:
        int _a;
        float _b;
    public:
        // 空参构造
        student(){
            std::cout << "empty constructor student ..." << std::endl;
        }

        // 构造函数
        student(int a, float b) : _a(a), _b(b) {
            std::cout << "_a: " << _a << ", _b: " << b << std::endl;
            std::cout << "constructor ..." << std::endl;
        }

        // 拷贝构造函数
        student(const student& stu){
            std::cout << "copy constructor student ..." << std::endl;
            this->_a = stu._a;
            this->_b = stu._b;
        }

        // move构造函数
        student(student&& stu){
            std::cout << "move constructor student ..." << std::endl;
            this->_a = stu._a;
            this->_b = stu._b;
        }

        // 析构函数
        ~student(){
            std::cout << "destructor student..." << std::endl;
        }

        // 拷贝赋值函数
        student& operator=(const student &stu){
            std::cout << "copy operator= student ..." << std::endl;
            this->_a = stu._a;
            this->_b = stu._b;
            return *this;
        }

        // move赋值函数
        student& operator=(student&& stu){
            std::cout << "move operator= student..." << std::endl;
            this->_a = stu._a;
            this->_b = stu._b;
            return *this;
        }

        // operator+
        student& operator+(const student &stu){
            std::cout << "operaotr+ student ..." << std::endl;
            this->_a += stu._a;
            this->_b += stu._b;
            return *this;
        }

        // operator()
        void operator()(){
            std::cout << "opeator()() ... " << std::endl;
        }

        void operator()(int a){
            std::cout << "operator(int a) : " << a  << " this->a: " << this->_a << std::endl;
        }

        // 成员函数
        void add(int a){
            std::cout << "add: " << a << "  this->a: " << this->_a << std::endl;
        }

        void change(int value){
            this->_a = value;
        }
};


class person{
private:
    student _stu1;
    student _stu2;
public:
    person(const student& stu): _stu1(stu){     //进入函数之前先拷贝构造，后空参构造
        _stu2 = stu;    //赋值
        std::cout << "person single param constructor ..." << std::endl;
    }

    ~person(){
        std::cout << "destructor person ..." << std::endl;
    }
};

void add(int a){
    std::cout << "normal add : " << a << std::endl;
}

void sub(int a){
    std::cout << "normal sub : " << a << std::endl;
}


void student_test(){
    student stu;                    //空参构造
    student stu2(stu);              //拷贝构造
    student stu3(std::move(stu));   //move构造
    stu = stu3;                     //拷贝赋值
    stu = std::move(stu3);          //move赋值
    std::cout << "----------" << std::endl;
    person p(stu);
}


void function_test(){
/*
    普通函数
    成员函数：本质上都是函数地址

    函数对象： 拷贝
    lamba     拷贝，但是可以在内部存放对象的引用
*/
    student stu(1, 2);

    //普通函数，存放地址
    std::function<void(int)> singleFunc;
    singleFunc = &add;
    singleFunc(100);


    //成员函数，存放地址
    std::function<void(student&, int)> singleMemFunc;
    singleMemFunc = &student::add;
    singleMemFunc(stu, 200);
    singleMemFunc(stu, 201);

    //传递引用
    singleFunc = [&stu](int a)mutable{
        stu.add(a);
    };

    //函数对象，本质是拷贝构造
    singleFunc = stu;
    std::cout << "--------" << std::endl;
    singleFunc(300);
}

#if 0
class ThreadPool : public TaskQueue {
public:
  explicit ThreadPool(size_t n, size_t mqr = 0)
      : shutdown_(false), max_queued_requests_(mqr) {
    while (n) {
      threads_.emplace_back(worker(*this));
      n--;
    }
  }

  ThreadPool(const ThreadPool &) = delete;
  ~ThreadPool() override = default;

  bool enqueue(std::function<void()> fn) override {
    {
      std::unique_lock<std::mutex> lock(mutex_);
      if (max_queued_requests_ > 0 && jobs_.size() >= max_queued_requests_) {
        return false;
      }
      jobs_.push_back(std::move(fn));
    }

    cond_.notify_one();
    return true;
  }

  void shutdown() override {
    // Stop all worker threads...
    {
      std::unique_lock<std::mutex> lock(mutex_);
      shutdown_ = true;
    }

    cond_.notify_all();

    // Join...
    for (auto &t : threads_) {
      t.join();
    }
  }

private:
  struct worker {
    explicit worker(ThreadPool &pool) : pool_(pool) {}

    void operator()() {
      for (;;) {
        std::function<void()> fn;
        {
          std::unique_lock<std::mutex> lock(pool_.mutex_);

          pool_.cond_.wait(
              lock, [&] { return !pool_.jobs_.empty() || pool_.shutdown_; });

          if (pool_.shutdown_ && pool_.jobs_.empty()) { break; }

          fn = std::move(pool_.jobs_.front());
          pool_.jobs_.pop_front();
        }

        assert(true == static_cast<bool>(fn));
        fn();
      }
    }

    ThreadPool &pool_;
  };
  friend struct worker;

  std::vector<std::thread> threads_;
  std::list<std::function<void()>> jobs_;

  bool shutdown_;
  size_t max_queued_requests_ = 0;

  std::condition_variable cond_;
  std::mutex mutex_;
};

#endif

class ThreadPool{
public:
    ThreadPool() = default;
    ThreadPool(const ThreadPool&) = delete;

    bool equeue(std::function<void()> fn){
        {
            std::lock_guard<std::mutex> lg(mutex_);
            jobs_.push_back(std::move(fn));
        }
        cond_.notify_one();
        return true;
    }

    void shutdown(){

    }

private:
    friend struct worker;

    struct worker{
        explicit worker(ThreadPool& pool) : pool_(pool){}

        void operator()(){
            for(;;){
                std::function<void()> fn;
                {
                    std::unique_lock<std::mutex> lg(pool_.mutex_);
                    // pool_.cond_.wait(lg, )
                }

            }

        }


        ThreadPool& pool_;
    };



    std::vector<thread> threads_;
    std::list<std::function<void()>> jobs_;
    std::mutex mutex_;
    std::condition_variable cond_;
};



int main(int argc, char* argv[]){

/**
 *  std::function   std::thread
 * 
 *  复制一个对象、引用一个对象。对象的拷贝
 * 
 *  普通函数
 *  成员函数
 * 
 *  函数对象
 *  lamba
 * 
 *  thread不支持拷贝构造，但是支持move构造
*/

    


    return 0;
}







