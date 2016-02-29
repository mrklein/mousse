#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TMPPIC_TAVERAGING_METHODS_TBASIC_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TMPPIC_TAVERAGING_METHODS_TBASIC_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::AveragingMethods::Basic
// Description
//   Basic lagrangian averaging procedure.
//   This is a cell-volume based average. Point values are summed over the
//   computational cells and the result is divided by the cell volume.
//   Interpolation is done assuming a constant value over each cells. Cell
//   gradients are calculated by the default fvc::grad scheme, and are also
//   assumed constant when interpolated.
// SourceFiles
//   _basic.cpp
#include "_averaging_method.hpp"
#include "point_mesh.hpp"
#include "tet_indices.hpp"
namespace mousse
{
namespace AveragingMethods
{
template<class Type>
class Basic
:
  public AveragingMethod<Type>
{
public:
  //- Public typedefs
    //- Gradient type
    typedef typename AveragingMethod<Type>::TypeGrad TypeGrad;
private:
  //- Private data
    //- Cell average field
    Field<Type>& data_;
    //- Gradient field
    Field<TypeGrad> dataGrad_;
  //- Private member functions
    //- Re-calculate gradient
    virtual void updateGrad();
public:
  //- Runtime type information
  TYPE_NAME("basic");
  //- Constructors
    //- Construct from components
    Basic
    (
      const IOobject& io,
      const dictionary& dict,
      const fvMesh &mesh
    );
    //- Construct a copy
    Basic(const Basic<Type>& am);
    //- Construct and return a clone
    virtual autoPtr<AveragingMethod<Type> > clone() const
    {
      return autoPtr<AveragingMethod<Type> >
      (
        new Basic<Type>(*this)
      );
    }
  //- Destructor
  virtual ~Basic();
  //- Member Functions
    //- Add point value to interpolation
    void add
    (
      const point position,
      const tetIndices& tetIs,
      const Type& value
    );
    //- Interpolate
    Type interpolate
    (
      const point position,
      const tetIndices& tetIs
    ) const;
    //- Interpolate gradient
    TypeGrad interpolateGrad
    (
      const point position,
      const tetIndices& tetIs
    ) const;
    //- Return an internal field of the average
    tmp<Field<Type> > internalField() const;
    //- Return an internal field of the gradient
    tmp<Field<TypeGrad> > internalFieldGrad() const;
};
}  // namespace AveragingMethods
}  // namespace mousse
#ifdef NoRepository
#   include "_basic.cpp"
#endif
#endif
