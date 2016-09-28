#ifndef EVENTC_TIMESTAMPPOLICIES_H
#define EVENTC_TIMESTAMPPOLICIES_H

#define NOTIME 0

#include "../event.h"

#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

/** Policy classes for storing (or not storing) time stamps. **/
class TimeStamp{
 private:
  timestamp time;
 protected:
  void readTime(basic_ifstream<char> &the_file);
  void writeTime(basic_ofstream<char> &the_file);
  void readTimeConditional(basic_ifstream<char> &the_file);
  void writeTimeConditional(basic_ofstream<char> &the_file);
  bool isEqualTime(const TimeStamp & other) const {return this->time==other.time;}
 public:
  timestamp getTime() const;
  void setTime(timestamp time);
  TimeStamp();
  TimeStamp( const TimeStamp &event);
  basic_string<char, char_traits<char>, allocator<char>> getLongTimeStr();
  basic_string<char, char_traits<char>, allocator<char>> getShortTimeStr();
  static const basic_string<char, char_traits<char>, allocator<char>> timeTypeStr;
};

class NoTimeStamp{
 public:
  timestamp getTime() const;
  void setTime(timestamp time);
  NoTimeStamp();
  NoTimeStamp( const NoTimeStamp &event);
  basic_string<char, char_traits<char>, allocator<char>> getLongTimeStr();
  basic_string<char, char_traits<char>, allocator<char>> getShortTimeStr();
  static const basic_string<char, char_traits<char>, allocator<char>> timeTypeStr;
 protected:
  void readTime(basic_ifstream<char> &the_file);
  void writeTime(basic_ofstream<char> &the_file);
  void readTimeConditional(basic_ifstream<char> &the_file){}
  void writeTimeConditional(basic_ofstream<char> &the_file){}
  static bool isEqualTime(const NoTimeStamp & other) {return true;}
};

const string TimeStamp::timeTypeStr = basic_string<char, char_traits<char>, allocator<char>>("time");
const string NoTimeStamp::timeTypeStr= basic_string < char, char_traits<char>, allocator<char>>("");

timestamp TimeStamp::getTime() const{
  return this->time;
}

void TimeStamp::setTime(timestamp time){
  this->time=time;
}

void TimeStamp::readTime(basic_ifstream<char> &the_file){
  the_file >> this->time;
}

void TimeStamp::writeTime(basic_ofstream<char> &the_file){
  the_file << this->time;
}

void TimeStamp::readTimeConditional(basic_ifstream<char> &the_file){
  the_file >> this->time;
}

void TimeStamp::writeTimeConditional(basic_ofstream<char> &the_file){
  the_file<< " " << this->time;
}

TimeStamp::TimeStamp(){
}

TimeStamp::TimeStamp(const TimeStamp &event){
  this->time=event.time;
}

basic_string<char, char_traits<char>, allocator<char>> TimeStamp::getLongTimeStr(){//convert TimeStamp time into string to print it
  basic_stringstream<char> s;
  s << this->time;
  return basic_string<char, char_traits<char>, allocator<char >> ("time: ") + s.str();
}

basic_string<char, char_traits<char>, allocator<char>> TimeStamp::getShortTimeStr(){
  basic_stringstream<char> s;
  s << this->time;
  return s.str();
}

timestamp NoTimeStamp::getTime() const{
  return NOTIME;
}

void NoTimeStamp::setTime(timestamp time){
}

void NoTimeStamp::readTime(basic_ifstream<char> &the_file){
  timestamp dummy;
  the_file >> dummy;
}

void NoTimeStamp::writeTime(basic_ofstream<char> &the_file){
  the_file << "NaN";
}

NoTimeStamp::NoTimeStamp(){
}

NoTimeStamp::NoTimeStamp(const NoTimeStamp &event){
}

basic_string<char, char_traits<char>, allocator<char>> NoTimeStamp::getLongTimeStr(){
  return basic_string<char, char_traits<char>, allocator<char >> ("no time");
}

basic_string<char, char_traits<char>, allocator<char>> NoTimeStamp::getShortTimeStr(){
  return basic_string<char, char_traits<char>, allocator<char >> ("");
}



#endif //EVENTC_TIMESTAMPPOLICIES_H
