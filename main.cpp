#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int MAX = 100;

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

    failas.close();

    for (int i = 0; i < n; i++) {
        char kabliataskis;
        failas >> ws;
        //getline, nes pavadinime yra tarpu, o >> sustotų ant pirmo tarpo.
        getline(failas, kategorijos[i], ';');
        getline(failas, pavadinimai[i], ';');
        //skaiciams
        failas >> kainos[i] >> kabliataskis >> kiekiai[i];

        for (int i = 0; i < n; i++) {
            cout << kategorijos[i] << " | " << pavadinimai[i] << endl;
        }
    }

    return 0;
}
