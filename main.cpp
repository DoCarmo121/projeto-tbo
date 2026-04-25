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
#include <chrono>
#include <unordered_set>
#include <iomanip>
#include <iterator>

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

unordered_set<int> filmesCarregados;

unordered_map<string, int> tconstParaIndice;

void merge(vector<int>& arr, int e, int m, int d) {
    int n1 = m - e + 1;
    int n2 = d - m;

    vector<int> esq(n1), dir(n2);

    for (int i = 0; i < n1; i++) esq[i] = arr[e + i];
    for (int j = 0; j < n2; j++) dir[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = e;

    while (i < n1 && j < n2) {
        if (esq[i] <= dir[j]) {
            arr[k++] = esq[i++];
        } else {
            arr[k++] = dir[j++];
        }
    }

    while (i < n1) arr[k++] = esq[i++];
    while (j < n2) arr[k++] = dir[j++];
}

void mergeSort(vector<int>& arr, int e, int d) {
    if (e < d) {
        int m = e + (d - e) / 2;
        mergeSort(arr, e, m);
        mergeSort(arr, m + 1, d);
        merge(arr, e, m, d);
    }
}

int parseTconst(const string& str) {
    if (str.length() < 3) return -1;
    try {
        return stoi(str.substr(2));
    } catch (const exception& e) {
        return -1;
    }
}

int parseInteger(const string& str) {
    if (str == "\\N" || str.empty()) return -1;
    try {
        return stoi(str);
    } catch (const exception& e) {
        return -1;
    }
}

float parseFloat(const string& str) {
    if (str == "\\N" || str.empty()) return -1.0f;
    try {
        return stof(str);
    } catch (const exception& e) {
        return -1.0f;
    }
}

vector<FilmesCrop> lerArquivoFilmes(int& totalValidos) {
    ifstream file("dados/filmesCrop.txt");
    vector<FilmesCrop> vetorFilmes;
    string linha;

    totalValidos = 0;

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
        int indice = (parseTconst(token) - 7917518) / 2;
        if (indice < 0) continue;

        tconstParaIndice[token] = indice;

        totalValidos++;
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
            stringstream ssGenre(token);
            string genre;
            while (getline(ssGenre, genre, ',')) {
                hash_Genres[genre].push_back(indice);
            }
        }

        if (indice >= (int)vetorFilmes.size()) {
            vetorFilmes.resize(indice + 1);
        }
        vetorFilmes[indice] = filme;
        filmesCarregados.insert(indice);
    }

    for (auto& par : hash_Genres)         { sort(par.second.begin(), par.second.end()); par.second.erase(unique(par.second.begin(), par.second.end()), par.second.end()); }
    for (auto& par : hash_type)           { sort(par.second.begin(), par.second.end()); par.second.erase(unique(par.second.begin(), par.second.end()), par.second.end()); }
    for (auto& par : hash_year)           { sort(par.second.begin(), par.second.end()); par.second.erase(unique(par.second.begin(), par.second.end()), par.second.end()); }
    for (auto& par : hash_RuntimeMinutes) { sort(par.second.begin(), par.second.end()); par.second.erase(unique(par.second.begin(), par.second.end()), par.second.end()); }
    for (auto& par : hash_PrimaryTitle)   { sort(par.second.begin(), par.second.end()); par.second.erase(unique(par.second.begin(), par.second.end()), par.second.end()); }
    for (auto& par : hash_OriginalTitle)  { sort(par.second.begin(), par.second.end()); par.second.erase(unique(par.second.begin(), par.second.end()), par.second.end()); }
    for (auto& par : hash_IsAdult)        { sort(par.second.begin(), par.second.end()); par.second.erase(unique(par.second.begin(), par.second.end()), par.second.end()); }

    file.close();
    return vetorFilmes;
}

vector<Cinemas> lerArquivoCinemas(int& totalCinemas, const vector<FilmesCrop>& vetorFilmes) {
    ifstream file("dados/cinemas(1).txt");
    vector<Cinemas> vetorCinemas;
    string linha;
    totalCinemas = 0;

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
        int indice = parseTconst(token) - 1;
        if (indice < 0) continue;

        totalCinemas++;
        cinema.set_cinemas_id(indice);

        getline(ss, token, ',');
        cinema.set_nome_cinema(token);
        hash_CineName[token].push_back(indice);

        getline(ss, token, ',');
        int x = parseInteger(token);
        cinema.set_coordenada_x(x);
        int xB = x / 1000;

        getline(ss, token, ',');
        int y = parseInteger(token);
        cinema.set_coordenada_y(y);
        int yB = y / 1000;

        long long chave_grid = (xB * 10000LL) + yB;
        hash_Coordenadas[chave_grid].push_back(indice);

        getline(ss, token, ',');
        float price = parseFloat(token);
        cinema.set_preco_ingresso(price);
        hash_TicketPrice[static_cast<int>(round(price))].push_back(indice);

        getline(ss, token);
        vector<int> listaFilmesId;
        if (token != "\\N" && !token.empty()) {
            stringstream ssFilmes(token);
            string idFilmeStr;

            while (getline(ssFilmes, idFilmeStr, ',')) {
                size_t start = idFilmeStr.find_first_not_of(" \r\t");
                size_t end   = idFilmeStr.find_last_not_of(" \r\t");
                if (start == string::npos) continue;
                idFilmeStr = idFilmeStr.substr(start, end - start + 1);

                if (tconstParaIndice.count(idFilmeStr)) {
                    int idFilme = tconstParaIndice[idFilmeStr];
                    listaFilmesId.push_back(idFilme);
                    hash_FilmesExibicao[idFilme].push_back(indice);
                }
            }
        }
        cinema.set_filmes_exibicao(listaFilmesId);

        if (indice >= (int)vetorCinemas.size()) {
            vetorCinemas.resize(indice + 1);
        }
        vetorCinemas[indice] = cinema;
    }

    for (auto& par : hash_FilmesExibicao) { sort(par.second.begin(), par.second.end()); par.second.erase(unique(par.second.begin(), par.second.end()), par.second.end()); }
    for (auto& par : hash_TicketPrice)    { sort(par.second.begin(), par.second.end()); par.second.erase(unique(par.second.begin(), par.second.end()), par.second.end()); }
    for (auto& par : hash_CineName)       { sort(par.second.begin(), par.second.end()); par.second.erase(unique(par.second.begin(), par.second.end()), par.second.end()); }

    file.close();
    return vetorCinemas;
}

vector<int> buscarPorAno(int ano) {
    if (hash_year.count(ano)) return hash_year[ano];
    return {};
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
    return {};
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

vector<int> buscarAtePreco(float precoMaximo, const vector<Cinemas>& vetorCinemas) {
    vector<int> cinemasEncontrados;
    for (const auto& cinema : vetorCinemas) {
        if (cinema.preco_ingresso() >= 0 && cinema.preco_ingresso() <= precoMaximo) {
            cinemasEncontrados.push_back(cinema.cinemas_id());
        }
    }
    // Como você iterou ordenadamente pela ID, o vetor resultante já sairá ordenado,
    // pronto para as suas funções de interseção (AND/OR).
    return cinemasEncontrados;
}

vector<int> buscarCinemasPorDistancia(int userX, int userY, int maxDistancia, const vector<Cinemas>& vetorCinemas) {
    vector<int> cinemasEncontrados;

    const int TAMANHO_BLOCO = 1000;
    int raioBlocos = (maxDistancia / TAMANHO_BLOCO) + 1;
    int centroXB = userX / TAMANHO_BLOCO;
    int centroYB = userY / TAMANHO_BLOCO;
    long long maxDistQuad = static_cast<long long>(maxDistancia) * maxDistancia;

    for (int dx = -raioBlocos; dx <= raioBlocos; ++dx) {
        for (int dy = -raioBlocos; dy <= raioBlocos; ++dy) {
            int lookXB = centroXB + dx;
            int lookYB = centroYB + dy;
            long long lookChave = (lookXB * 10000LL) + lookYB;

            if (hash_Coordenadas.count(lookChave)) {
                for (int idxCinema : hash_Coordenadas[lookChave]) {
                    const Cinemas& c = vetorCinemas[idxCinema];
                    long long diffX = static_cast<long long>(c.coordenada_x()) - userX;
                    long long diffY = static_cast<long long>(c.coordenada_y()) - userY;
                    long long distQuad = (diffX * diffX) + (diffY * diffY);
                    if (distQuad <= maxDistQuad) {
                        cinemasEncontrados.push_back(idxCinema);
                    }
                }
            }
        }
    }

    if (!cinemasEncontrados.empty()) {
        mergeSort(cinemasEncontrados, 0, cinemasEncontrados.size() - 1);
    }

    return cinemasEncontrados;
}

vector<int> buscarCinemasPorTituloFilme(string titulo) {
    vector<int> filmesComTitulo;
    if (hash_PrimaryTitle.count(titulo)) {
        filmesComTitulo = hash_PrimaryTitle[titulo];
    }

    if (hash_OriginalTitle.count(titulo)) {
        vector<int> tituloOriginal = hash_OriginalTitle[titulo];
        vector<int> temp;
        set_union(filmesComTitulo.begin(), filmesComTitulo.end(),
                  tituloOriginal.begin(), tituloOriginal.end(),
                  back_inserter(temp));
        filmesComTitulo = temp;
    }

    return buscarCinemasPorListaFilmes(filmesComTitulo);
}

vector<int> AND_Vectors(const vector<int>& v1, const vector<int>& v2) {
    if (v1.empty() || v2.empty()) return {};
    vector<int> result;
    set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(result));
    return result;
}

vector<int> OR_Vectors(const vector<int>& v1, const vector<int>& v2) {
    if (v1.empty()) return v2;
    if (v2.empty()) return v1;
    vector<int> result;
    set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(result));
    return result;
}

int main() {
    cout << "Iniciando leitura dos arquivos..." << endl;
    auto start = chrono::high_resolution_clock::now();

    int totalFilmes;
    vector<FilmesCrop> filmes = lerArquivoFilmes(totalFilmes);
    cout << "Total de filmes carregados: " << totalFilmes << endl;

    int totalCinemas;
    vector<Cinemas> cinemas = lerArquivoCinemas(totalCinemas, filmes);
    cout << "Total de cinemas carregados: " << totalCinemas << endl;

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Tempo de carregamento: " << duration.count() << " ms\n" << endl;

    int opcaoEntidade;
    do {
        cout << "\n=== MENU PRINCIPAL ===" << endl;
        cout << "O que voce deseja consultar?" << endl;
        cout << "1. Filmes" << endl;
        cout << "2. Cinemas" << endl;
        cout << "0. Sair" << endl;
        cout << "Escolha: ";
        if (!(cin >> opcaoEntidade)) break;

        if (opcaoEntidade == 0) break;

        // Nossa "Area de Trabalho" onde os blocos de consulta vao ficar salvos
        vector<vector<int>> historicoIds;
        vector<string> historicoNomes;
        int acao = -1;

        while (acao != 0) {
            cout << "\n--- AREA DE TRABALHO (" << (opcaoEntidade == 1 ? "FILMES" : "CINEMAS") << ") ---" << endl;
            if (historicoIds.empty()) {
                cout << "Nenhum filtro criado ainda. Comece adicionando blocos base." << endl;
            } else {
                for (size_t i = 0; i < historicoIds.size(); ++i) {
                    cout << "[" << i << "] " << historicoNomes[i] << " -> " << historicoIds[i].size() << " itens encontrados" << endl;
                }
            }

            cout << "\nOPCOES:" << endl;
            cout << "1. Criar novo filtro base" << endl;
            if (historicoIds.size() >= 2) {
                cout << "2. Combinar dois blocos com AND (Intersecao)" << endl;
                cout << "3. Combinar dois blocos com OR (Uniao)" << endl;
            }
            if (!historicoIds.empty()) {
                cout << "4. Ver e Exportar resultados de um bloco" << endl;
            }
            cout << "0. Voltar ao Menu Principal" << endl;
            cout << "Escolha: ";
            cin >> acao;

            if (acao == 1) {
                int opcaoFiltro;
                vector<int> resTemp;
                string nomeTemp;

                if (opcaoEntidade == 1) {
                    cout << "\n1. Tipo | 2. Genero | 3. Duracao | 4. Ano" << endl;
                    cout << "Filtro: "; cin >> opcaoFiltro;

                    if (opcaoFiltro == 1) {
                        string tipo; cout << "Tipo: "; cin >> tipo;
                        resTemp = buscarPorTipo(tipo); nomeTemp = "Tipo:" + tipo;
                    } else if (opcaoFiltro == 2) {
                        string genero; cout << "Genero: "; cin >> genero;
                        resTemp = buscarPorGenero(genero); nomeTemp = "Gen:" + genero;
                    } else if (opcaoFiltro == 3) {
                        int min, max; cout << "Min e Max: "; cin >> min >> max;
                        resTemp = buscarPorIntervaloDuracao(min, max); nomeTemp = "Duracao:" + to_string(min) + "-" + to_string(max);
                    } else if (opcaoFiltro == 4) {
                        int min, max; cout << "Ano Min e Max: "; cin >> min >> max;
                        resTemp = buscarPorIntervaloDeAnos(min, max); nomeTemp = "Ano:" + to_string(min) + "-" + to_string(max);
                    }
                } else if (opcaoEntidade == 2) {
                    cout << "\n1. Tipo Filme | 2. Genero Filme | 3. Duracao Filme | 4. Distancia | 5. Preco Max | 6. Ano Filme | 7. Titulo Filme" << endl;
                    cout << "Filtro: "; cin >> opcaoFiltro;

                    if (opcaoFiltro == 1) {
                        string tipo; cout << "Tipo do filme: "; cin >> tipo;
                        resTemp = buscarCinemasPorListaFilmes(buscarPorTipo(tipo)); nomeTemp = "Filme Tipo:" + tipo;
                    } else if (opcaoFiltro == 2) {
                        string genero; cout << "Genero do filme: "; cin >> genero;
                        resTemp = buscarCinemasPorListaFilmes(buscarPorGenero(genero)); nomeTemp = "Filme Gen:" + genero;
                    } else if (opcaoFiltro == 3) {
                        int min, max; cout << "Min e Max (minutos): "; cin >> min >> max;
                        resTemp = buscarCinemasPorListaFilmes(buscarPorIntervaloDuracao(min, max)); nomeTemp = "Filme Duracao:" + to_string(min) + "-" + to_string(max);
                    } else if (opcaoFiltro == 4) {
                        int x, y, dist; cout << "Coordenadas X, Y e Distancia: "; cin >> x >> y >> dist;
                        resTemp = buscarCinemasPorDistancia(x, y, dist, cinemas); nomeTemp = "Dist<=" + to_string(dist);
                    } else if (opcaoFiltro == 5) {
                        float preco; cout << "Preco maximo: "; cin >> preco;
                        resTemp = buscarAtePreco(preco, cinemas); nomeTemp = "Preco<=" + to_string(preco).substr(0,4);
                    } else if (opcaoFiltro == 6) {
                        int min, max; cout << "Ano Min e Max do filme: "; cin >> min >> max;
                        resTemp = buscarCinemasPorListaFilmes(buscarPorIntervaloDeAnos(min, max)); nomeTemp = "Filme Ano:" + to_string(min) + "-" + to_string(max);
                    } else if (opcaoFiltro == 7) {
                        string titulo; cout << "Titulo exato: "; cin.ignore(); getline(cin, titulo);
                        resTemp = buscarCinemasPorTituloFilme(titulo); nomeTemp = "Titulo:" + titulo;
                    }
                }

                if (!nomeTemp.empty()) {
                    historicoIds.push_back(resTemp);
                    historicoNomes.push_back(nomeTemp);
                }

            } else if ((acao == 2 || acao == 3) && historicoIds.size() >= 2) {
                int id1, id2;
                cout << "ID do primeiro bloco: "; cin >> id1;
                cout << "ID do segundo bloco: "; cin >> id2;

                if (id1 >= 0 && id1 < historicoIds.size() && id2 >= 0 && id2 < historicoIds.size()) {
                    auto startLogic = chrono::high_resolution_clock::now();

                    vector<int> resFinal;
                    string opNome = (acao == 2) ? " AND " : " OR ";
                    if (acao == 2) {
                        resFinal = AND_Vectors(historicoIds[id1], historicoIds[id2]);
                    } else {
                        resFinal = OR_Vectors(historicoIds[id1], historicoIds[id2]);
                    }

                    auto endLogic = chrono::high_resolution_clock::now();
                    cout << "-> Operacao logica executada em: " << chrono::duration_cast<chrono::microseconds>(endLogic - startLogic).count() << " microsegundos.\n";

                    historicoIds.push_back(resFinal);
                    historicoNomes.push_back("(" + historicoNomes[id1] + opNome + historicoNomes[id2] + ")");
                } else {
                    cout << "[!] IDs invalidos!" << endl;
                }
            } else if (acao == 4 && !historicoIds.empty()) {
                int idExport;
                cout << "Qual ID deseja visualizar? "; cin >> idExport;

                if (idExport >= 0 && idExport < (int)historicoIds.size()) {
                    vector<int>& resultado = historicoIds[idExport];
                    int total = (int)resultado.size();

                    cout << "\n================================================================================" << endl;
                    cout << "   RELATORIO DE BUSCA | Total: " << total << " encontrados" << endl;
                    cout << "   Logica: " << historicoNomes[idExport] << endl;
                    cout << "================================================================================" << endl;

                    int limite = min(total, 15);
                    for (int i = 0; i < limite; ++i) {
                        int id = resultado[i];
                        if (opcaoEntidade == 1) { // EXIBIÇÃO DE FILMES
                            cout << setw(2) << setfill('0') << i + 1 << ". "
                                 << left << setw(35) << (filmes[id].get_primary_title().substr(0, 34))
                                 << " | " << filmes[id].get_start_year()
                                 << " | " << setw(10) << filmes[id].get_title_type()
                                 << " | ";
                            // Extract the vector and print its contents separated by commas
                            const vector<string>& genres = filmes[id].get_genres();
                            for (size_t j = 0; j < genres.size(); ++j) {
                                cout << genres[j] << (j < genres.size() - 1 ? ", " : "");
                            }
                            cout << endl;
                        } else { // EXIBIÇÃO DE CINEMAS
                            cout << setw(2) << setfill('0') << i + 1 << ". "
                                 << left << setw(30) << (cinemas[id].nome_cinema().substr(0, 29))
                                 << " | Preco: R$ " << setw(6) << fixed << setprecision(2) << cinemas[id].preco_ingresso()
                                 << " | Local: (" << cinemas[id].coordenada_x() << "," << cinemas[id].coordenada_y() << ")" << endl;
                        }
                    }

                    if (total > 15) {
                        cout << "\n..." << endl;
                        cout << "-> Exibindo 15 de " << total << " registros. Faltam " << total - 15 << " itens." << endl;
                    }
                    cout << "================================================================================\n" << endl;
                }
            }
        }
    } while (opcaoEntidade != 0);

    return 0;
}
