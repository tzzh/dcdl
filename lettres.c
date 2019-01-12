#include <stdio.h>
#include <stdlib.h>


const int ALPHABET_SIZE = 26;
const int ASCII_OFFSET = 97; //ASCII lower case offset

typedef struct node {
    char letter;
    struct node * next[ALPHABET_SIZE];
} node_t;

typedef node_t* alphabet_t[ALPHABET_SIZE];


int print_tree(alphabet_t base_array){
    node_t* current_node = NULL;
    for(int i =0; i < ALPHABET_SIZE; i++){
        if(base_array[i] != NULL){
            current_node = base_array[i];
            printf("%c", current_node->letter);
            for(int i =0; i < ALPHABET_SIZE; i++){
                if(current_node->next[i] != NULL){
                    current_node = base_array[i];
                    printf(":%c", current_node->letter);
                }
            }
            printf("\n");
        }
    }
    return 0;
}

int main(){
    FILE * fp;
    fp = fopen("./a", "r");

    alphabet_t base_array = {NULL};
    node_t * current_node = malloc(sizeof(node_t));

    char current_char = fgetc(fp);
    int char_index = current_char - ASCII_OFFSET;

    while( !feof(fp) ){

        if(base_array[char_index] == NULL){
            node_t * new_letter = malloc(sizeof(node_t));
            new_letter->letter = current_char;
            base_array[char_index] = new_letter;
        }

        current_node = base_array[char_index];
        current_char = fgetc(fp);
        char_index = current_char - ASCII_OFFSET;
        while( current_char != '\n'){
            if(current_node->next[char_index] == NULL){
                node_t * new_letter = malloc(sizeof(node_t));
                new_letter->letter = current_char;
                current_node->next[char_index] = new_letter;
            }
            current_node = current_node->next[char_index];
            current_char = fgetc(fp);
        }
        current_char = fgetc(fp);
        char_index = current_char - ASCII_OFFSET;
    }

    fclose(fp);
    print_tree(base_array);
    return 0;
}
