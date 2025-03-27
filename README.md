# 🧵 User-Space Multithreaded Queue (Producer-Consumer Demo)

📺 **Demo Video**: [Watch here](https://youtu.be/v21JQaozgII)

This project demonstrates a **thread-safe dynamic queue** implementation in pure C, using **POSIX threads** (`pthreads`). It simulates a classic **producer-consumer problem**, where multiple threads interact with a shared queue protected by a mutex and condition variable.

---

## 🎯 Objectives

- Teach dynamic data structure design in C (linked-list queue)
- Demonstrate correct use of:
  - `pthread_mutex_t` for mutual exclusion
  - `pthread_cond_t` for signaling between threads
- Simulate a multithreaded environment
- Practice concurrent programming techniques
- Set the foundation for porting this design to **kernel space**

---
## 🧩 Project Structure
userspace_queue_demo/
queue_demo/
├── main.c         # producer/consumer test 
├── queue.c        # Logic 
├── queue.h        # header file
└── Makefile       #for building user-space code 
---

## 🧵 Threads Overview

- Two producer threads
  - Enqueue random numbers into the queue
- Two consumer threads
  - Dequeue and print values
- All access to the queue is synchronized via `pthread_mutex_t` and `pthread_cond_t`

---

## ✅ Key Features

| Feature           | Description                              |
|------------------|------------------------------------------|
| Dynamic queue     | Linked-list based, no size limit         |
| Mutex protection  | Prevents race conditions                 |
| Condition variable| Wakes consumers when producers enqueue   |
| Clean exit        | Queue memory is freed, threads joined    |
| Colorful output   | Terminal uses ANSI colors to trace roles |

---

## 🚀 Building and Running

 make 
 ./queue_demo

📚 References
  Beej's Guide to C Programming
  POSIX Threads Programming

  🔒 License
MIT License — for educational use
