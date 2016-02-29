#ifndef MESH_TOOLS_SETS_FACE_SOURCES_NORMAL_TO_FACE_NORMAL_TO_FACE_HPP_
#define MESH_TOOLS_SETS_FACE_SOURCES_NORMAL_TO_FACE_NORMAL_TO_FACE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::normalToFace
// Description
//   A topoSetSource to select faces based on normal.
// SourceFiles
//   normal_to_face.cpp
#include "topo_set_source.hpp"
#include "named_enum.hpp"
namespace mousse
{
class normalToFace
:
  public topoSetSource
{
private:
    //- Add usage string
    static addToUsageTable usage_;
    //- (unit)vector to compare to
    vector normal_;
    //- Tolerance (i.e. cos of angle between normal_ and faceNormal)
    const scalar tol_;
  // Private Member Functions
    //- Normalize normal and check tolerance
    void setNormal();
public:
  //- Runtime type information
  TYPE_NAME("normalToFace");
  // Constructors
    //- Construct from components
    normalToFace
    (
      const polyMesh& mesh,
      const vector& normal,
      const scalar tol
    );
    //- Construct from dictionary
    normalToFace(const polyMesh& mesh, const dictionary& dict);
    //- Construct from Istream
    normalToFace(const polyMesh& mesh, Istream&);
  //- Destructor
  virtual ~normalToFace();
  // Member Functions
    virtual sourceType setType() const
    {
      return FACESETSOURCE;
    }
    virtual void applyToSet
    (
      const topoSetSource::setAction action,
      topoSet&
    ) const;
};
}  // namespace mousse
#endif
