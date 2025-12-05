// Projet.V3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;
#include "BankSystem.h"
#include "Permission.h"
#include "String"
#include "Client.h"
#include <vector>
#include <algorithm>


void printMenu() {
    cout << "\n\n\t\t\t\t         ==== LISTE DES CHOIX ====\n";
    cout << "\n\t\t\t\t\ [1]. Liste clients          :\n";
    cout << "\n\t\t\t\t\ [2]. Ajouter client         :\n";
    cout << "\n\t\t\t\t\ [3]. Supprimer client       :\n";
    cout << "\n\t\t\t\t\ [4]. Modifier client        :\n";
    cout << "\n\t\t\t\t\ [5]. Ajouter compte à client:\n";
    cout << "\n\t\t\t\t\ [6]. Supprimer compte       :\n";
    cout << "\n\t\t\t\t\ [7]. Deposer                :\n";
    cout << "\n\t\t\t\t\ [8]. Retirer                :\n";
    cout << "\n\t\t\t\t\ [9]. Voir details client    :\n";
    cout << "\n\t\t\t\t\ [10]. Transfert             :\n";
    cout << "\n\t\t\t\t\ [0]. Quitter                :\n";
    cout << "\n\t\t\t\t\ [..].Choix                  : ";
}


int main()
{
    BankSystem system;
    string Title = "-BANK SYSTEM-";
    // Optimisation des E/S
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    //std::cout << "Bienvenue dans la démo de l'application bancaire (POO C++).\n";
    cout << "\n\t\t\t\t==========================================";
    cout << "\n\t\t\t\t\t        " << Title;
    cout << "\n\t\t\t\t==========================================";
    std::cout << "\n\t\t\t\t\tVeuillez vous authentifier.        \n";   
    // Déclaration des variables dans la bonne portée
    std::string uname, pwd;
    std::cout << "\t\t\t\t\tLogin d'utilisateur: ";
    std::getline(std::cin, uname); 
    std::cout << "\t\t\t\t\tMot de passe: ";
    std::getline(std::cin, pwd);

    // L'authentification utilise la version qui retourne l'objet User par valeur (fichier)
    User user = system.authenticate(uname, pwd);

    // Vérification de l'authentification (l'objet User retourné est vide si échec)
    if (user.getUsername() == "") {
        std::cout << "\n\t\t\t\tAuthentification echouee. Fin du programme.\n";
        return 0;
    }

    std::cout << "\n\t\t\t\tAuthentification reussie. Bonjour " << user.getUsername() << "!\n";

    // --- Menu principal ---
    int choice = -1;
    while (true) {
        printMenu();
        
        // Gestion de l'entrée et des erreurs (vérifie si la saisie est un entier valide)
        if (!(std::cin >> choice)) { 
            std::cout << "\n\t\t\t\tEntree invalide. Veuillez saisir un nombre.\n";
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Vider le buffer
            continue; 
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Vider le buffer après la lecture de 'choice'

        if (choice == 0) {
            std::cout << "\n\t\t\t\tAu revoir.\n";
            break;
        }

        switch (choice) {
            case 1: { // Lister tous les clients (VIEW_CLIENTS)
                // Utilisation correcte : user.getPermissions() (objet par valeur)
                if (!hasPermission(user.getPermissions(), Permission::VIEW_CLIENTS)) {
                    std::cout << "\n\t\t\t\tPermission refusee.\n"; break;
                }
                auto list = system.listAllClients();
                if (list.empty()) std::cout << "\n\t\t\t\tAucun client.\n";
                else {
                    for (auto c : list) {
                        std::cout <<"\n\t\t\t\t" << c->getId() << " | " << c->getFullName() << " | Tel: " << c->getPhone() << " | Email: " << c->getEmail();
                        
                    }
                }
                break;
            }
            case 2: { // Ajouter un nouveau client (ADD_CLIENT)
                if (!hasPermission(user.getPermissions(), Permission::ADD_CLIENT)) {
                    std::cout << "Permission refusee.\n"; break;
                }
                    string ln, fn, phone, email;
                    cout << "\n\t\t\t\t------------------------------------------";
                    cout << "\n\t\t\t\t\t        "<<"Ajouter New Client: ";
                    cout << "\n\t\t\t\t------------------------------------------\n";
                    cout << "\t\t\t\tNom: "; std::getline(cin, ln);
                    cout << "\t\t\t\tPrenom: "; std::getline(cin, fn);
                    cout << "\t\t\t\tTelephone: "; std::getline(cin, phone);
                    cout << "\t\t\t\tEmail: "; std::getline(cin, email);
                    string id = system.addClient(ln, fn, phone, email);
                    cout << "\t\t\t\tClient ajoute avec ID: " << id << "\n";
                break;
            }
            case 3: { // Supprimer un client (REMOVE_CLIENT)
                if (!hasPermission(user.getPermissions(), Permission::REMOVE_CLIENT)) {
                    std::cout << "\t\t\t\tPermission refusee.\n"; break;
                }
                cout << "\n\t\t\t\t------------------------------------------";
                cout << "\n\t\t\t\t\t      " << "Supprimer un Client         ";
                cout << "\n\t\t\t\t------------------------------------------\n";
                std::string id; std::cout << "\t\t\t\tID client a supprimer: "; std::getline(std::cin, id);
                if (system.removeClient(id)) std::cout << "\t\t\t\tLa Suppression est reussie.\n"; else std::cout << "\t\t\t\tEchec suppression (client introuvable).\n";
                // Après un succès de suppression:
                system.SaveAllClients(); // <--- AJOUTER CET APPEL ICI.
                break;
            }
            case 4: { // Modifier les informations d'un client (MODIFY_CLIENT)
                if (!hasPermission(user.getPermissions(), Permission::MODIFY_CLIENT)) {
                    std::cout << "\t\t\t\tPermission refusee.\n"; break;
                }
                std::string id, ln, fn, phone, email;
                cout << "\n\t\t\t\t------------------------------------------";
                cout << "\n\t\t\t\t\t      " << "Modifier un Client          ";
                cout << "\n\t\t\t\t------------------------------------------\n";
                std::cout << "\t\t\t\tID client a modifier: "; std::getline(std::cin, id);
                std::cout << "\t\t\t\tNouveau nom: "; std::getline(std::cin, ln);
                std::cout << "\t\t\t\tNouveau prenom: "; std::getline(std::cin, fn);
                std::cout << "\t\t\t\tNouveau telephone: "; std::getline(std::cin, phone);
                std::cout << "\t\t\t\tNouveau email: "; std::getline(std::cin, email);
                if (system.modifyClientInfo(id, ln, fn, phone, email)) std::cout << "\t\t\t\tModification reussie.\n"; else std::cout << "\t\t\t\tEchec modification (client introuvable).\n";
                break;
            }
            case 5: { // Ajouter un compte (MODIFY_CLIENT)
                if (!hasPermission(user.getPermissions(), Permission::MODIFY_CLIENT)) {
                    std::cout << "\t\t\t\tPermission refusee.\n"; break;
                }
                std::string id, accNum; double init;
                cout << "\n\t\t\t\t------------------------------------------";
                cout << "\n\t\t\t\t\t " << "Ajouter un compte a un Client    ";
                cout << "\n\t\t\t\t------------------------------------------\n";
                std::cout << "\t\t\t\tID client: "; std::getline(std::cin, id);
                std::cout << "\t\t\t\tNumero de compte (doit etre unique): "; std::getline(std::cin, accNum);
                std::cout << "\t\t\t\tSolde initial: "; 
                if (!(std::cin >> init)) { std::cin.clear(); init = 0; }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (system.addAccountToClient(id, accNum, init)) std::cout << "\t\t\t\tCompte ajoute.\n"; else std::cout << "\t\t\t\tEchec ajout compte (numero peut exister ou client introuvable).\n";
                break;
            }
            case 6: { // Supprimer un compte (MODIFY_CLIENT)
                if (!hasPermission(user.getPermissions(), Permission::MODIFY_CLIENT)) {
                    std::cout << "\t\t\t\tPermission refusee.\n"; break;
                }
                std::string id, accNum;
                cout << "\n\t\t\t\t------------------------------------------";
                cout << "\n\t\t\t\t\t" << "Supprimer un Compte d'un Client   ";
                cout << "\n\t\t\t\t------------------------------------------\n";
                std::cout << "\t\t\t\tID client: "; std::getline(std::cin, id);
                std::cout << "\t\t\t\tNumero de compte a supprimer: "; std::getline(std::cin, accNum);
                if (system.removeAccountFromClient(id, accNum)) std::cout << "\t\t\t\tCompte supprime.\n"; else std::cout << "\t\t\t\tEchec suppression compte.\n";
                break;
            }
            case 7: { // Dépôt (DEPOT)
                if (!hasPermission(user.getPermissions(), Permission::DEPOT)) {
                    std::cout << "\t\t\t\tPermission refusee.\n"; break;
                }
                std::string accNum; double amount;
                cout << "\n\t\t\t\t------------------------------------------";
                cout << "\n\t\t\t\t\t       " << "Deposer un Montant         ";
                cout << "\n\t\t\t\t------------------------------------------\n";
                std::cout << "\t\t\t\tNumero de compte: "; std::getline(std::cin, accNum);
                std::cout << "\t\t\t\tMontant a deposer: "; 
                if (!(std::cin >> amount)) { std::cin.clear(); amount = 0; }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (system.depositToAccount(accNum, amount)) std::cout << "\t\t\t\tDepot reussi.\n"; else std::cout << "\t\t\t\tEchec depot.\n";
                break;
            }
            case 8: { // Retrait (RETRAIT)
                if (!hasPermission(user.getPermissions(), Permission::RETRAIT)) {
                    std::cout << "\t\t\t\tPermission refusee.\n"; break;
                }
                std::string accNum; double amount;
                cout << "\n\t\t\t\t------------------------------------------";
                cout << "\n\t\t\t\t\t       " << "Retrait un Montant         ";
                cout << "\n\t\t\t\t------------------------------------------\n";
                std::cout << "\t\t\t\tNumero de compte: "; std::getline(std::cin, accNum);
                std::cout << "\t\t\t\tMontant à retirer: "; 
                if (!(std::cin >> amount)) { std::cin.clear(); amount = 0; }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (system.withdrawFromAccount(accNum, amount)) std::cout << "\t\t\t\tRetrait reussi.\n"; else std::cout << "\t\t\t\tEchec retrait (solde insuffisant ou compte introuvable).\n";
                break;
            }
            case 9: { // Afficher détails client (VIEW_CLIENTS)
                
                if (!hasPermission(user.getPermissions(), Permission::VIEW_CLIENTS)) {
                    std::cout << "\t\t\t\tPermission refusee.\n"; break;
                }
                std::string id; std::cout << "\t\t\t\tID client: "; std::getline(std::cin, id);
                system.printClientDetails(id);
                break;
            }
            case 10: { // Faire un Transfer d'un compte A1 vers un compte A2
                if (!hasPermission(user.getPermissions(), Permission::TRANSFERT)) {
                    std::cout << "\t\t\t\tPermission refusee.\n"; break;
                }
                string Acc1,Acc2; 
                cout << "\n\t\t\t\t------------------------------------------\n";
                cout << "\n\t\t\t\t\t      " << "Transfert d'un Montant      \n";
                cout << "\n\t\t\t\t------------------------------------------\n";
                cout << "\n\t\t\t\Numero Compte Emetteur: "; getline(cin, Acc1);
                cout << "\n\t\t\t\Numero Compte Recepteur: "; getline(cin, Acc1);
                //system.Transfert(Acc1, Acc2);
                break;


            }
            default:
                std::cout << "\t\t\t\tChoix invalide.\n";
        }
    }
    // pour charger les clients qui sont dans le fichier Clients.txt :
    system.SaveAllClients(); // <--- TRÈS IMPORTANT !
    return 0;
}
