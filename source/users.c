#include "../include/utils.h"

struct User *create_user() {
  // Allocate memory for the new user
  struct User *new_user = (struct User *) malloc(sizeof(struct User));

  // Check if memory allocation was successful
  if (new_user == NULL) {
    print_error_message("Failed to allocate memory for the new user");
    return NULL;
  }

  // Allocate memory for user attributes
  new_user->username = (char *) malloc((MAX_USERNAME_LENGTH + 1) * sizeof(char));
  new_user->password = (char *) malloc((MAX_PASSWORD_LENGTH + 1) * sizeof(char));
  new_user->salt = (char *) malloc((SALT_LENGTH + 1) * sizeof(char));

  // Check if memory allocation was successful
  if (new_user->username == NULL || new_user->password == NULL || new_user->salt == NULL) {
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
  bool isUsernameUnique = false; // Flag to check if the username is unique or not

  while (isUsernameUnique == false) {
    print_project_name();
    printf(DARK_GREEN UNDERLINE "\t--- Sign up ---\n\n" RESET);
    user->set_username(user); // Set the user's username

    // Check if the username is unique
    if (is_username_taken(user->username) == true) {
      print_warning_message("Username is already taken. Please choose a different username");
      continue;
    }

    isUsernameUnique = true;
  }

  // Set the user's password
  user->set_password(user);

  // Generate a random salt
  generate_salt(user->salt);

  // Save the user to the users file
  user->save_user(user);

  print_success_message("User successfully signed up");

  // Start a session for the user
  user_session(user);

  // Free the memory allocated for the user
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

  uint8_t failed_attempts = 0; // Counter for failed login attempts
  uint32_t lockout_time = INITIAL_LOCKOUT_TIME; // lockout time in seconds
  time_t lockout_start = 0; // Time when the user was locked out

  // Load the lockout data from the file
  load_lockout_data(&failed_attempts, &lockout_time, &lockout_start);

  bool isAuthenticated = false; // Flag to check if the user is authenticated or not

  while (isAuthenticated == false) {
    print_project_name();
    printf(DARK_GREEN UNDERLINE "\t--- Log in ---\n\n" RESET);

    if (should_user_get_locked_out(failed_attempts) == true) {
      handle_lockout(&failed_attempts, &lockout_time, &lockout_start);
      continue;
    }

    user->set_username(user); // Set the user's username
    user->set_password(user); // Set the user's password

    // Check if the user is authenticated
    if (authenticate_user(user->username, user->password) == false) {
      print_warning_message("Invalid username or password. Please try again");
      failed_attempts++;
      save_lockout_data(failed_attempts, lockout_time, lockout_start);
      continue;
    }

    print_success_message("User successfully logged in");
    isAuthenticated = true;
    // Reset lockout data after successful login
    failed_attempts = 0;
    lockout_time = INITIAL_LOCKOUT_TIME;
    lockout_start = 0;

    // Save the reset lockout data
    save_lockout_data(failed_attempts, lockout_time, lockout_start);
  }

  // Start a session for the user
  user_session(user);

  // Free the memory allocated for the user
  user->free_user(user);
}

void user_session(const struct User *user) {
  size_t session_choice; // Variable to store the user's choice
  char extra;

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
    if (scanf("%zu%c", &session_choice, &extra) != 2 || extra != '\n') { // Read the user's choice
      session_choice = 0; // If the user's choice is not a number, set it to 0
      clear_input_buffer();
    }

    // Perform the action based on the user's choice
    switch (session_choice) {
      case 1: // Display the user's information (username & password)
        user->display_user(user);
        system("pause");
        break;
      case 2: // Add, Modify or delete a product
        product_management_menu(user);
        break;
      case 3: // View all products in the database
        view_products(user);
        break;
      case 4: // Search for a product in the database
        print_project_name();
        printf(DARK_GREEN UNDERLINE "\t--- Search Products ---\n\n" RESET);

        // Create a new product
        struct Product *product = create_product();
        product->set_name(product);

        // Search for the product in the database
        if ((product = search_product(product->name, user->username)) == NULL) {
          print_warning_message("Product not found");
          break;
        }

        product->display_product(product);
        system("pause");
        product->free_product(product);
        break;
      case 5: // Sort products by name
        sort_products_by_name();
        break;
      case 6: // Log out
        log_out();
        break;
      case 7: // Exit the program
        exit_program();
        break;
      default: // Invalid choice
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

  char temp_username[MAX_USERNAME_LENGTH + 2];
  bool isValid = false; // Flag to check if the username is valid or not

  while (isValid == false) {
    printf("Username: ");

    // Use secure input reading with size limit
    if (fgets(temp_username, sizeof(temp_username) / sizeof(char), stdin) == NULL) {
      print_error_message("Failed to read username"); // Handle input error
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      continue;
    }

    // Sanitize input by removing traling newline
    size_t length = strcspn(temp_username, "\n");
    temp_username[length] = '\0';

    if (strcasecmp(temp_username, "Username") == 0) {
      print_warning_message("You can't use this username");
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      continue;
    }

    // Check the username length
    if (length < MIN_USERNAME_LENGTH || length > MAX_USERNAME_LENGTH) {
      print_warning_message("Username must be between %d and %d characters long", MIN_USERNAME_LENGTH, MAX_USERNAME_LENGTH);
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      continue;
    }

    // Check if the username is valid (no spaces or special characters)
    if (is_username_valid(temp_username) == false) {
      print_warning_message("Username can only contain alphanumeric characters and maximum 2 underscores, and must include at least one alphanumeric character");
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      continue;
    }

    // Copy the validated username to the user structure
    strncpy(self->username, temp_username, MAX_USERNAME_LENGTH);
    self->username[MAX_USERNAME_LENGTH] = '\0'; // Ensure null-termination
    isValid = true; // Exit loop after successful update
  }
}

bool is_username_valid(const char *username) {
  // Check if username is NULL
  if (username == NULL)
    return false;

  bool hasAlphaNumeric = false;
  size_t underscore_count = 0;

  // Check if username contains only alphanumeric characters and maximum 2 underscores
  for (size_t i = 0; username[i] != '\0'; i++) {
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

  char temp_password[MAX_PASSWORD_LENGTH + 2];
  bool isValid = false; // Flag to check if the password is valid or not

  // Get the standard input handle
  HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
  DWORD mode;

  while (isValid == false) {
    printf("Password: ");

    // Get current console mode
    GetConsoleMode(hStdin, &mode);
    // Disable echo
    SetConsoleMode(hStdin, mode & ~ENABLE_ECHO_INPUT);

    // Use secure input reading with size limit
    if (fgets(temp_password, sizeof(temp_password) / sizeof(char), stdin) == NULL) {
      print_error_message("Failed to read password"); // Handle input error
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      // Restore console mode
      SetConsoleMode(hStdin, mode);
      continue;
    }

    // Restore console mode
    SetConsoleMode(hStdin, mode);

    // Sanitize input by removing trailing newline
    size_t length = strcspn(temp_password, "\n");
    temp_password[length] = '\0';

    // Check password length
    if (length < MIN_PASSWORD_LENGTH || length > MAX_PASSWORD_LENGTH) {
      print_warning_message("Password must be between %d and %d characters long", MIN_PASSWORD_LENGTH, MAX_PASSWORD_LENGTH);
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      continue;
    }

    // Check if the password contains at least one uppercase letter, one lowercase letter, and one digit
    if (is_password_valid(temp_password) == false) {
      print_warning_message("Password must contain at least one uppercase letter, one lowercase letter, and one digit");
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      continue;
    }

    // Copy the validated password to the user structure
    strncpy(self->password, temp_password, MAX_PASSWORD_LENGTH);
    self->password[MAX_PASSWORD_LENGTH] = '\0'; // Ensure null-termination
    isValid = true; // Exit loop after successful update
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

  for (size_t i = 0; password[i] != '\0'; i++) {
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

void generate_salt(char *salt) {
  // Charset for generating random salt
  const char *charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
  size_t charset_size = strlen(charset); // Get the size of the charset (62 characters)

  do {
    for (size_t i = 0; i < SALT_LENGTH; i++)
      salt[i] = charset[rand() % charset_size]; // Generate a random character from the charset
  } while (is_salt_taken(salt) == true); // Keep generating until a unique salt is found

  salt[SALT_LENGTH] = '\0';
}

bool is_salt_taken(const char *salt) {
  FILE *file = fopen(USERS_FILE, "r");
  if (file == NULL) {
    print_error_message("Unable to open the users file");
    return false;
  }

  // Skip the header line
  fseek(file, strlen(USERS_HEADER_FILE), SEEK_SET);

  char temp_salt[SALT_LENGTH + 1];
  while (fscanf(file, "%*[^,],%*[^,],%16[^\n]\n", temp_salt) == 1)
    if (strcmp(temp_salt, salt) == 0) {
      fclose(file);
      return true; // Salt is already in use
    }

  fclose(file);
  return false; // Salt is unique
}

char *hash_password_with_salt(const char *password, const char *salt) {
  if (password == NULL || salt == NULL)
    return NULL;

  unsigned char hash[SHA256_DIGEST_LENGTH];
  static char hash_hex[(SHA256_DIGEST_LENGTH * 2) + 1];
  char salted_password[MAX_PASSWORD_LENGTH + SALT_LENGTH + 1];

  // Combine password and salt
  snprintf(salted_password, sizeof(salted_password), "%s%s", password, salt);

  // Perform SHA-256 hashing
  SHA256((unsigned char *)salted_password, strlen(salted_password), hash);

  // Convert the hash to hexadecimal
  for (size_t i = 0; i < SHA256_DIGEST_LENGTH; i++)
    sprintf(hash_hex + (i * 2), "%02x", hash[i]);

  return hash_hex;
}

bool is_username_taken(const char *username) {
  // Open the users file to search for the user
  FILE *file = fopen(USERS_FILE, "r");

  // Check if the file was opened successfully
  if (file == NULL) {
    print_error_message("Unable to open the users file");
    return false;
  }

  char temp_username[MAX_USERNAME_LENGTH + 1];
  while (fscanf(file, "%16[^,],%*s\n", temp_username) == 1)
    if (strcmp(temp_username, username) == 0) {
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

  char temp_username[MAX_USERNAME_LENGTH + 1],
       temp_password[(SHA256_DIGEST_LENGTH * 2) + 1],
       temp_salt[SALT_LENGTH + 1];

  while (fscanf(file, "%16[^,],%64[^,],%16[^\n]\n", temp_username, temp_password, temp_salt) == 3)
    if (strcmp(temp_username, username) == 0) {
      // Hash the entered password with the stored salt
      char *hashed_password = hash_password_with_salt(password, temp_salt);

      // Check if the password matches the stored hash
      if (hashed_password != NULL && strcmp(temp_password, hashed_password) == 0) {
        fclose(file);
        return true; // Authentication successful
      }
    }

  fclose(file);
  return false; // Authentication failed
}

void display_user(const struct User *self) {// Display the user's information
  printf(BROWN BOLD UNDERLINE "Username:" RESET GREEN " %s\n" BROWN BOLD UNDERLINE "Password:" RESET GREEN " %s\n" RESET, self->username, self->password);
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
    fprintf(file, USERS_HEADER_FILE); // Write headers

  // Hash the password before writing it to the users file
  char *hashed_password = hash_password_with_salt(self->password, self->salt);

  // Check if the password was hashed successfully
  if (hashed_password == NULL) {
    print_error_message("Failed to hash the password");
    return;
  }

  // Write username and password to the file
  fprintf(file, "%s,%s,%s\n", self->username, hashed_password, self->salt);

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