#include "Buscador.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;

// modulo 4: regra de inconsistencia de ids (busca binaria O(log n))
// encontra o id exato, ou o codigo maior mais próximo

int Buscador::buscarSucessorID(int id_procurado) {
    if (filmes.empty()) return -1;

    int inicio = 0;
    int fim = filmes.size() - 1;
    int melhor_indice = -1; // guarda o candidato a sucessor

    // Caso especial 1: O id procurado eh maior que o ultimo filme da base
    if (id_procurado > filmes[fim].identificacao) {
        return fim; // Retorna o maior possível
    }

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;

        if (filmes[meio].identificacao == id_procurado) {
            return meio; // encontrou exatamente
        }
        else if (filmes[meio].identificacao > id_procurado) {
            melhor_indice = meio; // eh um candidato a sucessor
            fim = meio - 1;       // tenta achar um sucessor ainda mais próximo (menor)
        }
        else {
            inicio = meio + 1; // o valor atual eh muito pequeno
        }
    }

    return melhor_indice; // retorna o sucessor mais proximo
}

void Buscador::carregarDados(const string& path_filmes, const string& path_cinemas) {
    ifstream arq_filmes(path_filmes);
    ifstream arq_cinemas(path_cinemas);

    if (!arq_filmes.is_open() || !arq_cinemas.is_open()) {
        cout << "Erro ao abrir os arquivos!" << endl;
        return;
    }

    string linha;
    getline(arq_filmes, linha); // pula cabecalho

    int index_filme = 0;
    
    // leitura dos filmess
    while (getline(arq_filmes, linha)) {
        stringstream ss(linha);
        string campo;
        vector<string> campos;
        while (getline(ss, campo, '\t')) campos.push_back(campo);
        if (campos.size() < 9) continue;

        Filme f;
        f.identificacao = stoi(campos[0].substr(2));
        f.tipo_do_filme = campos[1];
        f.titulo_primario = campos[2];
        f.titulo_original = campos[3];
        f.adulto = (campos[4] == "1");
        f.ano_estreia = (campos[5] == "\\N") ? 0 : stoi(campos[5]);
        f.ano_fim = (campos[6] == "\\N") ? 0 : stoi(campos[6]);
        f.duracao = (campos[7] == "\\N") ? 0 : stoi(campos[7]);

        if (campos[8] != "\\N") {
            stringstream gens(campos[8]);
            string g;
            while (getline(gens, g, ',')) {
                f.genero.push_back(g);
                // Indexação para Módulo 1 (Categorias)
                indice_generos[g].push_back(index_filme); 
            }
        }
        
        // indexacao para modulo 1 (tipos)
        indice_tipos[f.tipo_do_filme].push_back(index_filme);

        // indexação em arvore para modulo 2 (intervalos)
        if (f.duracao > 0) arvore_duracao.inserir(f.duracao, index_filme);
        if (f.ano_estreia > 0) arvore_ano.inserir(f.ano_estreia, index_filme);

        filmes.push_back(f);
        index_filme++;
    }

    // leitura dos cineminhas
    getline(arq_cinemas, linha); // pula cabecalho
    while (getline(arq_cinemas, linha)) {
        stringstream ss(linha);
        string campo;
        vector<string> campos;
        while (getline(ss, campo, ',')) {
            // remove espacos extras que vem apos a virgula
            if(!campo.empty() && campo[0] == ' ') campo = campo.substr(1);
            campos.push_back(campo);
        }
        
        if (campos.size() < 6) continue;

        Cinema c;
        c.ID = campos[0];
        c.nome = campos[1];
        c.coord_X = (campos[2] == "\\N") ? 0 : stod(campos[2]);
        c.coord_Y = (campos[3] == "\\N") ? 0 : stod(campos[3]);
        c.preco = (campos[4] == "\\N") ? 0 : stod(campos[4]);

        for (size_t i = 5; i < campos.size(); i++) {
            if (campos[i] != "\\N" && campos[i].length() > 2) {
                int id_filme_procurado = stoi(campos[i].substr(2));
                
                // modelo 4 aplicado aqui, resolve inconsistencias na hora do load
                int index_corrigido = buscarSucessorID(id_filme_procurado);
                if (index_corrigido != -1) {
                    c.filmes_ids.push_back(filmes[index_corrigido].identificacao);
                }
            }
        }
        cinemas.push_back(c);
    }
}

// implementacoes do modulo 1
vector<int> Buscador::buscarPorTipo(const string& tipo) {
    if (indice_tipos.find(tipo) != indice_tipos.end()) return indice_tipos[tipo];
    return {};
}

vector<int> Buscador::buscarPorGenero(const string& genero) {
    if (indice_generos.find(genero) != indice_generos.end()) return indice_generos[genero];
    return {};
}

// implementacoes do modulo 2
vector<int> Buscador::buscarPorDuracao(int min_minutos, int max_minutos) {
    return arvore_duracao.buscarIntervalo(min_minutos, max_minutos);
}

vector<int> Buscador::buscarPorAno(int min_ano, int max_ano) {
    return arvore_ano.buscarIntervalo(min_ano, max_ano);
}

// funcao auxiliar de exibicao
void Buscador::exibirResultados(const vector<int>& indices) {
    cout << "Encontrados " << indices.size() << " filmes." << endl;
    int limit = min((int)indices.size(), 10); // mostra no maximo 10 ou N pra nao floodar a tela
    for (int i = 0; i < limit; i++) {
        Filme& f = filmes[indices[i]];
        cout << " - [" << f.identificacao << "] " << f.titulo_original 
             << " (" << f.ano_estreia << ") - " << f.duracao << " min" << endl;
    }
    if (indices.size() > 5) cout << "   ... e mais " << (indices.size() - 5) << " filmes." << endl;
}


// teste modulo 4

void Buscador::demonstrarModulo4(int id_procurado) {
    cout << "\n[MODULO 4] Testando Regra de Inconsistencia para o ID: " << id_procurado << endl;
    
    auto start = chrono::high_resolution_clock::now();
    int index = buscarSucessorID(id_procurado);
    auto end = chrono::high_resolution_clock::now();
    
    if (index != -1) {
        Filme& f = filmes[index];
        cout << "-> ID associado pelo sistema: [" << f.identificacao << "] " 
             << f.titulo_original << endl;
        
        if (f.identificacao == id_procurado) {
            cout << "-> Conclusao: O ID exato existia na base." << endl;
        } else {
            cout << "-> Conclusao: ID inexistente. O sistema associou ao sucessor mais proximo!" << endl;
        }
    } else {
        cout << "Nenhum sucessor encontrado na base." << endl;
    }
    
    cout << "Tempo M4 (Busca Binaria O(log n)): " 
         << chrono::duration<double, milli>(end - start).count() << " ms\n" << endl;
}