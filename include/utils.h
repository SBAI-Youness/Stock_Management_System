#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <windows.h> // Even though I hate Windows (Microsoft), I have to use it for this project
#include "./openssl/sha.h"

#include "./users.h"
#include "./stock.h"

#define PROJECT_NAME "Stock Management System"

#define NORMAL_DELAY 3
#define NOT_NORMAL_DELAY 5

// Path to the lockout file
#define LOCKOUT_FILE "../data/lockout.csv"

// The header file of the lockout file
#define LOCKOUT_HEADER_FILE "Failed Attempts,Lockout Time,Lockout Start\n"

#define MAX_FAILED_ATTEMPTS 3
#define INITIAL_LOCKOUT_TIME 30 // In seconds

// Foreground text colors
#define RESET       "\033[0m"
#define RED         "\033[31m"                     // Error
#define GREEN       "\033[32m"                    // Success
#define YELLOW      "\033[1;33m"                 // Exit
#define ORANGE      "\033[38;5;208m"            // Warning
#define DARK_GREEN  "\033[38;2;0;102;51m"      // Sign up & Log in
#define BLUE        "\033[34m"                // Project name
#define CYAN        "\033[1;36m"             // Username
#define BROWN       "\033[38;2;139;69;19m"  // Display User

// Text styles
#define BOLD        "\033[1m"
#define UNDERLINE   "\033[4m"

// Function used to print the project name
extern void print_project_name();

// Function used to display the main menu and get the user's choice
extern void display_main_menu_and_get_choice(size_t *menu_choice);

// Function used to print success messages
extern void print_success_message(const char *success_message);

// Function used to print warning messages
extern void print_warning_message(const char *warning_message, ...);

// Function used to print error messages
extern void print_error_message(const char *error_message);

// Function used to log out of the session
extern void log_out();

// Function used to exit the program
extern void exit_program();

// Function used to handle invalid choices
extern void invalid_choice();

// Function used to check if the user should get locked out or not
extern bool should_user_get_locked_out(uint8_t failed_attempts);

// Function used to handle the lockout logic
extern void handle_lockout(uint8_t *failed_attempts, uint32_t *lockout_time, time_t *lockout_start);

// Function used to display the lockout time
extern void display_lockout_time(uint32_t seconds);

// Function used to load the lockout data from a file
extern void load_lockout_data(uint8_t *failed_attempts, uint32_t *lockout_time, time_t *lockout_start);

// Function used to save the lockout data to a file
extern void save_lockout_data(uint8_t failed_attempts, uint32_t lockout_time, time_t lockout_start);