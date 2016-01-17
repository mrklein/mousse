// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::perfectInterface
// Description
//   Hack of attachDetach to couple patches when they perfectly align.
//   Does not decouple. Used by stitchMesh app. Does geometric matching.
// SourceFiles
//   perfect_interface.cpp
#ifndef perfect_interface_hpp_
#define perfect_interface_hpp_
#include "poly_mesh_modifier.hpp"
#include "poly_patch_id.hpp"
#include "zone_i_ds.hpp"
#include "indirect_primitive_patch.hpp"
namespace mousse
{
// Forward declaration of classes
class Time;
class perfectInterface
:
  public polyMeshModifier
{
  // Private data
    //- Master face zone ID
    faceZoneID faceZoneID_;
    //- Master patch ID
    polyPatchID masterPatchID_;
    //- Slave patch ID
    polyPatchID slavePatchID_;
    //- Tolerance used for distance comparison (fraction of minimum edge
    //  length)
    static const scalar tol_;
  // Private Member Functions
    //- Calculate face centres on patch
    static pointField calcFaceCentres(const indirectPrimitivePatch&);
public:
  //- Runtime type information
  TYPE_NAME("perfectInterface");
  // Constructors
    //- Construct from components
    perfectInterface
    (
      const word& name,
      const label index,
      const polyTopoChanger& mme,
      const word& faceZoneName,
      const word& masterPatchName,
      const word& slavePatchName
    );
    //- Construct from dictionary
    perfectInterface
    (
      const word& name,
      const dictionary& dict,
      const label index,
      const polyTopoChanger& mme
    );
    //- Disallow default bitwise copy construct
    perfectInterface(const perfectInterface&) = delete;
    //- Disallow default bitwise assignment
    perfectInterface& operator=(const perfectInterface&) = delete;
  //- Destructor
  virtual ~perfectInterface();
  // Member Functions
    //- Check for topology change
    virtual bool changeTopology() const;
    //- Insert the layer addition/removal instructions
    //  into the topological change
    virtual void setRefinement(polyTopoChange&) const;
    //- Insert the layer addition/removal instructions
    //  into the topological change. Uses only mesh, not any of the
    //  patch and zone indices. Bit of a workaround - used in extruding
    //  a mesh.
    virtual void setRefinement
    (
      const indirectPrimitivePatch& pp0,
      const indirectPrimitivePatch& pp1,
      polyTopoChange&
    ) const;
    //- Modify motion points to comply with the topological change
    virtual void modifyMotionPoints(pointField& motionPoints) const;
    //- Force recalculation of locally stored data on topological change
    virtual void updateMesh(const mapPolyMesh&);
    //- Write
    virtual void write(Ostream&) const;
    //- Write dictionary
    virtual void writeDict(Ostream&) const;
};
}  // namespace mousse
#endif
