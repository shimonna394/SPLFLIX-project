#include <vector>
#include "../include/Session.h"
#include "../include/Watchable.h"
#include "../include/User.h"
#include <string>
#include <iostream>



User::User(const std::string &name) :history(),name(name){}///Constructor for user
User::User(const User &other):history(),name(other.name) {///Copy constructor

    for(size_t i=0;i<other.history.size();i++){
            history.push_back(other.history[i]);
    }
}
User::~User() {///Destructor
    clear();
}

///---------Public Mathods-------///

std::string User::getName() const {
    return name;
}
std::vector<Watchable*> User::get_history() const {
    return history;
}
void User::SetName(std::string name) {
    this->name=name;
}

void User::SetHistory(Watchable* toAdd) {//adds content to history
    history.push_back(toAdd);
}

void User::clear() {
for(size_t i=0;i<history.size();i++){
    history[i]= nullptr;
}
}

void User::setPointers(Session &s) {
for(size_t i=0;i<history.size();i++){
    history[i]=s.GetContent()[history[i]->GetId()-1];
    }
}

            ///-----------------------Inherits From User-----------------///

                          ///------- LengthRecommenderUser-------///
///----------------Rule Of Three------------///
///Constructor
LengthRecommenderUser::LengthRecommenderUser(const std::string &name) : User(name),notSeenContent() {}

///Copy Constructor
LengthRecommenderUser::LengthRecommenderUser(const LengthRecommenderUser &other):User(other),notSeenContent() {///Copy Constructor
    for(size_t i=0;i<other.notSeenContent.size();i++){
        notSeenContent.push_back(other.notSeenContent[i]);
    }
}

///Destructor
LengthRecommenderUser::~LengthRecommenderUser() {
    clear();
    notSeenContent.clear();
}

///--------Public Mathods---------///

    Watchable* LengthRecommenderUser::getRecommendation(Session &s) {
        if(history.size()==s.GetContent().size())
            return nullptr;//we watched everything
        if (history.size() > 0 && !history[history.size() - 1]->isMovie() &&history[history.size() - 1]->GetNextEpisodeId() >0)//if our last watchable was an episode and this was not the final episode for the show
            return s.GetContent()[history[history.size() - 1]->GetNextEpisodeId()];//return next episode
        else {
            int sum = 0;
            double avg = 0;
            for (size_t i = 0; i < history.size(); i++) {
                sum = sum + history[i]->GetLength();
            }
            if (sum != 0) {
                avg = sum / history.size();//avg of content the user watched
            }
            int i=0;//index for notSeenContent
            while(notSeenContent[i]==false)
                  i++;
            Watchable* ans=s.GetContent()[i];
            double dis=abs(ans->GetLength()-avg);//how close to avg
            for(size_t j=i+1;j<notSeenContent.size();j++){
                if(notSeenContent[j]==true){
                    double tempDis=abs(s.GetContent()[j]->GetLength()-avg);
                    if(tempDis<dis) {
                        dis = tempDis;
                        ans = s.GetContent()[j];
                    }
                }
            }
            return ans;
        }
    }


User* LengthRecommenderUser::Clone() {
    User* lenUser = new LengthRecommenderUser(*this);
    return lenUser;
}

bool LengthRecommenderUser::isRerun() {
    return false;
}

void LengthRecommenderUser::SetNotSeenContent(Watchable *watched, Session &s) {
    if(notSeenContent.size()==0){
        for(size_t i=0;i<s.GetContent().size();i++)
            notSeenContent.push_back(true);
    }
    long id= watched->GetId();
     notSeenContent[id-1]=false;
}

                     ///------GenreRecommenderUser--------///
    ///----------------Rule Of Three------------///
///Constructor///
GenreRecommenderUser::GenreRecommenderUser(const std::string &name):User(name),notSeenContent(),sortedTags() {}

///Copy Constructor///
GenreRecommenderUser::GenreRecommenderUser(const GenreRecommenderUser &other):User(other),notSeenContent(),sortedTags() {
    for(size_t i=0;i<other.notSeenContent.size();i++){
        notSeenContent.push_back(other.notSeenContent[i]);
    }
    for(size_t j=0;j<other.sortedTags.size();j++){
        sortedTags.push_back(std::make_pair(other.sortedTags[j].first,other.sortedTags[j].second));
    }
}

GenreRecommenderUser::~GenreRecommenderUser() {///Destructor
    clear();
    notSeenContent.clear();
    sortedTags.clear();
}

///--------Public Mathods-----///

Watchable* GenreRecommenderUser::getRecommendation(Session &s) {
    if (history.size() > 0 && !history[history.size() - 1]->isMovie() &&history[history.size() - 1]->GetNextEpisodeId() >0)//if our last watchable was an episode and this was not the final episode for the show
        return s.GetContent()[history[history.size() - 1]->GetNextEpisodeId()];//return next episode
    else{
        std::vector<std::string>tags;//tags of notSeenContent
        SetSortedTags();
        for(size_t i=0;i<sortedTags.size();i++){//loop to go over all the tags in the vector if needed
            for(size_t j=0;j<notSeenContent.size();j++){//finding the next notSeenContent
              if(notSeenContent[j]==true){
                  tags=s.GetContent()[j]->GetTags();
                  for(size_t k=0;k<tags.size();k++){//checking if the tags are matching
                      if(tags[k]==sortedTags[i].first)
                          return s.GetContent()[j];
                  }
              }
            }
        }
    }
    return nullptr;
}



User* GenreRecommenderUser::Clone() {
    User* genUser = new GenreRecommenderUser(*this);
    return genUser;
}

bool GenreRecommenderUser::isRerun() {
    return false;
}

void GenreRecommenderUser::SetNotSeenContent(Watchable *watched, Session &s) {
    if(notSeenContent.size()==0){
        for(size_t i=0;i<s.GetContent().size();i++)
            notSeenContent.push_back(true);
    }
    long id= watched->GetId();
    notSeenContent[id-1]=false;
}

///----------Private Mathods--------///

void GenreRecommenderUser::SetSortedTags() {
    bool found;
    Watchable *watch = history[history.size() - 1];//last watched content
    std::vector<std::string> tags = watch->GetTags();//tags of watched
    for (size_t i = 0; i < tags.size(); i++) {//loop for the tags of watch
        found = false;
        for (size_t j = 0; (j < sortedTags.size()) & (!found); j++) {//loop for tags i already have
            if (tags[i] == sortedTags[j].first) {//we have this tag
                found = true;
                sortedTags[j].second++;
            }
        }
        if (!found) {
            sortedTags.push_back(std::make_pair(tags[i], 1));//inserting the new tag
        }
    }
         std::sort(sortedTags.begin(),sortedTags.end(),sortbysecdesc);//sorting sortedTags
}

bool  GenreRecommenderUser::sortbysecdesc(const std::pair<std::string, int> &a, const std::pair<std::string, int> &b) {// sorting by key and by lexo if needed
    if(a.second>b.second)
        return true;
    else if(b.second>a.second)
        return false;
    else {
        std::string first=a.first;
        std::string second=b.first;
        std::transform(first.begin(),first.end(),first.begin(),::tolower);
        std::transform(second.begin(),second.end(),second.begin(),::tolower);
        return first < second;
    }
}

                            ///---------RerunRecommenderUser-------///
///----------------Rule Of Three------------///

///Constructor
RerunRecommenderUser::RerunRecommenderUser(const std::string &name) : User(name) {count=0;}

///Copy Constructor
RerunRecommenderUser::RerunRecommenderUser(const RerunRecommenderUser &other):User(other) {count=other.count;}

///Destructor
RerunRecommenderUser::~RerunRecommenderUser() {
    clear();
}

///----------Public Mathods----///

Watchable *RerunRecommenderUser::getRecommendation(Session &s) {
    if (history.size() > 0 && !history[history.size() - 1]->isMovie() &&history[history.size() - 1]->GetNextEpisodeId() >0)//if our last watchable was an episode and this was not the final episode for the show
        return s.GetContent()[history[history.size() - 1]->GetNextEpisodeId()];//return next episode
    else{
        count=(count+1)%history.size();
        return history[count];
            }

}

User* RerunRecommenderUser::Clone() {
    User* rerUser = new RerunRecommenderUser(*this);
    return  rerUser;
}


bool RerunRecommenderUser::isRerun() {
    return true;
}

void RerunRecommenderUser::SetNotSeenContent(Watchable *watched, Session &s) {
}

