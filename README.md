# 🚨 Disaster Rescue Coordination System

---

## 📌 Project Description
This project is a simulation-based system using Linux system calls and IPC mechanisms to manage disaster rescue operations. It demonstrates how multiple processes communicate and coordinate in real-time to handle disaster events.

---

## ⚙️ Features
- Multi-process system (C1, C2, C3)
- Multi-threading using pthreads
- IPC mechanisms:
  - Message Queue
  - FIFO Pipe
  - Shared Memory + Semaphore
- File logging and statistics tracking
- Signal Handling

---

## 🧠 Modules
- C1: Alert & Risk Manager
- C2: Rescue & Coordination
- C3: Monitoring & Analytics

---

## ⚙️ Processes Description

### 1. C1 – Alert & Risk Manager

* Generates random disaster events 🎲
* Calculates severity (LOW, MEDIUM, HIGH)
* Sends alerts using 📩 Message Queue

### 2. C2 – Rescue & Coordination

* Receives alerts from C1
* Assigns rescue teams based on severity 🚑
* Sends updates using 📡 FIFO Pipe

### 3. C3 – Monitoring & Analytics

* Receives data from C2
* Maintains statistics using 💾 Shared Memory
* Uses 🔒 Semaphore for synchronization
* Stores logs in 📁 files

---

## 🔗 IPC Mechanisms Used

* C1 → C2 : 📩 Message Queue
* C2 → C3 : 📡 FIFO (Named Pipe)
* C3 : 💾 Shared Memory + 🔒 Semaphore

---

## 🛠️ System Calls Used
- fork(), exec()
- pthread_create()
- mq_send(), mq_receive()
- open(), read(), write()
- shm_open(), mmap()
- sem_wait(), sem_post()

---
## ▶️ How to Run

```bash
make all
./build/main
```
---

## 📊 Sample Output

C1 Sent: Disaster: Fire | Severity: HIGH | Zone: 3 
C2 Received: Disaster: Fire | Severity: HIGH | Zone: 3
Allocating Rescue Team 1 
C3 Monitoring: Disaster: Fire | Severity: HIGH | Zone: 3 | Team Assigned: 1
Updating Stats 

---

## 👩‍💻 Author

Mrudula Bhandurge

---

