#include <iostream>
#include <stdexcept>


using namespace std;


template <typename T>
class Stack {
public:
    void push(T x) {
        if(ubound + 1 >= MAX_SIZE) return;
        ubound++;
        data[ubound] = x;
    }
    T pop() {
        return data[ubound--];
    }
    T top() {
        if(!empty()) return data[ubound];
    }
    bool empty() {
        return ubound < 0;
    }
    int size() {
        return ubound + 1;
    }
private:
    static const int MAX_SIZE = 100;
    T data[MAX_SIZE];
    int ubound = -1;
};

template <typename T>
class Queue {
public:
    Queue(int max_length_): max_length(max_length_), head(0), tail(0)  {
        ptr = new T[max_length];
    }
    ~Queue() {
        delete [] ptr;
    }
    void enqueue(T x) {
        // throw std::overflow_error("Stack overflow!");
        ptr[tail] = x;
        if(tail == max_length - 1)
            tail = 0;
        else
            tail++;
    }
    T dequeue() {
        T x = ptr[head];
        if(head == max_length - 1)
            head = 0;
        else
            head++;
        return x;
    }
    int size() {

    }
    bool empty() {
        return tail == head;
    }
    void dprint() {
        for(int i = 0; i < max_length; ++i) {
            cout << ptr[i] << ' ';
        }
        cout << endl;
    }

private:
    T* ptr;
    int max_length;
    int head;
    int tail;

};

int main()
{
    Queue<int> q(12);
    q.enqueue(0);
    q.dequeue();
    q.enqueue(0);
    q.dequeue();
    q.enqueue(0);
    q.dequeue();
    q.enqueue(0);
    q.dequeue();
    q.enqueue(0);
    q.dequeue();
    q.enqueue(0);
    q.dequeue();
    q.enqueue(15);
    q.enqueue(6);
    q.enqueue(9);
    q.enqueue(8);
    q.enqueue(4);
    //q.enqueue(0);
    q.dprint();
    q.enqueue(17);
    q.enqueue(3);
    q.enqueue(5);
    q.dprint();
    cout << q.dequeue() << endl;
    q.dprint();

    q.enqueue(999);
    q.enqueue(999);
    q.enqueue(999);
    q.enqueue(999);
    q.enqueue(999);

    cout << q.dequeue() << endl;
    q.dprint();

    cout << endl;


    return 0;
}
