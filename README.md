# customer-base
customer registration through the terminal (CLI)

## Descrição (pt-br)
Um projeto de cadastro de clientes é uma maneira prática de entender como armazenar e gerenciar dados usando a linguagem C.

Neste projeto, você criará um sistema simples para cadastrar clientes, onde poderá adicionar, visualizar, editar e excluir informações dos clientes.

O objetivo é praticar a manipulação de dados e o uso de estruturas de controle.

## Description (en-us)
A customer registration project is a practical way to understand how to store and manage data using the C language.

In this project, you will create a simple system for registering customers, where you can add, view, edit and delete customer information.

The goal is to practice data manipulation and the use of control structures.

## Linux environment instructions
1. Install gcc compiler
2. Install libcurl4-openssl-dev and libjson-c-dev
3. Clone the repository
4. Enter the directory
5. Compile the main.c file
6. Run the main file

```bash
sudo apt install gcc
sudo apt-get install libcurl4-openssl-dev
sudo apt install libjson-c-dev
git clone
cd customer-base
cd src
gcc -o main main.c -lcurl -ljson-c
./main
```

## Upcoming features
1. Verification of existing user according to email and CPF input
2. Reading the data in the .txt file and dynamically assigning the memory allocation from the customers array [ ]
3. Option to assign the customer's address manually, eliminating the request via API
