#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
class Watchable;
class Session;

class User{
public:
    User(const std::string& name);///Constructor for user
    User(const User &other);///Copy Constructor
    virtual ~User();///Destructor for User
    void clear();
    virtual Watchable* getRecommendation(Session& s) = 0;
    virtual User* Clone()=0;
    std::string getName() const;
    std::vector<Watchable*> get_history() const;
    void SetHistory(Watchable*);//adds content to history
    void setPointers(Session &s);
    void SetName(std::string name);
    virtual void SetNotSeenContent(Watchable* watched,Session& s)=0;//will delete the last watched content from NotSeenContent
    virtual bool isRerun()=0;
protected:
    std::vector<Watchable*> history;
private:
    std::string  name;
};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string &name);///Constructor
    LengthRecommenderUser(const LengthRecommenderUser &other);///Copy Constructor
    virtual ~LengthRecommenderUser();///Destructor
    virtual Watchable* getRecommendation(Session& s);
    virtual User* Clone();
    void SetNotSeenContent(Watchable* watched,Session& s);//will delete the last watched content from NotSeenContent
    virtual bool isRerun();
private:
    std::vector<bool>notSeenContent;
};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);///Constructor
    RerunRecommenderUser(const RerunRecommenderUser &other);///Copy Constructor
    virtual ~RerunRecommenderUser();///Destructor
    virtual Watchable* getRecommendation(Session& s);
    virtual User* Clone();

    void SetNotSeenContent(Watchable* watched,Session& s);//will delete the last watched content from NotSeenContent
    virtual bool isRerun();
private:
    int count;//index for the last recommended content
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);///Constructor
    GenreRecommenderUser(const GenreRecommenderUser &other);///Copy Constructor
    virtual ~GenreRecommenderUser();///Destructor
    virtual Watchable* getRecommendation(Session& s);
    virtual User* Clone();
    void SetNotSeenContent(Watchable* watched,Session& s);//will delete the last watched content from NotSeenContent
    virtual bool isRerun();
private:
    std::vector<bool >notSeenContent;
    std::vector<std::pair<std::string,int>>sortedTags;
    void SetSortedTags();//Inserts tags to field SortedTags
    bool static sortbysecdesc(const std::pair<std::string,int> &a,const std::pair<std::string,int> &b);//Sort tags in field SortedTags
};

#endif
