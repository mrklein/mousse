#ifndef ENGINE_ENGINE_PISTON_ENGINE_PISTON_HPP_
#define ENGINE_ENGINE_PISTON_ENGINE_PISTON_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::enginePiston
// Description
//   mousse::enginePiston

#include "poly_patch_id.hpp"
#include "coordinate_system.hpp"


namespace mousse {

// Forward declaration of classes
class polyMesh;
class engineTime;


class enginePiston
{
  // Private data
    //- Reference to engine mesh
    const polyMesh& mesh_;
    //- Reference to engine database
    const engineTime& engineDB_;
    //- Piston patch
    polyPatchID patchID_;
    //- Coordinate system
    autoPtr<coordinateSystem> csPtr_;
    // Piston layering data
      //- Min layer thickness
      const scalar minLayer_;
      //- Max layer thickness
      const scalar maxLayer_;
public:
  // Static data members
  // Constructors
    //- Construct from components
    enginePiston
    (
      const polyMesh& mesh,
      const word& pistonPatchName,
      const autoPtr<coordinateSystem>& pistonCS,
      const scalar minLayer,
      const scalar maxLayer
    );
    //- Construct from dictionary
    enginePiston
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Disallow default bitwise copy construct
    enginePiston(const enginePiston&) = delete;
    //- Disallow default bitwise assignment
    enginePiston& operator=(const enginePiston&) = delete;
  // Destructor - default
  // Member Functions
    //- Return coordinate system
    const coordinateSystem& cs() const { return csPtr_(); }
    //- Return ID of piston patch
    const polyPatchID& patchID() const { return patchID_; }
    // Piston layering thickness
      scalar minLayer() const { return minLayer_; }
      scalar maxLayer() const { return maxLayer_; }
    //- Write dictionary
    void writeDict(Ostream&) const;
};

}  // namespace mousse

#endif

