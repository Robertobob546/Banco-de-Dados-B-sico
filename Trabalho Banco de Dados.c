#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    
    int id;
    char login[16];
    char password[31];
    char gender;
    double salary;
    int ativo;

} Registro;


Registro *registros[1000] = {NULL}; // variavel global de todos os registros



// leitura do arquivo
int leituraArq(char* nomeArq){

    FILE *f = fopen(nomeArq, "r");
    
    char linha[256];
    int cont = 0;   // registros lidos 
    Registro temp; 


    int camposLidos = 0;

    while (fgets(linha, sizeof(linha), f)){
        
        if (strstr(linha, "\"id\"")){    // checa se existe "id" nessa linha do arquivo

            sscanf(linha, " %*[^0-9]%d", &temp.id); // pula tudo ate encontrar o primeiro numero 
            camposLidos++;

        }else if (strstr(linha, "\"login\"")){

            sscanf(linha, " %*[^:] : \"%[^\"]", temp.login);  // pula tudo ate encontrar ":" 
            camposLidos++;

        }else if (strstr(linha, "\"password\"")){

            sscanf(linha, " %*[^:] : \"%[^\"]", temp.password);  // pula tudo ate encontrar ":"
            camposLidos++;

        }else if (strstr(linha, "\"gender\"")){

            sscanf(linha, " %*[^:] : \"%c\"", &temp.gender); // pula tudo ate encontrar ":" e le apenas o char
            camposLidos++;

        }else if (strstr(linha, "\"salary\"")){

            sscanf(linha, " %*[^0-9]%lf", &temp.salary); //  pula tudo ate encontrar o primeiro numero 
            camposLidos++;
        }

        if (camposLidos == 5){
            
            for(int i = 0; i < 1000; i++){
                
                if (registros[i] == NULL){

                    registros[i] = malloc(sizeof(Registro));
                    *registros[i] = temp;
                    registros[i]->ativo = 1;
                    cont++;
                    break;

                }
            }
            
            camposLidos = 0;
        }
    }
    
    
    fclose(f);
    printf("%d registro(s) lido(s).\n", cont);
    return cont;

}
        




void imprimirRegistro(Registro *r){

    printf("{\n");

    printf("    \"id\": %d,\n", r->id); // printa o id da pessoa de forma formatada
    printf("    \"login\": \"%s\",\n", r->login); // printa o login da pessoa de forma formatada
    printf("    \"password\": \"%s\",\n", r->password); // printa o password da pessoa de forma formatada
    printf("    \"gender\": \"%c\",\n", r->gender); // printa o gender da pessoa de forma formatada
    printf("    \"salary\": %.2lf\n", r->salary);

    printf("}\n");

}

void inserirRegistro(int id, char* login, char* password, char gender, double salary){

    for(int i = 0; i < 1000; i++){

        if (registros[i] == NULL || registros[i]->ativo == 0){

            if (registros[i] == NULL){

                registros[i] = malloc(sizeof(Registro));
            }

            registros[i]->id = id;

            strncpy(registros[i]->login, login, sizeof(registros[i]->login) - 1);
            registros[i]->login[sizeof(registros[i]->login) - 1] = '\0';

            strncpy(registros[i]->password, password, sizeof(registros[i]->password) - 1);
            registros[i]->password[sizeof(registros[i]->password) - 1] = '\0';

            registros[i]->gender = gender;
            registros[i]->salary = salary;
            registros[i]->ativo = 1;
            printf("Registro inserido.\n");
            return;
        }
    }

    printf("Sem espaço para inserção.\n"); // quando os 1000 espaços estão ocupados essa mensagem é printada

}


void buscar(char* campo, char* valor){   // campo a ser buscado (tipo o id),  o valor que queremos encontrar nesse campo

    int encontrados = 0;

    for(int i = 0; i < 1000; i++){

        if (registros[i] && registros[i]->ativo){

            // comparacao dos campos para ver se o valor que esta sendo buscado é encontrado 
            if ((strcmp(campo, "id") == 0 && registros[i]->id == atoi(valor)) ||
                (strcmp(campo, "login") == 0 && strcmp(registros[i]->login, valor) == 0) ||
                (strcmp(campo, "password") == 0 && strcmp(registros[i]->password, valor) == 0) ||
                (strcmp(campo, "gender") == 0 && registros[i]->gender == valor[0]) ||
                (strcmp(campo, "salary") == 0 && fabs(registros[i]->salary - atof(valor)) < 1e-6)){
                
                // caso os valores sejam encontrados o codigo ira printar o registro em busca
                imprimirRegistro(registros[i]);
                encontrados++;

                }
        }

    }

    if (encontrados == 0) {
        printf("Nada encontrado.\n"); // caso nao encontre ninguem com o campo e ovalor buscado
    }

}


void remover(char* campo, char* valor){    // busca no campo e o id que se deseja remover

    int removidos = 0;

    for (int i = 0; i < 1000; i++){

        if (registros[i] && registros[i]->ativo){

            // faz a mesma coisa que a funcao de buscar
            if ((strcmp(campo, "id") == 0 && registros[i]->id == atoi(valor)) ||
                (strcmp(campo, "login") == 0 && strcmp(registros[i]->login, valor) == 0) ||
                (strcmp(campo, "password") == 0 && strcmp(registros[i]->password, valor) == 0) ||
                (strcmp(campo, "gender") == 0 && registros[i]->gender == valor[0]) ||
                (strcmp(campo, "salary") == 0 && fabs(registros[i]->salary - atof(valor)) < 1e-6)){

                registros[i]->ativo = 0; // altera esse valor no registro e marca como removido
                removidos++;

            }
        }
    }

    if (removidos > 0){

        printf("%d registro(s) removido(s).\n", removidos);

    }else{

        printf("Remoção inválida.\n"); // caso nao exista o que remover
    }

}



void liberarMemoria(){

    for (int i = 0; i < 1000; i++){

        if (registros[i]){

            free(registros[i]);

        }
    }
}



int main(){


    char linha[256];
    char campo[32]; 
    char valor[64];
    int id;
    char login[16];
    char password[31];
    char gender;
    double salary;


     if (!fgets(linha, sizeof(linha), stdin)) return 1; // caso nada seja digitado
    linha[strcspn(linha, "\n")] = '\0'; // remove o '\n' para nao aparecer isso na hora de abrir o arquivo
    leituraArq(linha);


    while(fgets(linha, sizeof(linha), stdin)){

        if (linha[0] == '1'){

            sscanf(linha, "1 %d \"%[^\"]\" \"%[^\"]\" \"%c\" %lf", &id, login, password, &gender, &salary);
            inserirRegistro(id, login, password, gender, salary);

        }else if (linha[0]=='2' || linha[0]=='3'){


            int comando;
            char resto[128];
            sscanf(linha,"%d %[^\n]",&comando,resto);

            char *p=strchr(resto,'"');
            char *q=strchr(p+1,'"');
            strncpy(campo,p+1,q-p-1);
            campo[q-p-1]='\0';

            p = q+1;
            while (*p==' ' || *p=='"') p++; // pula espaços/aspas
            q = p;
            while (*q && *q!='"' && *q!='\n') q++;
            strncpy(valor,p,q-p);
            valor[q-p]='\0';

            if (comando==2) buscar(campo,valor);
            else if (comando==3) remover(campo,valor);


        }


    }


    liberarMemoria();

    return 0;
}