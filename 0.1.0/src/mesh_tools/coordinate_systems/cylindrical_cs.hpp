#ifndef MESH_TOOLS_COORDINATE_SYSTEMS_CYLINDRICAL_CS_HPP_
#define MESH_TOOLS_COORDINATE_SYSTEMS_CYLINDRICAL_CS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cylindricalCS
// Description
//   Cylindrical coordinate system
// SourceFiles
//   cylindrical_cs.cpp
#include "coordinate_system.hpp"
namespace mousse
{
class cylindricalCS
:
  public coordinateSystem
{
  // Private data members
    //- Are angles in degrees? (default = true)
    bool inDegrees_;
protected:
  // Protected Member Functions
    //- Convert from local coordinate system to the global Cartesian system
    //  with optional translation for the origin
    virtual vector localToGlobal(const vector&, bool translate) const;
    //- Convert from local coordinate system to the global Cartesian system
    //  with optional translation for the origin
    virtual tmp<vectorField> localToGlobal
    (
      const vectorField&,
      bool translate
    ) const;
    //- Convert from global Cartesian system to the local coordinate system
    //  with optional translation for the origin
    virtual vector globalToLocal(const vector&, bool translate) const;
    //- Convert from global Cartesian system to the local coordinate system
    //  with optional translation for the origin
    virtual tmp<vectorField> globalToLocal
    (
      const vectorField&,
      bool translate
    ) const;
public:
  // Constructors
    //- Construct null
    cylindricalCS(const bool inDegrees=true);
    //- Construct copy
    cylindricalCS
    (
      const coordinateSystem&,
      const bool inDegrees=true
    );
    //- Construct copy with a different name
    cylindricalCS
    (
      const word& name,
      const coordinateSystem&,
      const bool inDegrees=true
    );
    //- Construct from origin and rotation
    cylindricalCS
    (
      const word& name,
      const point& origin,
      const coordinateRotation&,
      const bool inDegrees=true
    );
    //- Construct from origin and 2 axes
    cylindricalCS
    (
      const word& name,
      const point& origin,
      const vector& axis,
      const vector& dirn,
      const bool inDegrees=true
    );
    //- Construct from dictionary and name
    cylindricalCS(const word&, const dictionary&);
    //- Construct from dictionary and objectRegistry
    cylindricalCS(const objectRegistry&, const dictionary&);
  //- Destructor
  virtual ~cylindricalCS();
  // Member Functions
    //- Are angles in degrees?
    bool  inDegrees() const;
    //- Non-const access to inDegrees
    bool& inDegrees();
};
}  // namespace mousse
#endif
