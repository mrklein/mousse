// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ldu_matrix.hpp"


// Private Member Functions 
void mousse::lduMatrix::initMatrixInterfaces
(
  const FieldField<Field, scalar>& coupleCoeffs,
  const lduInterfaceFieldPtrsList& interfaces,
  const scalarField& psiif,
  scalarField& result,
  const direction cmpt
) const
{
  if (Pstream::defaultCommsType == Pstream::blocking
      || Pstream::defaultCommsType == Pstream::nonBlocking) {
    FOR_ALL(interfaces, interfaceI) {
      if (interfaces.set(interfaceI)) {
        interfaces[interfaceI].initInterfaceMatrixUpdate
        (
          result,
          psiif,
          coupleCoeffs[interfaceI],
          cmpt,
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
      interfaceI<interfaces.size();
      interfaceI++
    ) {
      if (interfaces.set(interfaceI)) {
        interfaces[interfaceI].initInterfaceMatrixUpdate
        (
          result,
          psiif,
          coupleCoeffs[interfaceI],
          cmpt,
          Pstream::blocking
        );
      }
    }
  } else {
    FATAL_ERROR_IN("lduMatrix::initMatrixInterfaces(..)")
      << "Unsuported communications type "
      << Pstream::commsTypeNames[Pstream::defaultCommsType]
      << exit(FatalError);
  }
}


void mousse::lduMatrix::updateMatrixInterfaces
(
  const FieldField<Field, scalar>& coupleCoeffs,
  const lduInterfaceFieldPtrsList& interfaces,
  const scalarField& psiif,
  scalarField& result,
  const direction cmpt
) const
{
  if (Pstream::defaultCommsType == Pstream::blocking) {
    FOR_ALL(interfaces, interfaceI) {
      if (interfaces.set(interfaceI)) {
        interfaces[interfaceI].updateInterfaceMatrix
        (
          result,
          psiif,
          coupleCoeffs[interfaceI],
          cmpt,
          Pstream::defaultCommsType
        );
      }
    }
  } else if (Pstream::defaultCommsType == Pstream::nonBlocking) {
    // Try and consume interfaces as they become available
    bool allUpdated = false;
    for (label i = 0; i < UPstream::nPollProcInterfaces; i++) {
      allUpdated = true;
      FOR_ALL(interfaces, interfaceI) {
        if (interfaces.set(interfaceI)) {
          if (!interfaces[interfaceI].updatedMatrix()) {
            if (interfaces[interfaceI].ready()) {
              interfaces[interfaceI].updateInterfaceMatrix
              (
                result,
                psiif,
                coupleCoeffs[interfaceI],
                cmpt,
                Pstream::defaultCommsType
              );
            } else {
              allUpdated = false;
            }
          }
        }
      }
      if (allUpdated) {
        break;
      }
    }
    // Block for everything
    if (Pstream::parRun()) {
      if (allUpdated) {
        // All received. Just remove all storage of requests
        // Note that we don't know what starting number of requests
        // was before start of sends and receives (since set from
        // initMatrixInterfaces) so set to 0 and loose any in-flight
        // requests.
        UPstream::resetRequests(0);
      } else {
        // Block for all requests and remove storage
        UPstream::waitRequests();
      }
    }
    // Consume
    FOR_ALL(interfaces, interfaceI) {
      if (interfaces.set(interfaceI)
          && !interfaces[interfaceI].updatedMatrix()) {
        interfaces[interfaceI].updateInterfaceMatrix
        (
          result,
          psiif,
          coupleCoeffs[interfaceI],
          cmpt,
          Pstream::defaultCommsType
        );
      }
    }
  } else if (Pstream::defaultCommsType == Pstream::scheduled) {
    const lduSchedule& patchSchedule = this->patchSchedule();
    // Loop over all the "normal" interfaces relating to standard patches
    FOR_ALL(patchSchedule, i) {
      label interfaceI = patchSchedule[i].patch;
      if (interfaces.set(interfaceI)) {
        if (patchSchedule[i].init) {
          interfaces[interfaceI].initInterfaceMatrixUpdate
          (
            result,
            psiif,
            coupleCoeffs[interfaceI],
            cmpt,
            Pstream::scheduled
          );
        } else {
          interfaces[interfaceI].updateInterfaceMatrix
          (
            result,
            psiif,
            coupleCoeffs[interfaceI],
            cmpt,
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
      interfaceI<interfaces.size();
      interfaceI++
    ) {
      if (interfaces.set(interfaceI)) {
        interfaces[interfaceI].updateInterfaceMatrix
        (
          result,
          psiif,
          coupleCoeffs[interfaceI],
          cmpt,
          Pstream::blocking
        );
      }
    }
  } else {
    FATAL_ERROR_IN("lduMatrix::updateMatrixInterfaces(..)")
      << "Unsuported communications type "
      << Pstream::commsTypeNames[Pstream::defaultCommsType]
      << exit(FatalError);
  }
}
