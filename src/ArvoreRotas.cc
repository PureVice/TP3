#include "ArvoreRotas.h"
#include <algorithm> // Para std::max

// Construtor do Nó
NoRota::NoRota(int origem, int destino) : dados(origem, destino), esquerda(nullptr), direita(nullptr), altura(1) {}

// Construtor da Árvore
ArvoreRotas::ArvoreRotas() : raiz(nullptr) {}

// Destrutor
ArvoreRotas::~ArvoreRotas() {
    limpar(raiz);
}

void ArvoreRotas::limpar(NoRota* no) {
    if (no) {
        limpar(no->esquerda);
        limpar(no->direita);
        delete no;
    }
}

// Gera uma chave única para a rota para ordenação na árvore
long long ArvoreRotas::getChave(int origem, int destino) const {
    return static_cast<long long>(origem) << 32 | destino;
}

long long ArvoreRotas::getChave(const Rota& rota) const {
    return getChave(rota.origem, rota.destino);
}

int ArvoreRotas::altura(NoRota* no) const {
    return no ? no->altura : 0;
}

void ArvoreRotas::setAltura(NoRota* no) {
    if (no) {
        no->altura = 1 + std::max(altura(no->esquerda), altura(no->direita));
    }
}

int ArvoreRotas::getBalanceamento(NoRota* no) const {
    return no ? altura(no->esquerda) - altura(no->direita) : 0;
}

NoRota* ArvoreRotas::rotacaoDireita(NoRota* y) {
    NoRota* x = y->esquerda;
    NoRota* T2 = x->direita;
    x->direita = y;
    y->esquerda = T2;
    setAltura(y);
    setAltura(x);
    return x;
}

NoRota* ArvoreRotas::rotacaoEsquerda(NoRota* x) {
    NoRota* y = x->direita;
    NoRota* T2 = y->esquerda;
    y->esquerda = x;
    x->direita = T2;
    setAltura(x);
    setAltura(y);
    return y;
}

NoRota* ArvoreRotas::balancear(NoRota* no) {
    setAltura(no);
    int balanco = getBalanceamento(no);
    if (balanco > 1) { // Esquerda
        if (getBalanceamento(no->esquerda) < 0) {
            no->esquerda = rotacaoEsquerda(no->esquerda);
        }
        return rotacaoDireita(no);
    }
    if (balanco < -1) { // Direita
        if (getBalanceamento(no->direita) > 0) {
            no->direita = rotacaoDireita(no->direita);
        }
        return rotacaoEsquerda(no);
    }
    return no;
}


NoRota* ArvoreRotas::inserirNo(NoRota* no, int origem, int destino) {
    if (!no) return new NoRota(origem, destino);

    long long chaveNova = getChave(origem, destino);
    long long chaveAtual = getChave(no->dados);

    if (chaveNova < chaveAtual) {
        no->esquerda = inserirNo(no->esquerda, origem, destino);
    } else if (chaveNova > chaveAtual) {
        no->direita = inserirNo(no->direita, origem, destino);
    } else {
        // A rota já existe, não faz nada aqui pois o incremento é feito antes
        return no;
    }
    return balancear(no);
}

NoRota* ArvoreRotas::buscarNo(NoRota* no, int origem, int destino) const {
    if (!no) return nullptr;

    long long chaveBusca = getChave(origem, destino);
    long long chaveAtual = getChave(no->dados);

    if (chaveBusca < chaveAtual) {
        return buscarNo(no->esquerda, origem, destino);
    } else if (chaveBusca > chaveAtual) {
        return buscarNo(no->direita, origem, destino);
    } else {
        return no;
    }
}

void ArvoreRotas::incrementar(int origem, int destino) {
    NoRota* no = buscarNo(raiz, origem, destino);
    if (no) {
        no->dados.contagem++;
    } else {
        raiz = inserirNo(raiz, origem, destino);
    }
}

void ArvoreRotas::coletarRotas(NoRota* no, ListaRotas& lista) const {
    if (no) {
        coletarRotas(no->esquerda, lista);
        lista.inserirOrdenado(no->dados);
        coletarRotas(no->direita, lista);
    }
}

ListaRotas ArvoreRotas::getRotasOrdenadas() const {
    ListaRotas lista;
    coletarRotas(raiz, lista);
    return lista;
}