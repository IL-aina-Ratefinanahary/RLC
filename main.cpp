#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>
#include <fstream> // Pour l'exportation et la lecture des fichiers

using namespace std;

// Fonction pour afficher le menu
void afficherMenu() {
    cout << "==== Menu ====\n";
    cout << "1. Saisir les valeurs de R et f0\n";
    cout << "2. Calculer L et C\n";
    cout << "3. Convertir les unités de L et C\n";
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

// Fonction pour saisir une fréquence dans un intervalle donné
double saisirFrequence() {
    double frequence;
    while (true) {
        cout << "Entrez la fréquence centrale f0 (entre 20 Hz et 20000 Hz) : ";
        cin >> frequence;

        if (cin.fail() || frequence < 20 || frequence > 20000) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Erreur : la fréquence doit être comprise entre 20 Hz et 20000 Hz.\n";
        } else {
            return frequence;
        }
    }
}

// Fonction pour convertir et afficher les unités de L et C
void afficherConversions(double L, double C) {
    cout << "\nConversion des unités :\n";

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

    // Avertissements pour des valeurs non réalistes
    if (L > 1 || L < 1e-12) {
        cout << "Avertissement : La valeur de L (" << L << " H) semble non réaliste.\n";
    }
    if (C > 1 || C < 1e-12) {
        cout << "Avertissement : La valeur de C (" << C << " F) semble non réaliste.\n";
    }
}

// Fonction pour exporter les résultats dans un fichier avec un nom personnalisé
void exporterResultats(double R, double f0, double L, double C) {
    string nomFichier;
    cout << "Entrez le nom du fichier d'exportation (par exemple, resultats.txt) : ";
    cin >> nomFichier;

    ofstream fichier(nomFichier);
    if (!fichier) {
        cout << "Erreur : Impossible de créer le fichier.\n";
        return;
    }

    fichier << "Résultats calculés :\n";
    fichier << fixed << setprecision(12);
    fichier << "R = " << R << " Ohms\n";
    fichier << "f0 = " << f0 << " Hz\n";
    fichier << "Inductance L = " << L << " H\n";
    fichier << "Capacité C = " << C << " F\n";
    fichier << "\nConversions des unités :\n";

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

    // Afficher le contenu du fichier
    cout << "\nContenu du fichier '" << nomFichier << "' :\n";
    ifstream fichierLecture(nomFichier);
    string ligne;
    if (fichierLecture.is_open()) {
        while (getline(fichierLecture, ligne)) {
            cout << ligne << '\n';
        }
        fichierLecture.close();
    } else {
        cout << "Erreur lors de l'ouverture du fichier pour lecture.\n";
    }
}

int main() {
    double R = 0.0, f0 = 0.0; // Résistance et fréquence centrale
    double L = 0.0, C = 0.0;  // Inductance et capacité
    int choix;

    do {
        afficherMenu();
        cin >> choix;

        switch (choix) {
            case 1:
                // Saisir R et f0 avec gestion des erreurs
                R = saisirValeurPositive("la résistance R (Ohms)");
                f0 = saisirFrequence();
                cout << "Valeurs enregistrées avec succès :\n";
                cout << "R = " << R << " Ohms, f0 = " << f0 << " Hz.\n";
                break;

            case 2:
                if (R == 0 || f0 == 0) {
                    cout << "Veuillez d'abord entrer les valeurs valides de R et f0.\n";
                } else {
                    // Calcul des valeurs de L et C
                    double omega0 = 2 * M_PI * f0; // Calcul de ω0
                    C = 1 / (R * omega0);         // C = 1 / (R * ω0)
                    L = 1 / (pow(omega0, 2) * C); // L = 1 / (ω0^2 * C)

                    // Affichage des résultats
                    cout << fixed << setprecision(12);
                    cout << "Pour R = " << R << " Ohms et f0 = " << f0 << " Hz :\n";
                    cout << "Inductance L = " << L << " H\n";
                    cout << "Capacité C = " << C << " F\n";
                }
                break;

            case 3:
                if (L == 0 || C == 0) {
                    cout << "Veuillez d'abord calculer les valeurs de L et C.\n";
                } else {
                    // Afficher les conversions des unités
                    afficherConversions(L, C);
                }
                break;

            case 4:
                if (L == 0 || C == 0) {
                    cout << "Veuillez d'abord calculer les valeurs de L et C.\n";
                } else {
                    // Exporter les résultats dans un fichier
                    exporterResultats(R, f0, L, C);
                }
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
