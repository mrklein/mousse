#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TMPPIC_TAVERAGING_METHODS_TDUAL_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TMPPIC_TAVERAGING_METHODS_TDUAL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::AveragingMethods::Dual
// Description
//   Dual-mesh lagrangian averaging procedure.
//   Point values are summed using the tetrahedral decomposition of the
//   computational cells. Summation is done in the cells, and also in the
//   terahedrons surrounding each point. The latter forms a type of dual mesh.
//   The interpolation is weighted by proximity to the cell centre or point, as
//   calculated by the barycentric coordinate within the tethrahedron.
//   Values are interpolated linearly across the tethrahedron. Gradients are
//   calculated directly from the point values using a first order finite
//   element basis. The computed gradient is assumed constant over the
//   tethrahedron.

#include "_averaging_method.hpp"
#include "point_mesh.hpp"
#include "tet_indices.hpp"


namespace mousse {
namespace AveragingMethods {

template<class Type>
class Dual
:
  public AveragingMethod<Type>
{
public:
  //- Public typedefs
    //- Gradient type
    typedef typename AveragingMethod<Type>::TypeGrad TypeGrad;
private:
  //- Private data
    //- Volume of the cell-centered regions
    const Field<scalar>& volumeCell_;
    //- Volume of the point-centered regions
    Field<scalar> volumeDual_;
    //- Data on the cells
    Field<Type>& dataCell_;
    //- Data on the points
    Field<Type>& dataDual_;
    //- Tet vertex labels
    mutable List<label> tetVertices_;
    //- Tet barycentric coordinates
    mutable List<scalar> tetCoordinates_;
  //- Private static member functions
    //- Return the size of the FieldField parts
    static autoPtr<labelList> size(const fvMesh &mesh);
  //- Private member functions
    //- Calculate indices and barycentric coordinates within a tetrahedron
    void tetGeometry
    (
      const point position,
      const tetIndices& tetIs
    ) const;
    //- Sync point data over processor boundaries
    void syncDualData();
public:
  //- Runtime type information
  TYPE_NAME("dual");
  //- Constructors
    //- Construct from components
    Dual
    (
      const IOobject& io,
      const dictionary& dict,
      const fvMesh &mesh
    );
    //- Construct a copy
    Dual(const Dual<Type>& am);
    //- Construct and return a clone
    virtual autoPtr<AveragingMethod<Type>> clone() const
    {
      return
        autoPtr<AveragingMethod<Type>>
        {
          new Dual<Type>{*this}
        };
    }
  //- Destructor
  virtual ~Dual();
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
    //- Calculate the average
    void average();
    void average(const AveragingMethod<scalar>& weight);
    //- Return an internal field of the average
    tmp<Field<Type>> internalField() const;
    //- Return an internal field of the gradient
    tmp<Field<TypeGrad>> internalFieldGrad() const;
};

}  // namespace AveragingMethods
}  // namespace mousse

#include "_dual.ipp"

#endif
