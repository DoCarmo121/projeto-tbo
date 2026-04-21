#ifndef PROJETO_TBO_FILMESCROP_H
#define PROJETO_TBO_FILMESCROP_H

#include <string>
#include <vector>

class FilmesCrop {

private:
    std::string titleType, primaryTitle, originalTitle;
    std::vector<std::string> genres;
    bool isAdult;
    int tconst, startYear, runtimeMinutes;

public:
    FilmesCrop();


    FilmesCrop(int tconst ,const std::string &title_type, const std::string &primary_title,
               const std::string &original_title, const std::string &raw_genres, bool is_adult, int start_year,
               int runtime_minutes);

    int get_tconst() const;
    void set_tconst(const int tconst);

    std::string get_title_type() const;
    void set_title_type(const std::string &title_type);

    std::string get_primary_title() const;
    void set_primary_title(const std::string &primary_title);

    std::string get_original_title() const;
    void set_original_title(const std::string &original_title);

    const std::vector<std::string>& get_genres() const;
    void set_genres(const std::string &raw_genres);

    bool get_is_adult() const;
    void set_is_adult(bool is_adult);

    int get_start_year() const;
    void set_start_year(int start_year);

    int get_runtime_minutes() const;
    void set_runtime_minutes(int runtime_minutes);
};

#endif //PROJETO_TBO_FILMESCROP_H