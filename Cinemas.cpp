//
// Created by bernardo on 20/04/2026.
//

#include "Cinemas.h"

Cinemas::Cinemas() : coordenadaX(0), coordenadaY(0), precoIngresso(0.0f) {
}

Cinemas::Cinemas(const int cinemas_id, const std::string &nome_cinema, int coordenada_x, int coordenada_y,
                 float preco_ingresso, const std::vector<int> &filmes_exibicao)
        : cinemasId(cinemas_id), nomeCinema(nome_cinema), coordenadaX(coordenada_x),
          coordenadaY(coordenada_y), precoIngresso(preco_ingresso), filmesExibicao(filmes_exibicao) {
}

int  Cinemas::cinemas_id() const {
    return cinemasId;
}

void Cinemas::set_cinemas_id(const int &cinemas_id) {
    cinemasId = cinemas_id;
}

std::string Cinemas::nome_cinema() const {
    return nomeCinema;
}

void Cinemas::set_nome_cinema(const std::string &nome_cinema) {
    nomeCinema = nome_cinema;
}

int Cinemas::coordenada_x() const {
    return coordenadaX;
}

void Cinemas::set_coordenada_x(int coordenada_x) {
    coordenadaX = coordenada_x;
}

int Cinemas::coordenada_y() const {
    return coordenadaY;
}

void Cinemas::set_coordenada_y(int coordenada_y) {
    coordenadaY = coordenada_y;
}

float Cinemas::preco_ingresso() const {
    return precoIngresso;
}

void Cinemas::set_preco_ingresso(float preco_ingresso) {
    precoIngresso = preco_ingresso;
}

std::vector<int> Cinemas::filmes_exibicao() const {
    return filmesExibicao;
}

void Cinemas::set_filmes_exibicao(const std::vector<int> &filmes_exibicao) {
    filmesExibicao = filmes_exibicao;
}