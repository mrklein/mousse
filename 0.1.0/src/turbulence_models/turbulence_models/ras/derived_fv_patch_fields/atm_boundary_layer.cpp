// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "atm_boundary_layer.hpp"
namespace mousse
{
// Constructors 
atmBoundaryLayer::atmBoundaryLayer()
:
  flowDir_(pTraits<vector>::zero),
  zDir_(pTraits<vector>::zero),
  kappa_(0.41),
  Cmu_(0.09),
  Uref_(0),
  Zref_(0),
  z0_(0),
  zGround_(0),
  Ustar_(0)
{}
atmBoundaryLayer::atmBoundaryLayer(const vectorField& p, const dictionary& dict)
:
  flowDir_(dict.lookup("flowDir")),
  zDir_(dict.lookup("zDir")),
  kappa_(dict.lookupOrDefault<scalar>("kappa", 0.41)),
  Cmu_(dict.lookupOrDefault<scalar>("Cmu", 0.09)),
  Uref_(readScalar(dict.lookup("Uref"))),
  Zref_(readScalar(dict.lookup("Zref"))),
  z0_("z0", dict, p.size()),
  zGround_("zGround", dict, p.size()),
  Ustar_(p.size())
{
  if (mag(flowDir_) < SMALL || mag(zDir_) < SMALL)
  {
    FATAL_ERROR_IN
    (
      "atmBoundaryLayer(const dictionary&)"
    )   << "magnitude of n or z must be greater than zero"
      << abort(FatalError);
  }
  // Ensure direction vectors are normalized
  flowDir_ /= mag(flowDir_);
  zDir_ /= mag(zDir_);
  Ustar_ = kappa_*Uref_/(log((Zref_ + z0_)/z0_));
}
atmBoundaryLayer::atmBoundaryLayer
(
  const atmBoundaryLayer& ptf,
  const fvPatchFieldMapper& mapper
)
:
  flowDir_(ptf.flowDir_),
  zDir_(ptf.zDir_),
  kappa_(ptf.kappa_),
  Cmu_(ptf.Cmu_),
  Uref_(ptf.Uref_),
  Zref_(ptf.Zref_),
  z0_(ptf.z0_, mapper),
  zGround_(ptf.zGround_, mapper),
  Ustar_(ptf.Ustar_, mapper)
{}
atmBoundaryLayer::atmBoundaryLayer(const atmBoundaryLayer& blpvf)
:
  flowDir_(blpvf.flowDir_),
  zDir_(blpvf.zDir_),
  kappa_(blpvf.kappa_),
  Cmu_(blpvf.Cmu_),
  Uref_(blpvf.Uref_),
  Zref_(blpvf.Zref_),
  z0_(blpvf.z0_),
  zGround_(blpvf.zGround_),
  Ustar_(blpvf.Ustar_)
{}
// Member Functions 
void atmBoundaryLayer::autoMap(const fvPatchFieldMapper& m)
{
  z0_.autoMap(m);
  zGround_.autoMap(m);
  Ustar_.autoMap(m);
}
void atmBoundaryLayer::rmap
(
  const atmBoundaryLayer& blptf,
  const labelList& addr
)
{
  z0_.rmap(blptf.z0_, addr);
  zGround_.rmap(blptf.zGround_, addr);
  Ustar_.rmap(blptf.Ustar_, addr);
}
tmp<vectorField> atmBoundaryLayer::U(const vectorField& p) const
{
  scalarField Un
  (
    (Ustar_/kappa_)
   *log(((zDir_ & p) - zGround_ + z0_)/z0_)
  );
  return flowDir_*Un;
}
tmp<scalarField> atmBoundaryLayer::k(const vectorField& /*p*/) const
{
  return sqr(Ustar_)/sqrt(Cmu_);
}
tmp<scalarField> atmBoundaryLayer::epsilon(const vectorField& p) const
{
  return pow3(Ustar_)/(kappa_*((zDir_ & p) - zGround_ + z0_));
}
void atmBoundaryLayer::write(Ostream& os) const
{
  z0_.writeEntry("z0", os) ;
  os.writeKeyword("flowDir")
    << flowDir_ << token::END_STATEMENT << nl;
  os.writeKeyword("zDir")
    << zDir_ << token::END_STATEMENT << nl;
  os.writeKeyword("kappa")
    << kappa_ << token::END_STATEMENT << nl;
  os.writeKeyword("Cmu")
    << Cmu_ << token::END_STATEMENT << nl;
  os.writeKeyword("Uref")
    << Uref_ << token::END_STATEMENT << nl;
  os.writeKeyword("Zref")
    << Zref_ << token::END_STATEMENT << nl;
  zGround_.writeEntry("zGround", os) ;
}
}  // namespace mousse
