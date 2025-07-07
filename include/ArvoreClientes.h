#ifndef ARVORE_CLIENTES_H
#define ARVORE_CLIENTES_H
#include "Cliente.h"
#include <string>

struct NoCliente {
    Cliente* dados;
    NoCliente* esquerda;
    NoCliente* direita;
  
    int altura;
    NoCliente(Cliente* dados);
};

// Classe para gerenciar uma árvore AVL de clientes.
// Métodos privados para manipulação interna da árvore.
// Métodos públicos para inserir, remover, buscar clientes e obter o tamanho.
class ArvoreClientes {
private:
    NoCliente* raiz;
    void limpar(NoCliente* no);
    int altura(NoCliente* no) const;
    int getBalanceamento(NoCliente* no) const;
    void setAltura(NoCliente* no);
    int contador;
    NoCliente* rotacaoDireita(NoCliente* y);
    NoCliente* rotacaoEsquerda(NoCliente* x);
    NoCliente* balancear(NoCliente* no);
    NoCliente* inserirNo(NoCliente* no, Cliente* dados);
    NoCliente* removerNo(NoCliente* no, const std::string& chave);
    NoCliente* buscarNo(NoCliente* no, const std::string& chave) const;
    NoCliente* noMinimo(NoCliente* no) const;
    

    public:

    ArvoreClientes();
    ~ArvoreClientes();
    void inserir(Cliente* dados);
    void remover(const std::string& chave);
    Cliente* buscar(const std::string& chave) const;
    int tamanho() const;
};

#endif 