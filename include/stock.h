#ifndef STOCK_H
#define STOCK_H

#include "./utils.h"

// Path to the CSV file where stock data is stored
#define STOCK_FILE "../data/stock.csv"

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
  uint8_t id;
  char *name;
  char *description;
  char *username;
  float unit_price;
  size_t quantity;
  size_t alert_threshold;
  struct Date last_entry_date;
  struct Date last_exit_date;

  ///* Methods
  void (*display_product)(const struct Product *self);
  void (*save_product)(const struct Product *self);
  void (*free_product)(struct Product *self);
};

// Function used to create a new product
extern struct Product *create_product();

// Function used to set the product's methods
extern void set_product_methods(struct Product *product);

// Function used to display the product management menu
extern void product_management_menu(const struct User **user);

// Function used to view products
extern void view_products();

// Function used to display a product's information
extern void display_product(const struct Product *self);

// Function used to save a product's information to a CSV file
extern void save_product(const struct Product *self);

// Function used to free the memory allocated for a product
extern void free_product(struct Product *self);

#endif