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
#include <algorithm>

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

void merge(vector<int>& indices, const vector<long long>& distancias, int e, int m, int d) {
    int n1 = m - e + 1;
    int n2 = d - m;

    vector<int> esq(n1), dir(n2);

    for (int i = 0; i < n1; i++) esq[i] = indices[e + i];
    for (int j = 0; j < n2; j++) dir[j] = indices[m + 1 + j];

    int i = 0, j = 0, k = e;

    while (i < n1 && j < n2) {
        // A lógica de ordenação baseada nos valores do vetor 'distancias'
        if (distancias[esq[i]] <= distancias[dir[j]]) {
            indices[k] = esq[i];
            i++;
        } else {
            indices[k] = dir[j];
            j++;
        }
        k++;
    }

    while (i < n1) indices[k++] = esq[i++];
    while (j < n2) indices[k++] = dir[j++];
}

void mergeSortIndices(vector<int>& indices, const vector<long long>& distancias, int e, int d) {
    if (e < d) {
        int m = e + (d - e) / 2;

        mergeSortIndices(indices, distancias, e, m);
        mergeSortIndices(indices, distancias, m + 1, d);

        merge(indices, distancias, e, m, d);
    }
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

vector<int> buscarPorAno(int ano) {
    if (hash_year.count(ano)) return hash_year[ano];
    return{};
}

vector<int> buscarPorIntervaloDeAnos(int min, int max) {
    vector<int> filmesEntreAnos;
    for (int t = min; t <= max; ++t) {
        if (hash_year.count(t)) {
            vector<int> resultadoLocal = hash_year[t];
            vector<int> temp;
            set_union(filmesEntreAnos.begin(), filmesEntreAnos.end(),
                            resultadoLocal.begin(), resultadoLocal.end(),
                            back_inserter(temp));
            filmesEntreAnos = temp;
        }
    }
    return filmesEntreAnos;
}

vector<int> buscarPorGenero(string genre) {
    if (hash_Genres.count(genre)) return hash_Genres[genre];
    return{};
}

vector<int> buscarPorDuracao(int duracao) {
    if (hash_RuntimeMinutes.count(duracao)) return hash_RuntimeMinutes[duracao];
    return {};
}

vector<int> buscarPorIntervaloDuracao(int min, int max) {
    vector<int> filmesNoTempo;
    for (int t = min; t <= max; ++t) {
        if (hash_RuntimeMinutes.count(t)) {
            vector<int> resultadoLocal = hash_RuntimeMinutes[t];
            vector<int> temp;
            set_union(filmesNoTempo.begin(), filmesNoTempo.end(),
                           resultadoLocal.begin(), resultadoLocal.end(),
                           back_inserter(temp));
            filmesNoTempo = temp;
        }
    }
    return filmesNoTempo;
}

vector<int> buscarPorTipo(string tipo) {
    if (hash_type.count(tipo)) return hash_type[tipo];
    return {};
}

vector<int> buscarCinemasPorListaFilmes(const vector<int>& filmesIds) {
    vector<int> cinemasEncontrados;
    for (int idFilme : filmesIds) {
        if (hash_FilmesExibicao.count(idFilme)) {
            vector<int> cinesDoFilme = hash_FilmesExibicao[idFilme];
            vector<int> temp;
            set_union(cinemasEncontrados.begin(), cinemasEncontrados.end(),
                           cinesDoFilme.begin(), cinesDoFilme.end(),
                           back_inserter(temp));
            cinemasEncontrados = temp;
        }
    }
    return cinemasEncontrados;
}

vector<int> buscarAtePreco(int preco) {
    vector<int> cinemasEncontrados;
    for (int i = 0; i <= preco; ++i) {
        if (hash_TicketPrice.count(i)) {
            vector<int> resultadoLocal = hash_TicketPrice[i];
            vector<int> temp;
            set_union(cinemasEncontrados.begin(), cinemasEncontrados.end(),
                                resultadoLocal.begin(), resultadoLocal.end(),
                                back_inserter(temp));
            cinemasEncontrados = temp;
        }
    }
    return cinemasEncontrados;
}

vector<int> buscarCinemasPorDistancia(int userX, int userY, int maxDistancia, const vector<Cinemas>& vetorCinemas) {
    vector<int> cinemasEncontrados;

    // Criamos um vetor paralelo para armazenar a distância de cada cinema encontrado.
    // Ele tem o tamanho total do vetorCinemas para que possamos usar o idxCinema direto como chave.
    vector<long long> distanciasCinemas(vetorCinemas.size(), 0);

    const int TAMANHO_BLOCO = 1000; // Deve ser EXATAMENTE o mesmo divisor usado na leitura do arquivo

    // Calcula quantos quadrantes precisamos "andar" para cobrir a distância
    int raioBlocos = (maxDistancia / TAMANHO_BLOCO) + 1;

    // Descobre o quadrante onde o usuário está pisando
    int centroXB = userX / TAMANHO_BLOCO;
    int centroYB = userY / TAMANHO_BLOCO;

    // Eleva o limite de distância ao quadrado uma única vez (usando cast para evitar overflow)
    long long maxDistQuad = static_cast<long long>(maxDistancia) * maxDistancia;

    // ====================================================================
    // FASE 1: BROAD PHASE (Filtro Grosso via Grid Hashing)
    // ====================================================================
    for (int dx = -raioBlocos; dx <= raioBlocos; ++dx) {
        for (int dy = -raioBlocos; dy <= raioBlocos; ++dy) {

            int lookXB = centroXB + dx;
            int lookYB = centroYB + dy;

            // Recria a chave única exatamente como no momento da indexação
            long long lookChave = (lookXB * 10000LL) + lookYB;

            // Se esse quadrante existir na nossa Tabela Hash, vamos bisbilhotar os cinemas dele
            if (hash_Coordenadas.count(lookChave)) {

                // ====================================================================
                // FASE 2: NARROW PHASE (Filtro Fino via Pitágoras)
                // ====================================================================
                for (int idxCinema : hash_Coordenadas[lookChave]) {
                    const Cinemas& c = vetorCinemas[idxCinema]; // Acesso O(1) ao objeto real

                    // Diferença matemática exata entre os pontos (usando long long para segurança)
                    long long diffX = static_cast<long long>(c.coordenada_x()) - userX;
                    long long diffY = static_cast<long long>(c.coordenada_y()) - userY;

                    // Pitágoras (a^2 + b^2 = c^2) - Ignoramos a raiz quadrada!
                    long long distQuad = (diffX * diffX) + (diffY * diffY);

                    // Se a distância ao quadrado for menor ou igual ao limite ao quadrado, está dentro do raio!
                    if (distQuad <= maxDistQuad) {
                        cinemasEncontrados.push_back(idxCinema);
                        distanciasCinemas[idxCinema] = distQuad;
                    }
                }
            }
        }
    }
    // Após encontrar todos os cinemas, usamos o seu Merge Sort para ordená-los!
    if (!cinemasEncontrados.empty()) {
        mergeSortIndices(cinemasEncontrados, distanciasCinemas, 0, cinemasEncontrados.size() - 1);
    }

    return cinemasEncontrados;
}

vector<int> buscarCinemasPorTituloFilme(string titulo) {
    vector<int> filmesComTitulo;
    if (hash_PrimaryTitle.count(titulo)) {
        filmesComTitulo = hash_PrimaryTitle[titulo];
    }

    // Pega os IDs pelo título original e faz a União (caso existam filmes diferentes com o mesmo título em colunas diferentes)
    if (hash_OriginalTitle.count(titulo)) {
        vector<int> tituloOriginal = hash_OriginalTitle[titulo];
        vector<int> temp;
        set_union(filmesComTitulo.begin(), filmesComTitulo.end(),
                  tituloOriginal.begin(), tituloOriginal.end(),
                  back_inserter(temp));
        filmesComTitulo = temp;
    }

    // Passa a lista de filmes encontrada para o buscador de cinemas
    return buscarCinemasPorListaFilmes(filmesComTitulo);
}

int main() {
    cout << "Iniciando leitura dos arquivos..." << endl;

    vector<FilmesCrop> filmes = lerArquivoFilmes();
    cout << "Total de filmes carregados: " << filmes.size() << endl;

    vector<Cinemas> cinemas = lerArquivoCinemas();
    cout << "Total de cinemas carregados: " << cinemas.size() << endl;

    return 0;
}