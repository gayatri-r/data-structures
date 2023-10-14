#include "Queue.h"
  
template<typename T>
class ArrayQueue:public Queue<T> {
public:
        ArrayQueue():head{0},tail{0},sz{10},data{new T[10]} {}
        ArrayQueue(const ArrayQueue &q);
	~ArrayQueue() {delete [] data;}
        void enqueue(const T &t);
        T dequeue();
        T peek() const;
        bool isEmpty() const;
private:
        int head,tail;
        int sz;
        T* data;
	int counter = 0;
};

template<typename T>
ArrayQueue<T>::ArrayQueue(const ArrayQueue &q):head{q.head},tail{q.tail},sz{q.sz},data{new T[q.sz]} {
	for(int i=0; i < q.sz; ++i) {
                data[i] = q.data[i];
        }
}

template<typename T>
void ArrayQueue<T>::enqueue(const T &t) {
        if((tail+1)%sz==head) {
                T *tmp = new T[sz*2];
                for (int i=0; i<sz-1; ++i) tmp[i]=data[(head+i)%sz];
                delete[] data;
                data = tmp;
                head = 0;
                tail = sz-1;
                sz *= 2;
        }
        data[tail] = t;
        tail=(tail+1)%sz;
	counter++;
}

template<typename T>
T ArrayQueue<T>::dequeue() {
	//if((head)%sz==tail) { //queue is 1 long
		//T tmp = head;
	//}
	counter--;
	T tmp = data[head];
	head=(head+1)%sz;
	return tmp;
}

template<typename T>
T ArrayQueue<T>::peek() const{
	return data[head];
}

template<typename T>
bool ArrayQueue<T>::isEmpty() const{
	if(counter==0) {
		return true;
	} else {
		return false;
	}
}
