#ifndef LISTA_INT_H
#define LISTA_INT_H

#include <iostream>
#include <stdexcept>

// Lista duplamente encadeada de inteiros
class ListaInt
{
private:
    struct No
    {
        int dados;
        No *proximo;
        No *anterior;

        No(int d) : dados(d), proximo(nullptr), anterior(nullptr) {}
    };

    No *cabeca; // ponteiro para o primeiro nó
    No *cauda;  // ponteiro para o último nó
    int tamanho;

    // Remove todos os nós da lista
    void limpar()
    {
        No *atual = cabeca;
        while (atual != nullptr)
        {
            No *proximoNode = atual->proximo;
            delete atual;
            atual = proximoNode;
        }
        cabeca = cauda = nullptr;
        tamanho = 0;
    }

public:
    ListaInt() : cabeca(nullptr), cauda(nullptr), tamanho(0) {}

    ~ListaInt()
    {
        limpar();
    }

    // Construtor de cópia
    ListaInt(const ListaInt &outra)
    {
        cabeca = cauda = nullptr;
        tamanho = 0;
        for (No *atual = outra.cabeca; atual != nullptr; atual = atual->proximo)
        {
            enfileirar(atual->dados);
        }
    }

    // Operador de atribuição
    ListaInt &operator=(const ListaInt &outra)
    {
        if (this != &outra)
        {
            limpar();
            for (No *atual = outra.cabeca; atual != nullptr; atual = atual->proximo)
            {
                enfileirar(atual->dados);
            }
        }
        return *this;
    }

    // Adiciona elemento ao final da lista
    void enfileirar(int dados)
    {
        No *novoNo = new No(dados);
        if (!cabeca)
        {
            cabeca = cauda = novoNo;
        }
        else
        {
            cauda->proximo = novoNo;
            novoNo->anterior = cauda;
            cauda = novoNo;
        }
        tamanho++;
    }

    int getTamanho() const
    {
        return tamanho;
    }

    bool estaVazia() const
    {
        return tamanho == 0;
    }
    void imprimir() const
    {
        No *atual = cabeca;
        while (atual != nullptr)
        {
            std::cout << atual->dados << " ";
            atual = atual->proximo;
        }
        std::cout << std::endl;
    }
    // Classe iteradora para percorrer a lista
    class Iterador
    {
    public:
        Iterador(No *no) : atual(no) {}

        bool eValido() const
        {
            return atual != nullptr;
        }

        int &operator*()
        {
            if (!eValido())
            {
                throw std::out_of_range("Tentativa de desreferenciar um iterador invalido.");
            }
            return atual->dados;
        }

        // Avança para o próximo nó
        Iterador &operator++()
        {
            if (eValido())
            {
                atual = atual->proximo;
            }
            return *this;
        }

    private:
        No *atual;
    };

    // Retorna iterador para o início da lista
    Iterador begin() const
    {
        return Iterador(cabeca);
    }
};

#endif