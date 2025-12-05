#pragma once
#include <string>
#include <sstream>
#include <stdexcept>
using namespace std;

class BankAccount {
private:
    string accountNumber; // doit être unique dans le BankSystem
    double balance;

public:
    BankAccount() = default;

    // Constructeur principal
    BankAccount(const string& accNum, double bal = 0.0) : accountNumber(accNum), balance(bal) {}

    const string& getAccountNumber() const { return accountNumber; }
    double getBalance() const { return balance; }

    bool depot(double amount) {
        if (amount <= 0) return false;
        balance += amount;
        return true;
    }
    bool retrait(double amount) {
        if (amount <= 0) return false;
        if (amount > balance) return false;
        balance -= amount;
        return true;
    }

    // *************************************************************
    // ** NOUVEAU : Méthodes de sérialisation pour Client **
    // *************************************************************

    // Convertit l'objet en une ligne (pour l'écriture)
    string getLine(string sep = "#//#") const {
        // Format: accountNumber$$balance
        return accountNumber + sep + to_string(balance);
    }

    // Construit l'objet à partir d'une ligne (pour la lecture)
    static BankAccount fromLine(const string& line, string sep = "#//#") {
        size_t pos = line.find(sep);
        if (pos == string::npos) {
            throw runtime_error("Format de BankAccount invalide.");
        }

        string accNum = line.substr(0, pos);
        string balStr = line.substr(pos + sep.length());

        double bal;
        try {
            bal = stod(balStr);
        }
        catch (const std::exception& e) {
            throw runtime_error("Solde BankAccount invalide.");
        }

        return BankAccount(accNum, bal);
    }
};








//#pragma once
//class BankAccount {
//private:
//    string accountNumber; // doit être unique dans le BankSystem
//    double balance;
//public:
//    BankAccount() = default;
//    BankAccount(const string& accNum, double bal = 0.0) : accountNumber(accNum), balance(bal) {}
//
//    const string& getAccountNumber() const { return accountNumber; }
//    double getBalance() const { return balance; }
//
//    bool depot(double amount) {
//        if (amount <= 0) return false;
//        balance += amount;
//        return true;
//    }
//    bool retrait(double amount) {
//        if (amount <= 0) return false;
//        if (amount > balance) return false;
//        balance -= amount;
//        return true;
//    }
//};
