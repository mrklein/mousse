#ifndef CORE_PRIMITIVES_NIL_NIL_HPP_
#define CORE_PRIMITIVES_NIL_NIL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::nil
// Description
//   A zero-sized class without any storage. Used, for example, in HashSet.
// Note
//   A zero-sized class actually does still require at least 1 byte storage.


namespace mousse {

// Forward declaration of classes
class Istream;
class Ostream;

// Forward declaration of friend functions and operators
class nil;
Istream& operator>>(Istream&, nil&);
Ostream& operator<<(Ostream&, const nil&);


class nil
{
public:
  // Constructors
    //- Construct null
    nil()
    {}
    //- Construct from Istream
    nil(Istream&)
    {}
  // IOstream Operators
    friend Istream& operator>>(Istream& is, nil&)
    {
      return is;
    }
    friend Ostream& operator<<(Ostream& os, const nil&)
    {
      return os;
    }
};

}  // namespace mousse

#endif
