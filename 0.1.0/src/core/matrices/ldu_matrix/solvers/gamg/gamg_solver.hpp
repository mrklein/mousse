// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::GAMGSolver
// Description
//   Geometric agglomerated algebraic multigrid solver.
//  Characteristics:
//    - Requires positive definite, diagonally dominant matrix.
//    - Agglomeration algorithm: selectable and optionally cached.
//    - Restriction operator: summation.
//    - Prolongation operator: injection.
//    - Smoother: Gauss-Seidel.
//    - Coarse matrix creation: central coefficient: summation of fine grid
//     central coefficients with the removal of intra-cluster face;
//     off-diagonal coefficient: summation of off-diagonal faces.
//    - Coarse matrix scaling: performed by correction scaling, using steepest
//     descent optimisation.
//    - Type of cycle: V-cycle with optional pre-smoothing.
//    - Coarsest-level matrix solved using ICCG or BICCG.
// SourceFiles
//   gamg_solver.cpp
//   gamg_solver_agglomerate_matrix.cpp
//   gamg_solver_interpolate.cpp
//   gamg_solver_scale.cpp
//   gamg_solver_solve.cpp
#ifndef gamg_solver_hpp_
#define gamg_solver_hpp_
#include "gamg_agglomeration.hpp"
#include "ldu_matrix.hpp"
#include "label_field.hpp"
#include "primitive_fields.hpp"
#include "_luscalar_matrix.hpp"
namespace mousse
{
class GAMGSolver
:
  public lduMatrix::solver
{
  // Private data
    bool cacheAgglomeration_;
    //- Number of pre-smoothing sweeps
    label nPreSweeps_;
    //- Lever multiplier for the number of pre-smoothing sweeps
    label preSweepsLevelMultiplier_;
    //- Maximum number of pre-smoothing sweeps
    label maxPreSweeps_;
    //- Number of post-smoothing sweeps
    label nPostSweeps_;
    //- Lever multiplier for the number of post-smoothing sweeps
    label postSweepsLevelMultiplier_;
    //- Maximum number of post-smoothing sweeps
    label maxPostSweeps_;
    //- Number of smoothing sweeps on finest mesh
    label nFinestSweeps_;
    //- Choose if the corrections should be interpolated after injection.
    //  By default corrections are not interpolated.
    bool interpolateCorrection_;
    //- Choose if the corrections should be scaled.
    //  By default corrections for symmetric matrices are scaled
    //  but not for asymmetric matrices.
    bool scaleCorrection_;
    //- Direct or iteratively solve the coarsest level
    bool directSolveCoarsest_;
    //- The agglomeration
    const GAMGAgglomeration& agglomeration_;
    //- Hierarchy of matrix levels
    PtrList<lduMatrix> matrixLevels_;
    //- Hierarchy of interfaces.
    PtrList<PtrList<lduInterfaceField> > primitiveInterfaceLevels_;
    //- Hierarchy of interfaces in lduInterfaceFieldPtrs form
    PtrList<lduInterfaceFieldPtrsList> interfaceLevels_;
    //- Hierarchy of interface boundary coefficients
    PtrList<FieldField<Field, scalar> > interfaceLevelsBouCoeffs_;
    //- Hierarchy of interface internal coefficients
    PtrList<FieldField<Field, scalar> > interfaceLevelsIntCoeffs_;
    //- LU decompsed coarsest matrix
    autoPtr<LUscalarMatrix> coarsestLUMatrixPtr_;
  // Private Member Functions
    //- Read control parameters from the control dictionary
    virtual void readControls();
    //- Simplified access to interface level
    const lduInterfaceFieldPtrsList& interfaceLevel
    (
      const label i
    ) const;
    //- Simplified access to matrix level
    const lduMatrix& matrixLevel(const label i) const;
    //- Simplified access to interface boundary coeffs level
    const FieldField<Field, scalar>& interfaceBouCoeffsLevel
    (
      const label i
    ) const;
    //- Simplified access to interface internal coeffs level
    const FieldField<Field, scalar>& interfaceIntCoeffsLevel
    (
      const label i
    ) const;
    //- Agglomerate coarse matrix. Supply mesh to use - so we can
    //  construct temporary matrix on the fine mesh (instead of the coarse
    //  mesh)
    void agglomerateMatrix
    (
      const label fineLevelIndex,
      const lduMesh& coarseMesh,
      const lduInterfacePtrsList& coarseMeshInterfaces
    );
    //- Agglomerate coarse interface coefficients
    void agglomerateInterfaceCoefficients
    (
      const label fineLevelIndex,
      const lduInterfacePtrsList& coarseMeshInterfaces,
      PtrList<lduInterfaceField>& coarsePrimInterfaces,
      lduInterfaceFieldPtrsList& coarseInterfaces,
      FieldField<Field, scalar>& coarseInterfaceBouCoeffs,
      FieldField<Field, scalar>& coarseInterfaceIntCoeffs
    ) const;
    //- Collect matrices from other processors
    void gatherMatrices
    (
      const labelList& procIDs,
      const lduMesh& dummyMesh,
      const label meshComm,
      const lduMatrix& mat,
      const FieldField<Field, scalar>& interfaceBouCoeffs,
      const FieldField<Field, scalar>& interfaceIntCoeffs,
      const lduInterfaceFieldPtrsList& interfaces,
      PtrList<lduMatrix>& otherMats,
      PtrList<FieldField<Field, scalar> >& otherBouCoeffs,
      PtrList<FieldField<Field, scalar> >& otherIntCoeffs,
      List<boolList>& otherTransforms,
      List<List<label> >& otherRanks
    ) const;
    //- Agglomerate processor matrices
    void procAgglomerateMatrix
    (
      // Agglomeration information
      const labelList& procAgglomMap,
      const List<label>& agglomProcIDs,
      const label levelI,
      // Resulting matrix
      autoPtr<lduMatrix>& allMatrixPtr,
      FieldField<Field, scalar>& allInterfaceBouCoeffs,
      FieldField<Field, scalar>& allInterfaceIntCoeffs,
      PtrList<lduInterfaceField>& allPrimitiveInterfaces,
      lduInterfaceFieldPtrsList& allInterfaces
    ) const;
    //- Agglomerate processor matrices
    void procAgglomerateMatrix
    (
      const labelList& procAgglomMap,
      const List<label>& agglomProcIDs,
      const label levelI
    );
    //- Interpolate the correction after injected prolongation
    void interpolate
    (
      scalarField& psi,
      scalarField& Apsi,
      const lduMatrix& m,
      const FieldField<Field, scalar>& interfaceBouCoeffs,
      const lduInterfaceFieldPtrsList& interfaces,
      const direction cmpt
    ) const;
    //- Interpolate the correction after injected prolongation and
    /// re-normalise
    void interpolate
    (
      scalarField& psi,
      scalarField& Apsi,
      const lduMatrix& m,
      const FieldField<Field, scalar>& interfaceBouCoeffs,
      const lduInterfaceFieldPtrsList& interfaces,
      const labelList& restrictAddressing,
      const scalarField& psiC,
      const direction cmpt
    ) const;
    //- Calculate and apply the scaling factor from Acf, coarseSource
    //  and coarseField.
    //  At the same time do a Jacobi iteration on the coarseField using
    //  the Acf provided after the coarseField values are used for the
    //  scaling factor.
    void scale
    (
      scalarField& field,
      scalarField& Acf,
      const lduMatrix& A,
      const FieldField<Field, scalar>& interfaceLevelBouCoeffs,
      const lduInterfaceFieldPtrsList& interfaceLevel,
      const scalarField& source,
      const direction cmpt
    ) const;
    //- Initialise the data structures for the V-cycle
    void initVcycle
    (
      PtrList<scalarField>& coarseCorrFields,
      PtrList<scalarField>& coarseSources,
      PtrList<lduMatrix::smoother>& smoothers,
      scalarField& scratch1,
      scalarField& scratch2
    ) const;
    //- Perform a single GAMG V-cycle with pre, post and finest smoothing.
    void Vcycle
    (
      const PtrList<lduMatrix::smoother>& smoothers,
      scalarField& psi,
      const scalarField& source,
      scalarField& Apsi,
      scalarField& finestCorrection,
      scalarField& finestResidual,
      scalarField& scratch1,
      scalarField& scratch2,
      PtrList<scalarField>& coarseCorrFields,
      PtrList<scalarField>& coarseSources,
      const direction cmpt=0
    ) const;
    //- Solve the coarsest level with either an iterative or direct solver
    void solveCoarsestLevel
    (
      scalarField& coarsestCorrField,
      const scalarField& coarsestSource
    ) const;
public:
  friend class GAMGPreconditioner;
  //- Runtime type information
  TYPE_NAME("GAMG");
  // Constructors
    //- Construct from lduMatrix and solver controls
    GAMGSolver
    (
      const word& fieldName,
      const lduMatrix& matrix,
      const FieldField<Field, scalar>& interfaceBouCoeffs,
      const FieldField<Field, scalar>& interfaceIntCoeffs,
      const lduInterfaceFieldPtrsList& interfaces,
      const dictionary& solverControls
    );
  //- Destructor
  virtual ~GAMGSolver();
  // Member Functions
    //- Solve
    virtual solverPerformance solve
    (
      scalarField& psi,
      const scalarField& source,
      const direction cmpt=0
    ) const;
};
}  // namespace mousse
#endif
