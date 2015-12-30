// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "raw_topo_changer_fv_mesh.hpp"
#include "map_poly_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "vol_fields.hpp"
#include "linear.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(rawTopoChangerFvMesh, 0);
  addToRunTimeSelectionTable
  (
    topoChangerFvMesh,
    rawTopoChangerFvMesh,
    IOobject
  );
}
// Constructors 
// Construct from components
mousse::rawTopoChangerFvMesh::rawTopoChangerFvMesh(const IOobject& io)
:
  topoChangerFvMesh(io)
{}
// Destructor 
mousse::rawTopoChangerFvMesh::~rawTopoChangerFvMesh()
{}
// Member Functions 
bool mousse::rawTopoChangerFvMesh::update()
{
  // Do mesh changes (use inflation - put new points in topoChangeMap)
  Info<< "rawTopoChangerFvMesh : Checking for topology changes..."
    << endl;
  // Mesh not moved/changed yet
  moving(false);
  topoChanging(false);
  // Do any topology changes. Sets topoChanging (through polyTopoChange)
  autoPtr<mapPolyMesh> topoChangeMap = topoChanger_.changeMesh(true);
  bool hasChanged = topoChangeMap.valid();
  if (hasChanged)
  {
    Info<< "rawTopoChangerFvMesh : Done topology changes..."
      << endl;
    // Temporary: fix fields on patch faces created out of nothing
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Two situations:
    // - internal faces inflated out of nothing
    // - patch faces created out of previously internal faces
    // Is face mapped in any way?
    PackedBoolList mappedFace(nFaces());
    const label nOldInternal = topoChangeMap().oldPatchStarts()[0];
    const labelList& faceMap = topoChangeMap().faceMap();
    for (label faceI = 0; faceI < nInternalFaces(); faceI++)
    {
      if (faceMap[faceI] >= 0)
      {
        mappedFace[faceI] = 1;
      }
    }
    for (label faceI = nInternalFaces(); faceI < nFaces(); faceI++)
    {
      if (faceMap[faceI] >= 0 && faceMap[faceI] >= nOldInternal)
      {
        mappedFace[faceI] = 1;
      }
    }
    const List<objectMap>& fromFaces = topoChangeMap().facesFromFacesMap();
    forAll(fromFaces, i)
    {
      mappedFace[fromFaces[i].index()] = 1;
    }
    const List<objectMap>& fromEdges = topoChangeMap().facesFromEdgesMap();
    forAll(fromEdges, i)
    {
      mappedFace[fromEdges[i].index()] = 1;
    }
    const List<objectMap>& fromPts = topoChangeMap().facesFromPointsMap();
    forAll(fromPts, i)
    {
      mappedFace[fromPts[i].index()] = 1;
    }
    // Set unmapped faces to zero
    Info<< "rawTopoChangerFvMesh : zeroing unmapped boundary values."
      << endl;
    zeroUnmappedValues<scalar, fvPatchField, volMesh>(mappedFace);
    zeroUnmappedValues<vector, fvPatchField, volMesh>(mappedFace);
    zeroUnmappedValues<sphericalTensor, fvPatchField, volMesh>(mappedFace);
    zeroUnmappedValues<symmTensor, fvPatchField, volMesh>(mappedFace);
    zeroUnmappedValues<tensor, fvPatchField, volMesh>(mappedFace);
    // Special handling for phi: set unmapped faces to recreated phi
    Info<< "rawTopoChangerFvMesh :"
      << " recreating phi for unmapped boundary values." << endl;
    const volVectorField& U = lookupObject<volVectorField>("U");
    surfaceScalarField& phi = const_cast<surfaceScalarField&>
    (
      lookupObject<surfaceScalarField>("phi")
    );
    setUnmappedValues
    (
      phi,
      mappedFace,
      (linearInterpolate(U) & Sf())()
    );
    if (topoChangeMap().hasMotionPoints())
    {
      pointField newPoints = topoChangeMap().preMotionPoints();
      // Give the meshModifiers opportunity to modify points
      Info<< "rawTopoChangerFvMesh :"
        << " calling modifyMotionPoints." << endl;
      topoChanger_.modifyMotionPoints(newPoints);
      // Actually move points
      Info<< "rawTopoChangerFvMesh :"
        << " calling movePoints." << endl;
      movePoints(newPoints);
    }
  }
  else
  {
    //Pout<< "rawTopoChangerFvMesh :"
    //    << " no topology changes..." << endl;
  }
  return hasChanged;
}
