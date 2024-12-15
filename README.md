# Custom Terminal in C++

This project is a custom terminal implemented in C++. It supports basic shell operations, built-in commands, and external executable execution. The terminal parses user input, executes commands, and provides useful shell-like functionality.

---

## Features

### 1. **Built-in Commands**
The terminal supports the following built-in commands:

- **`exit`**: Exit the terminal with an optional exit code.
  ```bash
  exit [code]
  ```
  Example:
  ```bash
  exit 0
  ```

- **`echo`**: Prints the given arguments to the console.
  ```bash
  echo [arguments]
  ```
  Example:
  ```bash
  echo Hello, World!
  ```

- **`type`**: Checks if a command is a shell built-in or an external executable.
  ```bash
  type [command]
  ```
  Example:
  ```bash
  type echo
  ```

- **`pwd`**: Prints the current working directory.
  ```bash
  pwd
  ```

- **`cd`**: Changes the current working directory.
  ```bash
  cd [directory]
  ```
  Example:
  ```bash
  cd /home/user
  cd ~  # Go to home directory
  ```

- **`cat`**: Displays the contents of one or more files.
  ```bash
  cat [file1] [file2] [...]
  ```
  Example:
  ```bash
  cat file.txt
  ```

### 2. **Argument Parsing**
Supports handling of quotes, escaped characters, and spaces in arguments.
- Double quotes (`"`) and single quotes (`'`) are handled properly.
- Backslashes (`\`) are interpreted as escape characters.

Example:
```bash
cat "file with spaces.txt"
```

---

## Dependencies

### Standard Libraries
- `<iostream>`: For input and output.
- `<string>`: For string operations.
- `<vector>`: For handling dynamic arrays.
- `<cstdlib>`: For `system()` calls.
- `<sstream>`: For string stream operations.
- `<fstream>`: For file operations.
- `<filesystem>`: For handling paths and files.
- `<unistd.h>`: For POSIX system calls (e.g., `fork`, `execvp`).
- `<sys/wait.h>`: For process management.
- `<sys/stat.h>`: For file metadata.

---

## Usage

### Compilation
Ensure you have a C++17 compatible compiler.

1. Save the code to a file, e.g., `main.cpp`.
2. Compile using the following command:
   ```bash
   g++ -std=c++17 -o main main.cpp
   ```

### Execution
Run the compiled binary:
```bash
./main
```

You will see a custom shell prompt (`$`) where you can enter commands.

---

## Example Commands

```bash
$ echo "Hello, World!"
Hello, World!

$ pwd
/home/user

$ cd /tmp
...

$ type echo
echo is a shell builtin

$ exit 0
```

---

## Design Details

### Parsing Input
- **`split_string`**: Splits strings based on a delimiter, used for parsing `PATH` and command arguments.
- **`parseFileNames`**: Extracts filenames from command arguments, handling quotes and escape sequences.

### Process Management
- Forks a new process using `fork()` to execute external commands.
- Uses `execvp()` for command execution.
- Parent process waits for the child process using `waitpid()`.

### File and Path Handling
- Uses `<filesystem>` for checking file existence and retrieving the current working directory.
- Searches for commands in `PATH` and predefined directories (e.g., `/usr/bin`, `/bin`).

---

## Limitations
- Error handling could be improved for edge cases (e.g., invalid input).
- Limited support for environment variables.
- Commands relying on advanced shell scripting features are not supported.

---

## Future Improvements
- Add support for piping (`|`) and redirection (`>`/`<`).
- Implement tab completion for commands and paths.
- Expand the set of built-in commands.
- Add support for background processes (`&`).

---

## License
This project is open-source and available for use under the MIT License.

---

## Author
Sai kaustubh

