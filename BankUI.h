#ifndef BANK_UI_H
#define BANK_UI_H

#include <cctype>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

#include "BankAccount.h"
#include "BankSystem.h"

class BankUI {
 private:
  BankSystem bankSystem;

  void showMainMenu() const {
    std::cout << "\n====== BANK SYSTEM ======\n";
    std::cout << "1. Create Account\n";
    std::cout << "2. Access Account\n";
    std::cout << "3. Delete Account\n";
    std::cout << "4. Exit\n";
  }

  void showAccountMenu(const BankAccount& account) const {
    std::cout << "\n--- Account Menu ---\n";
    std::cout << "Account Holder: " << account.getAccountName() << std::endl;
    std::cout << "Account Number: " << account.getAccountNumber() << std::endl;
    showBalance("Current Balance: ", account.getBalance());
    std::cout << "\n";
    std::cout << "1. Deposit\n";
    std::cout << "2. Withdraw\n";
    std::cout << "3. Display\n";
    std::cout << "4. Back\n";
  }

  void clearScreen() const {
#ifdef _WIN32
    std::system("cls");
#else
    std::cout << "\033[2J\033[H";
#endif
  }

  void pauseScreen() {
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
  }

  void showBalance(const std::string& label, double balance) const {
    std::cout << label << std::fixed << std::setprecision(2) << balance
              << std::endl;
  }

  int readInt(const std::string& message) {
    int value;

    while (true) {
      std::cout << message;

      if (std::cin >> value) {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return value;
      }

      std::cout << "Invalid input! Please enter a number.\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  double readDouble(const std::string& message) {
    double value;

    while (true) {
      std::cout << message;

      if (std::cin >> value) {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return value;
      }

      std::cout << "Invalid input! Please enter a valid amount.\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  bool isValidPin(const std::string& pin) const {
    if (pin.length() != 4) {
      return false;
    }

    for (char digit : pin) {
      if (!std::isdigit(static_cast<unsigned char>(digit))) {
        return false;
      }
    }

    return true;
  }

  std::string readPin(const std::string& message) {
    std::string pin;

    while (true) {
      std::cout << message;
      std::getline(std::cin, pin);

      if (isValidPin(pin)) {
        return pin;
      }

      std::cout << "PIN must be exactly 4 digits.\n";
    }
  }

  bool authenticateAccount(const BankAccount& account) {
    std::string pin = readPin("Enter account PIN: ");

    if (!account.verifyPin(pin)) {
      std::cout << "Incorrect PIN!\n";
      return false;
    }

    return true;
  }

  void createAccount() {
    std::string name;
    double initialBalance;
    std::string pin;

    std::cout << "Enter account name: ";
    std::getline(std::cin, name);

    initialBalance = readDouble("Enter initial balance: ");
    pin = readPin("Create a 4-digit PIN: ");

    int accountNumber;
    auto result =
        bankSystem.createAccount(name, initialBalance, pin, accountNumber);

    if (result == BankSystem::CreateAccountStatus::INVALID_INITIAL_BALANCE) {
      std::cout << "Initial balance cannot be negative!\n";
      return;
    }

    if (result == BankSystem::CreateAccountStatus::INVALID_PIN) {
      std::cout << "Invalid PIN!\n";
      return;
    }

    std::cout << "\nAccount created successfully!\n";
    std::cout << "Your Account Number is: " << accountNumber << std::endl;
  }

  void accessAccount() {
    if (bankSystem.isEmpty()) {
      std::cout << "No accounts exist. Create one first.\n";
      pauseScreen();
      return;
    }

    int accountNumber;
    accountNumber = readInt("Enter Account Number: ");

    int index = bankSystem.getAccountIndex(accountNumber);

    if (index == -1) {
      std::cout << "Account not found!\n";
      pauseScreen();
      return;
    }

    BankAccount& account = bankSystem.getAccountByIndex(index);

    if (!authenticateAccount(account)) {
      pauseScreen();
      return;
    }

    handleAccountMenu(account);
  }

  void handleAccountMenu(BankAccount& account) {
    int subChoice;

    while (true) {
      clearScreen();
      showAccountMenu(account);
      subChoice = readInt("Enter choice: ");

      if (subChoice == 1) {
        deposit(account);
        pauseScreen();
      } else if (subChoice == 2) {
        withdraw(account);
        pauseScreen();
      } else if (subChoice == 3) {
        displayAccount(account);
        pauseScreen();
      } else if (subChoice == 4) {
        break;
      } else {
        std::cout << "Invalid choice!\n";
        pauseScreen();
      }
    }
  }

  void deposit(BankAccount& account) {
    double amount;

    showBalance("\nCurrent balance: ", account.getBalance());
    amount = readDouble("Enter amount to deposit: ");

    auto result = account.deposit(amount);

    if (result == BankAccount::DepositStatus::SUCCESS) {
      bankSystem.saveToFile();

      std::cout << "Deposit successful!\n";

      showBalance("Updated balance: ", account.getBalance());
    } else {
      std::cout << "Invalid amount!\n";
      showBalance("Balance remains: ", account.getBalance());
    }
  }

  void withdraw(BankAccount& account) {
    double amount;

    showBalance("\nCurrent balance: ", account.getBalance());
    amount = readDouble("Enter amount to withdraw: ");

    auto result = account.withdraw(amount);

    if (result == BankAccount::WithdrawStatus::INVALID_AMOUNT) {
      std::cout << "Invalid amount!\n";
      showBalance("Balance remains: ", account.getBalance());
    } else if (result == BankAccount::WithdrawStatus::INSUFFICIENT_BALANCE) {
      std::cout << "Insufficient balance!\n";
      showBalance("Available balance: ", account.getBalance());
    } else {
      bankSystem.saveToFile();
      std::cout << "Withdrawal successful!\n";
      showBalance("Updated balance: ", account.getBalance());
    }
  }

  void displayAccount(const BankAccount& account) const {
    std::cout << "\n--- Account Details ---\n";
    std::cout << "Account Name: " << account.getAccountName() << std::endl;
    std::cout << "Account Number: " << account.getAccountNumber() << std::endl;
    showBalance("Balance: ", account.getBalance());
  }

  void deleteAccount() {
    if (bankSystem.isEmpty()) {
      std::cout << "No accounts exist.\n";
      return;
    }

    int accountNumber;
    accountNumber = readInt("Enter Account Number to delete: ");

    int index = bankSystem.getAccountIndex(accountNumber);

    if (index == -1) {
      std::cout << "Account not found!\n";
      return;
    }

    BankAccount& account = bankSystem.getAccountByIndex(index);

    if (!authenticateAccount(account)) {
      return;
    }

    char confirm;
    std::cout << "Are you sure you want to delete this account? (Y/N): ";
    std::cin >> confirm;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (confirm == 'Y' || confirm == 'y') {
      if (bankSystem.deleteAccount(accountNumber)) {
        std::cout << "Account deleted successfully!\n";
        std::cout << "Total accounts left: " << bankSystem.totalAccounts()
                  << std::endl;
      } else {
        std::cout << "Account not found!\n";
      }
    } else {
      std::cout << "Deletion cancelled.\n";
    }
  }

 public:
  void run() {
    int choice;

    while (true) {
      clearScreen();
      showMainMenu();
      choice = readInt("Enter choice: ");

      if (choice == 1) {
        createAccount();
        pauseScreen();
      } else if (choice == 2) {
        accessAccount();
      } else if (choice == 3) {
        deleteAccount();
        pauseScreen();
      } else if (choice == 4) {
        break;
      } else {
        std::cout << "Invalid choice!\n";
        pauseScreen();
      }
    }
  }
};

#endif
