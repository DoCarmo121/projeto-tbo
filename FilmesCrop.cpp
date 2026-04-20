#include "FilmesCrop.h"
#include <iostream>

using namespace std;


FilmesCrop::FilmesCrop() {}


FilmesCrop::FilmesCrop(const string &tconst, const string &title_type, const string &primary_title,
                       const string &original_title, const string &genres, bool is_adult, int start_year, int runtime_minutes) {
    this->tconst = tconst;
    this->titleType = title_type;
    this->primaryTitle = primary_title;
    this->originalTitle = original_title;
    this->genres = genres;
    this->isAdult = is_adult;
    this->startYear = start_year;
    this->runtimeMinutes = runtime_minutes;
}

std::string FilmesCrop::get_tconst() const {
    return tconst;
}
void FilmesCrop::set_tconst(const std::string &tconst) {
    this->tconst = tconst;
}

std::string FilmesCrop::get_title_type() const {
    return titleType;
}
void FilmesCrop::set_title_type(const std::string &title_type) {
    this->titleType = title_type;
}

std::string FilmesCrop::get_primary_title() const {
    return primaryTitle;
}
void FilmesCrop::set_primary_title(const std::string &primary_title) {
    this->primaryTitle = primary_title;
}

std::string FilmesCrop::get_original_title() const {
    return originalTitle;
}
void FilmesCrop::set_original_title(const std::string &original_title) {
    this->originalTitle = original_title;
}

std::string FilmesCrop::get_genres() const {
    return genres;
}
void FilmesCrop::set_genres(const std::string &genres) {
    this->genres = genres;
}

bool FilmesCrop::get_is_adult() const {
    return isAdult;
}
void FilmesCrop::set_is_adult(bool is_adult) {
    this->isAdult = is_adult;
}

int FilmesCrop::get_start_year() const {
    return startYear;
}
void FilmesCrop::set_start_year(int start_year) {
    this->startYear = start_year;
}

int FilmesCrop::get_runtime_minutes() const {
    return runtimeMinutes;
}
void FilmesCrop::set_runtime_minutes(int runtime_minutes) {
    this->runtimeMinutes = runtime_minutes;
}