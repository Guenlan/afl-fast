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

//通用函数声明
double cal_ham_distance(struct queue_entry *ql,struct queue_entry *qr);


//类声明
//parent ---------------------------------------------------------------------------------------------
class Sort
{
public:
	u8* outputfile; // 输出文件的位置
	u32 sort_id;
public:
	Sort(u32 sort_id):outputfile(0),sort_id(sort_id){}
	Sort():outputfile(0),sort_id(0){}


	virtual ~Sort(); //虚拟析构函数
	// 对于非纯虚函数,必须有声明, 有定义,才能编译通过
	virtual u8 on_new_seed_generate(struct queue_entry *queue, struct queue_entry *q);
	virtual void update_sort_file();
};
//end parent

//0 -----------------------------------------------------------------------------------------------
//没有任何操作
//ok
class No_Sort : public Sort{
public:

public:
	No_Sort(u32 sort_id): Sort(sort_id) { }
	~No_Sort() {}
};
//end 0 ---------------------------------------------------------------------------------------------

//1---------------------------------------------------------------------------------------------
//afl中没有任何操作,由angr进行乱序
//ok
class Random_Sort : public Sort{
public:
	Random_Sort(u32 sort_id): Sort(sort_id) { }
	~Random_Sort() {}

public:
};
//end 1 ---------------------------------------------------------------------------------------------

//2----------------------------------------------------------------------------------------------
class BT_dup_Sort: public Sort{
public:
	BT_dup_Sort(): Sort() { }
	BT_dup_Sort(u32 sort_id): Sort(sort_id) { }
	~BT_dup_Sort() {}

	//将测试用例按照路径长度排序
	virtual u8 on_new_seed_generate(struct queue_entry *queue, struct queue_entry *q);
	virtual void update_sort_file();
};

typedef struct BT_Dup {
    u8* short_name;
    u32 length; // the length of the testcase with duplicate blocks
}BTD_2; //BT set with duplicate blocks

//end 2 ---------------------------------------------------------------------------------------------



//3-----------------------------------------------------------------------------------------------
class BT_no_dup_Sort: public Sort{
public:
	BT_no_dup_Sort(u32 sort_id): Sort(sort_id) { }
	~BT_no_dup_Sort() {}

	virtual u8 on_new_seed_generate(struct queue_entry *queue, struct queue_entry *q);
	virtual void update_sort_file();
};

typedef struct BT_Nodup{
    u8* short_name;
    u32 length; // the length of the testcase with duplicate blocks
}BTND_3; //BT set with no duplicate blocks

//end 3 ---------------------------------------------------------------------------------------------




//4-------------------------------------------------------------------------------------------------
class BA_Sort: public Sort{
public:
	BA_Sort(u32 sort_id): Sort(sort_id) { }
	~BA_Sort() {}

	virtual u8 on_new_seed_generate(struct queue_entry *queue, struct queue_entry *q);
	virtual void update_sort_file();
};
//end 4 ---------------------------------------------------------------------------------------------

//5--------------------------------------------------------------------------------------------------
class Min_Max_Sort: public Sort{
public:
	Min_Max_Sort(u32 sort_id): Sort(sort_id) { }
	~Min_Max_Sort() {}

	virtual u8 on_new_seed_generate(struct queue_entry *queue, struct queue_entry *q);
	virtual void update_sort_file();
};

typedef struct min_max {
    u8* self_fname; //本身测试用例name
    u8* mini_target_fname; //对应测试用例的name
	u32 min_distance;// 最小hanming距离值作为属性
}MIMA_5;

//end 5 ---------------------------------------------------------------------------------------------



//6-------------------------------------------------------------------------------------------------
class Short_first_nodup_Sort: public Sort{
public:
	Short_first_nodup_Sort(u32 sort_id): Sort(sort_id) { }
	~Short_first_nodup_Sort() {}

	virtual u8 on_new_seed_generate(struct queue_entry *queue, struct queue_entry *q);
	virtual void update_sort_file();
};

typedef struct Short_First_Nodup {
    u8* short_name;
    u32 length; // the length of the testcase without duplicate blocks
}SFND_6; // without duplicate blocks
//end 6 ---------------------------------------------------------------------------------------------



//7--------------------------------------------------------------------------------------------------
// 第二篇文章用的hammming距离计算
class Short_by_hamming: public Sort{
public:
	Short_by_hamming(u32 sort_id): Sort(sort_id) { }
	~Short_by_hamming() {}

	virtual u8 on_new_seed_generate(struct queue_entry *queue, struct queue_entry *q); //父类中虚函数,子类中必须有声明
	virtual void update_sort_file();
};

typedef struct distance_power {
    double distance;
    u8* fname_min;
    u32 fmin_bitmap_size;
    u8* fname_max;
    u32 fmax_bitmap_size;
}DP_7; // the distance between two inputs
//end 7 ---------------------------------------------------------------------------------------------







#endif //SORT_H
