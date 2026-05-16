#ifndef BANK_SYSTEM_H
#define BANK_SYSTEM_H

#include <cstddef>
#include <string>
#include <vector>

#include "BankAccount.h"

class BankSystem {
 private:
  std::vector<BankAccount> accounts;

 public:
  enum class CreateAccountStatus { SUCCESS, INVALID_INITIAL_BALANCE };

  CreateAccountStatus createAccount(const std::string& name,
                                    double initialBalance,
                                    int& createdAccountNumber) {
    if (initialBalance < 0) {
      return CreateAccountStatus::INVALID_INITIAL_BALANCE;
    }

    BankAccount account(name, initialBalance);
    createdAccountNumber = account.getAccountNumber();
    accounts.push_back(account);
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

  BankAccount& getAccountByIndex(int index) { return accounts[index]; }

  bool deleteAccount(int accountNumber) {
    int index = getAccountIndex(accountNumber);
    if (index == -1) return false;

    accounts.erase(accounts.begin() + index);
    return true;
  }

  bool isEmpty() const { return accounts.empty(); }

  int totalAccounts() const { return accounts.size(); }
};

#endif
