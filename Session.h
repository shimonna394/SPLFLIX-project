#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"
#include "../include/json.hpp"

#include <fstream>

class User;
class Watchable;

class Session{
public:
    Session(const std::string &configFilePath);///Constructor for session
    Session(const Session &s);///Copy constructor for session
    Session &operator=(const Session& s);///Copy Assignment
    ~Session();///Destructor for session
    Session(Session &&s);///Move Constructor
    Session &operator=( Session&& s);///Move Assignment
    std::vector<Watchable*> GetContent();
    User* getActiveUser();//return active user
    std::vector<std::string> getInput();
    std::unordered_map<std::string,User*> getMap();
    std::vector<BaseAction*> GetActionLog();
    void SetActionLog(BaseAction*);
    void setActiveUser(User* user);
    void start();
    void addUser(std::string,User*);//adds users to usermap
    void deleteUser(std::string name,User* user);//Delete a user from the user map
private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
    User* def;
    std::vector<std::string>input;//contain user requests
    std::vector<std::string> getInputLine(std::string basicString);
    void InsertsMovies(nlohmann::json j);
    void InsertEpisodes(nlohmann::json j);
    void copy(const Session &s);
    void clear();
};
#endif
