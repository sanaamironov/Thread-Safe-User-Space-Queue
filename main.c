/*
 * File:        main.c
 * Author:      Sanaa Mironov
 * Created:     2025-03-27
 * Description:
 *   Entry point for the user-space multithreaded queue demo.
 *   This file creates and runs multiple producer and consumer threads
 *   which operate on a shared thread-safe queue.
 *   Producers enqueue random integers, and consumers dequeue and print them.
 *
 *   Part of the "User-Space Multithreaded Queue Demo" teaching project.
 *
 * Course:      Operating Systems
 * University:  UMBC
 * License:     MIT License (for educational use)
 */

#include "queue.h"

// -----------------------------------------------------------------------------
// Producer Thread Function
// -----------------------------------------------------------------------------

// Each producer enqueues 5 random integers into the shared queue
void *producer(void *arg)
{
    Queue *q = (Queue *)arg;
    for (int i = 0; i < 5; i++)
    {
        int val = rand() % 100; // Generate random number
        enqueue(q, val);        // Enqueue it to the shared queue
        usleep(100000);         // Sleep for 100ms to simulate work
    }
    return NULL;
}

// -----------------------------------------------------------------------------
// Consumer Thread Function
// -----------------------------------------------------------------------------

// Each consumer dequeues 5 values from the shared queue
void *consumer(void *arg)
{
    Queue *q = (Queue *)arg;
    for (int i = 0; i < 5; i++)
    {
        dequeue(q);     // Dequeue one value (blocks if empty)
        usleep(150000); // Sleep for 150ms to simulate work
    }
    return NULL;
}

// -----------------------------------------------------------------------------
// Main Program Entry Point
// -----------------------------------------------------------------------------

int main()
{
    Queue *q = createQueue(); // Allocate and initialize the shared queue
    pthread_t threads[4];     // Two producers, two consumers

    printf("\n🌀 Starting Thread-Safe Queue Demo (PID: %d)\n\n", getpid());

    // Create producer threads
    pthread_create(&threads[0], NULL, producer, q);
    pthread_create(&threads[1], NULL, producer, q);

    // Create consumer threads
    pthread_create(&threads[2], NULL, consumer, q);
    pthread_create(&threads[3], NULL, consumer, q);

    // Wait for all threads to complete
    for (int i = 0; i < 4; i++)
    {
        pthread_join(threads[i], NULL);
    }

    deleteQueue(q); // Cleanup queue memory and synchronization primitives

    printf("\n✅ Queue processing completed. Exiting.\n\n");
    return 0;
}
