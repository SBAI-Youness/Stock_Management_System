#pragma once

#include "./utils.h"

// Path to the CSV file where stock data is stored
#define STOCK_FILE "../data/stock.csv"

// Header line for the stock file
#define STOCK_HEADER_FILE "Id,Name,Description,Username,Unit Price(in $),Stock Quantity,Stock Alert Threshold,Last Stock Entry Date,Last Stock Exit Date\n"

#define MIN_NAME_LENGTH 4
#define MAX_NAME_LENGTH 20

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

/**
 * Creates and initializes a new Product structure
 * @return Pointer to newly allocated Product structure
*/
extern struct Product *create_product();

/**
 * Sets up function pointers for Product methods
 * @param product Pointer to Product structure to initialize
*/
extern void set_product_methods(struct Product *product);

/**
 * Displays and handles the product management menu options
 * @param user Pointer to the current logged-in user
*/
extern void product_management_menu(const struct User *user);

/**
 * Handles the process of adding a new product
 * @param user Pointer to the current logged-in user
*/
extern void add_product(const struct User *user);

/**
 * Generates a unique numeric identifier for a new product
 * @return Unique product ID
*/
extern unsigned long long int generate_unique_product_id();

/**
 * Checks if a product ID is already in use
 * @param id Product ID to check
 * @return true if ID is taken, false otherwise
*/
extern bool is_product_id_taken(unsigned long long int id);

/**
 * Sets the name for a product
 * @param self Pointer to Product structure
*/
extern void set_name(struct Product *self);

/**
 * Validates a product name against system requirements
 * @param name Product name to validate
 * @return true if name is valid, false otherwise
*/
extern bool is_name_valid(const char *name);

/**
 * Sets the description for a product
 * @param self Pointer to Product structure
*/
extern void set_description(struct Product *self);

/**
 * Validates a product description against system requirements
 * @param description Product description to validate
 * @return true if description is valid, false otherwise
*/
extern bool is_description_valid(const char *description);

/**
 * Sets the unit price for a product
 * @param self Pointer to Product structure
*/
extern void set_unit_price(struct Product *self);

/**
 * Validates a product unit price
 * @param price Price to validate
 * @return true if price is valid, false otherwise
*/
extern bool is_unit_price_valid(float price);

/**
 * Sets the quantity for a product
 * @param self Pointer to Product structure
*/
extern void set_quantity(struct Product *self);

/**
 * Validates a product quantity
 * @param quantity Quantity to validate
 * @return true if quantity is valid, false otherwise
*/
extern bool is_quantity_valid(size_t quantity);

/**
 * Sets the alert threshold for a product
 * @param self Pointer to Product structure
*/
extern void set_alert_threshold(struct Product *self);

/**
 * Validates a product alert threshold
 * @param threshold Threshold to validate
 * @param quantity Current product quantity
 * @return true if threshold is valid, false otherwise
*/
extern bool is_alert_threshold_valid(size_t threshold, size_t quantity);

/**
 * Sets a Date structure to the current system date
 * @param date Pointer to Date structure to update
*/
extern void set_current_date(struct Date *date);

/**
 * Handles the process of modifying an existing product
 * @param user Pointer to the current logged-in user
*/
extern void modify_product(const struct User *user);

/**
 * Handles the process of deleting a product
 * @param user Pointer to the current logged-in user
*/
extern void delete_product(const struct User *user);

/**
 * Displays all products for the current user
 * @param user Pointer to the current logged-in user
*/
extern void view_products(const struct User *user);

/**
 * Searches for a product by name and owner username
 * @param name Product name to search for
 * @param username Username of product owner
 * @return Pointer to found Product or NULL if not found
*/
extern struct Product *search_product(const char *name, const char *username);

/**
 * Sorts the product database alphabetically by name
*/
extern void sort_products_by_name();

/**
 * Comparison function for sorting products by name
 * @param a Pointer to first Product to compare
 * @param b Pointer to second Product to compare
 * @return Negative if a < b, 0 if equal, positive if a > b
*/
extern int compare_products_by_name(const void *a, const void *b);

/**
 * Displays product information
 * @param self Pointer to Product structure to display
*/
extern void display_product(const struct Product *self);

/**
 * Saves product data to CSV file
 * @param self Pointer to Product structure to save
*/
extern void save_product(const struct Product *self);

/**
 * Frees all memory associated with a Product structure
 * @param self Pointer to Product structure to free
*/
extern void free_product(struct Product *self);