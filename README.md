## Project Overview

This is a **CPU Task Scheduling Simulator** written in C that implements multiple scheduling algorithms commonly used in operating systems. The simulator demonstrates how different scheduling strategies handle task execution, context switching, and system calls like sleep.

## Features

- **Multiple Scheduling Algorithms:**
  - FCFS (First Come First Served)
  - RR (Round Robin)
  - SJF (Shortest Job First)
  - PRIORITY (Priority-based Scheduling)

- **System Features:**
  - Task state management (READY, RUNNING, SLEEPING, TERMINATED)
  - Sleep system call with wake-up timer
  - Hardware interrupt simulation (alarm checking)
  - Context switching
  - Time quantum support for Round Robin

## Project Structure

```
.
├── main.c              # Main simulation loop and entry point
├── system.c            # System initialization and syscall implementations
├── system.h            # System header with global variables
├── scheduler.c         # Scheduling algorithm logic
├── scheduler.h         # Scheduler interface
├── queue.c             # Queue data structure and task operations
├── queue.h             # Queue header
├── task.h              # Task control block (PCB) definition
├── Makefile            # Build configuration
└── README.md           # This file
```

## Building the Project

### Prerequisites
- GCC compiler
- Make utility

### Build Commands

```bash
# Build the project
make

# Clean build artifacts
make clean
```

### Output
The build produces an executable named `sim`.

## Usage

Run the simulator with a scheduling algorithm argument:

```bash
./sim FCFS        # First Come First Served
./sim RR          # Round Robin (default)
./sim SJF         # Shortest Job First
./sim PRIORITY    # Priority-based Scheduling
```

## Example Output

```
Simulation Starting with RR Scheduler...

Tick 0:
  -> Context Switch: Task_A started running.
  [CPU] Executing Task_A (Remaining: 6)

Tick 1:
  -> RR Time Quantum expired for Task_A.
  [CPU] Executing Task_B (Remaining: 10)
    [SYSCALL] Task Task_B calls Sleep(5). Wake up at tick 8.
  
Tick 8:
    [ALARM] Task_B woke up! Moving to ReadyQueue.

Simulation Finished.
```

## Architecture

### Task Control Block (PCB)
Defined in task.h, each task contains:
- Task identification (ID, name)
- Timing information (burst time, remaining time)
- Scheduling parameters (priority, RR quantum)
- State information (current state, wake-up time)

### Queue Management
queue.c and queue.h implement:
- Standard queue operations (enqueue, dequeue)
- Sorted insertion with multiple modes for different algorithms
- Support for priority-based and time-based ordering

### Scheduler
scheduler.c and scheduler.h manage:
- Algorithm selection via `SetSchedulerType()`
- Task queuing based on algorithm type
- Next task selection with appropriate initialization

### System Management
system.c and system.h handle:
- Global time tracking
- Sleep queue management
- Wake-up detection via `CheckAlarm()`
- System call implementations

## Test Case

The default test in main.c creates three tasks:

| Task ID | Name    | Burst Time | Priority |
|---------|---------|------------|----------|
| 1       | Task_A  | 6          | 1        |
| 2       | Task_B  | 10         | 3        |
| 3       | Task_C  | 4          | 2        |

Task_B demonstrates the sleep syscall at 8 ticks remaining, sleeping for 5 ticks.

## Key Components

- **main.c**: Simulation loop with task creation and scheduling decisions
- **system.c**: System initialization, sleep handling, and alarm checking
- **scheduler.c**: Queue management and algorithm-specific task ordering
- **queue.c**: Core data structure with polymorphic insertion logic

## Simulation Flow

1. Initialize system and ready queue
2. Create test tasks and add to ready queue
3. **Main Loop (per tick):**
   - Check alarms (wake sleeping tasks)
   - Select next task if CPU idle
   - Execute current task or go idle
   - Increment global time
4. Repeat until all tasks complete (max 100 ticks safety limit)

## Compilation Flags

- `-Wall`: Enable all compiler warnings
- `-g`: Include debugging symbols

## Notes

- Maximum simulation time: 100 ticks (safety limit)
- Round Robin time quantum: 2 ticks (configurable via `TIME_QUANTUM`)
- Priority values: Higher number = higher priority
- Task names limited to 20 characters
