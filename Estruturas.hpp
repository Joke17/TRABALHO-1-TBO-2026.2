#ifndef ESTRUTURAS_HPP
#define ESTRUTURAS_HPP
#include <vector>
#include <unordered_map> // para o cache de consultas
#include <string>        // para as chaves do cache

using namespace std;


// retorma buscas repetidas 
class CacheConsultas {
private:
    unordered_map<string, vector<int>> historico;
public:
    bool existe(const string& chave_busca) {
        return historico.find(chave_busca) != historico.end();
    }
    vector<int> obter(const string& chave_busca) {
        return historico[chave_busca];
    }
    void guardar(const string& chave_busca, const vector<int>& resultados) {
        historico[chave_busca] = resultados;
    }
};

// modulo 1 uniao e intersecao
class OperacoesConjuntos {
private:
    // funcao auxiliar de busca binaria
    static bool buscaBinaria(const vector<int>& v, int alvo) {
        int ini = 0, fim = v.size() - 1;
        while (ini <= fim) {
            int meio = ini + (fim - ini) / 2;
            if (v[meio] == alvo) return true;
            if (v[meio] < alvo) ini = meio + 1;
            else fim = meio - 1;
        }
        return false;
    }

public:
    static vector<int> intersecao(const vector<int>& v1, const vector<int>& v2) {
        vector<int> resultado;
        if (v1.empty() || v2.empty()) return resultado;

        if (v1.size() * 10 < v2.size()) { // pra cada valor de v1 é feita uma busca binária em v2
            for (int val : v1) {
                if (buscaBinaria(v2, val)) resultado.push_back(val);
            }
            return resultado;
        } 
        else if (v2.size() * 10 < v1.size()) {
            for (int val : v2) {
                if (buscaBinaria(v1, val)) resultado.push_back(val);
            }
            return resultado;
        }

        int i = 0, j = 0;
        while (i < v1.size() && j < v2.size()) {
            if (v1[i] < v2[j]) i++;
            else if (v2[j] < v1[i]) j++;
            else {
                resultado.push_back(v1[i]);
                i++; j++;
            }
        }
        return resultado;
    }
    static vector<int> uniao(const vector<int>& v1, const vector<int>& v2) {
        vector<int> resultado;
        int i = 0, j = 0;
        while (i < v1.size() && j < v2.size()) {
            if (v1[i] < v2[j]) resultado.push_back(v1[i++]);
            else if (v2[j] < v1[i]) resultado.push_back(v2[j++]);
            else {
                resultado.push_back(v1[i]);
                i++; j++;
            }
        }
        while (i < v1.size()) resultado.push_back(v1[i++]);
        while (j < v2.size()) resultado.push_back(v2[j++]);
        return resultado;
    }
};

// modulo 2 arvore binaria de busca para intervalos
class NoArvore {
public:
    double chave; //double pra suportar os precos do modulo 5
    vector<int> indices_filmes; // filmes que possuem essa chave
    NoArvore* esq;
    NoArvore* dir;
    NoArvore(double c, int indice) { //
        chave = c;
        indices_filmes.push_back(indice);
        esq = nullptr;
        dir = nullptr;
    }
};

class ArvoreBusca {
private:
    NoArvore* raiz;
    NoArvore* inserir(NoArvore* no, double chave, int indice) { 
        if (no == nullptr) return new NoArvore(chave, indice);
       
        if (chave < no->chave)
            no->esq = inserir(no->esq, chave, indice);
        else if (chave > no->chave)
            no->dir = inserir(no->dir, chave, indice);
        else
            no->indices_filmes.push_back(indice); // se a chave ja existe so adiciona o filme
           
        return no;
    }
    void buscarIntervalo(NoArvore* no, double min, double max, vector<int>& resultados) { 
        if (no == nullptr) return;
        if (no->chave > min)
            buscarIntervalo(no->esq, min, max, resultados);
        if (no->chave >= min && no->chave <= max) {
            for (int idx : no->indices_filmes) {
                resultados.push_back(idx);
            }
        }
        if (no->chave < max)
            buscarIntervalo(no->dir, min, max, resultados);
    }

    void buscarMenores(NoArvore* no, int& qtd_restante, vector<int>& resultados) {
        if (no == nullptr || qtd_restante <= 0) return;
        buscarMenores(no->esq, qtd_restante, resultados);
        if (qtd_restante > 0) {
            for (int idx : no->indices_filmes) {
                if (qtd_restante > 0) {
                    resultados.push_back(idx);
                    qtd_restante--;
                } else break;
            }
        }
        buscarMenores(no->dir, qtd_restante, resultados);
    }
    void buscarMaiores(NoArvore* no, int& qtd_restante, vector<int>& resultados) {
        if (no == nullptr || qtd_restante <= 0) return;
        buscarMaiores(no->dir, qtd_restante, resultados);
        if (qtd_restante > 0) {
            for (int idx : no->indices_filmes) {
                if (qtd_restante > 0) {
                    resultados.push_back(idx);
                    qtd_restante--;
                } else break;
            }
        }
        buscarMaiores(no->esq, qtd_restante, resultados);
    }

public:
    ArvoreBusca() { raiz = nullptr; }
    void inserir(double chave, int indice) { 
        raiz = inserir(raiz, chave, indice);
    }
    vector<int> buscarIntervalo(double min, double max) { 
        vector<int> resultados;
        buscarIntervalo(raiz, min, max, resultados);
        return resultados;
    }

    // funcoes publicas para o Modulo 5 de exibicao parcial
    vector<int> obterMenores(int quantidade) {
        vector<int> resultados;
        int qtd_restante = quantidade;
        buscarMenores(raiz, qtd_restante, resultados);
        return resultados;
    }

    vector<int> obterMaiores(int quantidade) {
        vector<int> resultados;
        int qtd_restante = quantidade;
        buscarMaiores(raiz, qtd_restante, resultados);
        return resultados;
    }
};
#endif