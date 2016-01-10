// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "gamg_interface.hpp"
// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(GAMGInterface, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(GAMGInterface, lduInterface);
DEFINE_RUN_TIME_SELECTION_TABLE(GAMGInterface, Istream);

}
// Constructors 
mousse::GAMGInterface::GAMGInterface
(
  const label index,
  const lduInterfacePtrsList& coarseInterfaces,
  Istream& is
)
:
  index_(index),
  coarseInterfaces_(coarseInterfaces),
  faceCells_(is),
  faceRestrictAddressing_(is)
{}
// Member Functions 
void mousse::GAMGInterface::combine(const GAMGInterface& coarseGi)
{
  const labelList& coarseFra = coarseGi.faceRestrictAddressing_;
  FOR_ALL(faceRestrictAddressing_, ffi)
  {
    faceRestrictAddressing_[ffi] = coarseFra[faceRestrictAddressing_[ffi]];
  }
  faceCells_ = coarseGi.faceCells_;
}
mousse::tmp<mousse::labelField> mousse::GAMGInterface::interfaceInternalField
(
  const labelUList& internalData
) const
{
  return interfaceInternalField<label>(internalData);
}
mousse::tmp<mousse::scalarField> mousse::GAMGInterface::agglomerateCoeffs
(
  const scalarField& fineCoeffs
) const
{
  tmp<scalarField> tcoarseCoeffs(new scalarField(size(), 0.0));
  scalarField& coarseCoeffs = tcoarseCoeffs();
  if (fineCoeffs.size() != faceRestrictAddressing_.size())
  {
    FATAL_ERROR_IN
    (
      "GAMGInterface::agglomerateCoeffs(const scalarField&) const"
    )   << "Size of coefficients " << fineCoeffs.size()
      << " does not correspond to the size of the restriction "
      << faceRestrictAddressing_.size()
      << abort(FatalError);
  }
  if (debug && max(faceRestrictAddressing_) > size())
  {
    FATAL_ERROR_IN
    (
      "GAMGInterface::agglomerateCoeffs(const scalarField&) const"
    )   << "Face restrict addressing addresses outside of coarse interface"
      << " size. Max addressing:" << max(faceRestrictAddressing_)
      << " coarse size:" << size()
      << abort(FatalError);
  }
  FOR_ALL(faceRestrictAddressing_, ffi)
  {
    coarseCoeffs[faceRestrictAddressing_[ffi]] += fineCoeffs[ffi];
  }
  return tcoarseCoeffs;
}
void mousse::GAMGInterface::write(Ostream& os) const
{
  os  << faceCells_ << token::SPACE << faceRestrictAddressing_;
}
