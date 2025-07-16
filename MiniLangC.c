#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define BUFFER_SIZE 1024
#define MAX_LINHAS 100

int i = 0; //variavel de acesso global que é usada para contagem de linhas
int quantidade_de_variaveis = 0; //quantidade de variaveis pressupostas no programa
int contadorDeVariaveis = 0; //contadores de variaveis (são nossa forma de endereço)
//vai indicar até pnde precisamos buscar variáveis em um arranjo (nossa versão de memoria)
int erro = 0; //variavel que vai parar o programa caso em algum momento ele ache um erro

char variavel_para_busca[32], completude[32]; //strings para trabalhar com as variaveis que o usuario digita

typedef struct { //struct var com nome e valor
	char nome[32];
	int valor;
} Variavel;

typedef struct { //struct comandos. cada struct dessa possui uma linha do arquivo txt
    char line[BUFFER_SIZE];
} Comands;

int linhas(){//essa função conta as linhas do txt. usamos isso para criar tantas struct's Comands quanto houverem linhas no txt
	char linhas[30];
	FILE *file = fopen("programa.txt", "r");
    if (file == NULL) { //clausula de erro padrão
        return -1;
    }

    i = 0;
    
    while(fgets(linhas, 30, file) != NULL) { //laço que se repete enquanto houver algo no txt
        i++;
    }
    fclose(file);
    
    return i;
	
}

int read(Comands *c) { //função que copia linha a linha do txt nas struct comand
	char teste[10];
    FILE *file = fopen("programa.txt", "r");
    if (file == NULL) { //outra clausula de erro padrão
        return -1;
    }

    i = 0;
    while (fgets(c[i].line, sizeof(c[i].line), file) != NULL) {
		sscanf(c[i].line, "%s", teste); //avalia o primeiro comando

		if(strlen(teste) == 3 || strcmp(teste, "INPUT") == 0){ //se o primeiro comando for SET, ADD, SUB, MUL, DIV, INPUT, presupomos que uma variável será declarada
			quantidade_de_variaveis++; //aumentamos a quantidade de variáveis com base na suposição anterior
		}

        size_t len = strlen(c->line);
        if (len > 0 && c[i].line[len - 1] == '\n') { //caso o ultimo caracter seja \n troca-o por \0
            c[i].line[len - 1] = '\0';
        }
        i++;
    }
    fclose(file);
    return 0;
}

int linha_vazia(const char *str) { //função que avalia se uma linha está vazia, caso no qual, faz o programa ignorar uma linha inteira para poupar tempo
	while (*str != '\0') {
		if (!isspace((unsigned char)*str)) {
			return 0;
		}
		str++;
	}
	return 1;
}

int identificarComando(const char *comando) { /*recebe o comando (primeira string de cada linha) e 
retorna o valor que irá ser usado para chamar a função que executa o comando correspondente*/
    if (strcmp(comando, "SET") == 0) return 1;
    if (strcmp(comando, "ADD") == 0) return 2;
    if (strcmp(comando, "SUB") == 0) return 3;
    if (strcmp(comando, "MUL") == 0) return 4;
    if (strcmp(comando, "DIV") == 0) return 5;
    if (strcmp(comando, "EXIBIR") == 0) return 6;
    if (strcmp(comando, "LOOP") == 0) return 7;
	if (strcmp(comando, "ENDLOOP") == 0) return 8;
	if (strcmp(comando, "IF") == 0) return 9;
	if (strcmp(comando, "ENDIF") == 0) return 10;
	if (strcmp(comando, "INPUT") == 0) return 11;
	erro = 1; //Se a função chega até aqui, o comando digitado não é reconhecido. Logo, temos um erro
    return -1;
}

void SET(char c[], Variavel *var, int k){ //Função SET
	int temporaria = 0;
	int teste_variavel_ja_existente = 0;
	char variavel_ja_existente[32];
	
	if(sscanf(c, "%*s %d", &temporaria) == 1){ //confere se o úsuario não tentou usa um número para nomear variáveis
		erro = 1;
		printf("Numeros nao podem nomear variaveis. Linha %d: %s", k+1, c);
		return ;
	}
	
	if(sscanf(c, "%*s %*s %s", completude) != 1){ //Verifica o tamanho do comando, menos que 3 palavras configura comando incorreto
		erro = 1;
		printf("Comando incompleto na linha %d: %s", k+1, c);
		return ;
	}
	
	if(sscanf(c, "%*s %*s %*s %s", completude) != 0){ //descobre se o usuario não excedeu o numero esperado de comandos
		erro = 1;
		printf("Comando com termos demais na linha %d: %s", k+1, c);
		return ;
	}

	sscanf(c, "%*s %s", variavel_para_busca);

	for(int prv = 0; prv <= contadorDeVariaveis; prv++){ //verifica se o úsuario está usando uma variável já existente
        if(strcmp(var[prv].nome, variavel_para_busca) == 0){ //caso ela já exista:
            if(sscanf(c, "%*s %*s %d", &temporaria) == 1){ //confere se o úsuario está tentando usar outra variavel para iniciar esta
            	var[prv].valor = temporaria; //caso não esteja, pega o valor inteiro e associa a variavel
			}
            else{ //caso esteja, procura a variavel correspondente e associa o seu valor no lugar do inteiro esperado
            	sscanf(c, "%*s %*s %s", variavel_ja_existente);
            	for(int atp = 0; atp < contadorDeVariaveis; prv++){
            		if(strcmp(var[atp].nome, variavel_ja_existente) == 0){
               			var[prv].valor = var[atp].valor;
                		break;
            		}
            		if(atp == contadorDeVariaveis - 1){ //caso não seja possivel encontrar a variável buscada:
                		printf("Variavel inexistente na linha %d: %s\n", k + 1, c);
                		erro = 1;
                		return ;
            		}
        		}
			}
		return ;
        }
    }
	
	char temp[32];
	//caso seja uma variável inedita:
    if(sscanf(c, "%*s %s %d", temp, &temporaria) == 2){ //avalia se o valor final é inteiro
    	strcpy(var[contadorDeVariaveis].nome, temp);
		var[contadorDeVariaveis].valor = temporaria;
	}
	else{//caso seja variavel, procura seu valor para poder usa-lo
		sscanf(c, "%*s %*s %s", variavel_ja_existente);
    	for(int atp = 0; atp <= contadorDeVariaveis; atp++){
           	if(strcmp(var[atp].nome, variavel_ja_existente) == 0){
           		strcpy(var[contadorDeVariaveis].nome, temp);
               	var[contadorDeVariaveis].valor = var[atp].valor;
               	contadorDeVariaveis++;
               	return ;
           	}
           	if(atp == contadorDeVariaveis){ //outra linha de erro
               	printf("Variavel inexistente na linha %d: %s\n", k + 1, c);
               	erro = 1;
                return ;
            }
        }
	}
	
	
	contadorDeVariaveis++; 
    
}

void INPUT(char c[], Variavel *var, int k){ //comando INPUT 
	char nome_var_input[32]; 
	int teste_erro = 0;
		
	if(sscanf(c, "%*s %*s %s", nome_var_input) != 0){ //avalia se o usuario não excedeu o limite esperado
		erro = 1;
		printf("INPUT com termos demais na linha %d: %s", k + 1, c);
		return ;
	}	
		
	if(sscanf(c, "%*s %d", teste_erro) == 1){
		printf("Tentativa de usar numero como variavel na linha %d: %s", k + 1, c);
	}
		
	if (sscanf(c, "%*s %s", nome_var_input) != 1) { //Associa a variável a ser escrita ao nome_var_input, 
		printf("Erro de sintaxe no comando INPUT: %s\n", c); //caso não hajam variaveis aqui alerta o úsuario do erro
		erro = 1;
		return;
	}

	int encontrado = 0;
	 
	for (int idx = 0; idx < contadorDeVariaveis; idx++) { //procura pela variavel;
		if (strcmp(var[idx].nome, nome_var_input) == 0) {
			printf("Digite um valor para %s: ", nome_var_input); //se ela existir, usa o seu "endereco" para alterar seu valor
			scanf("%d", &var[idx].valor);
			encontrado = 1;
			
			if (getchar() != '\n') { 
				printf("Erro: Valor invalido para a variavel '%s' na linha %d. Esperado um numero inteiro.\n", nome_var_input, k + 1);
				erro = 1;
				while (getchar() != '\n'); // limpa o buffer
				return;
			}
			encontrado = 1;
			break;
		}
	}

	if (!encontrado) { //se ela não existir, cria uma nova variável
		strcpy(var[contadorDeVariaveis].nome, nome_var_input);
		printf("Digite um valor para %s: ", nome_var_input);
		scanf("%d", &var[contadorDeVariaveis].valor);
		
		if (getchar() != '\n') {
			printf("Erro: Valor invalido para a variavel '%s' na linha %d. Esperado um numero inteiro.\n", nome_var_input, k + 1);
			erro = 1;
			while (getchar() != '\n'); // limpa o buffer
			return;
		}
		contadorDeVariaveis++;
	}
}


int operacao(char linha_comando[], Variavel *var, int k, char op) {
    int temp = 0, temp1 = 0;
    int teste_erro = 0;
    
    if(sscanf(linha_comando, "%*s %*s %*s %s", completude) != 1){ //avalia se o comando tem tantas palavras quanto esperado
    	erro = 1;
    	printf("Comando incompleto na linha %d: %s", k+1, linha_comando);
    	return -1;
	}
	
	if(sscanf(linha_comando, "%*s %*s %*s %*s %s", completude) != 0){ //avalia se o comando tem tantas palavras quanto esperado
		erro = 1;
		printf("Operacao aritmetica com termos inesperados na linha %d: %s", k+1, linha_comando);
		return -1;
	}
    
    
    if (sscanf(linha_comando, "%*s %d", &temp) != 1) { //descobre se estamos trabalhando com inteiros com var
        sscanf(linha_comando, "%*s %s", variavel_para_busca); //caso seja var:
        for(int prv = 0; prv <= contadorDeVariaveis; prv++){ //semelhante ao codigo de antes, descobre o valor da variavel digitada e armazena temporariamente
            if(strcmp(var[prv].nome, variavel_para_busca) == 0){
                temp = var[prv].valor;
                break;
            }
            if(prv == contadorDeVariaveis){ //clausula de erro
                printf("Variavel inexistente na linha %d: %s\n", k + 1, linha_comando);
                erro = 1;
                return -1;
            }
        }
    }

    if (sscanf(linha_comando, "%*s %*s %d", &temp1) != 1) { //mesmo procedimento que as linhas anteriores, só que para o segundo operando
        sscanf(linha_comando, "%*s %*s %s", variavel_para_busca);
        for(int prv = 0; prv <= contadorDeVariaveis; prv++){
            if(strcmp(var[prv].nome, variavel_para_busca) == 0){
                temp1 = var[prv].valor;
                break;
            }
            if(prv == contadorDeVariaveis){
                printf("Variavel inexistente na linha %d: %s\n", k + 1, linha_comando);
                erro = 1;
                return -1;
            }
        }
    }

	if(sscanf(linha_comando, "%*s %*s %*s %d", teste_erro) != 0){
		erro = 1;
		printf("Tentativa de usar numero inteiro para receber resultado de operacoes na linha %d: %s", k+1, linha_comando);
		return -1;
	}

    sscanf(linha_comando, "%*s %*s %*s %s", variavel_para_busca); //pega a variavel que usaremos
    int VarADD = 0; //variavel para testar se a variavel que usaremos já existe 

    for(int prv = 0; prv < contadorDeVariaveis; prv++){ //caso exista:
        if(strcmp(var[prv].nome, variavel_para_busca) == 0){ //encontra a variavel
            VarADD = 1;
            switch(op) { //faz com ela a operação sinalizada
                case '+': var[prv].valor = temp + temp1; break;
                case '-': var[prv].valor = temp - temp1; break;
                case '*': var[prv].valor = temp * temp1; break;
                case '/':
                    if (temp1 == 0) { //caso o úsuario tente pecar:
                        printf("Erro: Divisao por zero na linha %d: %s\n", k + 1, linha_comando);
                        erro = 1;
                        return -1;
                    }
                    var[prv].valor = temp / temp1; break;
            }
            return 0;
        }
    }

    if (!VarADD) { //caso seja variável inedita:
        strcpy(var[contadorDeVariaveis].nome, variavel_para_busca); //copia o nome da variavel na posição de um nova var
        switch(op) { //faz a operação sinalizada
            case '+': var[contadorDeVariaveis].valor = temp + temp1; break;
            case '-': var[contadorDeVariaveis].valor = temp - temp1; break;
            case '*': var[contadorDeVariaveis].valor = temp * temp1; break;
            case '/':
                if (temp1 == 0) {
                    printf("Erro: Divisao por zero na linha %d: %s\n", k + 1, linha_comando);
                    erro = 1;
                    return -1;
                }
                var[contadorDeVariaveis].valor = temp / temp1; break;
        }
        contadorDeVariaveis++; //nesse caso, como houve declaração de var, aumenta o valor do contador
        return 0;
    }
}

int EXIBIR(char c[], Variavel *var, int k){	//função EXIBIR
	int teste_erro = 0;
	
	if(sscanf(c, "%*s, %d", teste_erro) != 0){ //testa se o usuario esta tentando imprimit um inteiro
		printf("Tentativa de impressão de numero inteiro na linha %d: %s", k+1, c);
		erro = 1;
		return -1;
	} 
	
	if(sscanf(c, "%*s %s", completude) == 0){
		printf("EXIBIR incompleto na linha %d: %s", k+1, c);
		erro = 1;
		return -1;
	}
	
	if(sscanf(c, "%*s %*s %s", completude) != 0){ //testa se há termos demais
		printf("Comando EXIBIR com termos alem do tolerado na linha %d; %s ", k+1, c);
		erro = 1;
		return -1;
	}

	sscanf(c, "%*s %s", variavel_para_busca); 
	for(int prv = 0; prv <= contadorDeVariaveis; prv++){ //procura pelo valor da variavel a ser impresso
        if(strcmp(var[prv].nome, variavel_para_busca) == 0){
        	printf("%d\n", var[prv].valor); //imprime o valor
            return 0;
        }
        if(prv == contadorDeVariaveis){
           	printf("Variavel inexistente na linha %d: %s\n", k + 1, c);
            erro = 1;
            return -1;
       	}
    }
    return 0;
    
    
}

int LOOP(char c[], int *k, Variavel *var, int temp){ //comando LOOP
	int repeticoes = 0;
	if(sscanf(c, "%*s %s", completude) != 1){ //avalia o piso de termos
		printf("Comando loop incompleto na linha %d: %s", (*k)+1, c);
		erro = 1;
		return -1;
	}
	
	if(sscanf(c, "%*s %*s %s", completude) != 0){ //avalia o teto de termos
		printf("LOOP com termos alem do suportado na linha %d: %s", (*k)+1, c);
		erro = 1;
		return -1;
	}
	
	if(sscanf(c, "%*s %d", &repeticoes) == 1){ //se encontrar numero inteiro:
		if(repeticoes < 0){ //erro para inteiros < 0
			printf("Nao se pode repetir algo um numero negativo de vezes. Linha %d: %s", (*k)+1, c);
			erro = 1;
			return -1;
		}
		else if(repeticoes == 0){ //caso seja 0 pula para ENDLOOP que está na posição temp
			*k = temp; //note que estamos alterando direto no endereço. Sem ponteiros isso seria impossivel
		}
		else if(repeticoes > 0){ //caso seja um valor positivo retorna-o
			return repeticoes;
		}
    }
	else{ //se encontrar var
		sscanf(c, "%*s %s", variavel_para_busca);
		for(int prv = 0; prv <= contadorDeVariaveis; prv++){
        	if(strcmp(var[prv].nome, variavel_para_busca) == 0){ //obtem o valor da var e o impoe o mesmo tratamento anterior
        		if(var[prv].valor < 0){ //erro para inteiros < 0
					printf("Nao se pode repetir algo um numero negativo de vezes. Linha %d: %s", (*k)+1, c);
					erro = 1;
					return -1;
				}
				else if(var[prv].valor == 0){ //caso seja 0 pula para ENDLOOP que está na posição temp
					*k = temp; //note que estamos alterando direto no endereço. Sem ponteiros isso seria impossivel
				}
				else if(var[prv].valor > 0){ //caso seja um valor positivo retorna-o
					return var[prv].valor;
				}
           		return var[prv].valor; 
            	break;
        	}
        	if(prv == contadorDeVariaveis){
            	printf("Variavel inexistente na linha %d: %s\n", (*k) + 1, c);
            	erro = 1;
            	return -1;
        	}
    	}
	}
}

void IF(char c[], int *k, Variavel *var, int fim){ //Ccomando IF
	char operadordoif[3];
	int tempif1 = 0, tempif2 = 0;
	
	if(sscanf(c, "%*s %*s %*s %s", completude) != 1){
		printf("Sintaxa incompleta na linha %d: %s", (*k)+1, c);
		erro = 1;
		return ;
	}
	
	if(sscanf(c, "%*s %*s %*s %*s %s", completude) != 0){
		printf("Comando IF com termos demais na linha %d: %s", (*k)+1, c);
		erro = 1;
		return ;
	}
	
	if(sscanf(c, "%*s %d", &tempif1) != 1){ //tenta associar o valor inteiro a tempif1
		sscanf(c, "%*s %s", variavel_para_busca); //caso seja var, procura seu valor e associa-o, caso houver, a tempif1
    	for(int prv = 0; prv < contadorDeVariaveis; prv++){
       		if(strcmp(var[prv].nome, variavel_para_busca) == 0){
            	tempif1 = var[prv].valor;
            	break;
        	}
        	if(prv == contadorDeVariaveis - 1){
            	printf("Variavel inexistente na linha %d: %s\n", (*k) + 1, c);
            	erro = 1;
            	return ;
       		}
    	}
	}
	
	if(sscanf(c, "%*s %*s %*s %d", &tempif2) != 1){ //mesma coisa para o segundo operando
		sscanf(c, "%*s %*s %*s %s", variavel_para_busca);
    	for(int prv = 0; prv < contadorDeVariaveis; prv++){
        	if(strcmp(var[prv].nome, variavel_para_busca) == 0){
           		tempif2 = var[prv].valor;
            	break;
        	}
        	if(prv == contadorDeVariaveis - 1){
            	printf("Variavel inexistente na linha %d: %s\n", (*k) + 1, c);
            	erro = 1;
            	return ;
        	}
    	}
	}
	
	sscanf(c, "%*s %*s %s", operadordoif); //pega o operador logico >, < ou ==
	if(strcmp(operadordoif, "<") == 0){
		if(tempif1 < tempif2){ //faz nada
		return ;
		}
		else{
			(*k) = fim; //pula até ENDIF
			return ;
		}
	}
	else if(strcmp(operadordoif, ">") == 0) {
		if(tempif1 > tempif2){ //nada
		return ;
		}
		else{
			(*k) = fim; //pula
			return ;
		}
	}
	else if(strcmp(operadordoif, "==") == 0){
		if(tempif1 == tempif2){
		return ; //tambem nada
		}
		else{
			(*k) = fim;
			return ; //tambem pula
		}
	}
	else{ //clausula de erro
		printf("Operador logico inexistente na linha %d: %s", (*k) + 1, c);
		erro = 1;
		return ;
	}
}

int ONDE_ESTA_O_FIM(int k, Comands *c, const char qualfim[]){ //procura o ENDLOOP ou ENDIF
	int aux = 0;
	char procura_o_fim[32]; //percorre todo o arranjo c
				for(aux = k; aux < i; aux++){
					sscanf(c[aux].line, "%s", procura_o_fim);
					if(strcmp(procura_o_fim, qualfim) == 0){
						break;
					}
					
					if(aux == i){ //informa da inexistencia do fim
						printf("O comando iniciado em %d: %s\nNao possui fim\n", k+1, c->line[k]);
						erro = 1;
						return -1;
					}
				}
	return aux; //retorna a posição do fim
}

int main() { //comando principal
	int x = linhas();
	if(x == -1){
		printf("Erro ao abrir o arquivo programa.txt\n");
		return -1;
	}
	
	Comands c[x]; //declara tantos struct Comands quanto forem lidas linhas pela função linhas()
	char comando[10]; //declara um  char com tamanho 10 para receber comandos. (o maior comando tem 8 caracteres ENDLOOP + \0
	x = read(c); //chama a função que copia o txt nos arranjos 
	
	if (x == -1) {
		printf("Erro ao abrir o arquivo programa.txt\n");
		return -1;
	}

	int repeticoesloop = 0, comecoloop = 0, temp = 0; //variaveis de controle
	Variavel var[quantidade_de_variaveis]; //cria tantas Variaveis quanto o programar tiver suposto existir

	for (int k = 0; k < i; k++) { //executa i vezes. (i é a quantidade de linhas no programa)
		if (linha_vazia(c[k].line)) continue; //se o retorno da função linha_vazia for 1 ignora essa linha e pula proxima
		else{
			sscanf(c[k].line, "%s", comando); //obtem a primeira string da linha
			int codigo = identificarComando(comando); //manda essa string para a função de desambiguação
			switch (codigo) { //manda o programa executar a função que lida com o comando identificado
				case 1:
					SET(c[k].line, var, k); //chama e envia as informações necessárias para as funções
					break;
				case 2:
					operacao(c[k].line, var, k, '+');
					break;
				case 3:
					operacao(c[k].line, var, k, '-');
					break;
				case 4:
					operacao(c[k].line, var, k, '*');
					break;
				case 5:
					operacao(c[k].line, var, k, '/');
					break;
				case 6:
					EXIBIR(c[k].line, var, k);
					break;
				case 7:
					temp = ONDE_ESTA_O_FIM(k, c, "ENDLOOP");
					if(temp == -1){
						break;
					}
					comecoloop = k; //guarda a origem do loop em uma das variavies de controle
					repeticoesloop = LOOP(c[k].line, &k, var, temp); //obtem o valor de repetições
					break;
				case 8: //ENDLOOP
					if(repeticoesloop > 1){ //se houverem repetições a serem feitas altera k para a origem do loop
						k = comecoloop;
						repeticoesloop--; //reduz as repetições em um
					}
					break;
				case 9:
					temp = ONDE_ESTA_O_FIM(k, c, "ENDIF"); //encontra o fim
					if(temp == -1){
						break;
					}
					IF(c[k].line, &k, var, temp); //essa função altera o endereço de k caso o if seja falso mandando-o para o ENDIF e, caso contrario, não faz nada
					break;
				case 10: //ENDIF
				//faz nada
					break;
				case 11: 
					INPUT(c[k].line, var, k);
					break;
				default:
					printf("Erro de sintaxe ou comando desconhecido na linha %d: %s\n", k + 1, c[k].line); //caso o comando não seja reconhecido
			}
			if(erro == 1){//para imediatamnete a execução caso encontre um erro
				break;
			}
		}
	}

	return 0;
}

