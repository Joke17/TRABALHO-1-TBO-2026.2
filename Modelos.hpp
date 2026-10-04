#ifndef MODELOS_HPP
#define MODELOS_HPP

#include <string>
#include <vector>

using namespace std;

class Filme {
public:
    int identificacao;
    string tipo_do_filme;
    string titulo_primario;
    string titulo_original;
    bool adulto;
    int ano_estreia;
    int ano_fim;
    int duracao;
    vector<string> genero;
};

class Cinema {
public:
    string ID;
    string nome;
    double coord_X;
    double coord_Y;
    double preco;
    vector<int> filmes_ids; // Agora guarda IDs inteiros em vez de strings
};

#endif