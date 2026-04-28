/*
 * File Management System (real-file version)
 *
 * This program manages real files inside a "files/" folder next to the
 * program. The user can create, open, close, search, list, delete,
 * rename, view, and write to files using a terminal menu.
 *
 * For safety, all operations are restricted to the "files/" folder.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>      // opendir, readdir
#include <sys/stat.h>    // mkdir
#include <unistd.h>      // access

#define NAME_LEN 100
#define PATH_LEN 200
#define TEXT_LEN 500
#define FOLDER "files"

// Track the currently open file (only one at a time)
FILE *openedFile = NULL;
char openedName[NAME_LEN] = "";

// Make sure the file name is safe (no slashes, no "..", not empty)
int isValidName(char *name) {
    if (strlen(name) == 0) return 0;
    if (strchr(name, '/') != NULL) return 0;
    if (strstr(name, "..") != NULL) return 0;
    return 1;
}

// Build "files/<name>" into path
void buildPath(char *path, char *name) {
    snprintf(path, PATH_LEN, "%s/%s", FOLDER, name);
}

// Return 1 if the file exists in the files/ folder, else 0
int fileExists(char *name) {
    char path[PATH_LEN];
    buildPath(path, name);
    return access(path, F_OK) == 0;
}

// 1. Create a new empty file
void createFile() {
    char name[NAME_LEN];
    char path[PATH_LEN];

    printf("Enter file name: ");
    scanf("%99s", name);

    if (!isValidName(name)) {
        printf("Invalid file name.\n");
        return;
    }
    if (fileExists(name)) {
        printf("File '%s' already exists.\n", name);
        return;
    }

    buildPath(path, name);
    FILE *fp = fopen(path, "w");   // "w" creates a new empty file
    if (fp == NULL) {
        printf("Error: could not create '%s'.\n", name);
        return;
    }
    fclose(fp);
    printf("File '%s' created in %s/.\n", name, FOLDER);
}

// 2. Open a file (track it so we can close it later)
void openFile() {
    char name[NAME_LEN];
    char path[PATH_LEN];

    printf("Enter file name to open: ");
    scanf("%99s", name);

    if (!isValidName(name)) {
        printf("Invalid file name.\n");
        return;
    }
    if (!fileExists(name)) {
        printf("File '%s' does not exist.\n", name);
        return;
    }

    // Don't allow opening when something is already open
    if (openedFile != NULL) {
        if (strcmp(openedName, name) == 0) {
            printf("File '%s' is already open.\n", name);
        } else {
            printf("File '%s' is already open. Close it first.\n", openedName);
        }
        return;
    }

    buildPath(path, name);
    openedFile = fopen(path, "r");
    if (openedFile == NULL) {
        printf("Error: could not open '%s'.\n", name);
        return;
    }
    strcpy(openedName, name);
    printf("File '%s' is now open.\n", name);
}

// 3. Close the currently open file
void closeFile() {
    if (openedFile == NULL) {
        printf("No file is currently open.\n");
        return;
    }
    fclose(openedFile);
    printf("File '%s' is now closed.\n", openedName);
    openedFile = NULL;
    openedName[0] = '\0';
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

    if (fileExists(name)) {
        char *status = "closed";
        if (openedFile != NULL && strcmp(openedName, name) == 0) {
            status = "open";
        }
        printf("File '%s' exists. Status: %s\n", name, status);
    } else {
        printf("File '%s' does not exist.\n", name);
    }
}

// 5. Display every file in the files/ folder
void displayAll() {
    DIR *dir = opendir(FOLDER);
    if (dir == NULL) {
        printf("Error: could not read '%s/'.\n", FOLDER);
        return;
    }

    struct dirent *entry;
    int count = 0;

    printf("\n--- Files in '%s/' ---\n", FOLDER);
    while ((entry = readdir(dir)) != NULL) {
        // skip "." and ".." and hidden files
        if (entry->d_name[0] == '.') continue;

        char *status = "closed";
        if (openedFile != NULL && strcmp(openedName, entry->d_name) == 0) {
            status = "open";
        }
        printf("- %s (%s)\n", entry->d_name, status);
        count++;
    }
    if (count == 0) {
        printf("(no files)\n");
    }
    closedir(dir);
}

// 6. Delete a file from the folder
void deleteFile() {
    char name[NAME_LEN];
    char path[PATH_LEN];

    printf("Enter file name to delete: ");
    scanf("%99s", name);

    if (!isValidName(name)) {
        printf("Invalid file name.\n");
        return;
    }
    if (!fileExists(name)) {
        printf("File '%s' does not exist.\n", name);
        return;
    }
    if (openedFile != NULL && strcmp(openedName, name) == 0) {
        printf("File '%s' is open. Please close it first.\n", name);
        return;
    }

    buildPath(path, name);
    if (remove(path) == 0) {
        printf("File '%s' deleted.\n", name);
    } else {
        printf("Error: could not delete '%s'.\n", name);
    }
}

// 7. Rename an existing file
void renameFile() {
    char oldName[NAME_LEN], newName[NAME_LEN];
    char oldPath[PATH_LEN], newPath[PATH_LEN];

    printf("Enter file name to rename: ");
    scanf("%99s", oldName);

    if (!isValidName(oldName) || !fileExists(oldName)) {
        printf("File '%s' does not exist.\n", oldName);
        return;
    }
    if (openedFile != NULL && strcmp(openedName, oldName) == 0) {
        printf("File '%s' is open. Please close it first.\n", oldName);
        return;
    }

    printf("Enter new name: ");
    scanf("%99s", newName);

    if (!isValidName(newName)) {
        printf("Invalid new name.\n");
        return;
    }
    if (fileExists(newName)) {
        printf("A file named '%s' already exists.\n", newName);
        return;
    }

    buildPath(oldPath, oldName);
    buildPath(newPath, newName);
    if (rename(oldPath, newPath) == 0) {
        printf("File renamed to '%s'.\n", newName);
    } else {
        printf("Error: could not rename file.\n");
    }
}

// 8. View a file's contents
void viewContents() {
    char name[NAME_LEN];
    char path[PATH_LEN];

    printf("Enter file name to view: ");
    scanf("%99s", name);

    if (!isValidName(name) || !fileExists(name)) {
        printf("File '%s' does not exist.\n", name);
        return;
    }

    buildPath(path, name);
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        printf("Error: could not open '%s' for reading.\n", name);
        return;
    }

    printf("\n--- Contents of %s ---\n", name);
    int ch;
    int empty = 1;
    while ((ch = fgetc(fp)) != EOF) {
        putchar(ch);
        empty = 0;
    }
    if (empty) {
        printf("(file is empty)\n");
    } else {
        printf("\n--- end of file ---\n");
    }
    fclose(fp);
}

// 9. Append a line of text to a file
void writeToFile() {
    char name[NAME_LEN];
    char path[PATH_LEN];
    char text[TEXT_LEN];

    printf("Enter file name to write to: ");
    scanf("%99s", name);

    if (!isValidName(name) || !fileExists(name)) {
        printf("File '%s' does not exist.\n", name);
        return;
    }

    // Eat the leftover newline from scanf so fgets reads a real line
    while (getchar() != '\n');

    printf("Enter text to append: ");
    if (fgets(text, sizeof(text), stdin) == NULL) {
        return;
    }

    buildPath(path, name);
    FILE *fp = fopen(path, "a");   // "a" = append
    if (fp == NULL) {
        printf("Error: could not open '%s' for writing.\n", name);
        return;
    }
    fputs(text, fp);
    fclose(fp);
    printf("Text appended to '%s'.\n", name);
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

    // Make sure the files/ folder exists. If it already exists,
    // mkdir returns an error which we just ignore.
    mkdir(FOLDER, 0755);

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
                if (openedFile != NULL) {
                    fclose(openedFile);  // tidy up before exiting
                }
                printf("Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice. Please pick 1-10.\n");
        }
    }

    return 0;
}
