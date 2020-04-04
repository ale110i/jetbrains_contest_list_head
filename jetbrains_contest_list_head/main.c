#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct Node {
    int value;
    struct Node *next;
};

struct Element {
    int64_t pointer_numeral;
    short exist;
};

struct HashTable {
    int current_size;
    int element_count;
    struct Element* table;
};

struct Node* newNode(int value) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->value = value;
    node->next = NULL;
    
    return (node);
}

struct Element* newElement(int64_t pointer_numeral) {
    struct Element* temp = (struct Element*)malloc(sizeof(struct Element));
    temp->pointer_numeral = pointer_numeral;
    temp->exist = 0;
    
    return (temp);
}

struct HashTable* newHashTable(int def_size){
    struct HashTable* temp = (struct HashTable*)malloc(sizeof(struct HashTable));
    temp->current_size = def_size;
    temp->element_count = 0;
    temp->table = (struct Element*)malloc(sizeof(struct Element) * temp->current_size);
    for (int i = 0; i < temp->current_size; ++i){
        temp->table[i].pointer_numeral = 0;
        temp->table[i].exist = 0;
    }
    return (temp);
}

int Hash(int64_t pointer_numeral, int table_size) {
    int hash = pointer_numeral % table_size;
    return hash;
}

double loadfactor(struct HashTable* table) {
    double ec = table->element_count;
    double cs = table->current_size;
    return ec/cs;
}

void grow(struct HashTable* table);
short search(int64_t pointer_numeral, struct HashTable* table);

void insert(int64_t pointer_numeral, struct HashTable* table){
    if (search(pointer_numeral, table) == 1){
        return;
    }
    if (table->table[Hash(pointer_numeral, table->current_size)].exist == 0){
        table->table[Hash(pointer_numeral, table->current_size)].pointer_numeral = pointer_numeral;
        table->table[Hash(pointer_numeral, table->current_size)].exist = 1;
        ++table->element_count;
    }
    else {
        int i = 0;
        while (table->table[(Hash(pointer_numeral, table->current_size) + i*i) % table->current_size].exist == 1){
            ++i;
        }
        table->table[i].pointer_numeral = pointer_numeral;
        table->table[i].exist = 1;
        ++table->element_count;
    }
    if (loadfactor(table) >= 0.75){
        grow(table);
    }
}

void grow(struct HashTable* table){
    struct Element buff[table->current_size];
    for (int i = 0; i < table->current_size; ++i){
        buff[i] = table->table[i];
    }
    int buff_size = table->current_size;
    free(table->table);
    table->current_size *= 2;
    table->table = (struct Element*)malloc(sizeof(struct Element) * table->current_size);
    for (int i = 0; i < table->current_size; ++i){
        table->table[i].pointer_numeral = 0;
        table->table[i].exist = 0;
    }
    for (int i = 0; i < buff_size; ++i){
        if (buff[i].exist == 1){
            insert(buff[i].pointer_numeral, table);
        }
    }
    return;
}

short search(int64_t pointer_numeral, struct HashTable* table){
    if (table->table[Hash(pointer_numeral, table->current_size)].exist == 0){
        return 0;
    }
    else {
        int i = 0;
        while (table->table[(Hash(pointer_numeral, table->current_size) + i*i) % table->current_size].exist == 1){
            if (table->table[(Hash(pointer_numeral, table->current_size) + i*i) % table->current_size].pointer_numeral == pointer_numeral){
                return 1;
            }
            ++i;
        }
        return 0;
    }
}

struct Node* cycleStart(struct Node* head) {
    struct HashTable* ht = newHashTable(80);
    struct Node* current_node = head;
    while (1){
        if (current_node->next == NULL){
            return NULL;
        }
        else {
            if (search((int64_t)current_node, ht) == 1){
                return current_node;
            }
            else {
                insert((int64_t)current_node, ht);
                current_node = current_node->next;
            }
        }
    }
}

int main() {
    struct Node* p = newNode(-1);
    struct Node* c = p;
    for (int i = 0; i < 10; ++i){
        c->next = newNode(i);
        c = c->next;
    }
    c->next = p->next->next->next;
    struct Node* k = cycleStart(p);
    printf("%i\n", k->value);
    return 0;
}
