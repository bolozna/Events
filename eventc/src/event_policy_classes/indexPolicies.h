//
// Created by Rainer Kujala on 28/09/16.
//

#ifndef EVENTC_INDEXPOLICIES_H
#define EVENTC_INDEXPOLICIES_H

#define NOINDEX 0

/** Policy classes for storing index. **/

#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include "../event.h"

#define eventindex unsigned




class Index{
private:
    eventindex index;
public:
    eventindex getIndex();
    void setIndex(eventindex index);
    Index();
    Index( const Index &event);
    string getLongIndexStr();
    string getShortIndexStr();
    static const string indexTypeStr;
protected:
    void readIndex(ifstream &the_file);
    void writeIndex(ofstream &the_file);
    void readIndexConditional(ifstream &the_file);
    void writeIndexConditional(ofstream &the_file);
    bool isEqualIndex(const Index & other) const {return this->index==other.index;}
};
const string Index::indexTypeStr=string("index");



eventindex Index::getIndex(){
    return this->index;
}
void Index::readIndex(ifstream &the_file){
    the_file >> this->index;
}
void Index::writeIndex(ofstream &the_file){
    the_file << this->index;
}
void Index::readIndexConditional(ifstream &the_file){
    the_file >> this->index;
}
void Index::writeIndexConditional(ofstream &the_file){
    the_file<< " " << this->index;
}
Index::Index(){
}
Index::Index( const Index &event){
    this->index=event.index;
}

void Index::setIndex(eventindex index){
    this->index=index;
}

string Index::getLongIndexStr(){
    stringstream s;
    s << this->index;
    return string("index: ")+s.str();
}
string Index::getShortIndexStr(){
    stringstream s;
    s << this->index;
    return s.str();
}


class NoIndex{
public:
    eventindex getIndex() const;
    string getLongIndexStr() const;
    string getShortIndexStr() const;
    static const string indexTypeStr;
    void setIndex(int index);
    NoIndex();
    NoIndex( const Index &event);
protected:
    void readIndex(ifstream &the_file);
    void writeIndex(ofstream &the_file);
    void readIndexConditional(ifstream &the_file){}
    void writeIndexConditional(ofstream &the_file){}
    static bool isEqualIndex(const NoIndex & other) {return true;}
};
const string NoIndex::indexTypeStr=string("");

void NoIndex::readIndex(ifstream &the_file){
    eventindex dummy;
    the_file >> dummy;
}
void NoIndex::writeIndex(ofstream &the_file){
    the_file << "NaN";
}
eventindex NoIndex::getIndex() const{
    return NOINDEX;
}
string NoIndex::getLongIndexStr() const{
    return "no index";
}
string NoIndex::getShortIndexStr() const{
    return "";
}
NoIndex::NoIndex(){
}
NoIndex::NoIndex(const Index &event){
}
void NoIndex::setIndex(int index){
}

#endif //EVENTC_INDEXPOLICIES_H
