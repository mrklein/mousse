// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::molecule
// Description
//   mousse::molecule
// SourceFiles
//   molecule_i.hpp
//   molecule.cpp
//   molecule_io.cpp
#ifndef molecule_hpp_
#define molecule_hpp_
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
#include "molecule_i.hpp"
#endif
