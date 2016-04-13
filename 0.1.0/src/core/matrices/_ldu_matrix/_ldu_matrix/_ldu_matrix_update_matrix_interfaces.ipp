// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_ldu_matrix.hpp"
#include "ldu_interface_field.hpp"


// Member Functions 
template<class Type, class DType, class LUType>
void mousse::LduMatrix<Type, DType, LUType>::initMatrixInterfaces
(
  const FieldField<Field, LUType>& interfaceCoeffs,
  const Field<Type>& psiif,
  Field<Type>& result
) const
{
  if (Pstream::defaultCommsType == Pstream::blocking
      || Pstream::defaultCommsType == Pstream::nonBlocking) {
    FOR_ALL(interfaces_, interfaceI) {
      if (interfaces_.set(interfaceI)) {
        interfaces_[interfaceI].initInterfaceMatrixUpdate
        (
          result,
          psiif,
          interfaceCoeffs[interfaceI],
          //Amultiplier<Type, LUType>(interfaceCoeffs[interfaceI]),
          Pstream::defaultCommsType
        );
      }
    }
  } else if (Pstream::defaultCommsType == Pstream::scheduled) {
    const lduSchedule& patchSchedule = this->patchSchedule();
    // Loop over the "global" patches are on the list of interfaces but
    // beyond the end of the schedule which only handles "normal" patches
    for
    (
      label interfaceI=patchSchedule.size()/2;
      interfaceI<interfaces_.size();
      interfaceI++
    ) {
      if (interfaces_.set(interfaceI)) {
        interfaces_[interfaceI].initInterfaceMatrixUpdate
        (
          result,
          psiif,
          interfaceCoeffs[interfaceI],
          //Amultiplier<Type, LUType>(interfaceCoeffs[interfaceI]),
          Pstream::blocking
        );
      }
    }
  } else {
    FATAL_ERROR_IN("LduMatrix<Type, DType, LUType>::initMatrixInterfaces")
      << "Unsuported communications type "
      << Pstream::commsTypeNames[Pstream::defaultCommsType]
      << exit(FatalError);
  }
}


template<class Type, class DType, class LUType>
void mousse::LduMatrix<Type, DType, LUType>::updateMatrixInterfaces
(
  const FieldField<Field, LUType>& interfaceCoeffs,
  const Field<Type>& psiif,
  Field<Type>& result
) const
{
  if (Pstream::defaultCommsType == Pstream::blocking
      || Pstream::defaultCommsType == Pstream::nonBlocking) {
    // Block until all sends/receives have been finished
    if (Pstream::defaultCommsType == Pstream::nonBlocking) {
      IPstream::waitRequests();
      OPstream::waitRequests();
    }

    FOR_ALL(interfaces_, interfaceI) {
      if (interfaces_.set(interfaceI)) {
        interfaces_[interfaceI].updateInterfaceMatrix
        (
          result,
          psiif,
          interfaceCoeffs[interfaceI],
          //Amultiplier<Type, LUType>(interfaceCoeffs[interfaceI]),
          Pstream::defaultCommsType
        );
      }
    }
  } else if (Pstream::defaultCommsType == Pstream::scheduled) {
    const lduSchedule& patchSchedule = this->patchSchedule();
    // Loop over all the "normal" interfaces relating to standard patches
    FOR_ALL(patchSchedule, i) {
      label interfaceI = patchSchedule[i].patch;
      if (interfaces_.set(interfaceI)) {
        if (patchSchedule[i].init) {
          interfaces_[interfaceI].initInterfaceMatrixUpdate
          (
            result,
            psiif,
            interfaceCoeffs[interfaceI],
           //Amultiplier<Type, LUType>(interfaceCoeffs[interfaceI]),
            Pstream::scheduled
          );
        } else {
          interfaces_[interfaceI].updateInterfaceMatrix
          (
            result,
            psiif,
            interfaceCoeffs[interfaceI],
           //Amultiplier<Type, LUType>(interfaceCoeffs[interfaceI]),
            Pstream::scheduled
          );
        }
      }
    }
    // Loop over the "global" patches are on the list of interfaces but
    // beyond the end of the schedule which only handles "normal" patches
    for
    (
      label interfaceI=patchSchedule.size()/2;
      interfaceI<interfaces_.size();
      interfaceI++
    ) {
      if (interfaces_.set(interfaceI)) {
        interfaces_[interfaceI].updateInterfaceMatrix
        (
          result,
          psiif,
          interfaceCoeffs[interfaceI],
          //Amultiplier<Type, LUType>(interfaceCoeffs[interfaceI]),
          Pstream::blocking
        );
      }
    }
  } else {
    FATAL_ERROR_IN("LduMatrix<Type, DType, LUType>::updateMatrixInterfaces")
      << "Unsuported communications type "
      << Pstream::commsTypeNames[Pstream::defaultCommsType]
      << exit(FatalError);
  }
}
