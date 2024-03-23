#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Athlete {
    char *name;
    char *university;
    int age;
    float weight;
    int height;
    int result[3];
    float index;
} Athlete;

typedef struct NodeOfList {
    Athlete *data;
    struct NodeOfList *next;
} NodeOfList;

typedef struct ListOfAthlete {
    int length;
    NodeOfList *first;
} ListOfAthlete;


int from_str_to_int(char *x) {
    int ans = 0;

    while (*x != '\0') {
        ans = ans * 10 + (*x - '0');
        ++x;
    }
    return ans;
}

float from_str_to_float(char *x) {
    float ans = 0, a = 10, b = 1;

    while (*x != '\0') {
        if (*x == '.' || *x == ',') {
            a = 1;
            b = 10;
        } else {
            ans = ans * a + (float) (*x - '0') / b;
            if (b > 1) b *= 10;
        }
        ++x;
    }
    return ans;
}

void from_str_to_int_mas(char *x, int *mas) {
    int ind = 0, j = 0;

    while (x[j] != '\0') {
        if (x[j] == ';') {
            x[j] = '\0';
            mas[ind++] = from_str_to_int(x);
            x += j + 1;
            j = -1;
        }
        ++j;
    }
    mas[ind] = from_str_to_int(x);
}

Athlete *fill_struct(char *str) {
    Athlete *user = NULL;
    char *word = str;
    int ind = 0, tt;
    char *pole[5];

    user = (Athlete *) malloc(sizeof(Athlete));
    if (user != NULL) {
        for (tt = 0; str[tt] != '\n' && str[tt] != '\0'; ++tt) {
            if (str[tt] == ';' && ind < 5) {
                str[tt] = '\0';
                pole[ind++] = word;
                word = str + tt + 1;
            }
        }
        str[tt] = '\0';
        user->name = pole[0];
        user->university = pole[1];
        user->age = from_str_to_int(pole[2]);
        user->weight = from_str_to_float(pole[3]);
        user->height = from_str_to_int(pole[4]);
        from_str_to_int_mas(word, user->result);
        user->index = (float) (user->result[0] + user->result[1] + user->result[2]) / user->weight;
    }
    return user;
}

ListOfAthlete *make_list() {
    ListOfAthlete *ph = NULL;
    ph = (ListOfAthlete *) malloc(sizeof(ListOfAthlete));
    if (ph != NULL) {
        ph->length = 0;
        ph->first = NULL;
    }
    return ph;
}

NodeOfList *create_node(const char *text) {
    NodeOfList *new_node = NULL;
    char *copytext = NULL;

    new_node = (NodeOfList *) malloc(sizeof(NodeOfList));
    copytext = (char *) malloc((strlen(text) + 1) * sizeof(char));
    if (new_node && copytext) {
        strcpy(copytext, text);
        new_node->data = fill_struct(copytext);
        new_node->next = NULL;
    }
    return new_node;
}

NodeOfList *select_by_order(const ListOfAthlete *list, int ind) {
    NodeOfList *q = list->first;
    int n = list->length;

    if (ind < 0 || ind > n) {
        ind = n - 1;
        printf("The index is out of range!\n"
            "The item will be inserted at the top of the list.\n");
    }
    for (int i = 1; i < n - ind; ++i) q = q->next;
    return q;
}

void add(ListOfAthlete *list, NodeOfList *new_node, int ind) {
    NodeOfList *cur_node = select_by_order(list, ind);
    NodeOfList *q = NULL;

    if (list && new_node && cur_node) {
        if (list->first == cur_node) {
            new_node->next = cur_node;
            list->first = new_node;
            ++list->length;
        } else {
            q = list->first;
            while (q != NULL) {
                if (q->next == cur_node) {
                    q->next = new_node;
                    new_node->next = cur_node;
                    ++list->length;
                    q = NULL;
                } else q = q->next;
            }
        }
    }
}

void free_list(ListOfAthlete *list) {
    NodeOfList *cur_node = list->first;
    NodeOfList *next_node;

    free(list);
    while (cur_node != NULL) {
        next_node = cur_node->next;
        free(cur_node->data);
        free(cur_node);
        cur_node = next_node;
    }
}

void print_line() {
    printf("+");
    for (int i = 0; i < 22; printf("-"), ++i);
    printf("+------------+-----+--------+--------"
        "+------+------+------+-------+\n");
}

void print_node(Athlete *node) {
    printf("| %-20s | %-10s | %-3i | %0.1f  ", node->name, node->university,
           node->age, node->weight);
    if (node->weight < 100) printf(" ");
    printf("| %-6i | %-4i | %-4i | %-4i ", node->height, node->result[0],
           node->result[1], node->result[2]);
    printf("| %0.3f |\n", node->index);
}

void pprint(const ListOfAthlete *list) {
    NodeOfList *cur_node = list->first;

    print_line();
    printf("| Name                 | University | Age | Weight | Height "
        "| Res1 | Res2 | Res3 | Index |\n");
    print_line();
    while (cur_node != NULL) {
        print_node(cur_node->data);
        cur_node = cur_node->next;
    }
    print_line();
}

NodeOfList **get_mas(const ListOfAthlete *list) {
    NodeOfList *cur_node = list->first;
    NodeOfList **mas = NULL;

    mas = (NodeOfList **) malloc(list->length * sizeof(NodeOfList *));
    if (mas != NULL) {
        for (int i = 0; cur_node != NULL; ++i) {
            mas[i] = cur_node;
            cur_node = cur_node->next;
        }
    }
    return mas;
}

void my_swap(NodeOfList **mas, ListOfAthlete *list, int i, int j) {
    NodeOfList *q;

    if (i == 0) {
        list->first = mas[j];
    } else {
        mas[i - 1]->next = mas[j];
    }
    mas[j - 1]->next = mas[i];
    q = mas[j]->next;
    mas[j]->next = mas[i]->next;
    mas[i]->next = q;
    q = mas[i];
    mas[i] = mas[j];
    mas[j] = q;
}

void sort_list(ListOfAthlete *list, int param) {
    NodeOfList **mas = get_mas(list);
    int n = list->length;

    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            if ((param == 1 && mas[i]->data->age > mas[j]->data->age) ||
                (param == 2 && mas[i]->data->weight > mas[j]->data->weight) ||
                (param == 3 && mas[i]->data->height > mas[j]->data->height) ||
                (param == 4 && mas[i]->data->index > mas[j]->data->index)) {
                my_swap(mas, list, i, j);
            }
        }
    }
    free(mas);
}

char *m_strlwr(const char *str) {
    char *new_str = NULL;
    new_str = (char *) malloc((strlen(str) + 1) * sizeof(char));

    if (new_str != NULL) {
        strcpy(new_str, str);
        strlwr(new_str);
    }
    return new_str;
}

void sorted(int *mas, const ListOfAthlete *list, int param) {
    NodeOfList *cur_node, *min_node;
    int ind;

    for (int j = 0; j < list->length; ++j) {
        cur_node = list->first;
        min_node = NULL;
        for (int i = 0; cur_node != NULL && i < list->length; ++i, cur_node = cur_node->next) {
            if (mas[i] == 1) {
                if ((min_node == NULL) ||
                    ((param == 1 && min_node->data->age > cur_node->data->age) ||
                     (param == 2 && min_node->data->weight > cur_node->data->weight) ||
                     (param == 3 && min_node->data->height > cur_node->data->height) ||
                     (param == 4 && min_node->data->index > cur_node->data->index))) {
                    min_node = cur_node;
                    ind = i;
                }
            }
        }
        if (min_node != NULL) {
            mas[ind] = 2;
            print_node(min_node->data);
        }
    }

    for (int j = 0; j < list->length; ++j) {
        if (mas[j] == 2) mas[j] = 1;
    }
}

void find(ListOfAthlete *list, int param) {
    NodeOfList *cur_node = list->first;
    char x[128], *str, *new_str;
    int mas[list->length], fl = 0, ch;

    printf("Enter the search string:\n");
    getchar();
    fgets(x, sizeof(x), stdin);
    x[strlen(x) - 1] = '\0';
    strlwr(x);
    for (int i = 0; cur_node != NULL && i < list->length; ++i) {
        if (param == 1) str = cur_node->data->name;
        else str = cur_node->data->university;
        new_str = m_strlwr(str);
        if (strstr(new_str, x) != NULL) {
            if (fl == 0) {
                print_line();
                printf("| Name                 | University | Age | Weight "
                    "| Height | Res1 | Res2 | Res3 | Index |\n");
                print_line();
            }
            print_node(cur_node->data);
            fl = 1;
            mas[i] = 1;
        } else {
            mas[i] = 0;
        }
        free(new_str);
        cur_node = cur_node->next;
    }
    if (fl == 0) {
        printf("No matches found!\n");
    } else {
        print_line();
        do {
            printf("Select a field to sort by or exit:\n"
                "1 = age\n"
                "2 = weight\n"
                "3 = height\n"
                "4 = index\n"
                "0 = exit\n"
                "Enter only one number!\n");
            scanf("%i", &ch);
            if (ch < 0 || 4 < ch) {
                printf("Invalid command!\n");
            } else if (ch != 0) {
                print_line();
                printf("| Name                 | University | Age | Weight "
                    "| Height | Res1 | Res2 | Res3 | Index |\n");
                print_line();
                sorted(mas, list, ch);
                print_line();
            } else {
                printf("To display the data, enter the command \"!print\"\n"
                    "To find athletes, enter the command \"!find\"\n"
                    "To sort the data, enter the command \"!sort\"\n"
                    "To add new data, enter the command \"!add\"\n"
                    "To end the program, enter the command \"!end\"\n");
            }
        } while (ch != 0);
    }
}

int main() {
    ListOfAthlete *list;
    NodeOfList *cur_node = NULL, *last_node = NULL;
    char filename[128], text[1024], str[128];
    FILE *f;
    int ch;

    printf("Please enter the file name:\n");
    scanf("%s", filename);
    f = fopen(filename, "r");
    while (f == NULL) {
        printf("Something went wrong!\n"
            "Perhaps such a file does not exist.\n"
            "Please enter the file name again:\n");
        scanf("%s", filename);
        f = fopen(filename, "r");
    }

    list = make_list();
    while (fgets(text, sizeof(text), f)) {
        if (list->length == 0) {
            cur_node = create_node(text);
            list->first = cur_node;
            last_node = cur_node;
        } else {
            cur_node = create_node(text);
            last_node->next = cur_node;
            last_node = cur_node;
        }
        ++list->length;
    }
    printf("The file has successfully been processed!\n");
    fclose(f);

    printf("To display the data, enter the command \"!print\"\n"
        "To find athletes, enter the command \"!find\"\n"
        "To sort the data, enter the command \"!sort\"\n"
        "To add new data, enter the command \"!add\"\n"
        "To end the program, enter the command \"!end\"\n");

    do {
        scanf("%s", str);
        if (!strcmp(str, "!end")) {
            printf("Goodbye!\n");
        } else if (!strcmp(str, "!print")) {
            pprint(list);
        } else if (!strcmp(str, "!find")) {
            printf("Select a field to find by:\n"
                "1 = name\n"
                "2 = university\n"
                "Enter only one number!\n");
            scanf("%i", &ch);
            if (ch < 1 || 2 < ch) {
                printf("Invalid command!\n");
            } else {
                find(list, ch);
            }
        } else if (!strcmp(str, "!sort")) {
            printf("Select a field to sort by:\n"
                "1 = age\n"
                "2 = weight\n"
                "3 = height\n"
                "4 = index\n"
                "Enter only one number!\n");
            scanf("%i", &ch);
            if (ch < 1 || 4 < ch) {
                printf("Invalid command!\n");
            } else {
                sort_list(list, ch);
                printf("The data has been successfully sorted!\n");
                pprint(list);
            }
        } else if (!strcmp(str, "!add")) {
            printf("Enter data of the athlete in format:\n"
                "name;university;age;weight;height;result1,result2,result3\n");
            getchar();
            fgets(text, sizeof(text), stdin);
            printf("Enter the number of the item indicated at the end\n"
                "of the list before which you want to insert the athlete:\n");
            scanf("%i", &ch);
            --ch;
            add(list, create_node(text), ch);
            printf("The item has been successfully inserted!\n");
        } else {
            printf("Unknown command!\n");
        }
    } while (strcmp(str, "!end") != 0);

    free_list(list);
    return 0;
}
