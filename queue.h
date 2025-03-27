/*
 * File:        queue.h
 * Author:      Sanaa Mironov
 * Created:     2025-03-27
 * Description:
 *   Header file for a dynamic, thread-safe queue implementation using POSIX threads.
 *   Defines the data structures, synchronization primitives, and function prototypes
 *   used in the queue implementation.
 *
 *   Part of the "User-Space Multithreaded Queue Demo" teaching project.
 *
 * Course:      Operating Systems
 * University:  UMBC
 * License:     MIT License (for educational use)
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

// -----------------------------------------------------------------------------
// ANSI Terminal Color Macros for Output Formatting
// -----------------------------------------------------------------------------

#define COLOR_RESET "\033[0m"
#define COLOR_GREEN "\033[1;32m"   // Consumer
#define COLOR_BLUE "\033[1;34m"    // Producer
#define COLOR_YELLOW "\033[1;33m"  // Lock acquired
#define COLOR_MAGENTA "\033[1;35m" // Lock released

#define LOCK_TAG COLOR_YELLOW "[LOCK]   " COLOR_RESET
#define UNLOCK_TAG COLOR_MAGENTA "[UNLOCK] " COLOR_RESET

// -----------------------------------------------------------------------------
// Queue Node Definition
// -----------------------------------------------------------------------------

// Represents a single element in the queue
typedef struct Node
{
    int data;          // Payload value
    struct Node *next; // Pointer to the next node in the queue
} Node;

// -----------------------------------------------------------------------------
// Thread-Safe Queue Structure
// -----------------------------------------------------------------------------

typedef struct Queue
{
    Node *front;           // Pointer to the front of the queue
    Node *rear;            // Pointer to the rear of the queue
    pthread_mutex_t mutex; // Mutex to protect access to the queue
    pthread_cond_t cond;   // Condition variable for blocking dequeue
} Queue;

// -----------------------------------------------------------------------------
// Queue API (Public Interface)
// -----------------------------------------------------------------------------

// Allocates and initializes a new queue
Queue *createQueue();

// Safely deletes the queue and frees all resources
void deleteQueue(Queue *q);

// Adds an item to the queue (thread-safe)
void enqueue(Queue *q, int data);

// Removes an item from the queue, blocks if empty (thread-safe)
int dequeue(Queue *q);

// -----------------------------------------------------------------------------
// Logging Helpers for Locking (Optional)
// -----------------------------------------------------------------------------

// Logs a thread attempting to acquire a lock, then acquires it
void log_lock_acquire(pthread_mutex_t *mtx, const char *label);

// Logs a thread releasing a lock, then releases it
void log_lock_release(pthread_mutex_t *mtx, const char *label);

#endif // QUEUE_H
