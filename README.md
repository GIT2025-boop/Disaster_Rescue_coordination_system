# 🚨 Disaster Rescue Coordination System

---

## 📌 Project Description

This project simulates a disaster rescue system using Linux system calls and IPC mechanisms. It demonstrates how multiple processes communicate and coordinate in real-time to handle disaster events and assign rescue teams based on severity and zone.

---

## ⚙️ Features

* Multi-process system (C1, C2, C3)
* Multi-threading with pthreads
* IPC mechanisms:
  * Message Queue (C1 → C2)
  * FIFO Pipe (C2 → C3)
  * Shared Memory + Semaphore (C3 for stats)
* Real-time disaster monitoring and logging
* Signal Handling (SIGUSR1 triggers emergency snapshot)

---

## 🎯 Scenario

In real-world systems (IoT / Smart Cities / Industrial Monitoring), disasters like:
- Fire
- Flood
- Gas Leak
- Earthquake

must be detected and handled immediately.

This system:
1. Generates disaster events
2. Assigns rescue teams based on severity
3. Monitors and logs all activities
4. Maintains real-time statistics

---

## 📊 System Architecture

<img width="1536" height="1024" alt="diagram png" src="https://github.com/user-attachments/assets/6aa93438-2629-4e58-8338-8bc28130e37e" />

---
## 🔄 Data Flow

C1 (Message Queue) → C2 (FIFO Pipe) → C3 (Shared Memory + Logs)

---

## 🧠 Modules & Responsibilities

| Process                        | Threads                           | Responsibilities                                                                                                                                                                                 |
| ------------------------------ | --------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| **C1: Alert & Risk Manager**   | **T1: Sensor Simulator**          | - Generates random disaster events (Fire, Flood, Earthquake, Gas Leak, Building Collapse)<br>- Assigns severity (LOW, MEDIUM, HIGH)<br>- Sends alerts to C2 using **Message Queue**              |
| **C2: Rescue & Coordination**  | **T1: Receiver & Team Allocator** | - Receives disaster alerts from C1<br>- Assigns rescue teams based on severity (HIGH→1, MEDIUM→2, LOW→3)<br>- Forwards updated message to C3 using **FIFO (Named Pipe)**                         |
| **C3: Monitoring & Analytics** | **T1: Stats Monitor**             | - Receives updates from C2<br>- Maintains statistics (Total, HIGH, MEDIUM, LOW, Zone-wise)<br>- Displays real-time monitoring output with timestamp<br>- Detects critical alerts (HIGH priority) |
| **C3 (Signal Handler)**        | **T2: Emergency Reporter**        | - Handles **SIGUSR1** signal<br>- Generates emergency snapshot<br>- Saves statistics to file (`stats_snapshot.txt`)                                                                              |

---

## 🔗 IPC & Linux System Calls

| Process | IPC / Linux Calls                                                                                             |
| ------- | ------------------------------------------------------------------------------------------------------------- |
| **C1**  | `mq_send()`, `mq_receive()`, `rand()`, `pthread_create()`, `timer_create()`                                   |
| **C2**  | `pthread_create()`, `pthread_mutex_lock/unlock()`, `write()/read()` (FIFO), `mq_send()`                       |
| **C3**  | `shm_open()`, `mmap()`, `sem_init()`, `sem_wait()/sem_post()`, `open()/write()/close()`, `signal()` (SIGUSR1) |

---

## 🔔 Signals Used

| Signal          | Function                    |
| --------------- | --------------------------- |
| SIGUSR1         | Generate emergency snapshot |
| SIGINT (Ctrl+C) | Cleanup resources           |

---

## 📂 Project Structure

```

disaster_rescue_system/
│
├── src/
│   ├── alert_manager.c
│   ├── rescue_coord.c
│   ├── monitoring.c
│   └── control_center.c
│
├── include/
│   └── common.h
│
├── build/
│   ├── c1
│   ├── c2
│   ├── c3
│   └── control
│
├── logs/
│   └── disaster_log.txt
│
├── data/
│   └── stats_snapshot.txt
│
└── README.md

````

---

## 🛠️ Compilation

```bash
gcc src/alert_manager.c -o build/c1 -lpthread -lrt
gcc src/rescue_coord.c -o build/c2 -lpthread -lrt
gcc src/monitoring.c -o build/c3 -lpthread
gcc src/control_center.c -o build/control -lrt
````

---

## ▶️ Execution

```bash
./build/control
```

---

## 📊 Sample Output

```
[C1 ALERT MANAGER]
Generated -> Disaster: Fire | Severity: HIGH | Zone: 2

[C2 RESCUE COORDINATOR]
Received -> ...
Team Assigned -> 1

[C3 MONITORING]
Monitoring -> ...

🚨 EMERGENCY SNAPSHOT (SIGUSR1) 🚨
Total: 10
HIGH: 5 | MEDIUM: 3 | LOW: 2
```

---

## 📁 Log File Example

```
Disaster: Fire | Severity: HIGH | Zone: 2 | Team Assigned: 1
```

---

## 🧪 Key Learning Outcomes

* Process creation using `fork()`
* Thread synchronization using semaphores
* IPC mechanisms integration
* Signal handling
* Real-time system simulation

---

## 🚀 Future Improvements

* GUI dashboard
* Network-based communication (sockets)
* Database integration
* AI-based risk prediction

---

## 👩‍💻 Author

**Mrudula Bhandurge**
TE Electronics & Telecommunication

---
