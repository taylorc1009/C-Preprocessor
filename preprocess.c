/*
Taylor Courtney
40398643
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int countLines(FILE*);
int countComments(FILE*);
void createOutputFile(FILE*, int, char**);

int main(int argc, char **argv) {
	FILE *file;
	//Validates the users command
	//Checks for -i following preprocess
	if (argc > 1 && strcmp(argv[1], "-i") == 0) {
		//Detects whether a file name has been entered
		if (argc > 2) {
			//Checks if the file exists, if not then displays an error and exits
			if (!(file = fopen(argv[2], "r"))) {
				printf("Error: file not found");
				return -1;
			}
		}
		//Displays an error and exits if no file was entered
		else {
			printf("Error: no file specified");
			return -1;
		}
	}
	else {
		printf("Error: command requires -i");
		return -2;
	}
	printf("The file %s contains:\n> %d lines\n> %d comments\n", argv[2], countLines(file), countComments(file));
	createOutputFile(file, argc, argv);
	fclose(file);
}

int countLines(FILE *file) {
	//Resets the pointer of the file
	fseek(file, 0, SEEK_SET);
	//'c' stores each character when read one by one from the file
	int c = 0;
	int amountOfLines = 0;
	bool empty = true;
	//Loops through the characters of the file and stores the current one in 'c'
	for (c = getc(file); c != EOF; c = getc(file)) {
		//Checks whether the current line is empty
		if (empty == true && (c != '\n' && c != ' ')) empty = false;
		if (c == '\n' && empty == false) {
			amountOfLines++;
			//Resets 'empty' for the next iteration
			empty = true;
		}
	}
	return amountOfLines;
}

int countComments(FILE *file) {
	fseek(file, 0, SEEK_SET);
	char line[200];
	int amountOfComments = 0;
	//Loops through the lines of the file and stores the current one in 'line'
	while (fgets(line, 200, file) != NULL) {
		//Loops for the length (amount of characters) of the current line
		for (int i = 0; i < strlen(line); i++) {
			//Detects whether the current and next characters = "//"
			if (line[i] == '/' && line[i + 1] == '/') {
				amountOfComments++;
			}
		}
	}
	return amountOfComments;
}

void createOutputFile(FILE *file, int argc, char **argv) {
	fseek(file, 0, SEEK_SET);
	//Stores 'data', an array of chars, on the heap with a size of 200 bytes
	char *data = (char*)malloc(200);
	FILE *header;
	char name[20];
	//Separates the file name the user entered using the '.' for the extension
	char *token = strtok(argv[2], ".");
	//Places the concatenated filename with a '.o' extension into name
	strcpy(name, strcat(token, ".o"));
	//Opens the output file with the generated name
	FILE *output = fopen(name, "w");
	/* -- Used for part 5

	char constNames[10][15];
	char constValues[10][10];
	int constsStored = 0;*/

	while (fgets(data, 200, file) != NULL) {
		for (int i = 0; i < strlen(data); i++) {
			//'c' is used to loop through the characters in a line which are to be removed
			int c = i;
			//Used to stop lines from being printed more than once
			bool linePrinted = false;
			if (data[i] == '/' && data[i + 1] == '/') {
				//Checks whether the user entered "-c" in their command
				if (argc > 3 && strcmp(argv[3], "-c") == 0) {
					//Removes all characters from "//" to the new line
					while (data[c] != '\n') {
						data[c] = '\0';
						c++;
					}
					//Removes the new line indicator if it was left
					if (data[c] == '\n') data[c] = '\0';
					continue;
				}
			}
			//Detects whether the first 3 characters of the current line = "#in" (for "#include")
			else if (data[i] == '#' && data[i + 1] == 'i' && data[i + 2] == 'n') {
				//Removes the "#include" and the following space
				for (c = i; c <= i + 9; c++)
					data[c] = '\0';
				//'ch' is used to get the amount of charactes in the header file name
				int ch = 0;
				//'toInclude' stores the name of the header file to be included
				char *toInclude = "\0";
				//Loops through to the end of the header file name to be included
				while (data[c] != '"') {
					toInclude[ch] = data[c];
					data[c] = '\0';
					ch++;
					c++;
				}
				//Removes the quotation marks following the header file name
				if (data[c] == '"') data[c] = '\0';
				//Checks for the header file
				if (header = fopen(toInclude, "r")) {
					//Stores the lines of the header file
					char line[250];
					while (fgets(line, 250, header) != NULL) {
						for (int i = 0; i < strlen(line); i++) {
							//Prints the all characters of the header file to the '.o' file
							fprintf(output, "%c", line[i]);
							linePrinted = true;
						}
					}
					//Takes a new line after the line has been outputted
					fprintf(output, "\n");
				}
			}
			/* -- Part 5

			else if (data[i] == '#' && data[i + 1] == 'd') {
				for (c = i; c <= i + 7; c++) {
					//printf("%c", data[c]);
					data[c] = '\0';
				} int ch = 0;
				while (data[c] != ' ') {
					constNames[constsStored][ch] = data[c];
					data[c] = '\0';
					//printf("%c", constName[ch]);
					ch++;
					c++;
				} ch = 0;
				while (data[c] != '\n') {
					constValues[constsStored][ch] = data[c];
					data[c] = '\0';
					//printf("%c", value[ch]);
					ch++;
					c++;
				}
				if (data[c] == '\n') data[c] = '\0';
				constsStored++;
			}
			for (int ch = 0; ch <= constsStored; ch++) {
				if (data[i] == constNames[ch][0]) {
					//for (int k = 0; k <= strlen(data); k++)
						//printf("%c", data[k]);
					int ch2 = i + 1;
					int ch3 = 1;
					bool isConst = false;
					while (data[ch2] != ' ' && data[ch2] == constNames[ch][ch3]) {
						if (data[ch2] == constNames[ch][ch3] && isConst == false) isConst = true;
						//printf("%c", data[ch2]);
						//printf("%d", i);
						ch2++;
						ch3++;
					}
					//for (int k = 0; k <= strlen(data); k++)
						//printf("%c", data[k]);
					if (isConst || data[i + 1] == ' ') {
						ch3 = i;
						int ch4 = 0;
						char temp[200];
						while (data[ch3] != '\n' && data[ch3] != ' ' && data[ch3] != ';') {
							temp[ch4] = data[ch3];
							printf("%c", data[ch3]);
							ch3++;
							ch4++;
						}
						char line[200];
						ch4 = 0;
						//printf("%d", strlen(data));
						for (ch3 = i; ch3 <= strlen(data); ch3++) {
							line[ch4] = data[ch3];
							//printf("%c", data[ch3]);
							ch4++;
						}
						for (ch3 = 0; ch3 <= strlen(constValues[ch]); ch3++) {
							line[ch4] = constValues[ch][ch3];
							//printf("%c", constValues[ch][ch3]);
							ch4++;
						}
						for (ch3 = 0; ch3 <= strlen(temp); ch3++) {
							line[ch4] = temp[ch3];
							//printf("%c", temp[ch3]);
							ch4++;
						}
						line[ch4 + 1] = '\n';
						//for (int k = 0; k <= 200; k++)
							//printf("%c", line[k]);
						fprintf(output, "%c", line[200]);
						linePrinted = true;
					}
				}
			}
			//printf("%c", data[c]);*/

			//Outputs the current line if a modified line hasn't been printed instead
			if (!linePrinted)
				fprintf(output, "%c", data[i]);
		}
	}
	fclose(output);
	//Delets the array of chars 'data' from the heap
	free(data);
}