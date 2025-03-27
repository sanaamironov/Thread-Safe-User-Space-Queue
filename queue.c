/*
 * File:        queue.c
 * Author:      Sanaa Mironov
 * Created:     2025-03-27
 * Description:
 *   Contains the implementation of a dynamic, thread-safe queue using
 *   POSIX threads. The queue is protected by a pthread_mutex and uses a
 *   pthread_cond to signal between producer and consumer threads.
 *
 *   Part of the "User-Space Multithreaded Queue Demo" teaching project.
 *
 * Course:      Operating Systems
 * University:  UMBC
 * License:     MIT License (for educational use)
 */

#include "queue.h"

// -----------------------------------------------------------------------------
// Logging Helpers for Locking
// -----------------------------------------------------------------------------

// Logs and acquires a mutex lock
void log_lock_acquire(pthread_mutex_t *mtx, const char *label)
{
    printf(LOCK_TAG "TID: %lu is trying to acquire lock (%s) at %p\n",
           pthread_self(), label, (void *)mtx);
    pthread_mutex_lock(mtx);
    printf(LOCK_TAG "TID: %lu acquired lock (%s)\n",
           pthread_self(), label);
}

// Logs and releases a mutex lock
void log_lock_release(pthread_mutex_t *mtx, const char *label)
{
    pthread_mutex_unlock(mtx);
    printf(UNLOCK_TAG "TID: %lu released lock (%s)\n",
           pthread_self(), label);
}

// -----------------------------------------------------------------------------
// Queue Implementation
// -----------------------------------------------------------------------------

// Dynamically allocates and initializes a new thread-safe queue
Queue *createQueue()
{
    Queue *q = (Queue *)malloc(sizeof(Queue));
    if (!q)
    {
        fprintf(stderr, "Failed to allocate queue\n");
        return NULL;
    }

    q->front = q->rear = NULL;
    pthread_mutex_init(&q->mutex, NULL);
    pthread_cond_init(&q->cond, NULL);
    return q;
}

// Helper function to check if the queue is empty
static int isEmpty(Queue *q)
{
    return q->front == NULL;
}

// Enqueues a new value into the queue
// Called by producer threads
void enqueue(Queue *q, int data)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (!newNode)
    {
        fprintf(stderr, "Memory allocation failed for new node\n");
        return;
    }

    newNode->data = data;
    newNode->next = NULL;

    // Lock before modifying shared queue
    log_lock_acquire(&q->mutex, "queue->mutex");

    // Insert at end of queue
    if (isEmpty(q))
    {
        q->front = q->rear = newNode;
    }
    else
    {
        q->rear->next = newNode;
        q->rear = newNode;
    }

    // Log the enqueue action
    printf(COLOR_BLUE "[Producer | PID: %d | TID: %lu] Enqueued: %d\n" COLOR_RESET,
           getpid(), pthread_self(), data);

    // Wake up one waiting consumer
    pthread_cond_signal(&q->cond);

    // Unlock after update
    log_lock_release(&q->mutex, "queue->mutex");
}

// Dequeues a value from the queue
// Called by consumer threads
int dequeue(Queue *q)
{
    log_lock_acquire(&q->mutex, "queue->mutex");

    // Wait until there's something to dequeue
    while (isEmpty(q))
    {
        pthread_cond_wait(&q->cond, &q->mutex);
    }

    // Remove from front of queue
    Node *temp = q->front;
    int data = temp->data;
    q->front = q->front->next;

    // If queue becomes empty, reset rear
    if (q->front == NULL)
    {
        q->rear = NULL;
    }

    free(temp);

    // Log the dequeue action
    printf(COLOR_GREEN "[Consumer | PID: %d | TID: %lu] Dequeued: %d\n" COLOR_RESET,
           getpid(), pthread_self(), data);

    log_lock_release(&q->mutex, "queue->mutex");

    return data;
}

// Frees all nodes and destroys the queue safely
void deleteQueue(Queue *q)
{
    log_lock_acquire(&q->mutex, "queue->mutex");

    Node *current = q->front;
    while (current)
    {
        Node *temp = current;
        current = current->next;
        free(temp);
    }

    log_lock_release(&q->mutex, "queue->mutex");

    // Clean up synchronization primitives
    pthread_mutex_destroy(&q->mutex);
    pthread_cond_destroy(&q->cond);

    free(q);
}
