#include "../include/Action.h"
#include "../include/User.h"
#include <string>
#include <vector>
#include <iostream>
#include "../include/Session.h"
#include "../include/Watchable.h"
///----------------constructors---------------///
BaseAction::BaseAction():errorMsg(""),status() {status=PENDING;}
BaseAction::BaseAction(const BaseAction &other):errorMsg(other.errorMsg),status(other.status) {}

///-------public methods---------///
ActionStatus BaseAction::getStatus() const {
    return status;
}
bool BaseAction::Isfound(std::string name, std::unordered_map<std::string, User *> user) {
    for(const auto& element:user) {
        if (element.first == name)
            return true;
    }
    return false;
}
///--------protected methods--------///
void BaseAction::complete() {
    status=COMPLETED;
}
void BaseAction::error(const std::string &errorMsg) {//changing status to error and printing a massage
    status=ERROR;
    this->errorMsg=errorMsg;
    std::cout<<this->errorMsg<<std::endl;
}
std::string BaseAction::getErrorMsg() const {
    return this->errorMsg;
}

///-------inherits from BaseAction-------///
void CreateUser::act(Session &sess) {
    sess.SetActionLog(this);
    std::unordered_map<std::string,User*> usermap=sess.getMap();
    std::vector<std::string>cmd=sess.getInput();
    if(cmd.size()!=3){//unvalid input
        error("The input is not valid");
    }
    else if(Isfound(cmd[1],usermap)){//we found a user with the same username
        error("we found a user with the same username");
    }
    else if(cmd[2]=="len"){
        User* user=new LengthRecommenderUser(cmd[1]);
        sess.addUser(cmd[1],user);
        complete();
    }
    else if(cmd[2]=="rer"){
        User* user=new RerunRecommenderUser(cmd[1]);
        sess.addUser(cmd[1],user);
        complete();
    }
    else if(cmd[2]=="gen"){
        User* user=new GenreRecommenderUser(cmd[1]);
        sess.addUser(cmd[1],user);
        complete();
    }
    else{//unvalid algo type
        error("unvalid algo type");
    }
}
std::string CreateUser::toString() const {
    std::string s;
    if(getStatus()==COMPLETED)
        s="COMPLETED";
    else if(getStatus()==PENDING)
        s="PENDING";
    else
        s="ERROR:";
    return "CreateUser "+s+getErrorMsg();
}

BaseAction *CreateUser::clone() {
    return new CreateUser(*this);
}

void ChangeActiveUser::act(Session &sess) {
    sess.SetActionLog(this);
    std::unordered_map<std::string,User*> usermap=sess.getMap();
    std::vector<std::string>cmd=sess.getInput();
    if(cmd.size()!=2){error("unvalid input length");}//unvalid input length
    else if(!Isfound(cmd[1],usermap))
        error("user does not exists");//user does not exists
    else {
        sess.setActiveUser(usermap.find(cmd[1]).operator*().second);
        complete();
    }
}
std::string ChangeActiveUser::toString() const {
    std::string s;
    if(getStatus()==COMPLETED)
        s="COMPLETED";
    else if(getStatus()==PENDING)
        s="PENDING";
    else
        s="ERROR:";
    return "ChangeActiveUser "+s+getErrorMsg();
}

BaseAction *ChangeActiveUser::clone() {
    return new ChangeActiveUser(*this);
}

void DeleteUser::act(Session &sess) {
    sess.SetActionLog(this);
    std::unordered_map<std::string,User*> usermap=sess.getMap();
    std::vector<std::string>cmd=sess.getInput();
    std::string name;
    if(cmd.size()>1)
        name = cmd[1];
    if(cmd.size()!=2) {
        error("unvalid input length");//unvalid input length
    }
    else if(!Isfound(name,usermap))
        error("user does not exists");//user does not exists
        else if(name==sess.getActiveUser()->getName())
            error("you cannot delete the active user");//cannot delete the active user
    else{
        User* user1=usermap.at(name);
        sess.deleteUser(name,user1);
        complete();
    }
}
std::string DeleteUser::toString() const {
    std::string s;
    if(getStatus()==COMPLETED)
        s="COMPLETED";
    else if(getStatus()==PENDING)
        s="PENDING";
    else
        s="ERROR:";
    return "DeleteUser "+s+getErrorMsg();
}

BaseAction *DeleteUser::clone() {
    return new DeleteUser(*this);
}

void DuplicateUser::act(Session &sess) {
    sess.SetActionLog(this);
    std::unordered_map<std::string,User*> usermap=sess.getMap();
    std::vector<std::string>cmd=sess.getInput();
    if(cmd.size()!=3) {
        { error("unvalid input length"); }//unvalid input length

    }
    else if(!Isfound(cmd[1],usermap)) {
        error("user does not exists");//user does not exists
    }

    else if(Isfound(cmd[2],usermap))
        error("user allready exist");//user allready exist
    else {
        User* user = usermap.find(cmd[1]).operator*().second->Clone();
        user->SetName(cmd[2]);
        sess.addUser(cmd[2],user);
        complete();
    }
}
std::string DuplicateUser::toString() const {
    std::string s;
    if(getStatus()==COMPLETED)
        s="COMPLETED";
    else if(getStatus()==PENDING)
        s="PENDING";
    else
        s="ERROR:";
    return "DuplicateUser "+s+getErrorMsg();
}

BaseAction *DuplicateUser::clone() {
    return new DuplicateUser(*this);
}


void PrintContentList::act(Session &sess) {
    sess.SetActionLog(this);
    std::vector < Watchable * > content = sess.GetContent();
    for (size_t i = 0; i < content.size(); i++) {//loop fro all of the watchable in content
        Watchable *w1 = content[i];
        std::string s = "";
        std::string stringId=std::to_string(w1->GetId());
        std::string stringLen=std::to_string(w1->GetLength());
        s = s + stringId + ". " + w1->toString() + " " + stringLen + " minutes ";
        std::vector<std::string> tags = w1->GetTags();
        s = s + "[" + tags[0] + ", ";
        for (size_t j = 1; j < tags.size() - 1; j++) {//loops for tags
            s = s + tags[j] + ", ";
        }
        s = s + tags[tags.size() - 1] + "]";
        std::cout << s << std::endl;
    }
    complete();
}
std::string PrintContentList::toString() const {
    std::string s;
    if(getStatus()==COMPLETED)
        s="COMPLETED";
    else if(getStatus()==PENDING)
        s="PENDING";
    else
        s="ERROR:";
    return "PrintContentList "+s+getErrorMsg();
}

BaseAction *PrintContentList::clone() {
    return new PrintContentList(*this);
}

void PrintWatchHistory::act(Session &sess) {
    sess.SetActionLog(this);
    std::string userName = sess.getActiveUser()->getName();
    std::vector<Watchable*> history = sess.getActiveUser()->get_history();
    std::cout<<"watch history for "+userName<<std::endl;
    for(size_t i=0;i<history.size();i++){
        std::string id = std::to_string(i+1);
        std::cout<<id+"."+history[i]->toString()<<std::endl;
    }
    complete();
}
std::string PrintWatchHistory::toString() const {
    std::string s;
    if(getStatus()==COMPLETED)
        s="COMPLETED";
    else if(getStatus()==PENDING)
        s="PENDING";
    else
        s="ERROR:";
    return "PrintWatchHistory "+s+getErrorMsg();
}

BaseAction *PrintWatchHistory::clone() {
    return new PrintWatchHistory(*this);
}

void Watch::act(Session &sess) {
    sess.SetActionLog(this);
    std::vector<std::string> cmd = sess.getInput();
    if (cmd.size() != 2) { error("unvalid input length"); }//unvalid input length
    else if ((stol(cmd[1]) < 1) |(unsigned (stol(cmd[1])) > sess.GetContent().size()))
        error("the id is not valid");//unvalid id
    else {
        long id1 = stol(cmd[1]);//the id for the show
        Watchable* watch = sess.GetContent()[id1-1];
        std::cout << "Watching " + watch->toString() << std::endl;
        sess.getActiveUser()->SetHistory(watch);//adds watch to history
        complete();
        if(!sess.getActiveUser()->isRerun())//if the field notSeenContents exists
            sess.getActiveUser()->SetNotSeenContent(watch,sess);
        Watchable *watchRecommand = watch->getNextWatchable(sess);//gets the next recommended show
        if (watchRecommand != nullptr) {
            std::cout << "We recommand watching " + watchRecommand->toString() + ",continue watching?[y/n]"<< std::endl;
            std::string nextWatch;
            std::getline(std::cin, nextWatch);
            while ((nextWatch == "y") & (watchRecommand!= nullptr)) {
                Watch *nWatch=new Watch();
                sess.SetActionLog(nWatch);
                nWatch->complete();
                watch = watchRecommand;
                std::cout << "Watching " + watch->toString() << std::endl;
                sess.getActiveUser()->SetHistory(watch);
                complete();
                if(!sess.getActiveUser()->isRerun())//if the field notSeenContents exists
                   sess.getActiveUser()->SetNotSeenContent(watch,sess);
                watchRecommand = watch->getNextWatchable(sess);
                if (watchRecommand != nullptr) {
                    std::cout << "We recommand watching " + watchRecommand->toString() + ",continue watching?[y/n]"
                              << std::endl;
                    std::getline(std::cin, nextWatch);
                }
            }
        }
    }
}
std::string Watch::toString() const {
    std::string s;
    if(getStatus()==COMPLETED)
        s="COMPLETED";
    else if(getStatus()==PENDING)
        s="PENDING";
    else
        s="ERROR:";
    return "Watch "+s+getErrorMsg();
}

BaseAction *Watch::clone() {
    return new Watch(*this);
}

void PrintActionsLog::act(Session &sess) {
    std::vector<BaseAction*> ActionLog = sess.GetActionLog();
    for(int i=ActionLog.size()-1;i>=0;i--){
            std::cout<<ActionLog[i]->toString()<<std::endl;
    }
    complete();
    sess.SetActionLog(this);
}
std::string PrintActionsLog::toString() const {
    std::string s;
    if(getStatus()==COMPLETED)
        s="COMPLETED";
    else if(getStatus()==PENDING)
        s="PENDING";
    else
        s="ERROR:";
    return "PrintActionsLog "+s+" "+getErrorMsg();
}

BaseAction *PrintActionsLog::clone() {
    return new PrintActionsLog(*this);
}

void Exit::act(Session &sess) {
    sess.SetActionLog(this);
    complete();
}
std::string Exit::toString() const {
    std::string s;
    if(getStatus()==COMPLETED)
        s="COMPLETED";
    else if(getStatus()==PENDING)
        s="PENDING";
    else
        s="ERROR:";
    return "Exit "+s+getErrorMsg();
}

BaseAction *Exit::clone() {
    return new Exit(*this);
}

