// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::INew
// Description
//   A helper class when constructing from an Istream or dictionary
#ifndef inew_hpp_
#define inew_hpp_
#include "auto_ptr.hpp"
namespace mousse
{
// Forward declaration of classes
class Istream;
template<class T>
class INew
{
public:
  //- Construct null
  INew()
  {}
  //- Construct from Istream
  autoPtr<T> operator()(Istream& is) const
  {
    return T::New(is);
  }
  //- Construct from word and Istream
  autoPtr<T> operator()(const word&, Istream& is) const
  {
    return T::New(is);
  }
  //- Construct from dictionary
  autoPtr<T> operator()(const dictionary& dict) const
  {
    return T::New(dict);
  }
  //- Construct from word and dictionary
  autoPtr<T> operator()(const word&, const dictionary& dict) const
  {
    return T::New(dict);
  }
};
}  // namespace mousse
#endif
