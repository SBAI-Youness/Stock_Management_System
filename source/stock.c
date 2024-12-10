#include "../include/stock.h"

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