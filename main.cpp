//
// Created by docarmo on 4/20/26.
//

#include <string>
#include <vector>
#include <iostream>
using namespace std;
#include <fstream>
#include <sstream>

#include "Cinemas.h"
#include "FilmesCrop.h"


int main() {


}

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
    }catch (exception e) {
        return -1;
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