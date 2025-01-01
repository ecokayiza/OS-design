#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HISTORY 100
#define MAX_COMMAND_LENGTH 1024

char *history[MAX_HISTORY];
int history_count = 0;

void add_to_history(const char *command) {
    if (history_count < MAX_HISTORY) {
        history[history_count++] = strdup(command);
    } else {
        free(history[0]);
        for (int i = 1; i < MAX_HISTORY; i++) {
            history[i - 1] = history[i];
        }
        history[MAX_HISTORY - 1] = strdup(command);
    }
}

void print_history() {
    for (int i = 0; i < history_count; i++) {
        printf("%d: %s\n", i + 1, history[i]);
    }
}

int main() {
    char command[MAX_COMMAND_LENGTH];

    while (1) {
        printf("DOSKEY> ");
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break;
        }

        // Remove newline character
        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "exit") == 0) {
            break;
        } else if (strcmp(command, "history") == 0) {
            print_history();
        } else {
            add_to_history(command);
            // Here you can add code to execute the command if needed
        }
    }

    // Free allocated memory
    for (int i = 0; i < history_count; i++) {
        free(history[i]);
    }

    return 0;
}