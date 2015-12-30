// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::surfacePatch
// Description
//   'Patch' on surface as subset of triSurface.
// SourceFiles
//   surface_patch.cpp
#ifndef surface_patch_hpp_
#define surface_patch_hpp_
#include "geometric_surface_patch.hpp"
#include "word.hpp"
#include "label.hpp"
#include "class_name.hpp"
namespace mousse
{
class surfacePatch
:
  public geometricSurfacePatch
{
  // Private data
    //- Size of this patch in the triSurface face list
    label size_;
    //- Start label of this patch in the triSurface face list
    label start_;
public:
  //- Runtime type information
  ClassName("surfacePatch");
  // Constructors
    //- Construct null
    surfacePatch();
    //- Construct from components
    surfacePatch
    (
      const word& geometricType,
      const word& name,
      const label size,
      const label start,
      const label index
    );
    //- Construct from Istream
    surfacePatch(Istream& is, const label index);
    //- Construct from dictionary
    surfacePatch
    (
      const word& name,
      const dictionary& dict,
      const label index
    );
    //- Construct as copy
    surfacePatch(const surfacePatch&);
  // Member Functions
    //- Return start label of this patch in the polyMesh face list
    label start() const
    {
      return start_;
    }
    //- Return start label of this patch in the polyMesh face list
    label& start()
    {
      return start_;
    }
    //- Return size of this patch in the polyMesh face list
    label size() const
    {
      return size_;
    }
    //- Return size of this patch in the polyMesh face list
    label& size()
    {
      return size_;
    }
    //- Write
    void write(Ostream&) const;
    //- Write dictionary
    void writeDict(Ostream&) const;
  // Member Operators
    bool operator!=(const surfacePatch&) const;
    //- compare.
    bool operator==(const surfacePatch&) const;
  // IOstream Operators
    friend Ostream& operator<<(Ostream&, const surfacePatch&);
};
}  // namespace mousse
#endif
