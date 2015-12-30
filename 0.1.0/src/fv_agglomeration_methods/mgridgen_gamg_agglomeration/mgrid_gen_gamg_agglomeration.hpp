// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::MGridGenGAMGAgglomeration
// Description
//   Agglomerate using the MGridGen algorithm.
// SourceFiles
//   m_grid_gen_gamg_agglomeration.cpp
//   m_grid_gen_gamg_agglomerate.cpp
#ifndef m_grid_gen_gamg_agglomeration_hpp_
#define m_grid_gen_gamg_agglomeration_hpp_
#include "fv_mesh.hpp"
#include "gamg_agglomeration.hpp"
extern "C"
{
#   include "mgridgen.hpp"
}
namespace mousse
{
class fvMesh;
class MGridGenGAMGAgglomeration
:
  public GAMGAgglomeration
{
  // Private data
    const fvMesh& fvMesh_;
  // Private Member Functions
    void swap
    (
      const lduInterfacePtrsList& interfaces,
      const labelUList& cellValues,
      PtrList<labelList>& nbrValues
    ) const;
    void getNbrAgglom
    (
      const lduAddressing& addr,
      const lduInterfacePtrsList& interfaces,
      const PtrList<labelList>& nbrGlobalAgglom,
      labelList& cellToNbrAgglom
    ) const;
    void detectSharedFaces
    (
      const lduMesh& mesh,
      const labelList& value,
      labelHashSet& sharedFaces
    ) const;
    //- Construct the CSR format addressing
    void makeCompactCellFaceAddressingAndFaceWeights
    (
      const lduAddressing& fineAddressing,
      List<idxtype>& cellCells,
      List<idxtype>& cellCellOffsets,
      const scalarField& magSi,
      List<scalar>& faceWeights
    );
    //- Calculate and return agglomeration
    tmp<labelField> agglomerate
    (
      label& nCoarseCells,
      const label minSize,
      const label maxSize,
      const lduAddressing& fineAddressing,
      const scalarField& V,
      const scalarField& magSf,
      const scalarField& magSb
    );
    //- Disallow default bitwise copy construct
    MGridGenGAMGAgglomeration(const MGridGenGAMGAgglomeration&);
    //- Disallow default bitwise assignment
    void operator=(const MGridGenGAMGAgglomeration&);
public:
  //- Runtime type information
  TypeName("MGridGen");
  // Constructors
    //- Construct given mesh and controls
    MGridGenGAMGAgglomeration
    (
      const lduMesh& mesh,
      const dictionary& controlDict
    );
};
}  // namespace mousse
#endif
