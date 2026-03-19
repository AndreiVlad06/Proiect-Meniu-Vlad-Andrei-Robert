#include <stdio.h>
#include <stdlib.h>

void afisare_meniu();
void afisare_toate_salile();
void cautare_sala();
void efectuare_rezervare();
void anulare_rezervare();
void afisare_detalii_rezervare();
void curata_ecranul();
void pauza();

int main() {
    int optiune;

    do {
        curata_ecranul();
        afisare_meniu();
        printf("\nAlegeti o optiune: ");

        if (scanf("%d", &optiune) != 1) {
            printf("Eroare: Va rugam introduceti un numar!\n");
            while(getchar() != '\n');
            pauza();
            continue;
        }

        switch (optiune) {
            case 1:
                afisare_toate_salile();
                break;
            case 2:
                cautare_sala();
                break;
            case 3:
                efectuare_rezervare();
                break;
            case 4:
                anulare_rezervare();
                break;
            case 5:
                afisare_detalii_rezervare();
                break;
            case 0:
                curata_ecranul();
                printf("=== IESIRE ===\n");
                printf("Iesire din aplicatie. O zi frumoasa!\n");
                break;
            default:
                printf("Optiune invalida! Va rugam incercati din nou.\n");
                pauza();
        }
    } while (optiune != 0);

    return 0;
}

void curata_ecranul() {
    system("cls");
}

void pauza() {
    printf("\nApasati ENTER pentru a va intoarce...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    getchar();
}

void afisare_meniu() {
    printf("=== MENIU PRINCIPAL - REZERVARE SALI ===\n");
    printf("1. Afisare toate salile disponibile\n");
    printf("2. Cautare sala (dupa capacitate/facilitati)\n");
    printf("3. Efectuare rezervare noua\n");
    printf("4. Anulare rezervare\n");
    printf("5. Afisare detalii rezervare existenta\n");
    printf("0. Iesire\n");
}

void afisare_toate_salile() {
    curata_ecranul();
    printf("=== SUBMENIU: AFISARE SALI DISPONIBILE ===\n\n");
    printf("Aici vor fi listate salile...\n");
    pauza();
}

void cautare_sala() {
    curata_ecranul();
    printf("=== SUBMENIU: CAUTARE SALA ===\n\n");
    printf("Aici utilizatorul va cauta sali dupa criterii.\n");
    pauza();
}

void efectuare_rezervare() {
    curata_ecranul();
    printf("=== SUBMENIU: EFECTUARE REZERVARE ===\n\n");
    printf("Aici se va selecta sala, perioada si meniul.\n");
    pauza();
}

void anulare_rezervare() {
    curata_ecranul();
    printf("=== SUBMENIU: ANULARE REZERVARE ===\n\n");
    printf("Aici se va sterge o rezervare.\n");
    pauza();
}

void afisare_detalii_rezervare() {
    curata_ecranul();
    printf("=== SUBMENIU: DETALII REZERVARE ===\n\n");
    printf("Aici se vor afisa pretul total, facilitatile, etc.\n");
    pauza();
}
