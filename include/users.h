#pragma once

#include "./utils.h"

// Path to the CSV file where user data is stored
#define USERS_FILE "../data/users.csv"

// Header line for the users file
#define USERS_HEADER_FILE "Username,Password\n"

#define MIN_USERNAME_LENGTH 4
#define MAX_USERNAME_LENGTH 16
#define MIN_PASSWORD_LENGTH 6
#define MAX_PASSWORD_LENGTH 36

// User structure with attributes and methods (function pointers)
struct User {
  //* User attributes
  char *username;
  char *password;

  //* User methods
  void (*set_username)(struct User *self);
  void (*set_password)(struct User *self);
  void (*display_user)(const struct User *self);
  void (*save_user)(const struct User *self);
  void (*free_user)(struct User *self);
};

// Function used to create and initialize a new user
extern struct User *create_user();

// Function used to set the user methods
extern void set_user_methods(struct User *user);

// Function used to handle user sign up process
extern void sign_up();

// Function used to handle user log in process
extern void log_in();

// Function used to manage and handle a session for a given user
extern void user_session(const struct User *user);

// Function used to read the user's username in a secure way
extern void set_username(struct User *self);

// Function used to check if the username is valid
extern bool is_username_valid(const char *username);

// Function used to read the user's password in a secure way
extern void set_password(struct User *self);

// Function used to check if the password is valid
extern bool is_password_valid(const char *password);

// Function used to check if a username is taken or not
extern bool is_username_taken(const char *username);

// Function used to authenticate the user
extern bool authenticate_user(const char *username, const char *password);

// Function used to display the user's information
extern void display_user(const struct User *self);

// Function used to save the user data to a CSV file
extern void save_user(const struct User *self);

// Function used to free the memory allocated for a user
extern void free_user(struct User *self);