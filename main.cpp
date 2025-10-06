#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

// Structure pour les cours
struct Cours {
    string nomCours;
    Cours* suivant;

    Cours(const string& nom) : nomCours(nom), suivant(nullptr) {}
};

// Structure pour les etudiants
struct Etudiants {
    string nomEtudiant;
    Etudiants* suivant;

    Etudiants(const string& nom) : nomEtudiant(nom), suivant(nullptr) {}
};

// Structure pour les professeurs
struct Professeur {
    string nom;
    int ancien;
    Cours* listecours;
    Etudiants* listeetudiants;
    Professeur* suivant;

    Professeur() : ancien(0), listecours(nullptr), listeetudiants(nullptr), suivant(nullptr) {}
};

class DossierProfesseur {
private:
    Professeur* tete;

    // Fonction pour compter les etudiants d'un professeur
    int compterEtudiants(Etudiants* liste) const {
        int count = 0;
        while (liste != nullptr) {
            count++;
            liste = liste->suivant;
        }
        return count;
    }

    // Fonction pour liberer la memoire d'une liste de cours
    void libererCours(Cours* cours) {
        while (cours != nullptr) {
            Cours* temp = cours;
            cours = cours->suivant;
            delete temp;
        }
    }

    // Fonction pour liberer la memoire d'une liste d'etudiants
    void libererEtudiants(Etudiants* etudiants) {
        while (etudiants != nullptr) {
            Etudiants* temp = etudiants;
            etudiants = etudiants->suivant;
            delete temp;
        }
    }

    // Fonction pour ajouter a la fin de la liste (maintenir l'ordre)
    void ajouterALaFin(Professeur* nouveauProf) {
        if (tete == nullptr) {
            tete = nouveauProf;
        }
        else {
            Professeur* current = tete;
            while (current->suivant != nullptr) {
                current = current->suivant;
            }
            current->suivant = nouveauProf;
        }
    }

public:
    // Constructeur
    DossierProfesseur(const char* PF) : tete(nullptr) {
        ifstream fichier(PF);
        if (!fichier.is_open()) {
            cout << "Erreur: Impossible d'ouvrir le fichier " << PF << endl;
            return;
        }

        string ligne;
        while (getline(fichier, ligne)) {
            if (ligne.empty()) continue;

            Professeur* nouveauProf = new Professeur();

            // Lire le nom du professeur
            nouveauProf->nom = ligne;

            // Lire l'anciennete
            if (getline(fichier, ligne)) {
                stringstream ss(ligne);
                ss >> nouveauProf->ancien;
            }

            // Lire les cours jusqu'au &
            Cours* dernierCours = nullptr;
            while (getline(fichier, ligne) && ligne != "&") {
                if (!ligne.empty()) {
                    Cours* nouveauCours = new Cours(ligne);
                    if (nouveauProf->listecours == nullptr) {
                        nouveauProf->listecours = nouveauCours;
                    }
                    else {
                        dernierCours->suivant = nouveauCours;
                    }
                    dernierCours = nouveauCours;
                }
            }

            // Lire les etudiants jusqu'au &
            Etudiants* dernierEtudiant = nullptr;
            while (getline(fichier, ligne) && ligne != "&") {
                if (!ligne.empty()) {
                    Etudiants* nouvelEtudiant = new Etudiants(ligne);
                    if (nouveauProf->listeetudiants == nullptr) {
                        nouveauProf->listeetudiants = nouvelEtudiant;
                    }
                    else {
                        dernierEtudiant->suivant = nouvelEtudiant;
                    }
                    dernierEtudiant = nouvelEtudiant;
                }
            }

            // Ajouter a la fin pour maintenir l'ordre
            ajouterALaFin(nouveauProf);
        }

        fichier.close();
        cout << "Donnees chargees avec succes depuis " << PF << endl;
    }

    // Destructeur
    ~DossierProfesseur() {
        while (tete != nullptr) {
            Professeur* temp = tete;
            tete = tete->suivant;

            // Liberer les listes de cours et d'etudiants
            libererCours(temp->listecours);
            libererEtudiants(temp->listeetudiants);

            delete temp;
        }
    }

    // Supprimer tous les professeurs avec un nom donne
    void supprimer(const char* name) {
        string nomRecherche(name);

        // Supprimer en tete de liste
        while (tete != nullptr && tete->nom == nomRecherche) {
            Professeur* temp = tete;
            tete = tete->suivant;

            libererCours(temp->listecours);
            libererEtudiants(temp->listeetudiants);
            delete temp;
        }

        // Supprimer dans le reste de la liste
        if (tete != nullptr) {
            Professeur* current = tete;
            while (current->suivant != nullptr) {
                if (current->suivant->nom == nomRecherche) {
                    Professeur* temp = current->suivant;
                    current->suivant = temp->suivant;

                    libererCours(temp->listecours);
                    libererEtudiants(temp->listeetudiants);
                    delete temp;
                }
                else {
                    current = current->suivant;
                }
            }
        }

        cout << "Professeur(s) avec le nom '" << name << "' supprime(s)." << endl;
    }

    // Afficher le professeur avec le plus d'etudiants
    const char* afficherleprofplusetudiant() const {
        if (tete == nullptr) {
            cout << "Aucun professeur dans la liste." << endl;
            return nullptr;
        }

        Professeur* meilleurProf = tete;
        int maxEtudiants = compterEtudiants(tete->listeetudiants);
        int minAnciennete = tete->ancien;

        Professeur* current = tete->suivant;
        while (current != nullptr) {
            int nbEtudiants = compterEtudiants(current->listeetudiants);

            if (nbEtudiants > maxEtudiants ||
                (nbEtudiants == maxEtudiants && current->ancien < minAnciennete)) {
                meilleurProf = current;
                maxEtudiants = nbEtudiants;
                minAnciennete = current->ancien;
            }
            current = current->suivant;
        }

        cout << "Professeur avec le plus d'etudiants: " << meilleurProf->nom
            << " (" << maxEtudiants << " etudiants, anciennete: " << meilleurProf->ancien << ")" << endl;

        // Retourner une copie statique du nom
        static string resultat = meilleurProf->nom;
        return resultat.c_str();
    }

    // Afficher le cours le plus demande
    const char* affichercoursplusdemande() const {
        if (tete == nullptr) {
            cout << "Aucun professeur dans la liste." << endl;
            return nullptr;
        }

        // Compter tous les cours
        string cours[1000];  // Tableau pour stocker les cours
        int compteurs[1000]; // Compteurs correspondants
        int anciennetes[1000]; // Anciennetes des premiers professeurs pour chaque cours
        int nbCours = 0;

        Professeur* prof = tete;
        while (prof != nullptr) {
            Cours* coursActuel = prof->listecours;
            while (coursActuel != nullptr) {
                // Chercher si le cours existe deja
                int index = -1;
                for (int i = 0; i < nbCours; i++) {
                    if (cours[i] == coursActuel->nomCours) {
                        index = i;
                        break;
                    }
                }

                if (index == -1) {
                    // Nouveau cours
                    cours[nbCours] = coursActuel->nomCours;
                    compteurs[nbCours] = 1;
                    anciennetes[nbCours] = prof->ancien;
                    nbCours++;
                }
                else {
                    // Cours existant
                    compteurs[index]++;
                    if (prof->ancien < anciennetes[index]) {
                        anciennetes[index] = prof->ancien;
                    }
                }

                coursActuel = coursActuel->suivant;
            }
            prof = prof->suivant;
        }

        if (nbCours == 0) {
            cout << "Aucun cours trouve." << endl;
            return nullptr;
        }

        // Trouver le cours le plus demande
        int maxDemandes = compteurs[0];
        int indexMeilleur = 0;

        for (int i = 1; i < nbCours; i++) {
            if (compteurs[i] > maxDemandes ||
                (compteurs[i] == maxDemandes && anciennetes[i] < anciennetes[indexMeilleur])) {
                maxDemandes = compteurs[i];
                indexMeilleur = i;
            }
        }

        cout << "Cours le plus demande: " << cours[indexMeilleur]
            << " (" << maxDemandes << " demandes)" << endl;

        // Retourner une copie statique du nom
        static string resultat = cours[indexMeilleur];
        return resultat.c_str();
    }

    // Afficher le nombre de professeurs pour un cours donne
    int affichernbreprofpouruncours(const string* coursdonne) const {
        if (coursdonne == nullptr || tete == nullptr) {
            cout << "Nombre de professeurs pour le cours " << *coursdonne << ": 0" << endl;
            return 0;
        }

        int compteur = 0;
        Professeur* prof = tete;

        while (prof != nullptr) {
            Cours* coursActuel = prof->listecours;
            while (coursActuel != nullptr) {
                if (coursActuel->nomCours == *coursdonne) {
                    compteur++;
                    break;  // Ne compter qu'une fois par professeur
                }
                coursActuel = coursActuel->suivant;
            }
            prof = prof->suivant;
        }

        cout << "Nombre de professeurs pour le cours " << *coursdonne
            << ": " << compteur << endl;
        return compteur;
    }

    // Recopier la liste dans PFnew.txt
    void recopier(const char* nomFichierOriginal) {
        string nouveauNom = "PFnew.txt";

        ofstream fichier(nouveauNom);
        if (!fichier.is_open()) {
            cout << "Erreur: Impossible d'ouvrir le fichier " << nouveauNom << " pour l'ecriture." << endl;
            return;
        }

        Professeur* prof = tete;
        while (prof != nullptr) {
            // Ecrire le nom et l'anciennete
            fichier << prof->nom << endl;
            fichier << prof->ancien << endl;

            // Ecrire les cours
            Cours* cours = prof->listecours;
            while (cours != nullptr) {
                fichier << cours->nomCours << endl;
                cours = cours->suivant;
            }
            fichier << "&" << endl;

            // Ecrire les etudiants
            Etudiants* etudiant = prof->listeetudiants;
            while (etudiant != nullptr) {
                fichier << etudiant->nomEtudiant << endl;
                etudiant = etudiant->suivant;
            }
            fichier << "&" << endl;

            // Ligne vide entre professeurs sauf pour le dernier
            if (prof->suivant != nullptr) {
                fichier << endl;
            }

            prof = prof->suivant;
        }

        fichier.close();
        cout << "Liste mise a jour sauvegardee dans: " << nouveauNom << endl;
    }

    // Fonction pour afficher la liste (pour debug)
    void afficherListe() const {
        cout << "\n=== LISTE DES PROFESSEURS ===" << endl;
        Professeur* prof = tete;
        int numero = 1;

        while (prof != nullptr) {
            cout << numero << ". " << prof->nom << " (anciennete: " << prof->ancien << ")" << endl;

            cout << "   Cours: ";
            Cours* cours = prof->listecours;
            while (cours != nullptr) {
                cout << cours->nomCours;
                if (cours->suivant != nullptr) cout << ", ";
                cours = cours->suivant;
            }
            cout << endl;

            cout << "   Etudiants (" << compterEtudiants(prof->listeetudiants) << "): ";
            Etudiants* etudiant = prof->listeetudiants;
            while (etudiant != nullptr) {
                cout << etudiant->nomEtudiant;
                if (etudiant->suivant != nullptr) cout << ", ";
                etudiant = etudiant->suivant;
            }
            cout << endl << endl;

            prof = prof->suivant;
            numero++;
        }
    }
};

int main() {
    cout << "=== SYSTEME DE GESTION DES DOSSIERS PROFESSEURS ===" << endl;

    // Creer l'objet DossierProfesseur
    DossierProfesseur dossiers("PF.txt");

    // Afficher la liste initiale
    dossiers.afficherListe();

    // Lire les transactions depuis FT.txt
    ifstream fichierTransactions("FT.txt");
    if (!fichierTransactions.is_open()) {
        cout << "Erreur: Impossible d'ouvrir FT.txt" << endl;
        return 1;
    }

    cout << "\n=== EXECUTION DES TRANSACTIONS ===" << endl;
    string ligne;
    while (getline(fichierTransactions, ligne)) {
        if (ligne.empty()) continue;

        cout << "\nCommande: " << ligne << endl;

        if (ligne == "*") {
            // Afficher le cours le plus demande
            dossiers.affichercoursplusdemande();

        }
        else if (ligne == "#") {
            // Afficher le professeur avec le plus d'etudiants
            dossiers.afficherleprofplusetudiant();

        }
        else if (ligne == "$") {
            // Recopier dans PFnew.txt
            dossiers.recopier("PF.txt");

        }
        else if (ligne.length() > 1 && ligne[0] == '%' && ligne[1] == ' ') {
            // Extraire le nom du cours apres "% "
            string cours = ligne.substr(2);
            dossiers.affichernbreprofpouruncours(&cours);

        }
        else if (ligne.length() > 1 && ligne[0] == '-' && ligne[1] == ' ') {
            // Gerer la commande de suppression "- NOM"
            string nomASupprimer = ligne.substr(2);
            dossiers.supprimer(nomASupprimer.c_str());

        }
        else if (ligne[0] == '-') {
            // Alternative: si juste "-NOM" sans espace
            string nomASupprimer = ligne.substr(1);
            dossiers.supprimer(nomASupprimer.c_str());

        }
        else {
            // Commande non reconnue
            cout << "Commande non reconnue: " << ligne << endl;
        }
    }

    fichierTransactions.close();

    cout << "\n=== LISTE FINALE ===" << endl;
    dossiers.afficherListe();

    cout << "\nProgramme termine avec succes!" << endl;
    return 0;
}