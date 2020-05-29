#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MEM_ERR "Error: could not allocate memory"

struct string {
    char * literal;
    unsigned int length;
};

typedef struct string string;

string from_literal(char * str) {
    // Create a new string object
    string new_str = {
        .literal = NULL,
        .length = 0
    };

    // If passed paramenter is not (null) copy values
    if (str != NULL) {
        int len = strlen(str);
        new_str.literal = malloc(sizeof(char) * (len + 1));
        strcpy(new_str.literal, str);
        new_str.length = len;
    }

    // Return constructed object
    return new_str;
}

void println(string s) {
    printf("%s\n", s.literal);
}

string clone(string * dest, string * src) {
    strcpy(dest->literal, src->literal);
    dest->length = src->length;
}

int match(string s1, string s2) {
    if (strcmp(s1.literal, s2.literal) == 0) {
        return 1;
    } else {
        return 0;
    }
}

void free_string(string * s) {
    free(s->literal);
    s->literal = NULL;
    s->length = 0;
}

#define TYPE string
#define VECT_INIT_SIZE 2

struct vector {
    TYPE * values;
    unsigned int length;
    unsigned int size;
};

typedef struct vector vector;

vector initv() {
    vector new_vec = {
        .values = NULL,
        .length = 0,
        .size = 0,
    };
    return new_vec;
}

vector from_array(TYPE * arr, int arr_size) {
    // Code snippet to figure out size of container
    unsigned int container_size = 1;
    while(container_size < arr_size) {
        container_size = container_size << 1;
    }

    // Create return object, set up metadata and allocate memory
    vector ret_vector;
    ret_vector.length = arr_size;
    ret_vector.size = container_size;
    ret_vector.values = malloc(sizeof(TYPE) * container_size);

    if (ret_vector.values == NULL) {
        printf("%s\n", MEM_ERR);
    }

    // Copy values
    for (int i = 0; i < arr_size; i++) {
        ret_vector.values[i] = arr[i];
    }

    // Return created vector
    return ret_vector;
}

int push_into(vector * vec, TYPE value) {
    int len = vec->length;
    int sz = vec->size;

    // Special case for empty vector
    if (vec->values == NULL) {
        vec->values = malloc(sizeof(TYPE) * 2);
        vec->size = 2;
        vec->values[0] = value;
        vec->length = 1;
        return 0;
    }

    // If vector needs to be expanded
    if (len >= sz) {
        // Reallocate memory for additional  elements
        TYPE * temp = realloc(vec->values, (sizeof(TYPE) * (sz << 1)));
        if (temp == NULL) {
            printf("%s\n", MEM_ERR);
            return -1;
        } else {
            vec->values = temp;
            vec->size = vec->size << 1;
        }
    }

    // Insert value
    vec->values[len] = value;

    // Update vector metadata
    vec->length = len + 1;

    // Return success
    return 0;
}

// If vector is hogging excess memory return to OS
int compact(vector * v) {
    int ln = v->length;
    int sz = v->size;
    int capacity = 1;
    while(capacity < ln) {
        capacity = capacity << 1;
    }
    if (capacity < sz) {
        TYPE * temp = realloc(v->values, (sizeof(TYPE) * capacity));
        if (temp == NULL) {
            printf("%s\n", MEM_ERR);
            return -1;
        }
        v->values = temp;
        v->size = capacity;
    }
}

// Returns element at the end of the vector
// As a good practice, run compact() after pop
TYPE pop(vector * v) {
    // Figure out index to return
    int index = v->length - 1;
    TYPE elem = v->values[index];

    // Update vector metadata to reflect changes
    v->length = v->length - 1;

    // return element
    return elem;
}

// search(vector v, TYPE key, match(TYPE, TYPE))
// search for key in vector v using math function to compare
// search returns only first instance
// match function should return 1 on success and 0 otherwise
int search(vector v, TYPE key, int (*match)(TYPE, TYPE)) {
    for (int i = 0; i < v.length; i++) {
        if (match(v.values[i], key)) {
            return i;
        }
    }
    return -1;
}

// removes element at given index from vector
int vector_remove(vector * v, int index) {
    if (index >= v->length) {
        printf("%s\n", "Invalid index");
        return -1;
    }
    for (int i = index; i < v->length; i++) {
        v->values[i] = v->values[i + 1];
    }
    v->length -= 1;
}

void print_vector(vector v, void (*print_fun)(TYPE)) {
    printf("[...\n");
    for (int i = 0; i < v.length; i++) {
        printf("\t");
        print_fun(v.values[i]);
    }
    printf("...]\n");
    printf("Vector length: %d\n", v.length);
    printf("Vector size: %d\n", v.size);
}

// 24x7 suryavansham
#define SET_MAX 61

struct set {
    vector buckets[SET_MAX];
    unsigned int (* hash)(string, int);
    unsigned int size;
};

typedef struct set set;

unsigned int hash(string s, int hmax) {
    char * iter = s.literal;
    unsigned int hash = 216613;
    while(*iter != '\0') {
        hash ^= (int)(*iter);
        hash *= 16779;
        iter++;
    }
    return (hash % hmax);
}

set new_set() {
    set ret_set;
    for (int i = 0; i < SET_MAX; i++) {
        ret_set.buckets[i] = initv();
        ret_set.buckets[i].length = 0;
    }
    ret_set.hash = hash;
    ret_set.size = 0;
    return ret_set;
}

int insert_into(set * s, string value) {
    unsigned int pos = s->hash(value, SET_MAX);
    pos = pos % SET_MAX;
    // If chosen bucket is empty
    if (s->buckets[pos].values == NULL) {
        // DIrectly insert
        printf("Inserting into vector no: %d\n", pos);
        s->size++;
        return push_into(&(s->buckets[pos]), value);    
    } else {
        // Check for duplicate
        unsigned int ind = search(s->buckets[pos], value, match);
        if (ind != -1) {
            printf("%s\n", "Duplicate detected, no insertion");
            return -1;
        }
        // Then insert
        printf("Inserting into vector no: %d\n", pos);
        s->size++;
        return push_into(&(s->buckets[pos]), value);
    }
}

// Function to print items in a set
void print_set(set s, void (*print_fun)(TYPE)) {
    printf("{...\n");
    for (int i = 0; i < SET_MAX; i++) {
        if (s.buckets[i].values == NULL) {
            continue;
        } else {
            for (int j = 0; j < s.buckets[i].length; j++) {
                print_fun(s.buckets[i].values[j]);
            }
        }
    }
    printf("...}\n");
}

// Checks whether given key exists in set
int exists_in(set s, TYPE key) {
    int pos = s.hash(key, SET_MAX);
    return (search(s.buckets[pos], key, match) != -1);
}

// Constructs a set from given array
set set_from_array(string * arr, int len) {
    set ret_set = new_set();
    for (int i = 0; i < len; i++) {
        insert_into(&ret_set, arr[i]);
    }
    return ret_set;
}

// Removes given key from set, if it exists
void remove_from_set(set * s, string key) {
    int pos = s->hash(key, SET_MAX);
    int sr = search(s->buckets[pos], key, match);
    if (sr == -1) {
        printf("%s\n", "Key not found, thus cannot remove");
    } else {
        vector_remove(&(s->buckets[pos]), sr);
        printf("Removal successful\n");
        s->size--;
    }
}

int is_empty(set s) {
    return (s.size == 0);
}

unsigned int cardinality(set s) {
    return s.size;
}

void set_copy(set * dest, set * src) {
    for (int i = 0; i < SET_MAX; i++) {
        vector * v = &src->buckets[i];
        for (int i = 0; i < v->length; i++) {

        }
    }
}

set set_union(set s1, set s2) {
    // Copy all elements from s1 into union
    set ret_set = s1;
    for (int i = 0; i < SET_MAX; i++) {
        // Insert elements from s2 into union, one by one
        int nelems = s2.buckets[i].length;
        if (nelems > 0) {
            for (int j = 0; j < nelems; j++) {
                insert_into(&ret_set, s2.buckets[i].values[j]);
            }
        }
    }
    return ret_set;
}

set set_intersection(set s1, set s2) {
    set ret_set = new_set();
    for (int i = 0; i < SET_MAX; i++) {
        vector vec = s1.buckets[i];
        for (int j = 0; j < vec.length; j++) {
            if (exists_in(s2, vec.values[j])) {
                insert_into(&ret_set, vec.values[j]);
            }
        }
    }
    for (int i = 0; i < SET_MAX; i++) {
        vector vec = s2.buckets[i];
        for (int j = 0; j < vec.length; j++) {
            if (exists_in(s1, vec.values[j])) {
                insert_into(&ret_set, vec.values[j]);
            }
        }
    }
    return ret_set;
}

set set_difference(set s1, set s2) {
    set ret_set = new_set();
    for (int i = 0; i < SET_MAX; i++) {
        vector vec = s1.buckets[i];
        for (int j = 0; j < vec.length; j++) {
            if (!exists_in(s2, vec.values[j])) {
                insert_into(&ret_set, vec.values[j]);
            }
        }
    }
    for (int i = 0; i < SET_MAX; i++) {
        vector vec = s2.buckets[i];
        for (int j = 0; j < vec.length; j++) {
            if (!exists_in(s1, vec.values[j])) {
                insert_into(&ret_set, vec.values[j]);
            }
        }
    }
    return ret_set;
}

int is_subset(set sub, set sup) {
    int ret = 1;
    for (int i = 0; i < SET_MAX; i++) {
        vector vec = sub.buckets[i];
        for (int j = 0; j < vec.length; j++) {
            if (!exists_in(sup, vec.values[j])) {
                ret = 0;
            }
        }
    }
    return ret && (sub.size <= sup.size);
}

int main() {
    set s1 = new_set();
    int c1;
    printf("Enter number of items in first set: ");
    scanf("%d", &c1);
    for (int i = 0; i < c1; i++) {
        char temp[32];
        printf("Enter string to add to set 1: ");
        scanf("%s", temp);
        insert_into(&s1, from_literal(temp));
    }
    printf("===SET 1===\n");
    print_set(s1, println);

    set s2 = new_set();
    int c2;
    printf("Enter number of items in second set: ");
    scanf("%d", &c2);
    for (int i = 0; i < c1; i++) {
        char temp[32];
        printf("Enter string to add to set 2: ");
        scanf("%s", temp);
        insert_into(&s2, from_literal(temp));
    }
    printf("===SET 2===\n");
    print_set(s2, println);

    printf("===SEARCH===\n");
    char temp[32];
    printf("Enter search term: ");
    scanf("%s", temp);
    string query = from_literal(temp);
    printf("Query found in s1: %d\n", exists_in(s1, query));
    printf("Query found in s2: %d\n", exists_in(s2, query));

    printf("===REMOVAL===\n");
    printf("REMOVAL will affect union/intersection/difference\n");
    printf("Enter set from which you want to remove (1 oder 2)\n");
    int choice;
    scanf("%d", &choice);
    if (choice == 1) {
        printf("Removing from 1");
        char temp[32];
        printf("Enter removal term: ");
        scanf("%s", temp);
        string query = from_literal(temp);
        remove_from_set(&s1, query);
        printf("===SET 1===\n");
        print_set(s1, println);
    } else if (choice == 2) {
        printf("Removing from 2");
        char temp[32];
        printf("Enter removal term: ");
        scanf("%s", temp);
        string query = from_literal(temp);
        remove_from_set(&s2, query);
        printf("===SET 2===\n");
        print_set(s2, println);
    } else {
        printf("Skipping removal due to invalid choice\n");
    }
    
    printf("===UNION===\n");
    print_set(set_union(s1, s2), println);

    printf("===INTERSECTION===\n");
    print_set(set_intersection(s1, s2), println);

    printf("===DIFFERENCE===\n");
    print_set(set_difference(s1, s2), println);

    printf("===SUBSET===\n");
    printf("Is set 2 subset of s2?: %d\n", is_subset(s2, s1));
}
