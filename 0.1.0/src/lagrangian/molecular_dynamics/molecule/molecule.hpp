#ifndef LAGRANGIAN_MOLECULAR_DYNAMICS_MOLECULE_MOLECULE_HPP_
#define LAGRANGIAN_MOLECULAR_DYNAMICS_MOLECULE_MOLECULE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::molecule
// Description
//   mousse::molecule
// SourceFiles
//   molecule.cpp
//   molecule_io.cpp
#include "particle.hpp"
#include "iostream.hpp"
#include "auto_ptr.hpp"
#include "diag_tensor.hpp"
namespace mousse
{
// Class forward declarations
class moleculeCloud;
class molecule
:
  public particle
{
  // Private data
    //- Size in bytes of the fields
    static const std::size_t sizeofFields_;
public:
  // Values of special that are less than zero are for built-in functionality.
  // Values greater than zero are user specifiable/expandable (i.e. test
  // special_ >= SPECIAL_USER)
  enum specialTypes
  {
    SPECIAL_TETHERED = -1,
    SPECIAL_FROZEN   = -2,
    NOT_SPECIAL      = 0,
    SPECIAL_USER     = 1
  };
  //- Class to hold molecule constant properties
  class constantProperties
  {
    // Private data
      Field<vector> siteReferencePositions_;
      List<scalar> siteMasses_;
      List<scalar> siteCharges_;
      List<label> siteIds_;
      List<bool> pairPotentialSites_;
      List<bool> electrostaticSites_;
      diagTensor momentOfInertia_;
      scalar mass_;
    // Private Member Functions
      void checkSiteListSizes() const;
      void setInteracionSiteBools
      (
        const List<word>& siteIds,
        const List<word>& pairPotSiteIds
      );
      bool linearMoleculeTest() const;
  public:
    inline constantProperties();
    //- Construct from dictionary
    inline constantProperties(const dictionary& dict);
    // Member functions
    inline const Field<vector>& siteReferencePositions() const;
    inline const List<scalar>& siteMasses() const;
    inline const List<scalar>& siteCharges() const;
    inline const List<label>& siteIds() const;
    inline List<label>& siteIds();
    inline const List<bool>& pairPotentialSites() const;
    inline bool pairPotentialSite(label sId) const;
    inline const List<bool>& electrostaticSites() const;
    inline bool electrostaticSite(label sId) const;
    inline const diagTensor& momentOfInertia() const;
    inline bool linearMolecule() const;
    inline bool pointMolecule() const;
    inline label degreesOfFreedom() const;
    inline scalar mass() const;
    inline label nSites() const;
  };
  //- Class used to pass tracking data to the trackToFace function
  class trackingData
  :
    public particle::TrackingData<moleculeCloud>
  {
    // label specifying which part of the integration algorithm is taking
    label part_;
  public:
    // Constructors
      trackingData(moleculeCloud& cloud, label part)
      :
        particle::TrackingData<moleculeCloud>(cloud),
        part_(part)
      {}
    // Member functions
      inline label part() const
      {
        return part_;
      }
  };
private:
  // Private data
    tensor Q_;
    vector v_;
    vector a_;
    vector pi_;
    vector tau_;
    vector specialPosition_;
    scalar potentialEnergy_;
    // - r_ij f_ij, stress dyad
    tensor rf_;
    label special_;
    label id_;
    List<vector> siteForces_;
    List<vector> sitePositions_;
  // Private Member Functions
    tensor rotationTensorX(scalar deltaT) const;
    tensor rotationTensorY(scalar deltaT) const;
    tensor rotationTensorZ(scalar deltaT) const;
public:
  friend class Cloud<molecule>;
  // Constructors
    //- Construct from components
    inline molecule
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
    );
    //- Construct from Istream
    molecule
    (
      const polyMesh& mesh,
      Istream& is,
      bool readFields = true
    );
    //- Construct and return a clone
    autoPtr<particle> clone() const
    {
      return autoPtr<particle>(new molecule(*this));
    }
    //- Factory class to read-construct particles used for
    //  parallel transfer
    class iNew
    {
      const polyMesh& mesh_;
    public:
      iNew(const polyMesh& mesh)
      :
        mesh_(mesh)
      {}
      autoPtr<molecule> operator()(Istream& is) const
      {
        return autoPtr<molecule>(new molecule(mesh_, is, true));
      }
    };
  // Member Functions
    // Tracking
      bool move(trackingData&, const scalar trackTime);
      virtual void transformProperties(const tensor& T);
      virtual void transformProperties(const vector& separation);
      void setSitePositions(const constantProperties& constProps);
      void setSiteSizes(label size);
    // Access
      inline const tensor& Q() const;
      inline tensor& Q();
      inline const vector& v() const;
      inline vector& v();
      inline const vector& a() const;
      inline vector& a();
      inline const vector& pi() const;
      inline vector& pi();
      inline const vector& tau() const;
      inline vector& tau();
      inline const List<vector>& siteForces() const;
      inline List<vector>& siteForces();
      inline const List<vector>& sitePositions() const;
      inline List<vector>& sitePositions();
      inline const vector& specialPosition() const;
      inline vector& specialPosition();
      inline scalar potentialEnergy() const;
      inline scalar& potentialEnergy();
      inline const tensor& rf() const;
      inline tensor& rf();
      inline label special() const;
      inline bool tethered() const;
      inline label id() const;
  // Member Operators
    //- Overridable function to handle the particle hitting a patch
    //  Executed before other patch-hitting functions
    bool hitPatch
    (
      const polyPatch&,
      trackingData& td,
      const label patchI,
      const scalar trackFraction,
      const tetIndices& tetIs
    );
    //- Overridable function to handle the particle hitting a processorPatch
    void hitProcessorPatch
    (
      const processorPolyPatch&,
      trackingData& td
    );
    //- Overridable function to handle the particle hitting a wallPatch
    void hitWallPatch
    (
      const wallPolyPatch&,
      trackingData& td,
      const tetIndices&
    );
    //- Overridable function to handle the particle hitting a polyPatch
    void hitPatch
    (
      const polyPatch&,
      trackingData& td
    );
  // I-O
    static void readFields(Cloud<molecule>& mC);
    static void writeFields(const Cloud<molecule>& mC);
  // IOstream Operators
    friend Ostream& operator<<(Ostream&, const molecule&);
};
}  // namespace mousse

// Constructors 
inline mousse::molecule::constantProperties::constantProperties()
:
  siteReferencePositions_{Field<vector>{0}},
  siteMasses_{List<scalar>{0}},
  siteCharges_{List<scalar>{0}},
  siteIds_{List<label>{0}},
  pairPotentialSites_{List<bool>{false}},
  electrostaticSites_{List<bool>{false}},
  momentOfInertia_{diagTensor{0, 0, 0}},
  mass_{0}
{}
inline mousse::molecule::constantProperties::constantProperties
(
  const dictionary& dict
)
:
  siteReferencePositions_{dict.lookup("siteReferencePositions")},
  siteMasses_{dict.lookup("siteMasses")},
  siteCharges_{dict.lookup("siteCharges")},
  siteIds_{List<word>(dict.lookup("siteIds")).size(), -1},
  pairPotentialSites_{},
  electrostaticSites_{},
  momentOfInertia_{},
  mass_{}
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
  FOR_ALL(siteReferencePositions_, i)
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
    FOR_ALL(siteReferencePositions_, i)
    {
      centreOfMass += siteReferencePositions_[i]*siteMasses_[i];
    }
    centreOfMass /= mass_;
    siteReferencePositions_ -= centreOfMass;
    diagTensor momOfInertia = diagTensor::zero;
    FOR_ALL(siteReferencePositions_, i)
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
    FOR_ALL(siteReferencePositions_, i)
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
      FATAL_ERROR_IN("molecule::constantProperties::constantProperties")
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
    FOR_ALL(siteReferencePositions_, i)
    {
      centreOfMass += siteReferencePositions_[i]*siteMasses_[i];
    }
    centreOfMass /= mass_;
    siteReferencePositions_ -= centreOfMass;
    // Calculate the moment of inertia in the principle component
    // reference frame
    momOfInertia = tensor::zero;
    FOR_ALL(siteReferencePositions_, i)
    {
      const vector& p(siteReferencePositions_[i]);
      momOfInertia += siteMasses_[i]*tensor
      {
        p.y()*p.y() + p.z()*p.z(), -p.x()*p.y(), -p.x()*p.z(),
        -p.y()*p.x(), p.x()*p.x() + p.z()*p.z(), -p.y()*p.z(),
        -p.z()*p.x(), -p.z()*p.y(), p.x()*p.x() + p.y()*p.y()
      };
    }
    momentOfInertia_ = diagTensor
    {
      momOfInertia.xx(),
      momOfInertia.yy(),
      momOfInertia.zz()
    };
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
  particle{mesh, position, cellI, tetFaceI, tetPtI},
  Q_{Q},
  v_{v},
  a_{a},
  pi_{pi},
  tau_{tau},
  specialPosition_{specialPosition},
  potentialEnergy_{0.0},
  rf_{tensor::zero},
  special_{special},
  id_{id},
  siteForces_{constProps.nSites(), vector::zero},
  sitePositions_{constProps.nSites()}
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
    FATAL_ERROR_IN("molecule::constantProperties::checkSiteListSizes")
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
  FOR_ALL(siteIds_, i)
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
    FATAL_ERROR_IN("molecule.hpp") << nl
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
    FATAL_ERROR_IN
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
#endif
