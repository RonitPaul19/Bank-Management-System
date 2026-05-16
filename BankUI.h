#ifndef BANK_UI_H
#define BANK_UI_H

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

  void showAccountMenu() const {
    std::cout << "\n--- Account Menu ---\n";
    std::cout << "1. Deposit\n";
    std::cout << "2. Withdraw\n";
    std::cout << "3. Display\n";
    std::cout << "4. Back\n";
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

  void createAccount() {
    std::string name;
    double initialBalance;

    std::cout << "Enter account name: ";
    std::getline(std::cin, name);

    initialBalance = readDouble("Enter initial balance: ");

    int accountNumber;
    auto result = bankSystem.createAccount(name, initialBalance, accountNumber);

    if (result == BankSystem::CreateAccountStatus::INVALID_INITIAL_BALANCE) {
      std::cout << "Initial balance cannot be negative!\n";
      return;
    }

    std::cout << "\nAccount created successfully!\n";
    std::cout << "Your Account Number is: " << accountNumber << std::endl;
  }

  void accessAccount() {
    if (bankSystem.isEmpty()) {
      std::cout << "No accounts exist. Create one first.\n";
      return;
    }

    int accountNumber;
    accountNumber = readInt("Enter Account Number: ");

    int index = bankSystem.getAccountIndex(accountNumber);

    if (index == -1) {
      std::cout << "Account not found!\n";
      return;
    }

    BankAccount& account = bankSystem.getAccountByIndex(index);
    handleAccountMenu(account);
  }

  void handleAccountMenu(BankAccount& account) {
    int subChoice;

    while (true) {
      showAccountMenu();
      subChoice = readInt("Enter choice: ");

      if (subChoice == 1) {
        deposit(account);
      } else if (subChoice == 2) {
        withdraw(account);
      } else if (subChoice == 3) {
        displayAccount(account);
      } else if (subChoice == 4) {
        break;
      } else {
        std::cout << "Invalid choice!\n";
      }
    }
  }

  void deposit(BankAccount& account) {
    double amount;
    amount = readDouble("Enter amount to deposit: ");

    auto result = account.deposit(amount);

    if (result == BankAccount::DepositStatus::SUCCESS)
      std::cout << "Deposit successful!\n";
    else
      std::cout << "Invalid amount!\n";
  }

  void withdraw(BankAccount& account) {
    double amount;
    amount = readDouble("Enter amount to withdraw: ");

    auto result = account.withdraw(amount);

    if (result == BankAccount::WithdrawStatus::INVALID_AMOUNT)
      std::cout << "Invalid amount!\n";
    else if (result == BankAccount::WithdrawStatus::INSUFFICIENT_BALANCE)
      std::cout << "Insufficient balance!\n";
    else
      std::cout << "Withdrawal successful!\n";
  }

  void displayAccount(const BankAccount& account) const {
    std::cout << "\n--- Account Details ---\n";
    std::cout << "Account Name: " << account.getAccountName() << std::endl;
    std::cout << "Account Number: " << account.getAccountNumber() << std::endl;
    std::cout << "Balance: " << account.getBalance() << std::endl;
  }

  void deleteAccount() {
    if (bankSystem.isEmpty()) {
      std::cout << "No accounts exist.\n";
      return;
    }

    int accountNumber;
    accountNumber = readInt("Enter Account Number to delete: ");

    char confirm;
    std::cout << "Are you sure you want to delete this account? (Y/N): ";
    std::cin >> confirm;

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
      showMainMenu();
      choice = readInt("Enter choice: ");

      if (choice == 1) {
        createAccount();
      } else if (choice == 2) {
        accessAccount();
      } else if (choice == 3) {
        deleteAccount();
      } else if (choice == 4) {
        break;
      } else {
        std::cout << "Invalid choice!\n";
      }
    }
  }
};

#endif
