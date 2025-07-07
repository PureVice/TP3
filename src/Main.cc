#include "Simulador.h"
#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <arquivo_de_entrada>" << endl;
        return 1;
    }

    try {
        Simulador simulador;
        simulador.carregarEventos(argv[1]);
    } catch (const std::exception& e) {
        cerr << "Erro fatal durante a execucao: " << e.what() << endl;
        return 1;
    }

    return 0;
}