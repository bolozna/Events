//
// Created by Rainer Kujala on 28/09/16.
//

#ifndef EVENTC_REVERSEPOLICIES_H
#define EVENTC_REVERSEPOLICIES_H

#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>

#include "../event.h"



using namespace std;

/** Policy classes for storing reversed information. **/
class Reverse{
private:
    bool reversed;
protected:
    void readReversed(ifstream &the_file);
    void writeReversed(ofstream &the_file);
    void readReversedConditional(ifstream &the_file);
    void writeReversedConditional(ofstream &the_file);
    bool isEqualReversed(const Reverse & other) const {return this->reversed==other.reversed;}
public:
    void toggleReversed();
    void setReversed(const bool reversed);
    bool isReversed();
    Reverse();
    Reverse( const Reverse &event);
    string getLongReverseStr();
    string getShortReverseStr();
    static const string reverseTypeStr;
};
const string Reverse::reverseTypeStr=string("reverse");

void Reverse::readReversed(ifstream &the_file){
    the_file >> this->reversed;
}
void Reverse::writeReversed(ofstream &the_file){
    the_file << this->reversed;
}
void Reverse::readReversedConditional(ifstream &the_file){
    the_file >> this->reversed;
}
void Reverse::writeReversedConditional(ofstream &the_file){
    the_file << " " << this->reversed;
}

void Reverse::toggleReversed(){
    this->reversed = !this->reversed;
}

void Reverse::setReversed(const bool reversed){
    this->reversed = reversed;
}
bool Reverse::isReversed(){
    return this->reversed;
}
Reverse::Reverse(){
    this->reversed=false;
}
Reverse::Reverse(const Reverse &event){
    this->reversed=event.reversed;
}
string Reverse::getLongReverseStr(){
    if(this->reversed){
        return string("reversed: true");
    }else{
        return string("reversed: false");
    }
}
string Reverse::getShortReverseStr(){
    if(this->reversed){
        return string("true");
    }else{
        return string("false");
    }
}


class NoReverse{
protected:
    void toggleReversed();
    void readReversed(ifstream &the_file);
    void writeReversed(ofstream &the_file);
    void readReversedConditional(ifstream &the_file){}
    void writeReversedConditional(ofstream &the_file){}
    static bool isEqualReversed(const NoReverse & other) {return true;}
public:
    bool isReversed();
    NoReverse();
    NoReverse( const NoReverse &event);
    string getLongReverseStr();
    string getShortReverseStr();
    static const string reverseTypeStr;
};
const string NoReverse::reverseTypeStr=string("");

void NoReverse::readReversed(ifstream &the_file){
    timestamp dummy;
    the_file >> dummy;
}
void NoReverse::writeReversed(ofstream &the_file){
    the_file << "0"; //Always false
}

void NoReverse::toggleReversed(){
    cout << "Warning: trying to toggle reversed status of an event that cannot be reversed!" <<endl;
} //this should not be called
bool NoReverse::isReversed(){
    return false;
}
NoReverse::NoReverse(){
}
NoReverse::NoReverse(const NoReverse &event){
}
string NoReverse::getLongReverseStr(){
    return string("no reversing");
}
string NoReverse::getShortReverseStr(){
    return string("");
}

#endif //EVENTC_REVERSEPOLICIES_H
