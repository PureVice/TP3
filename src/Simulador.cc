#include "Simulador.h"
#include <iomanip>
#include <sstream>
#include <fstream> 
#include <iostream> 
#include "ParPacoteString.h"

using namespace std;


void Simulador::carregarEventos(const std::string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << nomeArquivo << endl;
        return;
    }

    for (string linha; getline(arquivo, linha); ) {
        if (linha.empty()) {
            continue;
        }

        if (linha.find("PC") != string::npos || linha.find("CL") != string::npos) {
            processarConsulta(linha);
            continue;
        }

        if (linha.find("EV") != string::npos) {
            processarEvento(Evento::lerEvento(linha));
        }
    }

    arquivo.close();
}


Simulador::Simulador() {}

Simulador::~Simulador()
{
    auto limparEventos = [](Evento *e)
    { delete e; };
    eventos.emOrdem(limparEventos);
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



// Processa um evento e atualiza as estruturas de dados

void Simulador::processarEvento(const Evento &evento)
{
    
    Pacote* pct = getPacote(evento.idPacote);
    if (!pct) {
        pct = createPacote(evento.idPacote);
    }

    // Armazena o evento na árvore de eventos
    Evento *novoEvento = new Evento(evento);
    eventos.inserir(novoEvento);

    // Atualiza primeiro e último evento do pacote
    if (pct->getPrimeiroEvento() == nullptr)
        pct->setPrimeiroEvento(novoEvento);
    pct->setUltimoEvento(novoEvento);

    // Se for registro geral, vincula remetente e destinatário
    if (evento.tipo == RG)
    {
        // Lógica atualizada para o remetente
        Cliente *remetente = getCliente(evento.remetente);
        if (!remetente) {
            remetente = createCliente(evento.remetente);
        }
        remetente->adicionarPacoteRemetente(evento.idPacote);

        // Lógica atualizada para o destinatário
        Cliente *destinatario = getCliente(evento.destinatario);
        if(!destinatario) {
            destinatario = createCliente(evento.destinatario);
        }
        destinatario->adicionarPacoteDestinatario(evento.idPacote);
    }
}

// Obtém histórico completo de um pacote
ListaEventos Simulador::getHistoricoPacote(int idPacote) const
{
    ListaEventos todosEventos = eventos.getTodosEventos();
    ListaEventos resultado;

    // Filtra eventos pelo ID do pacote
    for (auto it = todosEventos.begin(); it.eValido(); ++it)
        if ((*it).idPacote == idPacote)
            resultado.push_back(*it);

    return resultado;
}

// Função auxiliar para imprimir um evento formatado
void imprimirEvento(Evento *e)
{
    // Formatação padrão do timestamp e tipo
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

    // Campos específicos por tipo de evento
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

// Processa consultas do tipo PC (Pacote) ou CL (Cliente)
void Simulador::processarConsulta(const string &linha)
{
    istringstream iss(linha);
    int timestamp;
    string tipo;

    iss >> timestamp >> tipo;

    // Formata timestamp com zeros à esquerda
    string timestampFormatado = to_string(timestamp);
    while (timestampFormatado.length() < 6)
        timestampFormatado = "0" + timestampFormatado;
    timestampFormatado = timestampFormatado.substr(timestampFormatado.length() - 6);

    cout << timestampFormatado << " " << tipo;

    if (tipo == "PC") // Consulta de Pacote
    {
        int idPacote;
        iss >> idPacote;
        cout << " " << setfill('0') << setw(3) << idPacote << endl;

        ListaEventos historico = getHistoricoPacote(idPacote);
        cout << historico.getTamanho() << endl; // Quantidade de eventos

        // Imprime cada evento do histórico
        for (auto it = historico.begin(); it.eValido(); ++it)
            imprimirEvento(&(*it));
    }
    else if (tipo == "CL") // Consulta de Cliente
    {
        string nomeCliente;
        iss >> nomeCliente;
        cout << " " << nomeCliente << endl;

        Cliente *cliente = clientes.buscar(nomeCliente);
        if (!cliente)
        {
            cout << 0 << endl;
            return;
        }

        // Coleta eventos relevantes (primeiro e último de cada pacote)
        ArvoreEventos eventosRelevantes;

        // Pacotes como remetente
        for (auto it = cliente->getPacotesRemetente().begin(); it.eValido(); ++it)
        {
            int idPacote = *it;
            Pacote *pct = pacotes.buscar(idPacote);
            if (pct)
            {
                if (pct->getPrimeiroEvento())
                    eventosRelevantes.inserir(pct->getPrimeiroEvento());
                if (pct->getUltimoEvento() && pct->getUltimoEvento() != pct->getPrimeiroEvento())
                    eventosRelevantes.inserir(pct->getUltimoEvento());
            }
        }

        // Pacotes como destinatário
        for (auto it = cliente->getPacotesDestinatario().begin(); it.eValido(); ++it)
        {
            int idPacote = *it;
            Pacote *pct = pacotes.buscar(idPacote);
            if (pct)
            {
                if (pct->getPrimeiroEvento())
                    eventosRelevantes.inserir(pct->getPrimeiroEvento());
                if (pct->getUltimoEvento() && pct->getUltimoEvento() != pct->getPrimeiroEvento())
                    eventosRelevantes.inserir(pct->getUltimoEvento());
            }
        }

        cout << eventosRelevantes.tamanho() << endl; // Quantidade de eventos
        eventosRelevantes.emOrdem(imprimirEvento);   // Imprime em ordem
    }
}

// Obtém todos pacotes associados a um cliente (como remetente ou destinatário)
ListaPacotes Simulador::getPacotesCliente(const string &nomeCliente) const
{
    ListaPacotes resultado;
    Cliente *cliente = clientes.buscar(nomeCliente);

    if (cliente)
    {
        // Pacotes como remetente
        for (auto it = cliente->getPacotesRemetente().begin(); it.eValido(); ++it)
        {
            Pacote *pct = pacotes.buscar(*it);
            if (pct)
            {
                ListaEventos eventosPct = getHistoricoPacote(*it);
                if (!eventosPct.estaVazia())
                    resultado.push_back(PacoteEString(pct, "remetente"));
            }
        }

        // Pacotes como destinatário
        for (auto it = cliente->getPacotesDestinatario().begin(); it.eValido(); ++it)
        {
            Pacote *pct = pacotes.buscar(*it);
            if (pct)
            {
                ListaEventos eventosPct = getHistoricoPacote(*it);
                if (!eventosPct.estaVazia())
                    resultado.push_back(PacoteEString(pct, "destinatario"));
            }
        }
    }

    return resultado;
}