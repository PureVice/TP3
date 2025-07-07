#ifndef LISTA_PACOTES_H
#define LISTA_PACOTES_H

#include "Pacote.h"
#include "ParPacoteString.h"
#include <string>
#include <iostream>
#include <cstdlib>

// Classe de lista duplamente encadeada para armazenar pares PacoteEString.
// Permite inserção no final, cópia, iteração e limpeza automática.
// Possui iterador interno para percorrer os elementos.
class ListaPacotes {
private:
    using TipoPar = PacoteEString;

    struct No {
        TipoPar dados;
        No* proximo;
        No* anterior;

        No(const TipoPar& d) : dados(d), proximo(nullptr), anterior(nullptr) {}
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
    ListaPacotes() : cabeca(nullptr), cauda(nullptr), tamanho(0) {}

    ~ListaPacotes() {
        limpar();
    }

    ListaPacotes(const ListaPacotes& outra) {
        cabeca = cauda = nullptr;
        tamanho = 0;
        for (No* atual = outra.cabeca; atual != nullptr; atual = atual->proximo) {
            push_back(atual->dados);
        }
    }

    ListaPacotes& operator=(const ListaPacotes& outra) {
        if (this != &outra) {
            limpar();
            for (No* atual = outra.cabeca; atual != nullptr; atual = atual->proximo) {
                push_back(atual->dados);
            }
        }
        return *this;
    }

    void push_back(const TipoPar& dados) {
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

        TipoPar& operator*() {
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
