#include <stdio.h>
#include <stdlib.h>
#include "cinema.h"

struct list *init(film film) {
    struct list *lst;
    lst = (struct list*)malloc(sizeof(struct list));
    lst->film = film;
    lst->next = lst;
    lst->prev = lst;
    return lst;
}

struct list *addelem(node *lst, film new_film) {
    struct list *tmp, *p;
    tmp = (struct list*)malloc(sizeof(node));
    p = lst->next; 
    lst->next = tmp; 
    tmp->film = new_film; 
    tmp->next = p; 
    tmp->prev = lst; 
    p->prev = tmp;
    return tmp;
}

struct list *deletelem(node *lst) {
    struct list *prev, *next;
    prev = lst->prev; 
    next = lst->next; 
    prev->next = lst->next;
    next->prev = lst->prev; 
    free(lst); 
    return prev;
}
