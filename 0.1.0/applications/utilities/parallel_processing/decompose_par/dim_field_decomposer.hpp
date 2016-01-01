// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dimFieldDecomposer
// Description
//   Dimensioned field decomposer.
// SourceFiles
//   dim_field_decomposer.cpp
//   dim_field_decomposer_decompose_fields.cpp
#ifndef dim_field_decomposer_hpp_
#define dim_field_decomposer_hpp_
#include "fv_mesh.hpp"
#include "surface_fields.hpp"
namespace mousse
{
class IOobjectList;
class dimFieldDecomposer
{
private:
  // Private data
    //- Reference to complete mesh
    const fvMesh& completeMesh_;
    //- Reference to processor mesh
    const fvMesh& procMesh_;
    //- Reference to face addressing
    const labelList& faceAddressing_;
    //- Reference to cell addressing
    const labelList& cellAddressing_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    dimFieldDecomposer(const dimFieldDecomposer&);
    //- Disallow default bitwise assignment
    void operator=(const dimFieldDecomposer&);
public:
  // Constructors
    //- Construct from components
    dimFieldDecomposer
    (
      const fvMesh& completeMesh,
      const fvMesh& procMesh,
      const labelList& faceAddressing,
      const labelList& cellAddressing
    );
  //- Destructor
  ~dimFieldDecomposer();
  // Member Functions
    //- Decompose field
    template<class Type>
    tmp<DimensionedField<Type, volMesh> > decomposeField
    (
      const DimensionedField<Type, volMesh>& field
    ) const;
    //- Decompose llist of fields
    template<class GeoField>
    void decomposeFields(const PtrList<GeoField>& fields) const;
};
}  // namespace mousse
#ifdef NoRepository
  #include "dim_field_decomposer_decompose_fields.cpp"
#endif
#endif
