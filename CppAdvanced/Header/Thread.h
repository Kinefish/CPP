#ifndef __THREAD_H__
#define __THREAD_H__
#include <thread>
#include <mutex>
#include <condition_variable> //support c-v actions
#include <atomic>

#include <list>
#include <queue>
#include <iostream>
namespace thread_test_var {
    int ticket = 100;
    std::mutex ticket_mtx;

    std::mutex objct_pool_mtx;
    std::condition_variable object_pool_cv;

    volatile std::atomic_bool isReady = false;
    volatile std::atomic_int myCount = 0; //10 thread add this count to 1k
}

namespace thread_test {
    using namespace thread_test_var;
    
    static void thread01(int time) {
        std::this_thread::sleep_for(std::chrono::seconds(time));
        std::cout << "thread01 running" << std::endl;
    }
    
    static void test01() {
        std::thread t(thread01, 2);
        t.join();
        std::cout << "test01 run" << std::endl;
    }

    //tickets sell
    static void ticketSell(int idx) {
        while (ticket > 0) {
            {
                //mtx.lock();
                std::lock_guard<std::mutex> lgard(ticket_mtx);
                if (ticket > 0) {
                    std::cout << "window: " << idx << "selling ticket: " << ticket << std::endl;
                    ticket--;
                }
                //mtx.unlock();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    }
    static void test02() {
        std::list<std::thread> plist;

        for (int i = 1; i <= 3; ++i) {
            plist.push_back(std::thread(ticketSell, i));
        }

        for (auto &thread : plist) {
            thread.join();
        }

    }
    //end tickets sell


    //producer-consumer

    //object pools
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

    static void producer(Queue *que) {
        for (int i = 0; i < 10; ++i) {
            que->put(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
    static void consumer(Queue *que) {
        for (int i = 0; i < 10; ++i) {
            que->get();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
    static void test03() {
        Queue que;
        std::thread t1(producer, &que);
        std::thread t2(consumer, &que);
        t1.join();
        t2.join();
    }


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

}

#endif // !__THREAD_H__
