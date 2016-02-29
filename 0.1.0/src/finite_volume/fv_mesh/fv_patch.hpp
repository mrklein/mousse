#ifndef FINITE_VOLUME_FV_MESH_FV_PATCH_HPP_
#define FINITE_VOLUME_FV_MESH_FV_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fvPatch
// Description
//   A finiteVolume patch using a polyPatch and a fvBoundaryMesh
// SourceFiles
//   fv_patch.cpp
//   fv_patch_new.cpp


#include "poly_patch.hpp"
#include "label_list.hpp"
#include "sub_list.hpp"
#include "type_info.hpp"
#include "tmp.hpp"
#include "primitive_fields.hpp"
#include "sub_field.hpp"
#include "fv_patch_fields_fwd.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"

namespace mousse
{
class fvBoundaryMesh;
class surfaceInterpolation;
class fvPatch
{
  // Private data
    //- Reference to the underlying polyPatch
    const polyPatch& polyPatch_;
    //- Reference to boundary mesh
    const fvBoundaryMesh& boundaryMesh_;
protected:
  // Protected Member Functions
    //- Make patch weighting factors
    virtual void makeWeights(scalarField&) const;
    //- Initialise the patches for moving points
    virtual void initMovePoints();
    //- Correct patches after moving points
    virtual void movePoints();
public:
  typedef fvBoundaryMesh BoundaryMesh;
  friend class fvBoundaryMesh;
  friend class surfaceInterpolation;
  //- Runtime type information
  TYPE_NAME(polyPatch::typeName_());
  // Declare run-time constructor selection tables
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      fvPatch,
      polyPatch,
      (const polyPatch& patch, const fvBoundaryMesh& bm),
      (patch, bm)
    );
  // Constructors
    //- Construct from polyPatch and fvBoundaryMesh
    fvPatch(const polyPatch&, const fvBoundaryMesh&);
    //- Disallow construct as copy
    fvPatch(const fvPatch&) = delete;
    //- Disallow assignment
    fvPatch& operator=(const fvPatch&) = delete;
  // Selectors
    //- Return a pointer to a new patch created on freestore from polyPatch
    static autoPtr<fvPatch> New
    (
      const polyPatch&,
      const fvBoundaryMesh&
    );
  //- Destructor
  virtual ~fvPatch();
  // Member Functions
  // Access
      //- Return the polyPatch
      const polyPatch& patch() const
      {
        return polyPatch_;
      }
      //- Return name
      const word& name() const
      {
        return polyPatch_.name();
      }
      //- Return start label of this patch in the polyMesh face list
      label start() const
      {
        return polyPatch_.start();
      }
      //- Return size
      virtual label size() const
      {
        return polyPatch_.size();
      }
      //- Return true if this patch is coupled
      virtual bool coupled() const
      {
        return polyPatch_.coupled();
      }
      //- Return true if the given type is a constraint type
      static bool constraintType(const word& pt);
      //- Return a list of all the constraint patch types
      static wordList constraintTypes();
      //- Return the index of this patch in the fvBoundaryMesh
      label index() const
      {
        return polyPatch_.index();
      }
      //- Return boundaryMesh reference
      const fvBoundaryMesh& boundaryMesh() const
      {
        return boundaryMesh_;
      }
      //- Slice list to patch
      template<class T>
      const typename List<T>::subList patchSlice(const List<T>& l) const
      {
        return typename List<T>::subList(l, size(), start());
      }
      //- Return faceCells
      virtual const labelUList& faceCells() const;
    // Access functions for geometrical data
      //- Return face centres
      const vectorField& Cf() const;
      //- Return neighbour cell centres
      tmp<vectorField> Cn() const;
      //- Return face area vectors
      const vectorField& Sf() const;
      //- Return face area magnitudes
      const scalarField& magSf() const;
      //- Return face normals
      tmp<vectorField> nf() const;
      //- Return cell-centre to face-centre vector
      //  except for coupled patches for which the cell-centre
      //  to coupled-cell-centre vector is returned
      virtual tmp<vectorField> delta() const;
    // Access functions for demand driven data
      //- Return patch weighting factors
      const scalarField& weights() const;
      //- Return the face - cell distance coeffient
      //  except for coupled patches for which the cell-centre
      //  to coupled-cell-centre distance coeffient is returned
      const scalarField& deltaCoeffs() const;
    // Evaluation functions
      //- Return given internal field next to patch as patch field
      template<class Type>
      tmp<Field<Type> > patchInternalField(const UList<Type>&) const;
      //- Return given internal field next to patch as patch field
      template<class Type>
      void patchInternalField(const UList<Type>&, Field<Type>&) const;
      //- Return the corresponding patchField of the named field
      template<class GeometricField, class Type>
      const typename GeometricField::PatchFieldType& patchField
      (
        const GeometricField&
      ) const;
      //- Lookup and return the patchField of the named field from the
      //  local objectRegistry.
      //  N.B.  The dummy pointer arguments are used if this function is
      //  instantiated within a templated function to avoid a bug in gcc.
      //  See inletOutletFvPatchField.C and outletInletFvPatchField.C
      template<class GeometricField, class Type>
      const typename GeometricField::PatchFieldType& lookupPatchField
      (
        const word& name,
        const GeometricField* = NULL,
        const Type* = NULL
      ) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "fv_patch_templates.cpp"
#endif
#endif
