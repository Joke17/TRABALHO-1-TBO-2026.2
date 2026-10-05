#include "Buscador.hpp"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <limits>

//g++ main.cpp Buscador.cpp -o meutrabalho.exe
//.\meutrabalho.exe

using namespace std;

// func aux para limpar o buffer do teclado
void limparBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    Buscador sistema;

    cout << "carregando a base de dados e indices...\n";
   
    auto ini_read = chrono::high_resolution_clock::now();
    
    
    sistema.carregarDados("dados/filmesCrop.txt", "dados/cinemas.txt");
    
    auto fim_read = chrono::high_resolution_clock::now();
    cout << "carregamento concluido em: " << fixed << setprecision(2) 
         << chrono::duration<double, milli>(fim_read - ini_read).count() << " ms.\n\n";

    int opcao = -1;

    while (opcao != 0) {
        cout << "\nMenu de Testes \n";
        cout << "1. Modulo 1 -  Busca Categorica (Por Genero)\n";
        cout << "2. Modulo 1 - Busca Categorica (Por Tipo de Filme)\n";
        cout << "3. Modulo 2 - Busca Numerica em Arvore (Por Intervalo de Duracao)\n";
        cout << "4. Modulos 1 e 2 - Busca Composta (Duracao AND Genero)\n";
        cout << "5. Modulo 4 - Testar Busca Binaria (Inconsistencia de ID)\n";
        cout << "0. Sair do programa\n";
        cout << "Escolha uma opcao: ";
        
        if (!(cin >> opcao)) {
            limparBuffer();
            continue;
        }
        
        limparBuffer(); // limpa o enter 

        if (opcao == 1) {
            string genero;
            cout << "Digite o genero (Ex: Comedy, Drama, Documentary): ";
            getline(cin, genero);
            
            auto start = chrono::high_resolution_clock::now();
            vector<int> resultados = sistema.buscarPorGenero(genero);
            auto end = chrono::high_resolution_clock::now();
            
            sistema.exibirResultados(resultados);
            cout << "Tempo de resposta: " << fixed << setprecision(0) 
                 << chrono::duration<double, std::nano>(end - start).count() << " ns\n";

        } 
        else if (opcao == 2) {
            string tipo;
            cout << "Digite o tipo (Ex: short, movie, tvEpisode): ";
            getline(cin, tipo);
            
            auto start = chrono::high_resolution_clock::now();
            vector<int> resultados = sistema.buscarPorTipo(tipo);
            auto end = chrono::high_resolution_clock::now();
            
            sistema.exibirResultados(resultados);
            cout << "Tempo de resposta: " << fixed << setprecision(0) 
                 << chrono::duration<double, std::nano>(end - start).count() << " ns\n";
        }
        else if (opcao == 3) {
            int min_dur, max_dur;
            cout << "Digite a duracao minima (minutos): ";
            cin >> min_dur;
            cout << "Digite a duracao maxima (minutos): ";
            cin >> max_dur;
            
            auto start = chrono::high_resolution_clock::now();
            vector<int> resultados = sistema.buscarPorDuracao(min_dur, max_dur);
            auto end = chrono::high_resolution_clock::now();
            
            sistema.exibirResultados(resultados);
            cout << "Tempo de resposta (Arvore): " << fixed << setprecision(2) 
                 << chrono::duration<double, milli>(end - start).count() << " ms\n";
        }
        else if (opcao == 4) {
            int min_dur, max_dur;
            string genero;
            cout << "Digite a duracao minima (minutos): ";
            cin >> min_dur;
            cout << "Digite a duracao maxima (minutos): ";
            cin >> max_dur;
            limparBuffer();
            cout << "Digite o genero para cruzar os dados (Ex: Drama): ";
            getline(cin, genero);
            
            auto start = chrono::high_resolution_clock::now();
            vector<int> res_duracao = sistema.buscarPorDuracao(min_dur, max_dur);
            vector<int> res_genero = sistema.buscarPorGenero(genero);
            vector<int> cruzamento = OperacoesConjuntos::intersecao(res_duracao, res_genero);
            auto end = chrono::high_resolution_clock::now();
            
            sistema.exibirResultados(cruzamento);
            cout << "Tempo de resposta (Arvore + Conjuntos): " << fixed << setprecision(2) 
                 << chrono::duration<double, milli>(end - start).count() << " ms\n";
        }
        else if (opcao == 5) {
            int id_teste;
            cout << "digite o ID de um filme para testar: ";
            cin >> id_teste;
            limparBuffer(); // limpa o buffer apos ler o id
            sistema.demonstrarModulo4(id_teste);
        }
        else if (opcao != 0) {
            cout << "opcao invalida!\n";
        }
    }

    cout << "\nEncerrando o sistema. Obrigado!\n";
    return 0;
}