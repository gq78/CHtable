#include "hashtable.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void more_than_40(const char* key, ValueABSTRType abstrtype, Vtype *value) {

	if (abstrtype != INT_V)
		return ;
	value->ival *= 10;
}

int main() {
	HASHTABLE* ht = create_htable();
	insert_data_str_int(ht, "Mikki", 50);
	insert_data_str_int(ht, "Ann", 55640);
	insert_data_str_int(ht, "Anton", 12);
	insert_data_str_int(ht, "Lilo", 4);
	insert_data_str_float(ht, "Ucne", 556.24);
	insert_data_str_int(ht, "Georg", 540);

	printf_full_htable(ht);

	HASHTABLE* new_h = clone_htable(ht);
	printf_full_htable(new_h);
	apply_to_all(new_h, more_than_40);
	printf_full_htable(new_h);
	


	

		
	return 0;
}