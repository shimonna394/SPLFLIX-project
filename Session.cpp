#include <vector>
#include <unordered_map>
#include "../include/json.hpp"
#include "../include/Session.h"
#include "../include/Watchable.h"
#include "../include/User.h"
#include <fstream>

                ///-------Rule Of Five-------///

///constructor///
Session::Session(const std::string &configFilePath):content(),actionsLog(),userMap(),activeUser(),def(),input()//Constructor
{
    //read a JSON file
    using json = nlohmann::json;
    std::ifstream i(configFilePath);
    json j;
    i>>j;
    InsertsMovies(j);// inserting all movies to content
    InsertEpisodes(j);//inserting all tv series to content
    def=new LengthRecommenderUser("default");
    activeUser=def;
    std::pair<std::string,User*> p1("default",def);
    userMap.insert(p1);
}

Session::~Session() {///Destructor for session
    clear();
}

///Copy Constructor///
Session::Session(const Session &s):content(),actionsLog(),userMap(),activeUser(),def(),input(){
    copy(s);
}
/// copy ass///
Session &Session::operator=(const Session &s) {
    if (this != &s) {
        clear();
        copy(s);
    }
    return *this;
}

///move constructor
Session::Session(Session && s):content(s.content), actionsLog(s.actionsLog), userMap(s.userMap), activeUser(s.activeUser), def(s.def), input(s.input){
    for (size_t i = 0; i < s.content.size(); i++)
        s.content[i] = nullptr;
    for (size_t i = 0; i < s.actionsLog.size(); i++)
        s.actionsLog[i] = nullptr;
    for (auto &element:s.userMap)
        element.second = nullptr;
    s.activeUser = nullptr;
    s.def = nullptr;
    for (size_t i = 0; i < s.input.size(); i++)
        input[i] = nullptr;
}
///move ass///
Session &Session::operator=(Session &&s) {
    if (this != &s) {
        clear();
        for (auto element:s.content)
            content.push_back(element);
        for (auto element:s.actionsLog)
            actionsLog.push_back(element);
        for (auto element:s.userMap)
            userMap.insert(element);
        for (auto element:s.input)
            input.push_back(element);
        activeUser = s.activeUser;
        def = s.def;
        s.def = nullptr;
        s.activeUser = nullptr;
        s.input.clear();
        s.userMap.clear();
        s.actionsLog.clear();
        s.content.clear();
    }
    return *this;
}

/// ------------Public Methods---------///

std::vector<Watchable*> Session::GetContent() {
    return content;
}
User* Session::getActiveUser() {
    return activeUser;
}
std::vector<std::string> Session::getInput() {
    return input;
}
std::unordered_map<std::string,User*> Session::getMap() {
    return userMap;
}
std::vector<BaseAction*> Session::GetActionLog() {
    return actionsLog;
}
void Session::setActiveUser(User *user) {
    activeUser=user;
}

void Session::addUser(std::string s, User * u) {
    this->userMap.insert(std::make_pair(s,u));
}

void Session::SetActionLog(BaseAction * action) {
    actionsLog.push_back(action);
}

void Session::deleteUser(std::string name, User * user) {
    delete(user);
    userMap.erase(name);
}
///-------------Start-------///

void Session:: start(){
    std::cout<<"SPLFIX is now on"<<std::endl;
    std::string word;
    std::getline(std::cin,word);
    while(word!="exit"){
        input=getInputLine(word);//now the commend is in the vector divided by words
        std::string act=input[0];
        if(act=="createuser") {//if the action is CreateUser
            CreateUser *nUser = new CreateUser();
            nUser->act(*this);
        }
        else if(act=="changeuser"){//if the action is ChangeUser
            ChangeActiveUser *cUser=new ChangeActiveUser();
            cUser->act(*this);
        }
        else if(act=="deleteuser"){
            DeleteUser *dUser=new DeleteUser();
            dUser->act(*this);
        }
        else if(act=="dupuser"){
            DuplicateUser *dupUser=new DuplicateUser();
            dupUser->act(*this);
        }
        else if(act=="content"){
            PrintContentList *pList=new PrintContentList();
            pList->act(*this);
        }
        else if(act=="watchhist"){
            PrintWatchHistory *wHist=new PrintWatchHistory();
            wHist->act(*this);
        }
        else if(act=="watch"){
            Watch *watch=new Watch();
            watch->act(*this);
        }
        else if(act=="log"){
            PrintActionsLog *pLog=new PrintActionsLog();
            pLog->act(*this);
        }
        std::getline(std::cin,word);
        if(word=="exit") {
            Exit *exit1 = new Exit();
            exit1->act(*this);
        }
        input.clear();
    }
}
///----------Private Mathods---------///

    void Session::copy(const Session &s) {
        for (size_t i = 0; i < s.content.size(); i++) {
            content.push_back(s.content[i]->clone());
        }
        for (size_t i = 0; i < s.actionsLog.size(); i++) {
            actionsLog.push_back(s.actionsLog[i]->clone());
        }
        for (size_t i = 0; i < s.input.size(); i++) {
            input.push_back(s.input[i]);
        }
        for (const auto &element:s.userMap) {
            User *user = element.second->Clone();
            user->setPointers(*this);
            std::string name = element.first;
            userMap.insert({name, user});
            if (user->getName() == "default")
                def = user;
        }
        std::string name = s.activeUser->getName();
        for (const auto &element: userMap) {
            if (element.first == name)
                activeUser = element.second;
        }
    }

void Session::clear() {//deletes all of Sessions objects
    for(size_t i=0;i<content.size();i++){//deletes all Watchable
        delete(content[i]);
    }
    for(const auto& element:userMap){//deletes all users
        delete(element.second);
    }
    for(const auto& element:actionsLog){//deletes all logs
        delete(element);
    }
    activeUser= nullptr;
    def= nullptr;
    userMap.clear();
    content.clear();
    actionsLog.clear();
    input.clear();
}

void Session::InsertsMovies(nlohmann::json j) {
    for (unsigned long a = 0; a < j["movies"].size(); a++) {
        Watchable *m1 = new Movie(a + 1, j["movies"][a]["name"], j["movies"][a]["length"], j["movies"][a]["tags"]);
        content.push_back(m1);
    }
}

void Session::InsertEpisodes(nlohmann::json j) {
    long counter=j["movies"].size()+1;//id for the episodes
    int numOfTvSeries=j["tv_series"].size();
    for(int i1=0;i1<numOfTvSeries;i1++){//loop for al tv series
        int numOfSeasons=j["tv_series"][i1]["seasons"].size();
        for(int i2=0;i2<numOfSeasons;i2++){ //loop for the number of seasons
            int numOfEpisodes=j["tv_series"][i1]["seasons"][i2];
            for(int i3=0;i3<numOfEpisodes;i3++){//loop for the number of episodes per season
                Episode *ep1=new Episode(counter, j["tv_series"][i1]["name"], j["tv_series"][i1]["episode_length"], i2+1, i3+1, j["tv_series"][i1]["tags"]);
                content.push_back(ep1);
                counter++;
                if(i2+1==numOfSeasons&&i3+1==numOfEpisodes)//if there are no more episodes or seasons
                    ep1->setNextEpisodeId(-1);
            }// end of third loop
        }//end of second loop
    }// end of all loops
}

std::vector<std::string> Session::getInputLine(std::string word){//split the commend form user to separate words
    std::vector<std::string>ans;
    while(word.size()>0){
        int find=word.find(" ");
        if(find==-1){
            ans.push_back(word);
            break;
        }
        else{
            ans.push_back(word.substr(0,find));
            word=word.substr(find+1,word.size());
        }
    }
    return ans;
}


///end of session
