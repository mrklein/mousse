#ifndef CORE_MATRICES_LDU_MATRIX_SOLVERS_GAMG_GAMG_PROC_AGGLOMERATIONS_PROC_FACES_GAMG_PROC_AGGLOMERATION_HPP_
#define CORE_MATRICES_LDU_MATRIX_SOLVERS_GAMG_GAMG_PROC_AGGLOMERATIONS_PROC_FACES_GAMG_PROC_AGGLOMERATION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::procFacesGAMGProcAgglomeration
// Description
//   Processor agglomeration of GAMGAgglomerations. Needs nAgglomeratingCells
//   which is when to start agglomerating processors. Processors get agglomerated
//   by constructing a single cell mesh for each processor with each
//   processor interface a face. This then gets agglomerated using
//   the pairGAMGAgglomeration algorithm with the number of faces
//   on the original processor interface as face weight.
// SourceFiles
//   proc_faces_gamg_proc_agglomeration.cpp


#include "gamg_proc_agglomeration.hpp"
#include "dynamic_list.hpp"
#include "label_field.hpp"

namespace mousse
{
class GAMGAgglomeration;
class lduMesh;
class lduPrimitiveMesh;
class procFacesGAMGProcAgglomeration
:
  public GAMGProcAgglomeration
{
  // Private data

    //- When to processor agglomerate
    const label nAgglomeratingCells_;

    //- Allocated communicators
    DynamicList<label> comms_;

  // Private Member Functions

    //- Return (on master) all single-cell meshes collected. single-cell
    //  meshes are just one cell with all proc faces intact.
    autoPtr<lduPrimitiveMesh> singleCellMesh
    (
      const label singleCellMeshComm,
      const lduMesh& mesh,
      scalarField& faceWeights
    ) const;

    //- Construct processor agglomeration: for every processor the
    //  coarse processor-cluster it agglomerates onto
    tmp<labelField> processorAgglomeration(const lduMesh&) const;

    //- Do we need to agglomerate across processors?
    bool doProcessorAgglomeration(const lduMesh&) const;

public:

  //- Runtime type information
  TYPE_NAME("procFaces");

  // Constructors

    //- Construct given agglomerator and controls
    procFacesGAMGProcAgglomeration
    (
      GAMGAgglomeration& agglom,
      const dictionary& controlDict
    );

    //- Disallow default bitwise copy construct
    procFacesGAMGProcAgglomeration
    (
      const procFacesGAMGProcAgglomeration&
    ) = delete;

    //- Disallow default bitwise assignment
    procFacesGAMGProcAgglomeration& operator=
    (
      const procFacesGAMGProcAgglomeration&
    ) = delete;

  //- Destructor
  virtual ~procFacesGAMGProcAgglomeration();

  // Member Functions

   //- Modify agglomeration. Return true if modified
    virtual bool agglomerate();

};

}  // namespace mousse
#endif
