#include "../include/utils.h"
#include "../include/users.h"

int main() {
  size_t choice;

  do {
    display_main_menu(&choice);
    switch (choice) {
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
  } while (choice != 3);

  return EXIT_SUCCESS;
}