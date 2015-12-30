// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::engineMesh
// Description
//   mousse::engineMesh
// SourceFiles
//   engine_mesh.cpp
#ifndef engine_mesh_hpp_
#define engine_mesh_hpp_
#include "engine_time.hpp"
#include "fv_mesh.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class engineMesh
:
  public fvMesh
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    engineMesh(const engineMesh&);
    //- Disallow default bitwise assignment
    void operator=(const engineMesh&);
protected:
  // Protected data
    const engineTime& engineDB_;
    label pistonIndex_;
    label linerIndex_;
    label cylinderHeadIndex_;
    dimensionedScalar deckHeight_;
    dimensionedScalar pistonPosition_;
public:
  //- Runtime type information
  TypeName("engineMesh");
  // Declare run-time constructor selection table
    declareRunTimeSelectionTable
    (
      autoPtr,
      engineMesh,
      IOobject,
      (const IOobject& io),
      (io)
    );
  // Constructors
    //- Construct from objectRegistry, and read/write options
    explicit engineMesh(const IOobject& io);
  // Selectors
    //- Select null constructed
    static autoPtr<engineMesh> New(const IOobject& io);
  //- Destructor
  virtual ~engineMesh();
  // Member Functions
    // Edit
      virtual void move() = 0;
};
}  // namespace mousse
#endif
