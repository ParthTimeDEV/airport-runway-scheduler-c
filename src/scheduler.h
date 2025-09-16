#ifndef SCHEDULER_H
#define SCHEDULER_H

typedef struct
{
  char flight_id[10];
  char operation[10]; // "landing" or "takeoff"
  int emergency;      // 1 or 0
  int fuel_level;     // valid if landing
  int scheduled_time; // time of request
  int seq;            // tie-breaker
} Plane;

typedef struct
{
  Plane *planes[100]; // max 100 planes
  int size;
} PriorityQueue;

typedef struct
{
  Plane *current_plane;
  int busy_until;
} Runway;

typedef struct
{
  PriorityQueue pq;
  Runway runways[2]; // 2 runways
  int time_now;
  int seq_counter;
} Scheduler;

// function declarations
int comparePlanes(Plane *a, Plane *b);
void heapifyUp(PriorityQueue *pq, int idx);
void heapifyDown(PriorityQueue *pq, int idx);
void pushPlane(PriorityQueue *pq, Plane *p);
Plane *popPlane(PriorityQueue *pq);

void addPlane(Scheduler *s, char *id, char *op, int emergency, int fuel);
void stepScheduler(Scheduler *s);

#endif
