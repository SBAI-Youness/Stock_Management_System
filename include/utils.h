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

/**
 * Prints the project name with styling and formatting
*/
extern void print_project_name();

/**
 * Displays the main menu options and gets user input
 * @param menu_choice Pointer to store the user's menu selection
*/
extern void display_main_menu_and_get_choice(size_t *menu_choice);

/**
 * Prints a success message in green color
 * @param success_message The message to display
*/
extern void print_success_message(const char *success_message);

/**
 * Prints a warning message in orange color with variable arguments
 * @param warning_message The format string for the warning
 * @param ... Variable arguments for the format string
*/
extern void print_warning_message(const char *warning_message, ...);

/**
 * Prints an error message in red color
 * @param error_message The message to display
*/
extern void print_error_message(const char *error_message);

/**
 * Handles the user logout process and cleanup
*/
extern void log_out();

/**
 * Safely exits the program with cleanup
*/
extern void exit_program();

/**
 * Handles invalid menu choice selections
*/
extern void invalid_choice();

/**
 * Clears any remaining characters in the input buffer
*/
extern void clear_input_buffer();

/**
 * Determines if a user should be locked out based on failed attempts
 * @param failed_attempts Number of consecutive failed login attempts
 * @return true if user should be locked out, false otherwise
*/
extern bool should_user_get_locked_out(uint8_t failed_attempts);

/**
 * Handles user lockout logic after failed login attempts
 * @param failed_attempts Pointer to number of failed attempts
 * @param lockout_time Pointer to the current lockout duration
 * @param lockout_start Pointer to the lockout start timestamp
*/
extern void handle_lockout(uint8_t *failed_attempts, uint32_t *lockout_time, time_t *lockout_start);

/**
 * Displays the remaining lockout time to the user
 * @param seconds Number of seconds remaining in lockout
*/
extern void display_lockout_time(uint32_t seconds);

/**
 * Loads previous lockout data from file
 * @param failed_attempts Pointer to store number of failed attempts
 * @param lockout_time Pointer to store lockout duration
 * @param lockout_start Pointer to store lockout start time
*/
extern void load_lockout_data(uint8_t *failed_attempts, uint32_t *lockout_time, time_t *lockout_start);

/**
 * Saves current lockout state to file
 * @param failed_attempts Current number of failed attempts
 * @param lockout_time Current lockout duration
 * @param lockout_start Current lockout start timestamp
*/
extern void save_lockout_data(uint8_t failed_attempts, uint32_t lockout_time, time_t lockout_start);