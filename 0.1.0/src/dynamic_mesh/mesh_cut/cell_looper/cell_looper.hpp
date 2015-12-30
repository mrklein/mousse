// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cellLooper
// Description
//   Abstract base class. Concrete implementations know how to cut a cell
//   (i.e. determine a loop around the circumference).
//   Loop around the cell is given as the vertices to be cut and edges to
//   be cut (and a weight between 0 and 1 giving where the cut is to be
//   made). Main routine is 'cut' which gets called for every cell and
//   gets the current cut situation and expects to return a loop on the
//   cell circumference.
//   Calling function needs to determine whether cellLooper is compatible with
//   existing set of cuts.
//   Also contains various utility functions which implementations might want to
//   use.
// SourceFiles
//   cell_looper.cpp
#ifndef cell_looper_hpp_
#define cell_looper_hpp_
#include "edge_vertex.hpp"
#include "vector.hpp"
#include "bool_list.hpp"
#include "scalar_field.hpp"
#include "dynamic_list.hpp"
namespace mousse
{
// Forward declaration of classes
class polyMesh;
class plane;
class cellLooper
:
  public edgeVertex
{
  // Private data
protected:
  // Protected Member Functions
    //- Get faces (on cell) connected to vertI which are not using edgeI
    labelList getVertFacesNonEdge
    (
      const label cellI,
      const label edgeI,
      const label vertI
    ) const;
    //- Get first edge connected to vertI and on faceI
    label getFirstVertEdge
    (
      const label faceI,
      const label vertI
    ) const;
    //- Get edges (on cell) connected to vertI which are not on faceI
    labelList getVertEdgesNonFace
    (
      const label cellI,
      const label faceI,
      const label vertI
    ) const;
    //- Return edge from cellEdges that is most perpendicular
    //  to refinement direction.
    label getMisAlignedEdge(const vector& refDir, const label cellI) const;
private:
  // Private Member Functions
    //- Disallow default bitwise copy construct
    cellLooper(const cellLooper&);
    //- Disallow default bitwise assignment
    void operator=(const cellLooper&);
public:
  //- Runtime type information
  TypeName("cellLooper");
  // Declare run-time constructor selection table
    // For the direct constructor
    declareRunTimeSelectionTable
    (
      autoPtr,
      cellLooper,
      word,
      (
        const polyMesh& mesh
      ),
      (mesh)
    );
  // Constructors
    //- Construct from components
    cellLooper(const polyMesh& mesh);
    //- Clone
    autoPtr<cellLooper> clone() const
    {
      notImplemented("autoPtr<tcellLooper> clone() const");
      return autoPtr<cellLooper>(NULL);
    }
  // Selectors
    //- Return a reference to the selected cellLooper
    static autoPtr<cellLooper> New
    (
      const word& type,
      const polyMesh& mesh
    );
  //- Destructor
  virtual ~cellLooper();
  // Member Functions
    //- Create cut along circumference of cellI. Gets current mesh cuts
    //  vertIsCut, edgeIsCut, edgeWeight).
    //  Cut along circumference is expressed as cellVertCut,
    //  cellEdgeToWeight. Returns true if succesfull. Still might not
    //  be compatible with existing cuts but this should be handled by
    //  caller).
    virtual bool cut
    (
      const vector& refDir,
      const label cellI,
      const boolList& vertIsCut,
      const boolList& edgeIsCut,
      const scalarField& edgeWeight,
      labelList& loop,
      scalarField& loopWeights
    ) const = 0;
    //- Same but now also base point of cut provided (instead of always
    //  cell centre)
    virtual bool cut
    (
      const plane& cutPlane,
      const label cellI,
      const boolList& vertIsCut,
      const boolList& edgeIsCut,
      const scalarField& edgeWeight,
      labelList& loop,
      scalarField& loopWeights
    ) const = 0;
};
}  // namespace mousse
#endif
