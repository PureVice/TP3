#ifndef PACOTE_STRING_H
#define PACOTE_STRING_H

#include "Pacote.h"
#include <string>

// Estrutura que associa um ponteiro para Pacote a uma string representando o tipo.
struct PacoteEString {
    Pacote* pacote;
    std::string tipo;

    
    PacoteEString(Pacote* p, const std::string& t) : pacote(p), tipo(t) {}
};
#endif