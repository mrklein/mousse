#ifndef CORE_MESHES_POLY_MESH_MAP_POLY_MESH_OBJECT_MAP_HPP_
#define CORE_MESHES_POLY_MESH_MAP_POLY_MESH_OBJECT_MAP_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::objectMap
// Description
//   An objectMap is a pair of labels defining the mapping of an object from
//   another object, e.g. a cell mapped from a point.

#include "label_list.hpp"
#include "iostreams.hpp"


namespace mousse {

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


namespace mousse {

// Constructors 
inline objectMap::objectMap()
:
  index_{-1},
  masterObjects_{0}
{}


inline objectMap::objectMap(const label index, const labelList& master)
:
  index_{index},
  masterObjects_{master}
{}


inline objectMap::objectMap(Istream& is)
{
  // Read beginning of objectMap
  is.readBegin("objectMap");
  is >> index_ >> static_cast<labelList&>(masterObjects_);
  // Read master of objectMap
  is.readEnd("objectMap");
  // Check state of Istream
  is.check("objectMap::objectMap(Istream&)");
}


// Member Functions 
label& objectMap::index()
{
  return index_;
}


inline label objectMap::index() const
{
  return index_;
}


inline labelList& objectMap::masterObjects()
{
  return masterObjects_;
}


inline const labelList& objectMap::masterObjects() const
{
  return masterObjects_;
}


// Friend Operators 
inline bool operator==(const objectMap& a, const objectMap& b)
{
  return
  (
    (a.index_ == b.index_) && (a.masterObjects_ == b.masterObjects_)
  );
}


inline bool operator!=(const objectMap& a, const objectMap& b)
{
  return (!(a == b));
}


// Ostream Operator
inline Ostream& operator<<(Ostream& os, const objectMap& a)
{
  os<< token::BEGIN_LIST
    << a.index_ << token::SPACE
    << a.masterObjects_
    << token::END_LIST;
  // Check state of Ostream
  os.check("Ostream& operator<<(Ostream&, const objectMap&)");
  return os;
}


inline Istream& operator>>(Istream& is, objectMap& a)
{
  is.readBegin("objectMap");
  is  >> a.index_ >> a.masterObjects_;
  is.readEnd("objectMap");
  // Check state of Istream
  is.check("Istream& operator>>(Istream&, objectMap&)");
  return is;
}

}  // namespace mousse

#endif
