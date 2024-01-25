#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <locale.h>
#include <time.h>

#define MAX_LENGTH 50
#define MIN_LENGTH 5

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
    char name[MAX_LENGTH];
    char email[MAX_LENGTH];
    char cpf[MAX_LENGTH];
    char phone[MAX_LENGTH];
    Adress adress;
    Date dateRegister;
} Customer;

Customer *createCustomer();
void setName(Customer *customer);
void setEmail(Customer *customer);

int isValidRegex(const char *input, const char *pattern);
int isValidLengthInput(const char *input);

int main() {
    setlocale(LC_ALL, "Portuguese");
    
    return 0;
}

void setName(Customer *customer) {
    char name[MAX_LENGTH];
    const char *patternName = "^[A-Za-zÀ-ÖØ-öø-ÿ]+(?: [A-Za-zÀ-ÖØ-öø-ÿ]+)*$";

    printf("Digite o nome do cliente: ");
    scanf("%[^\n]", name);

    if(!isValidRegex(name, patternName) || !isValidInput(name)) {
        printf("O nome do cliente deve ter no mínimo %d caracteres e no máximo %d caracteres.\n", MIN_LENGTH, MAX_LENGTH);
        setName(customer); // Recursive function
    } else {
        strcpy(customer->name, name);
    }
}

int isValidLengthInput(const char *input) {
    return (strlen(input) > MAX_LENGTH || strlen(input) < MIN_LENGTH) ? 0 : 1;
}

void setEmail(Customer *customer) {
    char email[MAX_LENGTH];
    const char *patternEmail = "^[a-z0-9._]+@[a-z]+\\.[a-z]+\\.[a-z]{2,}$";

    printf("Digite o e-mail do cliente: ");
    scanf("%[^\n]", email);

    if(!isValidRegex(email, patternEmail) || !isValidInput(email)) {
        printf("O e-mail não confere com o padrão da expressão regular, tente novamente.\n");
        setEmail(customer); // Recursive function
    } else {
        strcpy(customer->email, email);
    }
}

int isValidRegex(const char *input, const char *pattern) {
    regex_t regex;
    int value; 

    if(regcomp(&regex, pattern, REG_EXTENDED | REG_NOSUB)) {
        fprintf(stderr, "Não foi possível compilar a expressão regular\n");
        exit(EXIT_FAILURE);
    }
    value = regexec(&regex, input, (size_t) 0, NULL, 0);
    regfree(&regex);

    return (!value) ? 1 : 0;
}

Customer *createCustomer() {
    Customer *customer = (Customer *) malloc(sizeof(Customer));

    setName(customer);
    setEmail(customer);
}
