#include <stdio.h>
#include <stdlib.h>

typedef struct Node_ {
    int val;
    struct Node_* next;
} Node;

typedef struct Queue_ {
    Node* front;
    Node* rear;
} Queue;


void enqueue(Queue* q, int a) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->val = a;
    node->next = NULL;
    if (!q->front) {
        q->front = q->rear = node;
    }
    else {
        q->rear->next = node;
        q->rear = node;
    }
}

int dequeue(Queue* q) {
    if (!q->front) {
        printf("Cannot dequeue, its empty\n");
        return 0;
    }

    Node* tmp = q->front;
    q->front = tmp->next;
    if (!q->front)
        q->rear = NULL;
    
    int out = tmp->val;
    free(tmp);
    return out;
}

int peek_front(Queue* q) {
    if (!q->front) {
        printf("Cannot get front, its empty\n");
        return 0;
    }
    return q->front->val;
}

int peek_rear(Queue* q) {
    if (!q->rear) {
        printf("Cannot get rear, its empty\n");
        return 0;
    }
    return q->rear->val;
}

void print_queue(Queue* q) {
    if (!q->front) {
        printf("Cannot print queue, its empty\n");
        return;
    }

    printf("%i <- front\n", q->front->val);
    Node* tmp = q->front->next;
    while (tmp) {
        if (!tmp->next) {
            printf("%i <- rear\n", tmp->val);
            break;
        }
        printf("%i\n", tmp->val);
        tmp = tmp->next;
    }
}

void free_queue(Queue* q) {
    while (q->front) {
        Node* tmp = q->front;
        q->front = tmp->next;
        free(tmp);
    }
}


int main() {
    Queue line = { NULL, NULL };
    
    enqueue(&line, 400);
    enqueue(&line, 500);
    enqueue(&line, 600);
    enqueue(&line, 700);
    enqueue(&line, 800);

    int front = peek_front(&line);
    printf("%i <- front\n", front);
    int rear = peek_rear(&line);
    printf("%i <- rear\n\n", rear);

    print_queue(&line);

    int out = dequeue(&line);
    printf("\n%i <- dequeue\n", out);

    int front2 = peek_front(&line);
    printf("%i <- front\n", front2);

    free_queue(&line);
}

