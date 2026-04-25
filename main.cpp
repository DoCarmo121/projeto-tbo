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
        cout << "\n=== MENU DE BUSCA ===" << endl;
        cout << "1. Filtrar Filmes" << endl;
        cout << "2. Filtrar Cinemas" << endl;
        cout << "0. Sair" << endl;
        cout << "Escolha: ";
        cin >> opcaoEntidade;

        if (opcaoEntidade == 0) break;

        vector<int> resultadoAtual;
        bool primeiraBusca = true;
        int continuar = 1;

        while (continuar != 0) {
            int operadorLogico = 1;

            if (!primeiraBusca) {
                cout << "\nComo deseja combinar o proximo filtro?" << endl;
                cout << "1. E (AND - Intersecao)" << endl;
                cout << "2. OU (OR - Uniao)" << endl;
                cout << "0. Finalizar busca e mostrar resultados" << endl;
                cout << "Escolha: ";
                cin >> operadorLogico;

                if (operadorLogico == 0) break;
                operadorLogico++;
            }

            vector<int> resultadoTemporario;
            int opcaoFiltro;

            if (opcaoEntidade == 1) {
                cout << "\n--- Filtros de Filme ---" << endl;
                cout << "1. Por Tipo (Ex: movie, short)" << endl;
                cout << "2. Por Genero" << endl;
                cout << "3. Por Duracao (min e max)" << endl;
                cout << "4. Por Ano (min e max, ou iguais para ano especifico)" << endl;
                cout << "Escolha o filtro: ";

                while (!(cin >> opcaoFiltro)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "[ERRO] Entrada invalida! Digite apenas o NUMERO da opcao: ";
                }

                if (opcaoFiltro == 1) {
                    string tipo;
                    cout << "Digite o tipo: ";
                    cin >> tipo;
                    resultadoTemporario = buscarPorTipo(tipo);
                } else if (opcaoFiltro == 2) {
                    string genero;
                    cout << "Digite o genero: ";
                    cin >> genero;
                    resultadoTemporario = buscarPorGenero(genero);
                } else if (opcaoFiltro == 3) {
                    int min, max;
                    cout << "Minimo de minutos: "; cin >> min;
                    cout << "Maximo de minutos: "; cin >> max;
                    resultadoTemporario = buscarPorIntervaloDuracao(min, max);
                } else if (opcaoFiltro == 4) {
                    int min, max;
                    cout << "Ano inicial: "; cin >> min;
                    cout << "Ano final: "; cin >> max;
                    resultadoTemporario = buscarPorIntervaloDeAnos(min, max);
                }

            } else if (opcaoEntidade == 2) {
                cout << "\n--- Filtros de Cinema ---" << endl;
                cout << "1. Por Tipo de Filme em exibicao" << endl;
                cout << "2. Por Genero de Filme" << endl;
                cout << "3. Por Intervalo de Duracao do Filme" << endl;
                cout << "4. Por Distancia (X, Y, MaxDist)" << endl;
                cout << "5. Por Preco Maximo" << endl;
                cout << "6. Por Intervalo de Ano do Filme" << endl;
                cout << "7. Por Titulo Especifico de Filme" << endl;
                cout << "Escolha o filtro: ";

                while (!(cin >> opcaoFiltro)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "[ERRO] Entrada invalida! Digite apenas o NUMERO do filtro: ";
                }

                if (opcaoFiltro == 1) {
                    string tipo;
                    cout << "Digite o tipo: "; cin >> tipo;
                    resultadoTemporario = buscarCinemasPorListaFilmes(buscarPorTipo(tipo));
                } else if (opcaoFiltro == 2) {
                    string genero;
                    cout << "Digite o genero: "; cin >> genero;
                    resultadoTemporario = buscarCinemasPorListaFilmes(buscarPorGenero(genero));
                } else if (opcaoFiltro == 3) {
                    int min, max;
                    cout << "Minimo e Maximo: "; cin >> min >> max;
                    resultadoTemporario = buscarCinemasPorListaFilmes(buscarPorIntervaloDuracao(min, max));
                } else if (opcaoFiltro == 4) {
                    int x, y, dist;
                    cout << "Coordenada X, Y e Distancia Maxima (separe por espacos): ";
                    cin >> x >> y >> dist;
                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Erro na digitacao! Tente novamente sem usar virgulas." << endl;
                    } else {
                        resultadoTemporario = buscarCinemasPorDistancia(x, y, dist, cinemas);
                    }
                } else if (opcaoFiltro == 5) {
                    float precoInput;
                    cout << "Preco maximo: ";
                    cin >> precoInput;
                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Entrada invalida! Usando preco = 0 por seguranca." << endl;
                        precoInput = 0;
                    }
                    // AS DUAS LINHAS ABAIXO FORAM CORRIGIDAS:
                    // Removemos o arredondamento (round/int) e passamos o vetor "cinemas"
                    resultadoTemporario = buscarAtePreco(precoInput, cinemas);
                } else if (opcaoFiltro == 6) {
                    int min, max;
                    cout << "Ano inicial e final: "; cin >> min >> max;
                    resultadoTemporario = buscarCinemasPorListaFilmes(buscarPorIntervaloDeAnos(min, max));
                } else if (opcaoFiltro == 7) {
                    string titulo;
                    cout << "Digite o titulo: ";
                    cin.ignore();
                    getline(cin, titulo);
                    resultadoTemporario = buscarCinemasPorTituloFilme(titulo);
                }
            }

            auto queryStart = chrono::high_resolution_clock::now();

            if (primeiraBusca) {
                resultadoAtual = resultadoTemporario;
                primeiraBusca = false;
            } else if (operadorLogico == 2) {
                resultadoAtual = AND_Vectors(resultadoAtual, resultadoTemporario);
            } else if (operadorLogico == 3) {
                resultadoAtual = OR_Vectors(resultadoAtual, resultadoTemporario);
            }

            auto queryEnd = chrono::high_resolution_clock::now();
            auto queryDuration = chrono::duration_cast<chrono::microseconds>(queryEnd - queryStart);

            cout << "-> Operacao logica executada em: " << queryDuration.count() << " microsegundos." << endl;
            cout << "-> Registros encontrados ate agora: " << resultadoAtual.size() << "\n" << endl;
        }

        cout << "\n=== RESULTADO FINAL (" << resultadoAtual.size() << " encontrados) ===" << endl;

        ofstream arquivoSaida("resultado_cpp.txt");
        if (arquivoSaida.is_open()) {
            for (int id : resultadoAtual) {
                if (opcaoEntidade == 1) {
                    arquivoSaida << filmes[id].get_tconst() << endl;
                } else {
                    arquivoSaida << cinemas[id].cinemas_id() << endl;
                }
            }
            arquivoSaida.close();
            cout << "[!] IDs exportados para 'resultado_cpp.txt' com sucesso." << endl;
        }

        int limiteImpressao = min((int)resultadoAtual.size(), 15);
        for (int i = 0; i < limiteImpressao; ++i) {
            int id = resultadoAtual[i];
            if (opcaoEntidade == 1) {
                cout << "- [" << filmes[id].get_tconst() << "] " << filmes[id].get_primary_title() << " (" << filmes[id].get_start_year() << ")" << endl;
            } else {
                cout << "- [" << cinemas[id].cinemas_id() << "] " << cinemas[id].nome_cinema() << " (R$ " << cinemas[id].preco_ingresso() << ")" << endl;
            }
        }
        if (resultadoAtual.size() > 15) cout << "... e mais " << resultadoAtual.size() - 15 << " registros." << endl;

    } while (opcaoEntidade != 0);

    return 0;
}
