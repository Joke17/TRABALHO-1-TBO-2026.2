#ifndef ESTRUTURAS_HPP
#define ESTRUTURAS_HPP

#include <vector>

using namespace std;


// modulo 1: funcoes de conjuntos (uniao e intersecao)
// ambas assumem que os vetores de indices estão ordenados

class OperacoesConjuntos {
public:
    // retorna elementos que estão em ambos os vetores (AND)
    static vector<int> intersecao(const vector<int>& v1, const vector<int>& v2) {
        vector<int> resultado;
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

    // retorna elementos que estao em qualquer um dos vetores (OR)
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


// modulo 2: arvore binaria de busca para intervalos

class NoArvore {
public:
    int chave; // pode ser a duracao ou o ano
    vector<int> indices_filmes; // filmes que possuem essa chave
    NoArvore* esq;
    NoArvore* dir;

    NoArvore(int c, int indice) {
        chave = c;
        indices_filmes.push_back(indice);
        esq = nullptr;
        dir = nullptr;
    }
};

class ArvoreBusca {
private:
    NoArvore* raiz;

    NoArvore* inserir(NoArvore* no, int chave, int indice) {
        if (no == nullptr) return new NoArvore(chave, indice);
        
        if (chave < no->chave)
            no->esq = inserir(no->esq, chave, indice);
        else if (chave > no->chave)
            no->dir = inserir(no->dir, chave, indice);
        else
            no->indices_filmes.push_back(indice); // se a chave ja existe, so adiciona o filme
            
        return no;
    }

    void buscarIntervalo(NoArvore* no, int min, int max, vector<int>& resultados) {
        if (no == nullptr) return;

        // se a chave atual for maior que o minimo, pode haver elementos a esquerda
        if (no->chave > min)
            buscarIntervalo(no->esq, min, max, resultados);

        // se esta dentro do intervalo, adicionamos
        if (no->chave >= min && no->chave <= max) {
            for (int idx : no->indices_filmes) {
                resultados.push_back(idx);
            }
        }

        // se a chave atual for menor que o maximo, pode haver elementos a direita
        if (no->chave < max)
            buscarIntervalo(no->dir, min, max, resultados);
    }

public:
    ArvoreBusca() { raiz = nullptr; }

    void inserir(int chave, int indice) {
        raiz = inserir(raiz, chave, indice);
    }

    vector<int> buscarIntervalo(int min, int max) {
        vector<int> resultados;
        buscarIntervalo(raiz, min, max, resultados);
        return resultados;
    }
};

#endif