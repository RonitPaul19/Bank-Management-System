#ifndef BANK_SYSTEM_H
#define BANK_SYSTEM_H

#include <cstddef>
#include <fstream>
#include <string>
#include <vector>

#include "BankAccount.h"

class BankSystem {
 private:
  std::vector<BankAccount> accounts;

 public:
  enum class CreateAccountStatus {
    SUCCESS,
    INVALID_INITIAL_BALANCE
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
           << account.getBalance() << "\n";
    }

    file.close();
  }

  void loadFromFile() {
    std::ifstream file("accounts.txt");

    int accNumber;
    std::string name;
    double balance;

    int highestAccountNumber = 1000;

    while (file >> accNumber >> name >> balance) {
      BankAccount account(accNumber, name, balance);

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
                                    int& createdAccountNumber) {
    if (initialBalance < 0) {
      return CreateAccountStatus::INVALID_INITIAL_BALANCE;
    }

    BankAccount account(name, initialBalance);

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
