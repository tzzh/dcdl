#include <stdio.h>
#include <stdlib.h>

typedef enum { ADD, SUB, MULT, DIV, NONE } op_type;

typedef struct calc {
    int val;
    op_type op;
    struct calc *left;
    struct calc *right;
} calc_t;

typedef struct node {
    calc_t *calc;
    struct node *next;
} node_t;

void insert(node_t **head, calc_t *calc) {
    node_t* new_node = malloc(sizeof(node_t));

    new_node->calc = calc;
    new_node->next = *head;
    *head = new_node;
}

void concat(node_t *head, node_t *rest) {
    node_t* last_node = head;
    while(last_node->next != NULL) {
        last_node = last_node->next;
    }
    last_node->next = rest;
}

node_t* get_parents(calc_t* calc) {
    if(calc->op == NONE){
        node_t* res = malloc(sizeof(node_t));
        res->calc = calc;
        res->next = NULL;
        return res;
    }
    else {
        node_t* left = get_parents(calc->left);
        node_t * right = get_parents(calc->right);
        concat(left, right);
        return left;
    }
}

void print_list(node_t* head) {
    node_t* current_node = head;
    while(current_node != NULL) {
        calc_t current_calc = *current_node->calc;
        printf("%i", current_calc.val);
        if( current_calc.op != NONE ) {
            char* s;
            switch(current_calc.op) {
               case ADD:
                  s = "+";
                  break;
               case SUB:
                  s = "-";
                  break;
               case MULT:
                  s = "*";
                  break;
               case DIV:
                  s = "/";
                  break;
                default:
                  s = "";
                  break;
            }
            printf(" : %i %s %i", current_calc.left->val, s, current_calc.right->val);
        }
        printf("\n");
        current_node = current_node->next;
    }
}

int overlap(calc_t *a, calc_t *b){
    node_t * a_parents = get_parents(a);
    node_t * b_parents = get_parents(b);

    node_t * node_i = a_parents;
    node_t * node_j;
    while(node_i != NULL){
        node_j = b_parents;
        while(node_j != NULL){
            if(node_i->calc == node_j->calc){
                return 1;
            }
            node_j = node_j->next;
        }
        node_i = node_i->next;
    }
    return 0;
}



node_t* combinations(calc_t* x_calc, calc_t* y_calc) {
    node_t *head = NULL;

    int x = x_calc->val;
    int y = y_calc->val;

    calc_t* add_calc = malloc(sizeof(calc_t));
    add_calc->val = x + y;
    add_calc->op = ADD;
    add_calc->left = x_calc;
    add_calc->right = y_calc;
    insert(&head, add_calc);

    if( x != 1 && y != 1) {
        calc_t* mult_calc = malloc(sizeof(calc_t));;
        mult_calc->val = x * y;
        mult_calc->op = MULT;
        mult_calc->left = x_calc;
        mult_calc->right = y_calc;
        insert(&head, mult_calc);
    }

    if( x != y) {
        calc_t* sub_calc = malloc(sizeof(calc_t));
        sub_calc->op = SUB;
        if( x > y){
            sub_calc->val = x - y;
            sub_calc->left = x_calc;
            sub_calc->right = y_calc;
        }
        else {
            sub_calc->val = y - x;
            sub_calc->left = y_calc;
            sub_calc->right = x_calc;
        }
        insert(&head, sub_calc);
    }

    if( (x % y == 0 && y != 1) ||  (y % x == 0 && x != 1) ) {
        calc_t *div_calc = malloc(sizeof(calc_t));
        div_calc->op = DIV;
        if( x % y == 0) {
            div_calc->val = x / y;
            div_calc->left = x_calc;
            div_calc->right = y_calc;
        }
        else {
            div_calc->val = y / x;
            div_calc->left = y_calc;
            div_calc->right = x_calc;
        }
        insert(&head, div_calc);
    }
    
    return head;
}


int main(int argc, char **argv)
{
    int NUMBERS = 4;
    if(argc != NUMBERS + 2){
        printf("Wrong number of arguments: %i\n", argc);
        return 0;
    }

    int goal = atoi(argv[1]);

    node_t *head = NULL;
    for (int i = 0; i < NUMBERS; ++i) {
        calc_t *x_calc = malloc(sizeof(calc_t));
        x_calc->val = atoi(argv[i + 2]);
        x_calc->op = NONE;
        insert(&head, x_calc);
    }

    node_t *prev_head = NULL;
    node_t *tmp_head = NULL;
    while(head != prev_head) {
        tmp_head = head;
        node_t *node_i = head;
        while( node_i != prev_head ) {
            node_t *node_j = node_i->next;
            while(node_j != NULL) {
                if(!overlap(node_i->calc, node_j->calc)) {
                    node_t * temp = combinations(node_i->calc, node_j->calc);
                    concat(temp, head);
                    head = temp;
                }
                node_j = node_j->next;
            }
            node_i = node_i->next;
        }
        prev_head = tmp_head;
    }

    print_list(head);
}
