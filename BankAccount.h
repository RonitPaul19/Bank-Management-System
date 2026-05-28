#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H

#include <string>

class BankAccount {
 private:
  int accountNumber;
  std::string accountName;
  double balance;

  inline static int nextAccountNumber = 1001;

 public:
  enum class DepositStatus { SUCCESS, INVALID_AMOUNT };
  enum class WithdrawStatus { SUCCESS, INSUFFICIENT_BALANCE, INVALID_AMOUNT };

  // Normal constructor
  BankAccount(std::string name, double initialBalance)
      : accountName(name), balance(initialBalance) {
    accountNumber = nextAccountNumber++;
  }

  // Constructor for loading from file
  BankAccount(int accNumber,
              std::string name,
              double initialBalance)
      : accountNumber(accNumber),
        accountName(name),
        balance(initialBalance) {}

  int getAccountNumber() const { return accountNumber; }

  std::string getAccountName() const { return accountName; }

  double getBalance() const { return balance; }

  DepositStatus deposit(double amount) {
    if (amount <= 0) return DepositStatus::INVALID_AMOUNT;

    balance += amount;
    return DepositStatus::SUCCESS;
  }

  WithdrawStatus withdraw(double amount) {
    if (amount <= 0) return WithdrawStatus::INVALID_AMOUNT;

    if (amount > balance) {
      return WithdrawStatus::INSUFFICIENT_BALANCE;
    }

    balance -= amount;
    return WithdrawStatus::SUCCESS;
  }

  static void setNextAccountNumber(int nextNumber) {
    nextAccountNumber = nextNumber;
  }
};

#endif
