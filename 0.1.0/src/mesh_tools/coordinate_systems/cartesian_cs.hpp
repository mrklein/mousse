// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cartesianCS
// Description
//   Cylindrical coordinate system
// SourceFiles
//   cartesian_cs.cpp
#ifndef cartesian_cs_hpp_
#define cartesian_cs_hpp_
#include "coordinate_system.hpp"
#include "type_info.hpp"
namespace mousse
{
class cartesianCS
:
  public coordinateSystem
{
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
  //- Runtime type information
  TypeName("cartesian");
  // Constructors
    //- Construct null
    cartesianCS();
    //- Construct copy
    cartesianCS
    (
      const coordinateSystem&
    );
    //- Construct copy with a different name
    cartesianCS
    (
      const word& name,
      const coordinateSystem&
    );
    //- Construct from origin and rotation
    cartesianCS
    (
      const word& name,
      const point& origin,
      const coordinateRotation&
    );
    //- Construct from origin and 2 axes
    cartesianCS
    (
      const word& name,
      const point& origin,
      const vector& axis,
      const vector& dirn
    );
    //- Construct from dictionary
    cartesianCS(const word&, const dictionary&);
    //- Construct from dictionary and objectRegistry
    cartesianCS(const objectRegistry&, const dictionary&);
  //- Destructor
  virtual ~cartesianCS();
};
}  // namespace mousse
#endif
