
#pragma once
#include <unordered_map>
#include "Person.h"
using namespace std;
#include <iostream>
#include "BankAccount.h"
#include <vector>
#include "Permission.h"
#include "Client.h"
#include "User.h" // Assurez-vous que User est inclus
#include <string>
#include "String.h" // Nécessaire pour String::split dans _LoadAllAccounts

class BankSystem {
private:
    unordered_map<string, Client> clientsById; // id interne -> client
    unordered_map<string, string> accountNumberToClientId; // accNum -> clientId (pour unicité)
    unordered_map<string, User> users; // Non utilisé dans cette logique, mais inclus

    uint64_t nextClientId = 1;

    string newClientId() {
        return "C" + to_string(nextClientId++);
    }  
     
     /*
     Cette fonction permet de charger les comptes stocker dans le fichier
     Accounts.txt, et permet de lier les accounts avec le clients par ID.
     
     */
    void _LoadAllAccounts() {
        // Accounts.txt Format: ID#//#Num Compte#//#Solde
        fstream file("Accounts.txt", ios::in);
        if (!file.is_open()) return;
        string line;
        while (getline(file, line)) {
            // Utilise le séparateur CLIENT_SEP="#//#"
            vector<string> data = String::split(line, "#//#");
            if (data.size() < 3) continue;

            string clientId = data[0];
            string accNum = data[1];
            double balance;
            try {
                balance = stod(data[2]);
            }
            catch (...) {
                continue; // Ignore les lignes avec un solde invalide
            }

            // Si le client existe en mémoire, ajouter le compte
            if (clientsById.count(clientId)) {
                clientsById.at(clientId).addAccount(BankAccount(accNum, balance));
                accountNumberToClientId[accNum] = clientId;
            }
        }
        file.close();
        cout << "\n\t\t\t\tComptes charges et liés aux clients en memoire.\n";
    }

    /*
     Cette fonction permet de sauvegarder les comptes dans le fichier Accounts.txt
     au format suvante: // Format: ID#//#Numero Compte#//#Solde
     */

    void _SaveAllAccounts() {
        fstream file("Accounts.txt", ios::out | ios::trunc); // TRUNC: Écrase le contenu
        if (!file.is_open()) {
            cerr << "ERREUR: Impossible d'ouvrir Accounts.txt pour la sauvegarde." << endl;
            return;
        }

        for (const auto& clientPair : clientsById) {
            const string& clientId = clientPair.first;
            for (const auto& acc : clientPair.second.getAccounts()) {
                string line = clientId + "#//#" + acc.getAccountNumber() + "#//#" + to_string(acc.getBalance());
                file << line << endl;
            }
        }
        file.close();
    }

    /*
     Cette fonction permet de charger les clients stocker dans le fichier
     Clients.txt,sans leurs comptes (pour la securite).

     */
    void _LoadAllClients() {
        // 1. Charger les clients (sans leurs comptes) depuis Clients.txt
        vector<Client> clients = Client::GetClientsFromFile();

        uint64_t maxId = 0;

        for (Client& client : clients) {
            string clientId = client.getId();
            if (clientId.empty()) continue;                         //si un ligne est vide, il passe a la ligne suivante
            clientsById.emplace(clientId, std::move(client));
                                                                    // Mise à jour de l'ID incrémental
            if (clientId.size() > 1 && clientId[0] == 'C') {
                try {
                    uint64_t currentId = stoull(clientId.substr(1));
                    if (currentId >= maxId) {
                        maxId = currentId;
                    }
                }
                catch (...) {}
            }
        }
        nextClientId = maxId + 1;

        cout << "\n\t\t\t\t" << clientsById.size() << " clients charges depuis Clients.txt.\n";

        // 2. Charger les comptes et les lier aux clients en mémoire
        _LoadAllAccounts();
    }

public:
    // --- CONSTRUCTEUR ---
    BankSystem() {
        _LoadAllClients();
    }

    // --- SAUVEGARDE GLOBALE ---
    void SaveAllClients() {
        vector<Client> clientsToSave;
        for (auto& pair : clientsById) {
            clientsToSave.push_back(pair.second);
        }

        // 1. Sauvegarder les données personnelles (Clients.txt) - Gère la suppression des clients
        Client::SaveAll(clientsToSave);

        // 2. Sauvegarder tous les comptes (Accounts.txt)
        _SaveAllAccounts();

        cout << "\n\t\t\t\t" << clientsToSave.size() << " clients et tous les comptes sauvegardes.\n";
    }

    // ---------------------- Méthodes de Gestion ---------------------- 

        // ---- Clients ----
    string addClient(const string& ln, const string& fn, const string& phone, const string& email) {
        string id = newClientId();
        Client newClient(id, ln, fn, phone, email, Client::enMode::enAddmode);

        clientsById.emplace(id, newClient);

        // Sauvegarde immédiate du client dans Clients.txt
        newClient.AddNew();

        return id;
    }

    bool removeClient(const string& clientId) {
        auto it = clientsById.find(clientId);
        if (it == clientsById.end()) return false;

        // 1. Marquer pour suppression
        it->second.MarkForDelete();

        // 2. Libérer les numéros de comptes associés (en mémoire)
        for (const auto& acc : it->second.getAccounts()) {
            accountNumberToClientId.erase(acc.getAccountNumber());
        }

        // 3. Supprimer de la mémoire
        clientsById.erase(it);

        // 4. Persister la suppression dans les deux fichiers
        SaveAllClients(); // Sauvegarde Clients.txt (sans le client) et Accounts.txt (sans ses comptes)

        return true;
    }

    Client* findClientById(const string& clientId) {
        auto it = clientsById.find(clientId);
        if (it == clientsById.end()) return nullptr;
        return &it->second;
    }

    vector<Client*> listAllClients() {
        vector<Client*> res;
        for (auto& p : clientsById) res.push_back(&p.second);
        return res;
    }

    // ---- Comptes ----
    bool accountNumberExists(const string& accNum) const {
        return accountNumberToClientId.find(accNum) != accountNumberToClientId.end();
    }

    bool addAccountToClient(const string& clientId, const string& accNum, double initialBalance = 0.0) {
        if (accountNumberExists(accNum)) return false;
        Client* c = findClientById(clientId);
        if (!c) return false;

        c->addAccount(BankAccount(accNum, initialBalance));
        accountNumberToClientId[accNum] = clientId;

        // Sauvegarde Accounts.txt pour persister le nouvel ajout
        _SaveAllAccounts();

        return true;
    }

    bool removeAccountFromClient(const string& clientId, const string& accNum) {
        Client* c = findClientById(clientId);
        if (!c) return false;

        bool ok = c->removeAccountByNumber(accNum);
        if (!ok) return false;

        accountNumberToClientId.erase(accNum);

        // Sauvegarde Accounts.txt pour persister la suppression
        _SaveAllAccounts();

        return true;
    }

    bool depositToAccount(const string& accNum, double amount) {
        auto itMap = accountNumberToClientId.find(accNum);
        if (itMap == accountNumberToClientId.end()) return false;
        Client* c = findClientById(itMap->second);
        if (!c) return false;
        BankAccount* a = c->findAccount(accNum);
        if (!a) return false;

        bool success = a->depot(amount);

        // Sauvegarde Accounts.txt pour persister le nouveau solde
        if (success) {
            _SaveAllAccounts();
        }

        return success;
    }

    bool withdrawFromAccount(const string& accNum, double amount) {
        auto itMap = accountNumberToClientId.find(accNum);
        if (itMap == accountNumberToClientId.end()) return false;
        Client* c = findClientById(itMap->second);
        if (!c) return false;
        BankAccount* a = c->findAccount(accNum);
        if (!a) return false;

        bool success = a->retrait(amount);

        // Sauvegarde Accounts.txt pour persister le nouveau solde
        if (success) {
            _SaveAllAccounts();
        }

        return success;
    }

    bool modifyClientInfo(const string& clientId, const string& ln, const string& fn, const string& phone, const string& email) {
        Client* c = findClientById(clientId);
        if (!c) return false;
        c->setLastName(ln);
        c->setFirstName(fn);
        c->setPhone(phone);
        c->setEmail(email);

        // La modification des informations personnelles nécessite une sauvegarde de Clients.txt
        SaveAllClients(); // Sauvegarde les deux fichiers pour être complet

        return true;
    }

    void printClientDetails(const string& clientId) {
        Client* c = findClientById(clientId);
        if (!c) {
            cout << "Client introuvable.\n";
            return;
        }
        c->printDetail();
    }



    // ... (Méthodes pour User::authenticate, etc., restent inchangées) ...
    User authenticate(const std::string& username, const std::string& password) {
                User user = User::Find(username);
        
                if (user.getUsername() == "") {
                    return User();
                }
        
                if (!user.checkPassword(password)) {
                    return User();
                }
        
                return user;
            }

};




























//#pragma once
//#include <unordered_map>
//#include "Person.h"
//using namespace std;
//#include <iostream>
//#include "BankAccount.h"
//#include <vector>
//#include "Permission.h"
//#include "Client.h"
//#include "User.h"
//#include <string>
//
//class BankSystem {
//private:
//    unordered_map<string, Client> clientsById; // id interne -> client
//    unordered_map<string, string> accountNumberToClientId; // accNum -> clientId (pour unicité)
//    unordered_map<string, User> users; // username -> user
//
//    uint64_t nextClientId = 1;
//
//    string newClientId() {
//        return "C" + to_string(nextClientId++);
//    }
//
//    void _LoadAllClients() {
//        // CORRECTION : Utilise la méthode publique GetClientsFromFile
//        vector<Client> clients = Client::GetClientsFromFile();
//
//        uint64_t maxId = 0;
//
//        for (Client& client : clients) {
//            string clientId = client.getId();
//            if (clientId.empty()) continue;
//
//            clientsById.emplace(clientId, std::move(client));
//
//            Client& storedClient = clientsById.at(clientId);
//
//            for (const auto& acc : storedClient.getAccounts()) {
//                accountNumberToClientId[acc.getAccountNumber()] = clientId;
//            }
//
//            if (clientId.size() > 1 && clientId[0] == 'C') {
//                try {
//                    uint64_t currentId = stoull(clientId.substr(1));
//                    if (currentId >= maxId) {
//                        maxId = currentId;
//                    }
//                }
//                catch (...) {
//                    // Ignorer les IDs non numériques
//                }
//            }
//        }
//        nextClientId = maxId + 1;
//
//        cout << "\n\t\t\t\t" << clientsById.size() << " clients chargés depuis le fichier.\n";
//    }
//
//public:
//    BankSystem() {
//        _LoadAllClients();
//    }
//
//    void SaveAllClients() {
//        vector<Client> clientsToSave;
//        for (auto& pair : clientsById) {
//            clientsToSave.push_back(pair.second);
//        }
//
//        // CORRECTION : Utilise la méthode publique SaveAll
//        Client::SaveAll(clientsToSave);
//        cout << "\n\t\t\t\t" << clientsToSave.size() << " clients sauvegardés.\n";
//    }
//
//    // ---- Users ----
//    bool addUser(const string& username, const string& password, Permission perms) {
//        if (users.count(username)) return false;
//        users.emplace(username, User(username, password, perms));
//        return true;
//    }
//
//    User authenticate(const std::string& username, const std::string& password) {
//        User user = User::Find(username);
//
//        if (user.getUsername() == "") {
//            return User();
//        }
//
//        if (!user.checkPassword(password)) {
//            return User();
//        }
//
//        return user;
//    }
//
//    // ---------------------- Clients ---------------------- //
//    string addClient(const string& ln, const string& fn, const string& phone, const string& email) {
//        string id = newClientId();
//        Client newClient(id, ln, fn, phone, email, Client::enMode::enAddmode);
//
//        clientsById.emplace(id, newClient);
//
//        // CORRECTION C2248 : Utilise la méthode publique AddNew()
//        newClient.AddNew();
//
//        return id;
//    }
//
//    bool removeClient(const string& clientId) {
//        auto it = clientsById.find(clientId);
//        if (it == clientsById.end()) return false;
//
//        // 1. Marquer pour suppression et libérer les comptes
//        it->second.MarkForDelete(); // Marque le client pour exclusion de la sauvegarde
//
//        for (const auto& acc : it->second.getAccounts()) {
//            accountNumberToClientId.erase(acc.getAccountNumber());
//        }
//
//        // 2. Supprimer de la mémoire
//        clientsById.erase(it);
//
//        // 3. *************** AJOUT CRUCIAL ***************
//        // Réécrire tout le fichier Clients.txt sans le client marqué
//        SaveAllClients(); // C'est cette fonction qui appelle Client::SaveAll(...)
//        // ************************************************
//
//        return true;
//    }
//
//
//    Client* findClientById(const string& clientId) {
//        auto it = clientsById.find(clientId);
//        if (it == clientsById.end()) return nullptr;
//        return &it->second;
//    }
//
//    vector<Client*> listAllClients() {
//        vector<Client*> res;
//        for (auto& p : clientsById) res.push_back(&p.second);
//        return res;
//    }
//
//    // ... (Reste des méthodes de BankSystem pour les comptes et la modification) ...
//    bool accountNumberExists(const string& accNum) const {
//        return accountNumberToClientId.find(accNum) != accountNumberToClientId.end();
//    }
//
//    bool addAccountToClient(const string& clientId, const string& accNum, double initialBalance = 0.0) {
//        if (accountNumberExists(accNum)) return false;
//        Client* c = findClientById(clientId);
//        if (!c) return false;
//        c->addAccount(BankAccount(accNum, initialBalance));
//        accountNumberToClientId[accNum] = clientId;
//        return true;
//    }
//
//    bool removeAccountFromClient(const string& clientId, const string& accNum) {
//        Client* c = findClientById(clientId);
//        if (!c) return false;
//        bool ok = c->removeAccountByNumber(accNum);
//        if (!ok) return false;
//        accountNumberToClientId.erase(accNum);
//        return true;
//    }
//
//    bool depositToAccount(const string& accNum, double amount) {
//        auto itMap = accountNumberToClientId.find(accNum);
//        if (itMap == accountNumberToClientId.end()) return false;
//        Client* c = findClientById(itMap->second);
//        if (!c) return false;
//        BankAccount* a = c->findAccount(accNum);
//        if (!a) return false;
//        return a->depot(amount);
//    }
//
//    bool withdrawFromAccount(const string& accNum, double amount) {
//        auto itMap = accountNumberToClientId.find(accNum);
//        if (itMap == accountNumberToClientId.end()) return false;
//        Client* c = findClientById(itMap->second);
//        if (!c) return false;
//        BankAccount* a = c->findAccount(accNum);
//        if (!a) return false;
//        return a->retrait(amount);
//    }
//
//    bool modifyClientInfo(const string& clientId, const string& ln, const string& fn, const string& phone, const string& email) {
//        Client* c = findClientById(clientId);
//        if (!c) return false;
//        c->setLastName(ln);
//        c->setFirstName(fn);
//        c->setPhone(phone);
//        c->setEmail(email);
//        return true;
//    }
//
//    void printClientDetails(const string& clientId) {
//        Client* c = findClientById(clientId);
//        if (!c) {
//            cout << "Client introuvable.\n";
//            return;
//        }
//        c->printDetail();
//    }
//};
//



