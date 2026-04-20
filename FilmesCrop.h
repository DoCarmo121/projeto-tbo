#ifndef PROJETO_TBO_FILMESCROP_H
#define PROJETO_TBO_FILMESCROP_H

#include <string>

class FilmesCrop {

private:
    std::string tconst, titleType, primaryTitle, originalTitle, genres;
    bool isAdult;
    int startYear, runtimeMinutes;

public:
    FilmesCrop();


    FilmesCrop(const std::string &tconst, const std::string &title_type, const std::string &primary_title,
               const std::string &original_title, const std::string &genres, bool is_adult, int start_year,
               int runtime_minutes);

    std::string get_tconst() const;
    void set_tconst(const std::string &tconst);

    std::string get_title_type() const;
    void set_title_type(const std::string &title_type);

    std::string get_primary_title() const;
    void set_primary_title(const std::string &primary_title);

    std::string get_original_title() const;
    void set_original_title(const std::string &original_title);

    std::string get_genres() const;
    void set_genres(const std::string &genres);

    bool get_is_adult() const;
    void set_is_adult(bool is_adult);

    int get_start_year() const;
    void set_start_year(int start_year);

    int get_runtime_minutes() const;
    void set_runtime_minutes(int runtime_minutes);
};

#endif //PROJETO_TBO_FILMESCROP_H