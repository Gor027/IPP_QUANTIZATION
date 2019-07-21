#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "commands.h"

#define BUFFER_SIZE 100
#define COMMAND_SIZE 10
#define COMMANDS_SIZE 5
#define ENERGY_SIZE 21

enum cmdEnum {
    DECLARE, REMOVE, VALID, ENERGY, EQUAL
};

// Function gets user command: returns NULL when ends with new line
char *getCommand(int *whiteChar) {
    int input;
    uint64_t size = COMMAND_SIZE;
    uint64_t currentSize = 0;
    char *command = (char *) malloc(size * sizeof(char));

    if (command == NULL)
        exit(1);

    while ((input = getchar()) != ' ') {
        if (input == '\n' || input == EOF) {
            *whiteChar = input;
            free(command);
            return NULL;
        }

        if (currentSize == size) {
            size = 2 * currentSize;
            command = (char *) realloc(command, size * sizeof(char));
        }

        command[currentSize] = (char) input;
        currentSize++;
    }

    command[currentSize] = '\0';
    *whiteChar = ' ';

    return command;
}

// Gets history to operate on: returns NULL when user input is not VALID
char *getHistory(int *whiteChar) {
    int input;
    uint64_t size = BUFFER_SIZE;
    uint64_t currentSize = 0;
    char *history = (char *) malloc(size * sizeof(char));

    if (history == NULL)
        exit(1);

    while ((input = getchar()) != ' ') {
        if (input == '\n') {
            history[currentSize] = '\0';
            *whiteChar = '\n';

            return history;
        }

        if (input == EOF) {
            *whiteChar = EOF;
            free(history);

            return NULL;
        }

        if (input < '0' || input > '3') {
            *whiteChar = ' ';
            free(history);

            return NULL;
        }

        if (currentSize == size) {
            size = 2 * currentSize;
            history = (char *) realloc(history, size * sizeof(char));
        }

        history[currentSize] = (char) input;
        currentSize++;
    }

    history[currentSize] = '\0';
    *whiteChar = ' ';

    return history;
}

// Gets energy as a string: returns NULL when user input is not a number or is bigger than 2^64 - 1
char *getNumberString(int *whiteChar) {
    int input;
    char *numberString = (char *) malloc(ENERGY_SIZE * sizeof(char));

    if (numberString == NULL)
        exit(1);

    int currentSize = 0;

    while ((input = getchar()) != '\n') {

        if (input == EOF) {
            *whiteChar = EOF;
            free(numberString);
            return NULL;
        }

        if (currentSize == 20) {
            *whiteChar = ' ';
            free(numberString);
            return NULL;
        }

        if (input < '0' || input > '9') {
            *whiteChar = ' ';
            free(numberString);
            return NULL;
        }

        numberString[currentSize] = (char) input;
        currentSize++;

        if (currentSize == 20) {
            numberString[currentSize] = '\0';
            if (strcmp(numberString, "18446744073709551615") > 0) {
                free(numberString);
                return NULL;
            }
        }
    }

    numberString[currentSize] = '\0';
    *whiteChar = '\n';

    return numberString;
}

// Converts string to number
uint64_t getEnergy(char *stringNumber) {
    if (stringNumber == NULL || stringNumber[0] == 0) {
        free(stringNumber);
        return 0;
    }

    uint64_t number = 0;

    for (size_t i = 0; i < strlen(stringNumber); i++) {
        number = number * 10 + (stringNumber[i] - '0');
    }

    free(stringNumber);

    return number;
}

void declareSW(Particle *particle, int *whiteChar) {
    char *history = getHistory(whiteChar);

    if (*whiteChar == EOF)
        return;

    if (*whiteChar == '\n' && history && strlen(history) != 0 && declareHistory(particle, history)) {
        fprintf(stdout, "OK\n");
    } else {
        fprintf(stderr, "ERROR\n");
    }

    free(history);
}

void removeSW(Particle *particle, int *whiteChar) {
    char *history = getHistory(whiteChar);

    if (*whiteChar == EOF)
        return;

    if (*whiteChar == '\n' && history && strlen(history) != 0 && removeHistory(particle, history)) {
        fprintf(stdout, "OK\n");
    } else {
        fprintf(stderr, "ERROR\n");
    }

    free(history);
}

void validSW(Particle *particle, int *whiteChar) {
    char *history = getHistory(whiteChar);

    if (*whiteChar == EOF)
        return;

    if (*whiteChar == '\n' && history && strlen(history) != 0) {
        if (isValid(particle, history))
            fprintf(stdout, "YES\n");
        else
            fprintf(stdout, "NO\n");
    } else {
        fprintf(stderr, "ERROR\n");
    }

    free(history);
}

void energySW(Particle *particle, int *whiteChar) {
    char *history = getHistory(whiteChar);

    if (*whiteChar == EOF)
        return;

    if (*whiteChar == '\n' && history && strlen(history) != 0) {
        if (!printEnergy(particle, history))
            fprintf(stderr, "ERROR\n");
    } else if (*whiteChar == ' ' && history && strlen(history) != 0) {
        uint64_t energy = getEnergy(getNumberString(whiteChar));

        if (*whiteChar == EOF)
            return;

        if (energy > 0 && assignEnergy(particle, history, energy))
            fprintf(stdout, "OK\n");
        else
            fprintf(stderr, "ERROR\n");
    } else {
        fprintf(stderr, "ERROR\n");
    }

    free(history);
}

void equalSW(Particle *particle, int *whiteChar) {
    char *historyA = getHistory(whiteChar);

    if (*whiteChar == EOF) {
        *whiteChar = EOF;
        return;
    }

    if (*whiteChar == ' ' && historyA && strlen(historyA) != 0) {
        char *historyB = getHistory(whiteChar);

        if (*whiteChar == EOF) {
            *whiteChar = EOF;
            return;
        }

        if (*whiteChar == '\n' && historyB && strlen(historyB) != 0 && equal(particle, historyA, historyB)) {
            fprintf(stdout, "OK\n");
        } else {
            fprintf(stderr, "ERROR\n");
        }

        free(historyB);
    } else {
        fprintf(stderr, "ERROR\n");
    }

    free(historyA);
}

// When command is not valid goes to default
void doOperation(Particle *particle, int index, int *whiteChar) {
    switch (index) {
        case DECLARE:
            declareSW(particle, whiteChar);
            break;
        case REMOVE:
            removeSW(particle, whiteChar);
            break;
        case VALID:
            validSW(particle, whiteChar);
            break;
        case ENERGY:
            energySW(particle, whiteChar);
            break;
        case EQUAL:
            equalSW(particle, whiteChar);
            break;
        default: {
            fprintf(stderr, "ERROR\n");
            break;
        }
    }
}

void execCommand(Particle *particle) {
    char *commands[COMMANDS_SIZE] = {"DECLARE", "REMOVE", "VALID", "ENERGY", "EQUAL"};
    int input;

    while ((input = getchar()) != EOF) {
        // When new line is the input or the line is comment ignores the line
        if (input == '#') {
            do {
                // Get whole line and ignore
                input = getchar();
                if (input == EOF) {
                    fprintf(stderr, "ERROR\n");
                    return;
                }

            } while (input != '\n');

            continue;
        }

        if (input != '\n') {
            int index = -1;
            int whiteChar;

            // Puts back input char to be read later in getCommand()
            ungetc(input, stdin);

            // Command is NULL when getCommand ends with new line
            char *command = getCommand(&whiteChar);

            if (whiteChar == EOF) {
                fprintf(stderr, "ERROR\n");
                return;
            }

            // When command is not valid ignores it
            if (command != NULL) {
                for (int i = 0; i < COMMANDS_SIZE; i++) {
                    if (strcmp(command, commands[i]) == 0) {
                        index = i;
                        break;
                    }
                }

                // If index is -1 the command is not accurate
                doOperation(particle, index, &whiteChar);

                if (whiteChar == EOF) {
                    fprintf(stderr, "ERROR\n");
                    return;
                }

            } else {
                // In case if command ends with end line
                fprintf(stderr, "ERROR\n");
            }

            // In case if ERROR is printed or line is ignored, but not reached to the end of line
            if (whiteChar != '\n') {
                do {
                    // Get whole line and ignore
                    input = getchar();

                    if (input == EOF) {
                        free(command);
                        return;
                    }

                } while (input != '\n');
            }

            free(command);
        }
    }
}