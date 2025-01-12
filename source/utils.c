#include "../include/utils.h"

void print_project_name() {
  system("cls");
  printf(BLUE BOLD UNDERLINE "\t\t=== %s ===\n\n" RESET, PROJECT_NAME);
}

void display_main_menu_and_get_choice(size_t *menu_choice) {
  system("cls");
  print_project_name();
  printf("1. Sign Up\n");
  printf("2. Login\n");
  printf("3. Exit\n");
  printf(" >> ");
  char extra;
  if (scanf("%zu%c", &(*menu_choice), &extra) != 2 || extra != '\n') // Read the user's choice
    (*menu_choice) = 0; // If the user's choice is not a number, set it to 0
  rewind(stdin); // Clear input buffer
}

void print_success_message(const char *success_message) {
  printf(GREEN "Success: %s!\n" RESET, success_message);
  sleep(NORMAL_DELAY); // Wait for 3 seconds
}

void print_warning_message(const char *warning_message, ...) {
  va_list args;
  va_start(args, warning_message);
  printf(ORANGE "Warning: ");
  vprintf(warning_message, args);
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

bool should_user_get_locked_out(uint8_t failed_attempts) {
  return failed_attempts >= MAX_FAILED_ATTEMPTS;
}

void handle_lockout(uint8_t *failed_attempts, uint32_t *lockout_time, time_t *lockout_start) {
  time_t current_time = time(NULL); // Get the current time

  // Checks if the lockout timer has been initialized
  if ((*lockout_start) == 0) {
    (*lockout_start) = current_time; // Set the start time of the lockout
    printf(RED "Too many failed attempts. Locked out for ");
    display_lockout_time((*lockout_time));
    printf("\n" RESET);
    sleep(NOT_NORMAL_DELAY); // Wait for 5 seconds
  }

  // Check if lockout period has passed
  if ((current_time - (*lockout_start)) < (*lockout_time)) {
    printf(ORANGE "Please wait ");
    display_lockout_time((*lockout_time) - (int)(current_time - (*lockout_start)));
    printf(RESET);
    sleep(1); // Prevent spamming
    return;
  }

  // Reset failed attempts and lockout time after the lockout period ends
  (*failed_attempts) = 0; // Reset failed attempts
  (*lockout_start) = 0;  // Reset lockout timer
  (*lockout_time) *= 2;  // Double the lockout time for the next lockout
  printf(GREEN "Lockout period over. You can try again.\n" RESET);
  sleep(NORMAL_DELAY); // Wait for a moment before letting the user try again

  save_lockout_data((*failed_attempts), (*lockout_time), (*lockout_start));
}

void display_lockout_time(uint32_t seconds) {
  int days = seconds / 86400;  // 86400 seconds in a day
  seconds %= 86400;
  int hours = seconds / 3600; // 3600 seconds in an hour
  seconds %= 3600;
  int minutes = seconds / 60; // 60 seconds in a minute
  seconds %= 60;

  if (days > 0)
    printf("%u day%s, ", days, (days > 1)? "s": "");

  if (hours > 0)
    printf("%u hour%s, ", hours, (hours > 1)? "s": "");

  if (minutes > 0)
    printf("%u minute%s, ", minutes, (minutes > 1)? "s": "");

  printf("%u second%s", seconds, (seconds > 1)? "s": "");
}

void load_lockout_data(uint8_t *failed_attempts, uint32_t *lockout_time, time_t *lockout_start) {
  // Open the file for reading
  FILE *file = fopen(LOCKOUT_FILE, "r");

  // Check if the file was opened successfully
  if (file == NULL) {
    // If the file doesn't exist, initialize the lockout data
    (*failed_attempts) = 0;
    (*lockout_time) = INITIAL_LOCKOUT_TIME;
    (*lockout_start) = 0;
    return;
  }

  // Skip the header file
  fseek(file, strlen(LOCKOUT_HEADER_FILE), SEEK_SET);

  // Read the lockout data from the file
  if (fscanf(file, "%hhu,%u,%lld", failed_attempts, lockout_time, lockout_start) != 3) {
    // If the file is empty or the data is corrupted, initialize the lockout data
    (*failed_attempts) = 0;
    (*lockout_time) = INITIAL_LOCKOUT_TIME;
    (*lockout_start) = 0;
  }

  // Close the file
  fclose(file);
}

void save_lockout_data(uint8_t failed_attempts, uint32_t lockout_time, time_t lockout_start) {
  // Open the file for writing
  FILE *file = fopen(LOCKOUT_FILE, "w");

  // Check if the file was opened successfully
  if (file == NULL) {
    print_error_message("Failed to save lockout data");
    return;
  }

  // Write the lockout data to the file
  fprintf(file, "%s%hhu,%u,%lld", LOCKOUT_HEADER_FILE, failed_attempts, lockout_time, lockout_start);

  // Close the file
  fclose(file);
}