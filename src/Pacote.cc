
#include "Pacote.h"

Pacote::Pacote(int id) : id(id), primeiroEvento(nullptr), ultimoEvento(nullptr) {} // Inicializa ponteiros

int Pacote::getId() const { return id; }


void Pacote::setPrimeiroEvento(Evento* ev) { this->primeiroEvento = ev; }
void Pacote::setUltimoEvento(Evento* ev) { this->ultimoEvento = ev; }
Evento* Pacote::getPrimeiroEvento() const { return this->primeiroEvento; }
Evento* Pacote::getUltimoEvento() const { return this->ultimoEvento; }