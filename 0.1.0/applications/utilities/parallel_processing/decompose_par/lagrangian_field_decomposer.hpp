#ifndef UTILITIES_PARALLEL_PROCESSING_DECOMPOSE_PAR_LAGRANGIAN_FIELD_DECOMPOSER_HPP_
#define UTILITIES_PARALLEL_PROCESSING_DECOMPOSE_PAR_LAGRANGIAN_FIELD_DECOMPOSER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::lagrangianFieldDecomposer
// Description
//   Lagrangian field decomposer.
// SourceFiles
//   lagrangian_field_decomposer.cpp
//   lagrangian_field_decomposer_decompose_fields.cpp
#include "cloud.hpp"
#include "compact_io_field.hpp"
#include "indexed_particle.hpp"
#include "passive_particle.hpp"
namespace mousse
{
class IOobjectList;
class lagrangianFieldDecomposer
{
  // Private data
    //- Reference to processor mesh
    const polyMesh& procMesh_;
    //- Lagrangian positions for this processor
    Cloud<passiveParticle> positions_;
    //- The indices of the particles on this processor
    labelList particleIndices_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    lagrangianFieldDecomposer(const lagrangianFieldDecomposer&);
    //- Disallow default bitwise assignment
    void operator=(const lagrangianFieldDecomposer&);
public:
  // Constructors
    //- Construct from components
    lagrangianFieldDecomposer
    (
      const polyMesh& mesh,
      const polyMesh& procMesh,
      const labelList& faceProcAddressing,
      const labelList& cellProcAddressing,
      const word& cloudName,
      const Cloud<indexedParticle>& lagrangianPositions,
      const List<SLList<indexedParticle*>*>& cellParticles
    );
  // Member Functions
    // Read the fields and hold on the pointer list
    template<class Type>
    static void readFields
    (
      const label cloudI,
      const IOobjectList& lagrangianObjects,
      PtrList<PtrList<IOField<Type> > >& lagrangianFields
//            PtrList<IOField<Type> >& lagrangianFields
    );
    template<class Type>
    static void readFieldFields
    (
      const label cloudI,
      const IOobjectList& lagrangianObjects,
      PtrList
      <
        PtrList<CompactIOField<Field<Type>, Type> >
      >& lagrangianFields
//            PtrList<CompactIOField<Field<Type>, Type > >& lagrangianFields
    );
    //- Decompose volume field
    template<class Type>
    tmp<IOField<Type> > decomposeField
    (
      const word& cloudName,
      const IOField<Type>& field
    ) const;
    template<class Type>
    tmp<CompactIOField<Field<Type>, Type> > decomposeFieldField
    (
      const word& cloudName,
      const CompactIOField<Field<Type>, Type>& field
    ) const;
    template<class GeoField>
    void decomposeFields
    (
      const word& cloudName,
      const PtrList<GeoField>& fields
    ) const;
    template<class GeoField>
    void decomposeFieldFields
    (
      const word& cloudName,
      const PtrList<GeoField>& fields
    ) const;
};
}  // namespace mousse
#ifdef NoRepository
  #include "lagrangian_field_decomposer_decompose_fields.cpp"
#endif
#endif
