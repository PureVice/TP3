#include "Cliente.h"

Cliente::Cliente(const std::string &nome)
{
    this->nome = nome;
}
void Cliente::imprimeCliente() const
{
    std::cout << "Nome: " << nome << std::endl;

    std::cout << "Pacotes Remetente: ";
    pacotesRemetente.imprimir();
    std::cout << std::endl;

    std::cout << "Pacotes Destinatario: ";
    pacotesDestinatario.imprimir(); 
    std::cout << std::endl;
}
void Cliente::adicionarPacoteDestinatario(int idPacote)
{
    pacotesDestinatario.enfileirar(idPacote);
}

void Cliente::adicionarPacoteRemetente(int idPacote)
{
    pacotesRemetente.enfileirar(idPacote);
}

const std::string &Cliente::getNome() const
{
    return nome;
}

const ListaInt &Cliente::getPacotesRemetente() const
{
    return pacotesRemetente;
}

const ListaInt &Cliente::getPacotesDestinatario() const
{
    return pacotesDestinatario;
}