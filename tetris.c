// #include <stdio.h>

// // Desafio Tetris Stack
// // Tema 3 - Integração de Fila e Pilha
// // Este código inicial serve como base para o desenvolvimento do sistema de controle de peças.
// // Use as instruções de cada nível para desenvolver o desafio.

// int main() {

//     // 🧩 Nível Novato: Fila de Peças Futuras
//     //
//     // - Crie uma struct Peca com os campos: tipo (char) e id (int).
//     // - Implemente uma fila circular com capacidade para 5 peças.
//     // - Crie funções como inicializarFila(), enqueue(), dequeue(), filaCheia(), filaVazia().
//     // - Cada peça deve ser gerada automaticamente com um tipo aleatório e id sequencial.
//     // - Exiba a fila após cada ação com uma função mostrarFila().
//     // - Use um menu com opções como:
//     //      1 - Jogar peça (remover da frente)
//     //      0 - Sair
//     // - A cada remoção, insira uma nova peça ao final da fila.



//     // 🧠 Nível Aventureiro: Adição da Pilha de Reserva
//     //
//     // - Implemente uma pilha linear com capacidade para 3 peças.
//     // - Crie funções como inicializarPilha(), push(), pop(), pilhaCheia(), pilhaVazia().
//     // - Permita enviar uma peça da fila para a pilha (reserva).
//     // - Crie um menu com opção:
//     //      2 - Enviar peça da fila para a reserva (pilha)
//     //      3 - Usar peça da reserva (remover do topo da pilha)
//     // - Exiba a pilha junto com a fila após cada ação com mostrarPilha().
//     // - Mantenha a fila sempre com 5 peças (repondo com gerarPeca()).


//     // 🔄 Nível Mestre: Integração Estratégica entre Fila e Pilha
//     //
//     // - Implemente interações avançadas entre as estruturas:
//     //      4 - Trocar a peça da frente da fila com o topo da pilha
//     //      5 - Trocar os 3 primeiros da fila com as 3 peças da pilha
//     // - Para a opção 4:
//     //      Verifique se a fila não está vazia e a pilha tem ao menos 1 peça.
//     //      Troque os elementos diretamente nos arrays.
//     // - Para a opção 5:
//     //      Verifique se a pilha tem exatamente 3 peças e a fila ao menos 3.
//     //      Use a lógica de índice circular para acessar os primeiros da fila.
//     // - Sempre valide as condições antes da troca e informe mensagens claras ao usuário.
//     // - Use funções auxiliares, se quiser, para modularizar a lógica de troca.
//     // - O menu deve ficar assim:
//     //      4 - Trocar peça da frente com topo da pilha
//     //      5 - Trocar 3 primeiros da fila com os 3 da pilha


//     return 0;
// }


#include <stdio.h>
#include <stdlib.h>  // Para rand(), srand(), system()
#include <string.h>  // Para strcpy()
#include <stdbool.h> // Para o tipo bool
#include <time.h>    // Para srand(time(NULL))

// --- Constantes do Jogo ---
#define TAM_FILA 5   // Nível Novato: Fila de 5 peças
#define TAM_PILHA 3  // Nível Aventureiro: Pilha de 3 peças

// --- Estrutura de Dados Base ---
typedef struct {
    char nome[10]; // 'I', 'O', 'T', 'L', 'S', 'Z', 'J'
    int id;        // ID exclusivo
} Peca;

// --- Estruturas de Controle Globais ---

// Fila Circular (Nível Novato)
Peca fila[TAM_FILA];
int inicio_fila = 0;
int fim_fila = 0;
int total_fila = 0; // Mais fácil para controlar fila cheia/vazia

// Pilha Linear (Nível Aventureiro)
Peca pilha[TAM_PILHA];
int topo_pilha = -1; // -1 indica pilha vazia

// --- Controle de Estado (Nível Mestre) ---
// Enum para sabermos qual foi a última ação
typedef enum {
    NENHUMA,
    JOGAR,    // 1
    RESERVAR, // 2
    USAR,     // 3
    TROCAR    // 4
} Acao;

// Estrutura para salvar a última jogada e permitir "Desfazer"
typedef struct {
    Acao tipo;
    Peca peca1; // Guarda a peça principal da ação (ex: a peça jogada)
    Peca peca2; // Guarda a peça secundária (ex: a nova peça que entrou)
} UltimaJogada;

UltimaJogada logDesfazer; // Variável global para o log

// --- Variáveis Auxiliares ---
int proximoId = 1; // Para garantir IDs exclusivos
// Nomes possíveis para as peças
const char pecaNomes[7][10] = {"I", "O", "T", "L", "S", "Z", "J"};

// --- Funções Auxiliares (Qualidade de Vida) ---
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pausar() {
    printf("\nPressione Enter para continuar...");
    limparBuffer();
}

// --- Função de Geração de Peças (Nível Novato) ---
Peca gerarPeca() {
    Peca p;
    // Escolhe um nome aleatório da lista
    strcpy(p.nome, pecaNomes[rand() % 7]); 
    p.id = proximoId++; // Atribui o ID e incrementa
    return p;
}

// --- Funções da Fila Circular (Nível Novato) ---
bool filaEstaCheia() {
    return total_fila == TAM_FILA;
}

bool filaEstaVazia() {
    return total_fila == 0;
}

// Enqueue
void enfileirar(Peca p) {
    if (filaEstaCheia()) {
        printf("ERRO: Fila cheia (isso nao deveria acontecer)\n");
        return;
    }
    fila[fim_fila] = p;
    fim_fila = (fim_fila + 1) % TAM_FILA; // Lógica circular
    total_fila++;
}

// Dequeue
Peca desenfileirar() {
    Peca pVazia = {"---", 0};
    if (filaEstaVazia()) {
        printf("ERRO: Fila vazia (isso nao deveria acontecer)\n");
        return pVazia;
    }
    Peca p = fila[inicio_fila];
    inicio_fila = (inicio_fila + 1) % TAM_FILA; // Lógica circular
    total_fila--;
    return p;
}

// --- Funções Especiais da Fila (Nível Mestre - Para Desfazer) ---
// Remove a peça do FIM da fila
Peca removerDoFim() {
    Peca pVazia = {"---", 0};
    if (filaEstaVazia()) return pVazia;
    
    // Move o ponteiro 'fim' para trás
    fim_fila = (fim_fila - 1 + TAM_FILA) % TAM_FILA;
    total_fila--;
    return fila[fim_fila]; // Retorna a peça que estava no fim
}

// Insere uma peça no INÍCIO da fila
void inserirNoInicio(Peca p) {
    if (filaEstaCheia()) return;
    
    // Move o ponteiro 'inicio' para trás
    inicio_fila = (inicio_fila - 1 + TAM_FILA) % TAM_FILA;
    fila[inicio_fila] = p;
    total_fila++;
}


// --- Funções da Pilha (Nível Aventureiro) ---
bool pilhaEstaCheia() {
    return topo_pilha == TAM_PILHA - 1;
}

bool pilhaEstaVazia() {
    return topo_pilha == -1;
}

// Push
void empilhar(Peca p) {
    if (pilhaEstaCheia()) {
        printf("ERRO: Pilha de reserva cheia!\n");
        return;
    }
    topo_pilha++;
    pilha[topo_pilha] = p;
}

// Pop
Peca desempilhar() {
    Peca pVazia = {"---", 0};
    if (pilhaEstaVazia()) {
        printf("ERRO: Pilha de reserva vazia!\n");
        return pVazia;
    }
    Peca p = pilha[topo_pilha];
    topo_pilha--;
    return p;
}

// --- Inicialização e Exibição ---

void inicializar() {
    // Inicializa a fila com 5 peças (Nível Novato)
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(gerarPeca());
    }
    // Reseta o log de "Desfazer"
    logDesfazer.tipo = NENHUMA;
}

void exibirEstado() {
    printf("==================== TETRIS STACK ====================\n\n");
    
    // 1. Exibir Pilha de Reserva (Aventureiro)
    printf("--- PILHA DE RESERVA (%d/%d) --- \n", topo_pilha + 1, TAM_PILHA);
    printf(" (Topo)\n");
    if (pilhaEstaVazia()) {
        printf(" [ Vazio ]\n [ Vazio ]\n [ Vazio ]\n");
    } else {
        // Imprime de cima para baixo
        for (int i = TAM_PILHA - 1; i >= 0; i--) {
            if (i <= topo_pilha) {
                printf(" [%s-%d]\n", pilha[i].nome, pilha[i].id);
            } else {
                printf(" [ Vazio ]\n");
            }
        }
    }
    printf(" (Base)\n");

    // 2. Exibir Fila de Próximas Peças (Novato)
    printf("\n--- FILA DE PROXIMAS PECAS (%d/%d) --- \n", total_fila, TAM_FILA);
    printf("(Frente) -> ");
    if (filaEstaVazia()) {
        printf("[ Vazia ]");
    } else {
        // Itera de forma circular
        int i = inicio_fila;
        for (int count = 0; count < total_fila; count++) {
            printf("[%s-%d] ", fila[i].nome, fila[i].id);
            i = (i + 1) % TAM_FILA;
        }
    }
    printf("<- (Fim)\n");
    printf("======================================================\n");
}

// --- Funções de Menu (Nível Mestre) ---

// 1. Jogar peça (Dequeue + Enqueue)
void jogarPeca() {
    if (filaEstaVazia()) {
        printf("Fila esta vazia, nada para jogar.\n");
        return;
    }
    
    Peca pecaJogada = desenfileirar();
    Peca pecaNova = gerarPeca();
    enfileirar(pecaNova); // Mantém a fila sempre cheia

    printf("Voce jogou a peca [%s-%d].\n", pecaJogada.nome, pecaJogada.id);
    printf("Uma nova peca [%s-%d] entrou na fila.\n", pecaNova.nome, pecaNova.id);
    
    // Salva a ação para o "Desfazer"
    logDesfazer.tipo = JOGAR;
    logDesfazer.peca1 = pecaJogada;
    logDesfazer.peca2 = pecaNova;
}

// 2. Reservar peça (Dequeue -> Push)
void reservarPeca() {
    if (pilhaEstaCheia()) {
        printf("Nao e possivel reservar: A pilha esta cheia!\n");
        return;
    }
    if (filaEstaVazia()) {
        printf("Nao e possivel reservar: A fila esta vazia!\n");
        return;
    }
    
    Peca pecaReservada = desenfileirar();
    empilhar(pecaReservada);
    
    Peca pecaNova = gerarPeca();
    enfileirar(pecaNova); // Mantém a fila sempre cheia

    printf("Voce reservou a peca [%s-%d].\n", pecaReservada.nome, pecaReservada.id);
    
    // Salva a ação para o "Desfazer"
    logDesfazer.tipo = RESERVAR;
    logDesfazer.peca1 = pecaReservada;
    logDesfazer.peca2 = pecaNova;
}

// 3. Usar peça reservada (Pop)
void usarPecaReservada() {
    if (pilhaEstaVazia()) {
        printf("Nao e possivel usar: A pilha esta vazia!\n");
        return;
    }
    
    Peca pecaUsada = desempilhar();
    printf("Voce usou a peca reservada [%s-%d].\n", pecaUsada.nome, pecaUsada.id);
    
    // Salva a ação para o "Desfazer"
    logDesfazer.tipo = USAR;
    logDesfazer.peca1 = pecaUsada;
    logDesfazer.peca2 = (Peca){"---", 0}; // Peca2 não é usada aqui
}

// 4. Trocar peça do topo da pilha com a da frente da fila
void trocarPecas() {
    if (pilhaEstaVazia()) {
        printf("Troca falhou: Pilha vazia.\n");
        return;
    }
    if (filaEstaVazia()) {
        printf("Troca falhou: Fila vazia.\n");
        return;
    }

    Peca daPilha = desempilhar();
    Peca daFila = desenfileirar(); // Pega a da FRENTE
    
    empilhar(daFila);
    inserirNoInicio(daPilha); // Coloca a da pilha na FRENTE da fila

    printf("Troca realizada com sucesso!\n");
    printf("[%s-%d] (da pilha) foi para a frente da fila.\n", daPilha.nome, daPilha.id);
    printf("[%s-%d] (da fila) foi para o topo da pilha.\n", daFila.nome, daFila.id);
    
    // Salva a ação para o "Desfazer"
    logDesfazer.tipo = TROCAR;
    logDesfazer.peca1 = daPilha;
    logDesfazer.peca2 = daFila;
}

// 5. Desfazer última jogada
void desfazerUltimaJogada() {
    printf("--- DESFAZER ACAO ---\n");
    
    switch(logDesfazer.tipo) {
        case NENHUMA:
            printf("Nenhuma acao para desfazer.\n");
            break;
            
        case JOGAR:
            // JOGAR = desenfileirar(peca1) + enfileirar(peca2)
            // DESFAZER = removerDoFim() + inserirNoInicio(peca1)
            removerDoFim(); // Remove a pecaNova (peca2)
            inserirNoInicio(logDesfazer.peca1); // Devolve a pecaJogada
            printf("Acao 'Jogar Peca' desfeita.\n");
            logDesfazer.tipo = NENHUMA;
            break;

        case RESERVAR:
            // RESERVAR = desenfileirar(peca1) + empilhar(peca1) + enfileirar(peca2)
            // DESFAZER = removerDoFim() + desempilhar() + inserirNoInicio(peca1)
            removerDoFim(); // Remove a pecaNova (peca2)
            desempilhar(); // Remove a pecaReservada (peca1) da pilha
            inserirNoInicio(logDesfazer.peca1); // Devolve a pecaReservada para a fila
            printf("Acao 'Reservar Peca' desfeita.\n");
            logDesfazer.tipo = NENHUMA;
            break;
            
        case USAR:
            // USAR = desempilhar(peca1)
            // DESFAZER = empilhar(peca1)
            empilhar(logDesfazer.peca1); // Devolve a pecaUsada para a pilha
            printf("Acao 'Usar Peca Reservada' desfeita.\n");
            logDesfazer.tipo = NENHUMA;
            break;

        case TROCAR:
            // TROCAR = desempilhar(peca1) + desenfileirar(peca2) + empilhar(peca2) + inserirNoInicio(peca1)
            // DESFAZER = desempilhar(peca2) + desenfileirar(peca1) + empilhar(peca1) + inserirNoInicio(peca2)
            desempilhar(); // Remove peca2 da pilha
            desenfileirar(); // Remove peca1 da fila
            empilhar(logDesfazer.peca1); // Devolve peca1 para pilha
            inserirNoInicio(logDesfazer.peca2); // Devolve peca2 para fila
            printf("Acao 'Trocar Pecas' desfeita.\n");
            logDesfazer.tipo = NENHUMA;
            break;
            
        default:
            printf("Log de acao desconhecido.\n");
    }
}

// 6. Inverter fila com pilha
void inverterEstruturas() {
    printf("--- INVERTER ESTRUTURAS ---\n");
    
    // 1. Esvazia a Fila para um array temporário
    Peca tempFila[TAM_FILA];
    int countFila = 0;
    while (!filaEstaVazia()) {
        tempFila[countFila++] = desenfileirar();
    }

    // 2. Esvazia a Pilha para um array temporário
    Peca tempPilha[TAM_PILHA];
    int countPilha = 0;
    while (!pilhaEstaVazia()) {
        tempPilha[countPilha++] = desempilhar();
    }
    
    // 3. Coloca o conteúdo da Fila (tempFila) na Pilha
    printf("Movendo %d itens da Fila -> Pilha...\n", countFila);
    for (int i = 0; i < countFila; i++) {
        if (!pilhaEstaCheia()) {
            empilhar(tempFila[i]);
        } else {
            printf("Pilha ficou cheia. Peca [%s-%d] descartada.\n", 
                   tempFila[i].nome, tempFila[i].id);
        }
    }
    
    // 4. Coloca o conteúdo da Pilha (tempPilha) na Fila
    // (Note que os itens da pilha são enfileirados na ordem em que saíram,
    // ou seja, o topo da pilha vira o início da fila)
    printf("Movendo %d itens da Pilha -> Fila...\n", countPilha);
    for (int i = 0; i < countPilha; i++) {
        if (!filaEstaCheia()) {
            enfileirar(tempPilha[i]);
        } else {
            printf("Fila ficou cheia. Peca [%s-%d] descartada.\n",
                   tempPilha[i].nome, tempPilha[i].id);
        }
    }
    
    printf("Inversao concluida. (Esta acao nao pode ser desfeita).\n");
    logDesfazer.tipo = NENHUMA; // Invalida o "Desfazer"
}

// --- Função Main (Menu Interativo) ---
int main() {
    srand(time(NULL)); // Inicializa o gerador aleatório
    inicializar();
    
    int opcao;

    do {
        limparTela();
        exibirEstado();
        
        printf("\n--- MENU DE ACOES (Mestre) ---\n");
        printf("1. Jogar peca (da fila)\n");
        printf("2. Reservar peca (fila -> pilha)\n");
        printf("3. Usar peca reservada (da pilha)\n");
        printf("4. Trocar (topo da pilha <-> frente da fila)\n");
        printf("5. Desfazer ultima jogada\n");
        printf("6. Inverter Fila com Pilha (PANICO!)\n");
        printf("0. Sair do Jogo\n");
        printf("======================================\n");
        printf("Escolha sua jogada: ");
        
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                jogarPeca();
                break;
            case 2:
                reservarPeca();
                break;
            case 3:
                usarPecaReservada();
                break;
            case 4:
                trocarPecas();
                break;
            case 5:
                desfazerUltimaJogada();
                break;
            case 6:
                inverterEstruturas();
                break;
            case 0:
                printf("\nObrigado por jogar Tetris Stack! Ate a proxima.\n");
                break;
            default:
                printf("\nOpcao invalida. Tente novamente.\n");
        }

        if (opcao != 0) {
            pausar();
        }

    } while (opcao != 0);

    return 0;
}