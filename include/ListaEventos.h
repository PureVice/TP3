#ifndef LISTA_EVENTOS_H
#define LISTA_EVENTOS_H

#include "Evento.h"
#include <iostream>
#include <cstdlib>

// Classe para lista duplamente encadeada de eventos.
// Permite inserção no final, cópia, iteração e limpeza automática.
// Possui iterador simples para percorrer os eventos.
class ListaEventos {
private:
    struct No {
        Evento dados;
        No* proximo;
        No* anterior;

        No(const Evento& d) : dados(d), proximo(nullptr), anterior(nullptr) {}
    };
    
    No* cabeca;
    No* cauda;
    int tamanho;

    void limpar() {
        No* atual = cabeca;
        while (atual != nullptr) {
            No* proximoNode = atual->proximo;
            delete atual;
            atual = proximoNode;
        }
        cabeca = cauda = nullptr;
        tamanho = 0;
    }

public:
    ListaEventos() : cabeca(nullptr), cauda(nullptr), tamanho(0) {}

    ~ListaEventos() {
        limpar();
    }

    ListaEventos(const ListaEventos& outra) {
        cabeca = cauda = nullptr;
        tamanho = 0;
        for (No* atual = outra.cabeca; atual != nullptr; atual = atual->proximo) {
            push_back(atual->dados);
        }
    }

    ListaEventos& operator=(const ListaEventos& outra) {
        if (this != &outra) {
            limpar();
            for (No* atual = outra.cabeca; atual != nullptr; atual = atual->proximo) {
                push_back(atual->dados);
            }
        }
        return *this;
    }

    void push_back(const Evento& dados) {
        No* novoNo = new No(dados);
        if (!cabeca) {
            cabeca = cauda = novoNo;
        } else {
            cauda->proximo = novoNo;
            novoNo->anterior = cauda;
            cauda = novoNo;
        }
        tamanho++;
    }

    int getTamanho() const {
        return tamanho;
    }

    bool estaVazia() const {
        return tamanho == 0;
    }

    class Iterador {
    public:
        Iterador(No* no) : atual(no) {}
        
        bool eValido() const {
            return atual != nullptr;
        }

        Evento& operator*() {
            if (!atual) {
                std::cerr << "Erro fatal: Tentativa de desreferenciar um iterador invalido." << std::endl;
                exit(1);
            }
            return atual->dados;
        }

        Iterador& operator++() {
            if (atual) {
                atual = atual->proximo;
            }
            return *this;
        }

    private:
        No* atual;
    };

    Iterador begin() const {
        return Iterador(cabeca);
    }
};

#endif
