#ifndef AFL_SORT_H
#define AFL_SORT_H


#include "types.h"
#include "config.h"
#include "debug.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/file.h>

//声明
extern FILE* distance_file; /*the file to record the distance*/ //defined in afl-fuzz 这个要修改成

struct queue_entry
{

	u8* fname; /* File name for the test case      */
	u32 len; /* Input length                     */

	u8 cal_failed, /* Calibration failed?              */
	trim_done, /* Trimmed?                         */
	passed_det, /* Deterministic stages passed?     */
	has_new_cov, /* Triggers new coverage?           */
	var_behavior, /* Variable behavior?               */
	favored, /* Currently favored?               */
	fs_redundant; /* Marked as redundant in the fs?   */

	u32 bitmap_size, /* Number of bits set in bitmap     */
	fuzz_level, /* Number of fuzzing iterations     */ //被fuzz过的次数吗，文章中的s(i)
			exec_cksum; /* Checksum of the execution trace  */

	u64 exec_us, /* Execution time (us)              */
	handicap, /* Number of queue cycles behind    */
	depth; /* Path depth                       */

	u8* trace_mini; /* Trace bytes, if kept             */
	u32 tc_ref; /* Trace bytes ref count            */

	struct queue_entry *next, /* Next element, if any             */
	*next_100; /* 100 elements ahead               */

#ifdef XIAOSA
	u32 parent_id; /* the parent test case id*/
	u32 self_id; /* the self test case id*/
	u8* change_op; /* mark the change operate*/
	u32 nm_child; /* count the child number*/
	u32 nm_crash_child; /* count the crash child number*/
	u8* fuzz_one_time; /*the time of function of fuzzone, in the level of second*/
	u8 in_top_rate; /*to mark the testcase is in the top_rate*/
	u8 has_in_trace_plot; /*to mark if it has been save in plot file*/
	u8 kill_signal; /*save the signal value if it has, 0 means no*/
#endif

#ifdef SORT
	u8* id; /*the short name of the testcase*/
#endif

};

typedef struct distance_power {
    double distance;
    u8* fname_min;
    u32 fmin_bitmap_size;
    u8* fname_max;
    u32 fmax_bitmap_size;
}DP; // the distance between two inputs
//左边为旧,小号;右边是新,大号,这里的大小表示id

enum {
    /* 00 */ NO_SORT_0,
    /* 01 */ Random_Sort_1,
    /* 02 */ BT_dup_Sort_2,
    /* 03 */ BT_no_dup_Sort_3,
    /* 04 */ BA_Sort_4,
	/* 05 */ MIn_Max_Sort_5,
	/* 06 */ Short_first_Sort_6,
	/* 07 */ Short_by_hamming_7,
};


//function declaration---------------------------------------------------------------------
//这里的函数在cpp中实现,在c中调用
#ifdef _cplusplus // 表示下面的定义是cpp文本中的函数
extern "C" {
#endif


// Initialize the searcher.
// Arg1: search_strategy: specify which searcher will use
// Return: 1 if intilized successfully, otherwise 0.
u8 initSort(u8 sort_id);

//when new testcase generated
void on_new_seed_generated(struct queue_entry *queue, struct queue_entry *q);


//void update_distance_file(u8* fnamer, u8* fnamel, double distance);
void update_sort_file();





#ifdef _cplusplus
}
#endif
//function declaration end--------------------------------------------------------------------

#endif //AFL_SORT_H
