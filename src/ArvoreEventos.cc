#include "ArvoreEventos.h"
#include <sstream>
#include <iomanip>

// Retorna o maior entre dois inteiros
int maximo(int a, int b) {
    return (a > b) ? a : b;
}

// Construtor do nó: inicializa com dados do evento e ponteiros nulos
NoEvento::NoEvento(Evento *ev) : dados(ev), esquerda(NULL), direita(NULL), altura(1) {}

// Gera chave única para ordenação dos eventos
int ArvoreEventos::getChave(Evento *ev) const {
    return gerarChaveEvento(*ev);
}


// Construtor: inicializa árvore vazia
ArvoreEventos::ArvoreEventos() : raiz(NULL), contador(0) {}

// Destrutor: limpa toda a árvore
ArvoreEventos::~ArvoreEventos() {
    limpar(raiz);
}

// Limpa a árvore recursivamente em pós-ordem
void ArvoreEventos::limpar(NoEvento *atual) {
    if (atual != NULL) {
        limpar(atual->esquerda);
        limpar(atual->direita);
        delete atual;
    }
}

// Retorna altura do nó (0 para nulo)
int ArvoreEventos::altura(NoEvento *atual) const {
    return atual ? atual->altura : 0;
}

// Atualiza altura do nó baseado nos filhos
void ArvoreEventos::setAltura(NoEvento *atual) {
    if (atual) {
        atual->altura = 1 + maximo(altura(atual->esquerda), altura(atual->direita));
    }
}

// Calcula fator de balanceamento do nó
int ArvoreEventos::getBalanceamento(NoEvento *atual) const {
    return atual ? altura(atual->esquerda) - altura(atual->direita) : 0;
}

// Rotação à direita para balanceamento AVL
NoEvento *ArvoreEventos::rotacaoDireita(NoEvento *pivot) {
    NoEvento *novoPivot = pivot->esquerda;
    NoEvento *subarvore = novoPivot->direita;
    
    novoPivot->direita = pivot;
    pivot->esquerda = subarvore;
    
    setAltura(pivot);
    setAltura(novoPivot);
    
    return novoPivot;
}

// Rotação à esquerda para balanceamento AVL
NoEvento *ArvoreEventos::rotacaoEsquerda(NoEvento *pivot) {
    NoEvento *novoPivot = pivot->direita;
    NoEvento *subarvore = novoPivot->esquerda;
    
    novoPivot->esquerda = pivot;
    pivot->direita = subarvore;
    
    setAltura(pivot);
    setAltura(novoPivot);
    
    return novoPivot;
}

// Balanceia a árvore conforme necessário
NoEvento *ArvoreEventos::balancear(NoEvento *atual) {
    setAltura(atual);
    int balanco = getBalanceamento(atual);
    
    // Caso esquerda-esquerda
    if (balanco > 1) {
        if (getBalanceamento(atual->esquerda) < 0) {  // Caso esquerda-direita
            atual->esquerda = rotacaoEsquerda(atual->esquerda);
        }
        return rotacaoDireita(atual);
    }
    
    // Caso direita-direita
    if (balanco < -1) {
        if (getBalanceamento(atual->direita) > 0) {  // Caso direita-esquerda
            atual->direita = rotacaoDireita(atual->direita);
        }
        return rotacaoEsquerda(atual);
    }
    
    return atual;  // Já balanceado
}

// Insere novo evento na árvore
void ArvoreEventos::inserir(Evento *novoEvento) {
    raiz = inserirNo(raiz, novoEvento);
    contador++;
}

// Insere nó mantendo ordenação e balanceamento
NoEvento *ArvoreEventos::inserirNo(NoEvento *atual, Evento *novoEvento) {
    if (!atual) return new NoEvento(novoEvento);
    
    int chaveNovo = getChave(novoEvento);
    int chaveAtual = getChave(atual->dados);
    
    if (chaveNovo != chaveAtual) {
        if (chaveNovo < chaveAtual) {
            atual->esquerda = inserirNo(atual->esquerda, novoEvento);
        } else {
            atual->direita = inserirNo(atual->direita, novoEvento);
        }
    }
    
    return balancear(atual);
}

// Busca evento por ID
Evento *ArvoreEventos::buscar(int id) const {
    NoEvento *resultado = buscarNo(raiz, id);
    return resultado ? resultado->dados : NULL;
}

// Busca recursiva por ID
NoEvento *ArvoreEventos::buscarNo(NoEvento *atual, int id) const {
    if (!atual || getChave(atual->dados) == id) {
        return atual;
    }
    
    return buscarNo(
        (id < getChave(atual->dados)) ? atual->esquerda : atual->direita,
        id
    );
}

// Remove evento por ID
void ArvoreEventos::remover(int id) {
    raiz = removerNo(raiz, id);
}

// Remove nó e rebalanceia a árvore
NoEvento *ArvoreEventos::removerNo(NoEvento *atual, int id) {
    if (!atual) return NULL;

    int chaveAtual = getChave(atual->dados);
    
    if (id < chaveAtual) {
        atual->esquerda = removerNo(atual->esquerda, id);
    } else if (id > chaveAtual) {
        atual->direita = removerNo(atual->direita, id);
    } else {
        // Nó com 0 ou 1 filho
        if (!atual->esquerda || !atual->direita) {
            NoEvento *temp = atual->esquerda ? atual->esquerda : atual->direita;
            
            if (!temp) {  // Nó folha
                delete atual;
                contador--;
                return NULL;
            } else {  // Nó com 1 filho
                *atual = *temp;  // Substitui pelo filho
                delete temp;
                contador--;
            }
        } else {  // Nó com 2 filhos
            NoEvento *sucessor = noMinimo(atual->direita);
            atual->dados = sucessor->dados;
            atual->direita = removerNo(atual->direita, getChave(sucessor->dados));
        }
    }
    
    return balancear(atual);
}

// Encontra nó com menor chave (mais à esquerda)
NoEvento *ArvoreEventos::noMinimo(NoEvento *inicio) const {
    while (inicio && inicio->esquerda) {
        inicio = inicio->esquerda;
    }
    return inicio;
}

// Percorre a árvore em ordem (esquerda-raiz-direita)
void ArvoreEventos::emOrdem(void (*visitar)(Evento *)) const {
    emOrdem(raiz, visitar);
}

void ArvoreEventos::emOrdem(NoEvento *atual, void (*visitar)(Evento *)) const {
    if (atual) {
        emOrdem(atual->esquerda, visitar);
        visitar(atual->dados);
        emOrdem(atual->direita, visitar);
    }
}

// Retorna todos os eventos em uma lista (ordem crescente)
ListaEventos ArvoreEventos::getTodosEventos() const {
    ListaEventos lista;
    coletarEmOrdem(raiz, lista);
    return lista;
}

// Coleta eventos em ordem na lista
void ArvoreEventos::coletarEmOrdem(NoEvento* no, ListaEventos& lista) const {
    if (no) {
        coletarEmOrdem(no->esquerda, lista);
        lista.push_back(*(no->dados));
        coletarEmOrdem(no->direita, lista);
    }
}

void ArvoreEventos::coletarNoIntervalo(NoEvento* no, int tempoInicio, int tempoFim, ListaEventos& lista) const {
    if (!no) {
        return;
    }

    int tempoAtual = no->dados->tempo;

    // A chave da árvore de eventos é baseada no tempo, então podemos podar a busca
    if (tempoInicio < tempoAtual) {
        coletarNoIntervalo(no->esquerda, tempoInicio, tempoFim, lista);
    }

    if (tempoAtual >= tempoInicio && tempoAtual <= tempoFim) {
        lista.push_back(*(no->dados));
    }
    
    if (tempoFim > tempoAtual) {
        coletarNoIntervalo(no->direita, tempoInicio, tempoFim, lista);
    }
}

// Nova implementação
ListaEventos ArvoreEventos::getEventosNoIntervalo(int tempoInicio, int tempoFim) const {
    ListaEventos lista;
    coletarNoIntervalo(raiz, tempoInicio, tempoFim, lista);
    return lista;
}



// Retorna quantidade de eventos
int ArvoreEventos::tamanho() const {
    return contador;
}