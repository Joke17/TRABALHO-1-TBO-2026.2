#ifndef BUSCADOR_HPP
#define BUSCADOR_HPP

#include "Modelos.hpp"
#include "Estruturas.hpp"
#include <string>
#include <vector>
#include <map> // usado apenas para mapear a categoria/genero para os vetores de índices

class Buscador {
private:
    vector<Filme> filmes;
    vector<Cinema> cinemas;

    // indices para o modulo 1 (busca categorica)
    map<string, vector<int>> indice_tipos;
    map<string, vector<int>> indice_generos;

    // arvores para o modulo 2 (busca numerica)
    ArvoreBusca arvore_duracao;
    ArvoreBusca arvore_ano;

    // modulo 4: Busca Binaria otimizada
    int buscarSucessorID(int id_procurado);

public:
    Buscador() {}

    void carregarDados(const string& arq_filmes, const string& arq_cinemas);

    // Módulo 1
    vector<int> buscarPorTipo(const string& tipo);
    vector<int> buscarPorGenero(const string& genero);
    
    // Módulo 2
    vector<int> buscarPorDuracao(int min_minutos, int max_minutos);
    vector<int> buscarPorAno(int min_ano, int max_ano);

    // Módulo 4
    void demonstrarModulo4(int id_procurado);

    // Exibição
    void exibirResultados(const vector<int>& indices);
};

#endif