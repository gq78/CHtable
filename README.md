# HASHTABLE

## Description:

A simple C library for interacting with hash tables. 
Key type: char * (string)
Value type: __int, float, char * (string) - union {int ival, float fval, char *sval} Vtype__


Auxiliary abstract types for easier operations: 
**INT_V, STRING_V, FLOAT_V**


## Features:

### Creation and deletion 
```c
HASHTABLE* create_htable() - // returns pointer to a new hash table
```
Example:
```c
HASHTABLE *ht = create_htable();
```
```c
HASHTABLE* clone_htable(HASHTABLE* htable);
void delete_htable(HASHTABLE* htable);
void clear_htable(HASHTABLE* htable) - // clears all buckets but does not delete the hash table
```

### Filters and Actions
Filter:
```c
typedef int (*FILTERFUNC)(const char* key, ValueABSTRType abstrtype, Vtype value);
HASHTABLE* select_all_which(HASHTABLE* htable, FILTERFUNC filter);
```
Example:
```c
int int_gt_30(const char *key, ValueABSTRType type, Vtype value) {
  if (type != INT_V)
    return 0;
  return value.ival >= 300;
}
```

in main():

```c
HASHTABLE* filtered = select_all_which(table, int_gt_30);
```


Actions:
```c
typedef int (*APPLYFUNC)(const char* key, ValueABSTRType abstrtype, Vtype *value);
void apply_to_all(HASHTABLE* htable, FILTERFUNC filter);
```
Example:

```c
void mult_by_10(const char *key, ValueABSTRType type, Vtype *value) {
  if (type != INT_V)
    return;
  value->ival *= 10;
}
```

in main():
```c
HASHTABLE *tb = create_htable();
...
apply_to_all(tb, mult_by_10);
```


### Display and inspection
```c
void printf_full_htable(HASHTABLE* htable)
void printf_ht_element(HASHTABLE* htable, char* key)
```
### Interaction
```c
char** get_keys(HASHTABLE* htable) - returns an array of all keys
void* get_value(HASHTABLE* htable, char* key) - returns the value by key
```
```c
int is_exists(HASHTABLE* htable, char* key);
```
#### Insearting by types:
```c
void insert_data_str_int(HASHTABLE* htable, char* key, int value);
void insert_data_str_float(HASHTABLE* htable, char* key, float value);
void insert_data_str_str(HASHTABLE* htable, char* key, char* value);
```
```c
int delete_data(HASHTABLE* htable, char* key);
```
### Debugging
```c
void ht_reallocation(HASHTABLE* htable, int new_tcapacity) - // changes the size of your hash table

float fill_factor_calculator(HASHTABLE* htable) - // calculates fill factor (used size/allocated size)

int get_htcapacity(HASHTABLE* ht);
int get_htsize(HASHTABLE* htable);
```

