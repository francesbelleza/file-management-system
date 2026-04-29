/*
 * File Management System (array-based version)
 *
 * This program simulates a small file system entirely in memory using a
 * fixed-size array of File records. It does NOT touch the host operating
 * system's filesystem -- no real files are created, read, or deleted on
 * disk. All "files" live in the fileSystem[] array for the duration of
 * the program and disappear when the program exits.
 *
 * The user can create, open, close, search, list, delete, rename, view,
 * and write to these in-memory files through a terminal menu.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_FILES   50
#define NAME_LEN    100
#define CONTENT_LEN 1000
#define TEXT_LEN    500

// One record in the simulated file system.
typedef struct {
    char name[NAME_LEN];
    char content[CONTENT_LEN];
    int isOpen;   // 1 if this file is currently open, 0 otherwise
    int inUse;    // 1 if this slot holds a file, 0 if the slot is free
} File;

// The simulated file system: a fixed array of file records.
File fileSystem[MAX_FILES];

// Index of the currently open file, or -1 if no file is open.
// Only one file may be open at a time.
int openedIndex = -1;

// Reject empty names. Slashes/".." don't matter for an in-memory store,
// but we still disallow empty names so the user can't create a blank entry.
int isValidName(char *name) {
    if (strlen(name) == 0) return 0;
    return 1;
}

// Search the array for a file with this name. Returns its index, or -1.
int findFile(char *name) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (fileSystem[i].inUse && strcmp(fileSystem[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

// Find the first free slot in the array. Returns its index, or -1 if full.
int findFreeSlot() {
    for (int i = 0; i < MAX_FILES; i++) {
        if (!fileSystem[i].inUse) return i;
    }
    return -1;
}

// 1. Create a new empty file
void createFile() {
    char name[NAME_LEN];

    printf("Enter file name: ");
    scanf("%99s", name);

    if (!isValidName(name)) {
        printf("Invalid file name.\n");
        return;
    }
    if (findFile(name) != -1) {
        printf("File '%s' already exists.\n", name);
        return;
    }

    int slot = findFreeSlot();
    if (slot == -1) {
        printf("Error: file system is full (max %d files).\n", MAX_FILES);
        return;
    }

    strcpy(fileSystem[slot].name, name);
    fileSystem[slot].content[0] = '\0';
    fileSystem[slot].isOpen = 0;
    fileSystem[slot].inUse = 1;
    printf("File '%s' created.\n", name);
}

// 2. Open a file (track it so we can view/write to it later)
void openFile() {
    char name[NAME_LEN];

    printf("Enter file name to open: ");
    scanf("%99s", name);

    if (!isValidName(name)) {
        printf("Invalid file name.\n");
        return;
    }
    int idx = findFile(name);
    if (idx == -1) {
        printf("File '%s' does not exist.\n", name);
        return;
    }

    // Don't allow opening when something is already open
    if (openedIndex != -1) {
        if (openedIndex == idx) {
            printf("File '%s' is already open.\n", name);
        } else {
            printf("File '%s' is already open. Close it first.\n",
                   fileSystem[openedIndex].name);
        }
        return;
    }

    fileSystem[idx].isOpen = 1;
    openedIndex = idx;
    printf("File '%s' is now open (you can now view or write to it).\n", name);
}

// 3. Close the currently open file
void closeFile() {
    if (openedIndex == -1) {
        printf("No file is currently open.\n");
        return;
    }
    fileSystem[openedIndex].isOpen = 0;
    printf("File '%s' is now closed.\n", fileSystem[openedIndex].name);
    openedIndex = -1;
}

// 4. Search for a file by name and report status
void searchFile() {
    char name[NAME_LEN];
    printf("Enter file name to search: ");
    scanf("%99s", name);

    if (!isValidName(name)) {
        printf("Invalid file name.\n");
        return;
    }

    int idx = findFile(name);
    if (idx != -1) {
        char *status = fileSystem[idx].isOpen ? "open" : "closed";
        printf("File '%s' exists. Status: %s\n", name, status);
    } else {
        printf("File '%s' does not exist.\n", name);
    }
}

// 5. Display every file currently stored in the array
void displayAll() {
    int count = 0;

    printf("\n--- Files in the system ---\n");
    for (int i = 0; i < MAX_FILES; i++) {
        if (!fileSystem[i].inUse) continue;
        char *status = fileSystem[i].isOpen ? "open" : "closed";
        printf("- %s (%s)\n", fileSystem[i].name, status);
        count++;
    }
    if (count == 0) {
        printf("(no files)\n");
    }
}

// 6. Delete a file from the array
void deleteFile() {
    char name[NAME_LEN];

    printf("Enter file name to delete: ");
    scanf("%99s", name);

    if (!isValidName(name)) {
        printf("Invalid file name.\n");
        return;
    }
    int idx = findFile(name);
    if (idx == -1) {
        printf("File '%s' does not exist.\n", name);
        return;
    }
    if (fileSystem[idx].isOpen) {
        printf("File '%s' is open. Please close it first.\n", name);
        return;
    }

    // Free the slot. The next createFile() can reuse it.
    fileSystem[idx].inUse = 0;
    fileSystem[idx].name[0] = '\0';
    fileSystem[idx].content[0] = '\0';
    printf("File '%s' deleted.\n", name);
}

// 7. Rename an existing file
void renameFile() {
    char oldName[NAME_LEN], newName[NAME_LEN];

    printf("Enter file name to rename: ");
    scanf("%99s", oldName);

    int idx = findFile(oldName);
    if (!isValidName(oldName) || idx == -1) {
        printf("File '%s' does not exist.\n", oldName);
        return;
    }
    if (fileSystem[idx].isOpen) {
        printf("File '%s' is open. Please close it first.\n", oldName);
        return;
    }

    printf("Enter new name: ");
    scanf("%99s", newName);

    if (!isValidName(newName)) {
        printf("Invalid new name.\n");
        return;
    }
    if (findFile(newName) != -1) {
        printf("A file named '%s' already exists.\n", newName);
        return;
    }

    strcpy(fileSystem[idx].name, newName);
    printf("File renamed to '%s'.\n", newName);
}

// 8. View the contents of the currently open file
void viewContents() {
    if (openedIndex == -1) {
        printf("No file is open. Please open a file first (option 2).\n");
        return;
    }

    File *f = &fileSystem[openedIndex];
    printf("\n--- Contents of %s ---\n", f->name);
    if (f->content[0] == '\0') {
        printf("(file is empty)\n");
    } else {
        printf("%s", f->content);
        printf("\n--- end of file ---\n");
    }
}

// 9. Append a line of text to the currently open file
void writeToFile() {
    char text[TEXT_LEN];

    if (openedIndex == -1) {
        printf("No file is open. Please open a file first (option 2).\n");
        return;
    }

    // Eat the leftover newline from the menu's scanf so fgets reads cleanly
    while (getchar() != '\n');

    printf("Enter text to append: ");
    if (fgets(text, sizeof(text), stdin) == NULL) {
        return;
    }

    File *f = &fileSystem[openedIndex];
    // Make sure there is room left in the content buffer before appending.
    if (strlen(f->content) + strlen(text) >= CONTENT_LEN) {
        printf("Error: file '%s' is full (max %d characters).\n",
               f->name, CONTENT_LEN - 1);
        return;
    }
    strcat(f->content, text);
    printf("Text appended to '%s'.\n", f->name);
}

void showMenu() {
    printf("\n==== File Management System ====\n");
    printf("1. Create File\n");
    printf("2. Open File\n");
    printf("3. Close File\n");
    printf("4. Search File\n");
    printf("5. Display All Files\n");
    printf("6. Delete File\n");
    printf("7. Rename File\n");
    printf("8. View File Contents\n");
    printf("9. Write to File\n");
    printf("10. Exit\n");
    printf("Enter your choice: ");
}

int main() {
    int choice;

    // The fileSystem[] array is statically allocated, so every slot
    // already starts with inUse = 0 (zero-initialized globals in C).
    // No setup needed -- the simulated file system begins empty.

    while (1) {
        showMenu();

        // Read the menu choice. Reject non-numbers gracefully.
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        switch (choice) {
            case 1:  createFile();    break;
            case 2:  openFile();      break;
            case 3:  closeFile();     break;
            case 4:  searchFile();    break;
            case 5:  displayAll();    break;
            case 6:  deleteFile();    break;
            case 7:  renameFile();    break;
            case 8:  viewContents();  break;
            case 9:  writeToFile();   break;
            case 10:
                printf("Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice. Please pick 1-10.\n");
        }
    }

    return 0;
}
