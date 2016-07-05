// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "linear_valve_fv_mesh.hpp"
#include "time.hpp"
#include "sliding_interface.hpp"
#include "map_poly_mesh.hpp"
#include "poly_topo_change.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(linearValveFvMesh, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoChangerFvMesh, linearValveFvMesh, IOobject);

}


// Private Member Functions 
void mousse::linearValveFvMesh::addZonesAndModifiers()
{
  // Add zones and modifiers for motion action
  if ( pointZones().size()
       || faceZones().size()
       || cellZones().size()
       || topoChanger_.size()) {
    Info << "void linearValveFvMesh::addZonesAndModifiers() : "
      << "Zones and modifiers already present.  Skipping."
      << endl;
    return;
  }
  Info << "Time = " << time().timeName() << endl
    << "Adding zones and modifiers to the mesh" << endl;
  // Add zones
  List<pointZone*> pz{1};
  // Add an empty zone for cut points
  pz[0] = new pointZone{"cutPointZone", labelList(0), 0, pointZones()};
  // Do face zones for slider
  List<faceZone*> fz{3};
  // Inner slider
  const word innerSliderName{motionDict_.subDict("slider").lookup("inside")};
  const polyPatch& innerSlider = boundaryMesh()[innerSliderName];
  labelList isf{innerSlider.size()};
  FOR_ALL(isf, i) {
    isf[i] = innerSlider.start() + i;
  }
  fz[0] = new faceZone{"insideSliderZone", isf,
                       boolList{innerSlider.size(), false}, 0, faceZones()};
  // Outer slider
  const word outerSliderName{motionDict_.subDict("slider").lookup("outside")};
  const polyPatch& outerSlider = boundaryMesh()[outerSliderName];
  labelList osf{outerSlider.size()};
  FOR_ALL(osf, i) {
    osf[i] = outerSlider.start() + i;
  }
  fz[1] = new faceZone{"outsideSliderZone", osf,
                       boolList{outerSlider.size(), false}, 1, faceZones()};
  // Add empty zone for cut faces
  fz[2] =
    new faceZone{"cutFaceZone", labelList{0}, boolList{0, false}, 2, faceZones()};
  List<cellZone*> cz{0};
  Info << "Adding point, face and cell zones" << endl;
  addZones(pz, fz, cz);
  // Add a topology modifier
  Info << "Adding topology modifiers" << endl;
  topoChanger_.setSize(1);
  topoChanger_.set
    (
      0,
      new slidingInterface
      {
        "mixerSlider",
        0,
        topoChanger_,
        outerSliderName + "Zone",
        innerSliderName + "Zone",
        "cutPointZone",
        "cutFaceZone",
        outerSliderName,
        innerSliderName,
        slidingInterface::INTEGRAL,
        true                          // Attach-detach action
      }
    );
  topoChanger_.writeOpt() = IOobject::AUTO_WRITE;
  // Write mesh
  write();
}


void mousse::linearValveFvMesh::makeSlidersDead()
{
  const polyTopoChanger& topoChanges = topoChanger_;
  // Enable layering
  FOR_ALL(topoChanges, modI) {
    if (isA<slidingInterface>(topoChanges[modI])) {
      topoChanges[modI].disable();
    } else {
      FATAL_ERROR_IN("void mousse::linearValveFvMesh::makeSlidersDead()")
        << "Don't know what to do with mesh modifier "
        << modI << " of type " << topoChanges[modI].type()
        << abort(FatalError);
    }
  }
}


void mousse::linearValveFvMesh::makeSlidersLive()
{
  const polyTopoChanger& topoChanges = topoChanger_;
  // Enable sliding interface
  FOR_ALL(topoChanges, modI) {
    if (isA<slidingInterface>(topoChanges[modI])) {
      topoChanges[modI].enable();
    } else {
      FATAL_ERROR_IN("void mousse::linearValveFvMesh::makeLayersLive()")
        << "Don't know what to do with mesh modifier "
        << modI << " of type " << topoChanges[modI].type()
        << abort(FatalError);
    }
  }
}


bool mousse::linearValveFvMesh::attached() const
{
  const polyTopoChanger& topoChanges = topoChanger_;
  bool result = false;
  FOR_ALL(topoChanges, modI) {
    if (isA<slidingInterface>(topoChanges[modI])) {
      result =
        result || refCast<const slidingInterface>(topoChanges[modI]).attached();
    }
  }
  // Check thal all sliders are in sync (debug only)
  FOR_ALL(topoChanges, modI) {
    if (!isA<slidingInterface>(topoChanges[modI]))
      continue;
    if (result != refCast<const slidingInterface>(topoChanges[modI]).attached()) {
      FATAL_ERROR_IN("bool linearValveFvMesh::attached() const")
        << "Slider " << modI
        << " named " << topoChanges[modI].name()
        << " out of sync: Should be" << result
        << abort(FatalError);
    }
  }
  if (result) {
    Info << "linearValveFvMesh: attached!" << endl;
  } else {
    Info << "linearValveFvMesh: detached!" << endl;
  }
  return result;
}


// Constructors 

// Construct from components
mousse::linearValveFvMesh::linearValveFvMesh(const IOobject& io)
:
  topoChangerFvMesh{io},
  motionDict_
  {
    IOdictionary
    {
      {
        "dynamicMeshDict",
        time().constant(),
        *this,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE,
        false
      }
    }.subDict(typeName + "Coeffs")
  },
  msPtr_(motionSolver::New(*this))
{
  addZonesAndModifiers();
}


// Destructor 
mousse::linearValveFvMesh::~linearValveFvMesh()
{}


// Member Functions 
void mousse::linearValveFvMesh::update()
{
  // Detaching the interface
  if (attached()) {
    Info << "Decoupling sliding interfaces" << endl;
    makeSlidersLive();
    // Changing topology by hand
    resetMorph();
    setMorphTimeIndex(3*time().timeIndex());
    updateMesh();
    msPtr_->updateMesh();
  } else {
    Info << "Sliding interfaces decoupled" << endl;
  }
  // Perform mesh motion
  makeSlidersDead();
  // Changing topology by hand
  resetMorph();
  setMorphTimeIndex(3*time().timeIndex() + 1);
  updateMesh();
  msPtr_->updateMesh();
  if (topoChangeMap.valid()) {
    if (topoChangeMap().hasMotionPoints()) {
      Info << "Topology change; executing pre-motion" << endl;
      movePoints(topoChangeMap().preMotionPoints());
    }
  }
  // Solve for motion
  msPtr_->solve();
  movePoints(msPtr_->curPoints());
  // Attach the interface
  Info << "Coupling sliding interfaces" << endl;
  makeSlidersLive();
  resetMorph();
  setMorphTimeIndex(3*time().timeIndex() + 2);
  updateMesh();
  Info << "Moving points post slider attach" << endl;
  msPtr_->updateMesh();
  Info << "Sliding interfaces coupled: " << attached() << endl;
}
