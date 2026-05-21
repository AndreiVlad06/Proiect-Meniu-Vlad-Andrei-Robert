#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"

typedef struct {
    int id;
    char nume[50];
    int capacitate;
    float pret;
    char facilitati[100];
} Sala;

typedef struct {
    int id_rezervare;
    int id_sala;
    char nume_client[50];
    int zi;
    int luna;
    int an;
    int zile;
    float pret_total;
    int tip_meniu;
} Rezervare;

void incarca_sali(Sala sali[], int *nr_sali);
void incarca_rezervari(Rezervare rezervari[], int *nr_rezervari);
void salveaza_rezervari(Rezervare rezervari[], int nr_rezervari);

int valideaza_data(int zi, int luna, int an);
int calculeaza_zile_ramase(int zi, int luna, int an, int zile_totale);
int se_suprapun(int z1, int l1, int a1, int zile1, int z2, int l2, int a2, int zile2);

void afisare_meniu();
void afisare_toate_salile(Sala sali[], int numar_sali);
void cautare_sala(Sala sali[], int numar_sali);
void efectuare_rezervare(Sala sali[], int numar_sali, Rezervare rezervari[], int *numar_rezervari);
void anulare_rezervare(Rezervare rezervari[], int *numar_rezervari, Sala sali[], int numar_sali);
void afisare_detalii_rezervare(Rezervare rezervari[], int numar_rezervari, Sala sali[], int numar_sali);

void curata_ecranul();
void pauza();
void transforma_litere_mici(char *str);

int main() {
    int optiune;
    Sala sali[100];
    int numar_sali = 0;
    Rezervare rezervari[200];
    int numar_rezervari = 0;

    incarca_sali(sali, &numar_sali);
    incarca_rezervari(rezervari, &numar_rezervari);

    do {
        curata_ecranul();
        afisare_meniu();
        printf(YELLOW "\nAlegeti o optiune: " RESET);

        if (scanf("%d", &optiune) != 1) {
            printf(RED "Eroare: Va rugam introduceti un numar!\n" RESET);
            while(getchar() != '\n');
            pauza();
            continue;
        }

        switch (optiune) {
            case 1:
                afisare_toate_salile(sali, numar_sali);
                break;
            case 2:
                cautare_sala(sali, numar_sali);
                break;
            case 3:
                efectuare_rezervare(sali, numar_sali, rezervari, &numar_rezervari);
                break;
            case 4:
                anulare_rezervare(rezervari, &numar_rezervari, sali, numar_sali);
                break;
            case 5:
                afisare_detalii_rezervare(rezervari, numar_rezervari, sali, numar_sali);
                break;
            case 0:
                curata_ecranul();
                printf(CYAN BOLD "=== IESIRE ===\n" RESET);
                printf(GREEN "Iesire din aplicatie. O zi frumoasa!\n" RESET);
                break;
            default:
                printf(RED "Optiune invalida! Va rugam incercati din nou.\n" RESET);
                pauza();
        }
    } while (optiune != 0);

    return 0;
}

int valideaza_data(int zi, int luna, int an) {
    if (zi < 1 || zi > 31 || luna < 1 || luna > 12 || an < 2024) return 0;

    time_t t = time(NULL);
    struct tm tm_azi = *localtime(&t);
    struct tm tm_azi_mid = {0};
    tm_azi_mid.tm_year = tm_azi.tm_year;
    tm_azi_mid.tm_mon = tm_azi.tm_mon;
    tm_azi_mid.tm_mday = tm_azi.tm_mday;
    time_t time_azi = mktime(&tm_azi_mid);

    struct tm tm_rez = {0};
    tm_rez.tm_year = an - 1900;
    tm_rez.tm_mon = luna - 1;
    tm_rez.tm_mday = zi;
    time_t time_start = mktime(&tm_rez);

    if (difftime(time_start, time_azi) >= 86400.0) {
        return 1;
    }
    return 0;
}

int calculeaza_zile_ramase(int zi, int luna, int an, int zile_totale) {
    time_t t = time(NULL);
    struct tm tm_azi = *localtime(&t);
    struct tm tm_azi_mid = {0};
    tm_azi_mid.tm_year = tm_azi.tm_year;
    tm_azi_mid.tm_mon = tm_azi.tm_mon;
    tm_azi_mid.tm_mday = tm_azi.tm_mday;
    time_t time_azi = mktime(&tm_azi_mid);

    struct tm tm_rez = {0};
    tm_rez.tm_year = an - 1900;
    tm_rez.tm_mon = luna - 1;
    tm_rez.tm_mday = zi;
    time_t time_start = mktime(&tm_rez);

    struct tm tm_end = tm_rez;
    tm_end.tm_mday += zile_totale;
    time_t time_end = mktime(&tm_end);

    if (difftime(time_start, time_azi) > 0) {
        return zile_totale;
    } else if (difftime(time_end, time_azi) > 0) {
        return (int)(difftime(time_end, time_azi) / 86400.0);
    } else {
        return 0;
    }
}

int se_suprapun(int z1, int l1, int a1, int zile1, int z2, int l2, int a2, int zile2) {
    struct tm tm1 = {0}; tm1.tm_year = a1-1900; tm1.tm_mon = l1-1; tm1.tm_mday = z1;
    time_t start1 = mktime(&tm1);
    struct tm tme1 = tm1; tme1.tm_mday += zile1;
    time_t end1 = mktime(&tme1);

    struct tm tm2 = {0}; tm2.tm_year = a2-1900; tm2.tm_mon = l2-1; tm2.tm_mday = z2;
    time_t start2 = mktime(&tm2);
    struct tm tme2 = tm2; tme2.tm_mday += zile2;
    time_t end2 = mktime(&tme2);

    if (start1 < end2 && start2 < end1) return 1;
    return 0;
}

void incarca_sali(Sala sali[], int *nr_sali) {
    *nr_sali = 0;
    FILE *fisier = fopen("sali.txt", "r");
    if (fisier != NULL) {
        while (fscanf(fisier, "%d %49s %d %f %99s", &sali[*nr_sali].id, sali[*nr_sali].nume, &sali[*nr_sali].capacitate, &sali[*nr_sali].pret, sali[*nr_sali].facilitati) == 5) {
            (*nr_sali)++;
        }
        fclose(fisier);
    }
}

void incarca_rezervari(Rezervare rezervari[], int *nr_rezervari) {
    *nr_rezervari = 0;
    FILE *fisier = fopen("rezervari.txt", "r");
    if (fisier != NULL) {
        while (fscanf(fisier, "%d %d %49s %d %d %d %d %f %d", &rezervari[*nr_rezervari].id_rezervare, &rezervari[*nr_rezervari].id_sala, rezervari[*nr_rezervari].nume_client, &rezervari[*nr_rezervari].zi, &rezervari[*nr_rezervari].luna, &rezervari[*nr_rezervari].an, &rezervari[*nr_rezervari].zile, &rezervari[*nr_rezervari].pret_total, &rezervari[*nr_rezervari].tip_meniu) == 9) {
            (*nr_rezervari)++;
        }
        fclose(fisier);
    }
}

void salveaza_rezervari(Rezervare rezervari[], int nr_rezervari) {
    FILE *fisier = fopen("rezervari.txt", "w");
    if (fisier != NULL) {
        for (int i = 0; i < nr_rezervari; i++) {
            fprintf(fisier, "%d %d %s %d %d %d %d %.2f %d\n",
                    rezervari[i].id_rezervare,
                    rezervari[i].id_sala,
                    rezervari[i].nume_client,
                    rezervari[i].zi,
                    rezervari[i].luna,
                    rezervari[i].an,
                    rezervari[i].zile,
                    rezervari[i].pret_total,
                    rezervari[i].tip_meniu);
        }
        fclose(fisier);
    }
}

void curata_ecranul() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauza() {
    printf("\nApasati " BOLD "ENTER" RESET " pentru a va intoarce...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    getchar();
}

void transforma_litere_mici(char *str) {
    for(int i = 0; str[i]; i++){
        str[i] = tolower(str[i]);
    }
}

void afisare_meniu() {
    printf(CYAN BOLD "\n=======================================================\n" RESET);
    printf(YELLOW BOLD "           MENIU PRINCIPAL - REZERVARE SALI          \n" RESET);
    printf(CYAN BOLD "=======================================================\n" RESET);
    printf(GREEN BOLD " [1] " RESET "Afisare toate salile disponibile\n");
    printf(GREEN BOLD " [2] " RESET "Cautare sala " CYAN "(dupa capacitate / facilitati)\n" RESET);
    printf(GREEN BOLD " [3] " RESET "Efectuare rezervare noua\n");
    printf(RED BOLD   " [4] " RESET "Anulare rezervare\n");
    printf(GREEN BOLD " [5] " RESET "Afisare detalii rezervare existenta\n");
    printf(CYAN BOLD "-------------------------------------------------------\n" RESET);
    printf(RED BOLD   " [0] " RESET "Iesire\n");
    printf(CYAN BOLD "=======================================================\n" RESET);
}

void afisare_toate_salile(Sala sali[], int numar_sali) {
    curata_ecranul();
    printf(CYAN BOLD "=== SUBMENIU: AFISARE TOATE SALILE ===\n\n" RESET);

    if (numar_sali == 0) {
        printf(YELLOW "Nu exista sali incarcate sau fisierul este gol.\n" RESET);
    } else {
        printf(BOLD "%-5s | %-20s | %-12s | %-10s | %-20s\n" RESET, "ID", "Nume Sala", "Capacitate", "Pret", "Facilitati");
        printf("---------------------------------------------------------------------------------\n");
        for (int i = 0; i < numar_sali; i++) {
            printf("%-5d | %-20s | %-12d | %-10.2f | %-20s\n", sali[i].id, sali[i].nume, sali[i].capacitate, sali[i].pret, sali[i].facilitati);
        }
    }

    printf("\n");
    pauza();
}

void cautare_sala(Sala sali[], int numar_sali) {
    if (numar_sali == 0) {
        curata_ecranul();
        printf(RED "Eroare: Nu exista sali disponibile pentru cautare!\n" RESET);
        pauza();
        return;
    }

    int optiune_cautare;
    do {
        curata_ecranul();
        printf(CYAN BOLD "=== SUBMENIU: CAUTARE SALA ===\n" RESET);
        printf("1. Cautare dupa Nume\n");
        printf("2. Cautare dupa Capacitate\n");
        printf("3. Cautare dupa Pret\n");
        printf("4. Cautare dupa Facilitati\n");
        printf(RED "0. Inapoi la meniul principal\n" RESET);
        printf("\nAlegeti criteriul de cautare: ");

        if (scanf("%d", &optiune_cautare) != 1) {
            while(getchar() != '\n');
            printf(RED "Eroare: Va rugam introduceti un numar!\n" RESET);
            pauza();
            continue;
        }

        if (optiune_cautare == 0) {
            return;
        }

        char nume_cautat[50] = "";
        char facilitate_cautata[100] = "";
        int optiune_interval = 0;
        int inapoi = 0;

        switch(optiune_cautare) {
            case 1:
                printf("\nIntroduceti numele salii (sau " RED "0" RESET " pentru inapoi): ");
                scanf(" %49[^\n]", nume_cautat);
                if (strcmp(nume_cautat, "0") == 0) inapoi = 1;
                break;
            case 2:
                printf(YELLOW "\n--- Intervale Capacitate ---\n" RESET);
                printf("1. Sub 50 locuri\n");
                printf("2. 50 - 150 locuri\n");
                printf("3. 150 - 300 locuri\n");
                printf("4. Peste 300 locuri\n");
                printf(RED "0. Inapoi\n" RESET);
                printf("Alegeti intervalul dorit: ");
                if (scanf("%d", &optiune_interval) != 1) {
                    while(getchar() != '\n');
                    inapoi = 1;
                } else if (optiune_interval == 0) {
                    inapoi = 1;
                }
                break;
            case 3:
                printf(YELLOW "\n--- Intervale Pret ---\n" RESET);
                printf("1. Sub 1000 RON\n");
                printf("2. 1000 - 3000 RON\n");
                printf("3. Peste 3000 RON\n");
                printf(RED "0. Inapoi\n" RESET);
                printf("Alegeti intervalul dorit: ");
                if (scanf("%d", &optiune_interval) != 1) {
                    while(getchar() != '\n');
                    inapoi = 1;
                } else if (optiune_interval == 0) {
                    inapoi = 1;
                }
                break;
            case 4:
                printf(YELLOW "\nFacilitati frecvente: WiFi, Parcare, Scena, Bar, Catering, Proiector\n" RESET);
                printf("Introduceti facilitatile (sau " RED "0" RESET " pt inapoi): ");
                scanf(" %99[^\n]", facilitate_cautata);
                if (strcmp(facilitate_cautata, "0") == 0) inapoi = 1;
                break;
            default:
                printf(RED "\nOptiune invalida!\n" RESET);
                pauza();
                inapoi = 1;
                break;
        }

        if (inapoi == 1) {
            continue;
        }

        curata_ecranul();
        printf(CYAN BOLD "=== REZULTATE CAUTARE ===\n\n" RESET);

        int gasit = 0;
        int antet_afisat = 0;

        for (int i = 0; i < numar_sali; i++) {
            int se_potriveste = 0;

            if (optiune_cautare == 1) {
                char copie_nume_cautat[50];
                char copie_nume_sala[50];
                strcpy(copie_nume_cautat, nume_cautat);
                strcpy(copie_nume_sala, sali[i].nume);
                transforma_litere_mici(copie_nume_cautat);
                transforma_litere_mici(copie_nume_sala);
                if (strstr(copie_nume_sala, copie_nume_cautat) != NULL) {
                    se_potriveste = 1;
                }
            }
            else if (optiune_cautare == 2) {
                if (optiune_interval == 1 && sali[i].capacitate < 50) se_potriveste = 1;
                else if (optiune_interval == 2 && sali[i].capacitate >= 50 && sali[i].capacitate <= 150) se_potriveste = 1;
                else if (optiune_interval == 3 && sali[i].capacitate > 150 && sali[i].capacitate <= 300) se_potriveste = 1;
                else if (optiune_interval == 4 && sali[i].capacitate > 300) se_potriveste = 1;
            }
            else if (optiune_cautare == 3) {
                if (optiune_interval == 1 && sali[i].pret < 1000.0) se_potriveste = 1;
                else if (optiune_interval == 2 && sali[i].pret >= 1000.0 && sali[i].pret <= 3000.0) se_potriveste = 1;
                else if (optiune_interval == 3 && sali[i].pret > 3000.0) se_potriveste = 1;
            }
            else if (optiune_cautare == 4) {
                int toate_gasite = 1;
                char copie_input[100];
                char copie_facilitati_sala[100];
                strcpy(copie_input, facilitate_cautata);
                strcpy(copie_facilitati_sala, sali[i].facilitati);
                transforma_litere_mici(copie_input);
                transforma_litere_mici(copie_facilitati_sala);
                char *token = strtok(copie_input, " ,");
                while (token != NULL) {
                    if (strstr(copie_facilitati_sala, token) == NULL) {
                        toate_gasite = 0;
                        break;
                    }
                    token = strtok(NULL, " ,");
                }
                if (toate_gasite == 1) se_potriveste = 1;
            }

            if (se_potriveste == 1) {
                if (antet_afisat == 0) {
                    printf(BOLD "%-5s | %-20s | %-12s | %-10s | %-20s\n" RESET, "ID", "Nume Sala", "Capacitate", "Pret", "Facilitati");
                    printf("---------------------------------------------------------------------------------\n");
                    antet_afisat = 1;
                }

                printf("%-5d | %-20s | %-12d | %-10.2f | %-20s\n", sali[i].id, sali[i].nume, sali[i].capacitate, sali[i].pret, sali[i].facilitati);
                gasit = 1;
            }
        }

        if (gasit == 0) {
            printf(YELLOW "Nu s-au gasit sali care sa corespunda criteriilor tale.\n" RESET);
        }

        printf("\n");
        pauza();
        return;

    } while (1);
}

void efectuare_rezervare(Sala sali[], int numar_sali, Rezervare rezervari[], int *numar_rezervari) {
    if (numar_sali == 0) {
        curata_ecranul();
        printf(RED "Eroare: Nu exista sali disponibile!\n" RESET);
        pauza();
        return;
    }

    Rezervare noua_rezervare;

    int data_valida = 0;
    do {
        curata_ecranul();
        printf(YELLOW BOLD "=== PAS 1: DATA REZERVARII ===\n\n" RESET);
        printf("Introduceti data de inceput (ZZ LL AAAA): ");

        if (scanf("%d %d %d", &noua_rezervare.zi, &noua_rezervare.luna, &noua_rezervare.an) != 3) {
            while(getchar() != '\n');
            printf(RED "\nEroare: Format invalid!\n" RESET);
            pauza();
            continue;
        }

        if (valideaza_data(noua_rezervare.zi, noua_rezervare.luna, noua_rezervare.an)) {
            data_valida = 1;
        } else {
            printf(RED "\nEroare: O sala poate fi rezervata cu minim o zi inainte!\nData introdusa trebuie sa fie strict dupa ziua de azi.\n" RESET);
            pauza();
        }
    } while (data_valida == 0);

    curata_ecranul();
    printf(YELLOW BOLD "=== PAS 2: DURATA REZERVARII ===\n\n" RESET);
    printf("Data aleasa: " CYAN "%02d/%02d/%04d\n\n" RESET, noua_rezervare.zi, noua_rezervare.luna, noua_rezervare.an);
    printf("Introduceti perioada (numarul de zile): ");
    if (scanf("%d", &noua_rezervare.zile) != 1 || noua_rezervare.zile <= 0) {
        printf(RED "\nEroare: Numar invalid de zile!\n" RESET);
        while(getchar() != '\n');
        pauza();
        return;
    }

    char facilitati_unice[50][50];
    int nr_fac_unice = 0;
    for (int i = 0; i < numar_sali; i++) {
        char copie_fac[100];
        strcpy(copie_fac, sali[i].facilitati);
        char *tok = strtok(copie_fac, ",");
        while (tok != NULL) {
            int gasit_fac = 0;
            for (int j = 0; j < nr_fac_unice; j++) {
                if (strcmp(facilitati_unice[j], tok) == 0) {
                    gasit_fac = 1;
                    break;
                }
            }
            if (gasit_fac == 0) {
                strcpy(facilitati_unice[nr_fac_unice], tok);
                nr_fac_unice++;
            }
            tok = strtok(NULL, ",");
        }
    }

    curata_ecranul();
    int optiune_interval = 0;
    printf(YELLOW BOLD "=== PAS 3: SELECTARE CAPACITATE ===\n\n" RESET);
    printf("1. Sub 50 locuri\n2. 50 - 150 locuri\n3. 150 - 300 locuri\n4. Peste 300 locuri\n" RED "0. Orice capacitate\n\n" RESET);
    printf("Alegeti intervalul dorit: ");
    if (scanf("%d", &optiune_interval) != 1) {
        while(getchar() != '\n');
        optiune_interval = 0;
    }

    curata_ecranul();
    char facilitate_cautata[100] = "";
    printf(YELLOW BOLD "=== PAS 4: SELECTARE FACILITATI ===\n\n" RESET);
    printf("Facilitati posibile: (");
    for(int i = 0; i < nr_fac_unice; i++) {
        printf(CYAN "%s" RESET, facilitati_unice[i]);
        if (i < nr_fac_unice - 1) printf(", ");
    }
    printf(")\n\n");
    printf("Introduceti facilitatile dorite (sau " RED "0" RESET " pt orice facilitate): ");
    scanf(" %99[^\n]", facilitate_cautata);

    curata_ecranul();
    printf(YELLOW BOLD "=== PAS 5: CONFIRMARE SALA ===\n\n" RESET);

    int gasit_filtru = 0;
    int antet_afisat = 0;
    int id_valide[50];
    int numar_id_valide = 0;

    for (int i = 0; i < numar_sali; i++) {
        int se_potriveste = 1;

        for (int r = 0; r < *numar_rezervari; r++) {
            if (rezervari[r].id_sala == sali[i].id) {
                if (se_suprapun(noua_rezervare.zi, noua_rezervare.luna, noua_rezervare.an, noua_rezervare.zile,
                                rezervari[r].zi, rezervari[r].luna, rezervari[r].an, rezervari[r].zile)) {
                    se_potriveste = 0;
                    break;
                }
            }
        }

        if (se_potriveste == 1) {
            if (optiune_interval == 1 && sali[i].capacitate >= 50) se_potriveste = 0;
            else if (optiune_interval == 2 && (sali[i].capacitate < 50 || sali[i].capacitate > 150)) se_potriveste = 0;
            else if (optiune_interval == 3 && (sali[i].capacitate <= 150 || sali[i].capacitate > 300)) se_potriveste = 0;
            else if (optiune_interval == 4 && sali[i].capacitate <= 300) se_potriveste = 0;

            if (se_potriveste == 1 && strcmp(facilitate_cautata, "0") != 0) {
                int toate_gasite = 1;
                char copie_input[100];
                char copie_facilitati_sala[100];
                strcpy(copie_input, facilitate_cautata);
                strcpy(copie_facilitati_sala, sali[i].facilitati);
                transforma_litere_mici(copie_input);
                transforma_litere_mici(copie_facilitati_sala);
                char *token = strtok(copie_input, " ,");
                while (token != NULL) {
                    if (strstr(copie_facilitati_sala, token) == NULL) {
                        toate_gasite = 0;
                        break;
                    }
                    token = strtok(NULL, " ,");
                }
                if (toate_gasite == 0) se_potriveste = 0;
            }
        }

        if (se_potriveste == 1) {
            if (antet_afisat == 0) {
                printf(BOLD "%-5s | %-20s | %-12s | %-10s | %-20s\n" RESET, "ID", "Nume Sala", "Capacitate", "Pret", "Facilitati");
                printf("---------------------------------------------------------------------------------\n");
                antet_afisat = 1;
            }
            printf("%-5d | %-20s | %-12d | %-10.2f | %-20s\n", sali[i].id, sali[i].nume, sali[i].capacitate, sali[i].pret, sali[i].facilitati);
            id_valide[numar_id_valide] = sali[i].id;
            numar_id_valide++;
            gasit_filtru++;
        }
    }

    if (gasit_filtru == 0) {
        printf(RED "Ne pare rau, nicio sala nu este disponibila pentru datele si caracteristicile selectate.\n" RESET);
        pauza();
        return;
    }

    int id_sala_dorita;
    int id_este_valid = 0;

    printf("\n");
    do {
        printf("Introduceti ID-ul salii pe care doriti sa o rezervati (sau " RED "0" RESET " pt anulare): ");
        if (scanf("%d", &id_sala_dorita) != 1) {
            while(getchar() != '\n');
            printf(RED "Eroare: Va rugam introduceti un numar!\n" RESET);
            continue;
        }

        if (id_sala_dorita == 0) return;

        id_este_valid = 0;
        for(int i = 0; i < numar_id_valide; i++) {
            if(id_valide[i] == id_sala_dorita) {
                id_este_valid = 1;
                break;
            }
        }

        if (id_este_valid == 0) {
            printf(RED "Eroare: ID-ul %d nu se afla in lista recomandarilor de mai sus! Incercati din nou.\n" RESET, id_sala_dorita);
        }

    } while (id_este_valid == 0);

    int gasit = -1;
    for (int i = 0; i < numar_sali; i++) {
        if (sali[i].id == id_sala_dorita) {
            gasit = i;
            break;
        }
    }

    noua_rezervare.id_sala = id_sala_dorita;
    noua_rezervare.id_rezervare = (*numar_rezervari) + 1;

    curata_ecranul();
    printf(YELLOW BOLD "=== PAS 6: NUME CLIENT ===\n\n" RESET);
    printf("Ati selectat sala: " CYAN "%s\n\n" RESET, sali[gasit].nume);
    printf("Introduceti numele dumneavoastra (ex: Popescu Ion): ");
    scanf(" %49[^\n]", noua_rezervare.nume_client);

    for(int i = 0; noua_rezervare.nume_client[i] != '\0'; i++) {
        if(noua_rezervare.nume_client[i] == ' ') {
            noua_rezervare.nume_client[i] = '_';
        }
    }

    curata_ecranul();
    printf(YELLOW BOLD "=== PAS 7: TIP MENIU ===\n\n" RESET);
    printf("1. Fara meniu " GREEN "(0 RON)" RESET "\n");
    printf("2. Standard " GREEN "(+ 500 RON/zi)" RESET "\n");
    printf("3. Premium " GREEN "(+ 1200 RON/zi)" RESET "\n\n");
    printf("Alegeti optiunea dorita: ");
    if (scanf("%d", &noua_rezervare.tip_meniu) != 1) {
        while(getchar() != '\n');
        noua_rezervare.tip_meniu = 1;
    }

    float cost_meniu = 0;
    if (noua_rezervare.tip_meniu == 2) cost_meniu = 500.0;
    else if (noua_rezervare.tip_meniu == 3) cost_meniu = 1200.0;

    noua_rezervare.pret_total = (sali[gasit].pret + cost_meniu) * noua_rezervare.zile;

    rezervari[*numar_rezervari] = noua_rezervare;
    (*numar_rezervari)++;

    salveaza_rezervari(rezervari, *numar_rezervari);

    curata_ecranul();
    printf(GREEN BOLD "=== REZERVARE FINALIZATA ===\n\n" RESET);
    printf("Rezervare efectuata cu succes!\n");
    printf("ID Rezervare: " BOLD "%d\n" RESET, noua_rezervare.id_rezervare);
    printf("Pret total de achitat: " GREEN BOLD "%.2f RON\n\n" RESET, noua_rezervare.pret_total);
    printf("Pentru mai multe informatii mergeti la submeniul 'Afisare detalii rezervare'.\n");
    pauza();
}

void anulare_rezervare(Rezervare rezervari[], int *numar_rezervari, Sala sali[], int numar_sali) {
    curata_ecranul();
    printf(CYAN BOLD "=== SUBMENIU: ANULARE REZERVARE ===\n\n" RESET);

    if (*numar_rezervari == 0) {
        printf(YELLOW "Nu exista rezervari active.\n" RESET);
        pauza();
        return;
    }

    printf(BOLD "%-6s | %-15s | %-15s | %-12s | %-5s | %-10s\n" RESET, "ID Rez", "Nume Sala", "Client", "Data Incep", "Zile", "Total(RON)");
    printf("----------------------------------------------------------------------------------\n");
    for (int i = 0; i < *numar_rezervari; i++) {
        char nume_afisare[50];
        strcpy(nume_afisare, rezervari[i].nume_client);
        for(int j=0; nume_afisare[j]; j++) if(nume_afisare[j]=='_') nume_afisare[j]=' ';

        char nume_sala_afisare[50] = "Necunoscut";
        for(int k=0; k < numar_sali; k++) {
            if(sali[k].id == rezervari[i].id_sala) {
                strcpy(nume_sala_afisare, sali[k].nume);
                break;
            }
        }

        char data_str[20];
        sprintf(data_str, "%02d/%02d/%04d", rezervari[i].zi, rezervari[i].luna, rezervari[i].an);

        printf("%-6d | %-15s | %-15s | %-12s | %-5d | %-10.2f\n",
            rezervari[i].id_rezervare, nume_sala_afisare, nume_afisare,
            data_str, rezervari[i].zile, rezervari[i].pret_total);
    }

    int id_stergere;
    printf("\nIntroduceti ID-ul rezervarii pe care doriti sa o anulati (sau " RED "0" RESET " pt inapoi): ");
    if (scanf("%d", &id_stergere) != 1 || id_stergere == 0) {
        while(getchar() != '\n');
        return;
    }

    int idx_stergere = -1;
    for (int i = 0; i < *numar_rezervari; i++) {
        if (rezervari[i].id_rezervare == id_stergere) {
            idx_stergere = i;
            break;
        }
    }

    if (idx_stergere != -1) {
        for (int i = idx_stergere; i < *numar_rezervari - 1; i++) {
            rezervari[i] = rezervari[i + 1];
        }
        (*numar_rezervari)--;

        for (int i = 0; i < *numar_rezervari; i++) {
            rezervari[i].id_rezervare = i + 1;
        }

        salveaza_rezervari(rezervari, *numar_rezervari);

        printf(GREEN "\nRezervarea a fost anulata cu succes!\n" RESET);
        printf("ID-urile rezervarilor ramase au fost reordonate.\n");
    } else {
        printf(RED "\nNu a fost gasita nicio rezervare cu acest ID.\n" RESET);
    }

    pauza();
}

void afisare_detalii_rezervare(Rezervare rezervari[], int numar_rezervari, Sala sali[], int numar_sali) {
    curata_ecranul();
    printf(CYAN BOLD "=== SUBMENIU: DETALII REZERVARE ===\n\n" RESET);

    if (numar_rezervari == 0) {
        printf(YELLOW "Nu exista rezervari active.\n" RESET);
        pauza();
        return;
    }

    printf(BOLD "%-6s | %-15s | %-15s | %-12s | %-5s | %-6s | %-10s\n" RESET, "ID Rez", "Nume Sala", "Client", "Data Incep", "Zile", "Ramase", "Total(RON)");
    printf("---------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < numar_rezervari; i++) {
        char nume_afisare[50];
        strcpy(nume_afisare, rezervari[i].nume_client);
        for(int j=0; nume_afisare[j]; j++) if(nume_afisare[j]=='_') nume_afisare[j]=' ';

        char nume_sala_afisare[50] = "Necunoscut";
        for(int k=0; k < numar_sali; k++) {
            if(sali[k].id == rezervari[i].id_sala) {
                strcpy(nume_sala_afisare, sali[k].nume);
                break;
            }
        }

        char data_str[20];
        sprintf(data_str, "%02d/%02d/%04d", rezervari[i].zi, rezervari[i].luna, rezervari[i].an);

        printf("%-6d | %-15s | %-15s | %-12s | %-5d | ",
            rezervari[i].id_rezervare, nume_sala_afisare, nume_afisare,
            data_str, rezervari[i].zile);

        int zile_ramase = calculeaza_zile_ramase(rezervari[i].zi, rezervari[i].luna, rezervari[i].an, rezervari[i].zile);
        if (zile_ramase > 0) printf(GREEN "%-6d" RESET " | ", zile_ramase);
        else printf(RED "%-6s" RESET " | ", "Exp.");

        printf("%-10.2f\n", rezervari[i].pret_total);
    }

    int id_cautat;
    printf("\nIntroduceti ID-ul rezervarii pentru a vedea detaliile complete (sau " RED "0" RESET " pt inapoi): ");
    if (scanf("%d", &id_cautat) != 1 || id_cautat == 0) {
        while(getchar() != '\n');
        return;
    }

    int gasit_rez = -1;
    for (int i = 0; i < numar_rezervari; i++) {
        if (rezervari[i].id_rezervare == id_cautat) {
            gasit_rez = i;
            break;
        }
    }

    if (gasit_rez == -1) {
        printf(RED "Rezervarea nu a fost gasita!\n" RESET);
        pauza();
        return;
    }

    int gasit_sala = -1;
    for (int i = 0; i < numar_sali; i++) {
        if (sali[i].id == rezervari[gasit_rez].id_sala) {
            gasit_sala = i;
            break;
        }
    }

    curata_ecranul();
    printf(YELLOW BOLD "=== FACTURA / DETALII REZERVARE ===\n\n" RESET);
    printf(CYAN "ID Rezervare:   " RESET "%d\n", rezervari[gasit_rez].id_rezervare);

    char nume_afisare_final[50];
    strcpy(nume_afisare_final, rezervari[gasit_rez].nume_client);
    for(int j=0; nume_afisare_final[j]; j++) if(nume_afisare_final[j]=='_') nume_afisare_final[j]=' ';

    printf(CYAN "Nume Client:    " RESET "%s\n", nume_afisare_final);
    printf(CYAN "Data Inceput:   " RESET "%02d/%02d/%04d\n", rezervari[gasit_rez].zi, rezervari[gasit_rez].luna, rezervari[gasit_rez].an);
    printf(CYAN "Perioada:       " RESET "%d zile\n", rezervari[gasit_rez].zile);

    int zile_ramase = calculeaza_zile_ramase(rezervari[gasit_rez].zi, rezervari[gasit_rez].luna, rezervari[gasit_rez].an, rezervari[gasit_rez].zile);
    if(zile_ramase > 0) printf(CYAN "Zile Ramase:    " GREEN "%d zile\n" RESET, zile_ramase);
    else printf(CYAN "Zile Ramase:    " RED "Expirata\n" RESET);

    char tip_meniu_str[20] = "Fara";
    if(rezervari[gasit_rez].tip_meniu == 2) strcpy(tip_meniu_str, "Standard");
    else if(rezervari[gasit_rez].tip_meniu == 3) strcpy(tip_meniu_str, "Premium");
    printf(CYAN "Meniu Selectat: " RESET "%s\n", tip_meniu_str);
    printf("-----------------------------------\n");

    if (gasit_sala != -1) {
        printf(CYAN "Nume Sala:      " RESET "%s\n", sali[gasit_sala].nume);
        printf(CYAN "Capacitate:     " RESET "%d locuri\n", sali[gasit_sala].capacitate);
        printf(CYAN "Facilitati:     " RESET "%s\n", sali[gasit_sala].facilitati);
    } else {
        printf(RED "Detalii Sala:   Indisponibile (Sala a fost stearsa/modificata)\n" RESET);
    }

    printf("-----------------------------------\n");
    printf(GREEN BOLD "PRET TOTAL:     %.2f RON\n" RESET, rezervari[gasit_rez].pret_total);

    printf("\n");
    pauza();
}
