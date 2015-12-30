// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::boxToFace
// Description
//   A topoSetSource to select faces based on face centres inside box.
// SourceFiles
//   box_to_face.cpp
#ifndef box_to_face_hpp_
#define box_to_face_hpp_
#include "topo_set_source.hpp"
#include "tree_bound_box_list.hpp"
namespace mousse
{
class boxToFace
:
  public topoSetSource
{
  // Private data
    //- Add usage string
    static addToUsageTable usage_;
    //- Bounding box.
    treeBoundBoxList bbs_;
  // Private Member Functions
    void combine(topoSet& set, const bool add) const;
public:
  //- Runtime type information
  TypeName("boxToFace");
  // Constructors
    //- Construct from components
    boxToFace
    (
      const polyMesh& mesh,
      const treeBoundBoxList& bbs
    );
    //- Construct from dictionary
    boxToFace
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    boxToFace
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~boxToFace();
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
