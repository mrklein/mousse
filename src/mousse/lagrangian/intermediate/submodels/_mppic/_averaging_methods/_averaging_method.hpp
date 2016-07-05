#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TMPPIC_TAVERAGING_METHODS_TAVERAGING_METHOD_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TMPPIC_TAVERAGING_METHODS_TAVERAGING_METHOD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::AveragingMethod
// Description
//   Base class for lagrangian averaging methods.

#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"


namespace mousse {

template<class Type>
class AveragingMethod
:
  public regIOobject,
  public FieldField<Field, Type>
{
protected:
  //- Protected typedefs
    //- Gradient type
    typedef typename outerProduct<vector, Type>::type TypeGrad;
  //- Protected data
    //- Dictionary
    const dictionary& dict_;
    //- The mesh on which the averaging is to be done
    const fvMesh& mesh_;
  //- Protected member functions
    //- Update the gradient calculation
    virtual void updateGrad();
public:
  //- Runtime type information
  TYPE_NAME("averagingMethod");
  //- Declare runtime constructor selection table
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    AveragingMethod,
    dictionary,
    (
      const IOobject& io,
      const dictionary& dict,
      const fvMesh& mesh
    ),
    (io, dict, mesh)
  );
  //- Constructors
    //- Construct from components
    AveragingMethod
    (
      const IOobject& io,
      const dictionary& dict,
      const fvMesh& mesh,
      const labelList& size
    );
    //- Construct a copy
    AveragingMethod(const AveragingMethod<Type>& am);
    //- Construct and return a clone
    virtual autoPtr<AveragingMethod<Type>> clone() const = 0;
  //- Selector
  static autoPtr<AveragingMethod<Type>> New
  (
    const IOobject& io,
    const dictionary& dict,
    const fvMesh& mesh
  );
  //- Destructor
  virtual ~AveragingMethod();
  //- Member Functions
    //- Add point value to interpolation
    virtual void add
    (
      const point position,
      const tetIndices& tetIs,
      const Type& value
    ) = 0;
    //- Interpolate
    virtual Type interpolate
    (
      const point position,
      const tetIndices& tetIs
    ) const = 0;
    //- Interpolate gradient
    virtual TypeGrad interpolateGrad
    (
      const point position,
      const tetIndices& tetIs
    ) const = 0;
    //- Calculate the average
    virtual void average();
    virtual void average(const AveragingMethod<scalar>& weight);
    //- Dummy write
    virtual bool writeData(Ostream&) const;
    //- Write using setting from DB
    virtual bool write() const;
    //- Return an internal field of the average
    virtual tmp<Field<Type>> internalField() const = 0;
    //- Assign to another average
    inline void operator=(const AveragingMethod<Type>& x);
    //- Assign to value
    inline void operator=(const Type& x);
    //- Assign to tmp
    inline void operator=(tmp<FieldField<Field, Type>> x);
    //- Add-equal tmp
    inline void operator+=(tmp<FieldField<Field, Type>> x);
    //- Multiply-equal tmp
    inline void operator*=(tmp<FieldField<Field, Type>> x);
    //- Divide-equal tmp
    inline void operator/=(tmp<FieldField<Field, scalar>> x);
};

}  // namespace mousse


// Member Functions 
template<class Type>
inline void mousse::AveragingMethod<Type>::operator=
(
  const AveragingMethod<Type>& x
)
{
  FieldField<Field, Type>::operator=(x);
  updateGrad();
}


template<class Type>
inline void mousse::AveragingMethod<Type>::operator=
(
  const Type& x
)
{
  FieldField<Field, Type>::operator=(x);
  updateGrad();
}


template<class Type>
inline void mousse::AveragingMethod<Type>::operator=
(
  tmp<FieldField<Field, Type>> x
)
{
  FieldField<Field, Type>::operator=(x());
  updateGrad();
}


template<class Type>
inline void mousse::AveragingMethod<Type>::operator+=
(
  tmp<FieldField<Field, Type>> x
)
{
  FieldField<Field, Type>::operator+=(x());
  updateGrad();
}


template<class Type>
inline void mousse::AveragingMethod<Type>::operator*=
(
  tmp<FieldField<Field, Type>> x
)
{
  FieldField<Field, Type>::operator*=(x());
  updateGrad();
}


template<class Type>
inline void mousse::AveragingMethod<Type>::operator/=
(
  tmp<FieldField<Field, scalar>> x
)
{
  FieldField<Field, Type>::operator/=(x());
  updateGrad();
}


#include "_averaging_method.ipp"

#endif
