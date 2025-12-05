#pragma once
#include "Person.h"
#include "Permission.h"
using namespace std;
#include <iostream>
#include "String.h"
#include <vector>
#include <fstream>
#include <string>

class User : public Person{
public:
    enum class Mode { Empty = 1, Update = 2, Add = 3 };

private:
    string username;
    string password;
    Permission permissions;
    Mode _mode = Mode::Empty;
    bool _MarkToDelete = false;

    // -------- Static Helpers --------

    static User _LineToRecord(string line, string sep = "#//#") {

        // 1. Vérification cruciale : Ignorer les lignes vides ou trop courtes.
        if (line.empty() || line.length() < 5) { // Une ligne valide doit être plus longue que '#//#...'
            return _GetEmpty(); // Retourne un objet vide au lieu de planter
        }

        vector<string> data = String::split(line, sep);

        // 2. Vérification de la taille du vecteur
        if (data.size() < 3) {
            // La ligne est corrompue/incomplète (moins de 3 champs)
            // Loggez l'erreur ici si possible.
            return _GetEmpty();
        }

        // Si nous arrivons ici, nous sommes certains d'avoir data[0], data[1], et data[2]
        return User(
            data[0],                     // username
            data[1],                     // password (lecture en clair)
            static_cast<Permission>(stoi(data[2])),
            Mode::Update
        );
    }

    static string _RecordToLine(const User& u, std::string sep = "#//#") {
        string line;
        // 1. Ajoute le nom d'utilisateur
        line += u.username;

        // 2. Ajoute le séparateur
        line += sep;
        // 3. Ajoute le mot de passe EN CLAIR (cryptage retiré)
        line += u.password;
        // 4. Ajoute le séparateur
        line += sep;
        // 5. Ajoute la permission (convertie en chaîne pour le fichier)
        line += std::to_string(static_cast<int>(u.permissions));
        return line;
    }

    static User _GetEmpty() {
        return User("", "", Permission::NONE, Mode::Empty);
    }

    static vector<User> _GetUsersFromFile() {
        vector<User> users;
        fstream file("Users.txt", ios::in);

        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                users.push_back(_LineToRecord(line));
            }
            file.close();
        }
        return users;
    }

    static void _SaveAll(vector<User> users) {
        fstream file("Users.txt", ios::out);
        if (file.is_open()) {
            for (User& u : users)
                if (!u._MarkToDelete)
                    file << _RecordToLine(u) << endl;

            file.close();
        }
    }

    void _Update() const {
        vector<User> users = _GetUsersFromFile();

        for (User& u : users) {
            if (u.username == username) {
                u = *this;
                break;
            }
        }
        _SaveAll(users);
    }

    void _AddNew() const {
        fstream file("Users.txt", ios::out | ios::app);

        if (file.is_open()) {
            file << _RecordToLine(*this) << endl;
            file.close();
        }
    }

public:

    // -------- Constructors ----------
    User() = default;

    User(const string& u, const string& p, Permission perms, Mode m = Mode::Add)
        : username(u), password(p), permissions(perms), _mode(m) {
    }

    // -------- Getters / Setters ------
    const string& getUsername() const { return username; }
    // La vérification compare le MDP en clair stocké avec le MDP en clair saisi
    //bool checkPassword(const string& p) const { return password == p; }
    Permission getPermissions() const { return permissions; }
    void setPermissions(Permission p) { permissions = p; }

    void MarkToDelete() { _MarkToDelete = true; }

    // -------- CRUD operations --------

    void Save() {
        if (_mode == Mode::Add)
            _AddNew();
        else if (_mode == Mode::Update)
            _Update();
    }

    static User Find(string username) {
        vector<User> users = _GetUsersFromFile();

        for (User& u : users) {
            // Ligne de test de débogage pour voir la comparaison des noms d'utilisateur
            //std::cout << "DEBUG FIND: Cherche '" << username << "'. Trouve dans fichier: '" << u.username << "'\n";

            if (u.username == username)
                return u;
        }
        std::cout << "DEBUG FIND: Utilisateur non trouve.\n";
        return _GetEmpty();
    }
    bool checkPassword(const string& p) const {
        // Ligne de test de débogage pour voir la comparaison des mots de passe
        //std::cout << "DEBUG PASS: Saisi: '" << p << "'. Stocke: '" << password << "'\n";

        return password == p;
    }
    static bool Delete(string username) {
        vector<User> users = _GetUsersFromFile();

        for (User& u : users) {
            if (u.username == username) {
                u._MarkToDelete = true;
                _SaveAll(users);
                return true;
            }
        }
        return false;
    }

    static vector<User> GetAll() {
        return _GetUsersFromFile();
    }
};




