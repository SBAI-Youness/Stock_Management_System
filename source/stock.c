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
  product->display_product = display_product;
  product->save_product = save_product;
  product->free_product = free_product;
}

void product_management_menu(const struct User **user) {
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
        // Add a product
        break;
      case 2:
        // Modify a product
        break;
      case 3:
        // Delete a product
        break;
      case 4:
        break;
      default:
        invalid_choice();
    }
  } while (menu_choice != 4); // Exit the loop if the user chooses to go back to the user session menu
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
  fseek(file, strlen("Id,Name,Description,Username,Unit Price(in $),Stock Quantity,Stock Alert Threshold,Last Stock Entry Date,Last Stock Exit Date\n"), SEEK_SET);

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
    fprintf(file, "Id,Name,Description,Username,Unit Price,Stock Quantity,Stock Alert Threshold,Last Stock Entry Date,Last Stock Exit Date\n"); // Write headers

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