Collecting workspace information# CPU Task Scheduling Simulator - Reproduction Document

## Overview
This document provides step-by-step instructions to reproduce and test the CPU Task Scheduling Simulator, a C-based project that implements multiple CPU scheduling algorithms.

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
- Output shows interleaved execution

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
- Optimal average waiting time

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
- Stack-based ordering

## Expected Output Format

Each simulation produces output like:

```
Simulation Starting with RR Scheduler...

Please enter the total number of tasks: 2
  Enter Burst Time for Task_1: 6
  Enter Priority (Higher number = Higher Priority) for Task_1: 0
  Enter Burst Time for Task_2: 4
  Enter Priority (Higher number = Higher Priority) for Task_2: 0
All tasks loaded. Starting simulation loop.

Tick 0:
  -> Context Switch: Task_1 started running (Remaining: 6).
  [CPU] Executing Task_1 (Remaining: 6)

Tick 1:
  -> RR Time Quantum expired for Task_1.
  [CPU] Executing Task_2 (Remaining: 4)

Tick 2:
  *** Task_2 Finished! ***

...

Simulation Finished. Total Ticks: 10
```

## Test Cases Summary

| Scheduler | Tasks | Burst Times | Expected Order | Notes |
|-----------|-------|-------------|-----------------|-------|
| FCFS | 3 | 6,10,4 | 1→2→3 | Non-preemptive |
| RR | 3 | 6,10,4 | Mixed interleaved | 2-tick quantum |
| SJF | 3 | 6,10,4 | 3→1→2 | Shortest first |
| PRIORITY | 3 | 6,10,4 + Priority | Depends on priority | User-defined |
| LIFO | 3 | 6,10,4 | 3→2→1 | Stack-based |

## Verification Checklist

- [ ] Project compiles without errors or warnings
- [ ] Executable `sim` is created
- [ ] All 5 scheduler types run without crashing
- [ ] Output formats correctly with tick-by-tick execution
- [ ] Tasks complete and final count matches input
- [ ] No memory leaks (test with `valgrind ./sim FCFS`)

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
- Check if `GLOBAL_TIME` exceeds 1000 ticks
- Verify task burst times are positive integers
- Ensure total tasks count matches input count

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
5 5 5 5 5 5 5 5 5 5  # 10 tasks with burst time 5
```

Expected: Completes in ~50 ticks without issues

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

## References

- Task Control Block Definition
- Queue Implementation
- Scheduler Logic
- System Calls

## Support & Notes

- Maximum simulation time: 1000 ticks (safety limit)
- Round Robin time quantum: 2 ticks (configurable in system.h)
- Priority: Higher numbers = higher priority
- Task names limited to 20 characters