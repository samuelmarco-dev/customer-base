#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

#define MAX_LENGTH 50

typedef struct {
    char street[MAX_LENGTH];
    char number[MAX_LENGTH];
    char neighborhood[MAX_LENGTH];
    char city[MAX_LENGTH];
    char state[MAX_LENGTH];
    char cep[MAX_LENGTH];
} Adress;

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    char nameCompleto[MAX_LENGTH];
    char email[MAX_LENGTH];
    char cpf[MAX_LENGTH];
    char phone[MAX_LENGTH];
    Adress adress;
    Date dateRegister;
} Customer;

int main() {
    setlocale(LC_ALL, "Portuguese");
    
    return 0;
}
