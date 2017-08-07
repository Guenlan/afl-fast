#include "sort.h"




//Sort ----------------------------------------------------
Sort::~Sort()
{

}

u8 Sort::on_new_seed_generate(struct queue_entry *queue, struct queue_entry *q)
{
	//虚函数,果子类不定义,则调用的是这个
	return 1;
}

void Sort::update_sort_file()
{
	//虚函数,果子类不定义,则调用的是这个
}

//end Sort

//0----------------------------------------------------
//无定义
//ok
//end 0

//1 ----------------------------------------------------
//无定义,在angr出排序
//ok
//end 1 ---------------------------------------------------------------------------------------------


//2 BT_dup_Sort----------------------------------------------------
//自定义排序
struct comp_BTD_2 {
    bool operator ()(const BTD_2& left, const BTD_2& right) {
        return (left.length >= right.length); //这个是从大到小
    }
};
set<BTD_2,comp_BTD_2>BTDS_2;

u8 BT_dup_Sort::on_new_seed_generate(struct queue_entry *queue, struct queue_entry *q){
	//每次生成新的测试用例,都进行基本块统计,然后排序
	//插入新的记录
	BTD_2 tmp;
	tmp.short_name = q->short_name; //指针的复制
	tmp.length = q->bitmap_size_dup; //轨迹的长度
	BTDS_2.insert(tmp); //插入之后是有序的

	//输出总的记录
	update_sort_file();
	return 0;
}

void BT_dup_Sort::update_sort_file(){
	fseek(afl_to_angr, 0, 0);	//移动到文件头,每次都重新覆盖
	set<u8*> cached_list; //记录record的name
	fprintf(afl_to_angr, "#short_name; dup_size\n");
	fflush(afl_to_angr);
	// 遍历 set
	for (auto it = BTDS_2.begin(), end = BTDS_2.end(); it != end; it++) {
		BTD_2 tmp = *it; //按照distance下的顺序进行
		if ( !cached_list.empty() ) {
			if ( cached_list.find(tmp.short_name) != cached_list.end() ) continue;
		}
		fprintf(afl_to_angr, "%s; %d\n", tmp.short_name, tmp.length);
		cached_list.insert(tmp.short_name);
		fflush(afl_to_angr);
	}
}
//end 2 ---------------------------------------------------------------------------------------------


//3 --------------------------------------------------------------------------------------------------
//自定义排序
struct comp_BTND_3 {
    bool operator ()(const BTND_3& left, const BTND_3& right) {
        return (left.length >= right.length); //这个是从大到小
    }
};
set<BTND_3,comp_BTND_3>BTNDS_3;

u8 BT_no_dup_Sort::on_new_seed_generate(struct queue_entry *queue, struct queue_entry *q){
	//每次生成新的测试用例,都进行基本块统计,然后排序
	//插入新的记录
	BTND_3 tmp;
	tmp.short_name = q->short_name;
	tmp.length = q->bitmap_size; //轨迹的长度
	BTNDS_3.insert(tmp); //插入之后是有序的
	//输出总的记录
	update_sort_file();
	return 0;
}

void BT_no_dup_Sort::update_sort_file(){
	fseek(afl_to_angr, 0, 0);	//移动到文件头,每次都重新覆盖
	set<u8*> cached_list; //记录record的name
	// 遍历 set
	fprintf(afl_to_angr, "# short_name, no_dup_size\n");
	for (auto it = BTNDS_3.begin(), end = BTNDS_3.end(); it != end;	it++) {
		BTND_3 tmp = *it; //按照distance下的顺序进行
		if (!cached_list.empty()) {
			if (cached_list.find(tmp.short_name) != cached_list.end())	continue;
		}
		fprintf(afl_to_angr, "%s; %d\n", tmp.short_name, tmp.length);
		cached_list.insert(tmp.short_name);
		fflush(afl_to_angr);
	}
}
//end 3 ---------------------------------------------------------------------------------------------


//4 ----------------------------------------------------
set<u8*> BAS_4; //记录input_mini的地址
u8 BA_Sort::on_new_seed_generate(struct queue_entry *queue, struct queue_entry *q){
	//不需要任何输出

	//输出所有轨迹的指针
	//输出一个文件，包含了所有当前元组轨迹的指针
    // depend on macro XIAOSA
//#ifndef XIAOSA
//	return 1
//#endif
////	if (q->trace_mini_path){
////		u8* new_trace_path=q->trace_mini_path;
////		BAS_4.insert(new_trace_path);
////		update_sort_file();
////	}
////	//输出总的记录
	return 1;
}

void BA_Sort::update_sort_file(){
//	fseek(afl_to_angr, 0, 0);	//移动到文件头,每次都重新覆盖
//	fprintf(afl_to_angr, "shortname \n" );
//
//	for (auto it = BAS_4.begin(), end = BAS_4.end(); it != end;	it++) {
//			u8* tmp = *it; //按照distance下的顺序进行
//			fprintf(afl_to_angr, "%s \n", tmp);
//			fflush(afl_to_angr);
//		}
//	fflush(afl_to_angr);

}
//end 4 ---------------------------------------------------------------------------------------------


//5 --------------------------------------------------------------------------------------------------
//自定义排序,仿函数,重载括号运算符
struct comp_MIMA_5 {
    bool operator ()(const MIMA_5& a, const MIMA_5& b) { //重载 () 运算, 自定义的仿函数
        return (a.min_distance >= b.min_distance); //这个是从大到小, 返回true的, 左边的在前
    }
};
set<MIMA_5,comp_MIMA_5> MIMAS_5; //给了 set 一个 comp_min_max 比较函数,默认是less()判断函数

u8 Min_Max_Sort::on_new_seed_generate(struct queue_entry *queue, struct queue_entry *q){
	//计算出当前测试用例和之前所有测试用例的hanmming距离,选取最小值作为其属性(不能是本身)
	//如果算出一个hamming距离比之前的测试用例的属性还要小,则替换
	//按照属性进行从大到小

	struct queue_entry *queue_cur = queue;
	double mini_dis =655360, cur_dis=655360; //默认很大
	u8* closet_test;
	while (queue_cur) {
		if (queue == q) {
			//只有一个testcase的情况
			q->min_distance=655360; //默认为 655360,无穷远
			q->close_testcase=q->short_name;
			closet_test= q->short_name;
			break; // the last is the self
		}
		if(queue_cur==q) break; // the last one
		cur_dis = cal_ham_distance(queue_cur, q); //左边为旧,小号;右边是新,大号 //有可能是为0的

		//算出与其他测试用例的hanming距离,选取最小的hanming距离作为属性
		if (mini_dis > cur_dis) {
			mini_dis = cur_dis;
			closet_test = queue_cur->short_name; //与当前测试用例形成最小hanmming距离的测试用例
		}

		//更新之前的测试用例的属性
		if (cur_dis < queue_cur->min_distance) {
			//遍历min_max_set,找到对应的,删除旧,加新
			for (auto it = MIMAS_5.begin(), end = MIMAS_5.end(); it != end; it++) {
				MIMA_5 tmp = *it;
				if (tmp.self_fname == queue_cur->short_name) {
					MIMAS_5.erase(it);
					//generate a new record,指向当前测试用例
					MIMA_5 tmp_mima;
					tmp_mima.self_fname = queue_cur->short_name;
					tmp_mima.mini_target_fname = q->short_name;
					tmp_mima.min_distance = cur_dis;
					MIMAS_5.insert(tmp_mima);
					break;
				}
			}
			//更新 queue对象中的数值
			queue_cur->min_distance = cur_dis;
			queue_cur->close_testcase = q->short_name;
		}
		queue_cur = queue_cur->next;
	}

	//插入新的记录
	MIMA_5 new_mima;
	new_mima.self_fname=q->short_name;
	new_mima.mini_target_fname=closet_test;
	new_mima.min_distance=mini_dis;
	MIMAS_5.insert(new_mima);

	q->min_distance=mini_dis;
	q->close_testcase=closet_test;

	//输出总的记录
	update_sort_file();
	return 1;

}

void Min_Max_Sort::update_sort_file(){
	//n个测试用例,就有n(n-1)/2次比较
	fseek(afl_to_angr, 0, 0); //移动到文件头,每次都重新覆盖
	fprintf(afl_to_angr, " short_name; short_name; min_max \n");
	// 遍历 set
	for (auto it = MIMAS_5.begin(), end = MIMAS_5.end(); it != end; it++) {
		MIMA_5 tmp = *it; //按照distance下的顺序进行
		if (tmp.min_distance==0) continue;
		//以最小hamming距离作为属性,按属性从大到小排序.
		fprintf(afl_to_angr, "%s; %s; %d \n", tmp.self_fname, tmp.mini_target_fname,tmp.min_distance);
		fflush(afl_to_angr);
		}
}
//end 5 ---------------------------------------------------------------------------------------------


//6 ----------------------------------------------------
////自定义排序
struct comp_SFND_6 {
    bool operator ()(const SFND_6& left, const SFND_6& right) {
        return (left.length <= right.length); //这个是从小到大
    }
};
set<SFND_6,comp_SFND_6>SFNDS_6;

u8 Short_first_nodup_Sort::on_new_seed_generate(struct queue_entry *queue, struct queue_entry *q){
	//每次生成新的测试用例,都进行基本块统计,然后排序
	//插入新的记录
	SFND_6 tmp;
	tmp.short_name = q->short_name;
	tmp.length = q->bitmap_size;
	SFNDS_6.insert(tmp); //插入之后是有序的
	//输出总的记录
	update_sort_file();
	return 0;
}

void Short_first_nodup_Sort::update_sort_file(){
	fseek(afl_to_angr, 0, 0);	//移动到文件头,每次都重新覆盖
	fprintf(afl_to_angr, "short_name; no_dup_size \n");
	set<u8*> cached_list; //记录record的name
	// 遍历 set
	for (auto it = SFNDS_6.begin(), end = SFNDS_6.end(); it != end; it++) {
		SFND_6 tmp = *it; //按照distance下的顺序进行
		if (!cached_list.empty()) {
			if (cached_list.find(tmp.short_name) != cached_list.end())
				continue;
		}
		fprintf(afl_to_angr, "%s; %d\n", tmp.short_name, tmp.length);
		cached_list.insert(tmp.short_name);
		fflush(afl_to_angr);
	}
}
//end 6 ---------------------------------------------------------------------------------------------


//7 ----------------------------------------------------------------------
//增加一些变量定义

//左边为旧,小号;右边是新,大号,这里的大小表示id
//自定义排序
struct comp_DP_7 {
    bool operator ()(const DP_7& dpl, const DP_7& dpr) {
        return (dpl.distance >= dpr.distance); //这个是从大到小,距离一样
    }
}; // comp_greater 成了一个仿函数,即一个类有函数一样的实现形式
// 这里的 <> 表示模板
set<DP_7,comp_DP_7> DPS_7; // 记录所有的轨迹, 这个在c中没法调用 对于元素是自定义的结构体,还需要重写排序算法

u8 Short_by_hamming::on_new_seed_generate(struct queue_entry *queue, struct queue_entry *q){
	struct queue_entry *queue_cur = queue;
	double distance = 0;
	while (queue_cur) {
		if (queue_cur == q) {
			break; // the last is the self
		}
		distance = cal_ham_distance(queue_cur, q); //左边为旧,小号;右边是新,大号
		//生成一调新的记录
		DP_7 tmp;
		tmp.distance = distance;
		tmp.fname_small = queue_cur->fname;
		tmp.fname_big = q->fname;
		tmp.fbig_bitmap_size=q->bitmap_size;
		tmp.fsmall_bitmap_size=queue_cur->bitmap_size;

		DPS_7.insert(tmp); //插入之后是有序的
		queue_cur = queue_cur->next;
	}
	//输出总的记录
	update_sort_file();
	return 0;
}

void Short_by_hamming::update_sort_file() {
	//n个测试用例,就有n(n-1)/2次比较
	fseek(afl_to_angr, 0, 0); //移动到文件头,每次都重新覆盖
	fprintf(afl_to_angr, "7 short_name; short_name; distance \n");
	fflush(afl_to_angr);
	set<u8*> cached_list;
	// 遍历 set
	for (auto it = DPS_7.begin(), end = DPS_7.end(); it != end; it++) {
		DP_7 tmp = *it; //按照distance下的顺序进行
		//只记录轨迹长的
		if (tmp.fbig_bitmap_size > tmp.fsmall_bitmap_size) //max 表示id大的,即是新的,但是新的也不一定好
		{
			if (cached_list.empty() or cached_list.find(tmp.fname_big) == cached_list.end()) {
				fprintf(afl_to_angr, "%s; %s; %0.f \n", "", tmp.fname_big,	tmp.distance);
				cached_list.insert(tmp.fname_big);
			}
		}
		else {
			if (cached_list.empty() or cached_list.find(tmp.fname_small)== cached_list.end()) {
				fprintf(afl_to_angr, "%s; %s; %0.f \n", tmp.fname_small, "",tmp.distance);
				cached_list.insert(tmp.fname_small);
			}
		}
	}
	fflush(afl_to_angr);
}

//end 7 ---------------------------------------------------------------------------------------------

//通用函数定义 ---------------------------------------------------------------------------------------------
double cal_ham_distance(struct queue_entry *qmin,struct queue_entry *qmax)
{
	// bitmap_size 基本块数量
	if (!qmin->trace_mini and !qmax->trace_mini) //trace_mini每一位表示一个元组
	{
		return 0;
	}

	double dot = 0.0;
	for (unsigned int i = 0u; i < (MAP_SIZE >> 3); i++)
	{
		//提取出不同的元组关系,利用异或提取
		u8 tmp=qmin->trace_mini[i] ^ qmax->trace_mini[i];
		while(tmp)
		{
			dot++;
			tmp &=(tmp-1);

		}
	}
	return dot;
}
