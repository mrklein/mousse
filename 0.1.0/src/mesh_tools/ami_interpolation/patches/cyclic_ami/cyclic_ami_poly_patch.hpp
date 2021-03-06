#ifndef MESH_TOOLS_AMI_INTERPOLATION_PATCHES_CYCLIC_AMI_CYCLIC_AMI_POLY_PATCH_CYCLIC_AMI_POLY_PATCH_HPP_
#define MESH_TOOLS_AMI_INTERPOLATION_PATCHES_CYCLIC_AMI_CYCLIC_AMI_POLY_PATCH_CYCLIC_AMI_POLY_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cyclicAMIPolyPatch
// Description
//   Cyclic patch for Arbitrary Mesh Interface (AMI)

#include "coupled_poly_patch.hpp"
#include "ami_patch_to_patch_interpolation.hpp"
#include "poly_boundary_mesh.hpp"
#include "couple_group_identifier.hpp"


namespace mousse {

class cyclicAMIPolyPatch
:
  public coupledPolyPatch
{
private:
  // Private Member Functions
    //- Return normal of face at max distance from rotation axis
    vector findFaceNormalMaxRadius(const pointField& faceCentres) const;
    void calcTransforms
    (
      const primitivePatch& half0,
      const pointField& half0Ctrs,
      const vectorField& half0Areas,
      const pointField& half1Ctrs,
      const vectorField& half1Areas
    );
protected:
  // Protected data
    //- Name of other half
    mutable word nbrPatchName_;
    //- Optional patchGroup to find neighbPatch
    const coupleGroupIdentifier coupleGroup_;
    //- Index of other half
    mutable label nbrPatchID_;
    // Transformations
      // For rotation
        //- Axis of rotation for rotational cyclics
        vector rotationAxis_;
        //- Point on axis of rotation for rotational cyclics
        point rotationCentre_;
        //- Flag to show whether the rotation angle is defined
        bool rotationAngleDefined_;
        //- Rotation angle
        scalar rotationAngle_;
      // For translation
        //- Translation vector
        vector separationVector_;
    //- AMI interpolation class
    mutable autoPtr<AMIPatchToPatchInterpolation> AMIPtr_;
    //- Flag to indicate that slave patch should be reversed for AMI
    const bool AMIReverse_;
    //- Flag to indicate that patches should match/overlap
    bool AMIRequireMatch_;
    //- Low weight correction threshold for AMI
    const scalar AMILowWeightCorrection_;
    //- Projection surface
    mutable autoPtr<searchableSurface> surfPtr_;
    //- Dictionary used during projection surface construction
    const dictionary surfDict_;
  // Protected Member Functions
    //- Reset the AMI interpolator
    virtual void resetAMI
    (
      const AMIPatchToPatchInterpolation::interpolationMethod& AMIMethod =
        AMIPatchToPatchInterpolation::imFaceAreaWeight
    ) const;
    //- Recalculate the transformation tensors
    virtual void calcTransforms();
    //- Initialise the calculation of the patch geometry
    virtual void initGeometry(PstreamBuffers&);
    //- Calculate the patch geometry
    virtual void calcGeometry(PstreamBuffers&);
    //- Initialise the patches for moving points
    virtual void initMovePoints(PstreamBuffers& pBufs, const pointField&);
    //- Correct patches after moving points
    virtual void movePoints(PstreamBuffers& pBufs, const pointField&);
    //- Initialise the update of the patch topology
    virtual void initUpdateMesh(PstreamBuffers&);
    //- Update of the patch topology
    virtual void updateMesh(PstreamBuffers&);
    //- Clear geometry
    virtual void clearGeom();
public:
  //- Runtime type information
  TYPE_NAME("cyclicAMI");
  // Constructors
    //- Construct from (base couped patch) components
    cyclicAMIPolyPatch
    (
      const word& name,
      const label size,
      const label start,
      const label index,
      const polyBoundaryMesh& bm,
      const word& patchType,
      const transformType transform = UNKNOWN
    );
    //- Construct from dictionary
    cyclicAMIPolyPatch
    (
      const word& name,
      const dictionary& dict,
      const label index,
      const polyBoundaryMesh& bm,
      const word& patchType
    );
    //- Construct as copy, resetting the boundary mesh
    cyclicAMIPolyPatch(const cyclicAMIPolyPatch&, const polyBoundaryMesh&);
    //- Construct given the original patch and resetting the
    //  face list and boundary mesh information
    cyclicAMIPolyPatch
    (
      const cyclicAMIPolyPatch& pp,
      const polyBoundaryMesh& bm,
      const label index,
      const label newSize,
      const label newStart,
      const word& nbrPatchName
    );
    //- Construct given the original patch and a map
    cyclicAMIPolyPatch
    (
      const cyclicAMIPolyPatch& pp,
      const polyBoundaryMesh& bm,
      const label index,
      const labelUList& mapAddressing,
      const label newStart
    );
    //- Construct and return a clone, resetting the boundary mesh
    virtual autoPtr<polyPatch> clone(const polyBoundaryMesh& bm) const
    {
      return autoPtr<polyPatch>(new cyclicAMIPolyPatch(*this, bm));
    }
    //- Construct and return a clone, resetting the face list
    //  and boundary mesh
    virtual autoPtr<polyPatch> clone
    (
      const polyBoundaryMesh& bm,
      const label index,
      const label newSize,
      const label newStart
    ) const
    {
      return autoPtr<polyPatch>
      {
        new cyclicAMIPolyPatch
        {
          *this,
          bm,
          index,
          newSize,
          newStart,
          nbrPatchName_
        }
      };
    }
    //- Construct and return a clone, resetting the face list
    //  and boundary mesh
    virtual autoPtr<polyPatch> clone
    (
      const polyBoundaryMesh& bm,
      const label index,
      const labelUList& mapAddressing,
      const label newStart
    ) const
    {
      return autoPtr<polyPatch>
      {
        new cyclicAMIPolyPatch
        {
          *this,
          bm,
          index,
          mapAddressing,
          newStart
        }
      };
    }
  //- Destructor
  virtual ~cyclicAMIPolyPatch();
  // Member Functions
    // Access
      //- Is patch 'coupled'. Note that on AMI the geometry is not
      //  coupled but the fields are!
      virtual bool coupled() const
      {
        return false;
      }
      //- Neighbour patch name
      inline const word& neighbPatchName() const;
      //- Neighbour patch ID
      virtual label neighbPatchID() const;
      //- Does this side own the patch?
      virtual bool owner() const;
      //- Return a reference to the neighbour patch
      virtual const cyclicAMIPolyPatch& neighbPatch() const;
      //- Return a reference to the projection surface
      const autoPtr<searchableSurface>& surfPtr() const;
      //- Return a reference to the AMI interpolator
      const AMIPatchToPatchInterpolation& AMI() const;
      //- Return true if applying the low weight correction
      bool applyLowWeightCorrection() const;
      // Transformations
        //- Axis of rotation for rotational cyclic AMI
        inline const vector& rotationAxis() const;
        //- Point on axis of rotation for rotational cyclic AMI
        inline const point& rotationCentre() const;
        //- Translation vector for translational cyclic AMI
        inline const vector& separationVector() const;
        //- Transform patch-based positions from nbr side to this side
        virtual void transformPosition(pointField&) const;
        //- Transform a patch-based position from nbr side to this side
        virtual void transformPosition
        (
          point& l,
          const label faceI
        ) const;
        //- Transform a patch-based position from this side to nbr side
        virtual void reverseTransformPosition
        (
          point& l,
          const label faceI
        ) const;
        //- Transform a patch-based direction from this side to nbr side
        virtual void reverseTransformDirection
        (
          vector& d,
          const label faceI
        ) const;
      // Interpolations
        //- Interpolate field
        template<class Type>
        tmp<Field<Type> > interpolate
        (
          const Field<Type>& fld,
          const UList<Type>& defaultValues = UList<Type>()
        ) const;
        //- Interpolate tmp field
        template<class Type>
        tmp<Field<Type> > interpolate
        (
          const tmp<Field<Type> >& tFld,
          const UList<Type>& defaultValues = UList<Type>()
        ) const;
        //- Low-level interpolate List
        template<class Type, class CombineOp>
        void interpolate
        (
          const UList<Type>& fld,
          const CombineOp& cop,
          List<Type>& result,
          const UList<Type>& defaultValues = UList<Type>()
        ) const;
    //- Calculate the patch geometry
    virtual void calcGeometry
    (
      const primitivePatch& referPatch,
      const pointField& thisCtrs,
      const vectorField& thisAreas,
      const pointField& thisCc,
      const pointField& nbrCtrs,
      const vectorField& nbrAreas,
      const pointField& nbrCc
    );
    //- Initialize ordering for primitivePatch. Does not
    //  refer to *this (except for name() and type() etc.)
    virtual void initOrder
    (
      PstreamBuffers&,
      const primitivePatch&
    ) const;
    //- Return new ordering for primitivePatch.
    //  Ordering is -faceMap: for every face
    //  index of the new face -rotation:for every new face the clockwise
    //  shift of the original face. Return false if nothing changes
    //  (faceMap is identity, rotation is 0), true otherwise.
    virtual bool order
    (
      PstreamBuffers&,
      const primitivePatch&,
      labelList& faceMap,
      labelList& rotation
    ) const;
    //- Return face index on neighbour patch which shares point p
    //  following trajectory vector n
    label pointFace
    (
      const label faceI,
      const vector& n,
      point& p
    ) const;
    //- Write the polyPatch data as a dictionary
    virtual void write(Ostream&) const;
};

}  // namespace mousse


inline const mousse::word& mousse::cyclicAMIPolyPatch::neighbPatchName() const
{
  if (nbrPatchName_.empty()) {
    // Try and use patchGroup to find samplePatch and sampleRegion
    label patchID = coupleGroup_.findOtherPatchID(*this);
    nbrPatchName_ = boundaryMesh()[patchID].name();
  }
  return nbrPatchName_;
}


inline const mousse::vector& mousse::cyclicAMIPolyPatch::rotationAxis() const
{
  return rotationAxis_;
}


inline const mousse::point& mousse::cyclicAMIPolyPatch::rotationCentre() const
{
  return rotationCentre_;
}


inline const mousse::vector& mousse::cyclicAMIPolyPatch::separationVector() const
{
  return separationVector_;
}

#include "cyclic_ami_poly_patch.ipp"

#endif
