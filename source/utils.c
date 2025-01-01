#include "../include/utils.h"

void print_project_name() {
  system("cls");
  printf(BLUE BOLD UNDERLINE "\t\t=== Stock Management System ===\n\n" RESET);
}

void display_main_menu(size_t *menu_choice) {
  system("cls");
  print_project_name();
  printf("1. Sign Up\n");
  printf("2. Login\n");
  printf("3. Exit\n");
  printf(" >> ");
  if (scanf("%zu", &(*menu_choice)) != 1) // Read the user's choice
    (*menu_choice) = 0; // If the user's choice is not a number, set it to 0
  rewind(stdin); // Clear input buffer
}

void print_success_message(const char *success_message) {
  printf(GREEN "Success: %s!\n" RESET, success_message);
  sleep(NORMAL_DELAY); // Wait for 3 seconds
}

void print_warning_message(const char *format, ...) {
    va_list args;
    va_start(args, format);
    printf(ORANGE "Warning: ");
    vprintf(format, args);
    printf("!\n" RESET);
    va_end(args);
    sleep(NOT_NORMAL_DELAY); // Wait for 5 seconds
}

void print_error_message(const char *error_message) {
  fprintf(stderr, RED "Error: %s!\n" RESET, error_message);
  sleep(NOT_NORMAL_DELAY); // Wait for 5 seconds
}

void log_out() {
  printf(YELLOW "Logging out...\n" RESET);
  sleep(NORMAL_DELAY); // Wait for 3 second
  return;
}

void exit_program() {
  printf(YELLOW "Exiting...\n" RESET);
  sleep(NORMAL_DELAY); // Wait for 3 seconds
  exit(EXIT_SUCCESS);
}

void invalid_choice() {
  print_warning_message("Invalid choice. Please try again");
}