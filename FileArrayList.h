#include <cstdio>
#include <string>
#include <iostream>

template<typename T>
class FileArrayList {
        FILE * fptr;
        int ct;

        FileArrayList(const FileArrayList<T> &that) = delete;
        FileArrayList<T> operator=(const FileArrayList<T> &that) = delete;
        void writeCt();
        void writeElem(int pos, T &t);
        void readElem(int pos, T &t);
        public:
                typedef T value_type;
                class const_iterator {
                        int pos;
                        FILE * fl;
                        public:
                                const_iterator(int i, FILE * f): pos(i), fl(f) {}
                                const_iterator(const const_iterator &i): pos(i.pos), fl(i.fl) {}
                                const_iterator& operator++() {++pos; return *this;}
				const_iterator& operator--() {--pos; return *this;}
				const_iterator operator++(int) {
					const_iterator i(*this);
					++pos;
					return *i;
				}
				const_iterator operator--(int) {
					const_iterator i(*this);
					--pos;
					return *i;
				}
				bool operator==(const const_iterator &i) { return pos==i.pos && fl==i.fl; }
                                bool operator!=(const const_iterator &i) { return !(*this==i);}
                                T operator*() {
                                        std::fseek(fl,sizeof(int)+(pos*sizeof(T)),SEEK_SET);
                                        T ret;
                                        std::fread(&ret,sizeof(T),1,fl);
                                        return ret;
                                }
				T getData() {
					std::fseek(fl,sizeof(int)+(pos*sizeof(T)),SEEK_SET);
                                        T ret;
                                        std::fread(&ret,sizeof(T),1,fl);
                                        return ret;
				}
				int getPos() { return pos; }

                };

                // General Methods
                FileArrayList(const std::string &fname);
		
		template<typename I> // The type I will be an iterator.
		FileArrayList(I begin,I end,const std::string &fname) {
			std::cout << "enter copy const" << std::endl;
			fptr = std::fopen(fname.c_str(),"w+b"); //to overwrite the file
			//BEGIN STARTS AT arr[0]
			clear(); //set ct to 0 and writes
			T tmp;
			std::fseek(fptr, sizeof(int), SEEK_SET); //set seek just after ct
			while(begin != end) {
				tmp = begin.getData();
				std::fseek(fptr, sizeof(int) + (ct*sizeof(T)), SEEK_SET);
				std::fwrite(&tmp, sizeof(T), 1, fptr);
				++ct;
				std::cout << size() << std::endl;
				++begin;
			}
			writeCt();
			std::cout << "exit copy const" << std::endl;
		}
                
		~FileArrayList() { std::fclose(fptr); };
                void push_back(const T &t);
                void pop_back();
		int size() const { return (ct); }
                void clear() {ct = 0; writeCt(); }
		const_iterator insert(const_iterator position, const T &t) {
			// shift all data forward one (starting from the end and working back to position)
			std::cout << "enter insert" << std::endl;
			T tmp;
			for(int i = ct; i != position.getPos(); --i){
				std::fseek(fptr, sizeof(int) + ((i-1) * sizeof(T)), SEEK_SET); //so you can read the data
				std::fread(&tmp, sizeof(T), 1, fptr); //reading data
				std::fseek(fptr, sizeof(int) + (i * sizeof(T)), SEEK_SET); //if you write after this, it'll be one forward
				std::fwrite(&tmp,sizeof(T),1,fptr); //write data one forward
			}
			std::fseek(fptr, sizeof(int) + ((position.getPos()) * sizeof(T)), SEEK_SET);
			std::fwrite(&t,sizeof(T),1,fptr);
			++ct;
			writeCt();
			std::cout << "exit insert" << std::endl;
		}
		T operator[](int index) const {
			std::cout << "enter []" << std::endl;
			T ret;
			std::fseek(fptr,sizeof(int)+((index)*sizeof(T)),SEEK_SET);
			std::fread(&ret, sizeof(T), 1, fptr);
			return ret;
			std::cout << "exit []" << std::endl;
		}
		const_iterator erase(const_iterator position) {
			std::cout << "enter erase" << std::endl;
			T tmp;
			for(int i = position.getPos(); i != ct-1; ++i) {
				std::fseek(fptr, sizeof(int) + ((i+1)*sizeof(T)), SEEK_SET); //cursor is set to read data right after erase spot
				std::fread(&tmp, sizeof(T), 1, fptr); //read data after value to be erased
				std::fseek(fptr, sizeof(int) + ((i)*sizeof(T)), SEEK_SET); //cursor set one behind to replace
				std::fwrite(&tmp,sizeof(T),1,fptr); //data has been shifted back one!
			}

			--ct;
			writeCt();
			std::cout << "exit erase" << std::endl;
		}
		void set(const T &value,int index) {
			std::cout << "enter set" << std::endl;
			T tmp;
			std::fseek(fptr, sizeof(int) + (index * sizeof(T)), SEEK_SET); //set pointer before data
			std::fwrite(&value,sizeof(T),1,fptr); //overwrite data
			std::cout << "exit set" << std::endl;
		}

                // iterator stuff
                const_iterator begin() { return const_iterator(0,fptr); }
                const_iterator end() { return const_iterator(ct,fptr);}
                const_iterator begin() const { return const_iterator(0,fptr); }
                const_iterator end() const { return const_iterator(ct,fptr); }
                const_iterator cbegin() const {return const_iterator(0,fptr);}
                const_iterator cend() const { return const_iterator(ct,fptr);}
};

template<typename T>
FileArrayList<T>::FileArrayList(const std::string &fname) {
	std::cout << "enter constructor" << std::endl;
        fptr = std::fopen(fname.c_str(),"r+b");
        //std::cout << fptr << std::endl;
        if (fptr==nullptr)fptr = std::fopen(fname.c_str(),"w+b");
        std::fseek(fptr,0,SEEK_END);
        if(std::ftell(fptr) > 0) {
                // seek to the beginning
                std::fseek(fptr,0,SEEK_SET);
                //      read ct into variable
                std::fread(&ct,sizeof(int),1,fptr);
        } else {
                ct = 0;
                std::fwrite(&ct,sizeof(int),1,fptr);
        }
	std::cout << "exit constructor" << std::endl;
}

template<typename T>
void FileArrayList<T>::push_back(const T &t) {
	std::cout << "enter pushback" << std::endl;
        // go to ct spot in file
        std::fseek(fptr, sizeof(int) + (ct * sizeof(T)), SEEK_SET);
        // write info to data structure
        std::fwrite(&t,sizeof(T),1,fptr);
        // increase ct
        ++ct;
        // write ct
        writeCt();
	std::cout << "exit pushback" << std::endl;
}

template<typename T>
void FileArrayList<T>::pop_back() {
	std::cout << "enter popback" << std::endl;
	--ct;
	writeCt();
	std::cout << "exit popback" << std::endl;
}

template<typename T>
void FileArrayList<T>::writeCt() {
        std::fseek(fptr,0,SEEK_SET);
        std::fwrite(&ct,sizeof(int),1,fptr);
}
