template<typename T>
class ArrayList {
        int ct;
        int sz;
        T* data;
        public:
                // Types
                typedef T value_type;
                // value_type
                // iterator
                class iterator {
                        T* loc;
                public:
                        iterator(T *l):loc{l} {}
                        iterator():loc{nullptr} {}
                        iterator(const iterator &i):loc{i.loc} {}
                        T &operator*() { return *loc; }
                        bool operator==(const iterator &i) const { return loc == i.loc;}
                        bool operator!=(const iterator &i) const { return !(*this==i);}
                        iterator &operator=(const iterator &i) {loc = i.loc; return *this;}
                        iterator &operator++() { ++loc; return *this; }
                        iterator &operator--() { --loc; return *this; }; //? DONE
                        iterator operator++(int) { 
				iterator i = *this;
				++loc;
				return *i;
			}; //????? DONE
                        iterator operator--(int) {
				iterator i = *this;
				--loc;
				return *i;
			}; //NOT DONE
                };
                // const_iterator
                class const_iterator {
                        T* loc;
                public:
                        const_iterator(T *l):loc{l} {}
                        const_iterator():loc{nullptr} {}
                        const_iterator(const const_iterator &i):loc{i.loc} {}
                        T &operator*() { return *loc; }
                        bool operator==(const const_iterator &i) const { return loc == i.loc;}
                        bool operator!=(const const_iterator &i) const { return !(*this==i);}
                        const_iterator &operator=(const const_iterator &i) {loc = i.loc; return *this;}
                        const_iterator &operator++() { ++loc; return *this; }
                        const_iterator &operator--() {};
                        const_iterator operator++(int) {};
                        const_iterator operator--(int) {};
                };

                // General Methods
                ArrayList():ct{0},sz{10},data{new T[10]} {}
                ArrayList(const ArrayList &that):ct{that.ct}, sz{that.sz}, data{new T[that.sz]} {
			for(int i = 0; i < that.sz; ++i) {
				data[i] = that.data[i];
			}
		};
                ArrayList<T> &operator=(const ArrayList<T> &other) {
			if(sz <= other.sz) {
				delete [] data;
				sz=other.sz;
				data=new T[sz];
			}
			ct=other.ct;
			for(int i=0; i!=sz; ++i) {
				data[i]=other.data[i];
			}
			return *this;
		};
		
		~ArrayList() {delete [] data;}
                void push_back(const T &t); // add to the end. DONE
                void pop_back() {
			--ct;
		}; // remove last element. DONE
                int size() const {
			return ct;
		}; //DONE
                void clear() {
			ct = 0;
		}; //DONE
                void insert(const T &t,int index) {
			bool move = false;
			if(sz == ct) {
				T* tmp = new T[sz*2];
				int i=0;
				while(i < ct){
					if(move){
                                                tmp[i+1] = data[i];
						++i;
                                        }
					else if(i==index){
						tmp[i] = t;
						move = true;
					}
					else{
						tmp[i] = data[i];
						++i;
					}
				}
				delete[] data;
				data = tmp;
				sz *= 2;
			}
			else {
				int i=0;
				T* tmp = new T[sz];
				while(i < ct){
                                        if(move){
                                                tmp[i+1] = data[i];
                                                ++i;
					}
                                        else if(i==index){
                                                tmp[i] = t;
                                                move = true; 
					}
                                        else{
                                                tmp[i] = data[i];
                                                ++i;
                                        	
					}
                                }
				delete[] data;
				data = tmp; 
			}
			++ct;
		}; // insert this element before the given index. DONE
                const T &operator[](int index) const; // get the element at index. DONE
                T &operator[](int index); // get the element at index. DONE
                void remove(int index) {
			bool move = false;  
                        int i=0;
                        while(i < ct){
                                if(move){
                                	data[i] = data[i+1];
                                	++i;
                                }
                                else if(i==index){ move = true; }
                                else{ ++i; }
                        }
			--ct;
		}; // remove the item at the given index. DONE

                iterator begin() {return iterator(data);}
                const_iterator begin() const { return const_iterator(data); } //? DONE
                iterator end() {return iterator(data+ct);}
                const_iterator end() const { return const_iterator(data+ct); } //? DONE
                const_iterator cbegin() const; //? DONE
                const_iterator cend() const; //? DONE
};

template<typename T>
typename ArrayList<T>::const_iterator ArrayList<T>::cbegin() const {
	return const_iterator(data);
}

template<typename T>
typename ArrayList<T>::const_iterator ArrayList<T>::cend() const {
	return const_iterator(data+ct);
}

template<typename T>
void ArrayList<T>::push_back(const T &t) {
        if (sz == ct) {
                T* tmp = new T[sz*2];
                for (int i = 0; i < sz; ++i) tmp[i] = data[i];
                delete[] data;
                data = tmp;
                sz *= 2;
        }
        data[ct++] = t;
}

template <typename T>
const T &ArrayList<T>::operator[](int index) const {
        return data[index];
}

template <typename T>
T &ArrayList<T>::operator[](int index) {
        return data[index];
}
