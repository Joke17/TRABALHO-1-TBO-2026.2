#include<stdio.h>
#include<string>
#include<iostream>
#include<fstream>
#include <chrono>
#include <vector>



class InsertionSort
{
private:
    /* data */
public:
    InsertionSort(/* args */);
    ~InsertionSort();
    static void Sort(Filme * filmes){
        
    }
};

InsertionSort::InsertionSort(/* args */)
{
}

InsertionSort::~InsertionSort()
{
}



using namespace std;

class Filme
{
public:
    std::string identificacao;
    std::string tipo_do_filme;
    std::string titulo_primario;
    std::string titulo_original;
    bool adulto;
    int ano_estreia;
    int ano_fim;
    int duracao;
    std::string genero;
};



int main(int argc, char const *argv[])
{
    // Abre o arquivo para leitura
    ifstream arquivo("filmesCrop.txt");
    
    // Verifica se o arquivo abriu corretamente
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo!" << endl;
        return 1;
    }
    
    string linha;
    int qtdlinha = 0;
    // Registra o tempo inicial
    auto inicio = std::chrono::high_resolution_clock::now();
    
    // Lê o arquivo linha por linha
    while (qtdlinha <= 100000) {
        getline(arquivo, linha);
        cout << linha << endl;
        qtdlinha++;
    }
    
    // Registra o tempo final
    auto fim = std::chrono::high_resolution_clock::now();

    // Calcula a duração subtraindo o início do fim
    std::chrono::duration<double, std::milli> duracao = (fim - inicio) ;

    std::cout << "Tempo de execução: " << duracao.count() << " ms" << std::endl;


    // Fecha o arquivo


    return 0;
}
