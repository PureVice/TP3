#ifndef PACOTE_H
#define PACOTE_H

#include "Evento.h"

// Representa um pacote com identificador e eventos associados.
class Pacote
{
private:
    int id;
    Evento *primeiroEvento;
    Evento *ultimoEvento;

public:
    Pacote(int id);
    int getId() const;

    void setPrimeiroEvento(Evento *ev);
    void setUltimoEvento(Evento *ev);
    Evento *getPrimeiroEvento() const;
    Evento *getUltimoEvento() const;
};

#endif