//
// Created by docarmo on 4/20/26.
//

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include<limits>

using namespace std;

#include "Cinemas.h"
#include "FilmesCrop.h"

int parseTconst(const string& str) {
    try {
        return stoi(str.substr(2));
    } catch (exception e) {
        return -1;
    }
}

int parseInteger(const string& str) {
    if (str == "\\N" || str.empty()) return -1;
    try {
        return stoi(str);
    }catch (exception& e) {
        return -1;
    }
}

float parseFloat(const string& str) {
    if (str == "\\N" || str.empty()) return -1.0f;
    try {
        return stof(str);
    } catch (exception& e) {
        return -1.0f;
    }
}

vector<FilmesCrop> lerArquivoFilmes() {
    ifstream file ("dados/filmesCrop.txt");
    vector<FilmesCrop> vetorFilmes;
    string linha;

    if (!file.is_open()) {
        cout << "Erro ao abrir o arquivo" << endl;
        return vetorFilmes;
    }

    getline(file, linha);

    vetorFilmes.reserve(1000000);

    while (getline(file, linha)) {
        stringstream ss(linha);
        string token;
        FilmesCrop filme;

        getline(ss, token, '\t');
        filme.set_tconst(parseTconst(token));

        getline(ss, token, '\t');
        filme.set_title_type(token);

        getline(ss, token, '\t');
        filme.set_primary_title(token);

        getline(ss, token, '\t');
        filme.set_original_title(token);

        getline(ss, token, '\t');
        filme.set_is_adult(token == "1");

        getline(ss, token, '\t');
        filme.set_start_year(parseInteger(token));

        ss.ignore(numeric_limits<streamsize>::max(), '\t');

        getline(ss, token, '\t');
        filme.set_runtime_minutes(parseInteger(token));

        getline(ss, token);
        filme.set_genres(token);

        vetorFilmes.push_back(filme);

    }

    file.close();

    return vetorFilmes;
}

vector<Cinemas> lerArquivoCinemas() {
    ifstream file ("dados/cinemas(1).txt");
    vector<Cinemas> vetorCinemas;
    string linha;

    if (!file.is_open()) {
        cout << "Erro ao abrir o arquivo" << endl;
        return vetorCinemas;
    }

    getline(file, linha);

    vetorCinemas.reserve(1000000);

    while (getline(file, linha)) {
        stringstream ss(linha);
        string token;
        Cinemas cinema;

        getline(ss, token, ',');
        cinema.set_cinemas_id(parseTconst(token));

        getline(ss, token, ',');
        cinema.set_nome_cinema(token);

        getline(ss, token, ',');
        cinema.set_coordenada_x(parseInteger(token));

        getline(ss, token, ',');
        cinema.set_coordenada_y(parseInteger(token));

        getline(ss, token, ',');
        cinema.set_preco_ingresso(parseFloat(token));

        getline(ss, token);
        vector<int> listaFilmesId;
        if (token != "\\N" && !token.empty()) {
            stringstream ssFilmes(token);
            string idFilmeStr;

            while (getline(ssFilmes, idFilmeStr, ',')) {

                size_t start = idFilmeStr.find_first_not_of(" ");
                if (start != string::npos) {
                    idFilmeStr = idFilmeStr.substr(start);
                }
                listaFilmesId.push_back(parseTconst(idFilmeStr));
            }
        }

        cinema.set_filmes_exibicao(listaFilmesId);

        vetorCinemas.push_back(cinema);

    }

    file.close();

    return vetorCinemas;
}

int main() {
    cout << "Iniciando leitura dos arquivos..." << endl;

    vector<FilmesCrop> filmes = lerArquivoFilmes();
    cout << "Total de filmes carregados: " << filmes.size() << endl;

    vector<Cinemas> cinemas = lerArquivoCinemas();
    cout << "Total de cinemas carregados: " << cinemas.size() << endl;

    return 0;
}