// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::boundaryPatch
// Description
//   Like polyPatch but without reference to mesh. patchIdentifier::index
//   is not used. Used in boundaryMesh to hold data on patches.
// SourceFiles
//   boundary_patch.cpp
#ifndef boundary_patch_hpp_
#define boundary_patch_hpp_
#include "patch_identifier.hpp"
#include "auto_ptr.hpp"
namespace mousse
{
class boundaryPatch
:
  public patchIdentifier
{
  // Private data
    label size_;
    label start_;
public:
  // Constructors
    //- Construct from components
    boundaryPatch
    (
      const word& name,
      const label index,
      const label size,
      const label start,
      const word& physicalType = word::null
    );
    //- Construct from dictionary
    boundaryPatch
    (
      const word& name,
      const dictionary& dict,
      const label index
    );
    //- Construct as copy
    boundaryPatch(const boundaryPatch&);
    //- Construct as copy, resetting the index
    boundaryPatch(const boundaryPatch&, const label index);
    //- Clone
    autoPtr<boundaryPatch> clone() const;
  //- Destructor
  ~boundaryPatch();
  // Member Functions
    label size() const
    {
      return size_;
    }
    label& size()
    {
      return size_;
    }
    label start() const
    {
      return start_;
    }
    label& start()
    {
      return start_;
    }
    //- Write dictionary
    virtual void write(Ostream&) const;
  // Ostream Operator
    friend Ostream& operator<<(Ostream&, const boundaryPatch&);
};
}  // namespace mousse
#endif
