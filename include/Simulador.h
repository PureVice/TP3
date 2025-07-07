#ifndef SISTEMA_LOGISTICO_HPP
#define SISTEMA_LOGISTICO_HPP

#include "ArvorePacotes.h"
#include "ArvoreClientes.h"
#include "ArvoreEventos.h"
#include "Evento.h"
#include "Pacote.h"
#include "Cliente.h"
#include "ListaEventos.h"
#include "ListaPacotes.h"
#include <string>

using namespace std;

// A classe Simulador orquestra toda a simulação logística,
// gerenciando pacotes, clientes e eventos.
class Simulador {
private:
    ArvorePacotes pacotes;
    ArvoreClientes clientes;
    ArvoreEventos eventos;

    Pacote* getPacote(int idPacote) const;
    Pacote* createPacote(int idPacote);
    Cliente* getCliente(const std::string& nome) const;
    Cliente* createCliente(const std::string& nome);

public:
    Simulador();
    ~Simulador();
    void carregarEventos(const std::string& nomeArquivo);
    void processarEvento(const Evento& evento);
    void processarConsulta(const string& linha);

    // Retorna o histórico de eventos de um pacote específico.
    ListaEventos getHistoricoPacote(int idPacote) const;
    // Retorna os pacotes associados a um cliente.
    ListaPacotes getPacotesCliente(const string& nomeCliente) const;
};

#endif