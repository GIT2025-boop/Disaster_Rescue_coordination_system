# Disaster Rescue Coordination System

## 📌 Project Description
This project is a simulation-based system using Linux system calls and IPC mechanisms to manage disaster rescue operations.

## ⚙️ Features
- Multi-process system (C1, C2, C3)
- Multi-threading using pthreads
- IPC mechanisms:
  - Message Queue
  - FIFO Pipe
  - Shared Memory + Semaphore
- File logging and statistics tracking

## 🧠 Modules
- C1: Alert & Risk Manager
- C2: Rescue & Coordination
- C3: Monitoring & Analytics

## 🔄 Data Flow
C1 → (Message Queue) → C2 → (FIFO Pipe) → C3 → (Shared Memory + Logs)

## 🛠️ System Calls Used
- fork(), exec()
- pthread_create()
- mq_send(), mq_receive()
- open(), read(), write()
- shm_open(), mmap()
- sem_wait(), sem_post()

## ▶️ How to Run

```bash
make all
./build/main
