#ifndef EVENTC_DURATIONPOLICIES_H
#define EVENTC_DURATIONPOLICIES_H

#define DEFAULT_DURATION 0

#include "../event.h"

using namespace std;

/** Policy classes for storing durations. **/
class Duration{
 private:
  timestamp duration;
 protected:
  void readDuration(basic_ifstream<char> &the_file);
  void writeDuration(basic_ofstream<char> &the_file);
  void readDurationConditional(basic_ifstream<char> &the_file);
  void writeDurationConditional(basic_ofstream<char> &the_file);
  bool isEqualDuration(const Duration & other) const {return this->duration==other.duration;}
 public:
  timestamp getDuration() const;
  void setDuration(timestamp duration);
  Duration();
  Duration( const Duration &event);
  basic_string<char, char_traits<char>, allocator<char> > getLongDurationStr();
  basic_string<char, char_traits<char>, allocator<char> > getShortDurationStr();
  static const basic_string<char, char_traits<char>, allocator<char> > durationTypeStr;
};

class NoDuration{
 public:
  timestamp getDuration() const;
  void setDuration(timestamp duration);
  NoDuration();
  NoDuration( const NoDuration &event);
  basic_string<char, char_traits<char>, allocator<char> > getLongDurationStr();
  basic_string<char, char_traits<char>, allocator<char> > getShortDurationStr();
  static const basic_string<char, char_traits<char>, allocator<char> > durationTypeStr;
 protected:
  void readDuration(basic_ifstream<char> &the_file);
  void writeDuration(basic_ofstream<char> &the_file);
  void readDurationConditional(basic_ifstream<char> &the_file){}
  void writeDurationConditional(basic_ofstream<char> &the_file){}
  static bool isEqualDuration(const NoDuration & other) {return true;}
};

void Duration::readDuration(basic_ifstream<char> &the_file){
  the_file >> this->duration;
}

void Duration::writeDuration(basic_ofstream<char> &the_file){
  the_file << this->duration;
}

void Duration::readDurationConditional(basic_ifstream<char> &the_file){
  the_file >> this->duration;
}

void Duration::writeDurationConditional(basic_ofstream<char> &the_file){
  the_file<< " " << this->duration;
}

timestamp Duration::getDuration() const{
  return this->duration;
}

Duration::Duration(){
}

Duration::Duration(const Duration &event){
  this->duration=event.duration;
}

void Duration::setDuration(timestamp duration){
  this->duration=duration;
}

const string Duration::durationTypeStr = basic_string<char, char_traits<char>, allocator<char> >("duration");





basic_string<char, char_traits<char>, allocator<char> > Duration::getLongDurationStr(){
  basic_stringstream<char> s;
  s << this->duration;
  return basic_string<char, char_traits<char>, allocator<char> > ("duration: ") + s.str();
}

basic_string<char, char_traits<char>, allocator<char> > Duration::getShortDurationStr(){
  basic_stringstream<char> s;
  s << this->duration;
  return s.str();
}

timestamp NoDuration::getDuration() const{
  return DEFAULT_DURATION;
}

void NoDuration::readDuration(basic_ifstream<char> &the_file){
  timestamp dummy;
  the_file >> dummy;
}

void NoDuration::writeDuration(basic_ofstream<char> &the_file){
  the_file << "NaN";
}

NoDuration::NoDuration(){
}

NoDuration::NoDuration(const NoDuration &event){
}

void NoDuration::setDuration(timestamp duration){
}

basic_string<char, char_traits<char>, allocator<char> > NoDuration::getLongDurationStr(){
  return basic_string<char, char_traits<char>, allocator<char> > ("no duration");
}

basic_string<char, char_traits<char>, allocator<char> > NoDuration::getShortDurationStr(){
  return basic_string<char, char_traits<char>, allocator<char> > ("");
}

const string NoDuration::durationTypeStr = basic_string<char, char_traits<char>, allocator<char> >("");


#endif // EVENTC_DURATIONPOLICIES_H
