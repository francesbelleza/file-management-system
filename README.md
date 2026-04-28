# File Management System

## Overview
This project is a simple terminal-based file management system written in C for Linux fo CS 149. It works with real files inside a local `files/` folder and supports basic file operations through a menu.

## What Changed
1. **Open File now uses `a+` mode**
   - The file is opened for both reading and appending.
   - Reads can start from the beginning after using `rewind()`.
   - Writes always go to the end of the file.
   - One mode supports both view and write, which keeps the program simple.

2. **View File Contents (option 8)**
   - No longer asks for a filename.
   - Only works on the currently open file.
   - Prints an error if no file is open.
   - Calls `rewind(openedFile)` so reading starts at the beginning.

3. **Write to File (option 9)**
   - No longer asks for a filename.
   - Only works on the currently open file.
   - Prints an error if no file is open.
   - Uses `fputs()` to append text to the open file.
   - Uses `fflush(openedFile)` so changes are written to disk right away.

4. **Open File gives a clearer message**
   - After opening a file, the program tells the user that they can now view or write to it.

## Main Functions

### `isValidName()`
Checks whether a file name is safe. It rejects empty names, names with `/`, and names with `..`.

### `buildPath()`
Builds the full path in the format `files/<filename>`.

### `fileExists()`
Checks whether a file already exists in the `files/` folder.

### `createFile()`
Creates a new empty file if the name is valid and not already used.

### `openFile()`
Opens a real file in `a+` mode and stores its file handle so the program can keep using it until the user closes it.

### `closeFile()`
Closes the currently open file and clears the saved file handle.

### `searchFile()`
Searches for a file by name and reports whether it exists and whether it is currently open or closed.

### `displayAll()`
Lists all files inside the `files/` folder.

### `deleteFile()`
Deletes a file from the folder, unless it is currently open.

### `renameFile()`
Renames a file, as long as the file exists, is not open, and the new name is valid.

### `viewContents()`
Shows the contents of the currently open file.

### `writeToFile()`
Appends user-entered text to the currently open file.

### `showMenu()`
Prints the menu options.

### `main()`
Creates the `files/` folder if needed, shows the menu in a loop, and calls the correct function based on user choice.

## How to Run

1. **Clone the repository**
   ```bash
   git clone <your-repo-url>
   cd <your-repo-folder>
   ```

2. **Compile the program**
   ```bash
   gcc -Wall -o fms file-management-system.c
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

## Example Demo Flow
```text
1  -> notes.txt
2  -> notes.txt
9  -> Hello world
8
3
10
```

## Notes
- The program automatically creates the `files/` folder if it does not already exist.
- Only one file can be open at a time.
- All file operations are restricted to the `files/` folder for safety.
- File names cannot contain spaces if the program uses `scanf("%s")`.