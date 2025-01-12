#include "../include/stock.h"

struct Product *create_product() {
  // Allocate memory for the new product
  struct Product *new_product = (struct Product *) malloc(sizeof(struct Product));

  // Check if memory allocation was successful
  if (new_product == NULL) {
    print_error_message("Failed to allocate memory for the new product");
    return NULL;
  }

  // Allocate memory for product attributes
  new_product->name = (char *) malloc((MAX_NAME_LENGTH + 1) * sizeof(char));
  new_product->description = (char *) malloc((MAX_DESCRIPTION_LENGTH + 1) * sizeof(char));
  new_product->username = (char *) malloc((MAX_USERNAME_LENGTH + 1) * sizeof(char));

  // Check if memory allocation was successful
  if (new_product->name == NULL || new_product->description == NULL || new_product->username == NULL) {
    print_error_message("Failed to allocate memory for the new product attributes");
    free_product(new_product);
    return NULL;
  }

  // Set the product methods
  set_product_methods(new_product);

  return new_product; // Return the pointer to the new product
}

void set_product_methods(struct Product *product) {
  // Assign the function pointers to the product methods
  product->set_name = set_name;
  product->set_description = set_description;
  product->set_unit_price = set_unit_price;
  product->set_quantity = set_quantity;
  product->set_alert_threshold = set_alert_threshold;
  product->display_product = display_product;
  product->save_product = save_product;
  product->free_product = free_product;
}

void product_management_menu(const struct User *user) {
  size_t menu_choice; // Variable to store the user's choice
  char extra;

  do {
    print_project_name();
    printf(DARK_GREEN UNDERLINE "\t--- Product Management Menu ---\n\n" RESET);
    printf("1. Add Product\n");
    printf("2. Modify Product\n");
    printf("3. Delete Product\n");
    printf("4. Go Back\n");
    printf(" >> ");
    if (scanf("%zu%c", &menu_choice, &extra) != 2 || extra != '\n') // Read the user's choice
      menu_choice = 0; // If the user's choice is not a number, set it to 0
    rewind(stdin); // Clear input buffer

    switch (menu_choice) {
      case 1: // Add a new product
        add_product(user);
        break;
      case 2: // Modify a product
        modify_product(user);
        break;
      case 3: // Delete a product
        delete_product(user);
        break;
      case 4: // Go back to the user session menu
        break;
      default: // Invalid choice
        invalid_choice();
    }
  } while (menu_choice != 4); // Exit the loop if the user chooses to go back to the user session menu
}

void add_product(const struct User *user) {
  // Create a new product
  struct Product *new_product = create_product();

  // Check if the new product was created successfully
  if (new_product == NULL) {
    print_error_message("Failed to add a new product");
    return;
  }

  // Generate a unique product ID
  if ((new_product->id = generate_unique_product_id()) == 0) {
    print_error_message("Failed to add a new product (Not enough space)");
    new_product->free_product(new_product);
    return;
  }

  // Set the product's name automatically
  strcpy(new_product->username, user->username);

  bool isUnique = false; // Flag to check if the product name is unique ot not
  while (isUnique == false) {
    print_project_name();
    printf(DARK_GREEN UNDERLINE "\t--- Add Product ---\n\n" RESET);
    new_product->set_name(new_product);

    // Check if the product's name is unique
    if (search_product(new_product->name, new_product->username) == NULL)
      isUnique = true;
    else
      print_warning_message("Product's name is already taken. Please choose a different name");
  }

  // Set the description of the product
  new_product->set_description(new_product);

  // Set the product's unit price
  new_product->set_unit_price(new_product);

  // Set the product's quantity
  new_product->set_quantity(new_product);

  // Set the product's alert threshold
  new_product->set_alert_threshold(new_product);

  // Set the product's last entry & exit date automatically
  // TODO: I should implement a better logic for the last exit date.
  set_current_date(&new_product->last_entry_date);
  set_current_date(&new_product->last_exit_date);

  // Save the product to the stock
  new_product->save_product(new_product);

  print_success_message("Product added successfully to the stock");

  // Free the memory allocated for the new product
  new_product->free_product(new_product);
}

unsigned long long int generate_unique_product_id() {
  unsigned long long int id,
                         max_attempts = ULLONG_MAX; // Maximum number of attempts to generate a unique ID

  do {
    // Generate a random number between 1 and ULLONG_MAX
    id = (unsigned long long int)rand() * rand();
    id = id % (ULLONG_MAX - 1) + 1; // Ensure the range is between 1 and ULLONG_MAX

    // Check if the generated ID is unique
    if (is_product_id_taken(id) == false)
      return id;

    // Decrement the number of attempts
    max_attempts--;
  } while(max_attempts > 0);

  // If no valid ID could be generated, return 0 to indicate no space left in the stock
  return 0;
}

bool is_product_id_taken(unsigned long long int id) {
  // Open the stock file in read-only mode
  FILE *file = fopen(STOCK_FILE, "r");

  // Check if the file was opened successfully
  if (file == NULL) {
    print_error_message("Failed to open the stock file");
    return true;
  }

  unsigned long long int temp_id;

  while (fscanf(file, "%llu,%*s\n", &temp_id) == 1)
    if (temp_id == id){
      fclose(file);
      return true; // ID is taken
    }

  // Close the file
  fclose(file);
  return false; // ID is not taken
}

void set_name(struct Product *self) {
  // Check if the product structure is valid
  if (self == NULL) {
    print_error_message("Invalid product structure");
    return;
  }

  char temp_name[MAX_NAME_LENGTH + 2]; // Temporary buffer to store the product's name
  bool isValid = false; // Flag to check if the product's name is valid or not

  while (isValid == false) {
    printf("Name: ");
    rewind(stdin);

    // Use secure input reading with size limit
    if (fgets(temp_name, sizeof(temp_name) / sizeof(char), stdin) == NULL) {
      print_error_message("Failed to read name"); // Handle input error
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      continue;
    }

    // Sanitize input by removing traling newline
    size_t length = strcspn(temp_name, "\n");
    temp_name[length] = '\0';

    if (strcmp(temp_name, "Name") == 0) {
      print_warning_message("You can't use this name");
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      continue;
    }

    // Check the name length
    if (length < MIN_NAME_LENGTH || length > MAX_NAME_LENGTH) {
      print_warning_message("Name must be between %d and %d characters long", MIN_NAME_LENGTH, MAX_NAME_LENGTH);
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      continue;
    }

    // Check if the name is valid (no special characters)
    if (is_name_valid(temp_name) == false) {
      print_warning_message("Name can only contain alphanumeric characters, maximum 2 dashes and 2 spaces, and must include at least one alphanumeric character");
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      continue;
    }

    // Copy the validated name to the product structure
    strncpy(self->name, temp_name, MAX_NAME_LENGTH);
    self->name[MAX_NAME_LENGTH] = '\0'; // Ensure null-termination
    isValid = true; // Exit loop after successful update
  }
}

bool is_name_valid(const char *name) {
  // Check if name is NULL
  if (name == NULL)
    return false;

  bool hasAlphaNumeric = false;
  size_t dash_count = 0,
         space_count = 0,
         length = strnlen(name, MAX_NAME_LENGTH);

  // Check if name contains only alphanumeric characters, maximum 2 dashs and 2 spaces
  for (size_t i = 0; i < length; i++) {
    unsigned char c = (unsigned char) name[i];

    if (isalnum(c))
      hasAlphaNumeric = true; // At least one alphanumeric character found
    else if (c == '-') {
      dash_count++;
      if (dash_count > 2)
        return false; // More than 2 dashs found
      if (i > 0 && name[i-1] == '-')
        return false; // Prevent consecutive dashs
    }
    else if (isspace(c)) {
      space_count++;
      if (space_count > 2)
        return false; // More than 2 spaces found
      if (i > 0 && isspace(name[i-1]))
        return false; // Prevent consecutive spaces
    }
    else
      return false; // Invalid character found
  }

  return hasAlphaNumeric; // name is valid if it contains at least one alphanumeric character, maximum 2 dashs and 2 spaces
}

void set_description(struct Product *self) {
  // Check if the product structure is valid
  if (self == NULL) {
    print_error_message("Invalid product structure");
    return;
  }

  char temp_description[MAX_DESCRIPTION_LENGTH + 2];
  bool isValid = false; // Flag to check if the product's description is valid or not

  while (isValid == false) {
    printf("Description: ");
    rewind(stdin);

    // Use secure input reading with size limit
    if (fgets(temp_description, sizeof(temp_description) / sizeof(char), stdin) == NULL) {
      print_error_message("Failed to read description"); // Handle input error
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      continue;
    }

    // Sanitize input by removing traling newline
    size_t length = strcspn(temp_description, "\n");
    temp_description[length] = '\0';

    if (length < MIN_DESCRIPTION_LENGTH || length > MAX_DESCRIPTION_LENGTH) {
      print_warning_message("Description must be between %d and %d characters long", MIN_DESCRIPTION_LENGTH, MAX_DESCRIPTION_LENGTH);
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      continue;
    }

    // Check if the description is valid
    if (is_description_valid(temp_description) == false) {
      print_warning_message("Description can only contain characters (alphanumeric and spaces)");
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      continue;
    }

    // Copy the validated description to the product structure
    strncpy(self->description, temp_description, MAX_DESCRIPTION_LENGTH);
    self->description[MAX_DESCRIPTION_LENGTH] = '\0'; // Ensure null-termination
    isValid = true; // Exit loop after successful update
  }
}

bool is_description_valid(const char *description) {
  // Check if description is NULL
  if (description == NULL)
    return false;

  size_t length = strnlen(description, MAX_DESCRIPTION_LENGTH);

  // Check if description contains only valid characters (alphanumeric and spaces)
  for (size_t i = 0; i < length; i++)
    if (!isalnum(description[i]) && !isspace(description[i]))
      return false;

  return true;
}

void set_unit_price(struct Product *self) {
  // Check if the product structure is valid
  if (self == NULL) {
    print_error_message("Invalid product structure");
    return;
  }

  float price;
  char extra;
  bool isValid = false; // Flag to check if the product's unit price is valid or not

  while (isValid == false) {
    printf("Unit price: ");
    rewind(stdin);

    // Read the unit price from the user
    if (scanf("%f%c", &price, &extra) != 2 || extra != '\n') {
      print_warning_message("Invalid input. Please enter a valid number");
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      continue;
    }

    // Validate the unit price
    if (is_unit_price_valid(price) == false) {
      print_warning_message("Unit price must be a strict positive number");
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      continue;
    }

    // Set the unit price for the product
    self->unit_price = price;
    isValid = true;
  }
}

bool is_unit_price_valid(float price) {
  // Check if the price is a strict positive number
  return (price > 0)? true: false;
}

void set_quantity(struct Product *self) {
  // Check if the product structure is valid
  if (self == NULL) {
    print_error_message("Invalid product structure");
    return;
  }

  size_t quantity;
  char extra;
  bool isValid = false; // Flag to check if the product's quantity is valid or not

  while (isValid == false) {
    printf("Quantity: ");
    rewind(stdin);

    // Read the unit quantity from the user
    if (scanf("%zu%c", &quantity, &extra) != 2 || extra != '\n') {
      print_warning_message("Invalid input. Please enter a valid number");
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      continue;
    }

    // Validate the quantity
    if (is_quantity_valid(quantity) == false) {
      print_error_message("Quantity must be a strict positive number");
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      continue;
    }

    // Set the quantity for the product
    self->quantity = quantity;
    isValid = true;
  }
}

bool is_quantity_valid(size_t quantity) {
  // Check if the quantity is a positive number
  return (quantity > 0)? true: false;
}

void set_alert_threshold(struct Product *self) {
  // Check if the product structure is valid
  if (self == NULL) {
    print_error_message("Invalid product structure");
    return;
  }

  size_t threshold;
  char extra;
  bool isValid = false; // Flag to check if the product's alert threshold is valid or not

  while (isValid == false) {
    printf("Alert threshold: ");
    rewind(stdin);

    // Read the alert threshold from the user
    if (scanf("%zu%c", &threshold, &extra) != 2 || extra != '\n') {
      print_warning_message("Invalid input. Please enter a valid number");
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      continue;
    }

    // Validate the alert threshold
    if (is_alert_threshold_valid(threshold, self->quantity) == false) {
      print_error_message("Alert threshold must be a strict positive number and should be less than the quantity");
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      continue;
    }

    // Set the alert threshold for the product
    self->alert_threshold = threshold;
    isValid = true;
  }
}

bool is_alert_threshold_valid(size_t threshold, size_t quantity) {
  // Check if the threshold is a strict positive number and should be less than the quantity
  return (threshold > 0 && threshold < quantity)? true: false;
}

void set_current_date(struct Date *date) {
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);

  date->day = tm.tm_mday;
  date->month = tm.tm_mon + 1; // tm_mon is 0-11, so add 1
  date->year = tm.tm_year + 1900; // tm_year is years since 1900
}

void modify_product(const struct User *user) {
  print_project_name();
  printf(DARK_GREEN UNDERLINE "\t--- Modify Product ---\n\n" RESET);

  unsigned long long int product_id;
  char extra;

  // Set the product's id
  while (true) {
    printf("Id: ");
    if (scanf("%llu%c", &product_id, &extra) != 2 || product_id == 0 || extra != '\n') {
      print_warning_message("Invalid product id");
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      rewind(stdin); // Clear the input buffer
      continue;
    }

    printf("--------------------------------------------------------------------\n");
    break;
  }

  // Open the database file in read mode
  FILE *file = fopen(STOCK_FILE, "r");

  // Check if the file was opened successfully
  if (file == NULL) {
    print_error_message("Failed to open the stock file");
    return;
  }

  // Read the entire file into memory
  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  // Allocate memory for the file contents
  char *file_contents = (char *) malloc(file_size + 1);

  // Check if memory allocation was successful
  if (file_contents == NULL) {
    print_error_message("Failed to allocate memory for file contents");
    fclose(file);
    return;
  }

  fread(file_contents, 1, file_size, file);
  file_contents[file_size] = '\0';
  fclose(file);

  // Open the database file in write mode
  file = fopen(STOCK_FILE, "w");

  // Check if the file was opened successfully
  if (file == NULL) {
    print_error_message("Failed to open the stock file");
    free(file_contents);
    return;
  }

  // Write the header to the file
  fprintf(file, STOCK_HEADER_FILE);

  bool product_found = false; // Flag to check if the product was found in the file or not
  struct Product *temp_product = create_product(); // Create a temporary product structure

  // Check if the temporary product was created successfully
  if (temp_product == NULL) {
    print_error_message("Failed to create a temporary product");
    free(file_contents);
    fclose(file);
    return;
  }

  // Parse the file contents and write back all products, modifying the specified product
  char *line = strtok(file_contents, "\n");
  while (line != NULL) {
    if (sscanf(line, "%llu,%[^,],%[^,],%[^,],%f,%zu,%zu,%hhu/%hhu/%hu,%hhu/%hhu/%hu",
               &temp_product->id,
               temp_product->name,
               temp_product->description,
               temp_product->username,
               &temp_product->unit_price,
               &temp_product->quantity,
               &temp_product->alert_threshold,
               &temp_product->last_entry_date.day, &temp_product->last_entry_date.month, &temp_product->last_entry_date.year,
               &temp_product->last_exit_date.day, &temp_product->last_exit_date.month, &temp_product->last_exit_date.year) == 13) {
      // If the current product's id matches the one to modify, update its details
      if (temp_product->id == product_id && strcmp(temp_product->username, user->username) == 0) {
        product_found = true;

        // Modify the product's details
        temp_product->set_name(temp_product);
        temp_product->set_description(temp_product);
        temp_product->set_unit_price(temp_product);
        temp_product->set_quantity(temp_product);
        temp_product->set_alert_threshold(temp_product);
      }

      // Write the current product to the file
      fprintf(file, "%llu,%s,%s,%s,%.2f,%zu,%zu,%hhu/%hhu/%hu,%hhu/%hhu/%hu\n",
                      temp_product->id,
                      temp_product->name,
                      temp_product->description,
                      temp_product->username,
                      temp_product->unit_price,
                      temp_product->quantity,
                      temp_product->alert_threshold,
                      temp_product->last_entry_date.day, temp_product->last_entry_date.month, temp_product->last_entry_date.year,
                      temp_product->last_exit_date.day, temp_product->last_exit_date.month, temp_product->last_exit_date.year);
    }
    line = strtok(NULL, "\n");
  }

  if (product_found == true)
    print_success_message("Product modified successfully");
  else
    print_warning_message("Product not found");

  // Close the file and free the temporary product
  fclose(file);
  temp_product->free_product(temp_product);
  free(file_contents);
}

void delete_product(const struct User *user) {
  // Open the database file in read mode
  FILE *file = fopen(STOCK_FILE, "r");

  // Check if the file was opened successfully
  if (file == NULL) {
    print_error_message("Failed to open the stock file");
    return;
  }

  print_project_name();
  printf(DARK_GREEN UNDERLINE "\t--- Delete Product ---\n\n" RESET);

  // Create a new product
  struct Product *product = create_product();

  // Check if the product was created successfully
  if (product == NULL) {
    print_error_message("Failed to create a new product");
    fclose(file);
    return;
  }

  // Set the product's name
  product->set_name(product);

  // Read the entire file into memory
  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  // Allocate memory for the file contents
  char *file_contents = (char *) malloc(file_size + 1);

  // Check if memory allocation was successful
  if (file_contents == NULL) {
    print_error_message("Failed to allocate memory for file contents");
    fclose(file);
    return;
  }

  fread(file_contents, 1, file_size, file);
  file_contents[file_size] = '\0';
  fclose(file);

  // Open the file in write mode to overwrite it
  file = fopen(STOCK_FILE, "w");

  // Check if the file was opened successfully
  if (file == NULL) {
    print_error_message("Failed to open the stock file in write mode");
    free(file_contents);
    return;
  }

  // Write the header to the file
  fprintf(file, STOCK_HEADER_FILE);

  bool product_found = false; // Flag to check if the product was found in the file or not
  struct Product *temp_product = create_product(); // Create a temporary product structure

  // Check if the temporary product was created successfully
  if (temp_product == NULL) {
    print_error_message("Failed to create a temporary product structure");
    free(file_contents);
    fclose(file);
    return;
  }

  // Parse the file contents and write back all products except the one to be deleted
  char *line = strtok(file_contents, "\n");
  while (line != NULL) {
    if (sscanf(line, "%llu,%[^,],%[^,],%[^,],%f,%zu,%zu,%hhu/%hhu/%hu,%hhu/%hhu/%hu",
               &temp_product->id,
               temp_product->name,
               temp_product->description,
               temp_product->username,
               &temp_product->unit_price,
               &temp_product->quantity,
               &temp_product->alert_threshold,
               &temp_product->last_entry_date.day, &temp_product->last_entry_date.month, &temp_product->last_entry_date.year,
               &temp_product->last_exit_date.day, &temp_product->last_exit_date.month, &temp_product->last_exit_date.year) == 13) {
      // If the current product's name matches the one to delete, skip writing it to the file
      if (strcasecmp(temp_product->name, product->name) == 0 && strcmp(temp_product->username, user->username) == 0)
        product_found = true;
      else {
        // Write the current product to the file
        fprintf(file, "%llu,%s,%s,%s,%.2f,%zu,%zu,%hhu/%hhu/%hu,%hhu/%hhu/%hu\n",
                      temp_product->id,
                      temp_product->name,
                      temp_product->description,
                      temp_product->username,
                      temp_product->unit_price,
                      temp_product->quantity,
                      temp_product->alert_threshold,
                      temp_product->last_entry_date.day, temp_product->last_entry_date.month, temp_product->last_entry_date.year,
                      temp_product->last_exit_date.day, temp_product->last_exit_date.month, temp_product->last_exit_date.year);
      }
    }
    line = strtok(NULL, "\n");
  }

  // Close the file
  fclose(file);

  // Free the allocated memory for the product and file contents
  temp_product->free_product(temp_product);
  product->free_product(product);
  free(file_contents);

  if (product_found == true)
    print_success_message("Product deleted successfully");
  else
    print_warning_message("Product not found in the stock file");
}

void view_products(const struct User *user) {
  // Open the database file in read mode
  FILE *file = fopen(STOCK_FILE, "r");

  // Check if the file was opened successfully
  if (file == NULL) {
    print_error_message("Failed to open the stock file");
    return;
  }

  print_project_name();
  printf(DARK_GREEN UNDERLINE "\t--- View Products ---\n\n" RESET);

  // Read the header line
  fseek(file, strlen(STOCK_HEADER_FILE), SEEK_SET);

  bool found_products = false; // Flag to check if any products are displayed

  // Create a temporary product structure
  struct Product *temp_product = create_product();

  // Check if the temporary product was created successfully
  if (temp_product == NULL) {
    print_error_message("Failed to create a temporary product");
    fclose(file);
    return;
  }

  // Read the file line by line and display the products
  while (fscanf(file, "%llu,%[^,],%[^,],%[^,],%f,%zu,%zu,%hhu/%hhu/%hu,%hhu/%hhu/%hu\n",
                &temp_product->id,
                temp_product->name,
                temp_product->description,
                temp_product->username,
                &temp_product->unit_price,
                &temp_product->quantity,
                &temp_product->alert_threshold,
                &temp_product->last_entry_date.day, &temp_product->last_entry_date.month, &temp_product->last_entry_date.year,
                &temp_product->last_exit_date.day, &temp_product->last_exit_date.month, &temp_product->last_exit_date.year) != EOF)
    // Check if the product belongs to the current user
    if (strcmp(temp_product->username, user->username) == 0) {
      if (found_products == false)
        printf("--------------------------------------------------------------------\n");
      found_products = true; // Mark that products were found
      display_product(temp_product);
      printf("--------------------------------------------------------------------\n");
    }

  // Check if no products were found
  if (found_products == false)
    print_warning_message("No products found");
  else
    system("pause");

  // Close the file & free the temporary product
  fclose(file);
  temp_product->free_product(temp_product);
}

struct Product *search_product(const char *name, const char *username) {
  if (name == NULL || username == NULL) {
    print_error_message("Invalid input");
    return NULL;
  }

  // Open the database file in read mode
  FILE *file = fopen(STOCK_FILE, "r");

  // Check if the file was opened successfully
  if (file == NULL) {
    print_error_message("Failed to open the database file");
    return NULL;
  }

  struct Product *temp_product = create_product();

  if (temp_product == NULL) {
    print_error_message("Failed to create a temporary product");
    fclose(file);
    return NULL;
  }

  // Skip the header line by seeking to the next line
  fseek(file, strlen(STOCK_HEADER_FILE), SEEK_SET);

  while (fscanf(file, "%llu,%[^,],%[^,],%[^,],%f,%zu,%zu,%hhu/%hhu/%hu,%hhu/%hhu/%hu\n",
                &temp_product->id,
                temp_product->name,
                temp_product->description,
                temp_product->username,
                &temp_product->unit_price,
                &temp_product->quantity,
                &temp_product->alert_threshold,
                &temp_product->last_entry_date.day, &temp_product->last_entry_date.month, &temp_product->last_entry_date.year,
                &temp_product->last_exit_date.day, &temp_product->last_exit_date.month, &temp_product->last_exit_date.year) != EOF)
    if (strcasecmp(temp_product->name, name) == 0 && strcmp(temp_product->username, username) == 0) {
      // Product found, return the pointer to the product
      fclose(file);
      return temp_product;
    }

  // Product not found, clean up and return NULL
  fclose(file);
  temp_product->free_product(temp_product);
  return NULL;
}

int compare_products_by_name(const void *a, const void *b) {
  struct Product *product_a = *(struct Product **)a,
                 *product_b = *(struct Product **)b;

  // Compare the names of the products
  return strcmp(product_a->name, product_b->name);
}

//TODO: every user should have his own stock file
void sort_products_by_name() {
  // Open the database file in read mode
  FILE *file = fopen(STOCK_FILE, "r");

  // Check if the file was opened successfully
  if (file == NULL) {
    print_error_message("Failed to open the stock file in read mode");
    return;
  }

  // Skip the header line
  fseek(file, strlen(STOCK_HEADER_FILE), SEEK_SET);

  // Create a dynamic array to store the products
  struct Product **products = NULL;
  size_t product_count = 0;

  // Read the products from the file
  struct Product *temp_product = create_product();
  while (fscanf(file, "%llu,%[^,],%[^,],%[^,],%f,%zu,%zu,%hhu/%hhu/%hu,%hhu/%hhu/%hu\n",
                &temp_product->id,
                temp_product->name,
                temp_product->description,
                temp_product->username,
                &temp_product->unit_price,
                &temp_product->quantity,
                &temp_product->alert_threshold,
                &temp_product->last_entry_date.day, &temp_product->last_entry_date.month, &temp_product->last_entry_date.year,
                &temp_product->last_exit_date.day, &temp_product->last_exit_date.month, &temp_product->last_exit_date.year) != EOF) {
    products = realloc(products, (product_count + 1) * sizeof(struct Product *));
    products[product_count] = temp_product;
    product_count++;
    temp_product = create_product();
  }

  // Close the file
  fclose(file);

  // Sort the products by name
  qsort(products, product_count, sizeof(struct Product *), compare_products_by_name);

  // Open the file in write mode to overwrite it
  file = fopen(STOCK_FILE, "w");

  // Check if the file was opened successfully
  if (file == NULL) {
    print_error_message("Failed to open the stock file in write mode");
    for (size_t i = 0; i < product_count; i++)
      products[i]->free_product(products[i]);
    free(products);
    return;
  }

  // Write the header to the file
  fprintf(file, STOCK_HEADER_FILE);

  // Write the sorted products to the file
  for (size_t i = 0; i < product_count; i++) {
    fprintf(file, "%llu,%s,%s,%s,%.2f,%zu,%zu,%hhu/%hhu/%hu,%hhu/%hhu/%hu\n",
            products[i]->id,
            products[i]->name,
            products[i]->description,
            products[i]->username,
            products[i]->unit_price,
            products[i]->quantity,
            products[i]->alert_threshold,
            products[i]->last_entry_date.day, products[i]->last_entry_date.month, products[i]->last_entry_date.year,
            products[i]->last_exit_date.day, products[i]->last_exit_date.month, products[i]->last_exit_date.year);
    products[i]->free_product(products[i]);
  }

  // Close the file and free the allocated memory
  fclose(file);
  free(products);

  print_success_message("Products sorted by name successfully");
}

void display_product(const struct Product *self) {
  // Display the product's information
  printf(BROWN BOLD UNDERLINE "Id:" RESET GREEN " %llu\n" BROWN BOLD UNDERLINE "Name:" RESET GREEN " %s\n" BROWN BOLD UNDERLINE "Description:" RESET GREEN " %s\n" BROWN BOLD UNDERLINE "Username:" RESET GREEN " %s\n" BROWN BOLD UNDERLINE "Unit Price:" RESET GREEN " %.2f\n" BROWN BOLD UNDERLINE "Quantity:" RESET GREEN " %zu\n" BROWN BOLD UNDERLINE "Alert Threshold:" RESET GREEN " %zu\n" BROWN BOLD UNDERLINE "Last Entry Date:" RESET GREEN " %hhu" RESET "/" RESET GREEN "%hhu" RESET "/" RESET GREEN "%hu\n" BROWN BOLD UNDERLINE "Last Exit Date:" RESET GREEN " %hhu" RESET "/" RESET GREEN "%hhu" RESET "/" RESET GREEN "%hu\n" RESET,
          self->id,
          self->name,
          self->description,
          self->username,
          self->unit_price,
          self->quantity,
          self->alert_threshold,
          self->last_entry_date.day, self->last_entry_date.month, self->last_entry_date.year,
          self->last_exit_date.day, self->last_exit_date.month, self->last_exit_date.year);
}

void save_product(const struct Product *self) {
  // Check if the product structure is valid
  if (self == NULL) {
    print_error_message("Invalid product structure");
    return;
  }

  // Open the products file in append mode to add the product data
  FILE *file = fopen(STOCK_FILE, "a");

  // Check if the file was opened successfully
  if (file == NULL) {
    print_error_message("Unable to open the products file");
    return;
  }

  // Check if the file is empty (to add headers)
  fseek(file, 0, SEEK_END); // Move to the end of the file
  long file_size = ftell(file); // Get file size
  if (file_size == 0)
    fprintf(file, STOCK_HEADER_FILE); // Write headers

  // Write product data to the file
  fprintf(file, "%llu,%s,%s,%s,%.2f,%zu,%zu,%hhu/%hhu/%hu,%hhu/%hhu/%hu\n",
          self->id,
          self->name,
          self->description,
          self->username,
          self->unit_price,
          self->quantity,
          self->alert_threshold,
          self->last_entry_date.day, self->last_entry_date.month, self->last_entry_date.year,
          self->last_exit_date.day, self->last_exit_date.month, self->last_exit_date.year);

  fclose(file); // Close the file after writing
}

void free_product(struct Product *self) {
  // Check if the product structure is valid
  if (self == NULL) {
    print_error_message("Invalid product structure");
    return;
  }

  if (self->name != NULL)
    free(self->name); // Free name memory
  if (self->description != NULL)
    free(self->description); // Free description memory
  if (self->username != NULL)
    free(self->username); // Free username memory
  free(self); // Free the Product struct itself
}