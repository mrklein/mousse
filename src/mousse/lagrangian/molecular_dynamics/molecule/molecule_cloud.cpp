// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "molecule_cloud.hpp"
#include "fv_mesh.hpp"
#include "mathematical_constants.hpp"


using namespace mousse::constant::mathematical;


// Static Data Members
namespace mousse {

DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(Cloud<molecule>, 0);

}


// Private Member Functions 
void mousse::moleculeCloud::buildConstProps()
{
  Info << nl << "Reading moleculeProperties dictionary." << endl;
  const List<word>& idList = pot_.idList();
  constPropList_.setSize(idList.size());
  const List<word>& siteIdList = pot_.siteIdList();
  IOdictionary moleculePropertiesDict
  {
    IOobject
    {
      "moleculeProperties",
      mesh_.time().constant(),
      mesh_,
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::NO_WRITE,
      false
    }
  };
  FOR_ALL(idList, i) {
    const word& id = idList[i];
    const dictionary& molDict = moleculePropertiesDict.subDict(id);
    List<word> siteIdNames = molDict.lookup("siteIds");
    List<label> siteIds{siteIdNames.size()};
    FOR_ALL(siteIdNames, sI) {
      const word& siteId = siteIdNames[sI];
      siteIds[sI] = findIndex(siteIdList, siteId);
      if (siteIds[sI] == -1) {
        FATAL_ERROR_IN("moleculeCloud::buildConstProps()")
          << siteId << " site not found."
          << nl << abort(FatalError);
      }
    }
    molecule::constantProperties& constProp = constPropList_[i];
    constProp = molecule::constantProperties(molDict);
    constProp.siteIds() = siteIds;
  }
}


void mousse::moleculeCloud::setSiteSizesAndPositions()
{
  FOR_ALL_ITER(moleculeCloud, *this, mol) {
    const molecule::constantProperties& cP = constProps(mol().id());
    mol().setSiteSizes(cP.nSites());
    mol().setSitePositions(cP);
  }
}


void mousse::moleculeCloud::buildCellOccupancy()
{
  FOR_ALL(cellOccupancy_, cO) {
    cellOccupancy_[cO].clear();
  }
  FOR_ALL_ITER(moleculeCloud, *this, mol) {
    cellOccupancy_[mol().cell()].append(&mol());
  }
  FOR_ALL(cellOccupancy_, cO) {
    cellOccupancy_[cO].shrink();
  }
}


void mousse::moleculeCloud::calculatePairForce()
{
  PstreamBuffers pBufs{Pstream::nonBlocking};
  // Start sending referred data
  label startOfRequests = Pstream::nRequests();
  il_.sendReferredData(cellOccupancy(), pBufs);
  molecule* molI = nullptr;
  molecule* molJ = nullptr;

  {
    // Real-Real interactions
    const labelListList& dil = il_.dil();
    FOR_ALL(dil, d) {
      FOR_ALL(cellOccupancy_[d],cellIMols) {
        molI = cellOccupancy_[d][cellIMols];
        FOR_ALL(dil[d], interactingCells) {
          List<molecule*> cellJ = cellOccupancy_[dil[d][interactingCells]];
          FOR_ALL(cellJ, cellJMols) {
            molJ = cellJ[cellJMols];
            evaluatePair(*molI, *molJ);
          }
        }
        FOR_ALL(cellOccupancy_[d], cellIOtherMols) {
          molJ = cellOccupancy_[d][cellIOtherMols];
          if (molJ > molI) {
            evaluatePair(*molI, *molJ);
          }
        }
      }
    }
  }

  // Receive referred data
  il_.receiveReferredData(pBufs, startOfRequests);

  {
    // Real-Referred interactions
    const labelListList& ril = il_.ril();
    List<IDLList<molecule> >& referredMols = il_.referredParticles();
    FOR_ALL(ril, r) {
      const List<label>& realCells = ril[r];
      IDLList<molecule>& refMols = referredMols[r];
      FOR_ALL_ITER(IDLList<molecule>, refMols, refMol) {
        FOR_ALL(realCells, rC) {
          List<molecule*> cellI = cellOccupancy_[realCells[rC]];
          FOR_ALL(cellI, cellIMols) {
            molI = cellI[cellIMols];
            evaluatePair(*molI, refMol());
          }
        }
      }
    }
  }
}


void mousse::moleculeCloud::calculateTetherForce()
{
  const tetherPotentialList& tetherPot = pot_.tetherPotentials();
  FOR_ALL_ITER(moleculeCloud, *this, mol) {
    if (mol().tethered()) {
      vector rIT = mol().position() - mol().specialPosition();
      label idI = mol().id();
      scalar massI = constProps(idI).mass();
      vector fIT = tetherPot.force(idI, rIT);
      mol().a() += fIT/massI;
      mol().potentialEnergy() += tetherPot.energy(idI, rIT);
      // What to do here?
      // mol().rf() += rIT*fIT;
    }
  }
}


void mousse::moleculeCloud::calculateExternalForce()
{
  FOR_ALL_ITER(moleculeCloud, *this, mol) {
    mol().a() += pot_.gravity();
  }
}


void mousse::moleculeCloud::removeHighEnergyOverlaps()
{
  Info << nl << "Removing high energy overlaps, limit = "
    << pot_.potentialEnergyLimit()
    << nl << "Removal order:";
  FOR_ALL(pot_.removalOrder(), rO) {
    Info << ' ' << pot_.idList()[pot_.removalOrder()[rO]];
  }
  Info << nl ;
  label initialSize = this->size();
  buildCellOccupancy();
  // Real-Real interaction
  molecule* molI = nullptr;
  molecule* molJ = nullptr;

  {
    DynamicList<molecule*> molsToDelete;
    const labelListList& dil = il_.dil();
    FOR_ALL(dil, d) {
      FOR_ALL(cellOccupancy_[d],cellIMols) {
        molI = cellOccupancy_[d][cellIMols];
        FOR_ALL(dil[d], interactingCells) {
          List<molecule*> cellJ = cellOccupancy_[dil[d][interactingCells]];
          FOR_ALL(cellJ, cellJMols) {
            molJ = cellJ[cellJMols];
            if (evaluatePotentialLimit(*molI, *molJ)) {
              label idI = molI->id();
              label idJ = molJ->id();
              if (idI == idJ
                  || findIndex(pot_.removalOrder(), idJ)
                     < findIndex(pot_.removalOrder(), idI)) {
                if (findIndex(molsToDelete, molJ) == -1) {
                  molsToDelete.append(molJ);
                }
              } else if (findIndex(molsToDelete, molI) == -1) {
                molsToDelete.append(molI);
              }
            }
          }
        }
      }
      FOR_ALL(cellOccupancy_[d], cellIOtherMols) {
        molJ = cellOccupancy_[d][cellIOtherMols];
        if (molJ > molI) {
          if (evaluatePotentialLimit(*molI, *molJ)) {
            label idI = molI->id();
            label idJ = molJ->id();
            if (idI == idJ
                || findIndex(pot_.removalOrder(), idJ)
                   < findIndex(pot_.removalOrder(), idI)) {
              if (findIndex(molsToDelete, molJ) == -1) {
                molsToDelete.append(molJ);
              }
            } else if (findIndex(molsToDelete, molI) == -1) {
              molsToDelete.append(molI);
            }
          }
        }
      }
    }
    FOR_ALL(molsToDelete, mTD) {
      deleteParticle(*(molsToDelete[mTD]));
    }
  }
  buildCellOccupancy();
  PstreamBuffers pBufs{Pstream::nonBlocking};
  // Start sending referred data
  label startOfRequests = Pstream::nRequests();
  il_.sendReferredData(cellOccupancy(), pBufs);
    // Receive referred data
  il_.receiveReferredData(pBufs, startOfRequests);

  // Real-Referred interaction
  {
    DynamicList<molecule*> molsToDelete;
    const labelListList& ril = il_.ril();
    List<IDLList<molecule>>& referredMols = il_.referredParticles();
    FOR_ALL(ril, r) {
      IDLList<molecule>& refMols = referredMols[r];
      FOR_ALL_ITER(IDLList<molecule>, refMols, refMol) {
        molJ = &refMol();
        const List<label>& realCells = ril[r];
        FOR_ALL(realCells, rC) {
          label cellI = realCells[rC];
          List<molecule*> cellIMols = cellOccupancy_[cellI];
          FOR_ALL(cellIMols, cIM) {
            molI = cellIMols[cIM];
            if (evaluatePotentialLimit(*molI, *molJ)) {
              label idI = molI->id();
              label idJ = molJ->id();
              if (findIndex(pot_.removalOrder(), idI)
                  < findIndex(pot_.removalOrder(), idJ)) {
                if (findIndex(molsToDelete, molI) == -1) {
                  molsToDelete.append(molI);
                }
              } else if (findIndex(pot_.removalOrder(), idI)
                         == findIndex(pot_.removalOrder(), idJ)) {
                // Remove one of the molecules
                // arbitrarily, assuring that a
                // consistent decision is made for
                // both real-referred pairs.
                if (molI->origId() > molJ->origId()) {
                  if (findIndex(molsToDelete, molI) == -1) {
                    molsToDelete.append(molI);
                  }
                }
              }
            }
          }
        }
      }
    }
    FOR_ALL(molsToDelete, mTD) {
      deleteParticle(*(molsToDelete[mTD]));
    }
  }
  buildCellOccupancy();
  // Start sending referred data
  startOfRequests = Pstream::nRequests();
  il_.sendReferredData(cellOccupancy(), pBufs);
  // Receive referred data
  il_.receiveReferredData(pBufs, startOfRequests);
  label molsRemoved = initialSize - this->size();
  if (Pstream::parRun()) {
    reduce(molsRemoved, sumOp<label>());
  }
  Info << tab << molsRemoved << " molecules removed" << endl;
}


void mousse::moleculeCloud::initialiseMolecules
(
  const IOdictionary& mdInitialiseDict
)
{
  Info << nl
    << "Initialising molecules in each zone specified in mdInitialiseDict."
    << endl;
  const cellZoneMesh& cellZones = mesh_.cellZones();
  if (!cellZones.size()) {
    FATAL_ERROR_IN("void mousse::moleculeCloud::initialiseMolecules")
      << "No cellZones found in the mesh."
      << abort(FatalError);
  }
  FOR_ALL(cellZones, z) {
    const cellZone& zone(cellZones[z]);
    if (zone.size()) {
      if (!mdInitialiseDict.found(zone.name())) {
        Info << "No specification subDictionary for zone "
          << zone.name() << " found, skipping." << endl;
      } else {
        const dictionary& zoneDict = mdInitialiseDict.subDict(zone.name());
        const scalar temperature =
          readScalar(zoneDict.lookup("temperature"));
        const vector bulkVelocity{zoneDict.lookup("bulkVelocity")};
        List<word> latticeIds
        {
          zoneDict.lookup("latticeIds")
        };
        List<vector> latticePositions
        {
          zoneDict.lookup("latticePositions")
        };
        if (latticeIds.size() != latticePositions.size()) {
          FATAL_ERROR_IN("mousse::moleculeCloud::initialiseMolecules")
            << "latticeIds and latticePositions must be the same "
            << " size." << nl
            << abort(FatalError);
        }
        diagTensor latticeCellShape
        {
          zoneDict.lookup("latticeCellShape")
        };
        scalar latticeCellScale = 0.0;
        if (zoneDict.found("numberDensity")) {
          scalar numberDensity =
            readScalar(zoneDict.lookup("numberDensity"));
          if (numberDensity < VSMALL) {
            WARNING_IN("moleculeCloud::initialiseMolecules")
              << "numberDensity too small, not filling zone "
              << zone.name() << endl;
            continue;
          }
          latticeCellScale =
            pow(latticeIds.size()/(det(latticeCellShape)*numberDensity),
                (1.0/3.0));
        } else if (zoneDict.found("massDensity")) {
          scalar unitCellMass = 0.0;
          FOR_ALL(latticeIds, i) {
            label id = findIndex(pot_.idList(), latticeIds[i]);
            const molecule::constantProperties& cP(constProps(id));
            unitCellMass += cP.mass();
          }
          scalar massDensity =
            readScalar(zoneDict.lookup("massDensity"));
          if (massDensity < VSMALL) {
            WARNING_IN("moleculeCloud::initialiseMolecules")
              << "massDensity too small, not filling zone "
              << zone.name() << endl;
            continue;
          }
          latticeCellScale =
            pow(unitCellMass/(det(latticeCellShape)*massDensity), (1.0/3.0));
        } else {
          FATAL_ERROR_IN("mousse::moleculeCloud::initialiseMolecules")
            << "massDensity or numberDensity not specified " << nl
            << abort(FatalError);
        }
        latticeCellShape *= latticeCellScale;
        vector anchor{zoneDict.lookup("anchor")};
        bool tethered = false;
        if (zoneDict.found("tetherSiteIds")) {
          tethered =
            bool(List<word>(zoneDict.lookup("tetherSiteIds")).size());
        }
        const vector orientationAngles{zoneDict.lookup("orientationAngles")};
        scalar phi{orientationAngles.x()*pi/180.0};
        scalar theta{orientationAngles.y()*pi/180.0};
        scalar psi{orientationAngles.z()*pi/180.0};
        const tensor R
        {
          cos(psi)*cos(phi) - cos(theta)*sin(phi)*sin(psi),
          cos(psi)*sin(phi) + cos(theta)*cos(phi)*sin(psi),
          sin(psi)*sin(theta),
          -sin(psi)*cos(phi) - cos(theta)*sin(phi)*cos(psi),
          -sin(psi)*sin(phi) + cos(theta)*cos(phi)*cos(psi),
          cos(psi)*sin(theta),
          sin(theta)*sin(phi),
          -sin(theta)*cos(phi),
          cos(theta)
        };
        // Find the optimal anchor position.  Finding the approximate
        // mid-point of the zone of cells and snapping to the nearest
        // lattice location.
        vector zoneMin = VGREAT*vector::one;
        vector zoneMax = -VGREAT*vector::one;
        FOR_ALL(zone, cell) {
          const point cellCentre = mesh_.cellCentres()[zone[cell]];
          if (cellCentre.x() > zoneMax.x()) {
            zoneMax.x() = cellCentre.x();
          }
          if (cellCentre.x() < zoneMin.x()) {
            zoneMin.x() = cellCentre.x();
          }
          if (cellCentre.y() > zoneMax.y()) {
            zoneMax.y() = cellCentre.y();
          }
          if (cellCentre.y() < zoneMin.y()) {
            zoneMin.y() = cellCentre.y();
          }
          if (cellCentre.z() > zoneMax.z()) {
            zoneMax.z() = cellCentre.z();
          }
          if (cellCentre.z() < zoneMin.z()) {
            zoneMin.z() = cellCentre.z();
          }
        }
        point zoneMid = 0.5*(zoneMin + zoneMax);
        point latticeMid = inv(latticeCellShape) & (R.T() & (zoneMid - anchor));
        point latticeAnchor
        {
          label(latticeMid.x() + 0.5*sign(latticeMid.x())),
          label(latticeMid.y() + 0.5*sign(latticeMid.y())),
          label(latticeMid.z() + 0.5*sign(latticeMid.z()))
        };
        anchor += (R & (latticeCellShape & latticeAnchor));
        // Continue trying to place molecules as long as at
        // least one molecule is placed in each iteration.
        // The "|| totalZoneMols == 0" condition means that the
        // algorithm will continue if the origin is outside the
        // zone.
        label n = 0;
        label totalZoneMols = 0;
        label molsPlacedThisIteration = 0;
        while (molsPlacedThisIteration != 0 || totalZoneMols == 0) {
          label sizeBeforeIteration = this->size();
          bool partOfLayerInBounds = false;
          // start of placement of molecules
          if (n == 0) {
            // Special treatment is required for the first position,
            // i.e. iteration zero.
            labelVector unitCellLatticePosition{0,0,0};
            FOR_ALL(latticePositions, p) {
              label id = findIndex(pot_.idList(), latticeIds[p]);
              const vector& latticePosition =
                vector
                {
                  unitCellLatticePosition.x(),
                  unitCellLatticePosition.y(),
                  unitCellLatticePosition.z()
                }
                + latticePositions[p];
              point globalPosition =
                anchor + (R & (latticeCellShape & latticePosition));
              partOfLayerInBounds =
                mesh_.bounds().contains(globalPosition);
              label cell = -1;
              label tetFace = -1;
              label tetPt = -1;
              mesh_.findCellFacePt
              (
                globalPosition,
                cell,
                tetFace,
                tetPt
              );
              if (findIndex(zone, cell) != -1) {
                createMolecule
                (
                  globalPosition,
                  cell,
                  tetFace,
                  tetPt,
                  id,
                  tethered,
                  temperature,
                  bulkVelocity
                );
              }
            }
          } else {
            // Place top and bottom caps.
            labelVector unitCellLatticePosition{0,0,0};
            for (unitCellLatticePosition.z() = -n;
                 unitCellLatticePosition.z() <= n;
                 unitCellLatticePosition.z() += 2*n) {
              for (unitCellLatticePosition.y() = -n;
                   unitCellLatticePosition.y() <= n;
                   unitCellLatticePosition.y()++) {
                for (unitCellLatticePosition.x() = -n;
                     unitCellLatticePosition.x() <= n;
                     unitCellLatticePosition.x()++) {
                  FOR_ALL(latticePositions, p) {
                    label id =
                      findIndex(pot_.idList(), latticeIds[p]);
                    const vector& latticePosition =
                      vector
                      {
                        unitCellLatticePosition.x(),
                        unitCellLatticePosition.y(),
                        unitCellLatticePosition.z()
                      }
                      + latticePositions[p];
                    point globalPosition =
                      anchor + (R & (latticeCellShape & latticePosition));
                    partOfLayerInBounds =
                      mesh_.bounds().contains(globalPosition);
                    label cell = -1;
                    label tetFace = -1;
                    label tetPt = -1;
                    mesh_.findCellFacePt
                    (
                      globalPosition,
                      cell,
                      tetFace,
                      tetPt
                    );
                    if (findIndex(zone, cell) != -1) {
                      createMolecule
                      (
                        globalPosition,
                        cell,
                        tetFace,
                        tetPt,
                        id,
                        tethered,
                        temperature,
                        bulkVelocity
                      );
                    }
                  }
                }
              }
            }
            for (unitCellLatticePosition.z() = -(n-1);
                 unitCellLatticePosition.z() <= (n-1);
                 unitCellLatticePosition.z()++) {
              for (label iR = 0; iR <= 2*n -1; iR++) {
                unitCellLatticePosition.x() = n;
                unitCellLatticePosition.y() = -n + (iR + 1);
                for (label iK = 0; iK < 4; iK++) {
                  FOR_ALL(latticePositions, p) {
                    label id = findIndex(pot_.idList(), latticeIds[p]);
                    const vector& latticePosition =
                      vector
                      {
                        unitCellLatticePosition.x(),
                        unitCellLatticePosition.y(),
                        unitCellLatticePosition.z()
                      }
                      + latticePositions[p];
                    point globalPosition =
                      anchor + (R & (latticeCellShape & latticePosition));
                    partOfLayerInBounds =
                      mesh_.bounds().contains(globalPosition);
                    label cell = -1;
                    label tetFace = -1;
                    label tetPt = -1;
                    mesh_.findCellFacePt
                    (
                      globalPosition,
                      cell,
                      tetFace,
                      tetPt
                    );
                    if (findIndex(zone, cell) != -1) {
                      createMolecule
                      (
                        globalPosition,
                        cell,
                        tetFace,
                        tetPt,
                        id,
                        tethered,
                        temperature,
                        bulkVelocity
                      );
                    }
                  }
                  unitCellLatticePosition =
                    labelVector
                    {
                     -unitCellLatticePosition.y(),
                      unitCellLatticePosition.x(),
                      unitCellLatticePosition.z()
                    };
                }
              }
            }
          }
          // end of placement of molecules
          if (totalZoneMols == 0 && !partOfLayerInBounds) {
            WARNING_IN("mousse::moleculeCloud::initialiseMolecules()")
              << "A whole layer of unit cells was placed "
              << "outside the bounds of the mesh, but no "
              << "molecules have been placed in zone '"
              << zone.name()
              << "'.  This is likely to be because the zone "
              << "has few cells ("
              << zone.size()
              << " in this case) and no lattice position "
              << "fell inside them.  "
              << "Aborting filling this zone."
              << endl;
            break;
          }
          molsPlacedThisIteration =
            this->size() - sizeBeforeIteration;
          totalZoneMols += molsPlacedThisIteration;
          n++;
        }
      }
    }
  }
}


void mousse::moleculeCloud::createMolecule
(
  const point& position,
  label cell,
  label tetFace,
  label tetPt,
  label id,
  bool tethered,
  scalar temperature,
  const vector& bulkVelocity
)
{
  if (cell == -1) {
    mesh_.findCellFacePt(position, cell, tetFace, tetPt);
  }
  if (cell == -1) {
    FATAL_ERROR_IN("mousse::moleculeCloud::createMolecule")
      << "Position specified does not correspond to a mesh cell." << nl
      << abort(FatalError);
  }
  point specialPosition{vector::zero};
  label special = 0;
  if (tethered) {
    specialPosition = position;
    special = molecule::SPECIAL_TETHERED;
  }
  const molecule::constantProperties& cP = constProps(id);
  vector v = equipartitionLinearVelocity(temperature, cP.mass());
  v += bulkVelocity;
  vector pi = vector::zero;
  tensor Q = I;
  if (!cP.pointMolecule()) {
    pi = equipartitionAngularMomentum(temperature, cP);
    scalar phi(rndGen_.scalar01()*twoPi);
    scalar theta(rndGen_.scalar01()*twoPi);
    scalar psi(rndGen_.scalar01()*twoPi);
    Q =
      tensor
      {
        cos(psi)*cos(phi) - cos(theta)*sin(phi)*sin(psi),
        cos(psi)*sin(phi) + cos(theta)*cos(phi)*sin(psi),
        sin(psi)*sin(theta),
        -sin(psi)*cos(phi) - cos(theta)*sin(phi)*cos(psi),
        -sin(psi)*sin(phi) + cos(theta)*cos(phi)*cos(psi),
        cos(psi)*sin(theta),
        sin(theta)*sin(phi),
        -sin(theta)*cos(phi),
        cos(theta)
      };
  }
  addParticle
  (
    new molecule
    {
      mesh_,
      position,
      cell,
      tetFace,
      tetPt,
      Q,
      v,
      vector::zero,
      pi,
      vector::zero,
      specialPosition,
      constProps(id),
      special,
      id
    }
  );
}


mousse::label mousse::moleculeCloud::nSites() const
{
  label n = 0;
  FOR_ALL_CONST_ITER(moleculeCloud, *this, mol) {
    n += constProps(mol().id()).nSites();
  }
  return n;
}


// Constructors 
mousse::moleculeCloud::moleculeCloud
(
  const polyMesh& mesh,
  const potential& pot,
  bool readFields
)
:
  Cloud<molecule>{mesh, "moleculeCloud", false},
  mesh_{mesh},
  pot_{pot},
  cellOccupancy_{mesh_.nCells()},
  il_{mesh_, pot_.pairPotentials().rCutMax(), false},
  constPropList_(),
  rndGen_{clock::getTime()}
{
  if (readFields) {
    molecule::readFields(*this);
  }
  buildConstProps();
  setSiteSizesAndPositions();
  removeHighEnergyOverlaps();
  calculateForce();
}


mousse::moleculeCloud::moleculeCloud
(
  const polyMesh& mesh,
  const potential& pot,
  const IOdictionary& mdInitialiseDict,
  bool readFields
)
:
  Cloud<molecule>{mesh, "moleculeCloud", false},
  mesh_{mesh},
  pot_{pot},
  il_{mesh_, 0.0, false},
  constPropList_(),
  rndGen_{clock::getTime()}
{
  if (readFields) {
    molecule::readFields(*this);
  }
  clear();
  buildConstProps();
  initialiseMolecules(mdInitialiseDict);
}


// Member Functions 
void mousse::moleculeCloud::evolve()
{
  molecule::trackingData td0{*this, 0};
  Cloud<molecule>::move(td0, mesh_.time().deltaTValue());
  molecule::trackingData td1{*this, 1};
  Cloud<molecule>::move(td1, mesh_.time().deltaTValue());
  molecule::trackingData td2{*this, 2};
  Cloud<molecule>::move(td2, mesh_.time().deltaTValue());
  calculateForce();
  molecule::trackingData td3{*this, 3};
  Cloud<molecule>::move(td3, mesh_.time().deltaTValue());
}


void mousse::moleculeCloud::calculateForce()
{
  buildCellOccupancy();
  // Set accumulated quantities to zero
  FOR_ALL_ITER(moleculeCloud, *this, mol) {
    mol().siteForces() = vector::zero;
    mol().potentialEnergy() = 0.0;
    mol().rf() = tensor::zero;
  }
  calculatePairForce();
  calculateTetherForce();
  calculateExternalForce();
}


void mousse::moleculeCloud::applyConstraintsAndThermostats
(
  const scalar targetTemperature,
  const scalar measuredTemperature
)
{
  scalar temperatureCorrectionFactor =
    sqrt(targetTemperature/max(VSMALL, measuredTemperature));
  Info
    << "----------------------------------------" << nl
    << "Temperature equilibration" << nl
    << "Target temperature = "
    << targetTemperature << nl
    << "Measured temperature = "
    << measuredTemperature << nl
    << "Temperature correction factor = "
    << temperatureCorrectionFactor << nl
    << "----------------------------------------"
    << endl;
  FOR_ALL_ITER(moleculeCloud, *this, mol) {
    mol().v() *= temperatureCorrectionFactor;
    mol().pi() *= temperatureCorrectionFactor;
  }
}


void mousse::moleculeCloud::writeXYZ(const fileName& fName) const
{
  OFstream os{fName};
  os << nSites() << nl << "moleculeCloud site positions in angstroms" << nl;
  FOR_ALL_CONST_ITER(moleculeCloud, *this, mol) {
    const molecule::constantProperties& cP = constProps(mol().id());
    FOR_ALL(mol().sitePositions(), i) {
      const point& sP = mol().sitePositions()[i];
      os << pot_.siteIdList()[cP.siteIds()[i]]
        << ' ' << sP.x()*1e10
        << ' ' << sP.y()*1e10
        << ' ' << sP.z()*1e10
        << nl;
    }
  }
}

