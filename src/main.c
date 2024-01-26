#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <ctype.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <memory.h>
#include <malloc.h>
#include <locale.h>
//#include <time.h>

#define MAX_LENGTH 50
#define LENGTH_STATE 2
#define MIN_LENGTH 5
#define LENGTH_CEP 8
#define MAX_CEP 10
#define LENGTH_INPUT 11
#define LENGTH_CPF 15
#define LENGTH_PHONE 16  

typedef struct {
    char street[MAX_LENGTH];
    char number[MAX_LENGTH];
    char neighborhood[MAX_LENGTH];
    char city[MAX_LENGTH];
    char state[MAX_LENGTH];
    char cep[MAX_CEP];
} Address;

// typedef struct {
//     int day;
//     int month;
//     int year;
// } Date;

typedef struct {
    char name[MAX_LENGTH];
    char email[MAX_LENGTH];
    char cpf[LENGTH_CPF];
    char phone[LENGTH_PHONE];
    Address address;
    // Date dateRegister;
} Customer;

struct MemoryStruct {
    char *memory;
    size_t size;
};

Customer *createCustomer();
void setName(Customer *customer);
void setEmail(Customer *customer);
void setCpf(Customer *customer);
void setPhone(Customer *customer);

char *formatCpf(char *cpf);
char *formatPhone(char *phone);

int isValidRegex(const char *input, const char *pattern);
int isValidLengthInput(const char *input);
int isValidCpfAndPhone(const char *input);

Address *createAddress();
void setCep(Address *adress);
void setNumber(Address *address);
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
int searchByCep(const char *cep, Address *address);

int isValidCep(const char *cep);
int isValidNumber(const char *number);
int isNumbers(const char *input);

int main() {
    setlocale(LC_ALL, "Portuguese");
    Customer *dataBase[1];
    
    dataBase[0] = createCustomer();
    printf("Cliente cadastrado com sucesso!\n");

    free(dataBase[0]);
    return 0;
}

void setName(Customer *customer) {
    char name[MAX_LENGTH];
    const char *patternName = "^[a-zA-Z]+( [a-zA-Z]+)*$";

    do {
        printf("Digite o nome do cliente: ");
        scanf("%50[^\n]", name);
        getchar();
        
        if(!isValidLengthInput(name))
            printf("O nome deve ter entre %d e %d caracteres.\n", MIN_LENGTH, MAX_LENGTH);
         else if(!isValidRegex(name, patternName)) 
            printf("O nome não confere com o padrão da expressão regular, tente novamente.\n");
         else break;
    } while(!(isValidLengthInput(name) && isValidRegex(name, patternName)));

    strcpy(customer->name, name);
}

void setEmail(Customer *customer) {
    char email[MAX_LENGTH];
    const char *patternEmail = "^[a-z0-9._]+@[a-z]+\\.[a-z]{3,}$";

    do {
        printf("Digite o e-mail do cliente (.com): ");
        scanf("%50[^\n]", email);
        getchar();

        if(!isValidLengthInput(email))
            printf("O e-mail deve ter entre %d e %d caracteres.\n", MIN_LENGTH, MAX_LENGTH);
        else if(!isValidRegex(email, patternEmail)) 
            printf("O e-mail não confere com o padrão da expressão regular, tente novamente.\n");
        else break;
    } while(!(isValidLengthInput(email) && isValidRegex(email, patternEmail)));

    strcpy(customer->email, email);
}

void setCpf(Customer *customer) {
    char cpf[LENGTH_INPUT];

    do {
        printf("Digite o CPF do cliente (Apenas números): ");
        scanf("%[^\n]", cpf);
        getchar();

        if(!isValidCpfAndPhone(cpf))
            printf("O CPF deve ter %d caracteres.\n", LENGTH_INPUT);
        else if(!isNumbers(cpf))
            printf("O CPF deve conter apenas números.\n");
        else break;
    } while(!(isValidCpfAndPhone(cpf) && isNumbers(cpf)));
    
    strcpy(customer->cpf, formatCpf(cpf));
}

void setPhone(Customer *customer) {
    char phone[LENGTH_INPUT];

    do{
        printf("Digite o telefone do cliente com DDD (Apenas números): ");
        scanf("%[^\n]", phone);
        getchar();

        if(!isValidCpfAndPhone(phone)) 
            printf("O telefone deve ter %d caracteres.\n", LENGTH_INPUT);
        else if(!isNumbers(phone))
            printf("O telefone deve conter apenas números.\n");
        else break;
    } while(!(isValidCpfAndPhone(phone) && isNumbers(phone)));

    strcpy(customer->phone, formatPhone(phone));
}

char *formatCpf(char *cpf) {
    char *cpfFormatted = (char *) malloc(LENGTH_CPF);
    
    if(cpfFormatted == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(EXIT_FAILURE);
    }

    snprintf(cpfFormatted, LENGTH_CPF, "%.*s.%.*s.%.*s-%.*s", 3, cpf, 3, cpf + 3, 3, cpf + 6, 2, cpf + 9);
    return cpfFormatted;
}

char *formatPhone(char *phone) {
    char *phoneFormatted = (char *) malloc(LENGTH_PHONE);

    if(phoneFormatted == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(EXIT_FAILURE);
    }

    snprintf(phoneFormatted, LENGTH_PHONE, "(%.*s) %.*s-%.*s", 2, phone, 5, phone + 2, 4, phone + 7);
    return phoneFormatted;
}

int isValidRegex(const char *input, const char *pattern) {    
    regex_t regex;
    int value; 

    if(regcomp(&regex, pattern, REG_EXTENDED | REG_NOSUB)) {
        fprintf(stderr, "Não foi possível compilar a expressão regular\n");
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "Expressão regular compilada com sucesso\n");
    value = regexec(&regex, input, (size_t) 0, (regmatch_t *) NULL, 0);
    regfree(&regex);

    return (value || value == REG_NOMATCH) ? 0 : 1;
}

int isValidLengthInput(const char *input) {
    return (strlen(input) > MAX_LENGTH || strlen(input) < MIN_LENGTH) ? 0 : 1;
}

int isValidCpfAndPhone(const char *input) {
    return (strlen(input) == LENGTH_INPUT) ? 1 : 0;
}

int isNumbers(const char *input) {
    for (int i = 0; input[i] != '\0'; i++) {
        if (!isdigit(input[i])) return 0;
    }

    return 1;
}

Customer *createCustomer() {
    Customer *customer = (Customer *) malloc(sizeof(Customer));

    if(customer == NULL) {
        fprintf(stderr, "Erro: Alocação de memória!\n");
        exit(EXIT_FAILURE);
    }
    
    // setName(customer);
    // setEmail(customer);
    //setCpf(customer);
    //setPhone(customer);

    Address *address = createAddress();
    customer->address = *address;

    return customer;
}

void setCep(Address *adress) {
    char cep[MAX_CEP];

    do{
        printf("Digite o CEP do cliente (Apenas números): ");
        scanf("%[^\n]", cep);
        getchar();

        if(!isValidCep(cep))
            printf("O CEP deve ter %d caracteres.\n", LENGTH_CEP);
        else if(!isNumbers(cep))
            printf("O CEP deve conter apenas números.\n");
        else break;
    } while(!(isValidCep(cep) && isNumbers(cep)));

    strcpy(adress->cep, cep);
}

void setNumber(Address *address) {
    char number[MIN_LENGTH];

    do {
        printf("Digite o número da residência: ");
        scanf("%[^\n]", number);
        getchar();

        if(!isValidNumber(number))
            printf("O número deve ter no máximo %d caracteres.\n", MIN_LENGTH);
        else if(!isNumbers(number))
            printf("Deve conter apenas números.\n");
        else break;
    } while(!(isValidNumber(number) && isNumbers(number)));

    strcpy(address->number, number);
}

int isValidCep(const char *cep) {
    return (strlen(cep) == LENGTH_CEP) ? 1 : 0;
}

int isValidNumber(const char *number) {
    return (strlen(number) > MIN_LENGTH) ? 0 : 1;
}

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size *nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(EXIT_FAILURE);
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

int searchByCep(const char *cep, Address *address) {
    CURL *curl;
    CURLcode res;

    struct MemoryStruct chunk = {0};

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        char url[MAX_LENGTH];
        snprintf(url, sizeof(url), "https://brasilaberto.com/api/v1/zipcode/%s", cep);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        
        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            return -1;
        }
        
        json_object *json = json_tokener_parse(chunk.memory);
        if(json == NULL) {
            fprintf(stderr, "Erro ao analisar JSON\n");
            free(chunk.memory);
            curl_easy_cleanup(curl);
            
            return -1;
        }

        json_object *response;
        if(json_object_object_get_ex(json, "result", &response)) {
            json_object *error, *street, *neighborhood, *city, *state;

            if(json_object_object_get_ex(response, "error", &error)) {
                fprintf(stderr, "Erro: %s\n", json_object_get_string(error));
                curl_easy_cleanup(curl);

                return -1;
            } else {
                if(json_object_object_get_ex(response, "street", &street)) {
                    strcpy(address->street, json_object_get_string(street));
                    address->street[strlen(address->street)] = '\0';
                }
                if(json_object_object_get_ex(response, "district", &neighborhood)) {
                    strcpy(address->neighborhood, json_object_get_string(neighborhood));
                    address->neighborhood[strlen(address->neighborhood)] = '\0';
                }
                if(json_object_object_get_ex(response, "city", &city)) {
                    strcpy(address->city, json_object_get_string(city));
                    address->city[strlen(address->city)] = '\0';
                }
                if(json_object_object_get_ex(response, "state", &state)) {
                    strcpy(address->state, json_object_get_string(state));
                    address->state[strlen(address->state)] = '\0';
                } 
            }
        }

        json_object_put(json);
        free(chunk.memory);

        curl_easy_cleanup(curl);
        curl_global_cleanup();

        return 0; 
    } else {
        fprintf(stderr, "Erro ao inicializar o CURL\n");

        free(chunk.memory);
        return -1;
    }
}

Address *createAddress() {
    Address *address = (Address *) malloc(sizeof(Address));

    if(address == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(EXIT_FAILURE);
    }

    do {
        setCep(address);

        if(searchByCep(address->cep, address) == -1) {
            printf("Erro ao buscar o CEP. Tente novamente!\n");
        } else break;
    } while(searchByCep(address->cep, address));

    setNumber(address);
    return address;
}
