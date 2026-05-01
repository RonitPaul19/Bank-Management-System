// BANK MANAGEMENT SYSTEM
#include <bits/stdc++.h>
using namespace std;

class BankAccount {
 private:
  int accountNumber;
  string accountName;
  double balance;

 public:
  BankAccount(string accName, int accNo, double initialBalance)
      : accountName(accName), accountNumber(accNo), balance(initialBalance) {}

  // Status for deposit
  enum Status_for_deposit {
    D_SUCCESS,
    D_INVALID
  };

  Status_for_deposit deposit(double amount) {
    if (amount <= 0) {
      return D_INVALID;
    }
    balance += amount;
    return D_SUCCESS;
  }

  // Status for withdraw
  enum Status_for_withdraw {
    W_SUCCESS,
    W_INSUFFICIENT_BALANCE,
    W_INVALID_AMOUNT
  };

  Status_for_withdraw withdraw(double amount) {
    if (amount <= 0) {
      return W_INVALID_AMOUNT;
    }
    if (amount > balance) {
      return W_INSUFFICIENT_BALANCE;
    }
    balance -= amount;
    return W_SUCCESS;
  }

  void displayAccount() const {
    cout << "\n--- Account Details ---\n";
    cout << "Account Name: " << accountName << endl;
    cout << "Account Number: " << accountNumber << endl;
    cout << "Balance: " << balance << endl;
  }
};

// Create account and return object
BankAccount createBank() {
  char ans;
  cout << "Do you want to create a bank account (Y/N): ";
  cin >> ans;

  string accName;
  int accNo;
  double initialBalance;

  if (ans == 'y' || ans == 'Y') {
    cin.ignore();  // FIX: clears buffer before getline

    cout << "Enter account name: ";
    getline(cin, accName);

    cout << "Enter account number: ";
    cin >> accNo;

    cout << "Enter initial balance: ";
    cin >> initialBalance;

    return BankAccount(accName, accNo, initialBalance);
  }

  return BankAccount("Default", 0, 0);
}

int main() {
  BankAccount user = createBank();

  int choice;

  while (true) {
    cout << "\n1. Deposit\n";
    cout << "2. Withdraw\n";
    cout << "3. Display information\n";
    cout << "4. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == 1) {
      double amount;
      cout << "What amount do you want to deposit: ";
      cin >> amount;

      BankAccount::Status_for_deposit result = user.deposit(amount);

      if (result == BankAccount::D_SUCCESS) {
        cout << "Amount " << amount << " deposited successfully!\n";
      } else {
        cout << "Invalid amount!\n";
      }

    } else if (choice == 2) {
      double amount;
      cout << "What amount do you want to withdraw: ";
      cin >> amount;

      BankAccount::Status_for_withdraw result = user.withdraw(amount);

      if (result == BankAccount::W_INVALID_AMOUNT) {
        cout << "Invalid amount!\n";
      } else if (result == BankAccount::W_INSUFFICIENT_BALANCE) {
        cout << "Insufficient bank balance!\n";
      } else if (result == BankAccount::W_SUCCESS) {
        cout << "Amount " << amount << " successfully withdrawn!\n";
      }

    } else if (choice == 3) {
      user.displayAccount();

    } else if (choice == 4) {
      break;

    } else {
      cout << "Invalid choice\n";
    }
  }

  return 0;
}
