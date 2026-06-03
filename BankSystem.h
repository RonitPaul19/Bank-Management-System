#ifndef BANK_SYSTEM_H
#define BANK_SYSTEM_H

#include <cctype>
#include <cstddef>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "BankAccount.h"

class BankSystem {
 private:
  std::vector<BankAccount> accounts;

  static bool isValidPin(const std::string& pin) {
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

 public:
  enum class CreateAccountStatus {
    SUCCESS,
    INVALID_INITIAL_BALANCE,
    INVALID_PIN
  };

  // Constructor
  BankSystem() {
    loadFromFile();
  }

  void saveToFile() {
    std::ofstream file("accounts.txt");

    for (const auto& account : accounts) {
      file << account.getAccountNumber() << " "
           << account.getAccountName() << " "
           << account.getBalance() << " "
           << account.getPin() << "\n";
    }

    file.close();
  }

  void loadFromFile() {
    std::ifstream file("accounts.txt");

    std::string line;

    int highestAccountNumber = 1000;

    while (std::getline(file, line)) {
      std::istringstream lineStream(line);

      int accNumber;
      std::string name;
      double balance;
      std::string pin;

      if (!(lineStream >> accNumber >> name >> balance)) {
        continue;
      }

      if (!(lineStream >> pin) || !isValidPin(pin)) {
        pin = "0000";
      }

      BankAccount account(accNumber, name, balance, pin);

      accounts.push_back(account);

      if (accNumber > highestAccountNumber) {
        highestAccountNumber = accNumber;
      }
    }

    BankAccount::setNextAccountNumber(highestAccountNumber + 1);

    file.close();
  }

  CreateAccountStatus createAccount(const std::string& name,
                                    double initialBalance,
                                    const std::string& pin,
                                    int& createdAccountNumber) {
    if (initialBalance < 0) {
      return CreateAccountStatus::INVALID_INITIAL_BALANCE;
    }

    if (!isValidPin(pin)) {
      return CreateAccountStatus::INVALID_PIN;
    }

    BankAccount account(name, initialBalance, pin);

    createdAccountNumber = account.getAccountNumber();

    accounts.push_back(account);

    saveToFile();

    return CreateAccountStatus::SUCCESS;
  }

  int getAccountIndex(int accountNumber) {
    for (std::size_t i = 0; i < accounts.size(); i++) {
      if (accounts[i].getAccountNumber() == accountNumber) {
        return static_cast<int>(i);
      }
    }

    return -1;
  }

  BankAccount& getAccountByIndex(int index) {
    return accounts[index];
  }

  bool deleteAccount(int accountNumber) {
    int index = getAccountIndex(accountNumber);

    if (index == -1) {
      return false;
    }

    accounts.erase(accounts.begin() + index);

    saveToFile();

    return true;
  }

  bool isEmpty() const {
    return accounts.empty();
  }

  int totalAccounts() const {
    return accounts.size();
  }
};

#endif
