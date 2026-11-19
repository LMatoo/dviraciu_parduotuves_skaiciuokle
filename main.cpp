#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

const int MAX = 100;
bool tinkamaKategorija(string kategorija) {
    if (kategorija == "Dviraciai" || kategorija == "Dalys" || kategorija == "Aksesuarai") {
        return true;
    }
    return false;
}

double eilutesSuma(double kaina, int kiekis) {
    return kaina * kiekis;
}

double nuolaida(double suma, int kiekis) {
    if (kiekis >= 10) {
        return suma * 0.10;
    }
    return 0;
}

int main() {
    string kategorijos[MAX];
    string pavadinimai[MAX];
    double kainos[MAX];
    int kiekiai[MAX];

    ifstream failas("uzsakymas.txt");
    if (!failas.is_open()) {
        cout << "Klaida: nepavyko atidaryti failo uzsakymas.txt" << endl;
        return 1;
    }

    int n;
    failas >> n;
    cout << "Prekiu skaicius faile: " << n << endl;

    if (failas.fail() || n < 1 || n > MAX) {
        cout << "Klaida: prekiu skaicius turi buti nuo 1 iki " << MAX << endl;
        return 1;
    }


    for (int i = 0; i < n; i++) {
        char kabliataskis;
        failas >> ws;
        //getline, nes pavadinime yra tarpu, o >> sustotų ant pirmo tarpo.
        getline(failas, kategorijos[i], ';');
        getline(failas, pavadinimai[i], ';');
        //skaiciams
        failas >> kainos[i] >> kabliataskis >> kiekiai[i];

        if (failas.fail() || kabliataskis != ';') {
            cout << "Klaida: nepavyko nuskaityti " << i + 1 << "-o iraso." << endl;
            return 1;
        }
        if (!tinkamaKategorija(kategorijos[i])) {
            cout << "Klaida: " << i + 1 << "-ame irase netinkama kategorija." << endl;
            return 1;
        }
        if (kainos[i] <= 0) {
            cout << "Klaida: " << i + 1 << "-ame irase kaina turi buti teigiama." << endl;
            return 1;
        }
        if (kiekiai[i] <= 0) {
            cout << "Klaida: " << i + 1 << "-ame irase kiekis turi buti teigiamas." << endl;
            return 1;
        }
    }

    cout << fixed << setprecision(2);

        for (int i = 0; i < n; i++) {
            double suma = eilutesSuma(kainos[i], kiekiai[i]);
            double nuol = nuolaida(suma, kiekiai[i]);
            cout << kategorijos[i] << " | " << pavadinimai[i] << " | " << kainos[i] << " | " << kiekiai[i] << " | " << suma << " | " << nuol << " | " << suma - nuol << endl;
        }
        failas.close();
    return 0;
}
