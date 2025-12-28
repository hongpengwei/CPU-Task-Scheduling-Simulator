Collecting workspace information# CPU Task Scheduling Simulator - Reproduction Document

## Overview
<<<<<<< HEAD
This document provides step-by-step instructions to reproduce and test the CPU Task Scheduling Simulator, a C-based project that implements multiple CPU scheduling algorithms with I/O operations support.
=======
This document provides step-by-step instructions to reproduce and test the CPU Task Scheduling Simulator, a C-based project that implements multiple CPU scheduling algorithms including FCFS, Round Robin, Shortest Job First, Priority Scheduling, and LIFO.


## System Requirements
- **OS:** Linux, macOS, or Windows (with WSL/MinGW)
- **Compiler:** GCC 7.0 or later
- **Build Tool:** GNU Make
- **RAM:** Minimum 256 MB
- **Disk Space:** ~5 MB

## Project Setup

### Step 1: Clone/Download the Project
```bash
cd ~/Desktop/c_project
```

The project structure should be:
```
c_project/
├── main.c
├── system.c
├── system.h
├── scheduler.c
├── scheduler.h
├── queue.c
├── queue.h
├── task.h
├── Makefile
└── README.md
```

### Step 2: Verify File Integrity
Confirm all files are present by running:
```bash
ls -la
```

Expected output should show all 10 files listed above.

## Building the Project

### Standard Build
```bash
make clean
make
```

**Expected Output:**
```
gcc -Wall -g -c main.c
gcc -Wall -g -c system.c
gcc -Wall -g -c scheduler.c
gcc -Wall -g -c queue.c
gcc -Wall -g -o sim main.o system.o scheduler.o queue.o
```

### Verify Build Success
```bash
ls -la sim
```

Should output: `-rwxr-xr-x ... sim` (executable file)

## Running the Simulator

### Test Case 1: FCFS (First Come First Served)
```bash
./sim FCFS
```

**Input Example:**
```
3
6
0
10
0
4
0
```

**Expected Behavior:**
- Tasks execute in arrival order: Task_1 → Task_2 → Task_3
- No preemption occurs
- Output shows sequential execution with increasing tick times
<<<<<<< HEAD
- Each task waits for the previous task to complete
=======
- Task_1 (6 ticks) completes first, then Task_2 (10 ticks), then Task_3 (4 ticks)


### Test Case 2: Round Robin (RR)
```bash
./sim RR
```

**Input Example:**
```
3
6
0
10
0
4
0
```

**Expected Behavior:**
- Tasks execute with 2-tick time quantum
- Context switches occur when quantum expires
<<<<<<< HEAD
- Output shows interleaved execution with fair time distribution
- Tasks are preempted after consuming their time quantum
=======
- Output shows interleaved execution: Task_1 → Task_2 → Task_3 → Task_1 → ...
- All tasks get fair CPU time


### Test Case 3: Shortest Job First (SJF)
```bash
./sim SJF
```

**Input Example:**
```
3
6
0
10
0
4
0
```

**Expected Behavior:**
- Task_3 (burst=4) executes first
- Task_1 (burst=6) executes second
- Task_2 (burst=10) executes last
<<<<<<< HEAD
- Optimal average waiting time among non-preemptive schedulers
=======
- Provides optimal average waiting time


### Test Case 4: Priority Scheduling
```bash
./sim PRIORITY
```

**Input Example:**
```
3
6
1
10
3
4
2
```

**Expected Behavior:**
- Higher priority values execute first
<<<<<<< HEAD
- Task_2 (priority=3) → Task_3 (priority=2) → Task_1 (priority=1)
- Respects priority ordering
- Input format for each task: burst_time, io_cycle, priority
=======
- Task_2 (priority=3) executes first
- Task_3 (priority=2) executes second
- Task_1 (priority=1) executes last
- Includes aging mechanism: priority increases by 1 every 10 ticks to prevent starvation


### Test Case 5: LIFO (Last In First Out)
```bash
./sim LIFO
```

**Input Example:**
```
3
6
0
10
0
4
0
```

**Expected Behavior:**
- Most recently added task executes first
<<<<<<< HEAD
- Task_3 → Task_2 → Task_1
- Stack-based ordering (FIFO reversed)

### Test Case 6: With I/O Operations
```bash
./sim RR
```

**Input Example with I/O:**
```
2
10
2
5
8
0
```

**Expected Behavior:**
- Task_1: Burst=10, I/O Cycle=2, I/O Sleep=5
  - Executes 2 ticks, then sleeps for 5 ticks
  - Wakes up and repeats until completion
- Task_2: Burst=8, no I/O (cycle=0)
  - Executes normally without I/O blocking
- Output shows SYSCALL messages for sleep and alarm events

## Input Format Reference

For each task, the simulator prompts for:

```
Enter Burst Time for Task_X: [positive integer]
Enter I/O Burst Cycle (Ticks before I/O, 0 if none): [0 or positive integer]
```

If I/O Cycle > 0:
```
Enter I/O Sleep Duration: [positive integer]
```

If scheduler type is PRIORITY:
```
Enter Priority (Higher number = Higher Priority) for Task_X: [integer]
```
=======
- Execution order: Task_3 → Task_2 → Task_1
- Stack-based ordering


## Input Format

For each task, the simulator prompts:

1. **Total number of tasks:** Integer value
2. **For each task:**
   - **Burst Time:** How long the task needs to execute (in ticks)
   - **I/O Burst Cycle:** How many ticks between I/O operations (0 = no I/O)
   - **I/O Sleep Duration:** How long to sleep during I/O (if I/O cycle > 0)
   - **Priority:** (Priority scheduling only) Higher numbers = higher priority

## Expected Output Format

Each simulation produces output like:

```
Simulation Starting with RR Scheduler...

Please enter the total number of tasks: 2
  Enter Burst Time for Task_1: 6
<<<<<<< HEAD
  Enter I/O Burst Cycle (Ticks before I/O, 0 if none): 0
  Enter Burst Time for Task_2: 4
  Enter I/O Burst Cycle (Ticks before I/O, 0 if none): 0
=======
  Enter I/O Burst Cycle (0 if none): 0
  Enter Burst Time for Task_2: 4
  Enter I/O Burst Cycle (0 if none): 0
>>>>>>> f75423b (feat: Implement aging mechanism and enhance task statistics in scheduler)
All tasks loaded. Starting simulation loop.

Tick 0:
  [System] Context Switch Triggered (Cost: 1 ticks). preparing Task_1
  [Aging] Task_1 priority increased to 1

Tick 1:
  -> Switch Complete: Task_1 started running.
  [CPU] Executing Task_1 (Remaining: 6)

Tick 2:
  -> RR Time Quantum expired for Task_1.
  [CPU] Executing Task_2 (Remaining: 4)

Tick 3:
  *** Task_2 Finished! ***
      Stats -> Total Waiting Time: 0 ticks

Tick 3:
  [CPU] Executing Task_1 (Remaining: 4)

...

Simulation Finished. Total Ticks: 11
```

## Test Cases Summary

<<<<<<< HEAD
| Scheduler | Tasks | Burst Times | I/O | Expected Order | Notes |
|-----------|-------|-------------|-----|-----------------|-------|
| FCFS | 3 | 6,10,4 | None | 1→2→3 | Non-preemptive |
| RR | 3 | 6,10,4 | None | Mixed | 2-tick quantum |
| SJF | 3 | 6,10,4 | None | 3→1→2 | Shortest first |
| PRIORITY | 3 | 6,10,4 | None | By priority | User-defined |
| LIFO | 3 | 6,10,4 | None | 3→2→1 | Stack-based |
| RR+I/O | 2 | 10,8 | Yes | Interleaved | With sleep/wake |
=======
| Scheduler | Tasks | Burst Times | Expected Order | Notes |
|-----------|-------|-------------|-----------------|-------|
| FCFS | 3 | 6,10,4 | 1→2→3 | Non-preemptive, fifo |
| RR | 3 | 6,10,4 | Interleaved | 2-tick quantum |
| SJF | 3 | 6,10,4 | 3→1→2 | Shortest first |
| PRIORITY | 3 | 6,10,4 + Priorities | User-defined | Aging enabled |
| LIFO | 3 | 6,10,4 | 3→2→1 | Stack-based |
>>>>>>> f75423b (feat: Implement aging mechanism and enhance task statistics in scheduler)

## Key Features

### Context Switch Overhead
- Context switch cost: 1 tick
- Simulated during task transitions
- Shows realistic CPU overhead

### Waiting Time Tracking
- Tracks total waiting time for each task
- Displayed when task completes
- Useful for analyzing scheduler efficiency

### I/O Simulation
- Tasks can perform I/O operations
- I/O triggers sleep syscall
- Task moved to SleepList and woken at specified time

### Aging Mechanism
- Priority scheduling includes aging
- Priority increases by 1 every 10 ticks
- Prevents starvation of low-priority tasks

## Verification Checklist

- [ ] Project compiles without errors or warnings
- [ ] Executable `sim` is created successfully
- [ ] All 5 scheduler types run without crashing
- [ ] Output formats correctly with tick-by-tick execution
- [ ] Tasks complete and final count matches input
<<<<<<< HEAD
- [ ] I/O operations trigger sleep and alarm events correctly
=======
- [ ] Context switch overhead is applied correctly
>>>>>>> f75423b (feat: Implement aging mechanism and enhance task statistics in scheduler)
- [ ] No memory leaks (test with `valgrind ./sim FCFS`)
- [ ] Time quantum works correctly for RR (2 ticks)

## Debugging

### Common Issues

**Issue: "No such file or directory"**
```bash
# Ensure you're in the correct directory
pwd  # Should end with /c_project
```

**Issue: Compilation errors**
```bash
# Check GCC version
gcc --version  # Should be 7.0+

# Clean and rebuild
make clean && make
```

<<<<<<< HEAD
**Issue: Infinite loop**
- Check if `GLOBAL_TIME` exceeds 1000 ticks (safety limit)
- Verify task burst times are positive integers
- Ensure total tasks count matches input count
- Check that `remaining_time` decrements correctly

**Issue: I/O not triggering**
- Verify `io_burst_cycle` is greater than 0
- Ensure `current_cycle_count` is properly incremented
- Check that task's `remaining_time > 0` when I/O should trigger
=======
**Issue: Infinite loop or exceeding 1000 ticks**
- Check if task burst times are positive integers
- Verify total tasks count matches input count
- Ensure I/O sleep durations are reasonable


### Enabling Verbose Output
The simulator already outputs detailed tick-by-tick information. For additional debugging, you can modify main.c to add more printf statements.

## Performance Testing

### Standard Test
```bash
./sim RR
3
6
0
10
0
4
0
```

Expected: Completes in ~15-20 ticks

### Stress Test
```bash
./sim RR
<<<<<<< HEAD
10        # 10 tasks
5 5 5 5 5 5 5 5 5 5  # 10 tasks with burst time 5 and no I/O
=======
10
5
0
5
0
5
0
5
0
5
0
5
0
5
0
5
0
5
0
5
0
>>>>>>> f75423b (feat: Implement aging mechanism and enhance task statistics in scheduler)
```

Expected: Completes in ~50-55 ticks without issues

### I/O Test
```bash
./sim RR
2
10
2
5
8
8
3
2
```

Expected: Task goes to sleep, wakes up at correct tick, completes successfully

## Project Architecture

### Core Components

- **task.h:** Task Control Block (PCB) definition with scheduling parameters
- **queue.h / queue.c:** Generic queue implementation with sorted insertion
- **scheduler.h / scheduler.c:** Scheduling algorithm dispatcher
- **system.h / system.c:** System calls and global state management
- **main.c:** Simulation engine and main loop

### Data Flow

1. User inputs task specifications
2. Tasks created and added to ReadyQueue
3. Scheduler selects next task based on algorithm
4. Context switch overhead applied
5. Task executes for one tick
6. Task state checked: completion, I/O, or time quantum expiration
7. Loop repeats until all tasks complete

### Complex I/O Test
```bash
./sim RR
3
15
3
5
10
2
4
8
0
```

Expected: Tasks interleave with I/O operations, completing within reasonable time

## Cleanup

To remove build artifacts:
```bash
make clean
```

To remove everything including executable:
```bash
make clean
rm -f sim
```

<<<<<<< HEAD
## Key Components

### task.h
Defines the Task Control Block (PCB) structure with:
- Task identification and timing information
- Scheduling parameters (burst_time, priority, quantum)
- I/O operation fields (io_burst_cycle, io_sleep_duration, current_cycle_count)
- Task state enumeration

### queue.h & queue.c
Implements queue operations:
- Basic enqueue/dequeue operations
- Sorted insertion with multiple comparison modes (wake time, remaining time, priority)

### scheduler.h & scheduler.c
Scheduler implementation supporting:
- FCFS, RR, SJF, PRIORITY, and LIFO algorithms
- Dynamic task placement based on current scheduler type

### system.h & system.c
System-level operations:
- Global time management
- Sleep system call handling
- Alarm checking for task wake-ups

### main.c
Main simulation loop:
- User input handling
- Task creation with I/O parameters
- Context switching and CPU execution simulation
- I/O blocking and task completion handling

## Limits & Configuration

- **Maximum simulation time:** 1000 ticks (safety limit)
- **Round Robin time quantum:** 2 ticks (configurable in system.h)
- **Priority:** Higher numbers = higher priority
- **Task name length:** Limited to 20 characters
- **Maximum tasks:** Limited only by available memory

## References

- Task Control Block Definition: task.h
- Queue Implementation: queue.h, queue.c
- Scheduler Logic: scheduler.h, scheduler.c
- System Calls: system.h, system.c

## Support & Notes

- The simulator uses a tick-based system for time management
- Tasks are removed from memory once they terminate
- I/O operations use a sleep queue sorted by wake-up time for efficiency
- Context switches are printed to show scheduler decisions
=======
## Configuration Parameters

Located in various header files:

| Parameter | Location | Default | Purpose |
|-----------|----------|---------|---------|
| `CONTEXT_SWITCH_COST` | main.c | 1 tick | Context switch overhead |
| `TIME_QUANTUM` | system.h | 2 ticks | RR time slice |
| `GLOBAL_TIME` limit | main.c | 1000 ticks | Simulation timeout |

## Support & Notes

- Maximum simulation time: 1000 ticks (safety limit to prevent infinite loops)
- Round Robin time quantum: 2 ticks (configurable via system.h)
- Priority: Higher numbers = higher priority
- Task names limited to 20 characters
- Maximum number of tasks: Limited by available memory
- I/O operations trigger sleep syscalls for realistic simulation
>>>>>>> f75423b (feat: Implement aging mechanism and enhance task statistics in scheduler)
