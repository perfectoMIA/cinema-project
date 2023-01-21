#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <termios.h>
#include <ctype.h>
#include <locale.h>
#include <regex.h>
#include <string.h>
#include <sys/ioctl.h>
#include "newcolors-c.h"
typedef struct film {
    char name[60];
    int year;
    char country[30];
    char genre[50];
    float rate;
    struct list *prev;
    struct list *next;
} film;

typedef struct list {
    film film;
    struct list *prev;
    struct list *next;
} filmList;

typedef struct node {
    film film;
    int size;
    film *current;
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

// // Добавление узла в список
// struct list *addelem(filmList *lst, film new_film) {
//     struct list *tmp, *p;
//     tmp = (struct list*)malloc(sizeof(filmList));
//     p = lst->next; // сохранение указателя на следующий узел
//     lst->next = tmp; // предыдущий узел указывает на создаваемый
//     tmp->film = new_film; // сохранение поля данных добавляемого узла
//     tmp->next = p; // созданный узел указывает на следующий узел
//     tmp->prev = lst; // созданный узел указывает на предыдущий узел
//     p->prev = tmp;
//     return tmp;
// }
// Добавление нового фильма в список
film *add_film(struct node *ring) {
    film *film = malloc(sizeof(film));

    if (ring->current == NULL) {
        ring->current = film;
        film->prev = film;
        film->next = film;
    } else {
        film->prev = ring->current->prev;
        film->next = ring->current;
        ring->current->prev->next = film;
        ring->current->prev = film;
        ring->current = film;
    }
    ring->size++;
    return film;
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

struct node *create_films_ring() {
     struct node *films = (struct node *)malloc(sizeof(struct node));
    films->size = 0;
    films->current = NULL;
    return films;
}
// Считываем данные о фильмах из файла
struct node *get_films_from_file(const char *filename) {
    FILE *txt = fopen(filename, "r");
    struct node *films = create_films_ring();

    fseek(txt, 0, SEEK_END);
    long pos = ftell(txt);
    if (pos == 0) {
        return films;
    }
    fseek(txt, 0, SEEK_SET);

    while (!feof(txt)) {
        film *film = add_film(films);
        fgets(film->name, sizeof(film->name), txt);
        fscanf(txt, "%d\n", &film->year);
        fgets(film->country, sizeof(film->country), txt);
        fgets(film->genre, sizeof(film->genre), txt);
        fscanf(txt, "%f\n", &film->rate);

        strtok(film->name, "\n");
        strtok(film->country, "\n");
        strtok(film->genre, "\n");
    }
    fclose(txt);
    return films;
}

// Счетчик длины
size_t get_len(const char *string) {
    size_t size = 0;
    char byte;
    for (int i = 0;; i++) {
        byte = string[i];
        if (byte == '\0') {
            return size;
        }
        size += (((byte & 0x80) == 0) || ((byte & 0xC0) == 0xC0));
    }
}

// Вывод линий с информацией о фильме.
void print_line_with_spaces(const char *string, const char position) {
    size_t title_length = get_len(string), title_spaces;
    if (!position) {
        title_spaces = (46 - title_length) / 2;
    } else {
        title_length /= 2;
        if (title_length % 2 == 0) title_length++;
        title_spaces = (23 - title_length) / 2;
    }

    if (position != 2) printf("║");
    for (int j = 0; j < title_spaces; j++) printf(" ");

    text_red(stdout);
    if (!position) printf("%s", string);
    else for (int j = 0; j < title_length; j++) printf("*");
    text_blue(stdout);

    if ((!position && title_length % 2 != 0) || (position && title_length % 2 == 0)) title_spaces++;
    for (int j = 0; j < title_spaces; j++) printf(" ");

    if (position != 1) printf("║");

}

// Вывод карусели с фильмами
void print_cards(film *film) {
    text_bold(stdout);
    text_blue(stdout);
    printf("                        ╔══════════════════════════════════════════════╗");
    printf("                        \n                        ");
    printf("║                                              ║");
    printf("                        \n                        ");
    printf("║                                              ║");
    printf("                        \n╔═══════════════════════");
    print_line_with_spaces(film->name, 0);
    printf("═══════════════════════╗\n║                       ");
    printf("║                                              ║");
    printf("                       ║\n");
    print_line_with_spaces(film->prev->film.name, 1);
    print_line_with_spaces(film->genre, 0);
    print_line_with_spaces(film->next->film.name, 2);
    printf("\n║                       ");
    printf("║                                              ║");
    printf("                       ║\n");
    print_line_with_spaces(film->prev->film.genre, 1);
    print_line_with_spaces(film->country, 0);
    print_line_with_spaces(film->next->film.genre, 2);
    printf("\n║                       ");
    printf("║                                              ║");
    printf("                       ║\n");
    print_line_with_spaces(film->prev->film.country, 1);
    printf("║                     %.1f+                     ║", film->rate);
    print_line_with_spaces(film->next->film.country, 2);
    printf("\n║                       ");
    printf("║                                              ║");
    printf("                       ║\n║                       ");
    printf("║                                              ║");
    printf("                       ║\n║                       ");
    printf("║                                              ║");
    printf("                       ║\n║          ***          ");
    printf("║                                              ║");
    printf("          ***          ║\n║                       ");
    printf("║                                              ║");
    printf("                       ║\n╚═══════════════════════");
    printf("║                     %d                     ║", film->year);
    printf("═══════════════════════╝\n                        ");
    printf("║                                              ║");
    printf("                        \n                        ");
    printf("║                                              ║");
    printf("                        \n                        ");
    printf("╚══════════════════════════════════════════════╝\n");
    reset_colors(stdout);
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

//структура пользователя
typedef struct user {
  char login[20];//логин
  char password[20];//пароль
  char number_card[16];//номер карты
  char admin[1];//администратор или пользователь
  int favorites_list;//размер списка избранного 
}user;

int correct_login (user tmp) {
  FILE* file = fopen ("users.txt", "a");
  user client;
  int correct_login = 0, correct_password = 0, correct_number_card = 0, correct_admin = 0;
  while (correct_login != 1) {
    int check_uppercase_letter = 0, check_number = 0, check_lowercase_letter = 0;
    printf ("придумайте корректный логин: ");
    //считывание логина с консоли
    fgets (client.login, 21, stdin);
    //подсчёт количества символов в логине
    int amount_login = strlen(client.login);
    //подсчёт количества символов в логине без учёта пробела и переноса строки
    for (int i = 0; i < 20; i++) {
      int box = (int)client.login[i];
      if (box == 10 || box == 32) {
        amount_login--;
      }
    }
    //проверка логина на корректность
    if (amount_login >= 3 && amount_login <= 20) {
      for (int i = 0; i < amount_login; i++) {
        int box = (int)client.login[i];
        if (box >= 65 && box <= 90) {
          check_uppercase_letter = 1;
        }
        else if (client.login[i] >= 48 && client.login[i] <= 57) {
        }
        else if (client.login[i] >= 97 && box <= 122) {
          check_lowercase_letter = 1;
        }
        else {
          break;
        }
        
        if (i == amount_login - 1 && (check_uppercase_letter == 1 || check_lowercase_letter == 1)) {
          correct_login = 1;
        }
      }
    }
  }
  fprintf (file, "%s", client.login);
}

int correct_password (user tmp) {
  
}
int correct_number_card () {
  
}
int correct_admin () {
  
}

//функция регистрации пользователя
user user_registration () {
  FILE* file = fopen ("users.txt", "a");
  //очистка буфера
  while (getchar () != '\n');
  //создание новой структуры для хранения данных пользователя
  user client;
  int correct_login = 0, correct_password = 0, correct_number_card = 0, correct_admin = 0;
  //создание корректного логина
  while (correct_login != 1) {
    int check_uppercase_letter = 0, check_number = 0, check_lowercase_letter = 0;
    printf ("придумайте корректный логин: ");
    //считывание логина с консоли
    fgets (client.login, 21, stdin);
    //подсчёт количества символов в логине
    int amount_login = strlen(client.login);
    //подсчёт количества символов в логине без учёта пробела и переноса строки
    for (int i = 0; i < 20; i++) {
      int box = (int)client.login[i];
      if (box == 10 || box == 32) {
        amount_login--;
      }
    }
    //проверка логина на корректность
    if (amount_login >= 3 && amount_login <= 20) {
      for (int i = 0; i < amount_login; i++) {
        int box = (int)client.login[i];
        if (box >= 65 && box <= 90) {
          check_uppercase_letter = 1;
        }
        else if (client.login[i] >= 48 && client.login[i] <= 57) {
        }
        else if (client.login[i] >= 97 && box <= 122) {
          check_lowercase_letter = 1;
        }
        else {
          break;
        }
        
        if (i == amount_login - 1 && (check_uppercase_letter == 1 || check_lowercase_letter == 1)) {
          correct_login = 1;
        }
      }
    }
  }
  fprintf (file, "%s", client.login);
  //создание корректного пароля
  while (correct_password != 1) {
    int check_uppercase_letter = 0, check_number = 0, check_lowercase_letter = 0;
    printf ("придумайте корректный пароль: ");
    //cчитывание пароля с консоли
    fgets (client.password, 21, stdin);
    //подсчёт количества символов в пароле
    int amount_password = strlen(client.password);
    //подсчёт количества символов в пароле без учёта пробела и переноса строки
    for (int i = 0; i < 20; i++) {
      int box = (int)client.password[i];
      if (box == 10 || box == 32) {
        amount_password--;
      }
    }
    //проверка пароля на корректность
    if (amount_password >= 6 && amount_password <= 20) {
      for (int i = 0; i < amount_password; i++) {
        int box = (int)client.password[i];
        if (box >= 65 && box <= 90) {
          check_uppercase_letter = 1;
        }
        else if (client.password[i] >= 48 && client.password[i] <= 57) {
          check_number = 1;
        }
        else if (client.password[i] >= 97 && box <= 122) {
          check_lowercase_letter = 1;
        }
        else {
          break;
        }
        if (i == amount_password - 1 && check_uppercase_letter == 1 && check_number == 1 && check_lowercase_letter == 1) {
          correct_password = 1;
        }
      }
    }
  }
  fprintf (file, "%s", client.password);
  //корректный ввод номера карты
  while (correct_number_card != 1) {
    printf ("введите корректный номер карты: ");
    //считывание номера карты с консоли
    fgets (client.number_card, 17, stdin);
    //подсчёт количества символов в номере карты
    int amount_number_card = strlen(client.number_card);
    //подсчёт количества символов в номере карты без учёта пробела и переноса строки
    for (int i = 0; i < 20; i++) {
      int box = (int)client.number_card[i];
      if (box == 10 || box == 32) {
        amount_number_card--;
      }
    }
    if (amount_number_card == 16) {
      for (int i = 0; i < amount_number_card; i++) {
        if (client.number_card[i] >= 48 && client.number_card[i] <= 57) {
        }
        else {
          break;
        }
        if (i == amount_number_card - 1) {
          correct_number_card = 1;
        }
      }
    }
  }
  fprintf (file, "%s\n", client.number_card);
  //количество избранных фильмов изначально = 0
  client.favorites_list = 0;
  //корректный ввод администратора или пользователя
  while (correct_admin != 1) {
    while (getchar () != '\n');
    printf ("вы 1 - админ, 0 - пользователь: ");
    //считывание с консоли 0 или 1
    fgets (client.admin, 2, stdin);
    int box = (int)client.admin[0];
    if (box == 48 || box == 49) {
      correct_admin = 1;
    }
  }
  fprintf (file, "%s", client.admin);
  fprintf (file, "\n");
  //возвращаем заполненный пользователем аккаунт 
  return  client;
}

//функция входа в аккаунт
void user_input () {
  FILE* file = fopen ("users.txt", "a");
  while (getchar () != '\n');
  char login_input[20], password_input[20], login_file[20], password_file[20];
  int correct_login = 0, correct_password = 0;
  while (correct_login != 1) {
    printf ("введите логин: ");
    //считывание логина с консоли
    fgets (login_input, 21, stdin);
    //подсчёт количества символов в логине
    int amount_login = strlen(login_input);
    //подсчёт количества символов в логине без учёта пробела и переноса строки
    for (int i = 0; i < 20; i++) {
      int box = (int)login_input[i];
      if (box == 10 || box == 32) {
        amount_login--;
      }
    }
    fgets (login_file, 21, file);
    printf ("%c", login_file);
    for (int i = 0; i < amount_login; i++) {
      printf ("%s", login_file);
      if (login_input[i] != login_file[i]) {
        break;
      }
      if (i == strlen(login_input) - 1) {
        correct_login = 1;
      }
    }
  }
  while (correct_password != 1) {
    printf ("введите пароль: ");
    //считывание пароля с консоли
    fgets (password_input, 21, stdin);
    //подсчёт количества символов в пароле
    int amount_password = strlen(password_input);
    //подсчёт количества символов в пароле без учёта пробела и переноса строки
    for (int i = 0; i < 20; i++) {
      int box = (int)password_input[i];
      if (box == 10 || box == 32) {
        amount_password--;
      }
    }
    fgets (password_file, 21, file);
    for (int i = 0; i < strlen(password_input); i++) {
      if (password_input[i] != password_file[i]) {
        break;
      }
      if (i == strlen(password_input) - 1) {
        correct_password = 1;
      }
    }
  }
}

int main(void) {
  int answer;
  printf ("здравствуйте, выберите что вам нужно: 1 - регистрация, 2 - вход\n");
  scanf ("%d", &answer);
  if (answer == 1) {
    user client = user_registration();
    //printf ("%s", client.login);
  }
  else {
    user_input();
  }
  
  return 0;
}