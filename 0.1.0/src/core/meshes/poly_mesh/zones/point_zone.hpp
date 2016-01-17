// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointZone
// Description
//   A subset of mesh points.
//   The labels of points in the zone can be obtained from the addressing()
//   list.
//   For quick check whether a point belongs to the zone use the lookup
//   mechanism in pointZoneMesh, where all the zoned points are registered
//   with their zone number.
// SourceFiles
//   point_zone.cpp
//   point_zone_new.cpp

#ifndef point_zone_hpp_
#define point_zone_hpp_

#include "zone.hpp"
#include "point_zone_mesh_fwd.hpp"

namespace mousse
{

// Forward declaration of friend functions and operators
class pointZone;
Ostream& operator<<(Ostream&, const pointZone&);

class pointZone
:
  public zone
{
protected:

  // Protected data

    //- Reference to zone list
    const pointZoneMesh& zoneMesh_;

public:

  // Static data members

    //- The name associated with the zone-labels dictionary entry
    static const char * const labelsName;

  //- Runtime type information
  TYPE_NAME("pointZone");

  // Declare run-time constructor selection tables
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      pointZone,
      dictionary,
      (
        const word& name,
        const dictionary& dict,
        const label index,
        const pointZoneMesh& zm
      ),
      (name, dict, index, zm)
    );

  // Constructors

    //- Construct from components
    pointZone
    (
      const word& name,
      const labelUList& addr,
      const label index,
      const pointZoneMesh&
    );

    //- Construct from components, transferring contents
    pointZone
    (
      const word& name,
      const Xfer<labelList>& addr,
      const label index,
      const pointZoneMesh&
    );

    //- Construct from dictionary
    pointZone
    (
      const word& name,
      const dictionary&,
      const label index,
      const pointZoneMesh&
    );

    //- Construct given the original zone and resetting the
    //  point list and zone mesh information
    pointZone
    (
      const pointZone&,
      const labelUList& addr,
      const label index,
      const pointZoneMesh&
    );

    //- Construct given the original zone, resetting the
    //  face list and zone mesh information
    pointZone
    (
      const pointZone&,
      const Xfer<labelList>& addr,
      const label index,
      const pointZoneMesh&
    );

    //- Construct and return a clone, resetting the zone mesh
    virtual autoPtr<pointZone> clone(const pointZoneMesh& zm) const
    {
      return autoPtr<pointZone>
      {
        new pointZone{*this, *this, index(), zm}
      };
    }

    //- Construct and return a clone, resetting the point list
    //  and zone mesh
    virtual autoPtr<pointZone> clone
    (
      const pointZoneMesh& zm,
      const label index,
      const labelUList& addr
    ) const
    {
      return autoPtr<pointZone>
      {
        new pointZone{*this, addr, index, zm}
      };
    }

    //- Disallow default bitwise copy construct
    pointZone(const pointZone&) = delete;

  // Selectors

    //- Return a pointer to a new point zone
    //  created on freestore from dictionary
    static autoPtr<pointZone> New
    (
      const word& name,
      const dictionary&,
      const label index,
      const pointZoneMesh&
    );

  //- Destructor
  virtual ~pointZone();

  // Member Functions

    //- Return zoneMesh reference
    const pointZoneMesh& zoneMesh() const;

    //- Helper function to re-direct to zone::localID(...)
    label whichPoint(const label globalPointID) const;

    //- Check zone definition. Return true if in error.
    virtual bool checkDefinition(const bool report = false) const;

    //- Check whether zone is synchronised across coupled boundaries. Return
    //  true if in error.
    virtual bool checkParallelSync(const bool report = false) const;

    //- Correct patch after moving points
    virtual void movePoints(const pointField&)
    {}

    //- Write dictionary
    virtual void writeDict(Ostream&) const;

  // Member Operators

    //- Assign to zone, clearing demand-driven data
    void operator=(const pointZone&);

    //- Assign addressing, clearing demand-driven data
    void operator=(const labelUList&);

    //- Assign addressing, clearing demand-driven data
    void operator=(const Xfer<labelList>&);

  // I-O

    //- Ostream Operator
    friend Ostream& operator<<(Ostream&, const pointZone&);

};

}  // namespace mousse
#endif
