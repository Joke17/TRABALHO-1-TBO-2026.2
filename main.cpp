#include "Buscador.hpp"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <limits>
#include <cstdlib> // funcao auxiliar para limpar a tela

//g++ main.cpp Buscador.cpp -o trabalhotbo.exe
//.\trabalhotbo.exe

using namespace std;

// func aux para limpar o buffer do teclado
void limparBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// funcao auxiliar para congelar a tela
void pausarTela() {
    cout << "\n[ Pressione ENTER para voltar ao menu ]";
    limparBuffer(); 
}

// funcao auxiliar para limpar a tela
void limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int main() {
    Buscador sistema;
    CacheConsultas cache; //inicializa a memoria cache para o modulo 1

    limparTela();
    cout << "carregando a base de dados e indices...\n";
   
    auto ini_read = chrono::high_resolution_clock::now();
   
    sistema.carregarDados("dados/filmesCrop.txt", "dados/cinemas.txt");
   
    auto fim_read = chrono::high_resolution_clock::now();
    cout << "carregamento concluido em: " << fixed << setprecision(2)
         << chrono::duration<double, milli>(fim_read - ini_read).count() << " ms.\n\n";
    pausarTela();

    int opcao = -1;
    while (opcao != 0) {
        limparTela();
        cout << "|                    MENU DE TESTES                    |\n";
        cout << "1. Modulo 1 - Busca Categorica (Por Genero)\n";
        cout << "2. Modulo 1 - Busca Categorica (Por Tipo de Filme)\n";
        cout << "3. Modulo 2 - Busca Numerica em Arvore (Intervalo Duracao)\n";
        cout << "4. Modulos 1 e 2 - Busca Composta (Duracao AND Genero)\n";
        cout << "5. Modulo 4 - Testar Busca Binaria (Inconsistencia de ID)\n";
        cout << "6. Modulo 5 - Consultar Cinemas por Preco Maximo\n";
        cout << "7. Modulo 5 - Exibir os 'X' Filmes Mais Longos\n";
        cout << "8. Modulo 5 - Exibir os 'X' Cinemas Mais Baratos\n";
        cout << "9. Modulo 5 - Busca Composta Livre (Filtro AND / OR)\n";
        cout << "0. Sair do programa\n";
        cout << "+======================================================+\n";
        cout << "Escolha uma opcao: ";
       
        if (!(cin >> opcao)) {
            limparBuffer();
            continue;
        }
       
        limparBuffer(); // limpa o enter
        limparTela();

        if (opcao == 1) {
            string genero;
            cout << "Digite o genero (Ex: Comedy, Drama, Documentary): ";
            getline(cin, genero);
           
            auto start = chrono::high_resolution_clock::now();
            vector<int> resultados = sistema.buscarPorGenero(genero);
            auto end = chrono::high_resolution_clock::now();
           
            sistema.exibirResultados(resultados);
            cout << "\nTempo de resposta: " << fixed << setprecision(0)
                 << chrono::duration<double, std::nano>(end - start).count() << " ns\n";
            pausarTela();
        }
        else if (opcao == 2) {
            string tipo;
            cout << "Digite o tipo (Ex: short, movie, tvEpisode): ";
            getline(cin, tipo);
           
            auto start = chrono::high_resolution_clock::now();
            vector<int> resultados = sistema.buscarPorTipo(tipo);
            auto end = chrono::high_resolution_clock::now();
           
            sistema.exibirResultados(resultados);
            cout << "\nTempo de resposta: " << fixed << setprecision(0)
                 << chrono::duration<double, std::nano>(end - start).count() << " ns\n";
            pausarTela();
        }
        else if (opcao == 3) {
            int min_dur, max_dur;
            cout << "Digite a duracao minima (minutos): ";
            cin >> min_dur;
            cout << "Digite a duracao maxima (minutos): ";
            cin >> max_dur;
            limparBuffer();
           
            auto start = chrono::high_resolution_clock::now();
            vector<int> resultados = sistema.buscarPorDuracao(min_dur, max_dur);
            auto end = chrono::high_resolution_clock::now();
           
            sistema.exibirResultados(resultados);
            cout << "\nTempo de resposta (Arvore): " << fixed << setprecision(2)
                 << chrono::duration<double, milli>(end - start).count() << " ms\n";
            pausarTela();
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
           
            // gerando chave unica para o cache
            string chave = "DUR_" + to_string(min_dur) + "_" + to_string(max_dur) + "_GEN_" + genero;
            
            auto start = chrono::high_resolution_clock::now();
            vector<int> cruzamento;

            // verificacao do cache (O(1))
            if (cache.existe(chave)) {
                cruzamento = cache.obter(chave);
                cout << "\n[!] RESULTADO RECUPERADO DO CACHE EM O(1)!\n";
            } else {
                vector<int> res_duracao = sistema.buscarPorDuracao(min_dur, max_dur);
                vector<int> res_genero = sistema.buscarPorGenero(genero);
                cruzamento = OperacoesConjuntos::intersecao(res_duracao, res_genero);
                cache.guardar(chave, cruzamento); // Salva no cache
            }

            auto end = chrono::high_resolution_clock::now();
           
            sistema.exibirResultados(cruzamento);
            cout << "\nTempo de resposta (Arvore + Conjuntos): " << fixed << setprecision(2)
                 << chrono::duration<double, milli>(end - start).count() << " ms\n";
            pausarTela();
        }
        else if (opcao == 5) {
            int id_teste;
            cout << "digite o ID de um filme para testar: ";
            cin >> id_teste;
            limparBuffer(); // limpa o buffer apos ler o id
            sistema.demonstrarModulo4(id_teste);
            pausarTela();
        }
        
        else if (opcao == 6) {
            double max_preco;
            cout << "Digite o preco maximo desejado (Ex: 12.50): ";
            cin >> max_preco;
            limparBuffer();

            auto start = chrono::high_resolution_clock::now();
            vector<int> resultados = sistema.buscarCinemasPorPrecoMaximo(max_preco);
            auto end = chrono::high_resolution_clock::now();

            sistema.exibirResultadosCinemas(resultados);
            cout << "\nTempo de resposta (Arvore de Precos): " << fixed << setprecision(2)
                 << chrono::duration<double, milli>(end - start).count() << " ms\n";
            pausarTela();
        }
        else if (opcao == 7) {
            int qtd;
            cout << "Quantos filmes deseja listar? (Ex: 5): ";
            cin >> qtd;
            limparBuffer();

            auto start = chrono::high_resolution_clock::now();
            vector<int> resultados = sistema.buscarTopFilmesMaisLongos(qtd);
            auto end = chrono::high_resolution_clock::now();

            sistema.exibirResultados(resultados);
            cout << "\nTempo de resposta: " << fixed << setprecision(2)
                 << chrono::duration<double, milli>(end - start).count() << " ms\n";
            pausarTela();
        }
        else if (opcao == 8) {
            int qtd;
            cout << "Quantos cinemas deseja listar? (Ex: 10): ";
            cin >> qtd;
            limparBuffer();

            auto start = chrono::high_resolution_clock::now();
            vector<int> resultados = sistema.buscarTopCinemasMaisBaratos(qtd);
            auto end = chrono::high_resolution_clock::now();

            sistema.exibirResultadosCinemas(resultados);
            cout << "\nTempo de resposta: " << fixed << setprecision(2)
                 << chrono::duration<double, milli>(end - start).count() << " ms\n";
            pausarTela();
        }
        else if (opcao == 9) {
            string g1, g2;
            int op_logica;
            cout << "Digite o primeiro genero (Ex: Action): ";
            getline(cin, g1);
            cout << "Digite o segundo genero (Ex: Comedy): ";
            getline(cin, g2);
            cout << "Escolha a operacao (1 = AND / Intersecao, 2 = OR / Uniao): ";
            cin >> op_logica;
            limparBuffer();

            auto start = chrono::high_resolution_clock::now();
            vector<int> res1 = sistema.buscarPorGenero(g1);
            vector<int> res2 = sistema.buscarPorGenero(g2);
            vector<int> final_res;

            if (op_logica == 1) final_res = OperacoesConjuntos::intersecao(res1, res2);
            else final_res = OperacoesConjuntos::uniao(res1, res2);

            auto end = chrono::high_resolution_clock::now();

            sistema.exibirResultados(final_res);
            cout << "\nTempo da operacao O(N) c/ Otimizacao Binaria: " << fixed << setprecision(2)
                 << chrono::duration<double, milli>(end - start).count() << " ms\n";
            pausarTela();
        }
        else if (opcao != 0) {
            cout << "opcao invalida!\n";
            pausarTela();
        }
    }
    cout << "\nEncerrando o sistema. Obrigado!\n";
    return 0;
}