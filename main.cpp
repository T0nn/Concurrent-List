#include <iostream>
#include <climits>
#include <random>
#include <chrono>
#include <thread>
#include <pthread.h>

#include "ListNode.hpp"
#include "ConcurrentList.hpp"
#include "ConcurrentListOpt.hpp"

using namespace std;

template <typename T, template <typename> class L>
struct thread_args {
    int thread_id;
    int numElement;
    shared_ptr<L<T>> list;
    thread_args(int id, int numElement, shared_ptr<L<T>> list): thread_id(id), numElement(numElement), list(list){}
};

template <typename T, template <typename> class L>
void SingleTestList(shared_ptr<L<T>> list) {
        
    list->contain(3);
    list->contain(4);

    list->remove(0);
    list->remove(1);

    list->add(1);
    list->add(3);
    list->add(0);
    list->add(4);

    list->remove(2);
    list->remove(2);
    list->remove(2);
    list->remove(1);
    list->remove(3);

    list->add(5);
    list->add(6);

    cout << list->contain(3) << endl;
    cout << list->contain(4) << endl;
    cout << list->contain(5) << endl;
    cout << list->contain(9) << endl;
    cout << list->contain(-1) << endl;
    cout << list->contain(0) << endl;

}

template <typename T, template <typename> class L>
void* ThreadTestList(void *args) {

    auto arg_struct = static_cast<thread_args<T, L>*>(args);
    auto thread_id = arg_struct->thread_id;
    auto numElement = arg_struct->numElement;
    auto list = arg_struct->list;

    auto gen = std::bind(std::uniform_int_distribution<>(0,1),std::default_random_engine());

    bool b1 = gen();
    bool b2 = gen();
    bool b3 = gen();

    auto count = 1;
    for(int i = numElement; i > -numElement; --i) {
        
        auto num = static_cast<T> (rand() % (2*numElement-1) - numElement + 1); 
        
        b1 = gen();
        b2 = gen();
        b3 = gen();
        
        if (b1) {
            list->add(num);
        }
        if (b2) {
            list->contain(num);
        }
        if (b3) {
            list->remove(num);
        }

        // for(int i = 0; i < 500; i++) {
        //     b2 = gen();
        //     if (b2) {
        //         list->add(num);
        //     }
        // }

        // list->add(i);
        // list->contain(i);
        // list->remove(i);
        
        
        if ( (numElement-i) / ((2*numElement/10)) > count) {
            // cout << count << "0.. " << "Thread "<<thread_id <<  endl;
            ++count;
        } 
    }

    cout << "Finish Thread  " << thread_id <<  endl;
    return nullptr;
}

template <typename T, template <typename> class L>
void MultiTestList(shared_ptr<L<T>> list, int numThreads = 1, int numElement = 100, int sleepTime = 0) {
    // thread pool test

    pthread_t thread[numThreads];

    cout << "Starting all threads.." << endl;

    for(int thread_num = 0; thread_num < numThreads; thread_num++) {
        auto args = thread_args<T, L>(thread_num, numElement, list);
        int result = pthread_create(&thread[thread_num], NULL, ThreadTestList<T, L>, static_cast<void *>(&args));
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
        if (result !=0) {
            cout << "Error creating thread " << thread_num << ". Return code:" << result <<  endl;
        }
    }

    for(int thread_num = 0; thread_num < numThreads; thread_num++) {
        pthread_join(thread[thread_num], NULL);
    }

    cout << "All threads completed." << endl;
}

template <typename T, template <typename> class L>
void printList(shared_ptr<L<T>> list, int flag = 0) {
    cout << "===========" << endl;
    cout << list->print(flag) << endl;
    cout << "===========" << endl;

}

int main() {
    
    static auto list = make_shared<ConcurrentList<double>>();
    // static auto list = make_shared<ConcurrentListOpt<double>>(); // suffer starvation

    int numThread = 100;
    int numElement = 2500;
    int sleepTime = 50;
    int printFlag = 0;

    // simple test
    SingleTestList(list);

    printFlag = 1;
    printList(list, printFlag);

    // multi thread test
    MultiTestList(list, numThread, numElement, sleepTime);

    printFlag = 0;
    printList(list, printFlag);

    list.reset();

    return 0;
}

