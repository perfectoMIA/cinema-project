#include <stdio.h>
#include <stdlib.h>

typedef struct film {
    char name[60];
    int year;
    char country[30];
    char genre[50];
    float rate;
} film;

typedef struct list {
    film film;
    struct list *prev;
    struct list *next;
} node;

// Инициализация двусвязного списка
struct list *init(film film) {
    struct list *lst;
    lst = (struct list*)malloc(sizeof(struct list));
    lst->film = film;
    lst->next = lst;
    lst->prev = lst;
    return lst;
}

// Добавление узла в список
struct list *addelem(node *lst, film new_film) {
    struct list *tmp, *p;
    tmp = (struct list*)malloc(sizeof(node));
    p = lst->next; // сохранение указателя на следующий узел
    lst->next = tmp; // предыдущий узел указывает на создаваемый
    tmp->film = new_film; // сохранение поля данных добавляемого узла
    tmp->next = p; // созданный узел указывает на следующий узел
    tmp->prev = lst; // созданный узел указывает на предыдущий узел
    p->prev = tmp;
    return tmp;
}

// Удаление узла из списка
struct list *deletelem(node *lst) {
    struct list *prev, *next;
    prev = lst->prev; 
    next = lst->next; 
    prev->next = lst->next; // переставляем указатель
    next->prev = lst->prev; // переставляем указатель
    free(lst); // освобождаем память удаляемого элемента
    return prev;
}

int main(void) {
  printf("boba");
    return 0;
}