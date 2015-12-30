// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::surfZone
// Description
//   A surface zone on a MeshedSurface.
//   Similar in concept to a faceZone, but the face list is contiguous.
// SourceFiles
//   surf_zone.cpp
#ifndef surf_zone_hpp_
#define surf_zone_hpp_
#include "word.hpp"
#include "label.hpp"
#include "class_name.hpp"
#include "surf_zone_identifier.hpp"
#include "auto_ptr.hpp"
#include "dictionary.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
class surfZone;
Istream& operator>>(Istream&, surfZone&);
Ostream& operator<<(Ostream&, const surfZone&);
class surfZone
:
  public surfZoneIdentifier
{
  // Private data
    //- Size of this group in the face list
    label size_;
    //- Start label of this group in the face list
    label start_;
public:
  //- Runtime type information
  ClassName("surfZone");
  // Constructors
    //- Construct null
    surfZone();
    //- Construct from components
    surfZone
    (
      const word& name,
      const label size,
      const label start,
      const label index,
      const word& geometricType = word::null
    );
    //- Construct from Istream
    surfZone(Istream& is, const label index);
    //- Construct from dictionary
    surfZone
    (
      const word& name,
      const dictionary& dict,
      const label index
    );
    //- Construct as copy
    surfZone(const surfZone&);
    //- Construct from another zone, resetting the index
    surfZone(const surfZone&, const label index);
    //- Return clone
    autoPtr<surfZone> clone() const
    {
      notImplemented("autoPtr<surfZone> clone() const");
      return autoPtr<surfZone>(NULL);
    }
    static autoPtr<surfZone> New(Istream& is)
    {
      word name(is);
      dictionary dict(is);
      return autoPtr<surfZone>(new surfZone(name, dict, 0));
    }
  // Member Functions
    //- Return start label of this zone in the face list
    label start() const
    {
      return start_;
    }
    //- Return start label of this zone in the face list
    label& start()
    {
      return start_;
    }
    //- Return size of this zone in the face list
    label size() const
    {
      return size_;
    }
    //- Return size of this zone in the face list
    label& size()
    {
      return size_;
    }
    //- Write
    void write(Ostream&) const;
    //- Write dictionary
    void writeDict(Ostream&) const;
  // Member Operators
    bool operator!=(const surfZone&) const;
    //- compare.
    bool operator==(const surfZone&) const;
  // IOstream Operators
    friend Istream& operator>>(Istream&, surfZone&);
    friend Ostream& operator<<(Ostream&, const surfZone&);
};
}  // namespace mousse
#endif
