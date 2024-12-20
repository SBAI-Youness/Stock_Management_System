#include "../include/utils.h"

struct User *create_user() {
  // Allocate memory for the new user
  struct User *new_user = (struct User *) malloc(sizeof(struct User));

  // Check if memory allocation was successful
  if (new_user == NULL) {
    print_error_message("Failed to  allocate memory for the new user");
    return NULL;
  }

  // Allocate memory for user attributes
  new_user->username = (char *) malloc(MAX_USERNAME_LENGTH * sizeof(char));
  new_user->password = (char *) malloc(MAX_PASSWORD_LENGTH * sizeof(char));

  // Check if memory allocation was successful
  if (new_user->username == NULL || new_user->password == NULL) {
    print_error_message("Failed to allocate memory for the new user attributes");
    free_user(new_user);
    return NULL;
  }

  // Set the user methods
  set_user_methods(new_user);

  return new_user; // Return the pointer to the new user
}

void set_user_methods(struct User *user) {
  // Assign the function pointers to the user methods
  user->set_username = set_username;
  user->set_password = set_password;
  user->display_user = display_user;
  user->save_user = save_user;
  user->free_user = free_user;
}

void sign_up() {
  // Create a new user
  struct User *user = create_user();

  // Check if the user was created successfully
  if (user == NULL) {
    print_error_message("Failed to create a new user");
    return;
  }

  // Set the user's username and password
  bool isUnique = false; // Flag to check if the username is unique or not

  while (isUnique == false) {
    print_project_name();
    printf(DARK_GREEN UNDERLINE "\t--- Sign up ---\n\n" RESET);
    user->set_username(user); // Set the user's username

    // Check if the username is unique
    if (is_username_taken(user->username) == false)
      isUnique = true;
    else {
      printf(ORANGE "Username is already taken. Please choose a different username.\n" RESET);
      sleep(NOT_NORMAL_DELAY); // Wait for 5 seconds
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
    }
  }

  // Set the user's password
  user->set_password(user);

  user->save_user(user);

  print_success_message("User successfully signed up");

  user_session(user);

  user->free_user(user);
}

void log_in() {
  // Create a new user
  struct User *user = create_user();

  // Check if the user was created successfully
  if (user == NULL) {
    print_error_message("Failed to create a user");
    return;
  }

  bool isAuthenticated = false; // Flag to check if the user is authenticated or not

  while (isAuthenticated == false) {
    print_project_name();
    printf(DARK_GREEN UNDERLINE "\t--- Log in ---\n\n" RESET);
    user->set_username(user); // Set the user's username
    user->set_password(user); // Set the user's password

    // Check if the user is authenticated
    if (authenticate_user(user->username, user->password) == true) {
      print_success_message("User successfully logged in");
      isAuthenticated = true;
    }
    else {
      printf(ORANGE "Invalid username or password. Please try again.\n" RESET);
      sleep(NOT_NORMAL_DELAY); // Wait for 5 seconds
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
    }
  }

  // Display the user's information
  user_session(user);

  // Free the user memory
  user->free_user(user);
}

void user_session(const struct User *user) {
  size_t session_choice; // Variable to store the user's choice

  do {
    print_project_name();
    printf(DARK_GREEN UNDERLINE "\t--- Welcome, " RESET CYAN UNDERLINE "%s" RESET DARK_GREEN UNDERLINE"! ---\n\n" RESET, user->username);
    printf("1. View Profile\n");
    printf("2. Product Management\n");
    printf("3. View Products\n");
    printf("4. Search Products\n");
    printf("5. Sort Products\n");
    printf("6. Log out\n");
    printf("7. Exit\n");
    printf(" >> ");
    if (scanf("%zu", &session_choice) != 1) // Read the user's choice
      session_choice = 0; // If the user's choice is not a number, set it to 0
    rewind(stdin); // Clear input buffer


    // Perform the action based on the user's choice
    switch (session_choice) {
      case 1:
        user->display_user(user);
        break;
      case 2:
        // Add, Modify or delete a product
        product_management_menu(&user);
        break;
      case 3:
        // View all products in the database
        break;
      case 4:
        // Search for a product by its name and username
        break;
      case 5:
        // Sort products by name and unit price
        break;
      case 6:
        log_out();
        break;
      case 7:
        exit_program();
        break;
      default:
        invalid_choice();
    }
  } while (session_choice != 6); // Exit the loop if the user chooses to log out
}

void set_username(struct User *self) {
  // Check if the user structure is valid
  if (self == NULL) {
    print_error_message("Invalid user structure");
    return;
  }

  char temp_username[MAX_USERNAME_LENGTH];
  bool isValid = false; // Flag to check if the username is valid or not

  while (isValid == false) {
    printf("Username: ");
    rewind(stdin);

    // Use secure input reading with size limit
    if (fgets(temp_username, MAX_USERNAME_LENGTH, stdin) == NULL) {
      print_error_message("Failed to read username"); // Handle input error
      return;
    }

    // Sanitize input by removing traling newline
    size_t length = strcspn(temp_username, "\n");
    temp_username[length] = '\0';

    // Check the username length
    if (length < MIN_USERNAME_LENGTH || length > MAX_USERNAME_LENGTH) {
      printf(ORANGE "Username must be between %d and %d characters long.\n" RESET, MIN_USERNAME_LENGTH, MAX_USERNAME_LENGTH);
      sleep(NOT_NORMAL_DELAY); // Wait for 5 secondes
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      continue;
    }

    // Check if the username is valid (no spaces or special characters)
    if (is_username_valid(temp_username) == false) {
      printf(ORANGE "Username can only contain alphanumeric characters and maximum 2 underscores, and must include at least one alphanumeric character.\n" RESET);
      sleep(NOT_NORMAL_DELAY); // Wait for 5 secondes
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      continue;
    }

    // Copy validated username to user struct using strncpy
    strncpy(self->username, temp_username, MAX_USERNAME_LENGTH - 1);
    self->username[MAX_USERNAME_LENGTH - 1] = '\0';
    isValid = true;
  }
}

bool is_username_valid(const char *username) {
  // Check if username is NULL
  if (username == NULL)
    return false;

  bool hasAlphaNumeric = false;
  size_t underscore_count = 0,
         length = strnlen(username, MAX_USERNAME_LENGTH);

  // Check if username contains only alphanumeric characters and maximum 2 underscores
  for (size_t i = 0; i < length; i++) {
    unsigned char c = (unsigned char) username[i];

    if (isalnum(c))
      hasAlphaNumeric = true; // At least one alphanumeric character found
    else if (c == '_') {
      underscore_count++;
      if (underscore_count > 2)
        return false; // More than 2 underscores found
      if (i > 0 && username[i-1] == '_')
        return false; // Prevent consecutive underscores
    }
    else
      return false; // Invalid character found
  }

  return hasAlphaNumeric; // Username is valid if it contains at least one alphanumeric character and maximum 2 underscores
}

void set_password(struct User *self) {
  // Check if the user structure is valid
  if (self == NULL) {
    print_error_message("Invalid user structure");
    return;
  }

  char temp_password[MAX_PASSWORD_LENGTH];
  bool isValid = false; // Flag to check if the password is valid or not

  while (isValid == false) {
    printf("Password: ");
    rewind(stdin);

    // Use secure input reading with size limit
    if (fgets(temp_password, MAX_PASSWORD_LENGTH, stdin) == NULL) {
      print_error_message("Failed to read password"); // Handle input error
      return;
    }

    // Sanitize input by removing trailing newline
    size_t length = strcspn(temp_password, "\n");
    temp_password[length] = '\0';

    // Check password length
    if (length < MIN_PASSWORD_LENGTH || length > MAX_PASSWORD_LENGTH) {
      printf(ORANGE "Password must be between %d and %d characters long.\n" RESET, MIN_PASSWORD_LENGTH, MAX_PASSWORD_LENGTH);
      sleep(NOT_NORMAL_DELAY); // Wait for 5 seconds
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      continue;
    }

    // Check if the password contains at least one uppercase letter, one lowercase letter, and one digit
    if (is_password_valid(temp_password) == false) {
      printf(ORANGE "Password must contain at least 3 of the following requirements: uppercase letter, one lowercase letter, one special character, and one digit.\n" RESET);
      sleep(NOT_NORMAL_DELAY); // Wait for 5 seconds
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      continue;
    }

    // Copy validated password to user struct using strncpy
    strncpy(self->password, temp_password, MAX_PASSWORD_LENGTH - 1);
    self->password[MAX_PASSWORD_LENGTH - 1] = '\0';
    isValid = true;
  }
}

bool is_password_valid(const char *password) {
  // Check if password is NULL
  if (password == NULL)
    return false;

  // Ensure password contains at least one digit, one lowercase, and one uppercase letter
  bool hasDigit = false,
       hasLower = false,
       hasUpper = false,
       hasSpecial = false;
  size_t length = strnlen(password, MAX_PASSWORD_LENGTH);

  for (size_t i = 0; i < length; i++) {
    unsigned char c = (unsigned char) password[i];

    if (isdigit(c))
      hasDigit = true;
    else if (islower(c))
      hasLower = true;
    else if (isupper(c))
      hasUpper = true;
    else if (strchr("!@#$%^&*()_+-=[]{}|;:,.<>?", c))
      hasSpecial = true;
  }

  // Require at least 3 of the 4 character types
  size_t criteria_met = hasDigit + hasLower + hasUpper + hasSpecial;
  return criteria_met >= 3;
}

bool is_username_taken(const char *username) {
  // Open the users file to search for the user
  FILE *file = fopen(USERS_FILE, "r");

  // Check if the file was opened successfully
  if (file == NULL) {
    print_error_message("Unable to open the users file");
    return false;
  }

  char tempUsername[MAX_USERNAME_LENGTH];
  while (fscanf(file, "%16[^,],%*s\n", tempUsername) == 1)
    if (strcmp(tempUsername, username) == 0 && strcmp(tempUsername, "Username") != 0) {
      fclose(file);
      return true; // Username is already taken
    }

  // User not found
  fclose(file);
  return false; // Username is available
}

bool authenticate_user(const char *username, const char *password) {
  // Open the users file to search for the user
  FILE *file = fopen(USERS_FILE, "r");

  // Check if the file was opened successfully
  if (file == NULL) {
    print_error_message("Unable to open the users file");
    return false;
  }

  struct User *temp_user = create_user();

  // Check if the user was created successfully
  if (temp_user == NULL) {
    fclose(file);
    return false;
  }

  while (fscanf(file, "%16[^,],%36[^\n]\n", temp_user->username, temp_user->password) == 2)
    if (strcmp(temp_user->username, username) == 0 && strcmp(temp_user->password, password) == 0) {
      temp_user->free_user(temp_user);
      fclose(file);
      return true; // Authentication successful
    }

  temp_user->free_user(temp_user);
  fclose(file);
  return false; // Authentication failed
}

void display_user(const struct User *self) {// Display the user's information
  printf(BROWN BOLD UNDERLINE "Username:" RESET GREEN " %s\n" BROWN BOLD UNDERLINE "Password:" RESET GREEN " %s\n" RESET, self->username, self->password);
  sleep(NORMAL_DELAY); // Wait for 5 seconds
}

void save_user(const struct User *self) {
  // Open the users file in append mode to add the user data
  FILE *file = fopen(USERS_FILE, "a");

  // Check if the file was opened successfully
  if (file == NULL) {
    print_error_message("Unable to open the users file");
    return;
  }

  // Check if the file is empty (to add headers)
  fseek(file, 0, SEEK_END); // Move to the end of the file
  long file_size = ftell(file); // Get file size
  if (file_size == 0)
    fprintf(file, "Username,Password\n"); // Write headers

  // Write username and password to the file
  fprintf(file, "%s,%s\n", self->username, self->password);
  fclose(file); // Close the file after writing
}

void free_user(struct User *self) {
  // Check if the user structure is valid
  if (self == NULL) {
    print_error_message("Invalid user structure");
    return;
  }

  if (self->username != NULL)
    free(self->username); // Free username memory
  if (self->password != NULL)
    free(self->password); // Free password memory
  free(self); // Free the User struct itself
}