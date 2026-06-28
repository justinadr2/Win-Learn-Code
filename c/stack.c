#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct Node_ {
    int val;
    struct Node_* next;
} Node;

typedef struct Stack_ {
    Node* top;
} Stack;

void Push(Stack* stack, int val) {
    Node* node = malloc(sizeof(Node));
    if (!node)
        return;
    node->next = stack->top;
    node->val = val;
    stack->top = node;
}

int Pop(Stack* stack) {
    if (!stack->top)
        return 0;
    Node* tmp = stack->top;
    int out = tmp->val;
    stack->top = tmp->next;
    free(tmp);
    return out;
}

void Print(Stack* stack) {
    if (!stack->top)
        return;
    printf("%i <- top\n", stack->top->val);
    Node* tmp = stack->top->next;
    while (tmp) {
        printf("%i\n", tmp->val);
        tmp = tmp->next;
    }
    printf("\n");
}

void Sort(Stack* stack) {
    if (!stack->top || !stack->top->next)
        return;

    for (Node* curr = stack->top; curr; curr = curr->next) {
        for (Node* next = curr->next; next; next = next->next) {
            if (curr->val > next->val) {
                int tmp = curr->val;
                curr->val = next->val;
                next->val = tmp;
            }
        }
    }
}

void Reverse(Stack* stack) {
    if (!stack->top || !stack->top->next)
        return;
    
    Node* prev = NULL;
    Node* curr = stack->top;
    Node* next = NULL;
    while (curr) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    stack->top = prev;
}

Stack StackInit(int* init, int size) {
    Stack stack;
    stack.top = NULL;
    for (int i = 0; i < size; i++)
        Push(&stack, init[i]);
    return stack;       
}

void Free(Stack* stack) {
    while (stack->top) {
        Node* tmp = stack->top;
        stack->top = tmp->next;
        free(tmp);
    }
}

int main() {
    int init[] = { 60, 40, 70, 50 };
    Stack stack = StackInit(init, sizeof(init) / sizeof(init[0]));        

    Push(&stack, 80);
    
    Print(&stack);
    
    Reverse(&stack);

    Print(&stack);

    Free(&stack);
}