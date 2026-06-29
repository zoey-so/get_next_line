# get_next_line

*This project has been created as part of the 42 curriculum by smilch.*

## Description

**get_next_line** is a C function that reads a file descriptor line by line. The function returns one line at a time, handling multiple file descriptors simultaneously while using only a single static variable. This project demonstrates advanced memory management, file I/O operations, and the implementation of a robust line-reading algorithm.

### Key Features

- **Line-by-line reading**: Returns one complete line per function call
- **Multiple file descriptors**: Tracks and manages independent read states for different FDs
- **Dynamic memory management**: Allocates and frees memory as needed using only `malloc()` and `free()`
- **Configurable buffer size**: Uses a compile-time `BUFFER_SIZE` macro to control read chunk size
- **Efficient buffering**: Minimizes read operations while handling lines of any length
- **Robust error handling**: Gracefully handles invalid FDs, EOF, and read errors

### Project Goal

The primary goal is to create a reliable, memory-safe function that can read from any file descriptor (files, stdin, pipes, etc.) while maintaining state across multiple calls. This project is fundamental for understanding how file I/O and static variables work in C.

## Algorithm Explanation

### Core Design

The implementation uses a **circular linked list** managed by a single static pointer to track multiple file descriptors. Each node in the list contains:

```c
typedef struct s_rdr {
    char        *buf;    // Buffer storing read data
    int         fd;      // File descriptor
    int         len;     // Total bytes in buffer
    int         pos;     // Current read position
    int         eof;     // End-of-file flag
    struct s_rdr *next;  // Next node in circular list
} t_rdr;
```

### The Algorithm Step-by-Step

#### 1. Node Management (Single Static Variable)

The function maintains a **single static pointer** to the head of a circular linked list:

```c
static t_rdr *r_head = NULL;
```

**Why a circular linked list?**
- Each node represents a unique file descriptor
- The circular structure allows O(1) node removal
- It simplifies traversal and cleanup
- Only one static variable needed to manage all FDs

**Finding or creating a node:**
```
1. Traverse the circular list looking for the FD
2. If found → return the existing node
3. If not found → create a new node and insert at the end
   - Allocate node structure
   - Allocate buffer of size BUFFER_SIZE
   - Initialize tracking variables
   - Insert into circular list
```

#### 2. Buffer Management

Each node maintains a buffer that stores unprocessed data between calls:

```
Buffer lifecycle:
┌─────────────────────────────────┐
│  Buffer (size = BUFFER_SIZE)    │
│  ┌───────────────────────────┐  │
│  │   Unread data              │  │
│  │   ↓                        │  │
│  │   pos → [data]             │  │
│  │         [data]             │  │
│  │         [data] ← len       │  │
│  └───────────────────────────┘  │
└─────────────────────────────────┘
```

**Key operations:**
- **Reading**: `read()` fills the buffer with new data
- **Tracking**: `pos` tracks current read position, `len` tracks total bytes
- **Processing**: Extract lines from `pos` to `len` when a newline is found

#### 3. Line Extraction Logic

The algorithm follows this flow for each function call:

```
┌─────────────────────────────────────────────────────┐
│  get_next_line(fd)                                  │
│  ┌─────────────────────────────────────────────────┐ │
│  │ 1. Find/create node for FD                     │ │
│  │    ↓                                            │ │
│  │ 2. Check buffer for complete line              │ │
│  │    ┌─────────────────────────────────────┐     │ │
│  │    │ Is there data in buffer?            │     │ │
│  │    │  ├─ YES: Search for newline         │     │ │
│  │    │  │   ├─ Found: Extract line         │     │ │
│  │    │  │   └─ Not found: Continue         │     │ │
│  │    │  └─ NO: Need to read more          │     │ │
│  │    └─────────────────────────────────────┘     │ │
│  │    ↓                                            │ │
│  │ 3. Read more data from FD                     │ │
│  │    ┌─────────────────────────────────────┐     │ │
│  │    │ read() returns:                     │     │ │
│  │    │  ├─ > 0: Process new data          │     │ │
│  │    │  ├─ 0: EOF - Return last line      │     │ │
│  │    │  └─ < 0: Error - Clean up          │     │ │
│  │    └─────────────────────────────────────┘     │ │
│  │    ↓                                            │ │
│  │ 4. Return complete line or NULL               │ │
│  └─────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────┘
```

#### 4. Handling Lines Longer Than BUFFER_SIZE

When a line exceeds the buffer size, the function:

```
1. Reads first chunk into buffer
2. Extracts and stores partial line in `concat_line`
3. Continues reading chunks
4. Concatenates chunks until newline is found
5. Returns the complete line
6. Keeps remaining data in buffer for next call
```

#### 5. Memory Management Strategy

**Allocation:**
- Each FD node: 1 `malloc()` for structure
- Each FD buffer: 1 `malloc()` for buffer
- Each returned line: 1 `malloc()` for line string
- Partial line concatenation: Reallocates as needed

**Deallocation:**
- When EOF is reached: Free node and buffer
- After returning line: Caller must `free()` the line
- On error: Free node and buffer immediately

**Memory safety:**
- All allocated memory is tracked and freed
- No memory leaks when properly used
- Handles multiple FDs without interference

#### 6. Error Handling

The implementation handles:

| Situation | Action |
|-----------|--------|
| Invalid FD | Return NULL immediately |
| `read()` error | Free node, return NULL |
| `malloc()` failure | Free resources, return NULL |
| EOF with data | Return last line (without newline) |
| EOF with no data | Free node, return NULL |
| Calling after NULL | Returns NULL (caller's responsibility) |

### Why This Algorithm Works

1. **Efficiency**: Uses a single static variable to manage all FDs
2. **Robustness**: Handles lines of any length and any number of FDs
3. **Memory safety**: Tracks and frees all allocations
4. **Flexibility**: Works with files, stdin, pipes, and terminals
5. **Simplicity**: Clear separation of concerns between buffer management and line extraction

## Instructions

### Compilation

The project uses a dynamic `BUFFER_SIZE` that can be defined at compile time:

```bash
# Default BUFFER_SIZE = 2048
gcc -Wall -Wextra -Werror *.c -o gnl

# Custom BUFFER_SIZE (e.g., 42)
gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 *.c -o gnl

# Minimum BUFFER_SIZE
gcc -Wall -Wextra -Werror -D BUFFER_SIZE=1 *.c -o gnl
```

### Usage

```c
#include "get_next_line.h"

int main(void) {
    int fd = open("file.txt", O_RDONLY);
    char *line;

    // Read all lines from file
    while ((line = get_next_line(fd)) != NULL) {
        printf("%s\n", line);
        free(line);  // IMPORTANT: Free each returned line
    }

    close(fd);
    return 0;
}
```

### Example Output

**File.txt:**
```
Hello World
42 is the answer
get_next_line works!
```

**Program Output:**
```
Hello World
42 is the answer
get_next_line works!
```

### Multiple File Descriptors

```c
int fd1 = open("file1.txt", O_RDONLY);
int fd2 = open("file2.txt", O_RDONLY);

char *line1 = get_next_line(fd1);  // First line from file1
char *line2 = get_next_line(fd2);  // First line from file2
char *line3 = get_next_line(fd1);  // Second line from file1

printf("fd1: %s\n", line1);
printf("fd2: %s\n", line2);
printf("fd1: %s\n", line3);

free(line1);
free(line2);
free(line3);
close(fd1);
close(fd2);
```

## Technical Choices

### Circular Linked List Implementation

**Why a circular list over a singly linked list?**
- **O(1) node removal**: No need to traverse to find previous node
- **Simplicity**: Only one pointer to manage (head)
- **Efficiency**: Fast insert and delete operations

**Trade-off**: Slightly more complex traversal logic to avoid infinite loops.

### Buffer Management Strategy

**Why keep leftover data in buffer instead of freeing and reallocating?**
- **Performance**: Avoids unnecessary `malloc()`/`free()` calls
- **Simplicity**: Single buffer per FD with position tracking
- **Memory efficiency**: Only allocates what's needed

**Trade-off**: Buffer persists between calls, but that's required for state management.

### EOF Detection

**Why not use `bytes_read < BUFFER_SIZE` to detect EOF?**
- **Incorrect**: Can happen with stdin or pipes
- **Unreliable**: Doesn't guarantee end of data
- **Only `read() == 0`** reliably indicates EOF

**Implementation**: The function only frees the node when `read()` returns 0 AND buffer is empty.

## Resources

### Documentation

- **man read**: System call documentation
- **man malloc**: Memory allocation manual
- **man open**: File descriptor operations
- **C Programming Language** (K&R): Chapter 7 (Input/Output)

### Articles & Tutorials

- **[Static Variables in C](https://www.geeksforgeeks.org/static-variables-in-c/)**: Understanding static storage
- **[File Descriptors Explained](https://www.bottomupcs.com/file_descriptors.html)**: Unix I/O fundamentals
- **[Linked Lists in C](https://www.programiz.com/dsa/linked-list)**: Data structure basics
- **[get_next_line Guide](https://www.42.fr)**: 42 project resources


## Testing

### Running Tests

```bash
# Compile with test file
gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 main.c get_next_line.c get_next_line_utils.c -o gnl_test

# Run tests
./gnl_test

# Check for memory leaks
valgrind --leak-check=full ./gnl_test
```

### Test Cases Included

1. **Basic file reading**: Standard file with newlines
2. **Multiple FDs**: Interleaved reading from multiple files
3. **Empty file**: Returns NULL correctly
4. **No newline at EOF**: Returns last line without newline
5. **Large lines**: Lines longer than BUFFER_SIZE
6. **stdin input**: Interactive terminal input
7. **Invalid FDs**: Proper error handling
8. **Memory leaks**: Valgrind verification

## Limitations

- **Text files only**: Not designed for binary files
- **Single static variable**: Cleanup depends on reaching EOF
- **Caller responsibility**: Must free returned lines and stop calling after NULL
- **BUFFER_SIZE**: Must be defined at compile time (positive integer)

### AI Assistance
AI (DeepSeek) was utilized to assist in:

- Structuring and formatting this `README.md` document.
- Generating **repeatable, systematic test suites** for validation of the library functions based on cases listed by me.
  These test suites were written in C and used to compare the custom implementations against the original libc functions, doing my best to cover the edge cases. <br>
  **Note:** The test code is part of open repository on GitHub, but is not part of the project submission. The actual implementation of every function in the library was written manually by the project author.

## License

This project is provided as open‑source under the terms of the [MIT License](https://opensource.org/licenses/MIT). Feel free to use, modify, and distribute it in your own projects.

---
This project is part of the 42 curriculum and is provided for educational purposes.
