#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <time.h>

#define PASSWORD "Moneeb"

/* Read one line from the keyboard into buf (at most size-1 characters). */
static void readLine(char *buf, size_t size) {
    fflush(stdout);
    if (fgets(buf, (int)size, stdin) == NULL) {
        exit(0);
    }
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';
    } else {
        int ch;                        /* line was too long: throw away the rest */
        while ((ch = getchar()) != '\n' && ch != EOF) { }
    }
}

/* Read a whole number. Returns 1 on success, 0 if the text wasn't a number. */
static int readInt(int *out) {
    char buf[64];
    readLine(buf, sizeof buf);

    char *end;
    errno = 0;
    long v = strtol(buf, &end, 10);
    if (end == buf || errno == ERANGE) return 0;
    while (isspace((unsigned char)*end)) end++;
    if (*end != '\0') return 0;
    if (v > INT_MAX || v < INT_MIN) return 0;

    *out = (int)v;
    return 1;
}

/* Remove spaces from both ends and make the text lowercase. */
static void trimLower(char *s) {
    char *start = s;
    while (isspace((unsigned char)*start)) start++;
    size_t len = strlen(start);
    while (len > 0 && isspace((unsigned char)start[len - 1])) len--;
    memmove(s, start, len);
    s[len] = '\0';
    for (size_t i = 0; i < len; i++) {
        s[i] = (char)tolower((unsigned char)s[i]);
    }
}

static void printRepeat(char c, int n) {
    for (int i = 0; i < n; i++) putchar(c);
    putchar('\n');
}

static int playGame(void) {
    printf("\n");
    printRepeat('~', 12);
    printf(" NUMBER GUESSING GAME \n");
    printRepeat('~', 12);

    printf("\nChoose difficulty:\n");
    printf("1. Easy (1-50, unlimited attempts)\n");
    printf("2. Medium (1-100, unlimited attempts)\n");
    printf("3. Hard (1-100, max 5 attempts)\n");

    int maxNum, maxAttempts;           /* maxAttempts 0 means unlimited */
    while (1) {
        int diff;
        printf("\nEnter 1, 2, or 3: ");
        if (!readInt(&diff)) {
            printf("Please enter a valid number.\n");
            continue;
        }
        if (diff == 1)      { maxNum = 50;  maxAttempts = 0; break; }
        else if (diff == 2) { maxNum = 100; maxAttempts = 0; break; }
        else if (diff == 3) { maxNum = 100; maxAttempts = 5; break; }
        else printf("Please enter 1, 2, or 3.\n");
    }

    int secret = rand() % maxNum + 1;
    int attempts = 0;
    int guessed = 0;

    printf("\nI've picked a number between 1 and %d. Can you guess it?\n", maxNum);
    if (maxAttempts) {
        printf("You have %d attempts maximum!\n", maxAttempts);
    }

    while (!guessed) {
        if (maxAttempts && attempts >= maxAttempts) {
            printf("\nGame Over! The number was %d.\n", secret);
            break;
        }

        int guess;
        printf("\nEnter your guess: ");
        if (!readInt(&guess)) {
            printf("Please enter a valid integer.\n");
            continue;
        }
        attempts++;

        if (guess < 1 || guess > maxNum) {
            printf("Please guess between 1 and %d.\n", maxNum);
            continue;
        }

        if (guess == secret) {
            guessed = 1;
            printf("\nCongratulations! You guessed it in %d attempts!\n", attempts);
        } else if (guess < secret) {
            printf("Too low! Try higher.\n");
        } else {
            printf("Too high! Try lower.\n");
        }
    }
    return guessed;
}

int main(void) {
    char input[64];
    srand((unsigned)time(NULL));

    printf("Welcome to the Guessing Game!\n");
    printRepeat('=', 28);
    printf("Enter the password to play: ");
    readLine(input, sizeof input);
    printRepeat('=', 28);

    if (strcmp(input, PASSWORD) == 0) {
        printf("\nAccess Granted! Enjoy\n");
    } else {
        printf("\nAcess Denied, Please leave immediately\n");
        return 0;
    }

    printf("\n\nWelcome to the Number Guessing Game!\n");

    while (1) {
        playGame();

        printf("\nPlay again? (y/n): ");
        readLine(input, sizeof input);
        trimLower(input);
        if (strcmp(input, "y") != 0) {
            printf("\nThanks for playing!\n");
            break;
        }
    }
    return 0;
}
