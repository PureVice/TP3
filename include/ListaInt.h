#ifndef LISTA_INT_H
#define LISTA_INT_H

#include <iostream>
#include <cstdlib> 

// Classe de lista duplamente encadeada de inteiros.
// Permite enfileirar elementos, copiar listas e iterar sobre os itens.
// Possui métodos para verificar tamanho e se está vazia.
class ListaInt {
private:
    struct No {
        int dados;
        No* proximo;
        No* anterior;

        No(int d) : dados(d), proximo(nullptr), anterior(nullptr) {}
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
    ListaInt() : cabeca(nullptr), cauda(nullptr), tamanho(0) {}

    ~ListaInt() {
        limpar();
    }
    
    ListaInt(const ListaInt& outra) {
        cabeca = cauda = nullptr;
        tamanho = 0;
        for (No* atual = outra.cabeca; atual != nullptr; atual = atual->proximo) {
            enfileirar(atual->dados);
        }
    }

    ListaInt& operator=(const ListaInt& outra) {
        if (this != &outra) {
            limpar();
            for (No* atual = outra.cabeca; atual != nullptr; atual = atual->proximo) {
                enfileirar(atual->dados);
            }
        }
        return *this;
    }

    void enfileirar(int dados) {
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

        int& operator*() {
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
