// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cellZone
// Description
//   A subset of mesh cells.
//   Currently set up as an indirect list but will be extended to use a
//   primitive mesh.  For quick check whether a cell belongs to the zone use
//   the lookup mechanism in cellZoneMesh, where all the zoned cells are
//   registered with their zone number.
// SourceFiles
//   cell_zone.cpp
//   cell_zone_new.cpp

#ifndef cell_zone_hpp_
#define cell_zone_hpp_

#include "zone.hpp"
#include "cell_zone_mesh_fwd.hpp"

namespace mousse
{

// Forward declaration of friend functions and operators
class cellZone;
Ostream& operator<<(Ostream&, const cellZone&);

class cellZone
:
  public zone
{
protected:

  // Protected data

    //- Reference to zone list
    const cellZoneMesh& zoneMesh_;

public:

  // Static data members

    //- The name associated with the zone-labels dictionary entry
    static const char * const labelsName;

  //- Runtime type information
  TYPE_NAME("cellZone");

  // Declare run-time constructor selection tables
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      cellZone,
      dictionary,
      (
        const word& name,
        const dictionary& dict,
        const label index,
        const cellZoneMesh& zm
      ),
      (name, dict, index, zm)
    );

  // Constructors

    //- Construct from components
    cellZone
    (
      const word& name,
      const labelUList& addr,
      const label index,
      const cellZoneMesh&
    );

    //- Construct from components, transferring contents
    cellZone
    (
      const word& name,
      const Xfer<labelList>& addr,
      const label index,
      const cellZoneMesh&
    );

    //- Construct from dictionary
    cellZone
    (
      const word& name,
      const dictionary&,
      const label index,
      const cellZoneMesh&
    );

    //- Construct given the original zone and resetting the
    //  cell list and zone mesh information
    cellZone
    (
      const cellZone&,
      const labelUList& addr,
      const label index,
      const cellZoneMesh&
    );

    //- Construct given the original zone, resetting the
    //  cell list and zone mesh information
    cellZone
    (
      const cellZone&,
      const Xfer<labelList>& addr,
      const label index,
      const cellZoneMesh&
    );

    //- Construct and return a clone, resetting the zone mesh
    virtual autoPtr<cellZone> clone(const cellZoneMesh& zm) const
    {
      return autoPtr<cellZone>
      (
        new cellZone(*this, *this, index(), zm)
      );
    }

    //- Construct and return a clone, resetting the cell list
    //  and zone mesh
    virtual autoPtr<cellZone> clone
    (
      const labelUList& addr,
      const label index,
      const cellZoneMesh& zm
    ) const
    {
      return autoPtr<cellZone>
      (
        new cellZone(*this, addr, index, zm)
      );
    }

    //- Disallow default bitwise copy construct
    cellZone(const cellZone&) = delete;

  // Selectors

    //- Return a pointer to a new cell zone
    //  created on freestore from dictionary
    static autoPtr<cellZone> New
    (
      const word& name,
      const dictionary&,
      const label index,
      const cellZoneMesh&
    );

  //- Destructor
  virtual ~cellZone();

  // Member Functions

    //- Helper function to re-direct to zone::localID(...)
    label whichCell(const label globalCellID) const;

    //- Return zoneMesh reference
    const cellZoneMesh& zoneMesh() const;

    //- Check zone definition. Return true if in error.
    virtual bool checkDefinition(const bool report = false) const;

    //- Check whether zone is synchronised across coupled boundaries. Return
    //  true if in error.
    virtual bool checkParallelSync(const bool = false) const
    {
      return false;
    }

    //- Write dictionary
    virtual void writeDict(Ostream&) const;

  // Member Operators

    //- Assign to zone, clearing demand-driven data
    void operator=(const cellZone&);

    //- Assign addressing, clearing demand-driven data
    void operator=(const labelUList&);

    //- Assign addressing, clearing demand-driven data
    void operator=(const Xfer<labelList>&);

  // I-O

    //- Ostream Operator
    friend Ostream& operator<<(Ostream&, const cellZone&);

};

}  // namespace mousse
#endif
