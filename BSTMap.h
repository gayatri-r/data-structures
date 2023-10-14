#include <utility>
#include <algorithm>
#include <iostream>

template<typename K,typename V>
class BSTMap {
    struct Node {
	Node *left, *right, *parent;
	std::pair<K,V> data;
   };
   Node *root;
   int ct;

public:
    typedef K key_type;
    typedef V mapped_type;
    typedef std::pair<K,V> value_type;

    class const_iterator;

    class iterator {
	Node *loc;
	bool isEnd;
    public:
        friend class const_iterator;
	iterator():loc(nullptr),isEnd(false) {}
        iterator(Node *n):loc(n),isEnd(false) {}
	iterator(Node *n, bool e):loc(n),isEnd(e) {}

        bool operator==(const iterator &i) const { return loc == i.loc && isEnd == i.isEnd; }
        bool operator!=(const iterator &i) const { return !(*this==i); }
        std::pair<K,V> &operator*() { return loc->data; }
        iterator &operator++() {
		if (loc == nullptr) return *this;
		if (loc->right == nullptr) {
			Node *c = loc;
			Node *start = loc;
			loc = loc->parent;
			while(loc != nullptr && c == loc->right) {
				c = loc;
				loc = loc->parent;
			}
			if (loc == nullptr) { // no more nodes to the right
				isEnd = true;
				loc = start;
			}
		} else {
			loc = loc->right;
			while(loc->left != nullptr) loc = loc->left;
		}
		return *this;
        } //DONE ?
        iterator &operator--() {
            if (loc == nullptr) return *this;
	    if(isEnd){
		isEnd = false;
	    	return *this;
	    }
	    if (loc->left == nullptr) {
		Node *c = loc;
		Node *start = loc;
		loc = loc -> parent;
		while(loc != nullptr && c == loc->left) {
			c = loc;
			loc = loc->parent;
		} 
		if (loc == nullptr) {
			loc = start;
		}
	    } else {
	    	loc = loc -> left;
		while(loc->right != nullptr) loc = loc -> right;
	    }
            return *this;
        } //DONE ?
        iterator operator++(int) {
            iterator tmp(*this);
            ++(*this);
            return tmp;
        }
        iterator operator--(int) {
            iterator tmp(*this);
            --(*this);
            return tmp;
        }
	Node* getLoc(){ return loc; }
    };

    class const_iterator {
	Node *loc;
	bool isEnd;
    public:
        friend class BSTMap<K,V>;  // You might not need this in your code, but it helped me.
	const_iterator():loc(nullptr),isEnd(false) {}
        const_iterator(Node *n):loc(n),isEnd(false) {}
	const_iterator(Node *n, bool e):loc(n),isEnd(e) {}

        const_iterator(const iterator &iter):loc(iter.loc),isEnd(iter.isEnd) {}

        bool operator==(const const_iterator &i) const { return loc == i.loc && isEnd == i.isEnd; }
        bool operator!=(const const_iterator &i) const { return !(*this==i); }
        const std::pair<K,V> &operator*() { return loc->data; }
        const_iterator &operator++() {
		if (loc == nullptr) return *this;
		if (loc->right == nullptr) {
			Node *c = loc;
			Node *start = loc;
			loc = loc->parent;
			while(loc != nullptr && c == loc->right) {
				c = loc;
				loc = loc->parent;
			}
			if (loc == nullptr) { // no more nodes to the right
				isEnd = true;
				loc = start;
			}
		} else {
			loc = loc->right;
			while(loc->left != nullptr) loc = loc->left;
		}
		return *this;
        }
        const_iterator &operator--() {
            if(loc == nullptr) return *this;
	    if(isEnd){
		isEnd = false;
	    	return *this;
	    }
	    if(loc->left == nullptr) {
		Node *c = loc;
		Node *start = loc;
		loc = loc -> parent;
		while(loc != nullptr && c == loc->left) {
			c = loc;
			loc = loc->parent;
		} 
		if (loc == nullptr) {
			loc = start;
		}
	    } else {
	    	loc = loc -> left;
		while(loc->right != nullptr) loc = loc -> right;
	    }
            return *this;
        } //DONE ?
        const_iterator operator++(int) {
            const_iterator tmp(*this);
            ++(*this);
            return tmp;
        }
        const_iterator operator--(int) {
            const_iterator tmp(*this);
            --(*this);
            return tmp;
        }
	Node* getLoc(){ return loc; }
    };

    BSTMap():root(nullptr),ct(0) {} //DONE ?
    ~BSTMap() {
	/*
	vector <Node*> del;
	iterator *rover = begin();
	while(!rover.isEnd) {
		del.push_back(rover.getLoc()); //should push back the address of the node .?
		rover++;
	}
	for(auto &v:del){
		delete v; //theoretically, this should delete all the node memory addresses, clearing the map
	}*/
    } //DONE ?
    BSTMap(const BSTMap<K,V> &that) {
	root = copyTree(that.root, nullptr);
	ct = that.ct;
    } //DONE ?
    BSTMap &operator=(const BSTMap<K,V> &that) {
	root = copyTree(that.root, nullptr);
	ct = that.ct;
    } //DONE ?
    Node* copyTree(Node *orig, Node *p) {
	Node *n = new Node();
	n->parent = p;
	n->data = orig->data;
	if(orig->left != nullptr) n->left = copyTree(orig->left, n);
	else{ n->left = nullptr; }
	if(orig->right != nullptr) n->right = copyTree(orig->right, n);
	else{ n->right = nullptr; }
	return n;
    }
    bool empty() const { return ct == 0; }
    unsigned size() const { return ct; }
    iterator find(const key_type& key){
	Node *rover = root;
	Node *trail = nullptr;
	while (rover != nullptr) {
		if(rover->data.first == key){
			return iterator(rover);
		}
		trail = rover;
		if(rover->data.first > key) rover = rover->left;
		else rover = rover->right;
	}
	// key not found
	return end();
    }//DONE ?
    const_iterator find(const key_type& key) const {
	Node *rover = root;
	Node *trail = nullptr;
	while (rover != nullptr) {
		if(rover->data.first == key){
			return const_iterator(rover);
		}
		trail = rover;
		if(rover->data.first > key) rover = rover->left;
		else rover = rover->right;
	}
	// key not found
	return cend();
    }//DONE ?
    unsigned int count(const key_type& key) const {
	Node *rover = root;
	Node *trail = nullptr;
	while (rover != nullptr) {
		if(rover->data.first == key){
			return 1;
		}
		trail = rover;
		if(rover->data.first > key) rover = rover->left;
		else rover = rover->right;
	}
	// key not found
	return 0;
    } //DONE ?
    std::pair<iterator,bool> insert(const value_type& val) {
	key_type key = val.first;
	Node *rover = root;
	Node *trail = nullptr;
	while (rover != nullptr) {
		if(rover->data.first == key){
			return std::make_pair(iterator(rover), false); //key found
		}
		trail = rover;
		if(rover->data.first > key) rover = rover->left;
		else rover = rover->right;
	}
	// key not found
	Node *n = new Node();
	n->parent = trail;
	n->data = val;
	if (trail == nullptr) root = n;
	else {
		if (trail->data.first > key) trail->left = n;
		else trail->right = n;
	}
	++ct;
	return std::make_pair(iterator(n), true);
    }//DONE ?
    template <class InputIterator>
    void insert(InputIterator first, InputIterator last) {
        for(auto iter = first; iter!=last; ++iter) {
        	insert(*iter);
        }
    }
    iterator erase(const_iterator position){
	if(position == cend()){
		return end();
	}
	Node *n = position.getLoc();
	iterator ret = iterator(n);
	++ret;
	Node *successor;
	//case 1: node has no children
	if(n->left == nullptr && n->right == nullptr){
		if(n->parent != nullptr && (n->parent)->right == n) (n->parent)->right = nullptr;
		if(n->parent != nullptr && (n->parent)->left == n) (n->parent)->left = nullptr;
		--ct;
		return ret;
	}
	//case 2: node has one child
	if(n->left == nullptr ^ n->right == nullptr){ //if just one child exists
		bool changeRet = false;
		if(n->right != nullptr){
			successor = n->right;
			while(successor->left != nullptr) successor = successor->left;
			changeRet = true;
		}
		if(n->left != nullptr){
			successor = n->left;
			while(successor->right != nullptr) successor = successor->right;
		}
		std::pair<K,V> sData = successor->data;
		erase(successor->data.first);
		n->data = sData;
		if(changeRet) return iterator(n);
		else{ return ret; }
	}
	//case 3: node has two children
	else{
		successor = n->right; //right first
		while(successor->left != nullptr) successor = successor->left; //as far left as possible
		std::pair<K,V> sData = successor->data;
		erase(successor->data.first);
		n->data = sData;
		return iterator(n);
   	}	
    }//DONE ? (????)
    unsigned int erase(const key_type& k){
	iterator position = find(k);
	if(position == end()){
		return 0;
	}
	Node *n = position.getLoc();
	Node *successor;
	//case 1: node has no children
	if(n->left == nullptr && n->right == nullptr){
		if(n->parent != nullptr && (n->parent)->right == n) (n->parent)->right = nullptr;
		if(n->parent != nullptr && (n->parent)->left == n) (n->parent)->left = nullptr;
		--ct;
		return 1;
	}
	//case 2: node has one child
	if(n->left == nullptr ^ n->right == nullptr){ //if just one child exists
		if(n->right != nullptr){
			successor = n->right;
			while(successor->left != nullptr) successor = successor->left;
		}
		if(n->left != nullptr){
			successor = n->left;
			while(successor->right != nullptr) successor = successor->right;
		}
		std::pair<K,V> sData = successor->data;
		erase(successor->data.first);
		n->data = sData;
		return 1;
	}
	//case 3: node has two children
	else{
		successor = n->right; //right first
		while(successor->left != nullptr) successor = successor->left; //as far left as possible
		std::pair<K,V> sData = successor->data;
		erase(successor->data.first);
		n->data = sData;
		return 1;
   	} 
    }//DONE ?
    void deleteNode(Node *n){
    	if(n->parent != nullptr){
		(n->parent)->left = n->left;
		if(n->left != nullptr) (n->left)->parent = n->parent;
		(n->parent)->right = n->right;
		if(n->right != nullptr) (n->right)->parent = n->parent;
	}
    }
    void clear(){
    	root = nullptr;
	ct = 0;
    }//DONE ?
    mapped_type &operator[](const K &key); //DONE
    bool operator==(const BSTMap<K,V>& rhs) const {
	if(rhs.ct != ct) return false;
	//bool ret = equals(root, rhs.root);
	//return ret;
	const_iterator rover = begin();
	const_iterator roverT = rhs.begin();
	while(rover != end() && roverT != rhs.end()){
		if((rover.getLoc())->data != (roverT.getLoc())->data){
			return false;
		}
		++rover;
		++roverT;
	}
	return true;
    }//DONE ?
    /*bool equals(Node *n, Node *n2) const {
	bool ret = true;
	if(n->data == n2->data) {
		if(ret && n->right != nullptr){
			if(n2->right != nullptr){
				ret = equals(n->right, n2->right);
			}
			else{ ret = false; }
		}
		if(ret && n->left != nullptr){
			if(n2->left != nullptr){
				ret = equals(n->left, n2->left);
			}
			else{ ret = false; }
		}
	}
	else{
		return false;
	}
    }*/ //if map equality is also based on position
    bool operator!=(const BSTMap<K,V>& rhs) const {
	    bool ret = *this==rhs;
	    return !ret;
    } //DONE ?

    iterator begin() {
	Node *n = root;
	while(n->left != nullptr) n = n->left;
	return iterator(n);
    } //DONE ?
    const_iterator begin() const {
    	Node *n = root;
	while(n->left != nullptr) n = n->left;
	return const_iterator(n);
    } //DONE ?
    iterator end() {
	Node *n = root;
	while(n->right != nullptr) n = n->right;
	return iterator(n,true);
    } //DONE ?
    const_iterator end() const {
	Node *n = root;
	while(n->right != nullptr) n = n->right;
	return const_iterator(n,true);
    } //DONE ?
    const_iterator cbegin() const { 
	Node *n = root;
	while(n->left != nullptr) n = n->left;
	return const_iterator(n); 
    }
    const_iterator cend() const { 
	Node *n = root;
	while(n->right != nullptr) n = n->right;
	return const_iterator(n,true);
    }
};

template<typename K, typename V>
typename BSTMap<K,V>::mapped_type &BSTMap<K,V>::operator[](const K &key) {
	Node *rover = root;
	Node *trail = nullptr;
	while (rover != nullptr) {
		if(rover->data.first == key) { 
			return rover->data.second;
		}
		trail = rover;
		if(rover->data.first > key) rover = rover->left;
		else rover = rover->right;
	}
	++ct;
	Node *n = new Node();
	n->parent = trail;
	n->data.first = key;
	if (trail == nullptr) root = n;
	else {
		if (trail->data.first > key) trail->left = n;
		else trail->right = n;
	}
	return n->data.second;
}
