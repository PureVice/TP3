#ifndef EVENTO_H
#define EVENTO_H
#include <string>

enum TipoEvento { 
    RG, AR, RM, UR, TR, EN 
};

// Representa um evento do sistema de logística.
// Contém informações como tempo, tipo, IDs, remetente e destinatário.
class Evento {
public:
    
    
    int idPacote;
    int armazemOrigem;
    int armazemDestino;
    int secaoDestino;
    int tempo;
    TipoEvento tipo;
    std::string remetente;
    std::string destinatario;
    Evento(const Evento& ev);
    Evento(int tempo, TipoEvento tipo, int idPacote, 
          const std::string& remetente = "", const std::string& destinatario = "",
          int armazemOrigem = -1, int armazemDestino = -1,
          int secaoDestino = -1);
    static TipoEvento lerTipo(const std::string& tipoStr);
    static Evento lerEvento(const std::string& linha);
};

#endif 