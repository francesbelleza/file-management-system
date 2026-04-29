# File Management System Code Summary

## Overview
This C program is a simple terminal-based file management system for Linux. It simulates a small file system **entirely in memory** using a fixed-size array of `File` records. The program never touches the host operating system's filesystem — no real files are created, read, or deleted on disk. All "files" live in the `fileSystem[]` array for the duration of the program and disappear when it exits.

The program uses a menu so the user can choose different file operations.

## Data Model

The simulated file system is built from one small struct and one array.

### `File` struct
Each file record holds:
- `name` — the file's name (max 99 characters)
- `content` — the file's contents as a single in-memory string buffer (max 999 characters)
- `isOpen` — `1` if the file is currently open, `0` otherwise
- `inUse` — `1` if this slot in the array holds a real file, `0` if the slot is free

### `fileSystem[]`
A fixed-size global array of `File` records (`MAX_FILES = 50`). Because it is a global, C zero-initializes every slot at program start — so every `inUse` field begins at `0` and the simulated file system starts empty. No setup function is needed.

### `openedIndex`
A single global integer that holds the array index of the currently open file, or `-1` if no file is open. The design only allows one file to be open at a time, which keeps the program simple.

## Main Program Flow
The program starts in `main()`. It then repeatedly:
- displays the menu
- reads the user's choice
- calls the matching function
- continues until the user chooses Exit

There is no folder to create and no file handles to close at exit, because everything lives in the array.

## Main Functions

### `isValidName()`
Checks whether a file name is safe to use. The only rule is:
- the name cannot be empty

Path-traversal characters like `/` and `..` aren't a concern here because the array is purely in-memory — a name is just a label, not a real path.

### `findFile()`
Searches the `fileSystem[]` array for a record whose `inUse` is `1` and whose `name` matches. Returns the slot's index, or `-1` if no match is found. This replaces the old `fileExists()` helper — instead of asking the OS whether a file is on disk, it checks our own array.

### `findFreeSlot()`
Scans the array for the first slot where `inUse == 0`. Returns its index, or `-1` if the array is full.

### `createFile()`
Creates a new empty file record.
Steps:
- asks the user for a file name
- validates the name
- checks the file does not already exist (`findFile`)
- finds a free slot (`findFreeSlot`)
- copies the name into the slot, clears `content`, sets `isOpen = 0` and `inUse = 1`

### `openFile()`
Opens an existing file by marking it as open.
Steps:
- looks up the file with `findFile`
- refuses if no file is open and another file is already open
- sets `fileSystem[idx].isOpen = 1` and stores `openedIndex = idx`

This allows the program to track which file is currently open. Only one file can be open at a time. If another file is already open, the program asks the user to close it first.

### `closeFile()`
Closes the currently open file by clearing `isOpen` on the open record and resetting `openedIndex` back to `-1`.

### `searchFile()`
Searches for a file by name using `findFile`. If found, it prints whether the file is currently open or closed.

### `displayAll()`
Walks the array from `0` to `MAX_FILES - 1` and prints every record where `inUse == 1`, along with its open/closed status. If no records are in use, it prints `(no files)`.

### `deleteFile()`
"Deletes" a file by freeing its slot in the array. The program does not allow deletion if the file is currently open.
Steps:
- looks up the file with `findFile`
- refuses if it is open
- sets `inUse = 0` and clears `name` and `content` so the slot can be reused

### `renameFile()`
Renames a file by overwriting its `name` field. It checks that:
- the old file exists
- the file is not currently open
- the new name is valid
- the new name is not already taken

### `viewContents()`
Displays the contents of the **currently open file**. If no file is open, the program prints an error message asking the user to open one first.

Steps:
- checks that a file is open (`openedIndex != -1`)
- prints the file's `content` buffer in one shot
- if `content` is an empty string, prints `(file is empty)` instead

There is no `rewind()` here because there is no file pointer — `content` is just a `char` array we read from the start.

### `writeToFile()`
Appends a line of text to the **currently open file**. If no file is open, the program prints an error message asking the user to open one first.

Steps:
- checks that a file is open
- reads a line of text from the user with `fgets()`
- checks that there is enough space left in the `content` buffer (`CONTENT_LEN - 1` characters total)
- appends the text using `strcat()`

There is no `fflush()` here because nothing is being written to disk — the change is just an in-memory string concatenation.

### `showMenu()`
Prints the list of menu options for the user.

## Open File Tracking
The program uses one global variable to track which file is open:
- `int openedIndex` — the array index of the open file, or `-1` if none

Both `viewContents()` and `writeToFile()` use this index to look up the same `File` struct in `fileSystem[]`:
- View reads from `fileSystem[openedIndex].content`
- Write appends to `fileSystem[openedIndex].content`

This is why Open File "matters" — without an open file, View and Write cannot run.

## Limits

Because the file system is array-based, it has fixed capacity:
- at most 50 files at once (`MAX_FILES`)
- each file holds at most 999 characters of content (`CONTENT_LEN - 1`)

Hitting either limit produces a clear error message rather than a crash.
