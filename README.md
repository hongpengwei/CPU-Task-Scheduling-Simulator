Collecting workspace information# CPU Task Scheduling Simulator - Reproduction Document

## Overview
This document provides step-by-step instructions to reproduce and test the CPU Task Scheduling Simulator, a C-based project that implements multiple CPU scheduling algorithms with I/O operations support.

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
- Each task waits for the previous task to complete

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
- Output shows interleaved execution with fair time distribution
- Tasks are preempted after consuming their time quantum

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
- Optimal average waiting time among non-preemptive schedulers

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
- Task_2 (priority=3) → Task_3 (priority=2) → Task_1 (priority=1)
- Respects priority ordering
- Input format for each task: burst_time, io_cycle, priority

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

## Expected Output Format

Each simulation produces output like:

```
Simulation Starting with RR Scheduler...

Please enter the total number of tasks: 2
  Enter Burst Time for Task_1: 6
  Enter I/O Burst Cycle (Ticks before I/O, 0 if none): 0
  Enter Burst Time for Task_2: 4
  Enter I/O Burst Cycle (Ticks before I/O, 0 if none): 0
All tasks loaded. Starting simulation loop.

Tick 0:
  -> Context Switch: Task_1 started running (Remaining: 6).
  [CPU] Executing Task_1 (Remaining: 6)

Tick 1:
  -> RR Time Quantum expired for Task_1.
  [CPU] Executing Task_2 (Remaining: 4)

Tick 2:
  *** Task_2 Finished! ***

Tick 3:
  [CPU] Executing Task_1 (Remaining: 4)

...

Simulation Finished. Total Ticks: 10
```

## Test Cases Summary

| Scheduler | Tasks | Burst Times | I/O | Expected Order | Notes |
|-----------|-------|-------------|-----|-----------------|-------|
| FCFS | 3 | 6,10,4 | None | 1→2→3 | Non-preemptive |
| RR | 3 | 6,10,4 | None | Mixed | 2-tick quantum |
| SJF | 3 | 6,10,4 | None | 3→1→2 | Shortest first |
| PRIORITY | 3 | 6,10,4 | None | By priority | User-defined |
| LIFO | 3 | 6,10,4 | None | 3→2→1 | Stack-based |
| RR+I/O | 2 | 10,8 | Yes | Interleaved | With sleep/wake |

## Verification Checklist

- [ ] Project compiles without errors or warnings
- [ ] Executable `sim` is created
- [ ] All 5 scheduler types run without crashing
- [ ] Output formats correctly with tick-by-tick execution
- [ ] Tasks complete and final count matches input
- [ ] I/O operations trigger sleep and alarm events correctly
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

**Issue: Infinite loop**
- Check if `GLOBAL_TIME` exceeds 1000 ticks (safety limit)
- Verify task burst times are positive integers
- Ensure total tasks count matches input count
- Check that `remaining_time` decrements correctly

**Issue: I/O not triggering**
- Verify `io_burst_cycle` is greater than 0
- Ensure `current_cycle_count` is properly incremented
- Check that task's `remaining_time > 0` when I/O should trigger

### Enable Verbose Debugging
Recompile with debug symbols:
```bash
make clean
gcc -Wall -g -DDEBUG -c main.c system.c scheduler.c queue.c
gcc -Wall -g -o sim *.o
```

## Performance Testing

### Stress Test
```bash
./sim RR
10        # 10 tasks
5 5 5 5 5 5 5 5 5 5  # 10 tasks with burst time 5 and no I/O
```

Expected: Completes in ~50 ticks without issues

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
