# Airport Runway Scheduler (C)

A data structures project using **Min-Heap (priority queue)** and **queue logic** to simulate airport runway scheduling.

## ✈️ Features

- Priority scheduling of planes:
  - Emergency flights always first
  - Among landings: lower fuel = higher priority
  - Otherwise: earlier scheduled time
- Manages multiple runways (2 by default)
- Uses heap for priority and simulation of time steps

## 🛠️ Build & Run

```bash
make
./scheduler
```
