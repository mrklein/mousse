#ifndef SURF_MESH_SURF_ZONE_SURF_ZONE_IDENTIFIER_HPP_
#define SURF_MESH_SURF_ZONE_SURF_ZONE_IDENTIFIER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::surfZoneIdentifier
// Description
//   An identifier for a surface zone on a meshed surface.
//   Similar in concept to a faceZone on the surface, but can also have a
//   "geometricType" as well.
// SourceFiles
//   surf_zone_identifier.cpp
#include "word.hpp"
#include "label.hpp"
#include "type_info.hpp"
namespace mousse
{
class dictionary;
// Forward declaration of friend functions and operators
class surfZoneIdentifier;
Ostream& operator<<(Ostream&, const surfZoneIdentifier&);
class surfZoneIdentifier
{
  // Private data
    //- Name of zone
    word name_;
    //- Index of zone in surface mesh
    label index_;
    //- Type name of zone
    mutable word geometricType_;
public:
  // Constructors
    //- Construct null
    surfZoneIdentifier();
    //- Construct from components
    surfZoneIdentifier
    (
      const word& name,
      const label index,
      const word& geometricType = word::null
    );
    //- Construct from dictionary
    surfZoneIdentifier
    (
      const word& name,
      const dictionary&,
      const label index
    );
    //- Construct from another zone identifier, resetting the index
    surfZoneIdentifier
    (
      const surfZoneIdentifier&,
      const label index
    );
  //- Destructor
  virtual ~surfZoneIdentifier();
  // Member Functions
    //- Return name
    const word& name() const
    {
      return name_;
    }
    //- Return name for modification
    word& name()
    {
      return name_;
    }
    //- Return the geometric type of the zone
    const word& geometricType() const
    {
      return geometricType_;
    }
    //- Return the geometric type of the zone for modification
    word& geometricType()
    {
      return geometricType_;
    }
    //- Return the index of this zone in the surface mesh
    label index() const
    {
      return index_;
    }
    //- Write surfZoneIdentifier as a dictionary
    void write(Ostream&) const;
    //- Write surfZoneIdentifier as a dictionary
//        void writeDict(Ostream&) const;
  // Member Operators
//        bool operator!=(const surfZoneIdentifier&) const;
//
//        //- compare.
//        bool operator==(const surfZoneIdentifier&) const;
  // Ostream Operator
    friend Ostream& operator<<(Ostream&, const surfZoneIdentifier&);
//        friend Istream& operator>>(Istream&, surfZoneIdentifier&);
};
}  // namespace mousse
#endif
