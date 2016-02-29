#ifndef CORE_DB_IOSTREAMS_IOSTREAMS_INFO_PROXY_HPP_
#define CORE_DB_IOSTREAMS_IOSTREAMS_INFO_PROXY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::InfoProxy
// Description
//   A helper class for outputting values to Ostream
namespace mousse
{
class Ostream;
template<class T> class InfoProxy;
template<class T> Ostream& operator<<(Ostream&, const InfoProxy<T>&);
template<class T>
class InfoProxy
{
public:
  const T& t_;
  InfoProxy(const T& t)
  :
    t_(t)
  {}
  friend Ostream& operator<< <T>
  (Ostream&, const InfoProxy<T>&);
};
}  // namespace mousse
#endif
