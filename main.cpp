// BANK MANAGEMENT SYSTEM (Multi-User + Auto Account Number + Delete Feature)

#include <bits/stdc++.h>
using namespace std;

class BankAccount {
 private:
  int accountNumber;
  string accountName;
  double balance;

  static int nextAccountNumber;

 public:
  BankAccount(string accName, double initialBalance)
      : accountName(accName), balance(initialBalance) {
    accountNumber = nextAccountNumber++;
  }

  int getAccountNumber() const {
    return accountNumber;
  }

  // Deposit status
  enum Status_for_deposit {
    D_SUCCESS,
    D_INVALID
  };

  Status_for_deposit deposit(double amount) {
    if (amount <= 0) return D_INVALID;
    balance += amount;
    return D_SUCCESS;
  }

  // Withdraw status
  enum Status_for_withdraw {
    W_SUCCESS,
    W_INSUFFICIENT_BALANCE,
    W_INVALID_AMOUNT
  };

  Status_for_withdraw withdraw(double amount) {
    if (amount <= 0) return W_INVALID_AMOUNT;
    if (amount > balance) return W_INSUFFICIENT_BALANCE;
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

// Initialize static variable
int BankAccount::nextAccountNumber = 1001;

// Create account
BankAccount createBank() {
  string accName;
  double initialBalance;

  cin.ignore();

  cout << "Enter account name: ";
  getline(cin, accName);

  cout << "Enter initial balance: ";
  cin >> initialBalance;

  BankAccount newAcc(accName, initialBalance);

  cout << "\nAccount created successfully!\n";
  cout << "Your Account Number is: " << newAcc.getAccountNumber() << endl;

  return newAcc;
}

int main() {
  vector<BankAccount> users;

  int choice;

  while (true) {
    cout << "\n====== BANK SYSTEM ======\n";
    cout << "1. Create Account\n";
    cout << "2. Access Account\n";
    cout << "3. Delete Account\n";
    cout << "4. Exit\n";
    cout << "Enter choice: ";
    cin >> choice;

    // CREATE ACCOUNT
    if (choice == 1) {
      users.push_back(createBank());
    }

    // ACCESS ACCOUNT
    else if (choice == 2) {
      if (users.empty()) {
        cout << "No accounts exist. Create one first.\n";
        continue;
      }

      int accNo;
      cout << "Enter Account Number: ";
      cin >> accNo;

      int index = -1;

      for (int i = 0; i < users.size(); i++) {
        if (users[i].getAccountNumber() == accNo) {
          index = i;
          break;
        }
      }

      if (index == -1) {
        cout << "Account not found!\n";
        continue;
      }

      int subChoice;

      while (true) {
        cout << "\n--- Account Menu ---\n";
        cout << "1. Deposit\n";
        cout << "2. Withdraw\n";
        cout << "3. Display\n";
        cout << "4. Back\n";
        cout << "Enter choice: ";
        cin >> subChoice;

        if (subChoice == 1) {
          double amount;
          cout << "Enter amount to deposit: ";
          cin >> amount;

          auto result = users[index].deposit(amount);

          if (result == BankAccount::D_SUCCESS)
            cout << "Deposit successful!\n";
          else
            cout << "Invalid amount!\n";
        }

        else if (subChoice == 2) {
          double amount;
          cout << "Enter amount to withdraw: ";
          cin >> amount;

          auto result = users[index].withdraw(amount);

          if (result == BankAccount::W_INVALID_AMOUNT)
            cout << "Invalid amount!\n";
          else if (result == BankAccount::W_INSUFFICIENT_BALANCE)
            cout << "Insufficient balance!\n";
          else
            cout << "Withdrawal successful!\n";
        }

        else if (subChoice == 3) {
          users[index].displayAccount();
        }

        else if (subChoice == 4) {
          break;
        }

        else {
          cout << "Invalid choice!\n";
        }
      }
    }

    // DELETE ACCOUNT
    else if (choice == 3) {
      if (users.empty()) {
        cout << "No accounts exist.\n";
        continue;
      }

      int accNo;
      cout << "Enter Account Number to delete: ";
      cin >> accNo;

      int index = -1;

      for (int i = 0; i < users.size(); i++) {
        if (users[i].getAccountNumber() == accNo) {
          index = i;
          break;
        }
      }

      if (index == -1) {
        cout << "Account not found!\n";
        continue;
      }

      char confirm;
      cout << "Are you sure you want to delete this account? (Y/N): ";
      cin >> confirm;

      if (confirm == 'Y' || confirm == 'y') {
        users.erase(users.begin() + index);
        cout << "Account deleted successfully!\n";
        cout << "Total accounts left: " << users.size() << endl;
      } else {
        cout << "Deletion cancelled.\n";
      }
    }

    else if (choice == 4) {
      break;
    }

    else {
      cout << "Invalid choice!\n";
    }
  }

  return 0;
}
