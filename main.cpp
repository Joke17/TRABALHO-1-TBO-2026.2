#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <sstream>
#include <cassert>

// #include<classe.cpp>

using namespace std;


// BuscaBinaria::BuscaBinaria(/* args */)
// {
// }

// BuscaBinaria::~BuscaBinaria()
// {
// }

class Filme
{
public:
    int identificacao;
    std::string tipo_do_filme;
    std::string titulo_primario;
    std::string titulo_original;
    bool adulto;
    int ano_estreia;
    int ano_fim;
    int duracao;
    vector<string> genero;
};
class Cinema
{
public:
    std::string ID;
    std::string nome;
    double coord_X;
    double coord_Y;
    double preco;
    vector<string> filmes;
};

class BuscaBinaria
{
private:
    /* data */
public:

    static int BuscaFilme(const vector<Filme>& filmes, int id_procurado)
    {
        int inicio = 0;
        int fim = filmes.size() - 1;
        int meio = 0;

        while (inicio <= fim)
        {
            meio = inicio + (fim - inicio) / 2;   // <-- recalculado a cada iteração, dentro do loop

            if (filmes[meio].identificacao == id_procurado)
            {
                return meio;
            }
            else if (filmes[meio].identificacao < id_procurado)
            {
                inicio = meio + 1;
            }
            else
            {
                fim = meio - 1;
            }
        }

        return meio;
        // return 0;

    };
};


int main(int argc, char const *argv[])
{
    // assert(0);
    // Abre o arquivo para leitura
    ifstream arq_filmes("filmesCrop.txt");
    ifstream arq_cinemas("cinemas.txt");

    vector<Filme> filmes;
    vector<Cinema> cinemas;

    // Verifica se o arquivo abriu corretamente
    if (!arq_filmes.is_open())
    {
        cout << "Erro ao abrir o arquivo de filmes!" << endl;
        return 1;
    }
    if (!arq_cinemas.is_open())
    {
        cout << "Erro ao abrir o arquivo de cinemas" << endl;
        return 1;
    }

    string linha;
    int qtdlinha = 0, linhaini = 1;
    // Registra o tempo inicial
    auto ini_read_filmes = std::chrono::high_resolution_clock::now();

    // Lê o arquivo de filmes linha por linha
    while (
        // qtdlinha <= 10 &&
        getline(arq_filmes, linha))
    {
        if (linhaini == 1)
        {
            linhaini++;
            qtdlinha--;
            continue;
        }

        stringstream ss(linha); // usa stringstream pra converter dps
        string campo;
        vector<string> campos;

        while (getline(ss, campo, '\t'))
        {
            campos.push_back(campo); // lÊ todos os campos da linha de filme
        }

        if (campos.size() < 9)
            continue;
        // for(int a = 0; a<9;a++){assert(campos[a]);}

        // atriui os trem lido a um filme, e coloca no array
        Filme f;
        f.identificacao = stoi(campos[0].substr(2));
        f.tipo_do_filme = campos[1];
        f.titulo_primario = campos[2];
        f.titulo_original = campos[3];
        f.adulto = (campos[4] == "1");
        // assert(0);
        f.ano_estreia = (campos[5] == "\\N") ? 0 : stoi(campos[5]); // se foe \n vira 0
        f.ano_fim = (campos[6] == "\\N") ? 0 : stoi(campos[6]);
        f.duracao = (campos[7] == "\\N") ? 0 : stoi(campos[7]);

        // f.genero            = campos[8];

        if (campos[8] != "\\N")
        {
            stringstream gens(campos[8]);
            string g;
            while (getline(gens, g, ','))
            {
                f.genero.push_back(g);
            }
        }

        filmes.push_back(f);

        // cout << f.identificacao << " " << f.titulo_original << endl;
        qtdlinha++;
    }
    // Registra o tempo final
    auto fim_read_filmes = std::chrono::high_resolution_clock::now();
    qtdlinha = 0;
    linhaini = 1;
    auto ini_read_cinemas = std::chrono::high_resolution_clock::now();
    // le o arquivo de cinemas linha por linha
    while (getline(arq_cinemas, linha))
    {
        if (linhaini == 1)
        {
            linhaini++;
            qtdlinha--;
            continue;
        }

        stringstream ss(linha);
        string campo;
        vector<string> campos;

        while (getline(ss, campo, ','))
        {
            campos.push_back(campo); // lÊ todos os campos da linha de cinema
        }

        // if (campos.size() < 6) continue;
        Cinema c;
        c.ID = campos[0];
        // assert(0);
        c.nome = campos[1];
        c.coord_X = (campos[2] == "\\N") ? 0 : stoi(campos[2]);
        c.coord_Y = (campos[3] == "\\N") ? 0 : stoi(campos[3]);
        c.preco = (campos[4] == "\\N") ? 0 : stod(campos[4]);
        // assert(0);
        for (int i = 0; campos[i] == "\\N"; i++)
            c.filmes.push_back(campos[i]); // le os filmes exibidos no cinema

        // loop corrigido para add os filmes do cinema
        //  for (size_t i = 5; i < campos.size(); i++)
        //  {
        //      if (campos[i] != "\\N") {
        //          c.filmes.push_back(campos[i]);
        //      }
        //  }

        // c.filmes.push_back(campos[6]);
        // c.filmes.push_back(campos[7]);

        cinemas.push_back(c);

        // std::cout << linha << endl;
        qtdlinha++;
    }
    auto fim_read_cinemas = std::chrono::high_resolution_clock::now();

    // Calcula a duração subtraindo o início do fim
    std::chrono::duration<double, std::milli> duracao_read_filmes = (fim_read_filmes - ini_read_filmes);
    std::chrono::duration<double, std::milli> duracao_read_cinemas = (fim_read_cinemas - ini_read_cinemas);

    cout << "---------------------------------------------------------------------------------------------------------------------------" << endl;
    std::cout << "Leitura filmes: " << duracao_read_filmes.count() << " ms " << "Filmes lidos: " << filmes.size() << std::endl;
    std::cout << "Leitura cinemas: " << duracao_read_cinemas.count() << " ms " << "Cinemas lidos: " << cinemas.size() << std::endl;


    //teste buca binaria
    // fazer casos especiais
    int a = BuscaBinaria::BuscaFilme(filmes, 9194990);
    Filme t;
    if(filmes[a].identificacao < 9194990){ // passar por próximo filme
        t = filmes[a+1];    // se o achado da busca bin for anterior, passa pro prox
    }else{
        t = filmes[a]; // se a busc bin não retornou menor, ou é o certo ou ja é o próximo
    }

    cout << t.identificacao << endl;

    // Fecha o arquivo

    return 0;
}