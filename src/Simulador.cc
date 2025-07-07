#include "Simulador.h"
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "ParPacoteString.h"

using namespace std;

Simulador::Simulador() {}

// Destrutor robusto para limpar toda a memória alocada dinamicamente.
Simulador::~Simulador()
{
    // Limpa todos os Eventos
    auto limparEventos = [](Evento *e) { delete e; };
    eventos.emOrdem(limparEventos);

    // Limpa todos os Pacotes
    auto limparPacotes = [](Pacote *p) { delete p; };
    pacotes.emOrdem(limparPacotes);
}

void Simulador::carregarEventos(const std::string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Erro ao abrir o arquivo: " + nomeArquivo);
    }

    string linha;
    int numeroLinha = 0;
    while (getline(arquivo, linha)) {
        numeroLinha++;
        if (linha.empty()) {
            continue;
        }
        istringstream iss(linha);
        int timestamp;
        string comando;
        iss >> timestamp >> comando;
        try {
            if (comando == "PC" || comando == "CL") {
                processarConsulta(linha); // Processa consultas
            } else if (comando == "EV") {
                processarEvento(Evento::lerEvento(linha)); // Processa eventos
            }
        } catch (const std::exception& e) {
            cerr << "Aviso: Erro ao processar a linha " << numeroLinha << ": " << e.what() << endl;
            // Continua o processamento das próximas linhas
        }
    }
}

// Busca um pacote pelo ID. Retorna nullptr se não encontrado.
Pacote* Simulador::getPacote(int idPacote) const {
    return pacotes.buscar(idPacote);
}

// Cria um novo pacote, o insere na árvore e o retorna.
Pacote* Simulador::createPacote(int idPacote) {
    Pacote* pct = new Pacote(idPacote);
    pacotes.inserir(pct);
    return pct;
}

// Busca um cliente pelo nome. Retorna nullptr se não encontrado.
Cliente* Simulador::getCliente(const std::string& nome) const {
    return clientes.buscar(nome);
}

// Cria um novo cliente, o insere na árvore e o retorna.
Cliente* Simulador::createCliente(const std::string& nome) {
    Cliente* cliente = new Cliente(nome);
    clientes.inserir(cliente);
    return cliente;
}

void Simulador::processarEvento(const Evento &evento)
{
    Pacote* pct = getPacote(evento.idPacote);
    if (!pct) {
        pct = createPacote(evento.idPacote); // Cria pacote se não existir
    }

    Evento *novoEvento = new Evento(evento);
    eventos.inserir(novoEvento);

    if (pct->getPrimeiroEvento() == nullptr)
        pct->setPrimeiroEvento(novoEvento);
    pct->setUltimoEvento(novoEvento);

    if (evento.tipo == RG)
    {
        // Atualiza remetente
        Cliente *remetente = getCliente(evento.remetente);
        if (!remetente) {
            remetente = createCliente(evento.remetente);
        }
        remetente->adicionarPacoteRemetente(evento.idPacote);

        // Atualiza destinatário
        Cliente *destinatario = getCliente(evento.destinatario);
        if(!destinatario) {
            destinatario = createCliente(evento.destinatario);
        }
        destinatario->adicionarPacoteDestinatario(evento.idPacote);
    }
}

ListaEventos Simulador::getHistoricoPacote(int idPacote) const
{
    ListaEventos todosEventos = eventos.getTodosEventos();
    ListaEventos resultado;

    for (auto it = todosEventos.begin(); it.eValido(); ++it)
        if ((*it).idPacote == idPacote)
            resultado.push_back(*it);

    return resultado;
}

// Imprime um evento formatado
void imprimirEvento(Evento *e)
{
    if (!e) return;
    cout << setfill('0') << setw(7) << e->tempo << " EV ";
    switch (e->tipo)
    {
        case RG: cout << "RG "; break;
        case AR: cout << "AR "; break;
        case RM: cout << "RM "; break;
        case UR: cout << "UR "; break;
        case TR: cout << "TR "; break;
        case EN: cout << "EN "; break;
    }
    cout << setfill('0') << setw(3) << e->idPacote;

    if (e->tipo == RG)
        cout << " " << e->remetente << " " << e->destinatario
             << " " << setfill('0') << setw(3) << e->armazemOrigem
             << " " << setfill('0') << setw(3) << e->armazemDestino;
    else if (e->tipo == AR)
        cout << " " << setfill('0') << setw(3) << e->armazemOrigem
             << " " << setfill('0') << setw(3) << e->armazemDestino;
    else if (e->tipo == RM || e->tipo == UR || e->tipo == TR)
        cout << " " << setfill('0') << setw(3) << e->armazemOrigem
             << " " << setfill('0') << setw(3) << e->armazemDestino;
    else if (e->tipo == EN)
        cout << " " << setfill('0') << setw(3) << e->armazemDestino;
    
    cout << endl;
}

void Simulador::processarConsulta(const string &linha)
{
    istringstream iss(linha);
    int timestamp;
    string tipo;

    if (!(iss >> timestamp >> tipo)) {
        throw std::runtime_error("Formato de consulta invalido: " + linha);
    }

    cout << setfill('0') << setw(6) << timestamp << " " << tipo;

    if (tipo == "PC")
    {
        int idPacote;
        if (!(iss >> idPacote)) {
            throw std::runtime_error("ID do pacote ausente na consulta PC: " + linha);
        }
        cout << " " << setfill('0') << setw(3) << idPacote << endl;

        ListaEventos historico = getHistoricoPacote(idPacote);
        cout << historico.getTamanho() << endl;

        for (auto it = historico.begin(); it.eValido(); ++it)
            imprimirEvento(&(*it));
    }
    else if (tipo == "CL")
    {
        string nomeCliente;
        if (!(iss >> nomeCliente)) {
             throw std::runtime_error("Nome do cliente ausente na consulta CL: " + linha);
        }
        cout << " " << nomeCliente << endl;

        Cliente *cliente = clientes.buscar(nomeCliente);
        if (!cliente)
        {
            cout << 0 << endl;
            return;
        }

        ArvoreEventos eventosRelevantes;
        // Adiciona eventos dos pacotes em que o cliente é remetente
        for (auto it = cliente->getPacotesRemetente().begin(); it.eValido(); ++it)
        {
            Pacote *pct = pacotes.buscar(*it);
            if (pct) {
                if (pct->getPrimeiroEvento()) eventosRelevantes.inserir(pct->getPrimeiroEvento());
                if (pct->getUltimoEvento() && pct->getUltimoEvento() != pct->getPrimeiroEvento()) eventosRelevantes.inserir(pct->getUltimoEvento());
            }
        }
        // Adiciona eventos dos pacotes em que o cliente é destinatário
        for (auto it = cliente->getPacotesDestinatario().begin(); it.eValido(); ++it)
        {
            Pacote *pct = pacotes.buscar(*it);
            if (pct) {
                if (pct->getPrimeiroEvento()) eventosRelevantes.inserir(pct->getPrimeiroEvento());
                if (pct->getUltimoEvento() && pct->getUltimoEvento() != pct->getPrimeiroEvento()) eventosRelevantes.inserir(pct->getUltimoEvento());
            }
        }

        cout << eventosRelevantes.tamanho() << endl;
        eventosRelevantes.emOrdem(imprimirEvento);
    }
}

ListaPacotes Simulador::getPacotesCliente(const string &nomeCliente) const
{
    ListaPacotes resultado;
    Cliente *cliente = clientes.buscar(nomeCliente);

    if (cliente)
    {
        // Pacotes em que o cliente é remetente
        for (auto it = cliente->getPacotesRemetente().begin(); it.eValido(); ++it)
        {
            Pacote *pct = pacotes.buscar(*it);
            if (pct) {
                resultado.push_back(PacoteEString(pct, "remetente"));
            }
        }
        // Pacotes em que o cliente é destinatário
        for (auto it = cliente->getPacotesDestinatario().begin(); it.eValido(); ++it)
        {
            Pacote *pct = pacotes.buscar(*it);
            if (pct) {
                resultado.push_back(PacoteEString(pct, "destinatario"));
            }
        }
    }
    return resultado;
}