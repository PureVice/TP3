#ifndef ARVORE_PACOTES_H
#define ARVORE_PACOTES_H

#include "Pacote.h"

struct NoPacote {
    Pacote* pacote;
    NoPacote* esquerda;
    NoPacote* direita;
    int altura;

    NoPacote(Pacote* pacote);
};

class ArvorePacotes {
private:
    NoPacote* raiz;
    int totalPacotes; 

    void limpar(NoPacote* no);
    int altura(NoPacote* no) const;
    int fatorBalanceamento(NoPacote* no) const;
    void atualizarAltura(NoPacote* no);
    NoPacote* rotacaoDireita(NoPacote* y);
    NoPacote* rotacaoEsquerda(NoPacote* x);
    NoPacote* balancear(NoPacote* no);
    NoPacote* inserir(NoPacote* no, Pacote* pacote);
    NoPacote* remover(NoPacote* no, int chave);
    NoPacote* buscar(NoPacote* no, int chave) const;
    NoPacote* noMinimo(NoPacote* no) const;
    void emOrdem(NoPacote* no, void (*visitar)(Pacote*)) const;

public:
    ArvorePacotes();
    ~ArvorePacotes();

    ArvorePacotes(const ArvorePacotes&) = delete;
    ArvorePacotes& operator=(const ArvorePacotes&) = delete;

    void inserir(Pacote* pacote);
    void remover(int chave);
    Pacote* buscar(int chave) const;
    void emOrdem(void (*visitar)(Pacote*)) const;
    int tamanho() const;
};

#endif 