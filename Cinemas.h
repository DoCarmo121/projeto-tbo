//
// Created by bernardo on 20/04/2026.
//

#ifndef PROJETO_TBO_CINEMAS_H
#define PROJETO_TBO_CINEMAS_H

#include <string>
#include <vector>

class Cinemas {
private:
    int cinemasId;
    std::string nomeCinema;
    int coordenadaX;
    int coordenadaY;
    float precoIngresso;
    std::vector<int> filmesExibicao;

public:
    Cinemas();

    Cinemas(const int cinemas_id, const std::string &nome_cinema, int coordenada_x, int coordenada_y,
            float preco_ingresso, const std::vector<int> &filmes_exibicao);

    int cinemas_id() const;
    void set_cinemas_id(const int &cinemas_id);

    std::string nome_cinema() const;
    void set_nome_cinema(const std::string &nome_cinema);

    int coordenada_x() const;
    void set_coordenada_x(int coordenada_x);

    int coordenada_y() const;
    void set_coordenada_y(int coordenada_y);

    float preco_ingresso() const;
    void set_preco_ingresso(float preco_ingresso);

    std::vector<int> filmes_exibicao() const;
    void set_filmes_exibicao(const std::vector<int> &filmes_exibicao);
};

#endif //PROJETO_TBO_CINEMAS_H