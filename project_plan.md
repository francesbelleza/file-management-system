# File Management System Project Plan

## Project Overview
This project implements a small file management system in **C** under a **Linux environment**. The program will run in the terminal and demonstrate core file system operations through a menu-driven interface.

The goal is to create a simplified file management system that shows how files can be created, opened, closed, searched, and managed.

## necessary features
These features come directly from the project instructions and must be included:

1. **Create File**
   - Allow the user to create a new file by entering a file name.
   - Prevent duplicate file names.

2. **Open File**
   - Allow the user to open an existing file.
   - Mark the file as currently open.
   - Do not allow the same file to be opened twice.

3. **Close File**
   - Allow the user to close an existing open file.
   - Update the file status from open to closed.

4. **Search File**
   - Allow the user to search for a file by name.
   - Show whether the file exists and whether it is open or closed.

## additional features
5. **Display All Files**
   - Show the list of all files currently stored in the system.
   - Display useful details such as file name and status.

6. **Delete File**
   - Allow the user to remove a file from the system.
   - Prevent deletion of a file that does not exist.

7. **Rename File**
   - Allow the user to rename an existing file.
   - Prevent renaming to a duplicate file name.

8. **Save Data Persistently**
   - Save file records to a local data file so the system can restore them when restarted.
   - This is optional, but helpful if time allows.

## Program Style
- Language: **C**
- Environment: **Linux**
- Interface: **Terminal / text-based menu**
- Compilation: `gcc`

## Suggested Program Structure
- a data structure to represent each file
- a way to store multiple file records
- separate functions for each major operation
- a looped menu in `main()` for user interaction

### Example File Record Fields
Each file can store:
- file name
- file id or index
- status (`open` or `closed`)
- existence flag

## Suggested Menu
```text
==== File Management System ====
1. Create File
2. Open File
3. Close File
4. Search File
5. Display All Files
6. Delete File
7. Rename File
8. Exit
Enter your choice:
```

## Functional Expectations
- The program should validate user input.
- The program should show clear success/error messages.
- The program should not crash on invalid input.
- The code should be clean and easy to explain during the presentation.
