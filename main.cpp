//
// Created by docarmo on 4/20/26.
//

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <limits>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iterator>

using namespace std;

#include "Cinemas.h"
#include "FilmesCrop.h"

template <typename K, typename V>
class SimpleHashMap {
private:
    vector<pair<K, V>> entries;
    vector<vector<size_t>> buckets;

    size_t compute_hash(int key) const { return (size_t)(key >= 0 ? key : -key) % buckets.size(); }
    size_t compute_hash(long long key) const { return (size_t)(key >= 0 ? key : -key) % buckets.size(); }
    size_t compute_hash(bool key) const { return key ? 1 : 0; }
    size_t compute_hash(const string& key) const {
        size_t h = 0;
        for (char c : key) h = h * 31 + (unsigned char)c;
        return h % buckets.size();
    }

public:
    SimpleHashMap(size_t num_buckets = 1000003) : buckets(num_buckets) {}

    V& operator[](const K& key) {
        size_t h = compute_hash(key);
        for (size_t idx : buckets[h]) {
            if (entries[idx].first == key) return entries[idx].second;
        }
        entries.push_back({key, V()});
        buckets[h].push_back(entries.size() - 1);
        return entries.back().second;
    }

    bool count(const K& key) const {
        size_t h = compute_hash(key);
        for (size_t idx : buckets[h]) {
            if (entries[idx].first == key) return true;
        }
        return false;
    }

    auto begin() -> decltype(entries.begin()) { return entries.begin(); }
    auto end() -> decltype(entries.end()) { return entries.end(); }
};

string getNextToken(const string& str, size_t& pos, char delimiter) {
    if (pos >= str.length()) return "";
    size_t end = str.find(delimiter, pos);
    if (end == string::npos) {
        string token = str.substr(pos);
        pos = str.length();
        return token;
    }
    string token = str.substr(pos, end - pos);
    pos = end + 1;
    return token;
}

SimpleHashMap<int, vector<int>> hash_year;
SimpleHashMap<string, vector<int>> hash_type;
SimpleHashMap<string, vector<int>> hash_PrimaryTitle;
SimpleHashMap<string, vector<int>> hash_OriginalTitle;
SimpleHashMap<bool, vector<int>> hash_IsAdult;
SimpleHashMap<int, vector<int>> hash_RuntimeMinutes;
SimpleHashMap<string, vector<int>> hash_Genres;

SimpleHashMap<string, vector<int>> hash_CineName;
SimpleHashMap<int, vector<int>> hash_TicketPrice;
SimpleHashMap<int, vector<int>> hash_FilmesExibicao;
SimpleHashMap<long long, vector<int>> hash_Coordenadas;

vector<pair<int, int>> tconstNumParaIndice;

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
        size_t pos = 0;
        string token;
        FilmesCrop filme;

        token = getNextToken(linha, pos, '\t');
        int indice = (parseTconst(token) - 7917518) / 2;
        if (indice < 0) continue;

        int tconstNum = parseTconst(token);
        tconstNumParaIndice.push_back({tconstNum, indice});

        totalValidos++;
        filme.set_tconst(indice);

        token = getNextToken(linha, pos, '\t');
        filme.set_title_type(token);
        hash_type[token].push_back(indice);

        token = getNextToken(linha, pos, '\t');
        filme.set_primary_title(token);
        hash_PrimaryTitle[token].push_back(indice);

        token = getNextToken(linha, pos, '\t');
        filme.set_original_title(token);
        hash_OriginalTitle[token].push_back(indice);

        token = getNextToken(linha, pos, '\t');
        bool isAdult = (token == "1");
        filme.set_is_adult(isAdult);
        hash_IsAdult[isAdult].push_back(indice);

        token = getNextToken(linha, pos, '\t');
        int year = parseInteger(token);
        filme.set_start_year(year);
        hash_year[year].push_back(indice);

        getNextToken(linha, pos, '\t');

        token = getNextToken(linha, pos, '\t');
        int rTM = parseInteger(token);
        filme.set_runtime_minutes(rTM);
        hash_RuntimeMinutes[rTM].push_back(indice);

        token = linha.substr(pos);
        filme.set_genres(token);
        if (token != "\\N" && !token.empty()) {
            size_t posG = 0;
            while (posG < token.length()) {
                string genre = getNextToken(token, posG, ',');
                hash_Genres[genre].push_back(indice);
            }
        }

        if (indice >= (int)vetorFilmes.size()) {
            vetorFilmes.resize(indice + 1);
        }
        vetorFilmes[indice] = filme;
    }

    for (auto& par : hash_Genres)         { if (!par.second.empty()) mergeSort(par.second, 0, par.second.size() - 1); }
    for (auto& par : hash_type)           { if (!par.second.empty()) mergeSort(par.second, 0, par.second.size() - 1); }
    for (auto& par : hash_year)           { if (!par.second.empty()) mergeSort(par.second, 0, par.second.size() - 1); }
    for (auto& par : hash_RuntimeMinutes) { if (!par.second.empty()) mergeSort(par.second, 0, par.second.size() - 1); }
    for (auto& par : hash_PrimaryTitle)   { if (!par.second.empty()) mergeSort(par.second, 0, par.second.size() - 1); }
    for (auto& par : hash_OriginalTitle)  { if (!par.second.empty()) mergeSort(par.second, 0, par.second.size() - 1); }
    for (auto& par : hash_IsAdult)        { if (!par.second.empty()) mergeSort(par.second, 0, par.second.size() - 1); }

    file.close();

    sort(tconstNumParaIndice.begin(), tconstNumParaIndice.end());

    return vetorFilmes;
}

vector<Cinemas> lerArquivoCinemas(int& totalCinemas) {
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
        size_t pos = 0;
        string token;
        Cinemas cinema;

        token = getNextToken(linha, pos, ',');
        int indice = parseTconst(token) - 1;
        if (indice < 0) continue;

        totalCinemas++;
        cinema.set_cinemas_id(indice);

        token = getNextToken(linha, pos, ',');
        cinema.set_nome_cinema(token);
        hash_CineName[token].push_back(indice);

        token = getNextToken(linha, pos, ',');
        int x = parseInteger(token);
        cinema.set_coordenada_x(x);
        int xB = x / 1000;

        token = getNextToken(linha, pos, ',');
        int y = parseInteger(token);
        cinema.set_coordenada_y(y);
        int yB = y / 1000;

        long long chave_grid = (xB * 10000LL) + yB;
        hash_Coordenadas[chave_grid].push_back(indice);

        token = getNextToken(linha, pos, ',');
        float price = parseFloat(token);
        cinema.set_preco_ingresso(price);
        hash_TicketPrice[static_cast<int>(round(price))].push_back(indice);

        token = linha.substr(pos);
        vector<int> listaFilmesId;
        if (token != "\\N" && !token.empty()) {
            size_t posF = 0;
            while (posF < token.length()) {
                string idFilmeStr = getNextToken(token, posF, ',');
                size_t start = idFilmeStr.find_first_not_of(" \r\t");
                size_t end   = idFilmeStr.find_last_not_of(" \r\t");
                if (start == string::npos) continue;
                idFilmeStr = idFilmeStr.substr(start, end - start + 1);

                int idFilmeNum = parseTconst(idFilmeStr);
                if (idFilmeNum != -1) {
                    auto it = lower_bound(tconstNumParaIndice.begin(), tconstNumParaIndice.end(),
                                          make_pair(idFilmeNum, 0),
                                          [](const pair<int, int>& a, const pair<int, int>& b) {
                                              return a.first < b.first;
                                          });

                    if (it != tconstNumParaIndice.end() && it->first == idFilmeNum) {
                        int idFilme = it->second;
                        listaFilmesId.push_back(idFilme);
                        hash_FilmesExibicao[idFilme].push_back(indice);
                    }
                }
            }
        }
        cinema.set_filmes_exibicao(listaFilmesId);

        if (indice >= (int)vetorCinemas.size()) {
            vetorCinemas.resize(indice + 1);
        }
        vetorCinemas[indice] = cinema;
    }

    for (auto& par : hash_FilmesExibicao) { if (!par.second.empty()) mergeSort(par.second, 0, par.second.size() - 1); }
    for (auto& par : hash_TicketPrice)    { if (!par.second.empty()) mergeSort(par.second, 0, par.second.size() - 1); }
    for (auto& par : hash_CineName)       { if (!par.second.empty()) mergeSort(par.second, 0, par.second.size() - 1); }

    file.close();
    return vetorCinemas;
}

vector<int> buscarPorIntervaloDeAnos(int min, int max) {
    vector<int> filmesEntreAnos;
    for (int t = min; t <= max; ++t) {
        if (hash_year.count(t)) {
            filmesEntreAnos.insert(filmesEntreAnos.end(), hash_year[t].begin(), hash_year[t].end());
        }
    }
    if (!filmesEntreAnos.empty()) {
        mergeSort(filmesEntreAnos, 0, filmesEntreAnos.size() - 1);
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
        if (!cinema.nome_cinema().empty() && cinema.preco_ingresso() >= 0 && cinema.preco_ingresso() <= precoMaximo) {
            cinemasEncontrados.push_back(cinema.cinemas_id());
        }
    }
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

int lerInteiro(const string& prompt) {
    int valor;
    while (true) {
        cout << prompt;
        if (cin >> valor) {
            return valor;
        }
        cout << "[!] Entrada invalida. Por favor, digite um numero valido.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int main() {
    cout << "Iniciando leitura dos arquivos..." << endl;
    auto start = chrono::high_resolution_clock::now();

    int totalFilmes;
    vector<FilmesCrop> filmes = lerArquivoFilmes(totalFilmes);
    cout << "Total de filmes carregados: " << totalFilmes << endl;

    int totalCinemas;
    vector<Cinemas> cinemas = lerArquivoCinemas(totalCinemas);
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
        opcaoEntidade = lerInteiro("Escolha: ");

        if (opcaoEntidade == 0) break;
        if (opcaoEntidade != 1 && opcaoEntidade != 2) {
            cout << "[!] Opcao invalida.\n";
            continue;
        }

        vector<vector<int>> historicoIds;
        vector<string> historicoNomes;
        vector<long long> historicoTempos;
        int acao = -1;

        while (acao != 0) {
            cout << "\n--- AREA DE TRABALHO (" << (opcaoEntidade == 1 ? "FILMES" : "CINEMAS") << ") ---" << endl;
            if (historicoIds.empty()) {
                cout << "Nenhum filtro criado ainda. Comece adicionando blocos base." << endl;
            } else {
                for (size_t i = 0; i < historicoIds.size(); ++i) {
                    cout << "[" << i << "] " << historicoNomes[i] << " -> " << historicoIds[i].size() << " itens encontrados (" << historicoTempos[i] << " us)" << endl;
                }
            }

            cout << "\nOPCOES:" << endl;
            cout << "1. Criar novo filtro base" << endl;
            if (historicoIds.size() >= 2) {
                cout << "2. Combinar dois blocos com AND (Intersecao)" << endl;
                cout << "3. Combinar dois blocos com OR (Uniao)" << endl;
            }
            if (!historicoIds.empty()) {
                cout << "4. Ver e Exportar resultados de um bloco (ENCERRA O PROGRAMA)" << endl;
            }
            cout << "0. Voltar ao Menu Principal" << endl;
            acao = lerInteiro("Escolha: ");

            if (acao == 1) {
                int opcaoFiltro;
                vector<int> resTemp;
                string nomeTemp;

                auto startFiltro = chrono::high_resolution_clock::now();

                if (opcaoEntidade == 1) {
                    cout << "\n1. Tipo | 2. Genero | 3. Duracao | 4. Ano" << endl;
                    opcaoFiltro = lerInteiro("Filtro: ");

                    if (opcaoFiltro == 1) {
                        string tipo; cout << "Tipo: "; cin >> tipo;
                        resTemp = buscarPorTipo(tipo); nomeTemp = "Tipo:" + tipo;
                    } else if (opcaoFiltro == 2) {
                        string genero; cout << "Genero: "; cin >> genero;
                        resTemp = buscarPorGenero(genero); nomeTemp = "Gen:" + genero;
                    } else if (opcaoFiltro == 3) {
                        int min = lerInteiro("Min (minutos): ");
                        int max = lerInteiro("Max (minutos): ");
                        resTemp = buscarPorIntervaloDuracao(min, max); nomeTemp = "Duracao:" + to_string(min) + "-" + to_string(max);
                    } else if (opcaoFiltro == 4) {
                        int min = lerInteiro("Ano Min: ");
                        int max = lerInteiro("Ano Max: ");
                        resTemp = buscarPorIntervaloDeAnos(min, max); nomeTemp = "Ano:" + to_string(min) + "-" + to_string(max);
                    }
                } else if (opcaoEntidade == 2) {
                    cout << "\n1. Tipo Filme | 2. Genero Filme | 3. Duracao Filme | 4. Distancia | 5. Preco Max | 6. Ano Filme | 7. Titulo Filme" << endl;
                    opcaoFiltro = lerInteiro("Filtro: ");

                    if (opcaoFiltro == 1) {
                        string tipo; cout << "Tipo do filme: "; cin >> tipo;
                        resTemp = buscarCinemasPorListaFilmes(buscarPorTipo(tipo)); nomeTemp = "Filme Tipo:" + tipo;
                    } else if (opcaoFiltro == 2) {
                        string genero; cout << "Genero do filme: "; cin >> genero;
                        resTemp = buscarCinemasPorListaFilmes(buscarPorGenero(genero)); nomeTemp = "Filme Gen:" + genero;
                    } else if (opcaoFiltro == 3) {
                        int min = lerInteiro("Min (minutos): ");
                        int max = lerInteiro("Max (minutos): ");
                        resTemp = buscarCinemasPorListaFilmes(buscarPorIntervaloDuracao(min, max)); nomeTemp = "Filme Duracao:" + to_string(min) + "-" + to_string(max);
                    } else if (opcaoFiltro == 4) {
                        int x = lerInteiro("Coordenada X: ");
                        int y = lerInteiro("Coordenada Y: ");
                        int dist = lerInteiro("Distancia Max: ");
                        resTemp = buscarCinemasPorDistancia(x, y, dist, cinemas); nomeTemp = "Dist<=" + to_string(dist);
                    } else if (opcaoFiltro == 5) {
                        float preco;
                        cout << "Preco maximo: ";
                        while (!(cin >> preco)) { cin.clear(); cin.ignore(10000, '\n'); cout << "Invalido. Preco: "; }
                        resTemp = buscarAtePreco(preco, cinemas); nomeTemp = "Preco<=" + to_string(preco).substr(0,4);
                    } else if (opcaoFiltro == 6) {
                        int min = lerInteiro("Ano Min do filme: ");
                        int max = lerInteiro("Ano Max do filme: ");
                        resTemp = buscarCinemasPorListaFilmes(buscarPorIntervaloDeAnos(min, max)); nomeTemp = "Filme Ano:" + to_string(min) + "-" + to_string(max);
                    } else if (opcaoFiltro == 7) {
                        string titulo; cout << "Titulo exato: "; cin.ignore(); getline(cin, titulo);
                        resTemp = buscarCinemasPorTituloFilme(titulo); nomeTemp = "Titulo:" + titulo;
                    }
                }

                auto endFiltro = chrono::high_resolution_clock::now();

                if (!nomeTemp.empty()) {
                    auto tempoBusca = chrono::duration_cast<chrono::microseconds>(endFiltro - startFiltro).count();
                    historicoIds.push_back(resTemp);
                    historicoNomes.push_back(nomeTemp);
                    historicoTempos.push_back(tempoBusca);

                    cout << "-> Busca executada em: " << tempoBusca << " microsegundos.\n";
                }

            } else if ((acao == 2 || acao == 3) && historicoIds.size() >= 2) {
                int id1 = lerInteiro("ID do primeiro bloco: ");
                int id2 = lerInteiro("ID do segundo bloco: ");

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
                    auto tempoLogica = chrono::duration_cast<chrono::microseconds>(endLogic - startLogic).count();
                    cout << "-> Operacao logica executada em: " << tempoLogica << " microsegundos.\n";

                    historicoIds.push_back(resFinal);
                    historicoNomes.push_back("(" + historicoNomes[id1] + opNome + historicoNomes[id2] + ")");
                    historicoTempos.push_back(tempoLogica);
                } else {
                    cout << "[!] IDs invalidos!" << endl;
                }
            } else if (acao == 4 && !historicoIds.empty()) {
                int idExport = lerInteiro("Qual ID deseja visualizar? ");

                if (idExport >= 0 && idExport < (int)historicoIds.size()) {
                    vector<int>& resultado = historicoIds[idExport];
                    int total = (int)resultado.size();

                    cout << "\n[ AVISO: Gerando relatorio e encerrando a aplicacao... ]\n" << endl;

                    cout << "================================================================================" << endl;
                    cout << "   RELATORIO DE BUSCA | Total: " << total << " encontrados" << endl;
                    cout << "   Logica: " << historicoNomes[idExport] << endl;
                    cout << "   Tempo de processamento deste bloco: " << historicoTempos[idExport] << " us" << endl;
                    cout << "================================================================================" << endl;

                    int limite = min(total, 15);
                    for (int i = 0; i < limite; ++i) {
                        int id = resultado[i];

                        cout << setw(2) << setfill('0') << i + 1 << ". " << setfill(' ');

                        if (opcaoEntidade == 1) {
                            cout << left << setw(35) << (filmes[id].get_primary_title().substr(0, 34))
                                 << " | " << filmes[id].get_start_year()
                                 << " | " << setw(10) << filmes[id].get_title_type()
                                 << " | ";

                            const vector<string>& genres = filmes[id].get_genres();
                            for (size_t j = 0; j < genres.size(); ++j) {
                                cout << genres[j] << (j < genres.size() - 1 ? ", " : "");
                            }
                            cout << endl;
                        } else {
                            cout << left << setw(30) << (cinemas[id].nome_cinema().substr(0, 29))
                                 << " | Preco: R$ " << setw(6) << fixed << setprecision(2) << cinemas[id].preco_ingresso()
                                 << " | Local: (" << cinemas[id].coordenada_x() << "," << cinemas[id].coordenada_y() << ")" << endl;
                        }
                    }

                    if (total > 15) {
                        cout << "\n..." << endl;
                        cout << "-> Exibindo 15 de " << total << " registros. Faltam " << total - 15 << " itens." << endl;
                    }
                    cout << "================================================================================\n" << endl;

                    return 0;
                } else {
                    cout << "[!] ID invalido." << endl;
                }
            }
        }
    } while (opcaoEntidade != 0);

    return 0;
}