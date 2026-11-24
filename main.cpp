#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

const int MAX_PREKIU = 100;

//**Funkcijos**

// Ar kategorija yra viena is triju leidziamu?
bool tinkamaKategorija(string kategorija) {
    if (kategorija == "Dviraciai" || kategorija == "Dalys" || kategorija == "Aksesuarai") {
        return true;
    }
    return false;
}

// Vienos prekes suma pries nuolaida: vieneto kaina * kiekis
double eilutesSuma(double vienetoKaina, int uzsakytasKiekis) {
    return vienetoKaina * uzsakytasKiekis;
}

// Nuolaida eurais: 10 %, jei uzsakyta bent 10 vienetu, kitaip 0
double nuolaida(double vienosEilutesSuma, int uzsakytasKiekis) {
    if (uzsakytasKiekis >= 10) {
        return vienosEilutesSuma * 0.10;
    }
    return 0;
}

// Viso uzsakymo suma pries nuolaidas
double bendraSumaPriesNuolaidas(double kainos[], int kiekiai[], int prekiuSkaicius) {
    double visuPrekiuSuma = 0;
    for (int i = 0; i < prekiuSkaicius; i++) {
        // Vienos prekes suma pries nuolaida: vieneto kaina * kiekis
        double vienosEilutesSuma = eilutesSuma(kainos[i], kiekiai[i]);
        visuPrekiuSuma = visuPrekiuSuma + vienosEilutesSuma;
    }
    return visuPrekiuSuma;
}

// Visu nuolaidu suma
double bendraNuolaida(double kainos[], int kiekiai[], int prekiuSkaicius) {
    double visuNuolaiduSuma = 0;
    for (int i = 0; i < prekiuSkaicius; i++) {
        // Vienos prekes suma pries nuolaida: vieneto kaina * kiekis
        double vienosEilutesSuma = eilutesSuma(kainos[i], kiekiai[i]);
        // Nuolaida eurais: 10 %, jei uzsakyta bent 10 vienetu, kitaip 0
        double vienosPrekesNuolaida = nuolaida(vienosEilutesSuma, kiekiai[i]);
        visuNuolaiduSuma = visuNuolaiduSuma + vienosPrekesNuolaida;
    }
    return visuNuolaiduSuma;
}

double kategorijosSuma(string kategorijos[], double kainos[], int kiekiai[],
    int prekiuSkaicius, string pasirinktaKategorija) {
    double kategorijosMoketinaSuma = 0;
    for (int i = 0; i < prekiuSkaicius; i++) {
        if (kategorijos[i] == pasirinktaKategorija) {
            // Vienos prekes suma pries nuolaida: vieneto kaina * kiekis
            double vienosEilutesSuma = eilutesSuma(kainos[i], kiekiai[i]);
            // Nuolaida eurais: 10 %, jei uzsakyta bent 10 vienetu, kitaip 0
            double vienosPrekesNuolaida = nuolaida(vienosEilutesSuma, kiekiai[i]);
            double sumaPoNuolaidos = vienosEilutesSuma - vienosPrekesNuolaida;
            kategorijosMoketinaSuma = kategorijosMoketinaSuma + sumaPoNuolaidos;
        }
    }
    return kategorijosMoketinaSuma;

    }

int main() {
    string kategorijos[MAX_PREKIU];
    string pavadinimai[MAX_PREKIU];
    double kainos[MAX_PREKIU];
    int kiekiai[MAX_PREKIU];

    ifstream failas("uzsakymas.txt");
    if (!failas.is_open()) {
        cout << "Klaida: nepavyko atidaryti failo uzsakymas.txt" << endl;
        return 1;
    }

    int prekiuSkaicius;
    failas >> prekiuSkaicius;
    cout << "Prekiu skaicius faile: " << prekiuSkaicius << endl;

    if (failas.fail() || prekiuSkaicius < 1 || prekiuSkaicius > MAX_PREKIU) {
        cout << "Klaida: prekiu skaicius turi buti nuo 1 iki " << MAX_PREKIU << endl;
        return 1;
    }

    for (int i = 0; i < prekiuSkaicius; i++) {
        char kabliataskis;
        failas >> ws;
        // getline, nes pavadinime yra tarpu, o >> sustotu ties pirmu tarpu
        getline(failas, kategorijos[i], ';');
        getline(failas, pavadinimai[i], ';');
        // skaiciai
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
    failas.close();

    cout << fixed << setprecision(2);

    char pasirinkimas;
    do {
        cout << endl;
        cout << "========= MENIU ==========" << endl;
        cout << "1. Perziureti visas uzsakymo prekes" << endl;
        cout << "2. Perziureti pasirinktos kategorijos prekes" << endl;
        cout << "3. Perziureti viso uzsakymo sumas" << endl;
        cout << "4. Perziureti sumas pagal kategorijas" << endl;
        cout << "5. Parodyti visa uzsakymo ataskaita" << endl;
        cout << "6. Issaugoti visa ataskaita i faila" << endl;
        cout << "7. Iseiti" << endl;
        cout << "Jusu pasirinkimas: ";
        cin >> pasirinkimas;
        cin.ignore(1000, '\n');
        cout << endl;

        switch (pasirinkimas) {
            case '1':
                // Kiekvienos prekes eilutes spausdinimas
                for (int i = 0; i < prekiuSkaicius; i++) {
                    double vienosEilutesSuma = eilutesSuma(kainos[i], kiekiai[i]);
                    double vienosPrekesNuolaida = nuolaida(vienosEilutesSuma, kiekiai[i]);
                    double vienosPrekesSumaPoNuolaidos = vienosEilutesSuma - vienosPrekesNuolaida;
                    cout << "Kategorija: " << kategorijos[i] << " | " << "Pavadinimas: " << pavadinimai[i] << " | "
                         << "Vnt. Kaina: " << kainos[i] << " | " << "Kiekis: " << kiekiai[i] << " | "
                         << "Suma pries nuolaida: " << vienosEilutesSuma << " | " << "Prekiu nuolaidos: " << vienosPrekesNuolaida << " | "
                         << "Galutine suma: " << vienosPrekesSumaPoNuolaidos << "\n" << endl;
                }
                break;
            case '2':
                cout << "Pasirinktos kategorijos prekes" << endl;
                break;
            case '3': {
                // Viso uzsakymo sumos
                double sumaPriesNuolaidas = bendraSumaPriesNuolaidas(kainos, kiekiai, prekiuSkaicius);
                double nuolaiduSuma = bendraNuolaida(kainos, kiekiai, prekiuSkaicius);
                double galutineSuma = sumaPriesNuolaidas - nuolaiduSuma;

                cout << endl;
                cout << "Suma pries nuolaidas: " << sumaPriesNuolaidas << endl;
                cout << "Nuolaidu suma: " << nuolaiduSuma << endl;
                cout << "Galutine suma: " << galutineSuma << endl;
                cout << endl;
                break;
            }
            case '4':
                cout << "Dviraciai: " << kategorijosSuma(kategorijos, kainos, kiekiai, prekiuSkaicius, "Dviraciai") << endl;
                cout << "Dalys: " << kategorijosSuma(kategorijos, kainos, kiekiai, prekiuSkaicius, "Dalys") << endl;
                cout << "Aksesuarai: " << kategorijosSuma(kategorijos, kainos, kiekiai, prekiuSkaicius, "Aksesuarai") << endl;
                break;
            case '5':
                cout << "Ataskaita" << endl;
                break;
            case '6':
                cout << "Ataskaitos issaugojimas" << endl;
                break;
            case '7':
                cout << "Viso gero!" << endl;
                break;

            default:
                cout << "Netinkamas pasirinkimas, irasykite nuo 1-7." << endl;
        }
    } while (pasirinkimas != '7');

    return 0;
}