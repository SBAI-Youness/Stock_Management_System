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
  new_product->name = (char *) malloc(MAX_NAME_LENGTH * sizeof(char));
  new_product->description = (char *) malloc(MAX_DESCRIPTION_LENGTH * sizeof(char));
  new_product->username = (char *) malloc(MAX_USERNAME_LENGTH * sizeof(char));

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

void product_management_menu(const struct User user) {
  size_t menu_choice; // Variable to store the user's choice

  do {
    print_project_name();
    printf(DARK_GREEN UNDERLINE "\t--- Product Management Menu ---\n\n" RESET);
    printf("1. Add Product\n");
    printf("2. Modify Product\n");
    printf("3. Delete Product\n");
    printf("4. Go Back\n");
    printf(" >> ");
    if (scanf("%zu", &menu_choice) != 1) // Read the user's choice
      menu_choice = 0; // If the user's choice is not a number, set it to 0
    rewind(stdin); // Clear input buffer

    switch (menu_choice) {
      case 1:
        add_product(user);
        break;
      case 2:
        // Modify a product
        break;
      case 3:
        delete_product(user);
        break;
      case 4:
        break;
      default:
        invalid_choice();
    }
  } while (menu_choice != 4); // Exit the loop if the user chooses to go back to the user session menu
}

void add_product(const struct User user) {
  // Create a new product
  struct Product *new_product = create_product();

  // Check if the new product was created successfully
  if (new_product == NULL) {
    print_error_message("Failed to add a new product");
    return;
  }

  // Generate a unique product ID
  if ((new_product->id = generate_unique_product_id()) > 65535) {
    print_error_message("Failed to add a new product (Not enough space)");
    new_product->free_product(new_product);
    return;
  }

  // Set the product's name automatically
  strcpy(new_product->username, user.username);

  bool isUnique = false; // Flag to check if the product name is unique ot not
  while (isUnique == false) {
    print_project_name();
    printf(DARK_GREEN UNDERLINE "\t--- Add Product ---\n\n" RESET);
    new_product->set_name(new_product);

    // Check if the product's name is unique
    if (search_product(new_product->name, new_product->username) == NULL)
      isUnique = true;
    else {
      printf(ORANGE "Product's name is already taken. Please choose a different name.\n" RESET);
      sleep(NOT_NORMAL_DELAY); // Wait for 5 seconds
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
    }
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

uint16_t generate_unique_product_id() {
  uint16_t id;

  do {
    id = rand() % 65536; // Generate a random number between 0 and 65535
  } while(is_product_id_taken(id) == true && is_id_valid(id) == true);

  return id;
}

bool is_id_valid(uint16_t id) {
  return id >= 0 && id <= 65535;
}

bool is_product_id_taken(uint16_t id) {
  // Open the stock file in read-only mode
  FILE *file = fopen(STOCK_FILE, "r");

  // Check if the file was opened successfully
  if (file == NULL) {
    print_error_message("Failed to open the stock file");
    return true;
  }

  uint16_t temp_id;

  while (fscanf(file, "%hu,%*s\n", &temp_id) == 1)
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

  char temp_name[MAX_NAME_LENGTH];
  bool isValid = false; // Flag to check if the product's name is valid or not

  while (isValid == false) {
    printf("Name: ");
    rewind(stdin);

    // Use secure input reading with size limit
    if (fgets(temp_name, MAX_NAME_LENGTH, stdin) == NULL) {
      print_error_message("Failed to read name"); // Handle input error
      sleep(NOT_NORMAL_DELAY); // Wait for 5 secondes
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      continue;
    }

    // Sanitize input by removing traling newline
    size_t length = strcspn(temp_name, "\n");
    temp_name[length] = '\0';

    if (strcmp(temp_name, "Name") == 0) {
      printf(ORANGE "You can't use this name.\n" RESET);
      sleep(NOT_NORMAL_DELAY); // Wait for 5 secondes
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      continue;
    }

    // Check the name length
    if (length < MIN_NAME_LENGTH || length > MAX_NAME_LENGTH) {
      printf(ORANGE "Name must be between %d and %d characters long.\n" RESET, MIN_NAME_LENGTH, MAX_NAME_LENGTH);
      sleep(NOT_NORMAL_DELAY); // Wait for 5 secondes
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      continue;
    }

    // Check if the name is valid (no special characters)
    if (is_name_valid(temp_name) == false) {
      printf(ORANGE "Name can only contain alphanumeric characters, maximum 2 dashes and 2 spaces, and must include at least one alphanumeric character.\n" RESET);
      sleep(NOT_NORMAL_DELAY); // Wait for 5 secondes
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      continue;
    }

    // Copy validated name to product struct using strncpy
    strncpy(self->name, temp_name, MAX_NAME_LENGTH - 1);
    self->name[MAX_NAME_LENGTH - 1] = '\0';
    isValid = true;
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

  char temp_description[MAX_DESCRIPTION_LENGTH];
  bool isValid = false; // Flag to check if the product's description is valid or not

  while (isValid == false) {
    printf("Description: ");
    rewind(stdin);

    // Use secure input reading with size limit
    if (fgets(temp_description, MAX_DESCRIPTION_LENGTH, stdin) == NULL) {
      print_error_message("Failed to read description"); // Handle input error
      sleep(NOT_NORMAL_DELAY); // Wait for 5 secondes
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      continue;
    }

    // Sanitize input by removing traling newline
    size_t length = strcspn(temp_description, "\n");
    temp_description[length] = '\0';

    if (length < MIN_DESCRIPTION_LENGTH || length > MAX_DESCRIPTION_LENGTH) {
      printf(ORANGE "Description must be between %d and %d characters long.\n" RESET, MIN_DESCRIPTION_LENGTH, MAX_DESCRIPTION_LENGTH);
      sleep(NOT_NORMAL_DELAY); // Wait for 5 secondes
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      continue;
    }

    // Check if the description is valid
    if (is_description_valid(temp_description) == false) {
      printf(ORANGE "Description can only contain characters (alphanumeric and spaces).\n" RESET);
      sleep(NOT_NORMAL_DELAY); // Wait for 5 secondes
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      continue;
    }

    // Copy validated description to product struct using strncpy
    strncpy(self->description, temp_description, MAX_DESCRIPTION_LENGTH - 1);
    self->description[MAX_DESCRIPTION_LENGTH - 1] = '\0';
    isValid = true;
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
  bool isValid = false; // Flag to check if the product's unit price is valid or not

  while (isValid == false) {
    printf("Unit price: ");
    rewind(stdin);

    // Read the unit price from the user
    if (scanf("%f", &price) != 1) {
      print_error_message("Invalid input. Please enter a valid number");
      sleep(NOT_NORMAL_DELAY); // Wait for 5 secondes
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      continue;
    }

    // Validate the unit price
    if (is_unit_price_valid(price) == false) {
      printf(ORANGE "Unit price must be a strict positive number.\n" RESET);
      sleep(NOT_NORMAL_DELAY); // Wait for 5 secondes
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
  bool isValid = false; // Flag to check if the product's quantity is valid or not

  while (isValid == false) {
    printf("Quantity: ");
    rewind(stdin);

    // Read the unit quantity from the user
    if (scanf("%zu", &quantity) != 1) {
      print_error_message("Invalid input. Please enter a valid number");
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      continue;
    }

    // Validate the quantity
    if (is_quantity_valid(quantity) == false) {
      print_error_message("quantity cannot be negative");
      sleep(NOT_NORMAL_DELAY); // Wait for 5 seconds
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
  bool isValid = false; // Flag to check if the product's alert threshold is valid or not

  while (isValid == false) {
    printf("Alert threshold: ");
    rewind(stdin);

    // Read the alert threshold from the user
    if (scanf("%zu", &threshold) != 1) {
      print_error_message("Invalid input. Please enter a valid number");
      printf("\033[A\033[2K");
      printf("\033[A\033[2K");
      continue;
    }

    // Validate the alert threshold
    if (is_alert_threshold_valid(threshold, self->quantity) == false) {
      print_error_message("Alert threshold must be a strict positive number and should be less than the quantity");
      sleep(NOT_NORMAL_DELAY); // Wait for 5 seconds
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

void delete_product(const struct User user) {
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
    if (sscanf(line, "%hu,%[^,],%[^,],%[^,],%f,%zu,%zu,%hhu/%hhu/%hu,%hhu/%hhu/%hu",
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
      if (strcmp(temp_product->name, product->name) == 0 && strcmp(temp_product->username, user.username) == 0)
        product_found = true;
      else {
        // Write the current product to the file
        fprintf(file, "%hu,%s,%s,%s,%.2f,%zu,%zu,%hhu/%hhu/%hu,%hhu/%hhu/%hu\n",
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

  if (product_found)
    print_success_message("Product deleted successfully");
  else {
    printf(ORANGE "Product not found in the stock file" RESET);
    sleep(NOT_NORMAL_DELAY); // Wait for 5 seconds
  }
}

void view_products(const struct User user) {
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
  while (fscanf(file, "%hu,%[^,],%[^,],%[^,],%f,%zu,%zu,%hhu/%hhu/%hu,%hhu/%hhu/%hu\n",
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
    if (strcmp(temp_product->username, user.username) == 0) {
      if (found_products == false)
        printf("--------------------------------------------------------------------\n");
      found_products = true; // Mark that products were found
      display_product(temp_product);
      printf("--------------------------------------------------------------------\n");
    }

  // Check if no products were found
  if (found_products == false) {
    printf(ORANGE "No products found.\n" RESET);
    sleep(NOT_NORMAL_DELAY); // Wait for 5 seconds
  }
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

  while (fscanf(file, "%hu,%[^,],%[^,],%[^,],%f,%zu,%zu,%hhu/%hhu/%hu,%hhu/%hhu/%hu\n",
                &temp_product->id,
                temp_product->name,
                temp_product->description,
                temp_product->username,
                &temp_product->unit_price,
                &temp_product->quantity,
                &temp_product->alert_threshold,
                &temp_product->last_entry_date.day, &temp_product->last_entry_date.month, &temp_product->last_entry_date.year,
                &temp_product->last_exit_date.day, &temp_product->last_exit_date.month, &temp_product->last_exit_date.year) != EOF)
    if (strcmp(temp_product->name, name) == 0 && strcmp(temp_product->username, username) == 0) {
      // Product found, return the pointer to the product
      fclose(file);
      return temp_product;
    }

  // Product not found, clean up and return NULL
  fclose(file);
  temp_product->free_product(temp_product);
  return NULL;
}

void display_product(const struct Product *self) {
  // Display the product's information
  printf(BROWN BOLD UNDERLINE "Id:" RESET GREEN " %hu\n" BROWN BOLD UNDERLINE "Name:" RESET GREEN " %s\n" BROWN BOLD UNDERLINE "Description:" RESET GREEN " %s\n" BROWN BOLD UNDERLINE "Username:" RESET GREEN " %s\n" BROWN BOLD UNDERLINE "Unit Price:" RESET GREEN " %.2f\n" BROWN BOLD UNDERLINE "Quantity:" RESET GREEN " %u\n" BROWN BOLD UNDERLINE "Alert Threshold:" RESET GREEN " %u\n" BROWN BOLD UNDERLINE "Last Entry Date:" RESET GREEN " %hhu" RESET "/" RESET GREEN "%hhu" RESET "/" RESET GREEN "%hu\n" BROWN BOLD UNDERLINE "Last Exit Date:" RESET GREEN " %hhu" RESET "/" RESET GREEN "%hhu" RESET "/" RESET GREEN "%hu\n" RESET,
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
  fprintf(file, "%hu,%s,%s,%s,%.2f,%zu,%zu,%hhu/%hhu/%hu,%hhu/%hhu/%hu\n",
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