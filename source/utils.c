#include "../include/utils.h"

void print_project_name() {
  printf(BLUE BOLD UNDERLINE "\t\t=== Stock Management System ===\n\n" RESET);
}

void display_main_menu(size_t *choice) {
  system("cls");
  print_project_name();
  printf("1. Sign Up\n");
  printf("2. Login\n");
  printf("3. Exit\n");
  printf(" >> ");
  scanf("%zu", &(*choice)); // Read the user's choice
  rewind(stdin); // Clear input buffer
}

void print_success_message(const char *message) {
  printf(GREEN "Success: %s!\n" RESET, message);
}

void print_error_message(const char *error_message) {
  fprintf(stderr, RED "Error: %s!\n" RESET, error_message);
}

void log_out() {
  printf(YELLOW "Logging out...\n" RESET);
  sleep(3);
  return;
}
void exit_program() {
  printf(YELLOW "Exiting...\n" RESET);
  sleep(3); // Wait for 3 seconds
  exit(EXIT_SUCCESS);
}

void invalid_choice() {
  printf(ORANGE "Invalid choice. Please try again.\n" RESET);
  sleep(3); // Wait for 3 seconds
}