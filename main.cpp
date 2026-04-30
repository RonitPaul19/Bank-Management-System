// BANK MANAGEMENT SYSTEM
#include <bits/stdc++.h>
using namespace std;

class bank {
 private:
  int accountNumber;
  string accountName;
  double balance;

 public:
  bank(string accName, int accNo, double initialBalance) {
    accountName = accName;
    accountNumber = accNo;
    balance = initialBalance;
  }

  void deposit(double amount) {
    if (amount <= 0) {
      cout << "Invalid amount!\n";
      return;
    }
    balance += amount;
    cout << "Successfully deposited INR " << amount << " in the bank";
  }

  void withdraw(double amount) {
    if (amount <= 0) {
      cout << "Invalid amount!\n";
      return;
    }
    if (amount > balance) {
      cout << "Insufficient balance!\n";
    } else {
      balance -= amount;
      cout << "Successfully withdrawn INR " << amount << endl;
    }
  }

  void displayAccount() const {  // const keyword is used so that it does not
                                 // change any data of the class
    cout << "\n--- Account Details ---\n";
    cout << "Account Name: " << accountName << endl;
    cout << "Account Number: " << accountNumber << endl;
    cout << "Balance: " << balance << endl;
  }
};

// Create account and return object
bank createBank() {
  char ans;
  cout << "Do you want to create a bank account (Y/N): ";
  cin >> ans;

  string accName;
  int accNo;
  double initialBalance;

  if (ans == 'y' || ans == 'Y') {
    cout << "Enter account name: ";
    cin >> accName;

    cout << "Enter account number: ";
    cin >> accNo;

    cout << "Enter initial balance: ";
    cin >> initialBalance;

    return bank(accName, accNo, initialBalance);
  }

  // temporary fallback
  return bank("Default", 0, 0);
}

int main() {
  bank user = createBank();

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
      cout << "What amount do you want to deposit : ";
      cin >> amount;
      user.deposit(amount);
    } else if (choice == 2) {
      double amount;
      cout << "What amount do you want to withdraw : ";
      cin >> amount;
      user.withdraw(amount);
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
