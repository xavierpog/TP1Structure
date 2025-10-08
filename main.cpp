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
    bool listeVide;

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
    DossierProfesseur(const char* PF) : tete(nullptr), listeVide(true) {
        cout << "\n[INFO] Tentative d'ouverture du fichier: " << PF << endl;

        ifstream fichier(PF);
        if (!fichier.is_open()) {
            cout << "\n[EXCEPTION] Impossible d'ouvrir le fichier " << PF << endl;
            cout << "[EXCEPTION] Le programme continuera avec une liste vide." << endl;
            return;
        }

        cout << "[INFO] Fichier " << PF << " ouvert avec succes." << endl;

        string ligne;
        int nbProfsCharges = 0;

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
            int nbCours = 0;
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
                    nbCours++;
                }
            }

            // Lire les etudiants jusqu'au &
            Etudiants* dernierEtudiant = nullptr;
            int nbEtudiants = 0;
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
                    nbEtudiants++;
                }
            }

            // Ajouter a la fin pour maintenir l'ordre
            ajouterALaFin(nouveauProf);
            nbProfsCharges++;
            listeVide = false;

            cout << "[INFO] Professeur charge: " << nouveauProf->nom
                << " (" << nbCours << " cours, " << nbEtudiants << " etudiants)" << endl;
        }

        fichier.close();

        if (nbProfsCharges == 0) {
            cout << "\n[EXCEPTION] Fichier " << PF << " ne contient aucun professeur valide!" << endl;
            cout << "[EXCEPTION] La liste restera vide." << endl;
        }
        else {
            cout << "\n[SUCCES] " << nbProfsCharges << " professeur(s) charge(s) avec succes!" << endl;
        }
    }

    // Destructeur
    ~DossierProfesseur() {
        cout << "\n[INFO] Liberation de la memoire..." << endl;
        int nbSupprime = 0;

        while (tete != nullptr) {
            Professeur* temp = tete;
            tete = tete->suivant;

            // Liberer les listes de cours et d'etudiants
            libererCours(temp->listecours);
            libererEtudiants(temp->listeetudiants);

            delete temp;
            nbSupprime++;
        }

        cout << "[INFO] " << nbSupprime << " professeur(s) libere(s) de la memoire." << endl;
    }

    // Supprimer tous les professeurs avec un nom donne
    void supprimer(const char* name) {
        cout << "\n[INFO] Tentative de suppression du professeur: " << name << endl;

        if (tete == nullptr) {
            cout << "[EXCEPTION] Impossible de supprimer - la liste est vide!" << endl;
            return;
        }

        string nomRecherche(name);
        int nbSupprimes = 0;

        // Supprimer en tete de liste
        while (tete != nullptr && tete->nom == nomRecherche) {
            Professeur* temp = tete;
            tete = tete->suivant;

            libererCours(temp->listecours);
            libererEtudiants(temp->listeetudiants);
            delete temp;
            nbSupprimes++;
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
                    nbSupprimes++;
                }
                else {
                    current = current->suivant;
                }
            }
        }

        if (nbSupprimes == 0) {
            cout << "[EXCEPTION] Aucun professeur trouve avec le nom: " << name << endl;
        }
        else {
            cout << "[SUCCES] " << nbSupprimes << " professeur(s) avec le nom '"
                << name << "' supprime(s)." << endl;
        }

        // Verifier si la liste est maintenant vide
        if (tete == nullptr) {
            listeVide = true;
            cout << "[ATTENTION] La liste est maintenant vide apres suppression!" << endl;
        }
    }

    // Afficher le professeur avec le plus d'etudiants
    const char* afficherleprofplusetudiant() const {
        cout << "\n[INFO] Recherche du professeur avec le plus d'etudiants..." << endl;

        if (tete == nullptr) {
            cout << "[EXCEPTION] Impossible - la liste des professeurs est vide!" << endl;
            return nullptr;
        }

        Professeur* meilleurProf = tete;
        int maxEtudiants = compterEtudiants(tete->listeetudiants);
        int minAnciennete = tete->ancien;

        cout << "[INFO] Candidat initial: " << tete->nom << " (" << maxEtudiants << " etudiants)" << endl;

        Professeur* current = tete->suivant;
        while (current != nullptr) {
            int nbEtudiants = compterEtudiants(current->listeetudiants);

            cout << "[INFO] Evaluation: " << current->nom << " (" << nbEtudiants << " etudiants)" << endl;

            if (nbEtudiants > maxEtudiants ||
                (nbEtudiants == maxEtudiants && current->ancien < minAnciennete)) {
                meilleurProf = current;
                maxEtudiants = nbEtudiants;
                minAnciennete = current->ancien;
                cout << "[INFO] Nouveau meilleur candidat!" << endl;
            }
            current = current->suivant;
        }

        cout << "[RESULTAT] Professeur avec le plus d'etudiants: " << meilleurProf->nom
            << " (" << maxEtudiants << " etudiants, anciennete: " << meilleurProf->ancien << ")" << endl;

        // Retourner une copie statique du nom
        static string resultat = meilleurProf->nom;
        return resultat.c_str();
    }

    // Afficher le cours le plus demande
    const char* affichercoursplusdemande() const {
        cout << "\n[INFO] Recherche du cours le plus demande..." << endl;

        if (tete == nullptr) {
            cout << "[EXCEPTION] Impossible - aucun professeur dans la liste!" << endl;
            return nullptr;
        }

        // Compter tous les cours
        string cours[20];  // Tableau pour stocker les cours
        int compteurs[20]; // Compteurs correspondants
        int anciennetes[20]; // Anciennetes des premiers professeurs pour chaque cours
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
                    cout << "[INFO] Nouveau cours detecte: " << coursActuel->nomCours << endl;
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
            cout << "[EXCEPTION] Aucun cours trouve dans toute la liste!" << endl;
            return nullptr;
        }

        // Trouver le cours le plus demande
        int maxDemandes = compteurs[0];
        int indexMeilleur = 0;

        cout << "[INFO] Analyse des " << nbCours << " cours detectes:" << endl;
        for (int i = 0; i < nbCours; i++) {
            cout << "[INFO] " << cours[i] << ": " << compteurs[i] << " demande(s)" << endl;
        }

        for (int i = 1; i < nbCours; i++) {
            if (compteurs[i] > maxDemandes ||
                (compteurs[i] == maxDemandes && anciennetes[i] < anciennetes[indexMeilleur])) {
                maxDemandes = compteurs[i];
                indexMeilleur = i;
            }
        }

        cout << "[RESULTAT] Cours le plus demande: " << cours[indexMeilleur]
            << " (" << maxDemandes << " demandes)" << endl;

        // Retourner une copie statique du nom
        static string resultat = cours[indexMeilleur];
        return resultat.c_str();
    }

    // Afficher le nombre de professeurs pour un cours donne
    int affichernbreprofpouruncours(const string* coursdonne) const {
        cout << "\n[INFO] Recherche du nombre de professeurs pour le cours: " << *coursdonne << endl;

        if (coursdonne == nullptr) {
            cout << "[EXCEPTION] Nom de cours invalide (pointeur null)!" << endl;
            return 0;
        }

        if (tete == nullptr) {
            cout << "[EXCEPTION] Impossible - la liste des professeurs est vide!" << endl;
            cout << "[RESULTAT] Nombre de professeurs pour le cours " << *coursdonne << ": 0" << endl;
            return 0;
        }

        int compteur = 0;
        Professeur* prof = tete;

        while (prof != nullptr) {
            Cours* coursActuel = prof->listecours;
            while (coursActuel != nullptr) {
                if (coursActuel->nomCours == *coursdonne) {
                    compteur++;
                    cout << "[INFO] Professeur " << prof->nom << " enseigne ce cours." << endl;
                    break;  // Ne compter qu'une fois par professeur
                }
                coursActuel = coursActuel->suivant;
            }
            prof = prof->suivant;
        }

        cout << "[RESULTAT] Nombre de professeurs pour le cours " << *coursdonne
            << ": " << compteur << endl;

        if (compteur == 0) {
            cout << "[ATTENTION] Aucun professeur ne souhaite enseigner ce cours!" << endl;
        }

        return compteur;
    }

    // Recopier la liste dans PFnew.txt
    void recopier(const char* nomFichierOriginal) {
        string nouveauNom = "PFnew.txt";

        cout << "\n[INFO] Tentative de sauvegarde dans: " << nouveauNom << endl;

        if (tete == nullptr) {
            cout << "[ATTENTION] La liste est vide - creation d'un fichier vide." << endl;
        }

        ofstream fichier(nouveauNom);
        if (!fichier.is_open()) {
            cout << "[EXCEPTION] Impossible d'ouvrir le fichier " << nouveauNom << " pour l'ecriture!" << endl;
            cout << "[EXCEPTION] Verifiez les permissions du repertoire." << endl;
            return;
        }

        int nbProfsSauves = 0;
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
            nbProfsSauves++;
        }

        fichier.close();
        cout << "[SUCCES] " << nbProfsSauves << " professeur(s) sauvegarde(s) dans: " << nouveauNom << endl;

        if (nbProfsSauves == 0) {
            cout << "[INFO] Fichier " << nouveauNom << " cree mais vide." << endl;
        }
    }

    // Fonction pour afficher la liste (pour debug)
    void afficherListe() const {
        cout << "\n=== LISTE DES PROFESSEURS ===" << endl;

        if (tete == nullptr) {
            cout << "[EXCEPTION] La liste est actuellement vide!" << endl;
            cout << "Aucun professeur a afficher." << endl;
            return;
        }

        Professeur* prof = tete;
        int numero = 1;

        while (prof != nullptr) {
            cout << numero << ". " << prof->nom << " (anciennete: " << prof->ancien << ")" << endl;

            cout << "   Cours: ";
            Cours* cours = prof->listecours;
            if (cours == nullptr) {
                cout << "[AUCUN]";
            }
            else {
                while (cours != nullptr) {
                    cout << cours->nomCours;
                    if (cours->suivant != nullptr) cout << ", ";
                    cours = cours->suivant;
                }
            }
            cout << endl;

            cout << "   Etudiants (" << compterEtudiants(prof->listeetudiants) << "): ";
            Etudiants* etudiant = prof->listeetudiants;
            if (etudiant == nullptr) {
                cout << "[AUCUN]";
            }
            else {
                while (etudiant != nullptr) {
                    cout << etudiant->nomEtudiant;
                    if (etudiant->suivant != nullptr) cout << ", ";
                    etudiant = etudiant->suivant;
                }
            }
            cout << endl << endl;

            prof = prof->suivant;
            numero++;
        }

        cout << "[INFO] Total: " << (numero - 1) << " professeur(s) affiches." << endl;
    }
};

int main() {
    cout << "=========================================" << endl;
    cout << "=== SYSTEME DE GESTION DES DOSSIERS PROFESSEURS ===" << endl;
    cout << "=== VERSION AVEC GESTION D'EXCEPTIONS ===" << endl;
    cout << "=========================================" << endl;

    // Creer l'objet DossierProfesseur
    DossierProfesseur dossiers("PF.txt");

    // Afficher la liste initiale
    dossiers.afficherListe();

    cout << "\n=========================================" << endl;
    cout << "=== TRAITEMENT DU FICHIER DE TRANSACTIONS ===" << endl;
    cout << "=========================================" << endl;

    // Lire les transactions depuis FT.txt
    cout << "\n[INFO] Tentative d'ouverture du fichier de transactions: FT.txt" << endl;
    ifstream fichierTransactions("FT.txt");
    if (!fichierTransactions.is_open()) {
        cout << "[EXCEPTION] Impossible d'ouvrir FT.txt!" << endl;
        cout << "[EXCEPTION] Verifiez que le fichier existe et est accessible." << endl;
        cout << "[EXCEPTION] Le programme va se terminer." << endl;
        return 1;
    }

    cout << "[INFO] Fichier FT.txt ouvert avec succes." << endl;

    string ligne;
    int nbCommandes = 0;
    int nbCommandesExecutees = 0;

    while (getline(fichierTransactions, ligne)) {
        if (ligne.empty()) {
            cout << "[INFO] Ligne vide ignoree." << endl;
            continue;
        }

        nbCommandes++;
        cout << "\n--- COMMANDE " << nbCommandes << " ---" << endl;
        cout << "[INFO] Commande lue: '" << ligne << "'" << endl;

        if (ligne == "*") {
            cout << "[INFO] Execution: Afficher le cours le plus demande" << endl;
            dossiers.affichercoursplusdemande();
            nbCommandesExecutees++;

        }
        else if (ligne == "#") {
            cout << "[INFO] Execution: Afficher le professeur avec le plus d'etudiants" << endl;
            dossiers.afficherleprofplusetudiant();
            nbCommandesExecutees++;

        }
        else if (ligne == "$") {
            cout << "[INFO] Execution: Sauvegarder la liste dans PFnew.txt" << endl;
            dossiers.recopier("PF.txt");
            nbCommandesExecutees++;

        }
        else if (ligne.length() > 1 && ligne[0] == '%' && ligne[1] == ' ') {
            cout << "[INFO] Execution: Compter les professeurs pour un cours" << endl;
            string cours = ligne.substr(2);
            dossiers.affichernbreprofpouruncours(&cours);
            nbCommandesExecutees++;

        }
        else if (ligne.length() > 1 && ligne[0] == '-' && ligne[1] == ' ') {
            cout << "[INFO] Execution: Supprimer un professeur" << endl;
            string nomASupprimer = ligne.substr(2);
            dossiers.supprimer(nomASupprimer.c_str());
            nbCommandesExecutees++;

        }
        else if (ligne[0] == '-') {
            cout << "[INFO] Execution: Supprimer un professeur (format alternatif)" << endl;
            string nomASupprimer = ligne.substr(1);
            dossiers.supprimer(nomASupprimer.c_str());
            nbCommandesExecutees++;

        }
        else {
            cout << "[EXCEPTION] Commande non reconnue ou format incorrect!" << endl;
            cout << "[EXCEPTION] Commande ignoree: '" << ligne << "'" << endl;
            cout << "[INFO] Formats valides: '#', '*', '$', '% COURS', '- NOM'" << endl;
        }
    }

    fichierTransactions.close();

    if (nbCommandes == 0) {
        cout << "\n[EXCEPTION] Aucune commande trouvee dans FT.txt!" << endl;
        cout << "[EXCEPTION] Le fichier est vide ou ne contient que des lignes vides." << endl;
    }
    else {
        cout << "\n[INFO] Traitement termine." << endl;
        cout << "[INFO] " << nbCommandes << " ligne(s) lue(s), "
            << nbCommandesExecutees << " commande(s) executee(s)." << endl;

        if (nbCommandesExecutees < nbCommandes) {
            cout << "[ATTENTION] " << (nbCommandes - nbCommandesExecutees)
                << " commande(s) ignoree(s) a cause d'erreurs!" << endl;
        }
    }

    cout << "\n=========================================" << endl;
    cout << "=== ETAT FINAL DU SYSTEME ===" << endl;
    cout << "=========================================" << endl;
    dossiers.afficherListe();

    cout << "\n=========================================" << endl;
    cout << "[SUCCES] Programme termine avec succes!" << endl;
    cout << "=========================================" << endl;
    return 0;
}