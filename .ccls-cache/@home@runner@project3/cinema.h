#define UNTITLED_CINEMA_H

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

struct list *init(film film);

struct list *addelem(node *lst, film new_film);

struct list *deletelem(node *lst);
