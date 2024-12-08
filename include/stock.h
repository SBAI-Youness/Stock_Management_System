#ifndef STOCK_H
#define STOCK_H

#include "./utils.h"

// Path to the CSV file where stock data is stored
#define STOCK_FILE "../data/stock.csv"

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

// Function used to display a product's information
extern void display_product(const struct Product *self);

// Function used to save a product's information to a CSV file
extern void save_product(const struct Product *self);

// Function used to free the memory allocated for a product
extern void free_product(struct Product *self);

#endif