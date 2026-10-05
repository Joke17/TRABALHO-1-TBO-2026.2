#include "Buscador.hpp"
#include <iostream>
#include <chrono>

using namespace std;

int main() {
    Buscador sistema;

    cout << "Carregando dados e construindo indices (Arvores e Pre-processamento)..." << endl;
    auto ini_read = chrono::high_resolution_clock::now();
    
    sistema.carregarDados("dados/filmesCrop.txt", "dados/cinemas.txt");
    
    auto fim_read = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duracao = (fim_read - ini_read);
    cout << "Dados carregados em: " << duracao.count() << " ms." << endl;
    cout << "--------------------------------------------------------" << endl;

    // teste modulo 1: categorico simples
    cout << "\n[MODULO 1] Buscando filmes do tipo 'short':" << endl;
    auto start_q1 = chrono::high_resolution_clock::now();
    vector<int> shorts = sistema.buscarPorTipo("short");
    auto end_q1 = chrono::high_resolution_clock::now();
    sistema.exibirResultados(shorts);
    cout << "Tempo M1: " << chrono::duration<double, milli>(end_q1 - start_q1).count() << " ms" << endl;

    // teste modulo 1: busca Composta (and)
    cout << "\n[MODULO 1] Buscando filmes do tipo 'short' AND genero 'Comedy':" << endl;
    vector<int> comedias = sistema.buscarPorGenero("Comedy");
    vector<int> short_comedies = OperacoesConjuntos::intersecao(shorts, comedias);
    sistema.exibirResultados(short_comedies);

    // teste modulo 2: intervalo numerico em arvore
    cout << "\n[MODULO 2] Buscando filmes entre 120 e 150 minutos:" << endl;
    auto start_q2 = chrono::high_resolution_clock::now();
    vector<int> duracao_longa = sistema.buscarPorDuracao(120, 150);
    auto end_q2 = chrono::high_resolution_clock::now();
    sistema.exibirResultados(duracao_longa);
    cout << "Tempo M2: " << chrono::duration<double, milli>(end_q2 - start_q2).count() << " ms" << endl;

    // teste modulo1 + modulo 2: intervalo + categorico
    cout << "\n[MODULOS 1 e 2 COMBINADOS] Filmes (120-150 min) AND do genero 'Drama':" << endl;
    vector<int> dramas = sistema.buscarPorGenero("Drama");
    // intersecao exige vetores ordenados. para garantir:
    vector<int> combinados = OperacoesConjuntos::intersecao(duracao_longa, dramas);
    sistema.exibirResultados(combinados);

   // teste modulo 4: busca binaria com sucessor
    // passando um ID que pode ou nao existir para ver o sistema corrigir
    sistema.demonstrarModulo4(9194990); 

    // teste com um id que nao existe na base para ver a correcao automatica
    // O ID 7917519 não existe (a base salta do 7917518 direto para o 7917520).
    // O sistema deve associar automaticamente ao 7917520.
    sistema.demonstrarModulo4(7917519); 

    // um id gigantesco que eh maior do que qualquer filme na base.
    // o sistema deve associar ao ultimo filme disponivel.
    sistema.demonstrarModulo4(99999999);

    cout << "Fim dos testes." << endl;

    return 0;
}