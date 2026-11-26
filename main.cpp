#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

const int MAX_PREKIU = 100;   // didziausias prekiu skaicius

// ===================== PATIKRINIMAS =====================

// Ar kategorija yra viena is triju?
bool tinkamaKategorija(string kategorija) {
    if (kategorija == "Dviraciai" || kategorija == "Dalys" || kategorija == "Aksesuarai") {
        return true;
    }
    return false;
}

// ===================== NUSKAITYMAS =====================

// 1.Nuskaito uzsakymas.txt, grazina prekiu skaiciu (0 prekiu = klaida)
int nuskaitytiDuomenis(string kategorijos[], string pavadinimai[], double kainos[], int kiekiai[]) {
    // 1. Failo atidarymas
    ifstream failas("uzsakymas.txt");
    if (!failas.is_open()) {
        cout << "Klaida: nepavyko atidaryti failo uzsakymas.txt" << endl;
        return 0;
    }

    // 2. Prekiu skaicius (pirma eilute) ir jo riba
    int prekiuSkaicius;
    failas >> prekiuSkaicius; //nuskaito iki pirmo tarpo

    if (failas.fail() || prekiuSkaicius < 1 || prekiuSkaicius > MAX_PREKIU) {
        cout << "Klaida: prekiu skaicius turi buti nuo 1 iki " << MAX_PREKIU << endl;
        return 0;
    }

    // 3. Prekiu nuskaitymas ir tikrinimas
    // Eilute: kategorija;pavadinimas;kaina;kiekis
    for (int i = 0; i < prekiuSkaicius; i++) {
        char kabliataskis;   // ; tarp kainos ir kiekio
        failas >> ws;        // praleidzia Enter
        // getline, nes pavadinime yra tarpu
        getline(failas, kategorijos[i], ';'); //(failas - is kur?, kategorijos[i] - kas?, ';' - kur sustoti skaityti
        getline(failas, pavadinimai[i], ';');
        failas >> kainos[i] >> kabliataskis >> kiekiai[i]; // >> nuskaito tik kintamojo tipui priklausancia info pvz int ar double

        if (failas.fail() || kabliataskis != ';') {
            cout << "Klaida: nepavyko nuskaityti " << i + 1 << "-o iraso." << endl;
            return 0;
        }
        if (!tinkamaKategorija(kategorijos[i])) {
            cout << "Klaida: " << i + 1 << "-ame irase netinkama kategorija." << endl;
            return 0;
        }
        if (kainos[i] <= 0) {
            cout << "Klaida: " << i + 1 << "-ame irase kaina turi buti teigiama." << endl;
            return 0;
        }
        if (kiekiai[i] <= 0) {
            cout << "Klaida: " << i + 1 << "-ame irase kiekis turi buti teigiamas." << endl;
            return 0;
        }
    }
    failas.close();
    return prekiuSkaicius;
}

// ===================== SKAICIAVIMAI =====================

// Eilutes suma: kaina * kiekis
double eilutesSuma(double vienetoKaina, int uzsakytasKiekis) {
    return vienetoKaina * uzsakytasKiekis;
}

// Nuolaida: 10 %, jei kiekis >= 10, kitaip 0
double nuolaida(double vienosEilutesSuma, int uzsakytasKiekis) {
    if (uzsakytasKiekis >= 10) {
        return vienosEilutesSuma * 0.10;
    }
    return 0;
}

// Viso uzsakymo suma pries nuolaidas: sudeda visas eilutes
double bendraSumaPriesNuolaidas(double kainos[], int kiekiai[], int prekiuSkaicius) {
    double visuPrekiuSuma = 0;
    for (int i = 0; i < prekiuSkaicius; i++) {
        double vienosEilutesSuma = eilutesSuma(kainos[i], kiekiai[i]);
        visuPrekiuSuma = visuPrekiuSuma + vienosEilutesSuma;
    }
    return visuPrekiuSuma;
}

// Visu nuolaidu suma: sudeda visas nuolaidas
double bendraNuolaida(double kainos[], int kiekiai[], int prekiuSkaicius) {
    double visuNuolaiduSuma = 0;
    for (int i = 0; i < prekiuSkaicius; i++) {
        double vienosEilutesSuma = eilutesSuma(kainos[i], kiekiai[i]);
        double vienosPrekesNuolaida = nuolaida(vienosEilutesSuma, kiekiai[i]);
        visuNuolaiduSuma = visuNuolaiduSuma + vienosPrekesNuolaida;
    }
    return visuNuolaiduSuma;
}

// Kategorijos moketina suma po nuolaidu (jei prekiu nera, 0)
double kategorijosSuma(string kategorijos[], double kainos[], int kiekiai[],
                       int prekiuSkaicius, string pasirinktaKategorija) {
    double kategorijosMoketinaSuma = 0;
    for (int i = 0; i < prekiuSkaicius; i++) {
        if (kategorijos[i] == pasirinktaKategorija) {   // tik tos kategorijos prekes
            double vienosEilutesSuma = eilutesSuma(kainos[i], kiekiai[i]);
            double vienosPrekesNuolaida = nuolaida(vienosEilutesSuma, kiekiai[i]);
            double sumaPoNuolaidos = vienosEilutesSuma - vienosPrekesNuolaida;
            kategorijosMoketinaSuma = kategorijosMoketinaSuma + sumaPoNuolaidos;
        }
    }
    return kategorijosMoketinaSuma;
}

// ===================== MENIU PAGALBA =====================

// Klausia kategorijos, kol ivesta teisingai
string pasirinktiKategorija() {
    char kategorijosPasirinkimas;
    while (true) {
        cout << "1. Dviraciai" << endl;
        cout << "2. Dalys" << endl;
        cout << "3. Aksesuarai" << endl;
        cout << "Pasirinkite kategorija: ";
        cin >> kategorijosPasirinkimas;
        cin.ignore(1000, '\n');   // ignoruoja likusi teksta

        switch (kategorijosPasirinkimas) {
            case '1': return "Dviraciai";
            case '2': return "Dalys";
            case '3': return "Aksesuarai";
            default:
                cout << "Netinkamas pasirinkimas, irasykite nuo 1-3" << endl;
        }
    }
}

// Kiek prekiu yra kategorijoje
int kiekPrekiuKategorijoje(string kategorijos[], int prekiuSkaicius, string pasirinktaKategorija) {
    int prekiuKiekis = 0;
    for (int i = 0; i < prekiuSkaicius; i++) {
        if (kategorijos[i] == pasirinktaKategorija) {
            prekiuKiekis = prekiuKiekis + 1;
        }
    }
    return prekiuKiekis;
}

// ===================== SPAUSDINIMAS IR FAILAS =====================

// Prekiu lentele: visos prekes arba tik kategorijos ("" = visos)
// "out" = cout (ekranas) arba failas
void spausdintiPrekiuLentele(ostream& out, string kategorijos[], string pavadinimai[],
                             double kainos[], int kiekiai[], int prekiuSkaicius,
                             string pasirinktaKategorija) {
    out << fixed << setprecision(2);

    out << left << setw(12) << "Kategorija" << setw(48) << "Pavadinimas"
        << right << setw(10) << "Kaina" << setw(8) << "Kiekis"
        << setw(14) << "Pries nuol." << setw(11) << "Nuolaida"
        << setw(12) << "Moketina" << endl;
    out << string(115, '-') << endl;

    for (int i = 0; i < prekiuSkaicius; i++) {
        if (pasirinktaKategorija == "" || kategorijos[i] == pasirinktaKategorija) {
            double vienosEilutesSuma = eilutesSuma(kainos[i], kiekiai[i]);
            double vienosPrekesNuolaida = nuolaida(vienosEilutesSuma, kiekiai[i]);
            double vienosPrekesSumaPoNuolaidos = vienosEilutesSuma - vienosPrekesNuolaida;
            out << left << setw(12) << kategorijos[i] << setw(48) << pavadinimai[i]
                << right << setw(10) << kainos[i] << setw(8) << kiekiai[i]
                << setw(14) << vienosEilutesSuma << setw(11) << vienosPrekesNuolaida
                << setw(12) << vienosPrekesSumaPoNuolaidos << endl;
        }
    }
}

// Visa ataskaita: lentele, viso uzsakymo sumos, kategoriju suvestine
void spausdintiAtaskaita(ostream& out, string kategorijos[], string pavadinimai[],
                         double kainos[], int kiekiai[], int prekiuSkaicius) {
    out << fixed << setprecision(2);

    out << "=========== UZSAKYMO ATASKAITA ===========" << endl << endl;

    // 1. Prekiu lentele
    spausdintiPrekiuLentele(out, kategorijos, pavadinimai, kainos, kiekiai, prekiuSkaicius, "");

    // 2. Viso uzsakymo sumos
    double sumaPriesNuolaidas = bendraSumaPriesNuolaidas(kainos, kiekiai, prekiuSkaicius);
    double nuolaiduSuma = bendraNuolaida(kainos, kiekiai, prekiuSkaicius);
    double galutineSuma = sumaPriesNuolaidas - nuolaiduSuma;

    out << endl << "--- Viso uzsakymo sumos ---" << endl;
    out << "Suma pries nuolaidas:   " << setw(10) << sumaPriesNuolaidas << " Eur" << endl;
    out << "Nuolaidu suma:          " << setw(10) << nuolaiduSuma << " Eur" << endl;
    out << "Galutine moketina suma: " << setw(10) << galutineSuma << " Eur" << endl;

    // 3. Kategoriju suvestine
    out << endl << "--- Sumos pagal kategorijas ---" << endl;
    out << left << setw(12) << "Dviraciai" << right << setw(10)
        << kategorijosSuma(kategorijos, kainos, kiekiai, prekiuSkaicius, "Dviraciai") << " Eur" << endl;
    out << left << setw(12) << "Dalys" << right << setw(10)
        << kategorijosSuma(kategorijos, kainos, kiekiai, prekiuSkaicius, "Dalys") << " Eur" << endl;
    out << left << setw(12) << "Aksesuarai" << right << setw(10)
        << kategorijosSuma(kategorijos, kainos, kiekiai, prekiuSkaicius, "Aksesuarai") << " Eur" << endl;
}

// Iraso ataskaita i ataskaita.txt (true = pavyko)
bool issaugotiAtaskaita(string kategorijos[], string pavadinimai[],
                        double kainos[], int kiekiai[], int prekiuSkaicius) {
    ofstream ataskaitosFailas("ataskaita.txt");   // senas turinys perrasomas
    if (!ataskaitosFailas.is_open()) {
        cout << "Klaida: nepavyko atidaryti failo ataskaita.txt rasymui." << endl;
        return false;
    }

    spausdintiAtaskaita(ataskaitosFailas, kategorijos, pavadinimai, kainos, kiekiai, prekiuSkaicius);

    if (ataskaitosFailas.fail()) {   // ar pavyko irasyti
        cout << "Klaida: nepavyko irasyti duomenu i faila ataskaita.txt." << endl;
        return false;
    }
    ataskaitosFailas.close();
    return true;
}

// ===================== PAGRINDINE PROGRAMA =====================

int main() {
    // 1. Masyvai (vienodas indeksas = ta pati preke)
    string kategorijos[MAX_PREKIU];
    string pavadinimai[MAX_PREKIU];
    double kainos[MAX_PREKIU];
    int kiekiai[MAX_PREKIU];

    // 2. Duomenu nuskaitymas (0 prekiu = klaida)
    int prekiuSkaicius = nuskaitytiDuomenis(kategorijos, pavadinimai, kainos, kiekiai);
    if (prekiuSkaicius == 0) {
        cout << "Ivyko klaida nuskaitant duomenis! Programa baigia darba." << endl;
        return 1;
    }
    cout << "Prekiu skaicius faile: " << prekiuSkaicius << endl;

    cout << fixed << setprecision(2);   // 2 skaitmenys po kablelio

    // 3. Meniu (kartojasi iki 7)
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
        cin.ignore(1000, '\n');   // ignoruoja likusi teksta: pvz. jei irasyta 12, 2 ignoruojamas
        cout << endl;

        switch (pasirinkimas) {
            // 1 - visos prekes
            case '1':
                spausdintiPrekiuLentele(cout, kategorijos, pavadinimai, kainos, kiekiai, prekiuSkaicius, "");
                break;

            // 2 - pasirinktos kategorijos prekes ir suma
            case '2': {
                string pasirinktaKategorija = pasirinktiKategorija();
                int prekiuKategorijoje = kiekPrekiuKategorijoje(kategorijos, prekiuSkaicius, pasirinktaKategorija);

                if (prekiuKategorijoje == 0) {
                    cout << "Sioje kategorijoje uzsakyme prekiu nera. Suma: 0.00 Eur" << endl;
                } else {
                    spausdintiPrekiuLentele(cout, kategorijos, pavadinimai, kainos, kiekiai, prekiuSkaicius, pasirinktaKategorija);
                    cout << endl;
                    cout << "Kategorijos " << pasirinktaKategorija << " moketina suma: "
                         << kategorijosSuma(kategorijos, kainos, kiekiai, prekiuSkaicius, pasirinktaKategorija) << " Eur" << endl;
                }
                break;
            }

            // 3 - viso uzsakymo sumos
            case '3': {
                double sumaPriesNuolaidas = bendraSumaPriesNuolaidas(kainos, kiekiai, prekiuSkaicius);
                double nuolaiduSuma = bendraNuolaida(kainos, kiekiai, prekiuSkaicius);
                double galutineSuma = sumaPriesNuolaidas - nuolaiduSuma;

                cout << "Suma pries nuolaidas: " << sumaPriesNuolaidas << endl;
                cout << "Nuolaidu suma: " << nuolaiduSuma << endl;
                cout << "Galutine suma: " << galutineSuma << endl;
                break;
            }

            // 4 - sumos pagal kategorijas
            case '4':
                cout << "Dviraciai: " << kategorijosSuma(kategorijos, kainos, kiekiai, prekiuSkaicius, "Dviraciai") << endl;
                cout << "Dalys: " << kategorijosSuma(kategorijos, kainos, kiekiai, prekiuSkaicius, "Dalys") << endl;
                cout << "Aksesuarai: " << kategorijosSuma(kategorijos, kainos, kiekiai, prekiuSkaicius, "Aksesuarai") << endl;
                break;

            // 5 - visa ataskaita ekrane
            case '5':
                spausdintiAtaskaita(cout, kategorijos, pavadinimai, kainos, kiekiai, prekiuSkaicius);
                break;

            // 6 - ataskaita i faila (sekme rodoma tik jei pavyko)
            case '6':
                if (issaugotiAtaskaita(kategorijos, pavadinimai, kainos, kiekiai, prekiuSkaicius)) {
                    cout << "Ataskaita sekmingai issaugota faile ataskaita.txt" << endl;
                }
                break;

            // 7 - iseiti
            case '7':
                cout << "Viso gero!" << endl;
                break;

            // netinkamas pasirinkimas - meniu rodomas is naujo
            default:
                cout << "Netinkamas pasirinkimas, irasykite nuo 1-7." << endl;
        }
    } while (pasirinkimas != '7');

    return 0;
}