# File Management System

## Overview
This project is a simple terminal-based file management system written in C for Linux fo CS 149. It works with real files inside a local `files/` folder and supports basic file operations through a menu (create, open, close, search, display, delete, rename, view, and write files).

## Main Functionalities

1. **Create File**
   - Creates a new empty file inside the `files/` folder.
   - Prevents duplicate file names.

2. **Open File**
   - Opens a real file in `a+` mode.
   - This allows the same open file to be used for both viewing and writing.
   - Only one file can be open at a time.

3. **Close File**
   - Closes the currently open file.
   - After closing, the user must open a file again before viewing or writing.

4. **Search File**
   - Checks whether a file exists in the `files/` folder.
   - Also shows whether the file is currently open or closed.

5. **Display All Files**
   - Lists all files stored in the `files/` folder.

6. **Delete File**
   - Deletes a file from the folder.
   - Prevents deletion if the file is currently open.

7. **Rename File**
   - Renames an existing file.
   - Prevents duplicate names.

8. **View File Contents**
   - Displays the contents of the currently open file.
   - Uses `rewind()` so reading starts from the beginning.

9. **Write to File**
   - Appends user-entered text to the currently open file.
   - Uses `fflush()` so updates are written to disk immediately.



## How to Run

1. **Clone the repository**
   ```bash
   git clone <copyTheURL>
   cd <repoName>
   ```

2. **Compile the program**
   ```bash
   gcc -o fms file-management-system.c
   ```

3. **Run the program**
   ```bash
   ./fms
   ```

4. **Use the menu**
   - Create a file
   - Open a file
   - Write to the open file
   - View the open file
   - Close the file
   - Exit when finished

## Notes
- program automatically creates the `files/` folder if it does not already exist
- only one file can be open at a time
- files must be open to be able to read, write or view
- file must be closed to rename it
- all file operations are restricted to the `files/` folder for safety.
- file names cannot contain spaces if the program uses