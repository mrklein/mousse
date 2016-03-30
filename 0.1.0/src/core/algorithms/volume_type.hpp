#ifndef CORE_ALGORITHMS_VOLUME_TYPE_HPP_
#define CORE_ALGORITHMS_VOLUME_TYPE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::volumeType
// Description
// SourceFiles
//   volume_type.cpp

#include "named_enum.hpp"
#include "contiguous.hpp"


namespace mousse {

// Forward declaration of friend functions and operators
class volumeType;
Istream& operator>>(Istream& is, volumeType&);
Ostream& operator<<(Ostream& os, const volumeType& C);

class volumeType
{
public:
  //- Volume types
  enum type
  {
    UNKNOWN = 0,
    MIXED   = 1,
    INSIDE  = 2,
    OUTSIDE = 3
  };
private:
  // Private data
    //- Volume type
    type t_;
public:
  // Static data
    static const NamedEnum<volumeType, 4> names;
  // Constructors
    //- Construct null
    volumeType()
    :
      t_{UNKNOWN}
    {}
    //- Construct from components
    volumeType(type t)
    :
      t_{t}
    {}
  // Member Functions
    operator type() const
    {
      return t_;
    }
  // IOstream operators
    friend Istream& operator>>(Istream& is, volumeType& vt);
    friend Ostream& operator<<(Ostream& os, const volumeType& vt);
};

//- Data associated with volumeType type are contiguous
template<>
inline bool contiguous<volumeType>() {return true;}

}  // namespace mousse

#endif
