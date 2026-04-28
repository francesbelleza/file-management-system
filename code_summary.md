# File Management System Code Summary

## Overview
This C program is a simple terminal-based file management system for Linux. It works with real files stored inside a local `files/` folder. The program uses a menu so the user can choose different file operations.

## Main Program Flow
The program starts in `main()`. It first makes sure the `files/` folder exists by calling `mkdir()`. Then it repeatedly:
- displays the menu
- reads the user's choice
- calls the matching function
- continues until the user chooses Exit

If a file is still open when the user exits, the program closes it first.

## Main Functions

### `isValidName()`
Checks whether a file name is safe to use. It rejects:
- empty names
- names containing `/`
- names containing `..`

This keeps all operations inside the `files/` folder.

### `buildPath()`
Builds the full file path in this format:

`files/<filename>`

This is used by other functions whenever they need to access a real file.

### `fileExists()`
Checks whether a file already exists in the `files/` folder using `access()`.

### `createFile()`
Creates a new empty file.
Steps:
- asks the user for a file name
- validates the name
- checks if the file already exists
- creates the file using `fopen(..., "w")`

### `openFile()`
Opens a real file for reading using `fopen(..., "r")`.
The program stores:
- the open file pointer in `openedFile`
- the file name in `openedName`

This allows the program to keep track of one currently open file.

### `closeFile()`
Closes the currently open file using `fclose()`. After closing it, the program clears the saved file pointer and name.

### `searchFile()`
Searches for a file by name. If the file exists, it prints whether the file is currently open or closed.

### `displayAll()`
Lists all files inside the `files/` folder using `opendir()` and `readdir()`. It skips hidden entries like `.` and `..`.

### `deleteFile()`
Deletes a file using `remove()`. The program does not allow deletion if the file is currently open.

### `renameFile()`
Renames a file using `rename()`. It checks that:
- the old file exists
- the new name is valid
- the new name is not already taken
- the file is not currently open

### `viewContents()`
Opens a file in read mode, reads it character by character using `fgetc()`, and prints the contents to the terminal.

### `writeToFile()`
Opens a file in append mode using `fopen(..., "a")`, reads text from the user with `fgets()`, and appends that text to the file.

### `showMenu()`
Prints the list of menu options for the user.

## Open File Tracking
The program uses two global variables:
- `FILE *openedFile`
- `char openedName[]`

These keep track of which file is currently open. The design only allows one file to be open at a time, which keeps the program simple.

## Summary
Overall, the code is organized around small functions, where each function handles one file operation. The program combines real Linux file handling with a simple menu interface, making it easy to test and explain.
