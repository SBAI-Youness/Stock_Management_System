#include "../include/utils.h"

int main() {
  size_t menu_choice; // Variable to store the user's choice

  srand(time(NULL)); // Seed the random number generator

  do {
    display_main_menu_and_get_choice(&menu_choice);

    // Switch statement to handle the user's choice
    switch (menu_choice) {
      case 1: // Sign up
        sign_up();
        break;
      case 2: // Log in
        log_in();
        break;
      case 3: // Exit
        exit_program();
        break;
      default: // Invalid choice
        invalid_choice();
    }
  } while (menu_choice != 3); // Exit the loop when the user chooses to exit

  return EXIT_SUCCESS;
}