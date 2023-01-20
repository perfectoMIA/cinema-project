#include <stdio.h>
#include <stdlib.h>

typedef struct film {
    char name[60];
    char year[5];
    char country[30];
    char genre[50];
    char rate[5];
} film;

typedef struct list {
    film film;
    struct list *prev;
    struct list *next;
} filmList;

typedef struct node {
    film film;
    struct node *next;
} favorites;

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
struct list *addelem(filmList *lst, film new_film) {
    struct list *tmp, *p;
    tmp = (struct list*)malloc(sizeof(filmList));
    p = lst->next; // сохранение указателя на следующий узел
    lst->next = tmp; // предыдущий узел указывает на создаваемый
    tmp->film = new_film; // сохранение поля данных добавляемого узла
    tmp->next = p; // созданный узел указывает на следующий узел
    tmp->prev = lst; // созданный узел указывает на предыдущий узел
    p->prev = tmp;
    return tmp;
}

// Удаление узла из списка
struct list *deletelem(filmList *lst) {
    struct list *prev, *next;
    prev = lst->prev; 
    next = lst->next; 
    prev->next = lst->next; // переставляем указатель
    next->prev = lst->prev; // переставляем указатель
    free(lst); // освобождаем память удаляемого элемента
    return prev;
}

void printFilm(filmList *cur, int n) {
    if (n) {
        printf("%s", cur->film.name);
        printf("%s", cur->film.year);
        printf("%s", cur->film.country);
        printf("%s", cur->film.genre);
        printf("%s\n", cur->film.rate);
    } else {
        printf("%s", cur->film.name);
        printf("%s\n", cur->film.rate);
    }
}

int checkName(char *name1, char *name2) {
    int i = 0;
    while (name1[i] != '\0' && name2[i] != '\0') {
        if (name1[i] != name2[i]) {
            return 0;
        }
        i++;
    }
    return 1;
}

int isFilmValid(filmList *lst, char *name) {
    filmList *cur = lst;
    do {
        if (checkName(cur->film.name, name)) {
            return 0;
        }
        cur = cur->next;
    } while(cur != lst);
    return 1;
}

//Список избранного
void addFilm(favorites **head, film film) {
    if (*head == NULL) {
        *head = (favorites *) malloc(sizeof(favorites));
        (*head)->film = film;
        (*head)->next = NULL;
    } else {
        favorites *cur = *head;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = (favorites *) malloc(sizeof(favorites));
        cur->next->film = film;
        cur->next->next = NULL;
    }
}

void deleteFilm(favorites **head, char *name) {
    if (checkName((*head)->film.name, name)) {
        favorites *next = (*head)->next;
        free(*head);
        *head = next;
        return;
    }
    favorites *cur = *head;
    while (!checkName(cur->next->film.name, name)) {
        cur = cur->next;
    }
    favorites *tmp = cur->next->next;
    free(cur->next);
    cur->next = tmp;
}

int isFilmInList(favorites *head, char *name) {
    favorites *cur = head;
    while (cur != NULL) {
        if (checkName(cur->film.name, name)) {
            return 1;
        }
        cur = cur->next;
    }
    return 0;
}

int main(void) {
  
    return 0;
}