#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scheduler.h"

int comparePlanes(Plane *a, Plane *b)
{
  int a_em = a->emergency ? 0 : 1;
  int b_em = b->emergency ? 0 : 1;
  if (a_em != b_em)
    return a_em - b_em;

  int a_fuel = (strcmp(a->operation, "landing") == 0) ? a->fuel_level : 9999;
  int b_fuel = (strcmp(b->operation, "landing") == 0) ? b->fuel_level : 9999;
  if (a_fuel != b_fuel)
    return a_fuel - b_fuel;

  if (a->scheduled_time != b->scheduled_time)
    return a->scheduled_time - b->scheduled_time;

  return a->seq - b->seq;
}

void heapifyUp(PriorityQueue *pq, int idx)
{
  while (idx > 0)
  {
    int parent = (idx - 1) / 2;
    if (comparePlanes(pq->planes[idx], pq->planes[parent]) < 0)
    {
      Plane *tmp = pq->planes[idx];
      pq->planes[idx] = pq->planes[parent];
      pq->planes[parent] = tmp;
      idx = parent;
    }
    else
      break;
  }
}

void heapifyDown(PriorityQueue *pq, int idx)
{
  while (1)
  {
    int left = 2 * idx + 1, right = 2 * idx + 2, smallest = idx;
    if (left < pq->size && comparePlanes(pq->planes[left], pq->planes[smallest]) < 0)
      smallest = left;
    if (right < pq->size && comparePlanes(pq->planes[right], pq->planes[smallest]) < 0)
      smallest = right;
    if (smallest != idx)
    {
      Plane *tmp = pq->planes[idx];
      pq->planes[idx] = pq->planes[smallest];
      pq->planes[smallest] = tmp;
      idx = smallest;
    }
    else
      break;
  }
}

void pushPlane(PriorityQueue *pq, Plane *p)
{
  pq->planes[pq->size] = p;
  heapifyUp(pq, pq->size);
  pq->size++;
}

Plane *popPlane(PriorityQueue *pq)
{
  if (pq->size == 0)
    return NULL;
  Plane *top = pq->planes[0];
  pq->planes[0] = pq->planes[pq->size - 1];
  pq->size--;
  heapifyDown(pq, 0);
  return top;
}

void addPlane(Scheduler *s, char *id, char *op, int emergency, int fuel)
{
  Plane *p = malloc(sizeof(Plane));
  strcpy(p->flight_id, id);
  strcpy(p->operation, op);
  p->emergency = emergency;
  p->fuel_level = fuel;
  p->scheduled_time = s->time_now;
  p->seq = s->seq_counter++;
  pushPlane(&s->pq, p);
  printf("[t=%d] Added plane %s (%s)\n", s->time_now, id, op);
}

void stepScheduler(Scheduler *s)
{
  s->time_now++;

  // free finished runways
  for (int i = 0; i < 2; i++)
  {
    if (s->runways[i].current_plane && s->time_now >= s->runways[i].busy_until)
    {
      printf("[t=%d] Runway-%d FINISHED %s\n", s->time_now, i + 1,
             s->runways[i].current_plane->flight_id);
      free(s->runways[i].current_plane);
      s->runways[i].current_plane = NULL;
    }
  }

  // assign planes to free runways
  for (int i = 0; i < 2; i++)
  {
    if (!s->runways[i].current_plane && s->pq.size > 0)
    {
      Plane *p = popPlane(&s->pq);
      s->runways[i].current_plane = p;
      int duration = (strcmp(p->operation, "landing") == 0) ? 3 : 2;
      s->runways[i].busy_until = s->time_now + duration;
      printf("[t=%d] Runway-%d START %s (%s)\n", s->time_now, i + 1, p->flight_id, p->operation);
    }
  }
}
