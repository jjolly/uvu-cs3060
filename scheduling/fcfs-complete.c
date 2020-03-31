/* First come, first served scheduling algorithm simulator
 * By: John Jolly (c)2020
 * This program simulates the First Come, First Served scheduling
 * algorithm. It can be given a list of jobs, each having a start time
 * and a total run time. All values should be separated with white space.
 * The program displays the results when all jobs have been processed.
 */
/* License and Attribution
 * MIT License
 *
 * Copyright (c) 2020 John Jolly
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include<stdio.h>

#define MAX_JOBS (100)

void fcfs(int, int*, int*);

int main(int argc, char *argv[]) {
  int count, start[MAX_JOBS], run[MAX_JOBS];
  FILE *fh = stdin;

  if(argc > 1 && NULL == (fh = fopen(argv[1], "r"))) {
    perror("Error opening file");
    return 1;
  }

  for(count = 0; count < MAX_JOBS && 2 == fscanf(fh, "%d %d", &start[count], &run[count]); count++);

  fcfs(count, start, run);
}

#define NEXT_Q(x) (((x) + 1) % MAX_JOBS)
#define PREV_Q(x) (((x) - 1 + MAX_JOBS) % MAX_JOBS)

void fcfs(int count, int *start, int *run) {
  int i, next_clock;
  int resp = 0, ta = 0, wait = 0, clock = 0;
  int ready[MAX_JOBS], remain[MAX_JOBS];
  // next: Next job to remove from the job list
  // last: Entry after the last job in the ready queue
  // curr: Current entry running in the ready queue
  int next = 0, last = 0, curr = 0;
  int running;

  while(next < count || curr != last) {
    // Move clock forward to the running job if necessary
    if(curr == last && clock < start[next])
      clock = start[next];

    // if the ready queue is empty add the next job to the queue
    while(next < count && clock >= start[next]) {
      ready[last] = next;
      last = NEXT_Q(last);
      remain[next] = run[next];
      next++;
    }
    
    // Keep track of the job that is now running on the CPU
    // [!!!]: Before chosing a job, make sure the queue has the correct
    // job at the top
    running = ready[curr];

    // Discover the next event (end of job for FCFS)
    // [!!!]: Each algorithm has it's own "next event", some even have two
    next_clock = clock + remain[running];

    // Calculate stats
    // [!!!]: Non-preemptive algorithms can have their stats calculated
    // in a group like this, but preemptive algorithms might be able to
    // calculate one stat now, but not the others
    resp += clock - start[running];
    ta += next_clock - start[running];
    wait += next_clock - start[running] - run[running];

    // Adjust remaining time
    // [!!!]: This means almost absolutely nothing to FCFS, but other
    // algorithms need to manage the remaining time and react to it
    remain[running] -= next_clock - clock;
    
    // Advance clock before processing next job
    clock = next_clock;

    // Add arriving jobs to the ready queue
    while(next < count && clock >= start[next]) {
      ready[last] = next;
      last = NEXT_Q(last);
      remain[next] = run[next];
      next++;
    }

    // [!!!]: In other algorithms the job isn't finished, and must be
    // re-added to the ready queue. This should be done after adding
    // any new jobs

    curr = NEXT_Q(curr);
  }

  printf("Avg. Resp.:%.2f, Avg. T.A.:%.2f, Avg. Wait:%.2f\n",
      (float)resp / count, (float)ta / count, (float)wait / count);
}
