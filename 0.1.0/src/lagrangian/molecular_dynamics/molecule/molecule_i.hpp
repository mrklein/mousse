// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
inline mousse::molecule::constantProperties::constantProperties()
:
  siteReferencePositions_(Field<vector>(0)),
  siteMasses_(List<scalar>(0)),
  siteCharges_(List<scalar>(0)),
  siteIds_(List<label>(0)),
  pairPotentialSites_(List<bool>(false)),
  electrostaticSites_(List<bool>(false)),
  momentOfInertia_(diagTensor(0, 0, 0)),
  mass_(0)
{}
inline mousse::molecule::constantProperties::constantProperties
(
  const dictionary& dict
)
:
  siteReferencePositions_(dict.lookup("siteReferencePositions")),
  siteMasses_(dict.lookup("siteMasses")),
  siteCharges_(dict.lookup("siteCharges")),
  siteIds_(List<word>(dict.lookup("siteIds")).size(), -1),
  pairPotentialSites_(),
  electrostaticSites_(),
  momentOfInertia_(),
  mass_()
{
  checkSiteListSizes();
  setInteracionSiteBools
  (
    List<word>(dict.lookup("siteIds")),
    List<word>(dict.lookup("pairPotentialSiteIds"))
  );
  mass_ = sum(siteMasses_);
  vector centreOfMass(vector::zero);
  // Calculate the centre of mass of the body and subtract it from each
  // position
  forAll(siteReferencePositions_, i)
  {
    centreOfMass += siteReferencePositions_[i]*siteMasses_[i];
  }
  centreOfMass /= mass_;
  siteReferencePositions_ -= centreOfMass;
  if (siteIds_.size() == 1)
  {
    // Single site molecule - no rotational motion.
    siteReferencePositions_[0] = vector::zero;
    momentOfInertia_ = diagTensor(-1, -1, -1);
  }
  else if (linearMoleculeTest())
  {
    // Linear molecule.
    Info<< nl << "Linear molecule." << endl;
    vector dir = siteReferencePositions_[1] - siteReferencePositions_[0];
    dir /= mag(dir);
    tensor Q = rotationTensor(dir, vector(1,0,0));
    siteReferencePositions_ = (Q & siteReferencePositions_);
    // The rotation was around the centre of mass but remove any
    // components that have crept in due to floating point errors
    centreOfMass = vector::zero;
    forAll(siteReferencePositions_, i)
    {
      centreOfMass += siteReferencePositions_[i]*siteMasses_[i];
    }
    centreOfMass /= mass_;
    siteReferencePositions_ -= centreOfMass;
    diagTensor momOfInertia = diagTensor::zero;
    forAll(siteReferencePositions_, i)
    {
      const vector& p(siteReferencePositions_[i]);
      momOfInertia +=
        siteMasses_[i]*diagTensor(0, p.x()*p.x(), p.x()*p.x());
    }
    momentOfInertia_ = diagTensor
    (
      -1,
      momOfInertia.yy(),
      momOfInertia.zz()
    );
  }
  else
  {
    // Fully 6DOF molecule
    // Calculate the inertia tensor in the current orientation
    tensor momOfInertia(tensor::zero);
    forAll(siteReferencePositions_, i)
    {
      const vector& p(siteReferencePositions_[i]);
      momOfInertia += siteMasses_[i]*tensor
      (
        p.y()*p.y() + p.z()*p.z(), -p.x()*p.y(), -p.x()*p.z(),
        -p.y()*p.x(), p.x()*p.x() + p.z()*p.z(), -p.y()*p.z(),
        -p.z()*p.x(), -p.z()*p.y(), p.x()*p.x() + p.y()*p.y()
      );
    }
    if (eigenValues(momOfInertia).x() < VSMALL)
    {
      FatalErrorIn("molecule::constantProperties::constantProperties")
        << "An eigenvalue of the inertia tensor is zero, the molecule "
        << dict.name() << " is not a valid 6DOF shape."
        << nl << abort(FatalError);
    }
    // Normalise the inertia tensor magnitude to avoid SMALL numbers in the
    // components causing problems
    momOfInertia /= eigenValues(momOfInertia).x();
    tensor e = eigenVectors(momOfInertia);
    // Calculate the transformation between the principle axes to the
    // global axes
    tensor Q =
      vector(1,0,0)*e.x() + vector(0,1,0)*e.y() + vector(0,0,1)*e.z();
    // Transform the site positions
    siteReferencePositions_ = (Q & siteReferencePositions_);
    // Recalculating the moment of inertia with the new site positions
    // The rotation was around the centre of mass but remove any
    // components that have crept in due to floating point errors
    centreOfMass = vector::zero;
    forAll(siteReferencePositions_, i)
    {
      centreOfMass += siteReferencePositions_[i]*siteMasses_[i];
    }
    centreOfMass /= mass_;
    siteReferencePositions_ -= centreOfMass;
    // Calculate the moment of inertia in the principle component
    // reference frame
    momOfInertia = tensor::zero;
    forAll(siteReferencePositions_, i)
    {
      const vector& p(siteReferencePositions_[i]);
      momOfInertia += siteMasses_[i]*tensor
      (
        p.y()*p.y() + p.z()*p.z(), -p.x()*p.y(), -p.x()*p.z(),
        -p.y()*p.x(), p.x()*p.x() + p.z()*p.z(), -p.y()*p.z(),
        -p.z()*p.x(), -p.z()*p.y(), p.x()*p.x() + p.y()*p.y()
      );
    }
    momentOfInertia_ = diagTensor
    (
      momOfInertia.xx(),
      momOfInertia.yy(),
      momOfInertia.zz()
    );
  }
}
inline mousse::molecule::molecule
(
  const polyMesh& mesh,
  const vector& position,
  const label cellI,
  const label tetFaceI,
  const label tetPtI,
  const tensor& Q,
  const vector& v,
  const vector& a,
  const vector& pi,
  const vector& tau,
  const vector& specialPosition,
  const constantProperties& constProps,
  const label special,
  const label id
)
:
  particle(mesh, position, cellI, tetFaceI, tetPtI),
  Q_(Q),
  v_(v),
  a_(a),
  pi_(pi),
  tau_(tau),
  specialPosition_(specialPosition),
  potentialEnergy_(0.0),
  rf_(tensor::zero),
  special_(special),
  id_(id),
  siteForces_(constProps.nSites(), vector::zero),
  sitePositions_(constProps.nSites())
{
  setSitePositions(constProps);
}
// constantProperties Private Member Functions
inline void mousse::molecule::constantProperties::checkSiteListSizes() const
{
  if
  (
    siteIds_.size() != siteReferencePositions_.size()
  || siteIds_.size() != siteCharges_.size()
  )
  {
    FatalErrorIn("molecule::constantProperties::checkSiteListSizes")
      << "Sizes of site id, charge and "
      << "referencePositions are not the same. "
      << nl << abort(FatalError);
  }
}
inline void mousse::molecule::constantProperties::setInteracionSiteBools
(
  const List<word>& siteIds,
  const List<word>& pairPotSiteIds
)
{
  pairPotentialSites_.setSize(siteIds_.size());
  electrostaticSites_.setSize(siteIds_.size());
  forAll(siteIds_, i)
  {
    const word& id(siteIds[i]);
    pairPotentialSites_[i] = (findIndex(pairPotSiteIds, id) > -1);
    electrostaticSites_[i] = (mag(siteCharges_[i]) > VSMALL);
  }
}
inline bool mousse::molecule::constantProperties::linearMoleculeTest() const
{
  if (siteIds_.size() == 2)
  {
    return true;
  }
  vector refDir = siteReferencePositions_[1] - siteReferencePositions_[0];
  refDir /= mag(refDir);
  for
  (
    label i = 2;
    i < siteReferencePositions_.size();
    i++
  )
  {
    vector dir = siteReferencePositions_[i] - siteReferencePositions_[i-1];
    dir /= mag(dir);
    if (mag(refDir & dir) < 1 - SMALL)
    {
      return false;
    }
  }
  return true;
}
// constantProperties Member Functions
inline const mousse::Field<mousse::vector>&
mousse::molecule::constantProperties::siteReferencePositions() const
{
  return siteReferencePositions_;
}
inline const mousse::List<mousse::scalar>&
mousse::molecule::constantProperties::siteMasses() const
{
  return siteMasses_;
}
inline const mousse::List<mousse::scalar>&
mousse::molecule::constantProperties::siteCharges() const
{
  return siteCharges_;
}
inline const mousse::List<mousse::label>&
mousse::molecule::constantProperties::siteIds() const
{
  return siteIds_;
}
inline mousse::List<mousse::label>&
mousse::molecule::constantProperties::siteIds()
{
  return siteIds_;
}
inline const mousse::List<bool>&
mousse::molecule::constantProperties::pairPotentialSites() const
{
  return pairPotentialSites_;
}
inline bool mousse::molecule::constantProperties::pairPotentialSite
(
  label sId
) const
{
  label s = findIndex(siteIds_, sId);
  if (s == -1)
  {
    FatalErrorIn("moleculeI.H") << nl
      << sId << " site not found."
      << nl << abort(FatalError);
  }
  return pairPotentialSites_[s];
}
inline const mousse::List<bool>&
mousse::molecule::constantProperties::electrostaticSites() const
{
  return electrostaticSites_;
}
inline bool mousse::molecule::constantProperties::electrostaticSite
(
  label sId
) const
{
  label s = findIndex(siteIds_, sId);
  if (s == -1)
  {
    FatalErrorIn
    (
      "molecule::constantProperties::electrostaticSite(label)"
    )   << sId << " site not found."
      << nl << abort(FatalError);
  }
  return electrostaticSites_[s];
}
inline const mousse::diagTensor&
mousse::molecule::constantProperties::momentOfInertia() const
{
  return momentOfInertia_;
}
inline bool mousse::molecule::constantProperties::linearMolecule() const
{
  return ((momentOfInertia_.xx() < 0) && (momentOfInertia_.yy() > 0));
}
inline bool mousse::molecule::constantProperties::pointMolecule() const
{
  return (momentOfInertia_.zz() < 0);
}
inline mousse::label mousse::molecule::constantProperties::degreesOfFreedom() const
{
  if (linearMolecule())
  {
    return 5;
  }
  else if (pointMolecule())
  {
    return 3;
  }
  else
  {
    return 6;
  }
}
inline mousse::scalar mousse::molecule::constantProperties::mass() const
{
  return mass_;
}
inline mousse::label mousse::molecule::constantProperties::nSites() const
{
  return siteIds_.size();
}
// molecule Member Functions
inline const mousse::tensor& mousse::molecule::Q() const
{
  return Q_;
}
inline mousse::tensor& mousse::molecule::Q()
{
  return Q_;
}
inline const mousse::vector& mousse::molecule::v() const
{
  return v_;
}
inline mousse::vector& mousse::molecule::v()
{
  return v_;
}
inline const mousse::vector& mousse::molecule::a() const
{
  return a_;
}
inline mousse::vector& mousse::molecule::a()
{
  return a_;
}
inline const mousse::vector& mousse::molecule::pi() const
{
  return pi_;
}
inline mousse::vector& mousse::molecule::pi()
{
  return pi_;
}
inline const mousse::vector& mousse::molecule::tau() const
{
  return tau_;
}
inline mousse::vector& mousse::molecule::tau()
{
  return tau_;
}
inline const mousse::List<mousse::vector>& mousse::molecule::siteForces() const
{
  return siteForces_;
}
inline mousse::List<mousse::vector>& mousse::molecule::siteForces()
{
  return siteForces_;
}
inline const mousse::List<mousse::vector>& mousse::molecule::sitePositions() const
{
  return sitePositions_;
}
inline mousse::List<mousse::vector>& mousse::molecule::sitePositions()
{
  return sitePositions_;
}
inline const mousse::vector& mousse::molecule::specialPosition() const
{
  return specialPosition_;
}
inline mousse::vector& mousse::molecule::specialPosition()
{
  return specialPosition_;
}
inline mousse::scalar mousse::molecule::potentialEnergy() const
{
  return potentialEnergy_;
}
inline mousse::scalar& mousse::molecule::potentialEnergy()
{
  return potentialEnergy_;
}
inline const mousse::tensor& mousse::molecule::rf() const
{
  return rf_;
}
inline mousse::tensor& mousse::molecule::rf()
{
  return rf_;
}
inline mousse::label mousse::molecule::special() const
{
  return special_;
}
inline bool mousse::molecule::tethered() const
{
  return special_ == SPECIAL_TETHERED;
}
inline mousse::label mousse::molecule::id() const
{
  return id_;
}
