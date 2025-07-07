#include "ArvoreClientes.h"

// Retorna o maior entre dois inteiros
int maximo1(int a, int b) {
    return (a > b) ? a : b;
}

// Construtor do nó: inicializa com dados do cliente e ponteiros nulos
NoCliente::NoCliente(Cliente *d) : dados(d), esquerda(nullptr), direita(nullptr), altura(1) {}

// Construtor da árvore: inicializa raiz nula e contador zerado
ArvoreClientes::ArvoreClientes() : raiz(nullptr), contador(0) {}

// Destrutor: limpa toda a árvore recursivamente
ArvoreClientes::~ArvoreClientes() {
    limpar(raiz);
}

// Insere novo cliente na árvore e incrementa contador
void ArvoreClientes::inserir(Cliente *novoCliente) {
    raiz = inserirNo(raiz, novoCliente);
    contador++;
}

// Limpa a árvore recursivamente (pós-ordem)
void ArvoreClientes::limpar(NoCliente *no) {
    if (no) {
        limpar(no->esquerda);
        limpar(no->direita);
        delete no;
    }
}

// Retorna a altura de um nó (0 se nulo)
int ArvoreClientes::altura(NoCliente *no) const {
    return no ? no->altura : 0;
}

// Atualiza a altura de um nó baseado nos filhos
void ArvoreClientes::setAltura(NoCliente *no) {
    if (no) {
        no->altura = 1 + maximo1(altura(no->esquerda), altura(no->direita));
    }
}

// Calcula fator de balanceamento (diferença de alturas)
int ArvoreClientes::getBalanceamento(NoCliente *no) const {
    if (!no) return 0;
    return altura(no->esquerda) - altura(no->direita);
}

// Rotação simples à direita para balanceamento
NoCliente *ArvoreClientes::rotacaoDireita(NoCliente *noDesbalanceado) {
    NoCliente *novaRaiz = noDesbalanceado->esquerda;
    NoCliente *subarvoreDoMeio = novaRaiz->direita;
    
    novaRaiz->direita = noDesbalanceado;
    noDesbalanceado->esquerda = subarvoreDoMeio;
    
    setAltura(noDesbalanceado);
    setAltura(novaRaiz);
    
    return novaRaiz;
}

// Rotação simples à esquerda para balanceamento
NoCliente *ArvoreClientes::rotacaoEsquerda(NoCliente *noDesbalanceado) {
    NoCliente *novaRaiz = noDesbalanceado->direita;
    NoCliente *subarvoreDoMeio = novaRaiz->esquerda;
    
    novaRaiz->esquerda = noDesbalanceado;
    noDesbalanceado->direita = subarvoreDoMeio;
    
    setAltura(noDesbalanceado);
    setAltura(novaRaiz);
    
    return novaRaiz;
}

// Balanceia a árvore AVL conforme necessário
NoCliente *ArvoreClientes::balancear(NoCliente *no) {
    int balanco = getBalanceamento(no);
    setAltura(no);
    
    // Caso esquerda-esquerda
    if (balanco > 1) {
        if (getBalanceamento(no->esquerda) < 0) {  // Caso esquerda-direita
            no->esquerda = rotacaoEsquerda(no->esquerda);
        }
        return rotacaoDireita(no);
    }
    
    // Caso direita-direita
    if (balanco < -1) {
        if (getBalanceamento(no->direita) > 0) {  // Caso direita-esquerda
            no->direita = rotacaoDireita(no->direita);
        }
        return rotacaoEsquerda(no);
    }
    
    return no;  // Já balanceado
}

// Insere nó mantendo a propriedade de árvore binária e balanceando
NoCliente *ArvoreClientes::inserirNo(NoCliente *noAtual, Cliente *novoCliente) {
    if (!noAtual) return new NoCliente(novoCliente);
    
    // Inserção ordenada por nome
    if (novoCliente->getNome() < noAtual->dados->getNome()) {
        noAtual->esquerda = inserirNo(noAtual->esquerda, novoCliente);
    } 
    else if (novoCliente->getNome() > noAtual->dados->getNome()) {
        noAtual->direita = inserirNo(noAtual->direita, novoCliente);
    } 
    else {
        return noAtual;  // Nome já existe
    }
    
    return balancear(noAtual);  // Balanceia após inserção
}

// Busca cliente por nome (retorna nullptr se não encontrar)
Cliente *ArvoreClientes::buscar(const std::string &nomeBusca) const {
    NoCliente *resultado = buscarNo(raiz, nomeBusca);
    return resultado ? resultado->dados : nullptr;
}

// Busca recursiva por nome na árvore
NoCliente *ArvoreClientes::buscarNo(NoCliente *noAtual, const std::string &nomeBusca) const {
    if (!noAtual || noAtual->dados->getNome() == nomeBusca) {
        return noAtual;
    }
    return buscarNo(
        (nomeBusca < noAtual->dados->getNome()) ? noAtual->esquerda : noAtual->direita,
        nomeBusca
    );
}

// Remove cliente por nome
void ArvoreClientes::remover(const std::string &nomeRemover) {
    raiz = removerNo(raiz, nomeRemover);
}

// Remove nó e balanceia a árvore
NoCliente *ArvoreClientes::removerNo(NoCliente *noAtual, const std::string &nomeRemover) {
    if (!noAtual) return noAtual;
    
    // Busca o nó a ser removido
    if (nomeRemover < noAtual->dados->getNome()) {
        noAtual->esquerda = removerNo(noAtual->esquerda, nomeRemover);
    } 
    else if (nomeRemover > noAtual->dados->getNome()) {
        noAtual->direita = removerNo(noAtual->direita, nomeRemover);
    } 
    else {
        // Nó com 0 ou 1 filho
        if (!noAtual->esquerda || !noAtual->direita) {
            NoCliente *filho = noAtual->esquerda ? noAtual->esquerda : noAtual->direita;
            
            if (!filho) {  // Nó folha
                delete noAtual;
                contador--;
                return nullptr;
            } 
            else {  // Nó com 1 filho
                *noAtual = *filho;  // Substitui pelo filho
                delete filho;
                contador--;
            }
        } 
        else {  // Nó com 2 filhos
            NoCliente *sucessor = noMinimo(noAtual->direita);  // Menor da subárvore direita
            noAtual->dados = sucessor->dados;  // Copia dados do sucessor
            noAtual->direita = removerNo(noAtual->direita, sucessor->dados->getNome());  // Remove o sucessor
        }
    }
    
    return balancear(noAtual);  // Balanceia após remoção
}

// Encontra nó com valor mínimo (mais à esquerda)
NoCliente *ArvoreClientes::noMinimo(NoCliente *noAtual) const {
    while (noAtual && noAtual->esquerda) {
        noAtual = noAtual->esquerda;
    }
    return noAtual;
}

// Retorna quantidade de clientes na árvore
int ArvoreClientes::tamanho() const {
    return contador;
}