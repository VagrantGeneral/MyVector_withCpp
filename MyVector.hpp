#ifndef MY_VECTOR_HPP
#define MY_VECTOR_HPP
#include<string.h>
#include<stdlib.h> // malloc free ; exit;
#include<iostream>

template<class _Ty>	// _Ty char , int ,short ,double;// class   char *;
class MyVector {
private:
	_Ty* data; //  
	size_t cursize;//现存元素个数
	size_t capacity;//最大元素个数

	//扩容函数
	bool IncSize() {
		size_t count = capacity * incsize;//计算扩容后的对象个数
		_Ty* newdata = (_Ty*)malloc(sizeof(_Ty) * count);//为新的数据分配空间
		if (nullptr == newdata) {
			return false;
		}
		for (int i = 0; i < cursize; ++i) {
			//newdata[i] = data[i];	 // MyVector<Int> ivec;
			new(&newdata[i]) _Ty(std::move(data[i]));//首选（移动构造），避免拷贝（对象赋给空间）
		}
		for (int i = 0; i < cursize; ++i) {
			(&data[i])->~_Ty();//对原空间进行析构
		}

		free(data);释放空间
		data = newdata;//改变指向
		capacity = count;
		return true;
	}

	static const size_t initsize = 10;//初始化大小
	static const size_t incsize = 2;//扩容倍数
	
	//对新空间分配内存空间，清零初始化
	static _Ty* GetMem(size_t objsize) {
		_Ty* newdata = (_Ty*)malloc(sizeof(_Ty) * objsize);
		if (nullptr == newdata) exit(EXIT_FAILURE);
		memset(newdata, 0, sizeof(_Ty) * objsize);
		return newdata;
	}
public:
	MyVector(size_t sz = initsize)
		:data(nullptr), cursize(0), capacity(sz) {
		//data = new _Ty[capacity];
		data = GetMem(capacity);
	}

	//拷贝构造（接受左值引用
	MyVector(const MyVector& my) :data(nullptr) {
		cursize = my.cursize;
		capacity = my.capacity;
		data = GetMem(my.capacity);

		for (int i = 0; i < cursize; ++i) {

			new(&data[i]) _Ty(my.data[i]));

		}
	}	// MyVector<int> mlist(ylist);

	//移动构造（接受右值引用
	MyVector(MyVector&& my)
		:data(my.data), cursize(my.cursize), capacity(my.capacity) {
		data = nullptr;
		cursize = 0;
		capacity = 0;
	}

	//拷贝赋值
	MyVector& operator=(const MyVector& my) {
		cursize = my.cursize;
		capacity = my.capacity;
		data = GetMem(my.capacity);

		for (int i = 0; i < cursize; ++i) {

			new(&data[i]) _Ty(my.data[i]));

		}
	}

	//移动赋值
	MyVector& operator=(MyVector&& my) {
		
	}

	//
	~MyVector() {
		clear();    //  heap in obj ~_Ty();
		free(data); // .heap;
		data = nullptr;
		cursize = 0;
		capacity = 0;
		//delete[]data;	 //delete data;
	}

	size_t getsize() const { return cursize; }
	size_t getcapa() const { return capacity; }
	bool empty() const { return getsize() == 0; }
	bool full() const { return getsize() == getcapa(); }

	//在指定位置插入
	bool insert(int pos, const _Ty& val) {
		if (pos < 0 || pos > cursize) {
			return false;
		}
		if (full() && !IncSize()) {
			return false;
		}

		new(&data[cursize]) = _Ty();	//在最后一个位置的下一个位置创建_Ty空对象

		for (int i = cursize; i > pos; --i) {
			data[i] = std::move(data[i - 1]);
		}
		data[pos] = val;
		cursize += 1;
		return true;
	}

	//
	void push_back(const _Ty& val) {
		insert(cursize, val);
	}

	//
	bool erase(const size_t pos) {
		if (pos < 1 || pos > cursize - 1) {
			return false;
		}
		cursize -= 1;
		for (int i = pos; i < cursize; ++i) {
			data[i] = std::move(data[i + 1]);
		}
		return true;
	}

	//
	void pop_back() {
		erase(cursize - 1);
	}

	//对容器中的每一个对象执行析构，再清零
	void clear() {
		for (int i = 0; i < cursize; ++i) {
			(&data[i])->~_Ty();
		}
		cursize = 0;
	}

	//
	void swap(MyVector& vec) {
		std::swap(this->data, vec.data);
		std::swap(this->cursize, vec.cursize);
		std::swap(this->capacity, vec.capacity);
	}

	//
	void Print() const {
		std::cout << "Capa: " << capacity << std::endl;
		std::cout << "Cur: " << cursize << std::endl;
		std::cout << "data: " << std::endl;
		for (int i = 0; i < cursize; ++i) {
			std::cout << data[i] << " ";
		}
		std::cout << std::endl;
	}

};


#endif 
