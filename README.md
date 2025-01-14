# Stock Management System
---

<p align="center">
  <img src="assets/icons/app_icon.png" alt="App Icon" width="180">
</p>


<p align="center">
  <a href="LICENSE"><img src="https://img.shields.io/badge/license-MIT-blue" alt="License"></a>
  <a href="https://github.com/SBAI-Youness/Stock_Management_System/releases"><img src="https://img.shields.io/badge/version-v1.0.0-blue" alt="Version"></a>
  <a href="https://en.wikipedia.org/wiki/C_(programming_language)"><img src="https://img.shields.io/badge/language-C-blue" alt="Language"></a>
  <a href="https://github.com/SBAI-Youness/Stock_Management_System/pulls"><img src="https://img.shields.io/badge/PRs-welcome-brightgreen" alt="Contributions"></a>
</p>


## Overview
The **Stock Management System** is a console-based application developed in C for managing inventory efficiently. It provides functionalities for handling product details and storing data persistently in CSV files. Designed with a user-friendly console interface, it simplifies inventory management tasks.


## Features
- **User Authentication**: Log in with a username to access the application.
- **Product Management**:
  - Add new products.
  - Modify existing products.
  - Delete products.
- **Product Listing**:
  View products with details like:
  - Product ID
  - Name
  - Description
  - Username (associated with the product)
  - Unit Price
  - Stock Quantity
  - Stock Alert Threshold
  - Last Stock-In Date
  - Last Stock-Out Date
- **Search & Sort**:
  - Search products by name or user.
  - Sort products by name or unit price.
- **Persistent Storage**:
  Save product data in a CSV file for future access.
- **Security Features**:
  - Password Hashing with OpenSSL: User passwords are securely salted and hashed before storage to enhance protection against unauthorized access.
  - Brute Force Prevention: Implemented mechanisms to prevent brute force attacks, ensuring the security of user accounts.
  - DDoS Protection: Measures in place to mitigate Distributed Denial of Service attacks, ensuring the availability of the system.

  - Session Management: Secure session handling to prevent session hijacking and fixation.


## Technologies Used
- **Programming Language**: C
- **Storage**: File-based (CSV)
- **Interface**: Console


## Prerequisites

To run this project, you need to install:

1. **MinGW-w64**: C compiler for Windows
   - Download from [MinGW-w64 releases](https://github.com/niXman/mingw-builds-binaries/releases)

2. **OpenSSL**: Required for security features
   - Download from [OpenSSL for Windows](https://slproweb.com/products/Win32OpenSSL.html)

3. **Make**: For building the project
   - Download from [Make for Windows](https://gnuwin32.sourceforge.net/packages/make.htm)

After installation, verify everything works by running:
```bash
gcc --version
openssl version
make --version
windres --version
```


## How to Build and Run

1. Clone the repository:
   ```bash
   git clone https://github.com/SBAI-Youness/Stock_Management_System.git
   ```
2. Navigate to the project directory:
   ```bash
   cd ./Stock_Management_System
   ```
3. Build the project:
   ```bash
   make
   ```
4. Navigate to the build directory and run the application:
   ```bash
   cd build
   .\main.exe
   ```


## License
Stock Management System is released under the MIT License. See the [LICENSE](LICENSE) file for more details.