# Project Name
> SPLFLIX Streaming-service-system

## Table of contents
* [General info](#general-info)
* [Technologies](#technologies)
* [Launch](#launch)
* [Goals](#Goals)
* [Inspiration](#inspiration)
* [Contact](#contact)

## General info
A C++ program that simulates a new streaming service -
SPLFLIX. SPLFLIX offers users two types of streaming content – movies and tv-episodes,
which belong to a given tv-series. Like most
streaming services we allow a creation of
multiple users, while each user receives userspecific recommendations, according to his
watch history.
However, on SPLFLIX, unlike current
streaming services, we give our users the
ability to choose which recommendation
algorithm to use, out of a number of algorithms
we offer. Each user stores its own watch history, and can use a different recommendation
algorithm. After the user chooses to watch a content, a recommendation can be given to the
user for the next content.

## Technologies
* C++ Standerd version.


## Launch
Choose the config file you would like to run, and change it's name to config.json.</br> 
There are two options available for you to choose from, that also can be used as tamplates to make a new config file.</br>
#### In Windows open the cmd in the project directory and run the next commend:</br>
#### Compile
> g++ -g -Wall -c -Include src/main.cpp src/Action.cpp src/Session.cpp src/User.cpp src/Watchable.cpp </br>
#### Build
> g++ -o splflix.exe src/main.cpp src/Action.cpp src/Session.cpp src/User.cpp src/Watchable.cpp
#### Run
>  splflix.exe config.json </br> 
#### In Linux open the terminal in the project directory and run the next commend:</br>
#### Compile&Build
> make </br>
#### Run
> ./bin/splflix config.json 

#### Input explanation
After you start SPLFLIX in the cmd\terminal, the program will print a message "SPLFLIX is now on", afterwards the program would accept any of the next input text below:</br>
Syntax: createuser <user_name> <recommendation_algorithm>, Where the recommendation_algorithm is one of the next 3 options:</br>
* gen - recommendation by genere
* len - recommendation by length of video
* rer - rewatch using the user watch history.</br>
After creating a user you could use any of the next commends:</br>
* Change Active User– Changes the current active user.
	* Syntax: changeuser <user_name>
* Delete User – Removes a given user
	* Syntax: deleteuser <user_name>
* Duplicate User – Creates a copy of the given user, with its watch history and recommendation algorithm, but with a new name.
	* Syntax: dupuser <original_user_name> <new_user_name>
* Print Content List – Prints all the watchable content available. 
	* Syntax: content
* Print Watch History – Prints the watch history of the current active user.
	* Syntax: watchhist
* Watch – Watches a content.
	* Syntax: watch <content_id>
	* <content_id> is the number of the content in the config file for exemple: Finding Nemo has id "2" in config2.json file provided in the project folder.
* Print Actions Log – Prints all the actions that were performed by all users (Excluding current log action), from the last one to the first.
	* Syntax: log
* Exit – Exits SPLFLIX.
#### Output explanation
You have Output files provided to you in 2 different folders as an exemple to what should be expected.</br>
the folder file in which the Output file resieds in tells you which config file you should use, and inside you also have the input needed to receive this exect output.
	



## Goals
The goal of this project is to design an object-oriented system and gain
implementation experience in C++ while using classes, standard data structures and unique
C++ properties while maintainning a progrem without memory leaks.  

## Inspiration
As part of our System Programing course we received this project as an assignment

## Contact
Created by [Itamar Lederman](https://github.com/Itamarled/) & [Shimi Nagar](https://github.com/Shimonna394)