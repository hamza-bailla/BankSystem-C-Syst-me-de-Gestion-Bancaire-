#pragma once
#include "Person.h"
#include "Permission.h"
using namespace std;
#include <iostream>
#include "BankAccount.h"
#include <vector>
#include <iomanip>
#include <fstream>
#include <string>
#include "String.h" // Nécessite la classe String avec String::split(string, string)
#include <algorithm>
#include <stdexcept>

class Client :public Person
{
public:
    enum enMode { enEmptymode = 1, enUpdatemode = 2, enAddmode = 3 };

private:
    string id; // ID interne
    enMode _Mode;
    bool _MarkTodelete = false;
    vector<BankAccount> accounts;

    // Constantes pour les séparateurs
    static constexpr const char* CLIENT_SEP = "#//#";
    static constexpr const char* ACCOUNT_SEP = "$$";

    
    static string _Recordtoline(const Client& client, string sep = CLIENT_SEP)
    {
        // Format du fichier Clients.txt : ID#//#LN#//#FN#//#Email#//#Phone
        string Line = client.id + sep;
        Line += client.lastName + sep;
        Line += client.firstName + sep;
        Line += client.email + sep;
        Line += client.phone;

        return Line; // Pas de séparateur final
    }

    // **********************************************
    // 2. MÉTHODE DE LECTURE (DÉSERIALISATION) : Lit UNIQUEMENT les données Personnelles
    // **********************************************
    static Client _LineToRecord(string line, string sep = CLIENT_SEP) {

        if (line.empty() || line.length() < 5) {
            return _GetEmpty();
        }

        vector<string> data = String::split(line, sep);

        // Nouveaux champs requis : ID, LN, FN, EM, PH (soit 5 champs)
        if (data.size() < 5) {
            return _GetEmpty();
        }

        // Création du client (constructeur: ID, LN, FN, PH, EM)
        Client client(
            data[0],     // ID
            data[1],     // LastName
            data[2],     // FirstName
            data[4],     // Phone 
            data[3],     // Email
            enMode::enUpdatemode
        );

        // ** IMPORTANT : La boucle de lecture des comptes DOIT être retirée d'ici **

        return client;
    }

    static Client _GetEmpty() {
        return Client("", "", "", "", "", enMode::enEmptymode);
    }

    // Implémentation privée de la sauvegarde
    static void _SaveAllImpl(const vector<Client>& clients) {
        fstream file("Clients.txt", ios::out);
        if (file.is_open()) {
            for (const Client& u : clients)
                if (!u._MarkTodelete)
                    file << _Recordtoline(u) << endl;

            file.close();
        }
    }

public:
    // CORRECTION C2512
    Client() = default;

    // Constructeur principal
    Client(const string& _id, const string& ln, const string& fn, const string& ph, const string& em, enMode m = enMode::enAddmode)
        : Person(ln, fn, ph, em), id(_id), _Mode(m) {
    };

    // --- CORRECTION C2248 : Rendre _AddNew() public ou l'encapsuler ---
    void AddNew() const {
        std::fstream file("Clients.txt", std::ios::out | std::ios::app);
        if (file.is_open()) {
            file << _Recordtoline(*this) << std::endl;
            file.close();
        }
    }

    // --- ACCESSEURS PUBLICS ---
    const string& getId() const { return id; }
    string getFullName() const { return firstName + " " + lastName; }

    // CORRECTION C2248: Méthode d'accès à la suppression
    void MarkForDelete() { _MarkTodelete = true; }

    // --- Méthodes de persistance PUBLIQUES pour BankSystem ---

    static vector<Client> GetClientsFromFile() {
        vector<Client> clients;
        fstream file("Clients.txt", ios::in);

        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                Client c = _LineToRecord(line);
                if (c._Mode != enEmptymode) {
                    clients.push_back(std::move(c));
                }
            }
            file.close();
        }
        return clients;
    }

    static void SaveAll(const vector<Client>& client) {
        _SaveAllImpl(client);
    }

    // --- Gestion des Comptes ---
    void addAccount(const BankAccount& acc) { accounts.push_back(acc); }
    bool removeAccountByNumber(const string& accNum) {
        auto it = find_if(accounts.begin(), accounts.end(),
            [&](const BankAccount& a) { return a.getAccountNumber() == accNum; });
        if (it == accounts.end()) return false;
        accounts.erase(it);
        return true;
    }
    BankAccount* findAccount(const string& accNum) {
        for (auto& a : accounts) if (a.getAccountNumber() == accNum) return &a;
        return nullptr;
    }
    const vector<BankAccount>& getAccounts() const { return accounts; }

    // --- Print (affichage) ---
    void printDetail() const {
        cout << "\n\t\t\t\t-------------------------------------------------------";
        cout << "\n\t\t\t\t                 Les Details d'un Client               ";
        cout << "\n\t\t\t\t-------------------------------------------------------\n";
        cout << "\n\t\t\t\tClient ID                         : " << id;
        cout << "\n\t\t\t\tNom                               : " << lastName;
        cout << "\n\t\t\t\tPrenom                            : " << firstName;
        cout << "\n\t\t\t\tTelephone                         : " << phone;
        cout << "\n\t\t\t\tEmail                             : " << email;
        cout << "\n\t\t\t\tComptes (" << accounts.size() << ")                       :";

        for (const auto& acc : accounts) {
            cout << "\n\t\t\t\tN Compte                          : " << acc.getAccountNumber()<<endl;
            cout << "\t\t\t\tSolde                             : " << fixed << setprecision(2) << acc.getBalance()<< " Euro"<<endl;
        }

        cout << "\n\t\t\t\t-------------------------------------------------------\n";
    }
};






//class Client :public Person 
//{ 
//private:
//    enum enMode { enEmptymode = 1, enUpdatemode = 2, enAddmode = 3 };
//    string id; // id interne (UUID simple ou incrémental)
//    string _Accountnumber;
//    string _Pincode;
//    float _Balance;
//    enMode _Mode;
//    bool _MarkTodelete = false;
//    // un client peut avoir plusieurs comptes (ici vector)
//    vector<BankAccount> accounts;
//    static Client _LineToRecord(string line, string sep = "#//#") {
//
//        // 1. Vérification cruciale : Ignorer les lignes vides ou trop courtes.
//        if (line.empty() || line.length() < 5) { // Une ligne valide doit être plus longue que '#//#...'
//            return _GetEmpty(); // Retourne un objet vide au lieu de planter
//        }
//
//        vector<string> data = String::split(line, sep);
//
//        // 2. Vérification de la taille du vecteur
//        if (data.size() < 3) {
//            // La ligne est corrompue/incomplète (moins de 3 champs)
//            // Loggez l'erreur ici si possible.
//            return _GetEmpty();
//        }
//
//        // Si nous arrivons ici, nous sommes certains d'avoir data[0], data[1], et data[2]
//        return Client(
//            data[0],                     // username
//            data[1],                     // password (lecture en clair)
//            data[2],
//            data[3],
//            data[4],
//            enMode::enUpdatemode
//        );
//    }
//
//    static Client _GetEmpty() {
//        return Client("", "", "", "", "", enMode::enEmptymode);
//    }
//
//
//
//
//    static string _Recordtoline(Client client, string space = "#//#")
//    {
//        string Line;
//        Line = Line + client.firstName + space;
//        Line = Line + client.lastName + space;
//        Line = Line + client.email + space;
//        Line = Line + client.phone + space;
//        Line = Line + client.Accountnumber() + space;
//        Line = Line + client.Pincode + space;
//        Line = Line + to_string(client.Balance);
//
//        return Line;
//    }
//    void _AddNew() const {
//        // ios::out : Pour l'écriture | ios::app : Pour ajouter à la fin
//        std::fstream file("Clients.txt", std::ios::out | std::ios::app);
//
//        if (file.is_open()) {
//            // Ajoute la ligne du client (transformée par _RecordToLine) suivie d'un saut de ligne
//            file << _Recordtoline(*this) << std::endl;
//            file.close();
//        }
//        // Note: Si l'ID est généré correctement, il ne devrait pas y avoir de doublons.
//    }
//    
//    static vector<Client> _GetClientsFromFile() {
//        vector<Client> users;
//        fstream file("Clients.txt", ios::in);
//
//        if (file.is_open()) {
//            string line;
//            while (getline(file, line)) {
//                users.push_back(_LineToRecord(line));
//            }
//            file.close();
//        }
//        return users;
//    }
//
//    static void _SaveAll(vector<Client> client) {
//        fstream file("Clients.txt", ios::out);
//        if (file.is_open()) {
//            for (Client& u : client)
//                if (!u._MarkTodelete)
//                    file << _Recordtoline(u) << endl;
//
//            file.close();
//        }
//    }
//
//    void _Update() {
//        vector<Client> client = _GetClientsFromFile();
//
//        for (Client& u : client) {
//            if (u.id == id) {
//                u = *this;
//                break;
//            }
//        }
//        _SaveAll(client);
//    }
//
//    
//
//
//public:
//    //Client() = default;
//    Client(const string& _id, const string& ln, const string& fn, const string& ph, const string& em, enMode m = enMode::enAddmode)
//        : Person(ln, fn, ph, em), id(_id) {
//    };
//
//    const string& getId() const { return id; }
//    string getFullName() const { return firstName + " " + lastName; }
//    const string& getLastName() const { return lastName; }
//    const string& getFirstName() const { return firstName; }
//    const string& getPhone() const { return phone; }
//    const string& getEmail() const { return email; }
//
//    void setLastName(const string& v) { lastName = v; }
//    void setFirstName(const string& v) { firstName = v; }
//    void setPhone(const string& v) { phone = v; }
//    void setEmail(const string& v) { email = v; }
//
//    void addAccount(const BankAccount& acc) { accounts.push_back(acc); }
//    bool removeAccountByNumber(const string& accNum) {
//        auto it = find_if(accounts.begin(), accounts.end(),
//            [&](const BankAccount& a) { return a.getAccountNumber() == accNum; });
//        if (it == accounts.end()) return false;
//        accounts.erase(it);
//        return true;
//    }
//    BankAccount* findAccount(const string& accNum) {
//        for (auto& a : accounts) if (a.getAccountNumber() == accNum) return &a;
//        return nullptr;
//    }
//    const vector<BankAccount>& getAccounts() const { return accounts; }
//
//    string Accountnumber()
//    {
//        return _Accountnumber;
//    }
//
//    void SetPincode(string Pincode)
//    {
//        _Pincode = Pincode;
//    }
//    string GetPincode()
//    {
//        return _Pincode;
//    }
//    __declspec(property(get = GetPincode, put = SetPincode)) string Pincode;
//
//    void SetBalance(float Balance)
//    {
//        _Balance = Balance;
//    }
//    float GetBalance()
//    {
//        return _Balance;
//    }
//    __declspec(property(get = GetBalance, put = SetBalance)) float Balance;
//
//    // affichage complet
//    void printDetail() const {
//        cout << "\n==================The Client Details===================";
//        cout << "\n-------------------------------------------------------";
//        cout << "\nClient ID                           : " << id << "\n";
//        cout << "\nNom                                 : " << lastName;
//        cout << "\nPrénom                              : " << firstName << "\n";
//        cout << "\nTéléphone                           : " << phone << "\n";
//        cout << "\nEmail                               :" << email << "\n";
//        cout << "\nComptes (" << accounts.size() << ") :\n";
//        
//
//
//        /*for (const auto& acc : accounts) {
//            cout << "N°                                : " << acc.getAccountNumber();
//            cout << "Solde                             : " << fixed << setprecision(2) << acc.getBalance() << "\n";
//        }*/
//        cout << "\n-------------------------------------------------------";
//    };
//
//};