#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

#define PROJECT_NAME "Stock Management System"

// Foreground text colors
#define RESET       "\033[0m"
#define RED         "\033[31m"                // Error
#define GREEN       "\033[32m"               // Success
#define YELLOW      "\033[1;33m"            // Exit
#define ORANGE      "\033[38;5;208m"       // Warning
#define DARK_GREEN  "\033[38;2;0;102;51m" // Sign up & Log in
#define BLUE        "\033[34m"           // Project name
#define CYAN        "\033[1;36m"        // Username

// Text styles
#define BOLD        "\033[1m"
#define UNDERLINE   "\033[4m"

// Function used to print the project name
extern void print_project_name();

// Function used to display the main menu
extern void display_main_menu(size_t *choice);

// Function used to print success messages
extern void print_success_message(const char *message);

// Function used to print error messages
extern void print_error_message(const char *message);

// Function used to log out
void log_out();

// Function used to exit the program
extern void exit_program();

// Function used to handle invalid choices
extern void invalid_choice();

#endif