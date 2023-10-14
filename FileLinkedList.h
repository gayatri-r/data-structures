#include <string>

template<typename T>
class FileLinkedList {
	int ct;
	int firstfree;
	FILE * fptr;
	
	//helper functions
	void writeCtAndFF();
	void readData(int pos, T &data) const;
	void writeData(int pos, const T &data);
	void writeNext(int pos, int next);
	int readNext(int pos) const;
	void writePrev(int pos, int prev);
	int readPrev(int pos) const;
	void writeNode(int pos, int prev,const T &data, int next);

	FileLinkedList(const FileLinkedList<T> &that) = delete;
	FileLinkedList<T> operator=(const FileLinkedList<T> &that) = delete;
	public:
		typedef T value_type;
		class const_iterator {
			int pos;
			FILE *fl;
			public:
			const_iterator(int i,FILE *f):pos{i},fl{f} {};
			const_iterator(const const_iterator &i):pos{i.pos},fl{i.fl} {};
			T operator*() {
				std::fseek(fl,3*sizeof(int) + pos*(sizeof(T)+2*sizeof(int)),SEEK_SET);
				T ret;
				std::fread(&ret,sizeof(T),1,fl);
				return ret;
			};
			bool operator==(const const_iterator &i) const {return pos==i.pos && fl==i.fl;};
			bool operator!=(const const_iterator &i) const {return !(*this==i); };
			const_iterator &operator=(const const_iterator &i) {
				pos = i.pos;
				fl = i.fl;
				return *this;
			}; //???
			const_iterator &operator++() { //++a
				std::fseek(fl,3*sizeof(int) + sizeof(T) + pos*(sizeof(T)+2*sizeof(int)),SEEK_SET);
				std::fread(&pos,sizeof(int),1,fl);
				return *this;
			};
			const_iterator &operator--() {	
				std::fseek(fl,2*sizeof(int) + pos*(sizeof(T)+2*sizeof(int)),SEEK_SET);
				std::fread(&pos, sizeof(int), 1, fl); //read prev into pos
				return *this;
			}; //DONE
			const_iterator operator++(int) {
				const_iterator i(*this);
				std::fseek(fl,3*sizeof(int) + sizeof(T) + pos*(sizeof(T)+2*sizeof(int)),SEEK_SET);
				std::fread(&pos,sizeof(int),1,fl);
				return *i;
			};
			const_iterator operator--(int) {
				const_iterator i(*this);
				std::fseek(fl,2*sizeof(int) + pos*(sizeof(T)+2*sizeof(int)),SEEK_SET);
				std::fread(&pos, sizeof(int), 1, fl);
				return *i;
			}; //???

			friend class FileLinkedList;
		};
		FileLinkedList(const std::string &fname);
		template<typename I> // The type I will be an iterator.
		FileLinkedList(I begin,I end,const std::string &fname) {
			fptr = std::fopen(fname.c_str(),"w+b"); //overwrite file
			clear(); //clear file
			while(begin.pos != end.pos) { //so while we haven't traversed to the end
				push_back(*begin); //push_back begin's data
				++begin; //increment begin
			}
		}
		~FileLinkedList() { std::fclose(fptr); };
		void push_back(const T &t);
		void pop_back(); //DONE
		int size() const {
			return (ct-1);
		}; //DONE
		void clear();
		const_iterator insert(const_iterator position, const T &t); //DONE
		const_iterator erase(const_iterator position); //DONE
		T operator[](int index) const;
		void set(const T &value,int index) {
			int rover = readNext(0);
			for (int i = 0; i != index; ++i) {
				rover = readNext(rover);
			}
			writeData(rover,value);
		}//DONE
		void set(const T &value,const_iterator position) {
			std::cout<< "enter set2" <<std::endl;
			int rover = position.pos;
			writeData(rover,value);
			std::cout<< "exit set2" <<std::endl;
		}//DONE
	
		const_iterator begin() { return const_iterator(readNext(0),fptr); }	
		const_iterator begin() const { return const_iterator(readNext(0),fptr); }	
		const_iterator cbegin() const { return const_iterator(readNext(0), fptr); }	
		const_iterator end() { return const_iterator(0,fptr); }	
		const_iterator end() const { return const_iterator(0,fptr); }	
		const_iterator cend() const { return const_iterator(0,fptr); }	
};

template<typename T>
FileLinkedList<T>::FileLinkedList(const std::string &fname) {
	fptr = std::fopen(fname.c_str(),"r+b");
	//std::cout << fptr << std::endl;
	if (fptr==nullptr)fptr = std::fopen(fname.c_str(),"w+b");
	std::fseek(fptr,0,SEEK_END);
	if(std::ftell(fptr) > 0) {
		// seek to the beginning
		std::fseek(fptr,0,SEEK_SET);
		//  read ct into variable
		std::fread(&ct,sizeof(int),1,fptr);
		// read ff
		std::fread(&firstfree,sizeof(int),1,fptr);
	} else {
	        ct = 1;
		firstfree = -1;
		writeCtAndFF();
		T garbage;
		writeNode(0,0,garbage,0);
	}
}

template<typename T>
typename FileLinkedList<T>::const_iterator FileLinkedList<T>::erase(const_iterator position) {
	int curr = position.pos;
	int prev = readPrev(curr);
	int next = readNext(curr);
	writePrev(next,prev);
	writeNext(prev,next);
	writeNext(curr,firstfree); //current's next is now the original first free
	firstfree = curr;
	--ct;
	writeCtAndFF();

	return const_iterator(next,fptr);
}

template<typename T>
typename FileLinkedList<T>::const_iterator FileLinkedList<T>::insert(const_iterator position, const T &t) {
	int next = position.pos;
	int prev = (--position).pos;
	int nPos;

	if(firstfree > 0) {
		nPos = firstfree;
		firstfree = readNext(firstfree);
	} else {
		nPos = ct;
	}

	writeNext(prev, nPos);
	writePrev(next, nPos);
	++ct;
	writeCtAndFF();

	writeNode(nPos, prev, t, next);
	
	position = const_iterator(nPos, fptr);

	return position;
}

template<typename T>
T FileLinkedList<T>::operator[](int index) const {
	int rover = readNext(0);
	for (int i = 0; i != index; ++i) rover = readNext(rover);
	T data;
	readData(rover,data);
	return data;
}

template<typename T>
void FileLinkedList<T>::clear() {
	ct = 1;
	firstfree = -1;
	writeNext(0,0);
	writePrev(0,0);
	writeCtAndFF();
}

template<typename T>
void FileLinkedList<T>::push_back(const T &t) {
	int nPos;
	if (firstfree < 0) {
		nPos = ct;
	} else {
		nPos = firstfree; //node position is ff
		firstfree = readNext(firstfree); //ff is now next ff	
	}
	int prev = readPrev(0);
	writeNode(nPos,prev,t,0);
	writeNext(prev,nPos); //prev's next is new node
	writePrev(0,nPos);
	++ct;
	writeCtAndFF();
}

template<typename T>
void FileLinkedList<T>::pop_back() {
	//get last node
	//go to prev of last node
	int rover = readNext(0); //first node
	while(readNext(rover) != 0) {
		rover = readNext(rover);
	} //should get the last node

	int last = readPrev(rover);
	writeNext(last, 0); //last's next is now the sent
	writePrev(0, last); //sent's prev is now the last node
	writeNext(rover,firstfree); //deleted node's next is now first free
	firstfree = rover; //first free updated to rover
	--ct;
	writeCtAndFF();
}

template<typename T>
void FileLinkedList<T>::writeCtAndFF() {
	std::fseek(fptr,0,SEEK_SET);
	std::fwrite(&ct,sizeof(int),1,fptr);
	std::fwrite(&firstfree,sizeof(int),1,fptr);
}

template<typename T>
void FileLinkedList<T>::writeData(int pos, const T &data) {
	std::fseek(fptr, 3*sizeof(int)+pos*(sizeof(T)+2*sizeof(int)), SEEK_SET);
	std::fwrite(&data,sizeof(T),1,fptr);
}

template<typename T>
void FileLinkedList<T>::readData(int pos, T &data) const {
	std::fseek(fptr,3*sizeof(int)+pos*(sizeof(T)+2*sizeof(int)),SEEK_SET);
	std::fread(&data,sizeof(T),1,fptr);
}

template<typename T>
void FileLinkedList<T>::writeNode(int pos, int prev, const T &t, int next) {
	std::fseek(fptr,2*sizeof(int)+pos*(sizeof(T)+2*sizeof(int)),SEEK_SET);
	std::fwrite(&prev,sizeof(int),1,fptr);
	std::fwrite(&t,sizeof(T),1,fptr);
	std::fwrite(&next,sizeof(int),1,fptr);
}

template<typename T>
void FileLinkedList<T>::writeNext(int pos, int next) {
	std::fseek(fptr,2*sizeof(int) + pos*(sizeof(T)+2*sizeof(int)) + sizeof(int) + sizeof(T),SEEK_SET);
	std::fwrite(&next,sizeof(int),1,fptr);
}

template<typename T>
void FileLinkedList<T>::writePrev(int pos, int prev) {
	std::fseek(fptr,2*sizeof(int) + pos*(sizeof(T)+2*sizeof(int)),SEEK_SET);
	std::fwrite(&prev,sizeof(int),1,fptr);
}

template<typename T>
int FileLinkedList<T>::readPrev(int pos) const{
	int prev;
	
	std::fseek(fptr,2*sizeof(int) + pos*(sizeof(T)+2*sizeof(int)),SEEK_SET);
	std::fread(&prev,sizeof(int),1,fptr);

	return prev;
}

template<typename T>
int FileLinkedList<T>::readNext(int pos) const{
	int next;
	
	std::fseek(fptr,3*sizeof(int) + sizeof(T) + pos*(sizeof(T)+2*sizeof(int)),SEEK_SET);
	std::fread(&next,sizeof(int),1,fptr);

	return next;
}
