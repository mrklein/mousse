// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::moleculeCloud
// Description
// SourceFiles
//   molecule_cloud.cpp
#ifndef molecule_cloud_hpp_
#define molecule_cloud_hpp_
#include "_cloud.hpp"
#include "molecule.hpp"
#include "iodictionary.hpp"
#include "potential.hpp"
#include "_interaction_lists.hpp"
#include "label_vector.hpp"
#include "random.hpp"
#include "file_name.hpp"
#include "constants.hpp"
using namespace mousse::constant;
namespace mousse
{
class moleculeCloud
:
  public Cloud<molecule>
{
private:
  // Private data
    const polyMesh& mesh_;
    const potential& pot_;
    List<DynamicList<molecule*> > cellOccupancy_;
    InteractionLists<molecule> il_;
    List<molecule::constantProperties> constPropList_;
    Random rndGen_;
  // Private Member Functions
    void buildConstProps();
    void setSiteSizesAndPositions();
    //- Determine which molecules are in which cells
    void buildCellOccupancy();
    void calculatePairForce();
    inline void evaluatePair
    (
      molecule& molI,
      molecule& molJ
    );
    inline bool evaluatePotentialLimit
    (
      molecule& molI,
      molecule& molJ
    ) const;
    void calculateTetherForce();
    void calculateExternalForce();
    void removeHighEnergyOverlaps();
    void initialiseMolecules
    (
      const IOdictionary& mdInitialiseDict
    );
    void createMolecule
    (
      const point& position,
      label cell,
      label tetFace,
      label tetPt,
      label id,
      bool tethered,
      scalar temperature,
      const vector& bulkVelocity
    );
    label nSites() const;
    inline vector equipartitionLinearVelocity
    (
      scalar temperature,
      scalar mass
    );
    inline vector equipartitionAngularMomentum
    (
      scalar temperature,
      const molecule::constantProperties& cP
    );
public:
  // Constructors
    //- Construct given mesh and potential references
    moleculeCloud
    (
      const polyMesh& mesh,
      const potential& pot,
      bool readFields = true
    );
    //- Construct given mesh, potential and mdInitialiseDict
    moleculeCloud
    (
      const polyMesh& mesh,
      const potential& pot,
      const IOdictionary& mdInitialiseDict,
      bool readFields = true
    );
    //- Disallow default bitwise copy construct
    moleculeCloud(const moleculeCloud&) = delete;
    //- Disallow default bitwise assignment
    moleculeCloud& operator=(const moleculeCloud&) = delete;
  // Member Functions
    //- Evolve the molecules (move, calculate forces, control state etc)
    void evolve();
    void calculateForce();
    void applyConstraintsAndThermostats
    (
      const scalar targetTemperature,
      const scalar measuredTemperature
    );
    // Access
      inline const polyMesh& mesh() const;
      inline const potential& pot() const;
      inline const List<DynamicList<molecule*> >& cellOccupancy() const;
      inline const InteractionLists<molecule>& il() const;
      inline const List<molecule::constantProperties> constProps() const;
      inline const molecule::constantProperties&
        constProps(label id) const;
      inline Random& rndGen();
  // Member Operators
    //- Write molecule sites in XYZ format
    void writeXYZ(const fileName& fName) const;
};
}  // namespace mousse

// Private Member Functions 
inline void mousse::moleculeCloud::evaluatePair
(
  molecule& molI,
  molecule& molJ
)
{
  const pairPotentialList& pairPot = pot_.pairPotentials();
  const pairPotential& electrostatic = pairPot.electrostatic();
  label idI = molI.id();
  label idJ = molJ.id();
  const molecule::constantProperties& constPropI = constProps(idI);
  const molecule::constantProperties& constPropJ = constProps(idJ);
  List<label> siteIdsI = constPropI.siteIds();
  List<label> siteIdsJ = constPropJ.siteIds();
  List<bool> pairPotentialSitesI = constPropI.pairPotentialSites();
  List<bool> electrostaticSitesI = constPropI.electrostaticSites();
  List<bool> pairPotentialSitesJ = constPropJ.pairPotentialSites();
  List<bool> electrostaticSitesJ = constPropJ.electrostaticSites();
  FOR_ALL(siteIdsI, sI)
  {
    label idsI(siteIdsI[sI]);
    FOR_ALL(siteIdsJ, sJ)
    {
      label idsJ(siteIdsJ[sJ]);
      if (pairPotentialSitesI[sI] && pairPotentialSitesJ[sJ])
      {
        vector rsIsJ =
          molI.sitePositions()[sI] - molJ.sitePositions()[sJ];
        scalar rsIsJMagSq = magSqr(rsIsJ);
        if (pairPot.rCutSqr(idsI, idsJ, rsIsJMagSq))
        {
          scalar rsIsJMag = mag(rsIsJ);
          vector fsIsJ = (rsIsJ/rsIsJMag)*pairPot.force(idsI, idsJ, rsIsJMag);
          molI.siteForces()[sI] += fsIsJ;
          molJ.siteForces()[sJ] += -fsIsJ;
          scalar potentialEnergy
          {
            pairPot.energy(idsI, idsJ, rsIsJMag)
          };
          molI.potentialEnergy() += 0.5*potentialEnergy;
          molJ.potentialEnergy() += 0.5*potentialEnergy;
          vector rIJ = molI.position() - molJ.position();
          tensor virialContribution((rsIsJ*fsIsJ)*(rsIsJ & rIJ)/rsIsJMagSq);
          molI.rf() += virialContribution;
          molJ.rf() += virialContribution;
        }
      }
      if (electrostaticSitesI[sI] && electrostaticSitesJ[sJ])
      {
        vector rsIsJ =
        molI.sitePositions()[sI] - molJ.sitePositions()[sJ];
        scalar rsIsJMagSq = magSqr(rsIsJ);
        if (rsIsJMagSq <= electrostatic.rCutSqr())
        {
          scalar rsIsJMag = mag(rsIsJ);
          scalar chargeI = constPropI.siteCharges()[sI];
          scalar chargeJ = constPropJ.siteCharges()[sJ];
          vector fsIsJ
          {
            (rsIsJ/rsIsJMag)*chargeI*chargeJ*electrostatic.force(rsIsJMag)
          };
          molI.siteForces()[sI] += fsIsJ;
          molJ.siteForces()[sJ] += -fsIsJ;
          scalar potentialEnergy
          {
            chargeI*chargeJ*electrostatic.energy(rsIsJMag)
          };
          molI.potentialEnergy() += 0.5*potentialEnergy;
          molJ.potentialEnergy() += 0.5*potentialEnergy;
          vector rIJ = molI.position() - molJ.position();
          tensor virialContribution{(rsIsJ*fsIsJ)*(rsIsJ & rIJ)/rsIsJMagSq};
          molI.rf() += virialContribution;
          molJ.rf() += virialContribution;
        }
      }
    }
  }
}
inline bool mousse::moleculeCloud::evaluatePotentialLimit
(
  molecule& molI,
  molecule& molJ
) const
{
  const pairPotentialList& pairPot = pot_.pairPotentials();
  const pairPotential& electrostatic = pairPot.electrostatic();
  label idI = molI.id();
  label idJ = molJ.id();
  const molecule::constantProperties& constPropI = constProps(idI);
  const molecule::constantProperties& constPropJ = constProps(idJ);
  List<label> siteIdsI = constPropI.siteIds();
  List<label> siteIdsJ = constPropJ.siteIds();
  List<bool> pairPotentialSitesI = constPropI.pairPotentialSites();
  List<bool> electrostaticSitesI = constPropI.electrostaticSites();
  List<bool> pairPotentialSitesJ = constPropJ.pairPotentialSites();
  List<bool> electrostaticSitesJ = constPropJ.electrostaticSites();
  FOR_ALL(siteIdsI, sI)
  {
    label idsI(siteIdsI[sI]);
    FOR_ALL(siteIdsJ, sJ)
    {
      label idsJ(siteIdsJ[sJ]);
      if (pairPotentialSitesI[sI] && pairPotentialSitesJ[sJ])
      {
        vector rsIsJ =
          molI.sitePositions()[sI] - molJ.sitePositions()[sJ];
        scalar rsIsJMagSq = magSqr(rsIsJ);
        if (pairPot.rCutSqr(idsI, idsJ, rsIsJMagSq))
        {
          scalar rsIsJMag = mag(rsIsJ);
          // Guard against pairPot.energy being evaluated
          // if rIJMag < SMALL. A floating point exception will
          // happen otherwise.
          if (rsIsJMag < SMALL)
          {
            WARNING_IN("moleculeCloud::removeHighEnergyOverlaps()")
              << "Molecule site pair closer than "
              << SMALL
              << ": mag separation = " << rsIsJMag
              << ". These may have been placed on top of each"
              << " other by a rounding error in mdInitialise in"
              << " parallel or a block filled with molecules"
              << " twice. Removing one of the molecules."
              << endl;
            return true;
          }
          // Guard against pairPot.energy being evaluated if rIJMag <
          // rMin.  A tabulation lookup error will occur otherwise.
          if (rsIsJMag < pairPot.rMin(idsI, idsJ))
          {
            return true;
          }
          if
          (
            mag(pairPot.energy(idsI, idsJ, rsIsJMag))
           > pot_.potentialEnergyLimit()
          )
          {
            return true;
          };
        }
      }
      if (electrostaticSitesI[sI] && electrostaticSitesJ[sJ])
      {
        vector rsIsJ =
          molI.sitePositions()[sI] - molJ.sitePositions()[sJ];
        scalar rsIsJMagSq = magSqr(rsIsJ);
        if (pairPot.rCutMaxSqr(rsIsJMagSq))
        {
          scalar rsIsJMag = mag(rsIsJ);
          // Guard against pairPot.energy being evaluated
          // if rIJMag < SMALL. A floating point exception will
          // happen otherwise.
          if (rsIsJMag < SMALL)
          {
            WARNING_IN("moleculeCloud::removeHighEnergyOverlaps()")
              << "Molecule site pair closer than "
              << SMALL
              << ": mag separation = " << rsIsJMag
              << ". These may have been placed on top of each"
              << " other by a rounding error in mdInitialise in"
              << " parallel or a block filled with molecules"
              << " twice. Removing one of the molecules."
              << endl;
            return true;
          }
          if (rsIsJMag < electrostatic.rMin())
          {
            return true;
          }
          scalar chargeI = constPropI.siteCharges()[sI];
          scalar chargeJ = constPropJ.siteCharges()[sJ];
          if
          (
            mag(chargeI*chargeJ*electrostatic.energy(rsIsJMag))
           > pot_.potentialEnergyLimit()
          )
          {
            return true;
          };
        }
      }
    }
  }
  return false;
}
inline mousse::vector mousse::moleculeCloud::equipartitionLinearVelocity
(
  scalar temperature,
  scalar mass
)
{
  return sqrt(physicoChemical::k.value()*temperature/mass)*vector
  (
    rndGen_.GaussNormal(),
    rndGen_.GaussNormal(),
    rndGen_.GaussNormal()
  );
}
inline mousse::vector mousse::moleculeCloud::equipartitionAngularMomentum
(
  scalar temperature,
  const molecule::constantProperties& cP
)
{
  scalar sqrtKbT = sqrt(physicoChemical::k.value()*temperature);
  if (cP.linearMolecule())
  {
    return sqrtKbT*vector
    {
      0.0,
      sqrt(cP.momentOfInertia().yy())*rndGen_.GaussNormal(),
      sqrt(cP.momentOfInertia().zz())*rndGen_.GaussNormal()
    };
  }
  else
  {
    return sqrtKbT*vector
    {
      sqrt(cP.momentOfInertia().xx())*rndGen_.GaussNormal(),
      sqrt(cP.momentOfInertia().yy())*rndGen_.GaussNormal(),
      sqrt(cP.momentOfInertia().zz())*rndGen_.GaussNormal()
    };
  }
}
// Member Functions 
inline const mousse::polyMesh& mousse::moleculeCloud::mesh() const
{
  return mesh_;
}
inline const mousse::potential& mousse::moleculeCloud::pot() const
{
  return pot_;
}
inline const mousse::List<mousse::DynamicList<mousse::molecule*> >&
  mousse::moleculeCloud::cellOccupancy() const
{
  return cellOccupancy_;
}
inline const mousse::InteractionLists<mousse::molecule>&
  mousse::moleculeCloud::il() const
{
  return il_;
}
inline const mousse::List<mousse::molecule::constantProperties>
  mousse::moleculeCloud::constProps() const
{
  return constPropList_;
}
inline const mousse::molecule::constantProperties&
  mousse::moleculeCloud::constProps(label id) const
{
  return constPropList_[id];
}
inline mousse::Random& mousse::moleculeCloud::rndGen()
{
  return rndGen_;
}
#endif
