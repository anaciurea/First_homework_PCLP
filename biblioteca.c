#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TITLE_LENGTH 42
#define MAX_AUTHOR_LENGTH 22
#define MAX_DESCRIPTION_LENGTH 502
#define MAX_COMMAND_LENGTH 10
#define MAX_CODE_LENGTH 5

/*We declare the structure with the given elemnts.
*/
typedef struct {
    int id;
    char title[MAX_TITLE_LENGTH];
    char author[MAX_AUTHOR_LENGTH];
    int release_year;
    char *description;
} book_t;

/*We creat this function to read all the elements.
*/
void read_books(book_t *books, int nr_of_books) {
    int i;

    for (i = 0; i < nr_of_books; i++) {
        scanf("%d\n", &books[i].id);

        fgets(books[i].title, MAX_TITLE_LENGTH, stdin);
        /*Instead of having the penultimate element \n and on the last one \0, we put
        \0 instead of the string terminator so that we can then use the other functions.
        */
        books[i].title[strlen(books[i].title) - 1] = '\0';

        fgets(books[i].author, MAX_AUTHOR_LENGTH, stdin);
        books[i].author[strlen(books[i].author) - 1] = '\0';

        scanf("%d\n", &books[i].release_year);

        char *current_description = malloc(MAX_DESCRIPTION_LENGTH * sizeof(char));
        fgets(current_description, MAX_DESCRIPTION_LENGTH, stdin);
        current_description[strlen(current_description) - 1] = '\0';

        books[i].description = malloc((strlen(current_description) + 1) * sizeof(char));
        strncpy(books[i].description, current_description, strlen(current_description) + 1);

        free(current_description);
    }
}

/*We creat this function to print all the elements.
*/
void print_books(book_t *books, int nr_of_books) {
    printf("%d\n", nr_of_books);

    int i;

    for (i = 0; i < nr_of_books; i++) {
        printf("%d\n", books[i].id);
        printf("%s\n", books[i].title);
        printf("%s\n", books[i].author);
        printf("%d\n", books[i].release_year);
        printf("%s\n", books[i].description);
    }
}

/*We make another function to print the elements for the function 'search',
 because in case 'search' is used, we have to leave a free row.
*/
void print_tmp_books(book_t *tmp_books[], int found) {
    int i;

    for (i = 0; i < found; i++) {
        printf("%d\n", tmp_books[i]->id);
        printf("%s\n", tmp_books[i]->title);
        printf("%s\n", tmp_books[i]->author);
        printf("%d\n", tmp_books[i]->release_year);
        printf("%s\n", tmp_books[i]->description);
        printf("\n");
    }
}

/*We creat this function to check if the 
element which is searched is a number or a string.
The isdigit() is a function in C. This method returns 
True if all the characters are digits, otherwise False
*/
int is_number(char element[MAX_TITLE_LENGTH]) {
    int i;

    for (i = 0; i < strlen(element); i++) {
        if (!isdigit(element[i])) {
            return 0;
        }
    }

    return 1;
}

void search_in_books(book_t *books, int nr_of_books, char element[MAX_TITLE_LENGTH]) {
    // Array of pointers to elements of type book_t [*book1, *book2, ...]
    //Doar adresa de inceput a vectorului books

    book_t *tmp_books[nr_of_books]; //Vector de pointeri catre structuri book_t care arata in ordinea cresc a id

    int found = 0, i, j;
    if (is_number(element)) {
        //After we check if the element is a number or a string, 
        //we have 2 different cases.
        for (i = 0; i < nr_of_books; i++) {
            if (books[i].release_year == atoi(element)) {
                tmp_books[found] = &books[i];
                found++;
            }
        }
    } else {
        for (i = 0; i < nr_of_books; i++) {
            if (strcmp(books[i].title, element) == 0
                || strcmp(books[i].author, element) == 0) {
                
                tmp_books[found] = &books[i];
                found++;
            }
        }
    }

    /*As mentioned, if there are 2 books fulfilling the conditions
    for the same search, we print the one with the smaller id.
    */
    for (i = 0; i < found - 1; i++) {
        for (j = i + 1; j < found; j++) {
            if (tmp_books[i]->id> tmp_books[j]->id ) {
                book_t *aux = tmp_books[i];
                tmp_books[i] = tmp_books[j];
                tmp_books[j] = aux;
            }
        }
    }

    print_tmp_books(tmp_books, found);

    if (found == 0) {
        printf("NOT FOUND\n\n");
    }
}
 /*This function is created in case we have to add a book.
 */
void read_book(book_t **books, int *nr_of_books) {
    book_t *aux = realloc(*books, (*nr_of_books + 1) * sizeof(book_t));
    if (aux == NULL) {
        fprintf(stderr, "%s\n", "Error at realloc for books array");
        exit(1);
    }

    *books = aux;

    scanf("%d\n", &((*books)[*nr_of_books].id));

    fgets((*books)[*nr_of_books].title, MAX_TITLE_LENGTH, stdin);
    (*books)[*nr_of_books].title[strlen((*books)[*nr_of_books].title) - 1] = '\0';

    fgets((*books)[*nr_of_books].author, MAX_AUTHOR_LENGTH, stdin);
    (*books)[*nr_of_books].author[strlen((*books)[*nr_of_books].author) - 1] = '\0';

    scanf("%d\n", &((*books)[*nr_of_books].release_year));

    char *current_description = malloc(MAX_DESCRIPTION_LENGTH * sizeof(char));
    fgets(current_description, MAX_DESCRIPTION_LENGTH, stdin);
    current_description[strlen(current_description) - 1] = '\0';

    (*books)[*nr_of_books].description = malloc((strlen(current_description) + 1) * sizeof(char));
    strncpy((*books)[*nr_of_books].description, current_description, strlen(current_description) + 1);

    free(current_description);

    *nr_of_books = *nr_of_books + 1;
}

/*We creat the funtion to encode every word from the description,
and then we call it in the 'encode _book'.
*/
char *encode_word(char word[]) {
    char *new_word = calloc(2 * strlen(word) + 1, sizeof(char));

    int i, j;
    for (i = 0; i < strlen(word); i++) {
        char code[MAX_CODE_LENGTH];
        //Daca nu e numar si e litera
        if (!isdigit(word[i]) && isalpha(word[i])) {
            int nr_of_letters = 0;

            for (j = i; j < strlen(word); j++) {
                if (word[i] == word[j]) {
                    nr_of_letters++;

                    if (j == strlen(word) - 1) {
                        i = j;
                        break;
                    }
                } else {
                    i = j - 1;
                    break;
                }
            }

            sprintf(code, "%c%X", word[i], nr_of_letters);
        } else {
            sprintf(code, "%c", word[i]);
        }

        strcat(new_word, code);
    }

    char *current_word = calloc(strlen(new_word) + 1, sizeof(char));
    strncpy(current_word, new_word, strlen(new_word) + 1);

    free(new_word);
    return current_word;
}

/*We creat this function to be used in 'encode_book' 
to reverse the description.
*/
void reverse(char description[]) {
    int i;
    for (i = 0; i < strlen(description) / 2; i++) {
        char aux = description[i];
        description[i] = description[strlen(description) - 1 - i];
        description[strlen(description) - 1 - i] = aux;
    }
}

void encode_book(book_t *book) {
    // In the worst case the new_description has double the lenght of the initial one.
    char *new_description = calloc(2 * strlen(book->description) + 1, sizeof(char));

    //We make a sepparation of the words from description.
    char *word = strtok(book->description, " ");
    while (word != NULL) {
        char *current_word = encode_word(word);
        strcat(new_description, current_word);

        word = strtok(NULL,  " ");
        if (word == NULL) {
            break;
        } else {
            strcat(new_description, " ");
        }
    }

    free(book->description);

    book->description = calloc(strlen(new_description) + 1, sizeof(char));
    strncpy(book->description, new_description, strlen(new_description) + 1);

    free(new_description);
    reverse(book->description);
}

void replace_word(book_t *books, int nr_of_books, char word1[25], char word2[25]) {
    int i;
    for (i = 0; i < nr_of_books; i++) {
        int nr_of_replaces = 0;

        /*We creat a copy of the description with the 'strdup' function which
        also allocates the memory for the new string by calling malloc().
        */
        char *initial = strdup(books[i].description);
        char *found = strtok(initial, " ");

        while (found != NULL) {
            if (strcmp(found, word1) == 0) {
                nr_of_replaces++;
            }

            //Moving to the next word.
            found = strtok(NULL, " ");
        }

        free(initial);

        if (nr_of_replaces != 0) {
            int new_description_size = strlen(books[i].description) + nr_of_replaces * (strlen(word2) - strlen(word1)) + 1;
            char *new_description = calloc(new_description_size, sizeof(char));

            //We make a sepparation of the words from description.
            char *word = strtok(books[i].description, " ");
            while (word != NULL) {
                if (strcmp(word, word1) == 0) {
                    strcat(new_description, word2);
                } else {
                    strcat(new_description, word);
                }

                //We continue searching for other words.
                word = strtok(NULL, " ");
                if (word == NULL) {
                    break;
                } else {
                    strcat(new_description, " ");
                }
            }

            free(books[i].description);
            books[i].description = new_description;
        }
    }
}

int main() 
{
    int nr_of_books, i;
    scanf("%d", &nr_of_books);
    
    book_t *books = malloc(nr_of_books * sizeof(book_t));

    read_books(books, nr_of_books);

    int nr_of_commands;
    scanf("%d", &nr_of_commands);

    for (i = 0; i < nr_of_commands; i++) {
        char command[MAX_COMMAND_LENGTH];
        scanf("%s", command);

        if (strcmp(command, "SEARCH") == 0) {
            getchar();

            char element[MAX_TITLE_LENGTH];
            fgets(element, MAX_TITLE_LENGTH, stdin);
            element[strlen(element) - 1] = '\0';

            search_in_books(books, nr_of_books, element);            
        } else if (strcmp(command, "ADD_BOOK") == 0) {
            read_book(&books, &nr_of_books);
        } else if (strcmp(command, "ENCODE") == 0) {
            int encode_id;
            scanf("%d", &encode_id);

            int j;
            for (j = 0; j < nr_of_books; j++) {
                if (books[j].id == encode_id) {
                    encode_book(&books[j]);
                }
            }
        } else if (strcmp(command, "REPLACE") == 0) {
            char word1[25], word2[25];
            scanf("%s", word1);
            scanf("%s", word2);

            replace_word(books, nr_of_books, word1, word2);
        }
    }
    
    print_books(books, nr_of_books);

    for (i = 0; i < nr_of_books; i++) {
        free(books[i].description);
    }

    free(books);
    return 0;
}
