//
// Created by docarmo on 4/20/26.
//

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include<limits>
#include<unordered_map>
#include <cmath>

using namespace std;

#include "Cinemas.h"
#include "FilmesCrop.h"

unordered_map<int, vector<int>> hash_year;
unordered_map<string, vector<int>> hash_type;
unordered_map<string, vector<int>> hash_PrimaryTitle;
unordered_map<string, vector<int>> hash_OriginalTitle;
unordered_map<bool, vector<int>> hash_IsAdult;
unordered_map<int, vector<int>> hash_RuntimeMinutes;
unordered_map<string, vector<int>> hash_Genres;

unordered_map<string, vector<int>> hash_CineName;
unordered_map<int, vector<int>> hash_TicketPrice;
unordered_map<int, vector<int>> hash_FilmesExibicao;
unordered_map<long long, vector<int>> hash_Coordenadas;

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
        int indice = (parseTconst(token)-7917518)/2;
        filme.set_tconst(indice);

        getline(ss, token, '\t');
        filme.set_title_type(token);
        hash_type[token].push_back(indice);

        getline(ss, token, '\t');
        filme.set_primary_title(token);
        hash_PrimaryTitle[token].push_back(indice);

        getline(ss, token, '\t');
        filme.set_original_title(token);
        hash_OriginalTitle[token].push_back(indice);

        getline(ss, token, '\t');
        bool isAdult = (token == "1");
        filme.set_is_adult(isAdult);
        hash_IsAdult[isAdult].push_back(indice);

        getline(ss, token, '\t');
        int year = parseInteger(token);
        filme.set_start_year(year);
        hash_year[year].push_back(indice);

        ss.ignore(numeric_limits<streamsize>::max(), '\t');

        getline(ss, token, '\t');
        int rTM = parseInteger(token);
        filme.set_runtime_minutes(rTM);
        hash_RuntimeMinutes[rTM].push_back(indice);

        getline(ss, token);
        filme.set_genres(token);
        if (token != "\\N" && !token.empty()) {
            stringstream ss(token);
            string genre;

            while (getline(ss, genre, ',')) {
                hash_Genres[genre].push_back(indice);
            }
        }
        if (indice >= vetorFilmes.size()) {
            vetorFilmes.resize(indice + 1);
        }
        vetorFilmes[indice] = filme;

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
        int indice = parseTconst(token)-1;
        cinema.set_cinemas_id(indice);

        getline(ss, token, ',');
        cinema.set_nome_cinema(token);
        hash_CineName[token].push_back(indice);

        getline(ss, token, ',');
        int x = parseInteger(token);
        cinema.set_coordenada_x(x);
        int xB = x/1000;

        getline(ss, token, ',');
        int y = parseInteger(token);
        cinema.set_coordenada_y(y);
        int yB = y/1000;

        long long chave_grid = (xB * 10000LL) + yB;
        hash_Coordenadas[chave_grid].push_back(indice);

        getline(ss, token, ',');
        float price = parseFloat(token);
        cinema.set_preco_ingresso(price);
        //converte para faixa inteira para trabalhar com o Hash
        hash_TicketPrice[static_cast<int>(round(price))].push_back(indice);

        getline(ss, token);
        vector<int> listaFilmesId;
        if (token != "\\N" && !token.empty()) {
            stringstream ssFilmes(token);
            string idFilmeStr;

            while (getline(ssFilmes, idFilmeStr, ',')) {

                size_t start = idFilmeStr.find_first_not_of(" ");
                if (start != string::npos) {
                    idFilmeStr = (idFilmeStr.substr(start));
                }
                int idFilme = (parseTconst(idFilmeStr)-7917518)/2;
                listaFilmesId.push_back(idFilme);
                hash_FilmesExibicao[idFilme].push_back(indice);
            }
        }
        cinema.set_filmes_exibicao(listaFilmesId);

        if (indice >= vetorCinemas.size()) {
            vetorCinemas.resize(indice + 1);
        }
        vetorCinemas[indice] = cinema;
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