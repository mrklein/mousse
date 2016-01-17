// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::patchToFace
// Description
//   A topoSetSource to select faces based on usage in patches.
// SourceFiles
//   patch_to_face.cpp
#ifndef patch_to_face_hpp_
#define patch_to_face_hpp_
#include "topo_set_source.hpp"
#include "word_re.hpp"
namespace mousse
{
class patchToFace
:
  public topoSetSource
{
  // Private data
    //- Add usage string
    static addToUsageTable usage_;
    //- Name/regular expression of patch
    wordRe patchName_;
  // Private Member Functions
    void combine(topoSet& set, const bool add) const;
public:
  //- Runtime type information
  TYPE_NAME("patchToFace");
  // Constructors
    //- Construct from components
    patchToFace
    (
      const polyMesh& mesh,
      const word& patchName
    );
    //- Construct from dictionary
    patchToFace
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    patchToFace
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~patchToFace();
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
