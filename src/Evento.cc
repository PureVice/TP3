#include "Evento.h"
#include <sstream>

int gerarChaveEvento(const Evento& ev) {
    // A chave é calculada para evitar colisões, assumindo limites razoáveis
    // para o ID do pacote e o número de tipos de evento.
    return (ev.tempo * 10000) + (ev.idPacote * 10) + static_cast<int>(ev.tipo);
}

// Converte uma string para o enum TipoEvento
TipoEvento Evento::lerTipo(const std::string& tipoStr) {

    if (tipoStr == "AR") return AR;
    if (tipoStr == "EN") return EN;
    if (tipoStr == "RG") return RG;
    if (tipoStr == "RM") return RM;
    if (tipoStr == "UR") return UR;
    if (tipoStr == "TR") return TR;


    exit(1);
}

// Cria um objeto Evento a partir de uma linha de texto formatada.
Evento Evento::lerEvento(const std::string& linha) {
    std::istringstream stream(linha);
    std::string token;

    int timestamp;
    int idPacote;

    stream >> timestamp >> token;
    stream >> token;
    stream >> idPacote;

    TipoEvento tipo = lerTipo(token);

    std::string remetente, destinatario;
    int armazemOrigem = -1;
    int armazemDestino = -1;
    int secaoDestino = -1;

    switch (tipo) {
        case RG:
            stream >> remetente >> destinatario >> armazemOrigem >> armazemDestino;
            break;
        case AR:
            stream >> armazemOrigem >> armazemDestino >> secaoDestino;
            break;
        case RM:
        case UR:
        case TR:
            stream >> armazemOrigem >> armazemDestino;
            break;
        case EN:
            stream >> armazemDestino;
            break;
    }

    return Evento(timestamp, tipo, idPacote, remetente, destinatario, armazemOrigem, armazemDestino, secaoDestino);
}

// Construtor principal
Evento::Evento(
    int p_timestamp,
    TipoEvento p_tipo,
    int p_idPacote,
    const std::string& p_remetente,
    const std::string& p_destinatario,
    int p_armazemOrigem,
    int p_armazemDestino,
    int p_secaoDestino
) {
    this->tempo = p_timestamp;
    this->tipo = p_tipo;
    this->idPacote = p_idPacote;
    this->remetente = p_remetente;
    this->destinatario = p_destinatario;
    this->armazemOrigem = p_armazemOrigem;
    this->armazemDestino = p_armazemDestino;
    this->secaoDestino = p_secaoDestino;
}

//faz uma copia
Evento::Evento(const Evento& ev) {
    this->tempo = ev.tempo;
    this->tipo = ev.tipo;
    this->idPacote = ev.idPacote;
    this->remetente = ev.remetente;
    this->destinatario = ev.destinatario;
    this->armazemOrigem = ev.armazemOrigem;
    this->armazemDestino = ev.armazemDestino;
    this->secaoDestino = ev.secaoDestino;
}
