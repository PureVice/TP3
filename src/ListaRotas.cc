#include "ListaRotas.h"

void ListaRotas::limpar() {
    No* atual = cabeca;
    while (atual != nullptr) {
        No* proximoNode = atual->proximo;
        delete atual;
        atual = proximoNode;
    }
    cabeca = cauda = nullptr;
    tamanho = 0;
}

ListaRotas::ListaRotas() : cabeca(nullptr), cauda(nullptr), tamanho(0) {}

ListaRotas::~ListaRotas() {
    limpar();
}

ListaRotas::ListaRotas(const ListaRotas& outra) {
    cabeca = cauda = nullptr;
    tamanho = 0;
    for (No* atual = outra.cabeca; atual != nullptr; atual = atual->proximo) {
        push_back(atual->dados);
    }
}

ListaRotas& ListaRotas::operator=(const ListaRotas& outra) {
    if (this != &outra) {
        limpar();
        for (No* atual = outra.cabeca; atual != nullptr; atual = atual->proximo) {
            push_back(atual->dados);
        }
    }
    return *this;
}

void ListaRotas::push_back(const Rota& dados) {
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

// Insere de forma que a maior contagem fique no início
void ListaRotas::inserirOrdenado(const Rota& dados) {
    No* novoNo = new No(dados);
    if (!cabeca || dados.contagem > cabeca->dados.contagem) {
        novoNo->proximo = cabeca;
        if (cabeca) cabeca->anterior = novoNo;
        cabeca = novoNo;
        if (!cauda) cauda = novoNo;
    } else {
        No* atual = cabeca;
        while (atual->proximo && atual->proximo->dados.contagem >= dados.contagem) {
            atual = atual->proximo;
        }
        novoNo->proximo = atual->proximo;
        if (atual->proximo) atual->proximo->anterior = novoNo;
        atual->proximo = novoNo;
        novoNo->anterior = atual;
        if (!novoNo->proximo) cauda = novoNo;
    }
    tamanho++;
}


int ListaRotas::getTamanho() const {
    return tamanho;
}