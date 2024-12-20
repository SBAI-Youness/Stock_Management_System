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

void view_products() {
  // TODO: Implement the view products function for the next commit, Yay!!
}

void display_product(const struct Product *self) {
  // Display the product's information
  printf("\nProduct Information:\n");
  printf("  Id: %u\n  Name: %s\n  Description: %s\n  Username: %s\n  Unit Price: %.2f\n  Quantity: %u\n  Alert Threshold: %u\n  Last Entry Date: %u/%u/%u\n  Last Exit Date: %u/%u/%u\n",
          self->id,
          self->name,
          self->description,
          self->username,
          self->unit_price,
          self->quantity,
          self->alert_threshold,
          self->last_entry_date.day, self->last_entry_date.month, self->last_entry_date.year,
          self->last_exit_date.day, self->last_exit_date.month, self->last_exit_date.year);
  sleep(NORMAL_DELAY); // Wait for 5 seconds
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
  fprintf(file, "%u,%s,%s,%s,%.2f,%u,%u,%u/%u/%u,%u/%u/%u\n",
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