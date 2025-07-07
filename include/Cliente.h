#ifndef CLIENTE_H
#define CLIENTE_H
#include <string>
#include "ListaInt.h"

/**
 * @brief Representa um cliente com nome e listas de pacotes enviados e recebidos.
 */
class Cliente {

    std::string nome;
    ListaInt pacotesRemetente; 
    ListaInt pacotesDestinatario; 

public:
    Cliente(const std::string& nome);
    const std::string& getNome() const;
    void adicionarPacoteRemetente(int idPacote);
    void imprimeCliente() const;
    void adicionarPacoteDestinatario(int idPacote);
    const ListaInt& getPacotesDestinatario() const; 
    const ListaInt& getPacotesRemetente() const; 

};

#endif 