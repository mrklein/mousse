// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::regionCoupledBase
// Description
//   Base class with common functinality for regionCoupled polyPatch.
//   It includes AMI.
// SourceFiles
//   region_coupled_base.cpp
#ifndef region_coupled_base_hpp_
#define region_coupled_base_hpp_
#include "ami_patch_to_patch_interpolation.hpp"
#include "poly_boundary_mesh.hpp"
namespace mousse
{
class regionCoupledBase
{
private:
  // Private data
    //- Patch to sample
    const polyPatch& patch_;
    //- Name of other half
    const word nbrPatchName_;
    //- Index of other half
    mutable label nbrPatchID_;
    //- Name of other region
    const word nbrRegionName_;
    //- Same region
    const bool sameRegion_;
    //- AMI interpolation class
    mutable autoPtr<AMIPatchToPatchInterpolation> AMIPtr_;
    //- Flag to indicate that slave patch should be reversed for AMI
    const bool AMIReverse_;
    //- Projection surface
    mutable autoPtr<searchableSurface> surfPtr_;
    //- Dictionary used during projection surface construction
    const dictionary surfDict_;
  // Private Member Functions
    //- Reset the AMI interpolator
    void resetAMI() const;
protected:
    //- Clear geometry
    virtual void clearGeom();
public:
  //- Runtime type information
  TypeName("regionCoupledBase");
  // Constructors
    //- Construct from patch
    regionCoupledBase(const polyPatch&);
    //- Construct from dictionary
    regionCoupledBase(const polyPatch&, const dictionary&);
    //- Construct as copy, resetting patch
    regionCoupledBase(const polyPatch&, const regionCoupledBase&);
  //- Destructor
  virtual ~regionCoupledBase();
  // Member Functions
    // Access
      //- Neighbour patch name
      const word& nbrPatchName() const
      {
        return nbrPatchName_;
      }
      //- Neighbour region name
      const word& nbrRegionName() const
      {
        return nbrRegionName_;
      }
      //- Cached sampleRegion != mesh.name()
      bool sameRegion() const
      {
        return sameRegion_;
      }
      //- Neighbour patch ID
      label neighbPatchID() const;
      //- Does this side own the patch?
      bool owner() const;
      //- Return a reference to the neighbour patch
      const regionCoupledBase& neighbPatch() const;
      //- Return a reference to the projection surface
      const autoPtr<searchableSurface>& surfPtr() const;
      //- Return a reference to the AMI interpolator
      const AMIPatchToPatchInterpolation& AMI() const;
      // Interpolations
        //- Interpolate field
        template<class Type>
        tmp<Field<Type> > interpolate(const Field<Type>& fld) const;
        //- Interpolate tmp field
        template<class Type>
        tmp<Field<Type> > interpolate
        (
          const tmp<Field<Type> >& tFld
        ) const;
        //- Low-level interpolate List
        template<class Type, class BinaryOp>
        void interpolate
        (
          const UList<Type>& fld,
          const BinaryOp& bop,
          List<Type>& result
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
    //- Return the type
    virtual const word& regionCoupleType() const = 0;
    //- Write the polyPatch data as a dictionary
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#ifdef NoRepository
  #include "region_coupled_base_templates.cpp"
#endif
#endif
