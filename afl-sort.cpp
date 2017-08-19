#include "sort.h"

Sort* AFLSort = NULL; //

//function---------------------------------------------------------------------------
u8 initSort(u8 sort_id) {
	switch (sort_id) {
	//0
	case NO_SORT_0: {
		AFLSort = new No_Sort(sort_id);
		break;	}
	case Random_Sort_1: {
		AFLSort = new  Random_Sort(sort_id);
		break;	}
	case BT_dup_Sort_2: {
		AFLSort = new BT_dup_Sort(sort_id);
		break;	}
	case BT_no_dup_Sort_3: {
		AFLSort = new BT_no_dup_Sort(sort_id);
		break;	}
	case BA_Sort_4: {
		AFLSort = new BA_Sort(sort_id);
		break;	}
	case Min_Max_Sort_5: {
		AFLSort = new Min_Max_Sort(sort_id);
		break;	}
	case Short_first_Sort_6: {
		AFLSort = new Short_first_nodup_Sort(sort_id);
		break;	}
	case Short_by_hamming_7: {
			AFLSort = new Short_by_hamming(sort_id);
			break;	}

	default:
		break;
	}

	return 1;
}

//when new test-cases generated, call this function and output the sort sequence
void on_new_seed_generated(struct queue_entry *queue,struct queue_entry *q)//q1 是最新的
{
	AFLSort->on_new_seed_generate(queue,q);
}


