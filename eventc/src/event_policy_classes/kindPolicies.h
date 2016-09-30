//
// Created by Rainer Kujala on 28/09/16.
//

#ifndef EVENTC_KINDPOLICIES_H
#define EVENTC_KINDPOLICIES_H

#define kind_t int

#include "../event.h"

using namespace std;
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>

/*
 *  Policy classes for storing event categories (or types).
 *  The word "kind" is used instead of "type" in order to avoid confusion.
 */


class Kind{
private:
    kind_t kind;
public:
    kind_t getKind();
    void setKind(kind_t kind);
    Kind();
    Kind( const Kind &other);
    string getLongKindStr();
    string getShortKindStr();
    static const string kindTypeStr;
protected:
    void readKind(ifstream &the_file);
    void writeKind(ofstream &the_file);
    void readKindConditional(ifstream &the_file);
    void writeKindConditional(ofstream &the_file);
    bool isEqualKind(const Kind & other) const {return this->kind==other.kind;}
};
const string Kind::kindTypeStr=string("kind");



kind_t Kind::getKind(){
    return this->kind;
}
void Kind::readKind(ifstream &the_file){
    the_file >> this->kind;
}
void Kind::writeKind(ofstream &the_file){
    the_file << this->kind;
}
void Kind::readKindConditional(ifstream &the_file){
    the_file >> this->kind;
}
void Kind::writeKindConditional(ofstream &the_file){
    the_file<< " " << this->kind;
}
Kind::Kind(){
}

Kind::Kind( const Kind &other){
    this->kind=other.kind;
}

void Kind::setKind(kind_t kind){
    this->kind=kind;
}

string Kind::getLongKindStr(){
    stringstream s;
    s << this->kind;
    return string("kind: ")+s.str();
}
string Kind::getShortKindStr(){
    stringstream s;
    s << this->kind;
    return s.str();
}


class NoKind{
public:
    kind_t getKind() const;
    string getLongKindStr() const;
    string getShortKindStr() const;
    static const string kindTypeStr;
    void setKind(int kind);
    NoKind();
    NoKind( const Kind &event);
protected:
    void readKind(ifstream &the_file);
    void writeKind(ofstream &the_file);
    void readKindConditional(ifstream &the_file){}
    void writeKindConditional(ofstream &the_file){}
    static bool isEqualKind(const NoKind & other) {return true;}
};
const string NoKind::kindTypeStr=string("");

void NoKind::readKind(ifstream &the_file){
    kind_t dummy;
    the_file >> dummy;
}
void NoKind::writeKind(ofstream &the_file){
    the_file << "NaN";
}
kind_t NoKind::getKind() const{
    return NOINDEX;
}
string NoKind::getLongKindStr() const{
    return "no kind";
}
string NoKind::getShortKindStr() const{
    return "";
}
NoKind::NoKind(){
}
NoKind::NoKind(const Kind &event){
}
void NoKind::setKind(int kind){
}



#endif // EVENTC_KINDPOLICIES_H
