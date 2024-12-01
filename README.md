
# CEW CEP

## PROBLEM DEFINITION:
We have designed an integrated environmental monitoring system in C, covering a range of fundamental concepts and practical
applications. The project involves interacting with a free API that provides real-time environmental data. The system's
core functionalities include data retrieval, processing and reporting.
- Interact with a free API to retrieve real-time environmental data (e.g., temperature, humidity).
- Store raw and processed data in files.
- Create shell scripts to automate tasks such as data retrieval and processing.
- Utilize pointers and dynamic memory allocation in the C program to optimize data manipulation and enhance efficiency
- Implement real-time alerts using Linux system calls to notify relevant personnel of critical environmental readings.
- Use header files to modularize the C code and enhance code readability.

## 🛠 Languages
- C

### Libraries
- <stdio.h>
- <stdlib.h> 
- <stdbool.h>
- <string.h> 
- curl/ curl.h 
- cJSON.  

## Interface
- Ubuntu Linux


## Deployment

Steps to Prepare and Run the Project
### 1. Prerequisites
Install the required tools for a C project:
- GCC (GNU Compiler Collection) or any C compiler.
- Make (to use the Makefile).
- Bash (for running the run_monitor.sh script).
- Optional: Install Git if working with the repository.

On Linux/Ubuntu:

```bash
sudo apt update
sudo apt install build-essential git

```

On macOS (via Homebrew):

```bash
brew install gcc git
```
On Windows, you can use tools like MinGW or WSL.

### 2. Building the Project
Run the Makefile to compile the program:

```bash
make
```

This will likely generate an executable file.

### 3. Running the System
The "run_monitor.sh" script is designed to execute the compiled binary or set up additional configurations. 
Run it with:

```bash
./run_monitor.sh
```
