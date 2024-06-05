#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <iostream>
using namespace std;

template <class T>
class Queue {
public:
    Queue(int k);
    void Push(const T& x);
    void Pop();
    T& Front();
    T& Rear();
    bool isEmpty();
    bool isFull();
private:
    T *queue;
    long long size;
    int front;
    int rear;
};

template <class T>
Queue<T>::Queue(int k) {
    size = k;
    queue = new T[size];
    front = 0;
    rear = 0;
}

template <class T>
void Queue<T>::Push(const T& x) {
    if(isFull()) throw "isFull";
    rear = (rear+1) % size;
    queue[rear] = x;
}

template <class T>
void Queue<T>::Pop() {
    if(isEmpty()) throw "isEmpty";
    front = (front+1)%size;
    queue[front].~T();
}

template <class T>
inline T& Queue<T>::Front() {
    if(isEmpty()) throw "isEmpty";
    return queue[(front+1)%size];
}

template <class T>
inline T& Queue<T>::Rear() {
    if(isEmpty()) throw "isEmpty";
    return queue[rear];
}

template <class T>
inline bool Queue<T>::isEmpty() {
    return front == rear;
}

template <class T>
inline bool Queue<T>::isFull() {
    return (rear+1)%size == front;
}

#endif