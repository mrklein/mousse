#ifndef CORE_DB_IOSTREAMS_IOSTREAMS_IOMANIP_HPP_
#define CORE_DB_IOSTREAMS_IOSTREAMS_IOMANIP_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "istream.hpp"
#include "ostream.hpp"


namespace mousse {

// Forward declaration of friend functions and operators
template<class T> class Smanip;
template<class T> class Imanip;
template<class T> class Omanip;
template<class T>
inline Istream& operator>>(Istream& is, const Smanip<T>& m);
template<class T>
inline Ostream& operator<<(Ostream& os, const Smanip<T>& m);
template<class T>
inline Istream& operator>>(Istream& is, const Imanip<T>& m);
template<class T>
inline Ostream& operator<<(Ostream& os, const Omanip<T>& m);
template<class T>

class Smanip
{
  T (IOstream::*_fPtr)(const T);
  T _i;
public:
  Smanip(T (IOstream::*fPtr)(const T), const T i)
  :
    _fPtr{fPtr},
    _i{i}
  {}
  friend Istream& operator>> <T>(Istream& is, const Smanip<T>& m);
  friend Ostream& operator<< <T>(Ostream& os, const Smanip<T>& m);
};


template<class T>
inline Istream& operator>>(Istream& is, const Smanip<T>& m)
{
  (is.*m._fPtr)(m._i);
  return is;
}


template<class T>
inline Ostream& operator<<(Ostream& os, const Smanip<T>& m)
{
  (os.*m._fPtr)(m._i);
  return os;
}


template<class T>
class Imanip
{
  T (Istream::*_fPtr)(const T);
  T _i;
public:
  Imanip(T (Istream::*fPtr)(const T), const T i)
  :
    _fPtr{fPtr},
    _i{i}
  {}
  friend Istream& operator>> <T>(Istream& is, const Imanip<T>& m);
};


template<class T>
inline Istream& operator>>(Istream& is, const Imanip<T>& m)
{
  (is.*m._fPtr)(m._i);
  return is;
}


template<class T>
class Omanip
{
  T (Ostream::*_fPtr)(const T);
  T _i;
public:
  Omanip(T (Ostream::*fPtr)(const T), const T i)
  :
    _fPtr{fPtr},
    _i{i}
  {}
  friend Ostream& operator<< <T>(Ostream& os, const Omanip<T>& m);
};


template<class T>
inline Ostream& operator<<(Ostream& os, const Omanip<T>& m)
{
  (os.*m._fPtr)(m._i);
  return os;
}


inline Smanip<ios_base::fmtflags> setf
(
  const ios_base::fmtflags flags
)
{
  // return Smanip<ios_base::fmtflags>(&IOstream::setf, flags);
  return {&IOstream::setf, flags};
}


inline Omanip<IOstream::streamFormat> setformat
(
  const IOstream::streamFormat fmt
)
{
  // return Omanip<IOstream::streamFormat>(&IOstream::format, fmt);
  return {&IOstream::format, fmt};
}


inline Omanip<IOstream::versionNumber> setversion
(
  const IOstream::versionNumber ver
)
{
  // return Omanip<IOstream::versionNumber>(&IOstream::version, ver);
  return {&IOstream::version, ver};
}


inline Omanip<IOstream::compressionType> setcompression
(
  const IOstream::compressionType cmp
)
{
  // return Omanip<IOstream::compressionType>(&IOstream::compression, cmp);
  return {&IOstream::compression, cmp};
}


inline Omanip<int> setw(const int i)
{
  // return Omanip<int>(&Ostream::width, i);
  return {&Ostream::width, i};
}


inline Omanip<int> setprecision(const int i)
{
  // return Omanip<int>(&Ostream::precision, i);
  return {&Ostream::precision, i};
}

}  // namespace mousse
#endif
