#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300

struct Dir;
struct File;

typedef struct Dir{
	char *name;
	struct Dir* parent;
	struct File* head_children_files;
	struct Dir* head_children_dirs;
	struct Dir* next;
}Dir;

typedef struct File {
	char *name;
	struct Dir* parent;
	struct File* next;
}File;

void touch(Dir* parent, char* name){ // New file
	File *newFile = (File *)malloc(sizeof(File));

	newFile->name = (char *)malloc(strlen(name) + 1);
	newFile->parent = parent;
	newFile->next = NULL;
	
	strcpy(newFile->name, name);

	// Check if a directory of similar name to the new file exists.
	// If there is, an error is printed, the memory for the file is freed and the function ends.
	if(parent->head_children_dirs != NULL){
		Dir *currentDir = parent->head_children_dirs;

		while(currentDir->next != NULL){
			if(strcmp(currentDir->name, newFile->name) == 0){
				printf("File already exists.\n");
				free(newFile->name);
				free(newFile);
				return;
			}

			currentDir = currentDir->next;
		}

		// Check again for the final directory in the list.
		if(strcmp(currentDir->name, newFile->name) == 0){
			printf("File already exists.\n");
			free(newFile->name);
			free(newFile);
			return;
		}
	}

	// Check if there is no other file in the current directory.
	// If there isn't, the new file is added to the list.
	if(parent->head_children_files == NULL){
		parent->head_children_files = newFile;
	}
	else{
		// Check if a file of similar name to the new one exists.
		// If there is, an error is printed, the memory for the file is freed and the function ends.
		File *currentFile = parent->head_children_files;

		while(currentFile->next != NULL){
			if(strcmp(currentFile->name, newFile->name) == 0){
				printf("File already exists.\n");
				free(newFile->name);
				free(newFile);
				return;
			}

			currentFile = currentFile->next;
		}

		// Check again for the final file in the list.
		if(strcmp(currentFile->name, newFile->name) == 0){
			printf("File already exists.\n");
			free(newFile->name);
			free(newFile);
			return;
		}

		// The new file is added to the end of the list.
		currentFile->next = newFile;
	}
}

void mkdir(Dir* parent, char* name){ // New directory
	Dir *newDir = (Dir *)malloc(sizeof(Dir));

	newDir->name = (char *)malloc(strlen(name) + 1);
	newDir->parent = parent;
	newDir->head_children_dirs = NULL;
	newDir->head_children_files = NULL;
	newDir->next = NULL;

	strcpy(newDir->name, name);

	// Check if a file of similar name to the new directory exists.
	// If there is, an error is printed, the memory for the directory is freed and the function ends.
	if(parent->head_children_files != NULL){
		File *currentFile = parent->head_children_files;

		while(currentFile->next != NULL){
			if(strcmp(currentFile->name, newDir->name) == 0){
				printf("Directory already exists.\n");
				free(newDir->name);
				free(newDir);
				return;
			}

			currentFile = currentFile->next;
		}

		// Check again for the final file in the list.
		if(strcmp(currentFile->name, newDir->name) == 0){
			printf("Directory already exists.\n");
			free(newDir->name);
			free(newDir);
			return;
		}
	}

	// Check if there is no other directory in the current directory.
	// If there isn't, the new directory is added to the list.
	if(parent->head_children_dirs == NULL){
		parent->head_children_dirs = newDir;
	}
	else{
		// Check if a directory of similar name to the new one exists.
		// If there is, an error is printed, the memory for the directory is freed and the function ends.
		Dir *currentDir = parent->head_children_dirs;

		while(currentDir->next != NULL){
			if(strcmp(currentDir->name, newDir->name) == 0){
				printf("Directory already exists.\n");
				free(newDir->name);
				free(newDir);
				return;
			}

			currentDir = currentDir->next;
		}

		// Check again for the final directory in the list.
		if(strcmp(currentDir->name, newDir->name) == 0){
			printf("Directory already exists.\n");
			free(newDir->name);
			free(newDir);
			return;
		}

		// The new directory is added to the end of the list.
		currentDir->next = newDir;
	}
}

void ls(Dir* parent){ // Print directory contents
	if(parent->head_children_dirs != NULL){
		Dir *currentDir = parent->head_children_dirs;

		while(currentDir->next != NULL){
			printf("%s\n", currentDir->name);
			currentDir = currentDir->next;
		}
		printf("%s\n", currentDir->name);
	}

	if(parent->head_children_files != NULL){
		File *currentFile = parent->head_children_files;

		while(currentFile->next != NULL){
			printf("%s\n", currentFile->name);
			currentFile = currentFile->next;
		}
		printf("%s\n", currentFile->name);
	}
}

void rm(Dir* parent, char* name){ // Remove file

	// Search for a file with the given name.
	// If found, remove it from the list and free the memory allocated to it.
	if(parent->head_children_files != NULL){
		File *currentFile = parent->head_children_files;

		if(strcmp(currentFile->name, name) == 0){
			parent->head_children_files = currentFile->next;

			free(currentFile->name);
			free(currentFile);

			return;
		}

		while(currentFile->next != NULL){
			if(strcmp(currentFile->next->name, name) == 0){
				File *p = currentFile->next;
				currentFile->next = currentFile->next->next;

				free(p->name);
				free(p);

				return;
			}
			currentFile = currentFile->next;
		}
	}
	
	printf("File not found.\n");
}

void rmdir_r(Dir *parent){ // Helper function that recursively removes a directory and everything inside it

	// Free the memory allocated to every file in the list.
	if(parent->head_children_files != NULL){
		File *currentFile = parent->head_children_files;

		while(currentFile->next != NULL){
			File *p = currentFile->next;
			free(currentFile->name);
			free(currentFile);
			currentFile = p;
		}
		free(currentFile->name);
		free(currentFile);
	}
	
	// Call the function recursively for every directory inside the given one.
	if(parent->head_children_dirs != NULL){
		Dir *currentDir = parent->head_children_dirs;

		while(currentDir->next != NULL){
			Dir *p = currentDir->next;
			rmdir_r(currentDir);
			currentDir = p;
		}
		rmdir_r(currentDir);
	}
	
	// Free the memory allocated to the directory.
	free(parent->name);
	free(parent);
}

void rmdir(Dir* parent, char* name){ // Remove directory

	// Search for a directory with the given name.
	// If found, call the helper function rmdir_r on it.
	if(parent->head_children_dirs != NULL){
		Dir *currentDir = parent->head_children_dirs;

		if(strcmp(currentDir->name, name) == 0){
			Dir *p = currentDir;
			parent->head_children_dirs = currentDir->next;
			rmdir_r(p);
			return;
		}

		while(currentDir->next != NULL){
			if(strcmp(currentDir->next->name, name) == 0){
				Dir *p = currentDir->next;
				currentDir->next = currentDir->next->next;
				rmdir_r(p);
				return;
			}
			currentDir = currentDir->next;
		}
	}

	printf("Directory not found.\n");
}

void cd(Dir** target, char *name){ // Change directory stored in the target pointer

	// If ".." is given as the name parameter, change directory to the parent one.
	if(strcmp(name, "..") == 0){
		if((*target)->parent != NULL){ // If the parent directory doesn't exist, it means the current directory is the root directory.
			*target = (*target)->parent;
		}
		return;
	}

	// Search for a directory with the given name.
	// If found, update the target pointer.
	if((*target)->head_children_dirs != NULL){
		Dir *currentDir = (*target)->head_children_dirs;

		while(currentDir->next != NULL){
			if(strcmp(currentDir->name, name) == 0){
				*target = currentDir;
				return;
			}
			currentDir = currentDir->next;
		}
		if(strcmp(currentDir->name, name) == 0){
			*target = currentDir;
			return;
		}
	}

	printf("Directory not found.\n");
}

char *pwd(Dir* target){ // Print working directory
	if(target->parent != NULL){
		pwd(target->parent);
	}
	printf("/%s", target->name);
}

void tree(Dir* target, int level){ // Recursively print all directory contents in a tree-like format
	int i;

	// Print all directories and subdirectories indented based on what level they are on.
	if(target->head_children_dirs != NULL){
		Dir *currentDir = target->head_children_dirs;

		while(currentDir->next != NULL){
			for(i = 0; i < level; ++i){
				printf("    ");
			}
			printf("%s\n", currentDir->name);
			tree(currentDir, level + 1);

			currentDir = currentDir->next;
		}

		for(i = 0; i < level; ++i){
			printf("    ");
		}
		printf("%s\n", currentDir->name);
		tree(currentDir, level + 1);
	}

	// Print all files indented based on what level they are on.
	if(target->head_children_files != NULL){
		File *currentFile = target->head_children_files;

		while(currentFile->next != NULL){
			for(i = 0; i < level; ++i){
				printf("    ");
			}
			printf("%s\n", currentFile->name);
			currentFile = currentFile->next;
		}

		for(i = 0; i < level; ++i){
			printf("    ");
		}
		printf("%s\n", currentFile->name);
	}
}

void mv(Dir* parent, char *oldname, char *newname){ // Rename a file or a directory
	int foundFile = 0, foundDir = 0, foundNewName = 0;

	// Check if either of the names already exist in the directory (files).
	if(parent->head_children_files != NULL){
		File *currentFile = parent->head_children_files;

		while(currentFile->next != NULL){
			if(strcmp(currentFile->name, oldname) == 0){
				foundFile = 1;
			}
			if(strcmp(currentFile->name, newname) == 0){
				foundNewName = 1;
			}
			currentFile = currentFile->next;
		}

		if(strcmp(currentFile->name, oldname) == 0){
			foundFile = 1;
		}
		if(strcmp(currentFile->name, newname) == 0){
			foundNewName = 1;
		}
	}

	// Check if either of the names already exist in the directory (directories).
	if(parent->head_children_dirs != NULL){
		Dir *currentDir = parent->head_children_dirs;

		while(currentDir->next != NULL){
			if(strcmp(currentDir->name, oldname) == 0){
				foundDir = 1;
			}
			if(strcmp(currentDir->name, newname) == 0){
				foundNewName = 1;
			}
			currentDir = currentDir->next;
		}

		if(strcmp(currentDir->name, oldname) == 0){
			foundDir = 1;
		}
		if(strcmp(currentDir->name, newname) == 0){
			foundNewName = 1;
		}
	}

	if(foundFile == 0 && foundDir == 0){
		printf("File / Directory not found.\n");
		return;
	}
	
	if(foundNewName == 1){
		printf("File / Directory already exists.\n");
		return;
	}
	
	if(foundFile == 1){
		// If a file is found, remove it from the list, but remember it in pointerFile.
		File *currentFile = parent->head_children_files, *pointerFile;

		if(strcmp(currentFile->name, oldname) == 0){
			pointerFile = currentFile;

			if(currentFile->next != NULL){
				parent->head_children_files = currentFile->next;
			}
			else{ // If the file is the only one in the list, or at the end of the list, change it's name and stop the function.
				currentFile->name = (char *)realloc(currentFile->name, strlen(newname) + 1);
				strcpy(currentFile->name, newname);
				return;
			}
		}

		while(currentFile->next != NULL){
			if(strcmp(currentFile->next->name, oldname) == 0){
				pointerFile = currentFile->next;
				if(currentFile->next->next != NULL){
					currentFile->next = currentFile->next->next;
				}
			}
			currentFile = currentFile->next;
		}

		// Change the name of the file and add it at the end of the list.
		pointerFile->name = (char *)realloc(pointerFile->name, strlen(newname) + 1);
		strcpy(pointerFile->name, newname);

		if(currentFile != pointerFile){
			currentFile->next = pointerFile;
			pointerFile->next = NULL;
		}
	}

	if(foundDir == 1){
		// If a directory is found, remove it from the list, but remember it in pointerDir.
		Dir *currentDir = parent->head_children_dirs, *pointerDir;

		if(strcmp(currentDir->name, oldname) == 0){
			pointerDir = currentDir;

			if(currentDir->next != NULL){
				parent->head_children_dirs = currentDir->next;
			}
			else{ // If the file is the only one in the list, or at the end of the list, change it's name and stop the function.
				currentDir->name = (char *)realloc(currentDir->name, strlen(newname) + 1);
				strcpy(currentDir->name, newname);
				return;
			}
		}

		while(currentDir->next != NULL){
			if(strcmp(currentDir->next->name, oldname) == 0){
				pointerDir = currentDir->next;
				if(currentDir->next->next != NULL){
					currentDir->next = currentDir->next->next;
				}
			}
			currentDir = currentDir->next;
		}

		// Change the name of the directory and add it at the end of the list.
		pointerDir->name = (char *)realloc(pointerDir->name, strlen(newname) + 1);
		strcpy(pointerDir->name, newname);

		if(currentDir != pointerDir){
			currentDir->next = pointerDir;
			pointerDir->next = NULL;
		}
	}
}


int main(){
	
	// Initialize and allocate memory for the root directory
    Dir *rootDir = (Dir *)malloc(sizeof(Dir));
    rootDir->name = (char *)malloc(5 * sizeof(char));
	rootDir->parent = NULL;
	rootDir->head_children_dirs = NULL;
	rootDir->head_children_files = NULL;
	rootDir->next = NULL;
    strcpy(rootDir->name, "root");

	// Set current working directory to the root directory
	Dir *workingDir = rootDir;

	char *tok1, *tok2, *string;
	string = (char*)malloc(MAX_INPUT_LINE_SIZE + 1);

	// Read and execute commands until the "stop" command is met.
	// Input is being parsed into words using strtok.
	do{
		fgets(string, MAX_INPUT_LINE_SIZE, stdin);

		// Remove the newline character at the end of the string.
		string[strlen(string) - 1] = '\0';

		tok1 = strtok(string, " ");
		if(strcmp(tok1, "touch") == 0){
			tok1 = strtok(NULL, " ");
			touch(workingDir, tok1);
		}
		if(strcmp(tok1, "mkdir") == 0){
			tok1 = strtok(NULL, " ");
			mkdir(workingDir, tok1);
		}
		if(strcmp(tok1, "ls") == 0){
			ls(workingDir);
		}
		if(strcmp(tok1, "rm") == 0){
			tok1 = strtok(NULL, " ");
			rm(workingDir, tok1);
		}
		if(strcmp(tok1, "rmdir") == 0){
			tok1 = strtok(NULL, " ");
			rmdir(workingDir, tok1);
		}
		if(strcmp(tok1, "cd") == 0){
			tok1 = strtok(NULL, " ");
			cd(&workingDir, tok1);
		}
		if(strcmp(tok1, "tree") == 0){
			tree(workingDir, 0);
		}
		if(strcmp(tok1, "pwd") == 0){
			pwd(workingDir);
			printf("\n");
		}
		if(strcmp(tok1, "mv") == 0){
			tok1 = strtok(NULL, " ");
			tok2 = tok1;
			tok1 = strtok(NULL, " ");
			mv(workingDir, tok2, tok1);
		}
		if(strcmp(tok1, "stop") == 0){
			rmdir_r(rootDir);
			break;
		}
	}while(1);

	free(string);

    return 0;
}