#include <bits/stdc++.h>
using namespace std;

// ==============================================================================

/*
1) HIGHER LEVEL ARCHITECTURE
      +----------------------+
      |        main()        |
      |  (Program Startup)   |
      +----------+-----------+
                 |
                 v
      +----------------------+
      |        BankUI        |
      | (Menus/Input/Output) |
      +----------+-----------+
                 |
                 v
      +----------------------+
      |      BankSystem      |
      |   (System Manager)   |
      +----------+-----------+
                 |
                 v
      +----------------------+
      |     BankAccount      |
      | (Single Account Unit)|
      +----------------------+

2) RESPONSIBILITIES
      main()
          - Starts the application only.

      BankUI
          - Shows menus.
          - Reads user input.
          - Prints success/failure messages.

      BankSystem
          - Adds accounts.
          - Finds accounts.
          - Deletes accounts.
          - Tracks total accounts.

      BankAccount
          - Stores account data.
          - Handles deposit and withdrawal rules.
*/

// ==============================================================================

class BankAccount {
 private:
  int accountNumber;
  string accountName;
  double balance;

  static int nextAccountNumber;

 public:
  BankAccount(string name, double initialBalance)
      : accountName(name), balance(initialBalance) {
    accountNumber = nextAccountNumber++;
  }

  int getAccountNumber() const { return accountNumber; }
  string getAccountName() const { return accountName; }
  double getBalance() const { return balance; }

  enum class DepositStatus { SUCCESS, INVALID_AMOUNT };

  DepositStatus deposit(double amount) {
    if (amount <= 0) return DepositStatus::INVALID_AMOUNT;
    balance += amount;
    return DepositStatus::SUCCESS;
  }

  enum class WithdrawStatus { SUCCESS, INSUFFICIENT_BALANCE, INVALID_AMOUNT };

  WithdrawStatus withdraw(double amount) {
    if (amount <= 0) return WithdrawStatus::INVALID_AMOUNT;
    if (amount > balance) return WithdrawStatus::INSUFFICIENT_BALANCE;
    balance -= amount;
    return WithdrawStatus::SUCCESS;
  }
};

int BankAccount::nextAccountNumber = 1001;

class BankSystem {
 private:
  vector<BankAccount> accounts;

 public:
  enum class CreateAccountStatus { SUCCESS, INVALID_INITIAL_BALANCE };

  CreateAccountStatus createAccount(const string& name, double initialBalance,
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
    for (int i = 0; i < accounts.size(); i++) {
      if (accounts[i].getAccountNumber() == accountNumber) {
        return i;
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

class BankUI {
 private:
  BankSystem bankSystem;

  void showMainMenu() const {
    cout << "\n====== BANK SYSTEM ======\n";
    cout << "1. Create Account\n";
    cout << "2. Access Account\n";
    cout << "3. Delete Account\n";
    cout << "4. Exit\n";
    cout << "Enter choice: ";
  }

  void showAccountMenu() const {
    cout << "\n--- Account Menu ---\n";
    cout << "1. Deposit\n";
    cout << "2. Withdraw\n";
    cout << "3. Display\n";
    cout << "4. Back\n";
    cout << "Enter choice: ";
  }

  void createAccount() {
    string name;
    double initialBalance;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter account name: ";
    getline(cin, name);

    cout << "Enter initial balance: ";
    cin >> initialBalance;

    int accountNumber;
    auto result =
        bankSystem.createAccount(name, initialBalance, accountNumber);

    if (result == BankSystem::CreateAccountStatus::INVALID_INITIAL_BALANCE) {
      cout << "Initial balance cannot be negative!\n";
      return;
    }

    cout << "\nAccount created successfully!\n";
    cout << "Your Account Number is: " << accountNumber << endl;
  }

  void accessAccount() {
    if (bankSystem.isEmpty()) {
      cout << "No accounts exist. Create one first.\n";
      return;
    }

    int accountNumber;
    cout << "Enter Account Number: ";
    cin >> accountNumber;

    int index = bankSystem.getAccountIndex(accountNumber);

    if (index == -1) {
      cout << "Account not found!\n";
      return;
    }

    BankAccount& account = bankSystem.getAccountByIndex(index);
    handleAccountMenu(account);
  }

  void handleAccountMenu(BankAccount& account) {
    int subChoice;

    while (true) {
      showAccountMenu();
      cin >> subChoice;

      if (subChoice == 1) {
        deposit(account);
      } else if (subChoice == 2) {
        withdraw(account);
      } else if (subChoice == 3) {
        displayAccount(account);
      } else if (subChoice == 4) {
        break;
      } else {
        cout << "Invalid choice!\n";
      }
    }
  }

  void deposit(BankAccount& account) {
    double amount;
    cout << "Enter amount to deposit: ";
    cin >> amount;

    auto result = account.deposit(amount);

    if (result == BankAccount::DepositStatus::SUCCESS)
      cout << "Deposit successful!\n";
    else
      cout << "Invalid amount!\n";
  }

  void withdraw(BankAccount& account) {
    double amount;
    cout << "Enter amount to withdraw: ";
    cin >> amount;

    auto result = account.withdraw(amount);

    if (result == BankAccount::WithdrawStatus::INVALID_AMOUNT)
      cout << "Invalid amount!\n";
    else if (result == BankAccount::WithdrawStatus::INSUFFICIENT_BALANCE)
      cout << "Insufficient balance!\n";
    else
      cout << "Withdrawal successful!\n";
  }

  void displayAccount(const BankAccount& account) const {
    cout << "\n--- Account Details ---\n";
    cout << "Account Name: " << account.getAccountName() << endl;
    cout << "Account Number: " << account.getAccountNumber() << endl;
    cout << "Balance: " << account.getBalance() << endl;
  }

  void deleteAccount() {
    if (bankSystem.isEmpty()) {
      cout << "No accounts exist.\n";
      return;
    }

    int accountNumber;
    cout << "Enter Account Number to delete: ";
    cin >> accountNumber;

    char confirm;
    cout << "Are you sure you want to delete this account? (Y/N): ";
    cin >> confirm;

    if (confirm == 'Y' || confirm == 'y') {
      if (bankSystem.deleteAccount(accountNumber)) {
        cout << "Account deleted successfully!\n";
        cout << "Total accounts left: " << bankSystem.totalAccounts() << endl;
      } else {
        cout << "Account not found!\n";
      }
    } else {
      cout << "Deletion cancelled.\n";
    }
  }

 public:
  void run() {
    int choice;

    while (true) {
      showMainMenu();
      cin >> choice;

      if (choice == 1) {
        createAccount();
      } else if (choice == 2) {
        accessAccount();
      } else if (choice == 3) {
        deleteAccount();
      } else if (choice == 4) {
        break;
      } else {
        cout << "Invalid choice!\n";
      }
    }
  }
};

int main() {
  BankUI bankUI;
  bankUI.run();

  return 0;
}
