#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>
#include <fstream>  // Pour l'exportation et la lecture des fichiers
#include <ctime>    // Pour horodatation

using namespace std;

// Déclarations des variables
double R = 0.0, L = 0.0, C = 0.0, f = 0.0, Q = 0.0; // Résistance, Inductance, Capacité, Fréquence, Facteur de qualité
bool rKnown = false, lKnown = false, cKnown = false, fKnown = false, qKnown = false; // Statut des valeurs connues

// Fonction pour afficher le menu principal
void afficherMenu() {
    cout << "==== Menu ====\n";
    cout << "1. Spécifier les valeurs connues (R, L, C, f, Q)\n";
    cout << "2. Calculer les valeurs inconnues\n";
    cout << "3. Afficher les conversions des unités\n";
    cout << "4. Exporter les résultats dans un fichier\n";
    cout << "5. Quitter\n";
    cout << "Votre choix : ";
}

// Fonction pour saisir une valeur positive avec une gestion des erreurs
double saisirValeurPositive(const string &nom) {
    double valeur;
    while (true) {
        cout << "Entrez la valeur de " << nom << " (> 0): ";
        cin >> valeur;

        if (cin.fail() || valeur <= 0) {
            cin.clear(); // Réinitialiser l'état de cin
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Vider le buffer
            cout << "Erreur : " << nom << " doit être strictement supérieur à 0.\n";
        } else {
            return valeur;
        }
    }
}

// Fonction pour spécifier les valeurs connues
void specifierValeursConnues() {
    int choix;
    cout << "Quelles valeurs connaissez-vous ?\n";
    cout << "1. Résistance R\n";
    cout << "2. Inductance L\n";
    cout << "3. Capacité C\n";
    cout << "4. Fréquence f\n";
    cout << "5. Facteur de qualité Q\n";
    cout << "6. Terminer la spécification\n";

    do {
        cout << "Votre choix : ";
        cin >> choix;
        switch (choix) {
            case 1:
                R = saisirValeurPositive("R (Ohms)");
                rKnown = true;
                break;
            case 2:
                L = saisirValeurPositive("L (Henrys)");
                lKnown = true;
                break;
            case 3:
                C = saisirValeurPositive("C (Farads)");
                cKnown = true;
                break;
            case 4:
                f = saisirValeurPositive("f (Hz)");
                fKnown = true;
                break;
            case 5:
                Q = saisirValeurPositive("Q (Facteur de qualité)");
                qKnown = true;
                break;
            case 6:
                cout << "Spécification terminée.\n";
                break;
            default:
                cout << "Choix invalide. Veuillez réessayer.\n";
        }
    } while (choix != 6);
}

// Fonction pour convertir et afficher les unités de L et C
void afficherConversions() {
    if (!lKnown || !cKnown) {
        cout << "Veuillez d'abord calculer ou spécifier les valeurs de L et C.\n";
        return;
    }

    cout << "\nConversions des unités :\n";

    // Inductance (L)
    cout << "Inductance (L) :\n";
    cout << fixed << setprecision(12);
    cout << " - " << L << " H (Henrys)\n";
    cout << " - " << L * 1e3 << " mH (millihenrys)\n";
    cout << " - " << L * 1e6 << " µH (microhenrys)\n";

    // Capacité (C)
    cout << "Capacité (C) :\n";
    cout << " - " << C << " F (Farads)\n";
    cout << " - " << C * 1e9 << " nF (nanofarads)\n";
    cout << " - " << C * 1e12 << " pF (picofarads)\n";
}

// Fonction pour calculer les valeurs inconnues
void calculerValeurs() {
    if (fKnown && rKnown) {
        // Calcul de C et L
        double omega0 = 2 * M_PI * f;
        C = 1 / (R * omega0);
        L = 1 / (omega0 * omega0 * C);
        cKnown = true;
        lKnown = true;
        cout << "Calculé à partir de R et f :\n";
        cout << "Capacité C = " << C << " F\n";
        cout << "Inductance L = " << L << " H\n";
    } else if (lKnown && cKnown) {
        // Calcul de f
        double omega0 = 1 / sqrt(L * C);
        f = omega0 / (2 * M_PI);
        fKnown = true;
        cout << "Calculé à partir de L et C :\n";
        cout << "Fréquence f = " << f << " Hz\n";
    } else if (fKnown && cKnown) {
        // Calcul de R
        double omega0 = 2 * M_PI * f;
        R = 1 / (omega0 * C);
        rKnown = true;
        cout << "Calculé à partir de f et C :\n";
        cout << "Résistance R = " << R << " Ohms\n";
    } else if (qKnown && rKnown) {
        // Calcul de L et C à partir de Q et R
        if (fKnown) {
            double omega0 = 2 * M_PI * f;
            C = 1 / (R * omega0);
            L = Q * Q * C * R * R;
            lKnown = true;
            cKnown = true;
            cout << "Calculé à partir de Q, R, et f :\n";
            cout << "Inductance L = " << L << " H\n";
            cout << "Capacité C = " << C << " F\n";
        } else {
            cout << "Fréquence f est nécessaire pour utiliser Q et R.\n";
        }
    } else {
        cout << "Pas assez de données pour effectuer les calculs.\n";
    }
}

// Fonction pour exporter les résultats dans un fichier
void exporterResultats() {
    if (!rKnown || !lKnown || !cKnown || !fKnown) {
        cout << "Veuillez calculer toutes les valeurs avant d'exporter.\n";
        return;
    }

    string nomFichier;
    cout << "Entrez le nom du fichier d'exportation (par exemple, resultats.txt) : ";
    cin >> nomFichier;

    // Ajouter une horodatation au fichier
    time_t maintenant = time(0);
    tm *ltm = localtime(&maintenant);

    ofstream fichier(nomFichier);
    if (!fichier) {
        cout << "Erreur : Impossible de créer le fichier.\n";
        return;
    }

    fichier << "Résultats calculés (Date : ";
    fichier << 1900 + ltm->tm_year << "-" 
            << 1 + ltm->tm_mon << "-" 
            << ltm->tm_mday << " ";
    fichier << ltm->tm_hour << ":" 
            << ltm->tm_min << ":" 
            << ltm->tm_sec << "):\n";

    fichier << fixed << setprecision(12);
    fichier << "R = " << R << " Ohms\n";
    fichier << "f = " << f << " Hz\n";
    fichier << "L = " << L << " H\n";
    fichier << "C = " << C << " F\n";

    if (qKnown) {
        fichier << "Facteur de qualité Q = " << Q << "\n";
    }

    fichier << "Conversions des unités :\n";
    fichier << "Inductance (L) :\n";
    fichier << " - " << L << " H\n";
    fichier << " - " << L * 1e3 << " mH\n";
    fichier << " - " << L * 1e6 << " µH\n";

    fichier << "Capacité (C) :\n";
    fichier << " - " << C << " F\n";
    fichier << " - " << C * 1e9 << " nF\n";
    fichier << " - " << C * 1e12 << " pF\n";

    fichier.close();
    cout << "Résultats exportés dans le fichier '" << nomFichier << "'.\n";
}

int main() {
    int choix;

    do {
        afficherMenu();
        cin >> choix;

        switch (choix) {
            case 1:
                specifierValeursConnues();
                break;
            case 2:
                calculerValeurs();
                break;
            case 3:
                afficherConversions();
                break;
            case 4:
                exporterResultats();
                break;
            case 5:
                cout << "Programme terminé. Au revoir !\n";
                break;
            default:
                cout << "Choix invalide. Veuillez réessayer.\n";
                break;
        }

    } while (choix != 5);

    return 0;
}