#ifndef ARVORE_EVENTOS_H
#define ARVORE_EVENTOS_H

#include "Evento.h"
#include "ListaEventos.h"
#include <string>

struct NoEvento {
    Evento* dados;
    NoEvento* esquerda;
    NoEvento* direita;
    int altura;

    NoEvento(Evento* dados);
};

class ArvoreEventos {
private:
    NoEvento* raiz;
    int contador;

    void limpar(NoEvento* no);
    int altura(NoEvento* no) const;
    int getBalanceamento(NoEvento* no) const;
    void setAltura(NoEvento* no);
    NoEvento* rotacaoDireita(NoEvento* y);
    NoEvento* rotacaoEsquerda(NoEvento* x);
    NoEvento* balancear(NoEvento* no);
    NoEvento* inserirNo(NoEvento* no, Evento* dados);
    NoEvento* removerNo(NoEvento* no, int chave);
    NoEvento* buscarNo(NoEvento* no, int chave) const;
    NoEvento* noMinimo(NoEvento* no) const;
    void emOrdem(NoEvento* no, void (*visitar)(Evento*)) const;
    int getChave(Evento* evento) const;
    void coletarEmOrdem(NoEvento* no, ListaEventos& lista) const;
    void coletarNoIntervalo(NoEvento* no, int tempoInicio, int tempoFim, ListaEventos& lista) const;

public:
    ArvoreEventos();
    ~ArvoreEventos();


    ArvoreEventos(const ArvoreEventos&) = delete;
    ArvoreEventos& operator=(const ArvoreEventos&) = delete;

    void inserir(Evento* dados);
    void remover(int chave);
    Evento* buscar(int chave) const;
    void emOrdem(void (*visitar)(Evento*)) const;
    int tamanho() const;
    ListaEventos getTodosEventos() const;
    ListaEventos getEventosNoIntervalo(int tempoInicio, int tempoFim) const;
};

#endif