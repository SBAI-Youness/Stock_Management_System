#pragma once

#include "./utils.h"

// Path to the CSV file where user data is stored
#define USERS_FILE "../data/users.csv"

// Header line for the users file
#define USERS_HEADER_FILE "Username,Hashed Password,Salt\n"

#define MIN_USERNAME_LENGTH 4
#define MAX_USERNAME_LENGTH 16

#define MIN_PASSWORD_LENGTH 6
#define MAX_PASSWORD_LENGTH 36

#define SALT_LENGTH 16

// User structure with attributes and methods (function pointers)
struct User {
  //* User attributes
  char *username;
  char *password;
  char *salt;

  //* User methods
  void (*set_username)(struct User *self);
  void (*set_password)(struct User *self);
  void (*display_user)(const struct User *self);
  void (*save_user)(const struct User *self);
  void (*free_user)(struct User *self);
};

/**
 * Creates and initializes a new User structure
 * @return Pointer to newly allocated User structure
*/
extern struct User *create_user();

/**
 * Sets up function pointers for User methods
 * @param user Pointer to User structure to initialize
*/
extern void set_user_methods(struct User *user);

/**
 * Handles the complete user registration process
*/
extern void sign_up();

/**
 * Handles the user authentication process
*/
extern void log_in();

/**
 * Manages an active user session after successful login
 * @param user Pointer to authenticated User structure
*/
extern void user_session(const struct User *user);

/**
 * Securely reads and sets the username for a user
 * @param self Pointer to User structure
*/
extern void set_username(struct User *self);

/**
 * Validates a username against system requirements
 * @param username Username to validate
 * @return true if username is valid, false otherwise
*/
extern bool is_username_valid(const char *username);

/**
 * Securely reads and sets the password for a user
 * @param self Pointer to User structure
*/
extern void set_password(struct User *self);

/**
 * Validates a password against system requirements
 * @param password Password to validate
 * @return true if password is valid, false otherwise
*/
extern bool is_password_valid(const char *password);

/**
 * Generates a cryptographic salt for password hashing
 * @param salt Buffer to store generated salt
*/
extern void generate_salt(char *salt);

/**
 * Checks if a salt is already in use
 * @param salt Salt to check
 * @return true if salt is taken, false otherwise
*/
extern bool is_salt_taken(const char *salt);

/**
 * Hashes a password with given salt using SHA-256
 * @param password Plain text password to hash
 * @param salt Salt to use in hashing
 * @return Allocated string containing hashed password
*/
extern char *hash_password_with_salt(const char *password, const char *salt);

/**
 * Checks if a username is already registered
 * @param username Username to check
 * @return true if username is taken, false otherwise
*/
extern bool is_username_taken(const char *username);

/**
 * Authenticates a user with username and password
 * @param username Username to authenticate
 * @param password Password to verify
 * @return true if authentication successful, false otherwise
*/
extern bool authenticate_user(const char *username, const char *password);

/**
 * Displays user information
 * @param self Pointer to User structure to display
*/
extern void display_user(const struct User *self);

/**
 * Saves user data to CSV file
 * @param self Pointer to User structure to save
*/
extern void save_user(const struct User *self);

/**
 * Frees all memory associated with a User structure
 * @param self Pointer to User structure to free
*/
extern void free_user(struct User *self);