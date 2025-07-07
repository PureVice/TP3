#include "ArvorePacotes.h"

int max(int a, int b) {
    return (a > b) ? a : b;  // Retorna o maior entre dois inteiros
}


// Construtor do nó: inicializa com dados do pacote e ponteiros nulos
NoPacote::NoPacote(Pacote* d) : pacote(d), esquerda(nullptr), direita(nullptr), altura(1) {}

// Construtor da árvore: inicializa raiz nula e totalPacotes zerado
ArvorePacotes::ArvorePacotes() : raiz(nullptr), totalPacotes(0) {}

// Destrutor: limpa toda a árvore recursivamente
ArvorePacotes::~ArvorePacotes() {
    limpar(raiz);
}

// Limpa a árvore em pós-ordem (esquerda, direita, raiz)
void ArvorePacotes::limpar(NoPacote* no) {
    if (no) {
        limpar(no->esquerda);
        limpar(no->direita);
        delete no;
    }
}

// Retorna altura do nó (0 se nulo)
int ArvorePacotes::altura(NoPacote* no) const {
    return no ? no->altura : 0;
}

// Atualiza altura do nó baseado na altura dos filhos
void ArvorePacotes::atualizarAltura(NoPacote* no) {
    if (no) {
        no->altura = 1 + std::max(altura(no->esquerda), altura(no->direita));
    }
}

// Calcula fator de balanceamento (diferença de alturas dos filhos)
int ArvorePacotes::fatorBalanceamento(NoPacote* no) const {
    return no ? altura(no->esquerda) - altura(no->direita) : 0;
}

// Rotação simples à direita para balanceamento AVL
NoPacote* ArvorePacotes::rotacaoDireita(NoPacote* y) {
    NoPacote* x = y->esquerda;
    NoPacote* T2 = x->direita;
    
    // Realiza a rotação
    x->direita = y;
    y->esquerda = T2;
    
    // Atualiza alturas
    atualizarAltura(y);
    atualizarAltura(x);
    
    return x;  // Retorna nova raiz da subárvore
}

// Rotação simples à esquerda para balanceamento AVL
NoPacote* ArvorePacotes::rotacaoEsquerda(NoPacote* x) {
    NoPacote* y = x->direita;
    NoPacote* T2 = y->esquerda;
    
    // Realiza a rotação
    y->esquerda = x;
    x->direita = T2;
    
    // Atualiza alturas
    atualizarAltura(x);
    atualizarAltura(y);
    
    return y;  // Retorna nova raiz da subárvore
}

// Balanceia a árvore AVL conforme necessário
NoPacote* ArvorePacotes::balancear(NoPacote* no) {
    atualizarAltura(no);
    int balance = fatorBalanceamento(no);
    
    // Caso esquerda-esquerda
    if (balance > 1) {
        if (fatorBalanceamento(no->esquerda) < 0) {  // Caso esquerda-direita
            no->esquerda = rotacaoEsquerda(no->esquerda);
        }
        return rotacaoDireita(no);
    }
    
    // Caso direita-direita
    if (balance < -1) {
        if (fatorBalanceamento(no->direita) > 0) {  // Caso direita-esquerda
            no->direita = rotacaoDireita(no->direita);
        }
        return rotacaoEsquerda(no);
    }
    
    return no;  // Árvore já balanceada
}

// Interface pública para inserção
void ArvorePacotes::inserir(Pacote* dados) {
    raiz = inserir(raiz, dados);
    totalPacotes++;
}

// Inserção recursiva com balanceamento
NoPacote* ArvorePacotes::inserir(NoPacote* no, Pacote* dados) {
    if (!no) return new NoPacote(dados);  // Cria novo nó se chegou na posição correta
    
    // Inserção ordenada por ID
    if (dados->getId() < no->pacote->getId()) {
        no->esquerda = inserir(no->esquerda, dados);
    } else if (dados->getId() > no->pacote->getId()) {
        no->direita = inserir(no->direita, dados);
    } else {
        return no;  // ID já existe (não permite duplicatas)
    }
    
    return balancear(no);  // Balanceia após inserção
}

// Interface pública para busca
Pacote* ArvorePacotes::buscar(int chave) const {
    NoPacote* resultado = buscar(raiz, chave);
    return resultado ? resultado->pacote : nullptr;
}

// Busca recursiva por ID
NoPacote* ArvorePacotes::buscar(NoPacote* no, int chave) const {
    if (!no || no->pacote->getId() == chave) {
        return no;
    }
    return buscar(
        (chave < no->pacote->getId()) ? no->esquerda : no->direita,
        chave
    );
}

// Interface pública para remoção
void ArvorePacotes::remover(int chave) {
    raiz = remover(raiz, chave);
}

// Remoção recursiva com balanceamento
NoPacote* ArvorePacotes::remover(NoPacote* no, int chave) {
    if (!no) return no;  // Nó não encontrado

    // Busca o nó a ser removido
    if (chave < no->pacote->getId()) {
        no->esquerda = remover(no->esquerda, chave);
    } else if (chave > no->pacote->getId()) {
        no->direita = remover(no->direita, chave);
    } else {
        // Nó encontrado - trata casos de 0, 1 ou 2 filhos
        if (!no->esquerda || !no->direita) {
            NoPacote* temp = no->esquerda ? no->esquerda : no->direita;
            
            if (!temp) {  // Nó folha
                temp = no;
                no = nullptr;
            } else {  // Nó com 1 filho
                *no = *temp;  // Copia filho para o nó atual
            }
            delete temp;
            totalPacotes--;
        } else {  // Nó com 2 filhos
            NoPacote* temp = noMinimo(no->direita);  // Encontra sucessor
            no->pacote = temp->pacote;  // Copia dados do sucessor
            no->direita = remover(no->direita, temp->pacote->getId());  // Remove o sucessor
        }
    }

    if (!no) return no;  // Caso a árvore ficou vazia
    return balancear(no);  // Balanceia após remoção
}

// Encontra nó com menor valor (mais à esquerda)
NoPacote* ArvorePacotes::noMinimo(NoPacote* no) const {
    NoPacote* atual = no;
    while (atual && atual->esquerda) {
        atual = atual->esquerda;
    }
    return atual;
}

// Percorre a árvore em ordem (esquerda, raiz, direita)
void ArvorePacotes::emOrdem(void (*visitar)(Pacote*)) const {
    emOrdem(raiz, visitar);
}

void ArvorePacotes::emOrdem(NoPacote* no, void (*visitar)(Pacote*)) const {
    if (no) {
        emOrdem(no->esquerda, visitar);
        visitar(no->pacote);
        emOrdem(no->direita, visitar);
    }
}

// Retorna quantidade de pacotes na árvore
int ArvorePacotes::tamanho() const {
    return totalPacotes;
}