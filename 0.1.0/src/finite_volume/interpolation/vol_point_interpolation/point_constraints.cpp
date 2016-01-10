// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point_constraints.hpp"
#include "empty_point_patch.hpp"
#include "poly_mesh.hpp"
#include "point_mesh.hpp"
#include "global_mesh_data.hpp"
#include "twod_point_corrector.hpp"
#include "map_distribute.hpp"
#include "point_fields.hpp"

namespace mousse
{
// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(pointConstraints, 0);
// Private Member Functions 
void pointConstraints::makePatchPatchAddressing()
{
  if (debug)
  {
    Pout<< "pointConstraints::makePatchPatchAddressing() : "
      << "constructing boundary addressing"
      << endl << incrIndent;
  }
  const pointMesh& pMesh = mesh();
  const polyMesh& mesh = pMesh();
  const pointBoundaryMesh& pbm = pMesh.boundary();
  const polyBoundaryMesh& bm = mesh.boundaryMesh();
  // first count the total number of patch-patch points
  label nPatchPatchPoints = 0;
  FOR_ALL(pbm, patchi)
  {
    if (!isA<emptyPointPatch>(pbm[patchi]) && !pbm[patchi].coupled())
    {
      const labelList& bp = bm[patchi].boundaryPoints();
      nPatchPatchPoints += bp.size();
      if (debug)
      {
        Pout<< indent << "On patch:" << pbm[patchi].name()
          << " nBoundaryPoints:" << bp.size() << endl;
      }
    }
  }
  if (debug)
  {
    Pout<< indent << "Found nPatchPatchPoints:" << nPatchPatchPoints
      << endl;
  }
  // Go through all patches and mark up the external edge points
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // From meshpoint to index in patchPatchPointConstraints_.
  Map<label> patchPatchPointSet(2*nPatchPatchPoints);
  // Constraints (initialised to unconstrained)
  patchPatchPointConstraints_.setSize(nPatchPatchPoints);
  patchPatchPointConstraints_ = pointConstraint();
  // From constraint index to mesh point
  labelList patchPatchPoints(nPatchPatchPoints);
  label pppi = 0;
  FOR_ALL(pbm, patchi)
  {
    if (!isA<emptyPointPatch>(pbm[patchi]) && !pbm[patchi].coupled())
    {
      const labelList& bp = bm[patchi].boundaryPoints();
      const labelList& meshPoints = pbm[patchi].meshPoints();
      FOR_ALL(bp, pointi)
      {
        label ppp = meshPoints[bp[pointi]];
        Map<label>::iterator iter = patchPatchPointSet.find(ppp);
        label constraintI = -1;
        if (iter == patchPatchPointSet.end())
        {
          patchPatchPointSet.insert(ppp, pppi);
          patchPatchPoints[pppi] = ppp;
          constraintI = pppi++;
        }
        else
        {
          constraintI = iter();
        }
        // Apply to patch constraints
        pbm[patchi].applyConstraint
        (
          bp[pointi],
          patchPatchPointConstraints_[constraintI]
        );
      }
    }
  }
  if (debug)
  {
    Pout<< indent << "Have (local) constrained points:"
      << nPatchPatchPoints << endl;
  }
  // Extend set with constraints across coupled points
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  {
    const globalMeshData& gd = mesh.globalData();
    const labelListList& globalPointSlaves = gd.globalPointSlaves();
    const mapDistribute& globalPointSlavesMap = gd.globalPointSlavesMap();
    const Map<label>& cpPointMap = gd.coupledPatch().meshPointMap();
    const labelList& cpMeshPoints = gd.coupledPatch().meshPoints();
    // Constraints on coupled points
    List<pointConstraint> constraints
    (
      globalPointSlavesMap.constructSize()
    );
    // Copy from patchPatch constraints into coupledConstraints.
    FOR_ALL(pbm, patchi)
    {
      if (!isA<emptyPointPatch>(pbm[patchi]) && !pbm[patchi].coupled())
      {
        const labelList& bp = bm[patchi].boundaryPoints();
        const labelList& meshPoints = pbm[patchi].meshPoints();
        FOR_ALL(bp, pointi)
        {
          label ppp = meshPoints[bp[pointi]];
          Map<label>::const_iterator fnd = cpPointMap.find(ppp);
          if (fnd != cpPointMap.end())
          {
            // Can just copy (instead of apply) constraint
            // will already be consistent across multiple patches.
            constraints[fnd()] = patchPatchPointConstraints_
            [
              patchPatchPointSet[ppp]
            ];
          }
        }
      }
    }
    // Exchange data
    globalPointSlavesMap.distribute(constraints);
    // Combine master with slave constraints
    FOR_ALL(globalPointSlaves, pointI)
    {
      const labelList& slaves = globalPointSlaves[pointI];
      // Combine master constraint with slave constraints
      FOR_ALL(slaves, i)
      {
        constraints[pointI].combine(constraints[slaves[i]]);
      }
      // Duplicate master constraint into slave slots
      FOR_ALL(slaves, i)
      {
        constraints[slaves[i]] = constraints[pointI];
      }
    }
    // Send back
    globalPointSlavesMap.reverseDistribute
    (
      cpMeshPoints.size(),
      constraints
    );
    // Add back into patchPatch constraints
    FOR_ALL(constraints, coupledPointI)
    {
      if (constraints[coupledPointI].first() != 0)
      {
        label meshPointI = cpMeshPoints[coupledPointI];
        Map<label>::iterator iter = patchPatchPointSet.find(meshPointI);
        label constraintI = -1;
        if (iter == patchPatchPointSet.end())
        {
          //Pout<< indent << "on meshpoint:" << meshPointI
          //    << " coupled:" << coupledPointI
          //    << " at:" << mesh.points()[meshPointI]
          //    << " have new constraint:"
          //    << constraints[coupledPointI]
          //    << endl;
          // Allocate new constraint
          if (patchPatchPoints.size() <= pppi)
          {
            patchPatchPoints.setSize(pppi+100);
          }
          patchPatchPointSet.insert(meshPointI, pppi);
          patchPatchPoints[pppi] = meshPointI;
          constraintI = pppi++;
        }
        else
        {
          //Pout<< indent << "on meshpoint:" << meshPointI
          //    << " coupled:" << coupledPointI
          //    << " at:" << mesh.points()[meshPointI]
          //    << " have possibly extended constraint:"
          //    << constraints[coupledPointI]
          //    << endl;
          constraintI = iter();
        }
        // Combine (new or existing) constraint with one
        // on coupled.
        patchPatchPointConstraints_[constraintI].combine
        (
          constraints[coupledPointI]
        );
      }
    }
  }
  nPatchPatchPoints = pppi;
  patchPatchPoints.setSize(nPatchPatchPoints);
  patchPatchPointConstraints_.setSize(nPatchPatchPoints);
  if (debug)
  {
    Pout<< indent << "Have (global) constrained points:"
      << nPatchPatchPoints << endl;
  }
  // Copy out all non-trivial constraints
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  patchPatchPointConstraintPoints_.setSize(nPatchPatchPoints);
  patchPatchPointConstraintTensors_.setSize(nPatchPatchPoints);
  label nConstraints = 0;
  FOR_ALL(patchPatchPointConstraints_, i)
  {
    // Note: check for more than zero constraints. (could check for
    //       more than one constraint but what about coupled points that
    //       inherit the constraintness)
    if (patchPatchPointConstraints_[i].first() != 0)
    {
      patchPatchPointConstraintPoints_[nConstraints] =
        patchPatchPoints[i];
      patchPatchPointConstraintTensors_[nConstraints] =
        patchPatchPointConstraints_[i].constraintTransformation();
      nConstraints++;
    }
  }
  if (debug)
  {
    Pout<< indent << "Have non-trivial constrained points:"
      << nConstraints << endl;
  }
  patchPatchPointConstraints_.setSize(nConstraints);
  patchPatchPointConstraintPoints_.setSize(nConstraints);
  patchPatchPointConstraintTensors_.setSize(nConstraints);
  if (debug)
  {
    Pout<< decrIndent
      << "pointConstraints::makePatchPatchAddressing() : "
      << "finished constructing boundary addressing"
      << endl;
  }
}
// Constructors 
pointConstraints::pointConstraints(const pointMesh& pm)
:
  MeshObject<pointMesh, mousse::UpdateableMeshObject, pointConstraints>(pm)
{
  if (debug)
  {
    Pout<< "pointConstraints::pointConstraints(const pointMesh&): "
      << "Constructing from pointMesh " << pm.name()
      << endl;
  }
  makePatchPatchAddressing();
}
// Destructor 
pointConstraints::~pointConstraints()
{
  if (debug)
  {
    Pout<< "pointConstraints::~pointConstraints()" << endl;
  }
}
// Member Functions 
void pointConstraints::updateMesh(const mapPolyMesh&)
{
  makePatchPatchAddressing();
}
bool pointConstraints::movePoints()
{
  return true;
}
void pointConstraints::constrainDisplacement
(
  pointVectorField& pf,
  const bool overrideFixedValue
) const
{
  // Override constrained pointPatchField types with the constraint value.
  // This relies on only constrained pointPatchField implementing the evaluate
  // function
  pf.correctBoundaryConditions();
  // Sync any dangling points
  syncUntransformedData
  (
    pf.mesh()(),
    pf.internalField(),
    maxMagSqrEqOp<vector>()
  );
  // Apply multiple constraints on edge/corner points
  constrainCorners(pf);
  // Apply any 2D motion constraints (or should they go before
  // corner constraints?)
  twoDPointCorrector::New(mesh()()).correctDisplacement
  (
    mesh()().points(),
    pf.internalField()
  );
  if (overrideFixedValue)
  {
    setPatchFields(pf);
  }
}
// Specialisation of constrainCorners for scalars because
// no constraint need be applied
template<>
void pointConstraints::constrainCorners<scalar>
(
  GeometricField<scalar, pointPatchField, pointMesh>& /*pf*/
) const
{}
template<>
void pointConstraints::constrainCorners<label>
(
  GeometricField<label, pointPatchField, pointMesh>& /*pf*/
) const
{}
}  // namespace mousse
