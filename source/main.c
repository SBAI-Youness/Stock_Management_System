#include "../include/utils.h"

int main() {
  size_t menu_choice; // Variable to store the user's choice

  do {
    display_main_menu(&menu_choice);

    // Switch statement to handle the user's choice
    switch (menu_choice) {
      case 1:
        sign_up();
        break;
      case 2:
        log_in();
        break;
      case 3:
        exit_program();
        break;
      default:
        invalid_choice();
    }
  } while (menu_choice != 3); // Exit the loop when the user chooses to exit

  return EXIT_SUCCESS;
}