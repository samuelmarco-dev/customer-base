#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <locale.h>
#include <time.h>

#define MAX_LENGTH 50
#define MIN_LENGTH 5
#define LENGTH_INPUT 11
#define LENGTH_SINGLE 12
#define LENGTH_CPF 15
#define LENGTH_PHONE 16  

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
    char cpf[LENGTH_CPF];
    char phone[LENGTH_PHONE];
    Adress adress;
    Date dateRegister;
} Customer;

Customer *createCustomer();
void setName(Customer *customer);
void setEmail(Customer *customer);
void setCpf(Customer *customer);
void setPhone(Customer *customer);
void setAdress(Customer *customer);

Adress *createAdress();
void setCep(Adress *adress);
void setStreet(Adress *adress);
void setNumber(Adress *adress);
void setNeighborhood(Adress *adress);
void setCity(Adress *adress);
void setState(Adress *adress);

char *formatCpf(char *cpf);
char *formatPhone(char *phone);

int isValidRegex(const char *input, const char *pattern);
int isValidLengthInput(const char *input);
int isValidCpfAndPhone(const char *input);

int main() {
    setlocale(LC_ALL, "Portuguese");
    
    return 0;
}

void setName(Customer *customer) {
    char name[MAX_LENGTH];
    const char *patternName = "^[A-Za-zÀ-ÖØ-öø-ÿ]+(?: [A-Za-zÀ-ÖØ-öø-ÿ]+)*$";

    printf("Digite o nome do cliente: ");
    scanf("%[^\n]", name);

    if(!isValidRegex(name, patternName) || !isValidLengthInput(name)) {
        printf("O nome do cliente deve ter no mínimo %d caracteres e no máximo %d caracteres.\n", MIN_LENGTH, MAX_LENGTH);
        setName(customer); // Recursive function
    } else {
        strcpy(customer->name, name);
    }
}

void setEmail(Customer *customer) {
    char email[MAX_LENGTH];
    const char *patternEmail = "^[a-z0-9._]+@[a-z]+\\.[a-z]+\\.[a-z]{2,}$";

    printf("Digite o e-mail do cliente: ");
    scanf("%[^\n]", email);

    if(!isValidRegex(email, patternEmail) || !isValidLengthInput(email)) {
        printf("O e-mail não confere com o padrão da expressão regular, tente novamente.\n");
        setEmail(customer); // Recursive function
    } else {
        strcpy(customer->email, email);
    }
}

void setCpf(Customer *customer) {
    char cpf[LENGTH_SINGLE];

    printf("Digite o CPF do cliente (Apenas números): ");
    scanf("%[^\n]", cpf);

    if(!isValidCpfAndPhone(cpf)) {
        printf("O CPF deve ter %d caracteres.\n", LENGTH_INPUT);
        setCpf(customer); // Recursive function
    } else {
        strcpy(customer->cpf, formatCpf(cpf));
    }
}

void setPhone(Customer *customer) {
    char phone[LENGTH_SINGLE];

    printf("Digite o telefone do cliente com DDD (Apenas números): ");
    scanf("%[^\n]", phone);

    if(!isValidCpfAndPhone(phone)) {
        printf("O telefone deve ter %d caracteres.\n", LENGTH_INPUT);
        setPhone(customer); // Recursive function
    } else {
        strcpy(customer->phone, formatPhone(phone));
    }
}

char *formatCpf(char *cpf) {
    char *cpfFormatted = (char *) malloc(LENGTH_CPF);
    
    if(cpfFormatted == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(EXIT_FAILURE);
    }

    sprintf(cpfFormatted, "%s.%s.%s-%s", cpf, cpf + 3, cpf + 6, cpf + 9);
    return cpfFormatted;
}

char *formatPhone(char *phone) {
    char *phoneFormatted = (char *) malloc(LENGTH_PHONE);

    if(phoneFormatted == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(EXIT_FAILURE);
    }

    sprintf(phoneFormatted, "(%s) %s-%s", phone, phone + 2, phone + 7);
    return phoneFormatted;
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

int isValidLengthInput(const char *input) {
    return (strlen(input) > MAX_LENGTH || strlen(input) < MIN_LENGTH) ? 0 : 1;
}

int isValidCpfAndPhone(const char *input) {
    return (strlen(input) == LENGTH_INPUT) ? 1 : 0;
}

Customer *createCustomer() {
    Customer *customer = (Customer *) malloc(sizeof(Customer));

    if(customer == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(EXIT_FAILURE);
    }

    setName(customer);
    setEmail(customer);
    setCpf(customer);
    setPhone(customer);

    //TODO: Adicionar data de cadastro e endereço 

    return customer;
}

void setCep(Adress *adress) {}

void setStreet(Adress *adress) {}

void setNumber(Adress *adress) {}

void setNeighborhood(Adress *adress) {}

void setCity(Adress *adress) {}

void setState(Adress *adress) {}

Adress *createAdress() {
    Adress *adress = (Adress *) malloc(sizeof(Adress));

    if(adress == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(EXIT_FAILURE);
    }

    return adress;
}
