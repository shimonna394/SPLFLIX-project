#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "User.h"

class Session;

enum ActionStatus{
    PENDING, COMPLETED, ERROR
};


class BaseAction{
public:
    BaseAction();///Constructor
    BaseAction(const BaseAction& other);///Copy Constructor
    virtual BaseAction* clone()=0;
    virtual ~BaseAction()= default;///default destructor
    ActionStatus getStatus() const;
    virtual void act(Session& sess)=0;
    virtual std::string toString() const=0;
    bool Isfound(std::string name,std::unordered_map<std::string,User*> user);
protected:
    void complete();
    void error(const std::string& errorMsg);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;
};

class CreateUser  : public BaseAction {
public:
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual BaseAction* clone();
    virtual ~CreateUser()= default;
};

class ChangeActiveUser : public BaseAction {
public:
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual BaseAction* clone();
    virtual ~ChangeActiveUser()= default;///default destructor
};

class DeleteUser : public BaseAction {
public:
    virtual void act(Session & sess);
    virtual std::string toString() const;
    virtual BaseAction* clone();
    virtual ~DeleteUser()= default;///default destructor
};


class DuplicateUser : public BaseAction {
public:
    virtual void act(Session & sess);
    virtual std::string toString() const;
    virtual BaseAction* clone();
    virtual ~DuplicateUser()= default;///default destructor
};

class PrintContentList : public BaseAction {
public:
    virtual void act (Session& sess);
    virtual std::string toString() const;
    virtual BaseAction* clone();
    virtual ~PrintContentList()= default;///default destructor
};

class PrintWatchHistory : public BaseAction {
public:
    virtual void act (Session& sess);
    virtual std::string toString() const;
    virtual BaseAction* clone();
    virtual ~PrintWatchHistory()= default;///default destructor
};


class Watch : public BaseAction {
public:
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual BaseAction* clone();
    virtual ~Watch()= default;///default destructor
};


class PrintActionsLog : public BaseAction {
public:
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual BaseAction* clone();
    virtual ~PrintActionsLog()= default;///default destructor
};

class Exit : public BaseAction {
public:
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual BaseAction* clone();
    virtual ~Exit()= default;///default destructor
};
#endif