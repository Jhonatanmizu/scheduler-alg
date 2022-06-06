#include <stdio.h>
#include <stdlib.h>
#include <locale.h>


struct processos{
	int tamProc, totJaProc , IdProc , prioridade ;
	bool terminou ;

}; // Propiedades que todos os processos criados hedarão;


int procQtdTot = 0; // Quantidade total de processos;
int procQtdPendente = 0; // Quantidade total de processos pendentes;
int quantumCpu = 3; //Quantum da CPU, 3 por padrão;
int pontodeparada = 0; //Guarda a posição na struct aonde o processamento foi parado;
int quantunUsados = 0;  //Guarda quantos quantuns aquele processo usou dessa vez;
int cont = 0; // Guarda a posição da criação dos processos

processos processo[256]; // Quantidade máxima de processos

void criaProcesso(); 
void listarProcessos();
void escalonar();
void processar();


//Função MAIN cuida do menu e da chamada das funções do projeto;
int main(){
	
	setlocale(LC_ALL, "Portuguese");
	
	int op; // Referente a opção do menu principal
	int op2 = 0; // Referente a criação especifica de um processo;
	int op3 = 0; // Referente a confirmação da criação de novos processos;
	
	do{
		printf("=======MENU=======\n");
		printf("[1] %c Definir quantidade de processos\n", 26);
		printf("[2] %c Adicionar UM processo\n",26);
		printf("[3] %c Redefinir quantum do CPU - Atual: (%d)\n", 26,quantumCpu);
		printf("[4] %c Listar Processos - Pendente: (%d) - Total: (%d)\n", 26,procQtdPendente, procQtdTot);
		printf("[5] %c Processar\n",26);
		printf("[6] %c Sair\n",26);
		printf("\nOpção: ");
		scanf("%d", &op);
		
		switch(op){
		case 1:
			system("cls");
			printf("%c%cSERÁ CRIADO PROCESSOS DE ACORDO COM A QUANTIDADE INFORMADA ABAIXO%c%c",16,16,17,17);
			if(procQtdPendente <= 0){
				printf("\nDefina a nova quantidade de processos: ");
				scanf("%d", &procQtdTot);
				procQtdPendente = procQtdTot;
				fflush(stdin);
				criaProcesso();
			}else{
				printf("\a"); // Toca um som ao entrar nesta opção;
				printf("\n%c%c%cESTA OPÇÃO REDEFINE TODOS OS PROCESSOS!%c%c%c\n",16,16,16,17,17,17);
				printf("Existem [%d] processos pendentes...\n", procQtdPendente);
				printf("Deseja continuar mesmo assim?\n[1] %c Sim\n[2] %c Não\nOpção: ",26,26);
				scanf("%d", &op3);
				if(op3 == 1){
					printf("\nDefina a nova quantidade de processos: ");
					scanf("%d", &procQtdTot);
					procQtdPendente = procQtdTot;
					fflush(stdin);
					criaProcesso();
				}
			}
			break;
		case 2:
			system("cls");
			op2 = 0;
			printf("[1] %c Adicionar UM processo aleatório\n",26);
			printf("[2] %c Adicionar UM processo específico\n",26);
			printf("\nInforme qualquer outro valor para voltar ao menu.\n");
			printf("Opção: ");
			scanf("%d", &op2);
			fflush(stdin);
			switch(op2){
				case 1:
					procQtdTot++;
					procQtdPendente++;
					criaProcesso();
					break;
				case 2:
					procQtdTot++;
					procQtdPendente++;
					printf("\nDefina o ID: ");
					scanf("%d", &processo[cont].IdProc);
					printf("\nDefina o tamanho do processo: ");
					scanf("%d", &processo[cont].tamProc);
					printf("\nDefina a prioridade: ");
					scanf("%d", &processo[cont].prioridade);
					
					processo[cont].totJaProc = 0;
					processo[cont].terminou = false;
					
					cont++;
					break;
				default:
					printf("Opção Inválida");
			}
			break;
		case 3:
			system("cls");
			printf("Quantum atual: %d\n", quantumCpu);
			printf("Informe o novo quantum: ");
			scanf("%d", &quantumCpu);
			fflush(stdin);
			break;
		case 4:
			system("cls");
			listarProcessos();
			break;
		case 5:
			system("cls");
			printf("================================\nIniciando Processamento...\n================================\n");
			escalonar();
			processar();
			break;
		case 6:
			exit(1);
			break;
		default:
			printf("Opção Inválida!");
			break;
		}
		system("cls");
}while(op != 6);

return 0;
    
}
//Esta função cria processos de forma aleatória;
void criaProcesso(){
    for (cont; cont < procQtdTot; cont++) {
		processo[cont].tamProc = 3+rand()%12;  //Gera um processo de tamanho aleatório de 3 a 15;
		processo[cont].IdProc = 1+cont; // Gera ID apartir do número 1;
		processo[cont].totJaProc = 0; // Processo acabou de ser criando, então ainda não foi enviado ao processador
		processo[cont].prioridade = 1+rand()%4; // Propiedade aleatória entre 1 a 5;
		processo[cont].terminou = false; // Indica que o processo ainda está pendente;
	}
}
//Esta função listar todos os processos, pendentes ou não;
void listarProcessos(){
	for (int i = 0;i < procQtdTot; i++){
		printf("=======================================\n");
		printf("Processo %d\n", i+1);
		printf(" %c ID: %d\n",16 , processo[i].IdProc);
		printf(" %c Tamanho: %d\n",16 , processo[i].tamProc);
		printf(" %c Quantidade Processada: %d\n",16 , processo[i].totJaProc);
		printf(" %c Prioridade: %d\n",16 , processo[i].prioridade);
		if(processo[i].terminou == false){
			printf(" %c Status: Pendende\n", 16);
		}else{
			printf(" %c Status: Concluído\n", 16);
		}
	}
	printf("\nPressione ENTER para voltar ao menu...");
	getchar();
	getchar();
}
//Esta função calcula as quantidades de quantums usados por cada processo e exibe o progresso no processamento;
void processar(){
	int op3;
	int i = pontodeparada, x;
	do{
		if(procQtdPendente == 0){
			printf("\a");
			printf("\n=======================================");
			printf("\nTodos os processos pendentes foram concluídos...");
			printf("\nPressione ENTER para voltar ao MENU...");
			getchar();
			getchar();
			break;
		} // Checa se existem processos pendentes, caso não haja, sai da função de processamento e retorna ao menu;
		
		if(processo[i].terminou == true || processo[i].IdProc == 0){
			while(processo[i].terminou == true && procQtdTot > 0 || processo[i].IdProc == 0){
				if(processo[i].terminou == true || processo[i].IdProc == 0){
					i++;
						if(i > procQtdTot){
						i=0;
					}
				}else{
					break;
				}
				
			}
		} // Checa se o próximo processo é válido para ser processado, checa antes do iniciar o processamento, 
		  // já que outro processo pode ter cido adicionado após o inicio do processamento;
		
		quantunUsados = 0; // Conta a quantidade de quantuns que foi usado pelo processo no momento;
		x = 0; // Conta as tentativas de processamentos do processo, isso garante que o processo ira passar pelo processamento ao chegar neste ponto;  
		
		do{
			if(processo[i].totJaProc >= processo[i].tamProc){
				processo[i].terminou = true;
				printf("\a");
				printf("O processo de ID [%d] foi concluído...\n", processo[i].IdProc);
				procQtdPendente--;
				break;
			}else{
				processo[i].totJaProc++;
				quantunUsados++;
			}
			x++; // Incrementa uma tentativa;	
		}while(x < quantumCpu);
		
		if(procQtdPendente == 0){
			printf("\a");
			printf("\n=======================================");
			printf("\nTodos os processos pendentes foram concluídos...\n");
			printf("\nPressione ENTER para voltar ao MENU...");
			getchar();
			getchar();
			break;
		} // Após sair de um processo, checa se ainda existem processos pendentes;
		
		printf("O processo de ID [%d] usou [%d] quantums...\n", processo[i].IdProc, quantunUsados);
		
		i++; // Passa para o próximo processo;
		
		if(i >= procQtdTot){
			i=0;
		} // Caso próximo processo não exista, retorna ao inicio da lista;
		
		if(processo[i].terminou == true || processo[i].IdProc == 0){
			while(processo[i].terminou == true && procQtdTot > 0 || processo[i].IdProc == 0){
				if(processo[i].terminou == true || processo[i].IdProc == 0){
					i++;
						if(i > procQtdTot){
						i=0;
					}
				}else{
					break;
				}
			}
		} // Checa se o próximo processo é válido para ser processado, caso não haja mais nenhum processo pendente, sai da função de processamento;
				
		
		printf("O próximo processo será o de ID [%d]...\n", processo[i].IdProc);
		
		printf("\nContinuar o processamento? \n[1] %c Sim\n[2] %c Não\nOpção: ",26,26);
		scanf("%d", &op3);
		printf("\n=======================================\n");
		fflush(stdin);
		if(op3 == 2){
			pontodeparada = i;
			processo[i].prioridade ++; // Caso o processo não seja executado quando deveria a sua prioridade deve subir;
		} // Caso o processamento seja pausado, salva o ponto de parada para continuar o processamento de onde parou;	
		
	}while(op3 != 2);
}
//Função responsável por organizar os processos por ordem de prioridade;
void escalonar(){

	processos aux[1]; //Cria um struct auxiliar para ordenar os processos;
	
	int n = procQtdTot; // Ao atinjir a quantidade total de processos, encerra o escalonamento;
	for (int k = 1; k < n; k++) {
        for (int j = 0; j < n - 1; j++) {
            if (processo[j].prioridade < processo[j+1].prioridade) {
                aux[0].prioridade = processo[j].prioridade;
                aux[0].tamProc = processo[j].tamProc;
                aux[0].totJaProc = processo[j].totJaProc;
                aux[0].IdProc = processo[j].IdProc;
                aux[0].terminou = processo[j].terminou;
                
                processo[j].prioridade = processo[j+1].prioridade;
                processo[j].tamProc = processo[j+1].tamProc;
                processo[j].totJaProc = processo[j+1].totJaProc;
                processo[j].IdProc = processo[j+1].IdProc;
                processo[j].terminou = processo[j+1].terminou;
                
                processo[j+1].prioridade = aux[0].prioridade;
                processo[j+1].tamProc = aux[0].tamProc;
                processo[j+1].totJaProc = aux[0].totJaProc;
                processo[j+1].IdProc = aux[0].IdProc;
                processo[j+1].terminou = aux[0].terminou;
            }
        }
    }
}
