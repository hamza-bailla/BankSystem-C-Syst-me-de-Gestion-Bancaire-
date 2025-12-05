# BankSystem-C-Syst-me-de-Gestion-Bancaire-
C++ Console Bank Management System. Manages clients (Client) and accounts (BankAccount) with secure transactions (deposit/withdraw). Features a User class for authentication and granular permissions. Data persistence is handled via flat files (*.txt). Robust OOP design with BankSystem as the central controller.
C++ Konsolen-Bankverwaltungssystem. Verwaltet Kunden (Client) und Konten (BankAccount) mit sicheren Transaktionen (Einzahlung/Abhebung). Verfügt über eine User-Klasse für Authentifizierung und granulare Berechtigungen. Datenpersistenz erfolgt über flache Dateien (*.txt). Robustes OOP-Design mit BankSystem als zentraler Steuerungseinheit.
**Fonctionnalités Principales:**
Gestion des Clients (Client) : Fonctions CRUD (Création, Lecture, Mise à jour, Suppression) pour maintenir les informations personnelles des clients. La classe Client hérite de la classe générique Person.
Gestion des Comptes (BankAccount) : Permet d'ouvrir et de fermer des comptes pour chaque client. Gère le solde du compte et supporte les opérations de dépôt (depot) et de retrait (retrait) avec validation du solde.
Gestion des Utilisateurs (User) : Mise en place d'un mécanisme d'authentification des employés du système. Les utilisateurs sont associés à un ensemble de droits d'accès définis par une énumération de flags (Permission).
Persistance des Données : Toutes les données sont stockées de manière persistante dans des fichiers plats (texte) dédiés :
Clients.txt (informations personnelles)
Accounts.txt (numéros de compte et soldes)
Users.txt (identifiants et permissions)
Logique Centrale (BankSystem) : Cette classe agit comme le contrôleur principal, gérant le chargement initial des données, la coordination entre les clients et les comptes (via des maps en mémoire), et la sauvegarde globale des modifications.
Utilitaires (String) : Utilisation de méthodes statiques pour le parsing des fichiers de données (split) et d'autres manipulations de chaînes nécessaires à la sérialisation/désérialisation.
**Structure des Classes Clés:**
BankSystem: Le contrôleur, gérant les collections de Client et User.
Client:Hérite de Person. Contient une agrégation de BankAccount.
User:Contient les informations d'authentification et les Permissions.
BankAccount: Contient le solde et les méthodes de transaction.



