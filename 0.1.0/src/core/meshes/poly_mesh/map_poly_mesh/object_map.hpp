// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::objectMap
// Description
//   An objectMap is a pair of labels defining the mapping of an object from
//   another object, e.g. a cell mapped from a point.
// SourceFiles
//   object_map_i.hpp
#ifndef object_map_hpp_
#define object_map_hpp_
#include "label_list.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
class objectMap;
inline bool operator==(const objectMap& a, const objectMap& b);
inline bool operator!=(const objectMap& a, const objectMap& b);
inline Ostream& operator<<(Ostream&, const objectMap&);
inline Istream& operator>>(Istream&, objectMap&);
class objectMap
{
  // Private data
    //- Object index
    label index_;
    //- Master object index
    labelList masterObjects_;
public:
  // Constructors
    //- Null constructor for lists
    inline objectMap();
    //- Construct from components
    inline objectMap(const label index, const labelList& master);
    //- Construct from Istream
    inline objectMap(Istream&);
  // Member Functions
    //- Return object index
    inline label& index();
    inline label index() const;
    //- Return master object index
    inline labelList& masterObjects();
    inline const labelList& masterObjects() const;
  // Friend Operators
    friend bool operator==(const objectMap& a, const objectMap& b);
    friend bool operator!=(const objectMap& a, const objectMap& b);
  // IOstream Operators
    friend Ostream& operator<<(Ostream&, const objectMap&);
    friend Istream& operator>>(Istream&, objectMap&);
};
}  // namespace mousse
#include "object_map_i.hpp"
#endif
