#ifndef SORT_H
#define SORT_H

//不能在头文件中做定义,否则会多次定义, 在头文件中只有声明
extern "C"{
#include "afl-sort.h" //这里申明的函数,在cpp中定义,在c中调用
}

#include <set>
#include <map>  //必须在cpp中

using namespace std;

//变量声明
class Sort;
extern "C" Sort* AFLSort;  // 表示以c语言的形式编译



//*********************************************************************
//parent
class Sort
{
public:
	u32 aa; // 输出文件的位置
	u32 sort_id;
public:
	Sort():aa(NULL),sort_id(0){}


	virtual ~Sort(); //虚函数
//	virtual void on_new_seed_generated(struct queue_entry *queue, struct queue_entry *q);
//	virtual void update_sort_file();
};
//----------------------------------------------------------------------

//*********************************************************************
//0
class No_Sort : public Sort{
public:

public:
	No_Sort(): Sort() { }
	~No_Sort() {}

};

//----------------------------------------------------------------------

//*********************************************************************
//1
class Random_Sort : public Sort{
public:
	u8 a=0;
public:
	Random_Sort(): Sort() { }
	~Random_Sort() {}

public:
	u8 aa();



};

//----------------------------------------------------------------------


//*********************************************************************
//2
class BT_dup_Sort: public Sort{
public:
	BT_dup_Sort(): Sort() { }
	~BT_dup_Sort() {}
};

//----------------------------------------------------------------------


//*********************************************************************
//3
class BT_no_dup_Sort: public Sort{
public:
	BT_no_dup_Sort(): Sort() { }
	~BT_no_dup_Sort() {}
};

//----------------------------------------------------------------------

//*********************************************************************
//4
class BA_Sort: public Sort{
public:
	BA_Sort(): Sort() { }
	~BA_Sort() {}
};

//----------------------------------------------------------------------


//*********************************************************************
//5
class MIn_Max_Sort: public Sort{
public:
	MIn_Max_Sort(): Sort() { }
	~MIn_Max_Sort() {}
};

//----------------------------------------------------------------------


//*********************************************************************
//6
class Short_first_Sort: public Sort{
public:
	Short_first_Sort(): Sort() { }
	~Short_first_Sort() {}
};

//----------------------------------------------------------------------


//*********************************************************************
//7
class Short_by_hamming: public Sort{
public:
	Short_by_hamming(): Sort() { }
	~Short_by_hamming() {}

//	void on_new_seed_generated(struct queue_entry *queue, struct queue_entry *q);
//	void update_sort_file();
};

//----------------------------------------------------------------------



double cal_two_input(struct queue_entry *ql,struct queue_entry *qr);


#endif //SORT_H
