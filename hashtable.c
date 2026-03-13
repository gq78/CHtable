#define _CRT_SECURE_NO_WARNINGS

#include "HASHTABLE.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// STRUCTS ---------------------------------

typedef struct entry {
	struct entry* next;

	char* key;
	ValueABSTRType vabstrtype;
	Vtype value;

} Entry;


struct HASHTABLE  {
	Entry** buckets;
	int tsize;
	int tcapacity;
};



// STATIC INCLUDES ---------------------------

static char* str_dup(const char* s) {
	if (!s) return NULL;

	char* p;
	p = (char*)malloc(strlen(s) + 1);
	if (p != NULL) {
		strcpy(p, s);
		return p;
	}
	else {
		return NULL;
	}
}




static Entry* create_entry(const char* key, ValueABSTRType vabstrtype, Vtype value) {
	Entry* entry = malloc(sizeof(Entry));

	entry->key = str_dup(key);

	if (vabstrtype == STRING_V) {
		entry->value.sval = str_dup(value.sval);
	}
	else {
		entry->value = value;
	}
	entry->vabstrtype = vabstrtype;
	entry->next = NULL;

	return entry;
}


static unsigned hash(const char* s, int capacity) {
	unsigned hashval;
	for (hashval = 0; *s != '\0'; s++) {
		hashval = *s + 31 * hashval;
	}
	return hashval % capacity;
}


static void insert_data(HASHTABLE* htable, const char* key, ValueABSTRType vabstrtype, Vtype value) {
	unsigned index = hash(key, htable->tcapacity);

	Entry* curr = htable->buckets[index];
	while (curr != NULL) {
		if (strcmp(curr->key, key) == 0) {
			if (curr->vabstrtype == STRING_V) {
				free(curr->value.sval);
			}
			if (vabstrtype == STRING_V) {
				curr->value.sval = str_dup(value.sval);
			}
			else {
				curr->value = value;

			}
			curr->vabstrtype = vabstrtype;

			return;
		}
		curr = curr->next;
	}
	Entry* new_entry = create_entry(key, vabstrtype, value);
	new_entry->next = htable->buckets[index];
	htable->buckets[index] = new_entry;

	htable->tsize++;
	if (fill_factor_calculator(htable) >= 0.7) {
		ht_reallocation(htable, htable->tcapacity * 2);
	}
}


// MAIN API FUNCS  ------------------------

HASHTABLE* create_htable() {
	HASHTABLE* table = (HASHTABLE*)malloc(sizeof(HASHTABLE));

	table->tsize = 0;
	table->tcapacity = 100;
	table->buckets = calloc(table->tcapacity, sizeof(Entry*));

	return table;
}

HASHTABLE* clone_htable(HASHTABLE* htable) {
	HASHTABLE* new_htable = create_htable();

	for (int i = 0; i < htable->tcapacity; i++) {
		Entry* curr = htable->buckets[i];

		while (curr != NULL) {
			Entry* next = curr->next;
			insert_data(new_htable, curr->key, curr->vabstrtype, curr->value);
			curr = next;
		}
	}

	return new_htable;
}



void delete_htable(HASHTABLE* htable) {
	for (int i = 0; i < htable->tcapacity; i++) {
		Entry* curr = htable->buckets[i];

		while (curr != NULL) {
			Entry* next = curr->next;

			free(curr->key);
			if (curr->vabstrtype == STRING_V) {
				free(curr->value.sval);
			}
			free(curr);
			curr = next;
		}
	}

	free(htable->buckets);
	free(htable);
}

void clear_htable(HASHTABLE* htable) {
	for (int i = 0; i < htable->tcapacity; i++) {
		Entry* curr = htable->buckets[i];

		while (curr != NULL) {
			Entry* next = curr->next;

			free(curr->key);
			if (curr->vabstrtype == STRING_V) {
				free(curr->value.sval);
			}
			free(curr);
			curr = next;
		}
		htable->buckets[i] = NULL;
	}

	free(htable->buckets);
	htable->tsize = 0;
}



// API -------------------------------------


HASHTABLE* select_all_which(HASHTABLE* htable, FILTERFUNC filter) {
	HASHTABLE* new_htable = create_htable();

	for (int i = 0; i < htable->tcapacity; i++) {
		Entry* curr = htable->buckets[i];

		while (curr != NULL) {
			if (filter(curr->key, curr->vabstrtype, curr->value)) {
				insert_data(new_htable, curr->key, curr->vabstrtype, curr->value);
			}
			curr = curr->next;
		}
	}
	return new_htable;
}

void apply_to_all(HASHTABLE* htable, APPLYFUNC filter) {
	for (int i = 0; i < htable->tcapacity; i++) {
		Entry* curr = htable->buckets[i];
		while (curr != NULL) {
			filter(curr->key, curr->vabstrtype, &curr->value);
			curr = curr->next;
		}
	}
}




void printf_full_htable(HASHTABLE* htable) {
	int ind = 1;
	for (int i = 0; i < htable->tcapacity; i++) {
		Entry* curr = htable->buckets[i];

		while (curr != NULL) {
			switch (curr->vabstrtype) {
			case INT_V:
				printf("%d. %s : %d\n", ind, curr->key, curr->value.ival);
				break;
			case FLOAT_V:
				printf("%d. %s : %f\n", ind, curr->key, curr->value.fval);
				break;
			case STRING_V:
				printf("%d. %s : %s\n", ind, curr->key, curr->value.sval);
				break;
			default:
				printf("Error");
				break;
			}
			ind++;
			curr = curr->next;
		}
	}
	printf("\n");
}

void printf_ht_element(HASHTABLE* htable, char* key) {
	unsigned index = hash(key, htable->tcapacity);

	Entry* curr = htable->buckets[index];

	while (curr != NULL) {
		if (strcmp(curr->key, key) == 0) {
			switch (curr->vabstrtype) {
			case INT_V:
				printf("%s : %d", curr->key, curr->value.ival);
				break;
			case FLOAT_V:
				printf("%s : %f", curr->key, curr->value.fval);
				break;
			case STRING_V:
				printf("%s : %s", curr->key, curr->value.sval);
				break;
			default:
				printf("Error");
				break;
			}
			return;
		}
		curr = curr->next;
	}

}

int is_exists(HASHTABLE* htable, char* key) {

	if (!key) {
		return 0;
	}
	unsigned index = hash(key, htable->tcapacity);

	Entry* curr = htable->buckets[index];

	while (curr != NULL) {
		if (strcmp(curr->key, key) == 0) {
			return 1;
		}
		curr = curr->next;
	}
	return 0;
}


char** get_keys(HASHTABLE* htable) {
	char** all_keys = malloc(sizeof(char*) * htable->tsize);
	int index = 0;

	for (int i = 0; i < htable->tcapacity; i++) {
		Entry* curr = htable->buckets[i];

		while (curr != NULL) {
			all_keys[index++] = curr->key;
			curr = curr->next;
		}
	}
	return all_keys;
}

void* get_value(HASHTABLE* htable, char* key) {
	unsigned index = hash(key, htable->tcapacity);

	Entry* curr = htable->buckets[index];
	
	while (curr != NULL) {
		if (strcmp(curr->key, key) == 0) {
			return &(curr->value);
		}
		curr = curr->next;
	}
	return NULL;

}



void insert_data_str_int(HASHTABLE* htable, char* key, int value) {
	Vtype type;
	type.ival = value;
	insert_data(htable, key, INT_V, type);
}
void insert_data_str_float(HASHTABLE* htable, char* key, float value) {
	Vtype type;
	type.fval = value;
	insert_data(htable, key, FLOAT_V, type);
}
void insert_data_str_str(HASHTABLE* htable, char* key, char* value) {
	Vtype type;
	type.sval = str_dup(value);
	insert_data(htable, key, STRING_V, type);
}



int delete_data(HASHTABLE* htable, char* key) {
	unsigned index = hash(key, htable->tcapacity);

	Entry* curr = htable->buckets[index];
	Entry* prev = NULL;

	while (curr != NULL) {
		if (strcmp(curr->key, key) == 0) {
			if (prev == NULL) {
				htable->buckets[index] = curr->next;
			}
			else {
				prev->next = curr->next;
			}
			free(curr->key);
			if (curr->vabstrtype == STRING_V) {
				free(curr->value.sval);
			}
			free(curr);
			htable->tsize--;

			return 1;
		}
		prev = curr;
		curr = curr->next;
	}
	return 0;
}



// OPTIMIZATION AND DEBUGGING ---------------------

void ht_reallocation(HASHTABLE* htable, int new_tcapacity) {

	Entry** updated_buckets = calloc(new_tcapacity, sizeof(Entry*));

	for (int i = 0; i < htable->tcapacity; i++) {
		Entry* curr = htable->buckets[i];

		while (curr != NULL) {
			Entry* next = curr->next;

			unsigned index = hash(curr->key, new_tcapacity);
			curr->next = updated_buckets[index];
			updated_buckets[index] = curr;

			curr = next;
		}
	}

	free(htable->buckets);

	htable->buckets = updated_buckets;
	htable->tcapacity = new_tcapacity;

}

float fill_factor_calculator(HASHTABLE* htable) {
	return (float)htable->tsize / htable->tcapacity;
}

int get_htcapacity(HASHTABLE* htable) {
	return htable->tcapacity;
}

int get_htsize(HASHTABLE* htable) {
	return htable->tsize;
}








