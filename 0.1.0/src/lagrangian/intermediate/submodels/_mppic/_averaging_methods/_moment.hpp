// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::AveragingMethods::Moment
// Description
//   Moment lagrangian averaging procedure.
//   Point values and moments from the cell centroid are summed over
//   computational cells. A linear function is generated which has the same
//   integrated moment as that of the point data.
//   The computed linear function is used to interpolate values within a cell.
//   The gradient is calculated from the coefficients of the function, and is
//   assumed constant over the cell.
// SourceFiles
//   _moment.cpp
#ifndef _moment_hpp_
#define _moment_hpp_
#include "_averaging_method.hpp"
#include "point_mesh.hpp"
#include "tet_indices.hpp"
namespace mousse
{
namespace AveragingMethods
{
template<class Type>
class Moment
:
  public AveragingMethod<Type>
{
public:
  //- Public typedefs
    //- Gradient type
    typedef typename AveragingMethod<Type>::TypeGrad TypeGrad;
private:
  //- Private data
    //- Data mean
    Field<Type>& data_;
    //- X-data moment
    Field<Type>& dataX_;
    //- Y-data moment
    Field<Type>& dataY_;
    //- Z-data moment
    Field<Type>& dataZ_;
    //- Transform tensor from moment to gradient
    Field<symmTensor> transform_;
    //- Length scale for moment values
    Field<scalar> scale_;
  //- Private member functions
    //- Re-calculate gradient
    virtual void updateGrad();
public:
  //- Runtime type information
  TypeName("moment");
  //- Constructors
    //- Construct from components
    Moment
    (
      const IOobject& io,
      const dictionary& dict,
      const fvMesh &mesh
    );
    //- Construct a copy
    Moment(const Moment<Type>& am);
    //- Construct and return a clone
    virtual autoPtr<AveragingMethod<Type> > clone() const
    {
      return autoPtr<AveragingMethod<Type> >
      (
        new Moment<Type>(*this)
      );
    }
  //- Destructor
  virtual ~Moment();
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
#   include "_moment.cpp"
#endif
#endif
