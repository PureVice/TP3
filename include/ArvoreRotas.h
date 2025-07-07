#ifndef ARVORE_ROTAS_H
#define ARVORE_ROTAS_H

#include "ListaRotas.h"

struct NoRota {
    Rota dados;
    NoRota *esquerda;
    NoRota *direita;
    int altura;

    NoRota(int origem, int destino);
};

class ArvoreRotas {
private:
    NoRota* raiz;

    // Funções auxiliares da AVL
    void limpar(NoRota* no);
    int altura(NoRota* no) const;
    void setAltura(NoRota* no);
    int getBalanceamento(NoRota* no) const;
    long long getChave(int origem, int destino) const;
    long long getChave(const Rota& rota) const;

    NoRota* rotacaoDireita(NoRota* y);
    NoRota* rotacaoEsquerda(NoRota* x);
    NoRota* balancear(NoRota* no);

    NoRota* inserirNo(NoRota* no, int origem, int destino);
    NoRota* buscarNo(NoRota* no, int origem, int destino) const;
    
    void coletarRotas(NoRota* no, ListaRotas& lista) const;

public:
    ArvoreRotas();
    ~ArvoreRotas();

    void incrementar(int origem, int destino);
    ListaRotas getRotasOrdenadas() const;
};

#endif