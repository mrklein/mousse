// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// \defgroup grpBoundaryConditions Boundary Conditions
// @{
//   This group contains OpenFOAM boundary condition types.  All conditions
//   are derived from the base mousse::fvPatchField class.  Patch values are
//   typically updated using the virtual functions
//   - mousse::fvPatchField::updateCoeffs()
//   - mousse::fvPatchField::evaluate()
//   The difference between the methods is based on when the patch values are
//   updated.  When the condition is applied to a solution variable, the
//   call to updateCoeffs() occurs as a preliminary step of the
//   \<matrix\>.solve().  The evaluate() method is invoked after, or independent
//   of the matrix solve, via a call to \<field\>.correctBoundaryConditions().
// @}
// \defgroup grpConstraintBoundaryConditions Constraint boundary Conditions
// @{
//   \ingroup grpBoundaryConditions
//   This group contains constraint boundary condition types.  These conditions
//   are mainly employed to reduced dimensioned cases.
// @}
// \defgroup grpInletBoundaryConditions Inlet boundary Conditions
// @{
//   \ingroup grpBoundaryConditions
//   This group contains inlet boundary condition types
// @}
// \defgroup grpOutletBoundaryConditions Outlet boundary Conditions
// @{
//   \ingroup grpBoundaryConditions
//   This group contains outlet boundary condition types
// @}
// \defgroup grpGenericBoundaryConditions Generic boundary Conditions
// @{
//   \ingroup grpBoundaryConditions
//   This group contains generic boundary condition types
// @}
// \defgroup grpCoupledBoundaryConditions Coupled boundary Conditions
// @{
//   \ingroup grpBoundaryConditions
//   This group contains coupled boundary condition types
// @}
// \defgroup grpWallBoundaryConditions Wall boundary Conditions
// @{
//   \ingroup grpBoundaryConditions
//   This group contains wall boundary condition types
// @}
