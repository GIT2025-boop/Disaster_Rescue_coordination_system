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

## 🧠 Modules & Responsibilities

| Process                        | Threads                                        | Responsibilities                                                                                                                                                                  |
| ------------------------------ | ---------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **C1: Alert & Risk Manager**   | T1: Sensor Simulator<br>T2: Risk Calculator    | - Generates random disasters (Earthquake, Gas Leak, Building Collapse)<br>- Calculates severity (LOW, MEDIUM, HIGH)<br>- Sends alerts via Message Queue                           |
| **C2: Rescue & Coordination**  | T1: Team Allocator<br>T2: Message Broadcaster  | - Receives alerts from C1<br>- Assigns rescue teams based on severity and zone<br>- Sends updates to Monitoring using FIFO                                                        |
| **C3: Monitoring & Analytics** | T1: Stats Aggregator<br>T2: Emergency Reporter | - Receives disaster updates from C2<br>- Maintains and updates statistics: Total, HIGH, MEDIUM, LOW<br>- Responds to SIGUSR1 with emergency snapshot<br>- Logs monitoring details |

---

## 🔗 IPC & Linux System Calls

| Process | IPC / Linux Calls                                                                                             |
| ------- | ------------------------------------------------------------------------------------------------------------- |
| **C1**  | `mq_send()`, `mq_receive()`, `rand()`, `pthread_create()`, `timer_create()`                                   |
| **C2**  | `pthread_create()`, `pthread_mutex_lock/unlock()`, `write()/read()` (FIFO), `mq_send()`                       |
| **C3**  | `shm_open()`, `mmap()`, `sem_init()`, `sem_wait()/sem_post()`, `open()/write()/close()`, `signal()` (SIGUSR1) |

---

## 🛠️ How to Run

```bash
make all
./build/main
```

---

## 📊 Sample Output

```text
===== DISASTER RESCUE CONTROL CENTER =====

[CONTROL CENTER RUNNING]

[C1 ALERT MANAGER]
Generated -> Disaster: Earthquake | Severity: HIGH | Zone: 3

[C2 RESCUE COORDINATOR]
Received -> Disaster: Earthquake | Severity: HIGH | Zone: 3
Team Assigned -> 1

[C3 MONITORING]
Monitoring -> Disaster: Earthquake | Severity: HIGH | Zone: 3 | Team Assigned: 1

...

[STATS] Total = 5

[CONTROL] Sending SIGUSR1 to Monitoring...

[CONTROL CENTER RUNNING]

🚨 EMERGENCY SNAPSHOT (SIGUSR1) 🚨
Total: 5
HIGH: 2 | MEDIUM: 2 | LOW: 1
```

---

## 👩‍💻 Author

**Mrudula Bhandurge**

---

