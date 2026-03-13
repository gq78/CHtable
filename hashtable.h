#pragma once

typedef struct HASHTABLE HASHTABLE;


typedef enum { STRING_V, INT_V, FLOAT_V } ValueABSTRType;

typedef union {
	int ival;
	float fval;
	char* sval;
} Vtype;



typedef int (*FILTERFUNC)(const char* key, ValueABSTRType abstrtype, Vtype value);
typedef int (*APPLYFUNC)(const char* key, ValueABSTRType abstrtype, Vtype *value);

HASHTABLE* create_htable();
HASHTABLE* clone_htable(HASHTABLE* htable);
void delete_htable(HASHTABLE* htable);
void clear_htable(HASHTABLE* htable);


HASHTABLE* select_all_which(HASHTABLE* htable, FILTERFUNC filter);
void apply_to_all(HASHTABLE* htable, FILTERFUNC filter);

void printf_full_htable(HASHTABLE* htable);
void printf_ht_element(HASHTABLE* htable, char* key);

char** get_keys(HASHTABLE* htable);
void* get_value(HASHTABLE* htable, char* key);

int is_exists(HASHTABLE* htable, char* key);

void insert_data_str_int(HASHTABLE* htable, char* key, int value);
void insert_data_str_float(HASHTABLE* htable, char* key, float value);
void insert_data_str_str(HASHTABLE* htable, char* key, char* value);

int delete_data(HASHTABLE* htable, char* key);



void ht_reallocation(HASHTABLE* htable, int new_tcapacity);
float fill_factor_calculator(HASHTABLE* htable);

int get_htcapacity(HASHTABLE* ht);
int get_htsize(HASHTABLE* htable);