#ifndef BUSCADOR_HPP
#define BUSCADOR_HPP
#include "Modelos.hpp"
#include "Estruturas.hpp"
#include <string>
#include <vector>
#include <map> // usado apenas para mapear a categoria/genero para os vetores de indices

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
    
    // modulo 5 - arvore para os precos dos cinemas
    ArvoreBusca arvore_preco_cinema;

    // modulo 4 busca binaria otimizada
    int buscarSucessorID(int id_procurado);

public:
    Buscador() {}
    void carregarDados(const string& arq_filmes, const string& arq_cinemas);
    // modulo 1
    vector<int> buscarPorTipo(const string& tipo);
    vector<int> buscarPorGenero(const string& genero);
   
    // modulo 2
    vector<int> buscarPorDuracao(int min_minutos, int max_minutos);
    vector<int> buscarPorAno(int min_ano, int max_ano);

    // modulo 5
    vector<int> buscarCinemasPorPrecoMaximo(double max_preco);
    vector<int> buscarTopFilmesMaisLongos(int quantidade);
    vector<int> buscarTopCinemasMaisBaratos(int quantidade);

    // modulo 4
    void demonstrarModulo4(int id_procurado);
    // exibicao
    void exibirResultados(const vector<int>& indices);
    
    // NOVO: Funcao para exibir resultados especificos de Cinemas
    void exibirResultadosCinemas(const vector<int>& indices);
};
#endif