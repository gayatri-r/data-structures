#include <iostream>

template<typename T>
class LinkedList {
        struct Node {
                Node():next{nullptr},prev{nullptr} {}
                Node(const T &t, Node *p, Node *n):data{t},next{n},prev{p} {}
                T data;
                Node *next, *prev;
        };
        Node sent;
        int ct;
public:
        typedef T value_type;
        class iterator {
                Node * loc;
        public:
                iterator():loc{nullptr} {}
                iterator(Node *l): loc{l} {}
                iterator &operator++() {loc=loc->next; return *this;}
                bool operator==(const iterator &i) { return loc == i.loc; }
                bool operator!=(const iterator &i) { return !(*this==i);}
                T &operator*() { return loc->data;}
                iterator &operator=(const iterator &i) { loc = i.loc; return *this; }
                iterator &operator--() { loc=loc->prev; return *this; }
                iterator operator++(int) {
                        iterator i = *this;
                        ++loc;
                        return *i;
                }
                iterator operator--(int) {
			iterator i = *this;
                        --loc;
			return *i;
                }

                friend class const_iterator;
                friend class LinkedList;
        };

        class const_iterator {
                const Node * loc;
        public:
                const_iterator():loc{nullptr} {}
		const_iterator(const Node *l): loc{l} {}
		const_iterator(iterator &i) {
			loc = i.loc;
		}
                const_iterator &operator++() {loc=loc->next; return *this;}
		bool operator==(const const_iterator &i) { return loc == i.loc; }
                bool operator!=(const const_iterator &i) { return !(*this==i);}
                const T &operator*() { return loc->data;}
                const_iterator &operator=(const const_iterator &i) { loc = i.loc; return *this; }
		const_iterator &operator--() { loc=loc->prev; return *this; }
                const_iterator operator++(int) {
                        const_iterator i = *this;
			++loc;
			return *i;
                }
                const_iterator operator--(int) {
                	const_iterator i = *this;
			--loc;
			return *i;
                }

        };
        // LinkedList methods
        LinkedList();
	LinkedList &operator=(const LinkedList &al) {
		std::cout << "equals operator start\n";
		clear();

		for(int i = 0; i < al.size(); ++i){
			push_back(al[i]);
		}
		std::cout << "equals operator end\n";
		return *this;
	}
	LinkedList(const LinkedList &al) {
		std::cout << "copy constructor start\n";
		ct = 0;
                sent.next = &sent;
                sent.prev = &sent;
		std::cout << "copied sent\n";
		/*for(int i = 0; i < al.size(); ++i){
                        push_back(al[i]);
                }*/
		//TRAVERSE AL AND PUT IN LINKED LIST. NOT PUSHBACK
		Node * rover = al.sent.next;
		for(int i = 0; i<al.size(); ++i){
			push_back(rover->data);
			rover = rover->next;
		}
		std::cout << "copy constructor ends\n";
	}
        ~LinkedList() {
                for (Node * rover = sent.next; rover != &sent; ) {
                        rover = rover->next;
                        delete rover->prev;
                }
        }
        void push_back(const T &t);
        T &operator[](int index);
        void pop_back() {
		sent.prev = sent.prev->prev;
		delete sent.prev->next;
		sent.prev->next = &sent;
		--ct;	
	} // remove last element.
        int size() const { return ct; }
        void clear() {
		std::cout << "clear start\n";
		for (Node * rover = sent.next; rover != &sent; ) {
                        rover = rover->next;
                        delete rover->prev;
             	}
		ct = 0;
        	sent.next = &sent;
        	sent.prev = &sent;
		std::cout << "clear end\n";
	}
        iterator insert(iterator position,const T &t) {
		std::cout << "insert start\n";
		Node * prevNode = (--position).loc;
		Node * nextNode = (++position).loc;
		Node * nNode = new Node(t, prevNode, nextNode);
		prevNode->next = nNode;
		nextNode->prev = nNode;
		++ct;
		position.loc = nNode;
		std::cout << "insert end\n";
		return position;
	} // insert this element before the given index.
        const T &operator[](int index) const {
		Node * rover = sent.next;
        	for(int i = 0; i!=index; ++i) rover = rover->next;
		return rover->data;
	}; // get the element at index.
        iterator erase(iterator position) {
		Node * currentNode = position.loc;
		Node * prevNode = (--position).loc; //node before deleted node
		Node * nextNode = (++(++position)).loc; //node after deleted node

		prevNode->next = nextNode;
		nextNode->prev = prevNode;
		--ct;
		delete  currentNode;
		return position; //returns node after deleted node
	}; // remove the item at the given index.

        // iterator stuff
        iterator begin() {return iterator(sent.next);}
        iterator end() {return iterator(&sent);}
        const_iterator begin() const { return const_iterator(sent.next); }
        const_iterator end() const { return const_iterator(&sent); }
        const_iterator cbegin() const { return const_iterator(sent.next); }
        const_iterator cend() const { return const_iterator(&sent); }
};

//template<typename T>
//typename LinkedList<T>::const_iterator LinkedList<T>::cbegin() {}

template<typename T>
LinkedList<T>::LinkedList() {
        ct = 0;
        sent.next = &sent;
        sent.prev = &sent;
}

template<typename T>
void LinkedList<T>::push_back(const T &t) {
        Node * n = new Node(t,sent.prev,&sent);
        sent.prev->next = n; // n->prev->next = n
        sent.prev = n; // n->next->prev = n
        ++ct;
}

template<typename T>
T &LinkedList<T>::operator[](int index) {
        Node * rover = sent.next;
        for(int i = 0; i!=index; ++i) rover = rover->next;
        return rover->data;
}
