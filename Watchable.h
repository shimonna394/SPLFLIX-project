#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>


class Session;

class Watchable{
public:
    Watchable(long id, int length, const std::vector<std::string>& tags);///Constructor
    Watchable(const Watchable &other);///Copy Constructor
    virtual ~Watchable();///Destructor
    virtual Watchable* clone()=0;//copy  the watchable to a new place
    void clear();
    virtual std::string toString() const = 0;
    virtual Watchable* getNextWatchable(Session&) const = 0;
    virtual long GetId() const;
    virtual int GetLength();
    virtual std::vector<std::string> GetTags();
    virtual bool isMovie()=0;
    virtual long GetNextEpisodeId()=0;
private:
    const long id;
    int length;
    std::vector<std::string> tags;
};

class Movie : public Watchable{
public:
    Movie(long id, const std::string &name,
          int length, const std::vector<std::string> &tags);///Constructor for movie
    Movie(const Movie &other);///Copy Constructor
    virtual ~Movie();///Destructor for movie
    virtual Watchable* clone();///copy the watchable to a new place
    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual long GetNextEpisodeId();
    virtual bool isMovie();
private:
    std::string name;
};


class Episode: public Watchable{
public:
    Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);//Constructor for Episode
    Episode(const Episode &other);///Copy Constructor for Episode
    virtual Watchable* clone();///copy the watchable to a new place
    virtual ~Episode();///Destructor for Episode
    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual bool isMovie();
    virtual void setNextEpisodeId(long id);
    virtual long GetNextEpisodeId();
private:
    std::string seriesName;
    int season;
    int episode;
    long nextEpisodeId;
};

#endif
