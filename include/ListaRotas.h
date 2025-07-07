#ifndef LISTA_ROTAS_H
#define LISTA_ROTAS_H

#include <iostream>
#include <cstdlib>

// Estrutura para armazenar informações da rota
struct Rota {
    int origem;
    int destino;
    int contagem;

    Rota(int o, int d) : origem(o), destino(d), contagem(1) {}
    Rota() : origem(-1), destino(-1), contagem(0) {} // Construtor padrão
};


class ListaRotas {
private:
    struct No {
        Rota dados;
        No* proximo;
        No* anterior;
        No(const Rota& d) : dados(d), proximo(nullptr), anterior(nullptr) {}
    };
    
    No* cabeca;
    No* cauda;
    int tamanho;

    void limpar();

public:
    ListaRotas();
    ~ListaRotas();
    ListaRotas(const ListaRotas& outra);
    ListaRotas& operator=(const ListaRotas& outra);

    void push_back(const Rota& dados);
    void inserirOrdenado(const Rota& dados); // Insere ordenado por contagem
    int getTamanho() const;
    
    class Iterador {
    public:
        Iterador(No* no) : atual(no) {}
        bool eValido() const { return atual != nullptr; }
        Rota& operator*() {
            if (!atual) {
                std::cerr << "Erro: Tentativa de desreferenciar iterador invalido." << std::endl;
                exit(1);
            }
            return atual->dados;
        }
        Iterador& operator++() {
            if (atual) atual = atual->proximo;
            return *this;
        }
    private:
        No* atual;
    };

    Iterador begin() const { return Iterador(cabeca); }
};

#endif