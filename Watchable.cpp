
#include "../include/Watchable.h"
#include <string>
#include <vector>
#include "../include/Session.h"
#include "../include/User.h"



///-----Constructor------///
Watchable::Watchable(long id, int length, const std::vector<std::string> &tags) : id(id), length(length),tags(tags){}

///---Copy Constructor---///
Watchable::Watchable(const Watchable &other):id(other.GetId()),length(other.length),tags() {
    for(size_t i=0;i<other.tags.size();i++){
        tags.push_back(other.tags[i]);
    }
}

///-----Destructor-----///
Watchable::~Watchable() {clear();//Destructor for Watchable
}

///---------Public Mathods-------///
void Watchable::clear() {
    for(size_t i=0;i<tags.size();i++){
        tags[i].clear();
    }
}

long Watchable::GetId() const {
    return id;
}
int Watchable::GetLength() {
    return length;
}
std::vector<std::string> Watchable::GetTags() {
    return tags;
}


                    ///------------Inherits From Watchabla-----------///

                ///-----Movie-----///
            ///----Rule Of Three----///

///Constructor
Movie::Movie(long id, const std::string &name, int length, const std::vector<std::string> &tags) : Watchable(id, length,tags),name(name) {}

///Copy Constructor
Movie::Movie(const Movie &other):Watchable(other),name(other.name) {}//Copy Constructor
Watchable* Movie::clone() {
    return new Movie(*this);
}

///Destructor for movie
Movie::~Movie() {clear();}


///--------Public Mathods----///

std::string Movie::toString() const {
    return name;
}
Watchable* Movie::getNextWatchable(Session & sess) const {
    return sess.getActiveUser()->getRecommendation(sess);
}

long Movie::GetNextEpisodeId() {
    return -1;
}
bool Movie::isMovie() {
    return true;
}

                ///----------Episode----------///
        ///-----Rule Of Three-----///

///Constructor
Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,const std::vector<std::string> &tags) : Watchable(id, length, tags), seriesName(seriesName),season(season), episode(episode),nextEpisodeId(id+1) {}

///Copy Constructor
Episode::Episode(const Episode &other):Watchable(other),seriesName(other.seriesName),season(other.season),episode(other.episode),nextEpisodeId(other.nextEpisodeId) {}
Watchable* Episode::clone() {
    return new Episode(*this);
}

///Destructor
Episode::~Episode() {clear();}

///------Public Mathods--------

std::string Episode::toString() const {
    std::string seasons=std::to_string(season);
    std::string episodes=std::to_string(episode);
    if(season<10)
        seasons="0"+std::to_string(season);
    if(episode<10)
        episodes="0"+std::to_string(episode);
    return seriesName+" "+"S"+seasons+"E"+episodes;
}
Watchable* Episode::getNextWatchable(Session &sess) const {
    return sess.getActiveUser()->getRecommendation(sess);
}
void Episode::setNextEpisodeId(long id) {
    nextEpisodeId = id;
}
long Episode::GetNextEpisodeId() {
    return nextEpisodeId-1;
}
bool Episode::isMovie() {
    return false;
}



