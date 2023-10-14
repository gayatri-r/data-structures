#include "Stack.h"

template<typename T>
class ArrayStack:public Stack<T> {
        public:
                ArrayStack();
                ArrayStack(const ArrayStack &s);
                ~ArrayStack();
                void push(const T &i);
                T pop();
                T peek() const;
                ArrayStack<T>& operator=(const ArrayStack<T> &other) {
			if(sz >= other.sz) {
				delete [] data;
				sz=other.sz;
				data=new T[sz];
			}
			top=other.top;
			for(int i = 0; i!=sz; ++i) {
				data[i]=other.data[i];
			}
			return *this;
		}
		bool isEmpty() const;
        private:
                int top;
                int sz;
                T * data;
};

template<typename T>
ArrayStack<T>::ArrayStack():top{0},sz{10},data{new T[10]} {
// make array
// set top=0
// set sz=...
}

template<typename T>
ArrayStack<T>::ArrayStack(const ArrayStack &s):top{s.top},sz{s.sz},data{new T[s.sz]} {
	for(int i = 0; i<s.sz; ++i) {
		data[i] = s.data[i];
	}
}

template<typename T>
ArrayStack<T>::~ArrayStack() {
    delete [] data;
}

template<typename T>
void ArrayStack<T>::push(const T &i) {
    if (top == sz) {
        // resize array
        T * tmp = new T[sz*2];
        for(int i = 0; i!=top; ++i) {
            tmp[i] = data[i];
        }
        delete [] data;
        data = tmp;
        sz *= 2;
    }
    data[top++] = i;
}

template<typename T>
T ArrayStack<T>::pop(){
    return data[--top];
}

template<typename T>
T ArrayStack<T>::peek() const{
    return data[top-1];
}

template<typename T>
bool ArrayStack<T>::isEmpty() const{
    return top==0;
}
