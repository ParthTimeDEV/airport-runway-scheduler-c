#include <stdio.h>
#include "scheduler.h"

int main()
{
  Scheduler s;
  s.pq.size = 0;
  s.time_now = 0;
  s.seq_counter = 0;
  for (int i = 0; i < 2; i++)
  {
    s.runways[i].current_plane = NULL;
    s.runways[i].busy_until = 0;
  }

  addPlane(&s, "AI101", "landing", 0, 25);
  addPlane(&s, "SG202", "takeoff", 0, 0);
  addPlane(&s, "IN999", "landing", 0, 5);
  addPlane(&s, "EM001", "landing", 1, 60);

  for (int t = 0; t < 20; t++)
  {
    stepScheduler(&s);
  }

  return 0;
}
