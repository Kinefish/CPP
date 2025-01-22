# c11多线程

> 通过`thread`来创建线程，语言级别。

## 多线程模拟售票

> 互斥需要用`mutex`，如果没有及时调用`mtx.unlock()`，就提前`return`，会导致锁没有及时释放，可以使用`lock_guard`

- 售票窗口可以通过创建多个线程对象实现
- `tickets`需要线程安全的
- 锁+双重判断

```cpp
//tickets sell
static void ticketSell(int idx) {
    while (ticket > 0) {
        {
            //mtx.lock();
            std::lock_guard<std::mutex> lgard(mtx);//出作用域自动析构
            if (ticket > 0) {
                std::cout << "window: " << idx << "selling ticket: " << ticket << std::endl;
                ticket--;
            }
            //mtx.unlock();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}
```

## 生产者-消费者模型

> 实现简单的生1取1

- `cpp STL`中都是线程不安全的，需要自己封装一下

```cpp
class Queue {
public:
    void put(int val) {
        std::unique_lock<std::mutex> ulock(objct_pool_mtx);
        while (!que.empty()) {
            object_pool_cv.wait(ulock);
        }
        que.push(val);
        std::cout << "produce No." << val << std::endl;
        object_pool_cv.notify_all();
    }
    void get() {
        std::unique_lock<std::mutex> ulock(objct_pool_mtx);
        while (que.empty()) {
            object_pool_cv.wait(ulock);
        }
        std::cout << "consume No." << que.front() << std::endl;
        que.pop();
        object_pool_cv.notify_all();
    }
private:
    std::queue<int> que;
};
```

## CAS

> 保证++ --的原子操作，不需要用锁

```cpp
//CAS atomic
static void add() {
    while (!isReady) {
        std::this_thread::yield();
    }
    for (int i = 0; i < 100; i++) {
        myCount++;
    }
}
static void test04() {
    std::list<std::thread> plist;
    for (int i = 0; i < 10; i++) {
        plist.push_back(std::thread(add));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    isReady = true;
    for (std::thread& t : plist) {
        t.join();
    }
    std::cout << myCount << std::endl;
}
```

