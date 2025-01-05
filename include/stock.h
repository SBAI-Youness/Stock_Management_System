#pragma once

#include "./utils.h"

// Path to the CSV file where stock data is stored
#define STOCK_FILE "../data/stock.csv"

// Header line for the stock file
#define STOCK_HEADER_FILE "Id,Name,Description,Username,Unit Price(in $),Stock Quantity,Stock Alert Threshold,Last Stock Entry Date,Last Stock Exit Date\n"

#define MIN_NAME_LENGTH 4
#define MAX_NAME_LENGTH 16

#define MIN_DESCRIPTION_LENGTH 8
#define MAX_DESCRIPTION_LENGTH 64

struct Date {
  uint8_t day;
  uint8_t month;
  uint16_t year;
};

// Product structure with attributes and methods (function pointers)
struct Product {
  //* Attributes
  unsigned long long int id;
  char *name;
  char *description;
  char *username;
  float unit_price;
  size_t quantity;
  size_t alert_threshold;
  struct Date last_entry_date;
  struct Date last_exit_date;

  //* Methods
  void (*set_name)(struct Product *self);
  void (*set_description)(struct Product *self);
  void (*set_unit_price)(struct Product *self);
  void (*set_quantity)(struct Product *self);
  void (*set_alert_threshold)(struct Product *self);
  void (*display_product)(const struct Product *self);
  void (*save_product)(const struct Product *self);
  void (*free_product)(struct Product *self);
};

// Function used to create a new product
extern struct Product *create_product();

// Function used to set the product's methods
extern void set_product_methods(struct Product *product);

// Function used to display the product management menu
extern void product_management_menu(const struct User *user);

// Function used to add a product
extern void add_product(const struct User *user);

// Function used to generate a unique id for a product
extern unsigned long long int generate_unique_product_id();

// Function used to check if a product id is already taken
extern bool is_product_id_taken(unsigned long long int id);

// Function used to set the product's name
extern void set_name(struct Product *self);

// Function used to check if the product's name is valid
extern bool is_name_valid(const char *name);

// Function used to set the product's description
extern void set_description(struct Product *self);

// Function used to check if the product's description is valid
extern bool is_description_valid(const char *description);

// Function used to set the product's unit price
extern void set_unit_price(struct Product *self);

// Function used to check if the product's unit price is valid
extern bool is_unit_price_valid(float price);

// Function used to set the product's quantity
extern void set_quantity(struct Product *self);

// Function used to check if the product's quantity is valid
extern bool is_quantity_valid(size_t quantity);

// Function used to set the product's alert threshold
extern void set_alert_threshold(struct Product *self);

// Function used to check if the product's alert threshold is valid
extern bool is_alert_threshold_valid(size_t threshold, size_t quantity);

// Function used to set a Date structure to the current date
extern void set_current_date(struct Date *date);

// Function used to modify a product
extern void modify_product(const struct User *user);

// Function used to delete a product
extern void delete_product(const struct User *user);

// Function used to view the products in the stock
extern void view_products(const struct User *user);

// Function used to search for a product by its name and username
extern struct Product *search_product(const char *name, const char *username);

// Function used to sort the database by name
extern void sort_products_by_name();

// Function used to compare products by name
extern int compare_products_by_name(const void *a, const void *b);

// Function used to display a product's information
extern void display_product(const struct Product *self);

// Function used to save a product's information to a CSV file
extern void save_product(const struct Product *self);

// Function used to free the memory allocated for a product
extern void free_product(struct Product *self);