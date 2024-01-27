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
#include <time.h>

#define DIFF_DAY 1
#define LENGTH_STATE 2
#define MIN_LENGTH 5
#define LENGTH_CEP 8
#define MAX_CEP 10
#define MAX_CUSTOMERS 10
#define LENGTH_INPUT 11
#define LENGTH_CPF 15
#define LENGTH_PHONE 16  
#define MAX_LENGTH 50
#define YEAR_MIN 1900

#define API_URL "https://brasilaberto.com/api/v1/zipcode/%s"
#define FILE_PATH "../data/customers.csv"
#define AMOUNT_FIELDS 13

typedef struct {
    char street[MAX_LENGTH];
    char number[MAX_LENGTH];
    char neighborhood[MAX_LENGTH];
    char city[MAX_LENGTH];
    char state[MAX_LENGTH];
    char cep[MAX_CEP];
} Address;

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
    Address address;
    Date dateRegister;
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
void showDataCustomer(Customer *customer);

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

Date *createDate();
void getCurrentDate(Date *currentDate);

// void saveCustomersToFile(Customer *dataBase[], int count);
// void loadCustomersFromFile(Customer *dataBase[], int *count);
void listAllCustomers(Customer *dataBase[], int count);
// Customer *searchCustomerByCPF(Customer *dataBase[], int count, const char *cpf);
// void editCustomer(Customer *customer);
// void searchForCpf(char *cpfInput);

int main() {
    setlocale(LC_ALL, "Portuguese");
    Customer *dataBase[MAX_CUSTOMERS];
    int count = 0;
    int choice;

    // loadCustomersFromFile(dataBase, &count);

    do {
        printf("Bem-vindo ao Sistema de Cadastro de Clientes\n");
        printf("\nMenu:\n");
        printf("1. Adicionar Cliente\n");
        printf("2. Listar Todos os Clientes\n");
        printf("3. Buscar Cliente por CPF\n");
        printf("4. Editar Cliente\n");
        printf("5. Salvar e Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &choice);
        getchar(); 

        // char cpfInput[LENGTH_CPF];
        // searchForCpf(cpfInput);

        switch (choice) {
            case 1:
                dataBase[count++] = createCustomer();
                printf("Cliente cadastrado com sucesso!\n");
                break;
            case 2:
                listAllCustomers(dataBase, count);
                break;
            case 3:
                printf("teste\n");
                // Customer *foundCustomer = searchCustomerByCPF(dataBase, count, cpfInput);

                // if(foundCustomer != NULL) { showDataCustomer(foundCustomer); } 
                // else { printf("Cliente não encontrado.\n"); }
                break;
            case 4:
                printf("teste\n");
                // Customer *customerToEdit = searchCustomerByCPF(dataBase, count, cpfInput);
               
                // if(customerToEdit != NULL) {
                //     editCustomer(customerToEdit);
                //     printf("Cliente editado com sucesso!\n");
                // } else { printf("Cliente não encontrado.\n"); }
                break;
            case 5:
                printf("teste\n");
                // saveCustomersToFile(dataBase, count);
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (choice != 5);

    for (int i = 0; i < count; i++) {
        free(dataBase[i]);
    }

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

    char *cpfFormatted = formatCpf(cpf);
    strcpy(customer->cpf, cpfFormatted);
    free(cpfFormatted);
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

    char *phoneFormatted = formatPhone(phone);
    strcpy(customer->phone, phoneFormatted);
    free(phoneFormatted);
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
    
    setName(customer);
    setEmail(customer);
    setCpf(customer);
    setPhone(customer);

    Address *address = createAddress();
    customer->address = *address;

    Date *dateRegister = createDate();
    customer->dateRegister = *dateRegister;

    free(address);
    free(dateRegister);

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
    int num = atoi(number);
    return (strlen(number) > MIN_LENGTH && num <= 0) ? 0 : 1;
}

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size *nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL) {
        fprintf(stderr, "Erro ao alocar memória.\n");
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
        snprintf(url, sizeof(url), API_URL, cep);

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
    } while(searchByCep(address->cep, address) != 0);

    setNumber(address);
    return address;
}

void getCurrentDate(Date *currentDate) {
    time_t t;
    time(&t);

    struct tm *currentDateTime = (struct tm *) malloc(sizeof(struct tm));
    if(currentDateTime == NULL) {
        fprintf(stderr, "Erro ao alocar memória.\n");
        exit(EXIT_FAILURE);
    }

    currentDateTime = localtime(&t);
    
    currentDate->day = (int) currentDateTime->tm_mday;
    currentDate->month = (int) currentDateTime->tm_mon + DIFF_DAY;
    currentDate->year = (int) currentDateTime->tm_year + YEAR_MIN;
}

Date *createDate() {
    Date *currentDate = (Date *) malloc(sizeof(Date));

    if(currentDate == NULL) {
        fprintf(stderr, "Erro ao alocar memória.\n");
        exit(EXIT_FAILURE);
    }

    getCurrentDate(currentDate);
    return currentDate;
}

void showDataCustomer(Customer *customer) {
    if(customer != NULL) {
        printf("Dados do cliente cadastrado:\n");

        printf("\nDados pessoais:\n");
        printf("Nome: %s\n", customer->name);
        printf("E-mail: %s\n", customer->email);
        printf("CPF: %s\n", customer->cpf);
        printf("Telefone: %s\n", customer->phone);
        
        printf("\nEndereço:\n");
        printf("CEP: %s\n", customer->address.cep);
        printf("Rua: %s\n", customer->address.street);
        printf("Número: %s\n", customer->address.number);
        printf("Bairro: %s\n", customer->address.neighborhood);
        printf("Cidade: %s\n", customer->address.city);
        printf("Estado: %s\n", customer->address.state);

        printf("\nCadastro:\n");
        printf("Data: %.2d/%.2d/%.4d\n", customer->dateRegister.day, customer->dateRegister.month, customer->dateRegister.year);
    } else {
        fprintf(stderr, "Erro: Cliente não encontrado!\n");
        exit(EXIT_FAILURE);
    }
}

// void saveCustomersToFile(Customer *dataBase[], int count) {
//     const char *directoryFile = FILE_PATH;
//     FILE *file = fopen(directoryFile, "w");
    
//     if(file == NULL) {
//         fprintf(stderr, "Erro ao abrir o arquivo para escrita.\n");
//         return;
//     }

//     for(int i = 0; i < count; i++) {
//         fprintf(file, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%d,%d,%d",
//                 dataBase[i]->name,
//                 dataBase[i]->email,
//                 dataBase[i]->cpf,
//                 dataBase[i]->phone,
//                 dataBase[i]->address.cep,
//                 dataBase[i]->address.street,
//                 dataBase[i]->address.number,
//                 dataBase[i]->address.neighborhood,
//                 dataBase[i]->address.city,
//                 dataBase[i]->address.state,
//                 dataBase[i]->dateRegister.day,
//                 dataBase[i]->dateRegister.month,
//                 dataBase[i]->dateRegister.year
//             );
//         fprintf(file, "\n");
//     }

//     fclose(file);
//     printf("Clientes salvos com sucesso!\n");
// }

// void loadCustomersFromFile(Customer *dataBase[], int *count) {
//     const char *directoryFile = FILE_PATH;
//     FILE *file = fopen(directoryFile, "r");
    
//     if(file == NULL) {
//         fprintf(stderr, "Arquivo de clientes não encontrado ou erro ao abrir para leitura.\n");
//         return;
//     }

//     *count = 0;
//     int capacity = MAX_CUSTOMERS;
//     dataBase = realloc(dataBase, capacity * sizeof(Customer *));

//     if(dataBase == NULL) {
//         fprintf(stderr, "Erro ao alocar memória.\n");
//         fclose(file);
//         exit(EXIT_FAILURE);
//     }

//     /*while (1) {
//         if(*count >= capacity) {
//             capacity += MAX_CUSTOMERS;
//             dataBase = realloc(dataBase, capacity * sizeof(Customer *));
            
//             if(dataBase == NULL) {
//                 fprintf(stderr, "Erro ao alocar memória.\n");
//                 fclose(file);
//                 exit(EXIT_FAILURE);
//             }
//         }

//         Customer *customer = (Customer *) malloc(sizeof(Customer));
//         if(customer == NULL) {
//             fprintf(stderr, "Erro ao alocar memória.\n");
//             fclose(file);
//             exit(EXIT_FAILURE);
//         }

//         //"%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n"

//         TODO: // Corrigir a leitura do arquivo -> "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%d,%d,%d\n"

//         if(fscanf(
//             file, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%d,%d\n",
//             customer->name, customer->email, customer->cpf, customer->phone,
//             customer->address.cep, customer->address.street, customer->address.number,
//             customer->address.neighborhood, customer->address.city, customer->address.state,
//             &customer->dateRegister.day, &customer->dateRegister.month, &customer->dateRegister.year
//             ) == AMOUNT_FIELDS) {
            
//             dataBase[*count] = customer;
//             (*count)++;
//         } else {
//             free(customer);
//             break;
//         }
//     }*/

//     while (*count < capacity && fscanf(
//             file, "%49[^,],%49[^,],%14[^,],%15[^,],%9[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%d,%d,%d\n",
//             dataBase[*count]->name,
//             dataBase[*count]->email,
//             dataBase[*count]->cpf,
//             dataBase[*count]->phone,
//             dataBase[*count]->address.cep,
//             dataBase[*count]->address.street,
//             dataBase[*count]->address.number,
//             dataBase[*count]->address.neighborhood,
//             dataBase[*count]->address.city,
//             dataBase[*count]->address.state,
//             &dataBase[*count]->dateRegister.day,
//             &dataBase[*count]->dateRegister.month,
//             &dataBase[*count]->dateRegister.year
//         ) == AMOUNT_FIELDS) {
//         (*count)++;
//         dataBase[*count] = (Customer *)malloc(sizeof(Customer));

//         if (dataBase[*count] == NULL) {
//             fprintf(stderr, "Erro ao alocar memória para o próximo cliente.\n");
//             fclose(file);
//             exit(EXIT_FAILURE);
//         }
//     }

//     fclose(file);
// }

void listAllCustomers(Customer *dataBase[], int count) {
    if (count == 0) {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }

    printf("Lista de todos os clientes:\n");
    for (int i = 0; i < count; i++) {
        printf("\nCliente %d:\n", i + 1);
        showDataCustomer(dataBase[i]);
    }
}

// Customer *searchCustomerByCPF(Customer *dataBase[], int count, const char *cpf) {
//     for(int i = 0; i < count; i++) {
//         if(strcmp(dataBase[i]->cpf, cpf) == 0) return dataBase[i];
//     }
    
//     return NULL;
// }

// void editCustomer(Customer *customer) {
//     printf("Editar Cliente:\n");
//     printf("1. Nome\n");
//     printf("2. E-mail\n");
//     printf("3. CPF\n");
//     printf("4. Telefone\n");
//     printf("5. Endereço\n");
//     printf("Escolha uma opção: ");

//     int choice;
//     scanf("%d", &choice);
//     getchar();

//     switch(choice) {
//         case 1:
//             setName(customer);
//             break;
//         case 2:
//             setEmail(customer);
//             break;
//         case 3:
//             setCpf(customer);
//             break;
//         case 4:
//             setPhone(customer);
//             break;
//         case 5:
//             printf("Editar Endereço:\n");
//             printf("1. CEP\n");
//             printf("2. Número\n");
//             printf("Escolha uma opção: ");
            
//             int addressChoice;
//             scanf("%d", &addressChoice);
//             getchar();

//             switch(addressChoice) {
//                 case 1:
//                     setCep(&customer->address);
//                     break;
//                 case 2:
//                     setNumber(&customer->address);
//                     break;
//                 default:
//                     printf("Opção inválida.\n");
//             }
//             break;
//         default:
//             printf("Opção inválida.\n");
//     }
// }

// void searchForCpf(char *cpfInput) {
//     char cpfSearch[LENGTH_INPUT];

//     do {
//         printf("Digite o CPF de busca: ");
//         scanf("%[^\n]", cpfSearch);
//         getchar();

//         if(!isValidCpfAndPhone(cpfSearch))
//             printf("O CPF deve ter %d caracteres.\n", LENGTH_INPUT);
//         else if(!isNumbers(cpfSearch))
//             printf("O CPF deve conter apenas números.\n");
//         else break;
//     } while(!(isValidCpfAndPhone(cpfSearch) && isNumbers(cpfSearch)));

//     char *cpf = formatCpf(cpfSearch);
//     strcpy(cpfInput, cpf);
//     free(cpf);        
// }
