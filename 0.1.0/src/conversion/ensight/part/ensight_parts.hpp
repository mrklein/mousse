// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ensightParts
// Description
//   A collection of several ensightPart elements
// SourceFiles
//   ensight_parts.cpp
//   ensight_parts_templates.cpp
#ifndef ensight_parts_hpp_
#define ensight_parts_hpp_
#include "ensight_part.hpp"
#include "ensight_part_faces.hpp"
#include "ensight_part_cells.hpp"
#include "vol_fields.hpp"
namespace mousse
{
class ensightParts
{
  // Private Data
    //- List of parts
    PtrList<ensightPart> partsList_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    ensightParts(const ensightParts&);
    //- Disallow default bitwise assignment
    void operator=(const ensightParts&);
public:
  // Constructors
    //- Construct from polyMesh
    ensightParts(const polyMesh&);
    //- Construct from IOobject
    ensightParts(const IOobject&);
  //- Destructor
  ~ensightParts();
  // Member functions
    //- Clear old information and construct anew from polyMesh
    void recalculate(const polyMesh&);
    //- Renumber elements
    void renumber
    (
      const labelUList& origCellId,
      const labelUList& origFaceId
    );
    //- Number of parts
    label size() const
    {
      return partsList_.size();
    }
    //- Write the geometry
    void writeGeometry(ensightGeoFile&) const;
    //- Write summary information about the objects
    bool writeSummary(Ostream&) const;
    //- Write the lists
    void writeData(Ostream&) const;
    //- Write (volume) scalar field
    //  optionally write data for face parts
    //  optionally write data per node
    void writeScalarField
    (
      ensightFile&,
      const List<scalar>& field,
      const bool useFaceData = false,
      const bool perNode = false
    ) const;
    //- Write (volume) vector field components
    //  optionally write data for face parts
    //  optionally write data per node
    void writeVectorField
    (
      ensightFile&,
      const List<scalar>& field0,
      const List<scalar>& field1,
      const List<scalar>& field2,
      const bool useFaceData = false,
      const bool perNode = false
    ) const;
    //- Write generalized volume field components
    template<class Type>
    void writeField
    (
      ensightFile&,
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const;
  // Friend Operators
    //- Write geometry
    friend ensightGeoFile& operator<<(ensightGeoFile&, const ensightParts&);
};
}  // namespace mousse
#ifdef NoRepository
#   include "ensight_parts_templates.cpp"
#endif
#endif
