# 🚨 Disaster Rescue 
Coordination System

## 📌 Project Description
This project simulates a **Disaster Rescue Coordination System** using Linux system calls and POSIX APIs. It demonstrates real-time disaster management using multiple processes, threads, and IPC mechanisms.

---

## 🧠 System Architecture
    +----------------------+
    |   Control Center     |
    |      (Parent)        |
    +----------+-----------+
               |
 -----------------------------------
 |                |                |
+---------+ +---------+ +---------+
| C1 | | C2 | | C3 |
| Alert | --> | Rescue | --> | Monitor |
| Manager | MQ | Coord | FIFO| & Stats |
+---------+ +---------+ +---------+
|
Shared Memory
|
Logs/File

## 🔄 Data Flow Diagram

---

C1 (Sensor Simulation)
↓
Message Queue (mq_send)
↓
C2 (Team Allocation)
↓
FIFO Pipe (write)
↓
C3 (Monitoring & Analytics)
↓
Shared Memory + Semaphore
↓
Logs + Stats Snapshot

---

## ⚙️ Features

- Multi-process system (fork, exec)
- Multi-threading using pthreads
- Multiple IPC mechanisms:
  - Message Queue
  - FIFO Pipe
  - Shared Memory + Semaphore
- Real-time logging system
- Random disaster simulation

---

## 🧩 Modules

### 🔹 C1 – Alert & Risk Manager
- Generates random disasters
- Calculates severity (LOW, MEDIUM, HIGH)
- Sends alerts using **message queue**

### 🔹 C2 – Rescue & Coordination
- Receives disaster messages
- Assigns rescue teams
- Sends updates via **FIFO pipe**

### 🔹 C3 – Monitoring & Analytics
- Reads messages from pipe
- Updates statistics
- Stores data in **shared memory**
- Logs results to files

---

## 🛠️ System Calls Used

### Process Control
- `fork()`, `execl()`

### Threads
- `pthread_create()`

### IPC
- `mq_send()`, `mq_receive()`
- `pipe()`, `mkfifo()`
- `shm_open()`, `mmap()`
- `sem_wait()`, `sem_post()`

### File Handling
- `open()`, `read()`, `write()`, `close()`

---

## 📂 Folder Structure
disaster_rescue_system/
├── src/ # Source files
├── include/ # Header files
├── build/ # Executables
├── logs/ # Log files
└── data/ # Output data

---

## ▶️ How to Run

```bash
make all
./build/main

📊 Sample Output
C1 Sent: Disaster: Fire | Severity: HIGH | Zone: 2
C2 Received: Disaster Event Level: HIGH
Allocating Rescue Team 1...
C3 Monitoring: Disaster Event Level: HIGH | Team Assigned: 1
Updating Stats...
