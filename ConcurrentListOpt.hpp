#ifndef ConcurrentListOpt_H
#define ConcurrentListOpt_H
#include <iostream>
#include <climits>
#include <random>
#include <chrono>
#include <thread>
#include <pthread.h>

#include "ListNode.hpp"
#include "ConcurrentList.hpp"


template <typename T>
class ConcurrentListOpt final : public ConcurrentList<T> {
public:

    ConcurrentListOpt() {
        head = new ListNode<T>(std::numeric_limits<T>::min());
        head->next = new ListNode<T>(std::numeric_limits<T>::max());
    }
    ~ConcurrentListOpt() {
        ListNode<T> *prev, *curr;

        prev = head;
        curr = prev->next;
        while (curr != nullptr) {
            delete prev;
            prev = curr;
            curr = prev->next;
        }
        delete prev;
    }

    bool validate(ListNode<T>* prev, ListNode<T>* curr) {
        ListNode<T>* node = head;
        while (node->val <= prev->val) {
            if (node == prev){
                return (prev->next == curr);
            }
            node = node->next;
        }
        return false;
    }

    bool contain(T val) {
        // find and validate
        ListNode<T> *prev, *curr;

        prev = head;
        curr = prev->next;
        while (curr != nullptr) {
            if (curr->val >= val) {
                pthread_mutex_lock(&prev->mutex);
                pthread_mutex_lock(&curr->mutex);
                if(validate(prev, curr)) {
                    bool ret = (curr->val == val);
                    pthread_mutex_unlock(&curr->mutex);
                    pthread_mutex_unlock(&prev->mutex);
                    return ret;
                } else {
                    pthread_mutex_unlock(&curr->mutex);
                    pthread_mutex_unlock(&prev->mutex);
                    return false;
                }
            }
            prev = curr;
            curr = prev->next;
        }
        return false;

    }

    bool add(T val) {
        ListNode<T> *prev, *curr;

        prev = head;
        curr = prev->next;
        while ( (curr != nullptr)  ) {
            if (curr->val >= val) {
                pthread_mutex_lock(&prev->mutex);
                pthread_mutex_lock(&curr->mutex);
                if(validate(prev, curr)) {
                    bool ret = false;
                    if (curr->val > val){
                        ret = true;
                        auto newNode = new ListNode<T>(val);
                        newNode->next = prev->next;
                        prev->next = newNode;
                    } else{
                        ret = false;
                    }
                    pthread_mutex_unlock(&curr->mutex);
                    pthread_mutex_unlock(&prev->mutex);
                    return ret;
                    
                } else {
                    pthread_mutex_unlock(&curr->mutex);
                    pthread_mutex_unlock(&prev->mutex);
                    return false;
                }
            }
            prev = curr;
            curr = prev->next;
        }
        return false;
    }

    bool remove(T val) {
        ListNode<T> *prev, *curr;

        prev = head;
        curr = prev->next;
        while (curr != nullptr) {
            if (curr->val >= val) {
                pthread_mutex_lock(&prev->mutex);
                pthread_mutex_lock(&curr->mutex);
                if(validate(prev, curr)) {
                    bool ret = false;
                    if (curr->val == val){
                        ret = true;
                        prev->next = curr->next;
                        delete curr;
                    } else{
                        ret = false;
                    }
                    if (!ret) {
                        pthread_mutex_unlock(&curr->mutex);
                    }                    
                    pthread_mutex_unlock(&prev->mutex);
                    return ret;
                    
                } else {
                    pthread_mutex_unlock(&curr->mutex);
                    pthread_mutex_unlock(&prev->mutex);
                    return false;
                }
            }
            prev = curr;
            curr = prev->next;
        }
        return false;
    }

    int print(int flag = 0){
        ListNode<T>* temp = head;
        int count = 0;
        while(temp != nullptr) {
            if (flag) {
                cout << temp->val << endl;
            }
            temp = temp->next;
            count++;
        }
        return count;
    }


protected:
    ListNode<T>* head;
};
#endif