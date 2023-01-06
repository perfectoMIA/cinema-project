#include <stdio.h>
#include <stdlib.h>

typedef struct film {
    char name;
    int year;
    char country;
    char genre;
    float rate;
} film;

typedef struct list {
    film film;
    struct list *prev;
    struct list *next;
} node;

int main(void) {
  
    return 0;
}