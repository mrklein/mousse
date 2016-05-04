// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cyclic_poly_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "poly_boundary_mesh.hpp"
#include "poly_mesh.hpp"
#include "demand_driven_data.hpp"
#include "ofstream.hpp"
#include "patch_zones.hpp"
#include "match_points.hpp"
#include "edge_map.hpp"
#include "time.hpp"
#include "diag_tensor.hpp"
#include "transform_field.hpp"
#include "sub_field.hpp"
#include "unit_conversion.hpp"
#include "indirect_list.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(cyclicPolyPatch, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(polyPatch, cyclicPolyPatch, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(polyPatch, cyclicPolyPatch, dictionary);

}


mousse::label mousse::cyclicPolyPatch::findMaxArea
(
  const pointField& points,
  const faceList& faces
)
{
  label maxI = -1;
  scalar maxAreaSqr = -GREAT;
  FOR_ALL(faces, faceI) {
    scalar areaSqr = magSqr(faces[faceI].normal(points));
    if (areaSqr > maxAreaSqr) {
      maxAreaSqr = areaSqr;
      maxI = faceI;
    }
  }
  return maxI;
}


void mousse::cyclicPolyPatch::calcTransforms()
{
  if (size()) {
    // Half0
    const cyclicPolyPatch& half0 = *this;
    vectorField half0Areas{half0.size()};
    FOR_ALL(half0, facei) {
      half0Areas[facei] = half0[facei].normal(half0.points());
    }
    // Half1
    const cyclicPolyPatch& half1 = neighbPatch();
    vectorField half1Areas(half1.size());
    FOR_ALL(half1, facei) {
      half1Areas[facei] = half1[facei].normal(half1.points());
    }
    calcTransforms
    (
      half0,
      half0.faceCentres(),
      half0Areas,
      half1.faceCentres(),
      half1Areas
    );
  }
}


void mousse::cyclicPolyPatch::calcTransforms
(
  const primitivePatch& half0,
  const pointField& half0Ctrs,
  const vectorField& half0Areas,
  const pointField& half1Ctrs,
  const vectorField& half1Areas
)
{
  if (debug && owner()) {
    fileName casePath(boundaryMesh().mesh().time().path());

    {
      fileName nm0{casePath/name()+"_faces.obj"};
      Pout << "cyclicPolyPatch::calcTransforms : Writing " << name()
        << " faces to OBJ file " << nm0 << endl;
      writeOBJ(nm0, half0, half0.points());
    }

    const cyclicPolyPatch& half1 = neighbPatch();

    {
      fileName nm1{casePath/half1.name()+"_faces.obj"};
      Pout << "cyclicPolyPatch::calcTransforms : Writing " << half1.name()
        << " faces to OBJ file " << nm1 << endl;
      writeOBJ(nm1, half1, half1.points());
    }

    {
      OFstream str{casePath/name()+"_to_" + half1.name() + ".obj"};
      label vertI = 0;
      Pout << "cyclicPolyPatch::calcTransforms :"
        << " Writing coupled face centres as lines to " << str.name()
        << endl;
      FOR_ALL(half0Ctrs, i) {
        const point& p0 = half0Ctrs[i];
        str << "v " << p0.x() << ' ' << p0.y() << ' ' << p0.z() << nl;
        vertI++;
        const point& p1 = half1Ctrs[i];
        str << "v " << p1.x() << ' ' << p1.y() << ' ' << p1.z() << nl;
        vertI++;
        str << "l " << vertI-1 << ' ' << vertI << nl;
      }
    }
  }
  // Some sanity checks
  if (half0Ctrs.size() != half1Ctrs.size()) {
    FATAL_ERROR_IN
    (
      "cyclicPolyPatch::calcTransforms()"
    )
    << "For patch " << name()
    << " there are " << half0Ctrs.size()
    << " face centres, for the neighbour patch " << neighbPatch().name()
    << " there are " << half1Ctrs.size()
    << exit(FatalError);
  }
  if (transform() != neighbPatch().transform()) {
    FATAL_ERROR_IN
    (
      "cyclicPolyPatch::calcTransforms()"
    )
    << "Patch " << name()
    << " has transform type " << transformTypeNames[transform()]
    << ", neighbour patch " << neighbPatchName()
    << " has transform type "
    << neighbPatch().transformTypeNames[neighbPatch().transform()]
    << exit(FatalError);
  }
  // Calculate transformation tensors
  if (half0Ctrs.size() > 0) {
    vectorField half0Normals{half0Areas.size()};
    vectorField half1Normals{half1Areas.size()};
    scalar maxAreaDiff = -GREAT;
    label maxAreaFacei = -1;
    FOR_ALL(half0, facei) {
      scalar magSf = mag(half0Areas[facei]);
      scalar nbrMagSf = mag(half1Areas[facei]);
      scalar avSf = (magSf + nbrMagSf)/2.0;
      if (magSf < ROOTVSMALL && nbrMagSf < ROOTVSMALL) {
        // Undetermined normal. Use dummy normal to force separation
        // check. (note use of sqrt(VSMALL) since that is how mag
        // scales)
        half0Normals[facei] = point(1, 0, 0);
        half1Normals[facei] = half0Normals[facei];
      } else {
        scalar areaDiff = mag(magSf - nbrMagSf)/avSf;
        if (areaDiff > maxAreaDiff) {
          maxAreaDiff = areaDiff;
          maxAreaFacei = facei;
        }
        if (areaDiff > matchTolerance()) {
          FATAL_ERROR_IN
          (
            "cyclicPolyPatch::calcTransforms()"
          )
          << "face " << facei
          << " area does not match neighbour by "
          << 100*areaDiff
          << "% -- possible face ordering problem." << endl
          << "patch:" << name()
          << " my area:" << magSf
          << " neighbour area:" << nbrMagSf
          << " matching tolerance:" << matchTolerance()
          << endl
          << "Mesh face:" << start()+facei
          << " fc:" << half0Ctrs[facei]
          << endl
          << "Neighbour fc:" << half1Ctrs[facei]
          << endl
          << "If you are certain your matching is correct"
          << " you can increase the 'matchTolerance' setting"
          << " in the patch dictionary in the boundary file."
          << endl
          << "Rerun with cyclic debug flag set"
          << " for more information." << exit(FatalError);
        } else {
          half0Normals[facei] = half0Areas[facei]/magSf;
          half1Normals[facei] = half1Areas[facei]/nbrMagSf;
        }
      }
    }
    // Print area match
    if (debug) {
      Pout << "cyclicPolyPatch::calcTransforms :"
        << " patch:" << name()
        << " Max area error:" << 100*maxAreaDiff << "% at face:"
        << maxAreaFacei << " at:" << half0Ctrs[maxAreaFacei]
        << " coupled face at:" << half1Ctrs[maxAreaFacei]
        << endl;
    }
    // Calculate transformation tensors
    if (transform() == ROTATIONAL) {
      // Calculate using the given rotation axis and centre. Do not
      // use calculated normals.
      vector n0 = findFaceMaxRadius(half0Ctrs);
      vector n1 = -findFaceMaxRadius(half1Ctrs);
      n0 /= mag(n0) + VSMALL;
      n1 /= mag(n1) + VSMALL;
      if (debug) {
        scalar theta = radToDeg(acos(n0 & n1));
        Pout << "cyclicPolyPatch::calcTransforms :"
          << " patch:" << name()
          << " Specified rotation :"
          << " n0:" << n0 << " n1:" << n1
          << " swept angle: " << theta << " [deg]"
          << endl;
      }
      // Extended tensor from two local coordinate systems calculated
      // using normal and rotation axis
      const tensor E0
      {
        rotationAxis_,
        (n0 ^ rotationAxis_),
        n0
      };
      const tensor E1
      {
        rotationAxis_,
        (-n1 ^ rotationAxis_),
        -n1
      };
      const tensor revT{E1.T() & E0};
      const_cast<tensorField&>(forwardT()) = tensorField(1, revT.T());
      const_cast<tensorField&>(reverseT()) = tensorField(1, revT);
      const_cast<vectorField&>(separation()).setSize(0);
      const_cast<boolList&>(collocated()) = boolList(1, false);
    } else {
      scalarField half0Tols
      {
        matchTolerance()
        *calcFaceTol
        (
          half0,
          half0.points(),
          static_cast<const pointField&>(half0Ctrs)
        )
      };
      calcTransformTensors
      (
        static_cast<const pointField&>(half0Ctrs),
        static_cast<const pointField&>(half1Ctrs),
        half0Normals,
        half1Normals,
        half0Tols,
        matchTolerance(),
        transform()
      );
      if (transform() == TRANSLATIONAL) {
        if (debug) {
          Pout << "cyclicPolyPatch::calcTransforms :"
            << " patch:" << name()
            << " Specified separation vector : "
            << separationVector_ << endl;
        }
        // Check that separation vectors are same.
        const scalar avgTol = average(half0Tols);
        if (mag(separationVector_ + neighbPatch().separationVector_) > avgTol) {
          WARNING_IN
          (
            "cyclicPolyPatch::calcTransforms()"
          )
          << "Specified separation vector " << separationVector_
          << " differs by that of neighbouring patch "
          << neighbPatch().separationVector_
          << " by more than tolerance " << avgTol << endl
          << "patch:" << name()
          << " neighbour:" << neighbPatchName()
          << endl;
        }
        // Override computed transform with specified.
        if (separation().size() != 1
            || mag(separation()[0] - separationVector_) > avgTol) {
          WARNING_IN
          (
            "cyclicPolyPatch::calcTransforms()"
          )
          << "Specified separationVector " << separationVector_
          << " differs from computed separation vector "
          << separation() << endl
          << "This probably means your geometry is not consistent"
          << " with the specified separation and might lead"
          << " to problems." << endl
          << "Continuing with specified separation vector "
          << separationVector_ << endl
          << "patch:" << name()
          << " neighbour:" << neighbPatchName()
          << endl;
        }
        // Set tensors
        const_cast<tensorField&>(forwardT()).clear();
        const_cast<tensorField&>(reverseT()).clear();
        const_cast<vectorField&>(separation()) = vectorField
        {
          1,
          separationVector_
        };
        const_cast<boolList&>(collocated()) = boolList(1, false);
      }
    }
  }
}


void mousse::cyclicPolyPatch::getCentresAndAnchors
(
  const primitivePatch& pp0,
  const primitivePatch& pp1,
  pointField& half0Ctrs,
  pointField& half1Ctrs,
  pointField& anchors0,
  scalarField& tols
) const
{
  // Get geometric data on both halves.
  half0Ctrs = pp0.faceCentres();
  anchors0 = getAnchorPoints(pp0, pp0.points(), transform());
  half1Ctrs = pp1.faceCentres();
  if (debug) {
    Pout << "cyclicPolyPatch::getCentresAndAnchors :"
      << " patch:" << name() << nl
      << "half0 untransformed faceCentres (avg) : "
      << gAverage(half0Ctrs) << nl
      << "half1 untransformed faceCentres (avg) : "
      << gAverage(half1Ctrs) << endl;
  }
  if (half0Ctrs.size()) {
    switch (transform()) {
      case ROTATIONAL:
      {
        vector n0 = findFaceMaxRadius(half0Ctrs);
        vector n1 = -findFaceMaxRadius(half1Ctrs);
        n0 /= mag(n0) + VSMALL;
        n1 /= mag(n1) + VSMALL;
        if (debug) {
          scalar theta = radToDeg(acos(n0 & n1));
          Pout << "cyclicPolyPatch::getCentresAndAnchors :"
            << " patch:" << name()
            << " Specified rotation :"
            << " n0:" << n0 << " n1:" << n1
            << " swept angle: " << theta << " [deg]"
            << endl;
        }
        // Extended tensor from two local coordinate systems calculated
        // using normal and rotation axis
        const tensor E0
        {
          rotationAxis_,
          (n0 ^ rotationAxis_),
          n0
        };
        const tensor E1
        {
          rotationAxis_,
          (-n1 ^ rotationAxis_),
          -n1
        };
        const tensor revT{E1.T() & E0};
        // Rotation
        FOR_ALL(half0Ctrs, faceI) {
          half0Ctrs[faceI] =
            mousse::transform
            (
              revT,
              half0Ctrs[faceI] - rotationCentre_
            )
           + rotationCentre_;
          anchors0[faceI] =
            mousse::transform
            (
              revT,
              anchors0[faceI] - rotationCentre_
            )
           + rotationCentre_;
        }
        break;
      }
      case TRANSLATIONAL:
      {
        // Transform 0 points.
        if (debug) {
          Pout << "cyclicPolyPatch::getCentresAndAnchors :"
            << " patch:" << name()
            << "Specified translation : " << separationVector_
            << endl;
        }
        // Note: getCentresAndAnchors gets called on the slave side
        // so separationVector is owner-slave points.
        half0Ctrs -= separationVector_;
        anchors0 -= separationVector_;
        break;
      }
      default:
      {
        // Assumes that cyclic is rotational. This is also the initial
        // condition for patches without faces.
        // Determine the face with max area on both halves. These
        // two faces are used to determine the transformation tensors
        label max0I = findMaxArea(pp0.points(), pp0);
        vector n0 = pp0[max0I].normal(pp0.points());
        n0 /= mag(n0) + VSMALL;
        label max1I = findMaxArea(pp1.points(), pp1);
        vector n1 = pp1[max1I].normal(pp1.points());
        n1 /= mag(n1) + VSMALL;
        if (mag(n0 & n1) < 1-matchTolerance()) {
          if (debug) {
            Pout << "cyclicPolyPatch::getCentresAndAnchors :"
              << " patch:" << name()
              << " Detected rotation :"
              << " n0:" << n0 << " n1:" << n1 << endl;
          }
          // Rotation (around origin)
          const tensor revT(rotationTensor(n0, -n1));
          // Rotation
          FOR_ALL(half0Ctrs, faceI) {
            half0Ctrs[faceI] = mousse::transform
            (
              revT,
              half0Ctrs[faceI]
            );
            anchors0[faceI] = mousse::transform
            (
              revT,
              anchors0[faceI]
            );
          }
        } else {
          // Parallel translation. Get average of all used points.
          const point ctr0{sum(pp0.localPoints())/pp0.nPoints()};
          const point ctr1{sum(pp1.localPoints())/pp1.nPoints()};
          if (debug) {
            Pout << "cyclicPolyPatch::getCentresAndAnchors :"
              << " patch:" << name()
              << " Detected translation :"
              << " n0:" << n0 << " n1:" << n1
              << " ctr0:" << ctr0 << " ctr1:" << ctr1 << endl;
          }
          half0Ctrs += ctr1 - ctr0;
          anchors0 += ctr1 - ctr0;
        }
        break;
      }
    }
  }
  // Calculate typical distance per face
  tols = matchTolerance()*calcFaceTol(pp1, pp1.points(), half1Ctrs);
}


mousse::vector mousse::cyclicPolyPatch::findFaceMaxRadius
(
  const pointField& faceCentres
) const
{
  // Determine a face furthest away from the axis
  const vectorField n{(faceCentres - rotationCentre_) ^ rotationAxis_};
  const scalarField magRadSqr{magSqr(n)};
  label faceI = findMax(magRadSqr);
  if (debug) {
    Info << "findFaceMaxRadius(const pointField&) : patch: " << name() << nl
      << "    rotFace  = " << faceI << nl
      << "    point    = " << faceCentres[faceI] << nl
      << "    distance = " << mousse::sqrt(magRadSqr[faceI])
      << endl;
  }
  return n[faceI];
}


// Constructors
mousse::cyclicPolyPatch::cyclicPolyPatch
(
  const word& name,
  const label size,
  const label start,
  const label index,
  const polyBoundaryMesh& bm,
  const word& patchType,
  const transformType transform
)
:
  coupledPolyPatch{name, size, start, index, bm, patchType, transform},
  neighbPatchName_{word::null},
  neighbPatchID_{-1},
  rotationAxis_{vector::zero},
  rotationCentre_{point::zero},
  separationVector_{vector::zero},
  coupledPointsPtr_{NULL},
  coupledEdgesPtr_{NULL}
{
  // Neighbour patch might not be valid yet so no transformation
  // calculation possible.
}


mousse::cyclicPolyPatch::cyclicPolyPatch
(
  const word& name,
  const label size,
  const label start,
  const label index,
  const polyBoundaryMesh& bm,
  const word& neighbPatchName,
  const transformType transform,
  const vector& rotationAxis,
  const point& rotationCentre,
  const vector& separationVector
)
:
  coupledPolyPatch{name, size, start, index, bm, typeName, transform},
  neighbPatchName_{neighbPatchName},
  neighbPatchID_{-1},
  rotationAxis_{rotationAxis},
  rotationCentre_{rotationCentre},
  separationVector_{separationVector},
  coupledPointsPtr_{NULL},
  coupledEdgesPtr_{NULL}
{
  // Neighbour patch might not be valid yet so no transformation
  // calculation possible.
}


mousse::cyclicPolyPatch::cyclicPolyPatch
(
  const word& name,
  const dictionary& dict,
  const label index,
  const polyBoundaryMesh& bm,
  const word& patchType
)
:
  coupledPolyPatch{name, dict, index, bm, patchType},
  neighbPatchName_{dict.lookupOrDefault("neighbourPatch", word::null)},
  coupleGroup_{dict},
  neighbPatchID_{-1},
  rotationAxis_{vector::zero},
  rotationCentre_{point::zero},
  separationVector_{vector::zero},
  coupledPointsPtr_{NULL},
  coupledEdgesPtr_{NULL}
{
  if (neighbPatchName_ == word::null && !coupleGroup_.valid()) {
    FATAL_IO_ERROR_IN
    (
      "cyclicPolyPatch::cyclicPolyPatch\n"
      "(\n"
      "    const word& name,\n"
      "    const dictionary& dict,\n"
      "    const label index,\n"
      "    const polyBoundaryMesh& bm\n"
      ")",
      dict
    )
    << "No \"neighbourPatch\" provided." << endl
    << "Is your mesh uptodate with split cyclics?" << endl
    << "Run foamUpgradeCyclics to convert mesh and fields"
    << " to split cyclics." << exit(FatalIOError);
  }
  if (neighbPatchName_ == name) {
    FATAL_IO_ERROR_IN("cyclicPolyPatch::cyclicPolyPatch(..)", dict)
      << "Neighbour patch name " << neighbPatchName_
      << " cannot be the same as this patch " << name
      << exit(FatalIOError);
  }
  switch (transform()) {
    case ROTATIONAL:
    {
      dict.lookup("rotationAxis") >> rotationAxis_;
      dict.lookup("rotationCentre") >> rotationCentre_;
      scalar magRot = mag(rotationAxis_);
      if (magRot < SMALL) {
        FATAL_IO_ERROR_IN("cyclicPolyPatch::cyclicPolyPatch(..)", dict)
          << "Illegal rotationAxis " << rotationAxis_ << endl
          << "Please supply a non-zero vector."
          << exit(FatalIOError);
      }
      rotationAxis_ /= magRot;
      break;
    }
    case TRANSLATIONAL:
    {
      dict.lookup("separationVector") >> separationVector_;
      break;
    }
    default:
    {
      // no additional info required
    }
  }
  // Neighbour patch might not be valid yet so no transformation
  // calculation possible.
}


mousse::cyclicPolyPatch::cyclicPolyPatch
(
  const cyclicPolyPatch& pp,
  const polyBoundaryMesh& bm
)
:
  coupledPolyPatch{pp, bm},
  neighbPatchName_{pp.neighbPatchName_},
  coupleGroup_{pp.coupleGroup_},
  neighbPatchID_{-1},
  rotationAxis_{pp.rotationAxis_},
  rotationCentre_{pp.rotationCentre_},
  separationVector_{pp.separationVector_},
  coupledPointsPtr_{NULL},
  coupledEdgesPtr_{NULL}
{
  // Neighbour patch might not be valid yet so no transformation
  // calculation possible.
}


mousse::cyclicPolyPatch::cyclicPolyPatch
(
  const cyclicPolyPatch& pp,
  const polyBoundaryMesh& bm,
  const label index,
  const label newSize,
  const label newStart,
  const word& neighbName
)
:
  coupledPolyPatch{pp, bm, index, newSize, newStart},
  neighbPatchName_{neighbName},
  coupleGroup_{pp.coupleGroup_},
  neighbPatchID_{-1},
  rotationAxis_{pp.rotationAxis_},
  rotationCentre_{pp.rotationCentre_},
  separationVector_{pp.separationVector_},
  coupledPointsPtr_{NULL},
  coupledEdgesPtr_{NULL}
{
  if (neighbName == name()) {
    FATAL_ERROR_IN("cyclicPolyPatch::cyclicPolyPatch(..)")
      << "Neighbour patch name " << neighbName
      << " cannot be the same as this patch " << name()
      << exit(FatalError);
  }
  // Neighbour patch might not be valid yet so no transformation
  // calculation possible.
}


mousse::cyclicPolyPatch::cyclicPolyPatch
(
  const cyclicPolyPatch& pp,
  const polyBoundaryMesh& bm,
  const label index,
  const labelUList& mapAddressing,
  const label newStart
)
:
  coupledPolyPatch{pp, bm, index, mapAddressing, newStart},
  neighbPatchName_{pp.neighbPatchName_},
  coupleGroup_{pp.coupleGroup_},
  neighbPatchID_{-1},
  rotationAxis_{pp.rotationAxis_},
  rotationCentre_{pp.rotationCentre_},
  separationVector_{pp.separationVector_},
  coupledPointsPtr_{NULL},
  coupledEdgesPtr_{NULL}
{}


// Destructor
mousse::cyclicPolyPatch::~cyclicPolyPatch()
{
  deleteDemandDrivenData(coupledPointsPtr_);
  deleteDemandDrivenData(coupledEdgesPtr_);
}


// Member Functions
const mousse::word& mousse::cyclicPolyPatch::neighbPatchName() const
{
  if (neighbPatchName_.empty()) {
    // Try and use patchGroup to find samplePatch and sampleRegion
    label patchID = coupleGroup_.findOtherPatchID(*this);
    neighbPatchName_ = boundaryMesh()[patchID].name();
  }
  return neighbPatchName_;
}


mousse::label mousse::cyclicPolyPatch::neighbPatchID() const
{
  if (neighbPatchID_ == -1) {
    neighbPatchID_ = this->boundaryMesh().findPatchID(neighbPatchName());
    if (neighbPatchID_ == -1) {
      FATAL_ERROR_IN("cyclicPolyPatch::neighbPatchID() const")
        << "Illegal neighbourPatch name " << neighbPatchName()
        << endl << "Valid patch names are "
        << this->boundaryMesh().names()
        << exit(FatalError);
    }
    // Check that it is a cyclic
    const cyclicPolyPatch& nbrPatch = refCast<const cyclicPolyPatch>
    (
      this->boundaryMesh()[neighbPatchID_]
    );
    if (nbrPatch.neighbPatchName() != name()) {
      WARNING_IN("cyclicPolyPatch::neighbPatchID() const")
        << "Patch " << name()
        << " specifies neighbour patch " << neighbPatchName()
        << endl << " but that in return specifies "
        << nbrPatch.neighbPatchName()
        << endl;
    }
  }
  return neighbPatchID_;
}


void mousse::cyclicPolyPatch::transformPosition(pointField& l) const
{
  if (!parallel()) {
    if (transform() == ROTATIONAL) {
      l = mousse::transform(forwardT(), l-rotationCentre_)
        + rotationCentre_;
    } else {
      l = mousse::transform(forwardT(), l);
    }
  } else if (separated()) {
    // transformPosition gets called on the receiving side,
    // separation gets calculated on the sending side so subtract.
    const vectorField& s = separation();
    if (s.size() == 1) {
      FOR_ALL(l, i) {
        l[i] -= s[0];
      }
    } else {
      l -= s;
    }
  }
}


void mousse::cyclicPolyPatch::transformPosition(point& l, const label facei) const
{
  if (!parallel()) {
    const tensor& T =
    (
      forwardT().size() == 1
      ? forwardT()[0]
      : forwardT()[facei]
    );
    if (transform() == ROTATIONAL) {
      l = mousse::transform(T, l-rotationCentre_) + rotationCentre_;
    } else {
      l = mousse::transform(T, l);
    }
  } else if (separated()) {
    const vector& s =
    (
      separation().size() == 1
      ? separation()[0]
      : separation()[facei]
    );
    l -= s;
  }
}


void mousse::cyclicPolyPatch::initGeometry(PstreamBuffers& pBufs)
{
  polyPatch::initGeometry(pBufs);
}


void mousse::cyclicPolyPatch::initGeometry
(
  const primitivePatch& /*referPatch*/,
  pointField& /*nbrCtrs*/,
  vectorField& /*nbrAreas*/,
  pointField& /*nbrCc*/
)
{}


void mousse::cyclicPolyPatch::calcGeometry
(
  const primitivePatch& referPatch,
  const pointField& thisCtrs,
  const vectorField& thisAreas,
  const pointField& /*thisCc*/,
  const pointField& nbrCtrs,
  const vectorField& nbrAreas,
  const pointField& /*nbrCc*/
)
{
  calcTransforms
  (
    referPatch,
    thisCtrs,
    thisAreas,
    nbrCtrs,
    nbrAreas
  );
}


void mousse::cyclicPolyPatch::calcGeometry(PstreamBuffers& /*pBufs*/)
{
  calcGeometry
  (
    *this,
    faceCentres(),
    faceAreas(),
    faceCellCentres(),
    neighbPatch().faceCentres(),
    neighbPatch().faceAreas(),
    neighbPatch().faceCellCentres()
  );
}


void mousse::cyclicPolyPatch::initMovePoints
(
  PstreamBuffers& pBufs,
  const pointField& p
)
{
  polyPatch::initMovePoints(pBufs, p);
}


void mousse::cyclicPolyPatch::movePoints
(
  PstreamBuffers& pBufs,
  const pointField& p
)
{
  polyPatch::movePoints(pBufs, p);
  calcTransforms();
}


void mousse::cyclicPolyPatch::initUpdateMesh(PstreamBuffers& pBufs)
{
  polyPatch::initUpdateMesh(pBufs);
}


void mousse::cyclicPolyPatch::updateMesh(PstreamBuffers& pBufs)
{
  polyPatch::updateMesh(pBufs);
  deleteDemandDrivenData(coupledPointsPtr_);
  deleteDemandDrivenData(coupledEdgesPtr_);
}


const mousse::edgeList& mousse::cyclicPolyPatch::coupledPoints() const
{
  if (!coupledPointsPtr_) {
    const faceList& nbrLocalFaces = neighbPatch().localFaces();
    const labelList& nbrMeshPoints = neighbPatch().meshPoints();
    // Now all we know is that relative face index in *this is same
    // as coupled face in nbrPatch and also that the 0th vertex
    // corresponds.
    // From local point to nbrPatch or -1.
    labelList coupledPoint{nPoints(), -1};
    FOR_ALL(*this, patchFaceI) {
      const face& fA = localFaces()[patchFaceI];
      const face& fB = nbrLocalFaces[patchFaceI];
      FOR_ALL(fA, indexA) {
        label patchPointA = fA[indexA];
        if (coupledPoint[patchPointA] == -1) {
          label indexB = (fB.size() - indexA) % fB.size();
          // Filter out points on wedge axis
          if (meshPoints()[patchPointA] != nbrMeshPoints[fB[indexB]]) {
            coupledPoint[patchPointA] = fB[indexB];
          }
        }
      }
    }
    coupledPointsPtr_ = new edgeList{nPoints()};
    edgeList& connected = *coupledPointsPtr_;
    // Extract coupled points.
    label connectedI = 0;
    FOR_ALL(coupledPoint, i) {
      if (coupledPoint[i] != -1) {
        connected[connectedI++] = edge(i, coupledPoint[i]);
      }
    }
    connected.setSize(connectedI);
    if (debug) {
      OFstream str
      {
        boundaryMesh().mesh().time().path()/name() + "_coupledPoints.obj"
      };
      label vertI = 0;
      Pout << "Writing file " << str.name() << " with coordinates of "
        << "coupled points" << endl;
      FOR_ALL(connected, i) {
        const point& a = points()[meshPoints()[connected[i][0]]];
        const point& b = points()[nbrMeshPoints[connected[i][1]]];
        str << "v " << a.x() << ' ' << a.y() << ' ' << a.z() << nl;
        str << "v " << b.x() << ' ' << b.y() << ' ' << b.z() << nl;
        vertI += 2;
        str << "l " << vertI-1 << ' ' << vertI << nl;
      }
    }
  }
  return *coupledPointsPtr_;
}


const mousse::edgeList& mousse::cyclicPolyPatch::coupledEdges() const
{
  if (!coupledEdgesPtr_) {
    const edgeList& pointCouples = coupledPoints();
    // Build map from points on *this (A) to points on neighbourpatch (B)
    Map<label> aToB{2*pointCouples.size()};
    FOR_ALL(pointCouples, i) {
      const edge& e = pointCouples[i];
      aToB.insert(e[0], e[1]);
    }
    // Map from edge on A to points (in B indices)
    EdgeMap<label> edgeMap{nEdges()};
    FOR_ALL(*this, patchFaceI) {
      const labelList& fEdges = faceEdges()[patchFaceI];
      FOR_ALL(fEdges, i) {
        label edgeI = fEdges[i];
        const edge& e = edges()[edgeI];
        // Convert edge end points to corresponding points on B side.
        Map<label>::const_iterator fnd0 = aToB.find(e[0]);
        if (fnd0 != aToB.end()) {
          Map<label>::const_iterator fnd1 = aToB.find(e[1]);
          if (fnd1 != aToB.end()) {
            edgeMap.insert(edge(fnd0(), fnd1()), edgeI);
          }
        }
      }
    }
    // Use the edgeMap to get the edges on the B side.
    const cyclicPolyPatch& neighbPatch = this->neighbPatch();
    const labelList& nbrMp = neighbPatch.meshPoints();
    const labelList& mp = meshPoints();
    coupledEdgesPtr_ = new edgeList{edgeMap.size()};
    edgeList& coupledEdges = *coupledEdgesPtr_;
    label coupleI = 0;
    FOR_ALL(neighbPatch, patchFaceI) {
      const labelList& fEdges = neighbPatch.faceEdges()[patchFaceI];
      FOR_ALL(fEdges, i) {
        label edgeI = fEdges[i];
        const edge& e = neighbPatch.edges()[edgeI];
        // Look up A edge from HashTable.
        EdgeMap<label>::iterator iter = edgeMap.find(e);
        if (iter != edgeMap.end()) {
          label edgeA = iter();
          const edge& eA = edges()[edgeA];
          // Store correspondence. Filter out edges on wedge axis.
          if (edge(mp[eA[0]], mp[eA[1]]) != edge(nbrMp[e[0]], nbrMp[e[1]])) {
            coupledEdges[coupleI++] = edge(edgeA, edgeI);
          }
          // Remove so we build unique list
          edgeMap.erase(iter);
        }
      }
    }
    coupledEdges.setSize(coupleI);
    // Some checks
    FOR_ALL(coupledEdges, i) {
      const edge& e = coupledEdges[i];
      if (e[0] < 0 || e[1] < 0) {
        FATAL_ERROR_IN("cyclicPolyPatch::coupledEdges() const")
          << "Problem : at position " << i
          << " illegal couple:" << e
          << abort(FatalError);
      }
    }
    if (debug) {
      OFstream str
      {
        boundaryMesh().mesh().time().path()/name() + "_coupledEdges.obj"
      };
      label vertI = 0;
      Pout << "Writing file " << str.name() << " with centres of "
        << "coupled edges" << endl;
      FOR_ALL(coupledEdges, i) {
        const edge& e = coupledEdges[i];
        const point& a = edges()[e[0]].centre(localPoints());
        const point& b = neighbPatch.edges()[e[1]].centre
        (
          neighbPatch.localPoints()
        );
        str << "v " << a.x() << ' ' << a.y() << ' ' << a.z() << nl;
        str << "v " << b.x() << ' ' << b.y() << ' ' << b.z() << nl;
        vertI += 2;
        str << "l " << vertI-1 << ' ' << vertI << nl;
      }
    }
  }

  return *coupledEdgesPtr_;
}


void mousse::cyclicPolyPatch::initOrder
(
  PstreamBuffers&,
  const primitivePatch& pp
) const
{
  if (owner()) {
    // Save patch for use in non-owner side ordering. Equivalent to
    // processorPolyPatch using OPstream.
    ownerPatchPtr_.reset
    (
      new primitivePatch
      {
        pp,
        pp.points()
      }
    );
  }
}


bool mousse::cyclicPolyPatch::order
(
  PstreamBuffers& /*pBufs*/,
  const primitivePatch& pp,
  labelList& faceMap,
  labelList& rotation
) const
{
  if (debug) {
    Pout << "order : of " << pp.size()
      << " faces of patch:" << name()
      << " neighbour:" << neighbPatchName()
      << endl;
  }
  faceMap.setSize(pp.size());
  faceMap = -1;
  rotation.setSize(pp.size());
  rotation = 0;
  if (transform() == NOORDERING) {
    // No faces, nothing to change.
    return false;
  }
  if (owner()) {
    // Do nothing (i.e. identical mapping, zero rotation).
    // See comment at top.
    FOR_ALL(faceMap, patchFaceI) {
      faceMap[patchFaceI] = patchFaceI;
    }
    return false;
  } else {
    // Get stored geometry from initOrder invocation of owner.
    const primitivePatch& pp0 = neighbPatch().ownerPatchPtr_();
    // Get geometric quantities
    pointField half0Ctrs, half1Ctrs, anchors0;
    scalarField tols;
    getCentresAndAnchors
    (
      pp0,
      pp,
      half0Ctrs,
      half1Ctrs,
      anchors0,
      tols
    );
    if (debug) {
      Pout << "half0 transformed faceCentres (avg)   : "
        << gAverage(half0Ctrs) << nl
        << "half1 untransformed faceCentres (avg) : "
        << gAverage(half1Ctrs) << endl;
    }
    // Geometric match of face centre vectors
    bool matchedAll = matchPoints
    (
      half1Ctrs,
      half0Ctrs,
      tols,
      true,
      faceMap
    );
    if (!matchedAll || debug) {
      // Dump halves
      fileName nm0
      {
        boundaryMesh().mesh().time().path()/neighbPatch().name()+"_faces.obj"
      };
      Pout << "cyclicPolyPatch::order : Writing neighbour"
        << " faces to OBJ file " << nm0 << endl;
      writeOBJ(nm0, pp0, pp0.points());
      fileName nm1
      {
        boundaryMesh().mesh().time().path()/name()+"_faces.obj"
      };
      Pout << "cyclicPolyPatch::order : Writing my"
        << " faces to OBJ file " << nm1 << endl;
      writeOBJ(nm1, pp, pp.points());
      OFstream ccStr
      {
        boundaryMesh().mesh().time().path()/name() + "_faceCentres.obj"
      };
      Pout << "cyclicPolyPatch::order : "
        << "Dumping currently found cyclic match as lines between"
        << " corresponding face centres to file " << ccStr.name()
        << endl;
      // Recalculate untransformed face centres
      //pointField rawHalf0Ctrs =
      //    calcFaceCentres(half0Faces, pp.points());
      label vertI = 0;
      FOR_ALL(half1Ctrs, i) {
        if (faceMap[i] != -1) {
          // Write edge between c1 and c0
          const point& c0 = half0Ctrs[faceMap[i]];
          const point& c1 = half1Ctrs[i];
          writeOBJ(ccStr, c0, c1, vertI);
        }
      }
    }
    if (!matchedAll) {
      SERIOUS_ERROR_IN
      (
        "cyclicPolyPatch::order"
        "(const primitivePatch&, labelList&, labelList&) const"
      )
      << "Patch:" << name() << " : "
      << "Cannot match vectors to faces on both sides of patch"
      << endl
      << "    Perhaps your faces do not match?"
      << " The obj files written contain the current match." << endl
      << "    Continuing with incorrect face ordering from now on!"
      << endl;
      return false;
    }
    // Set rotation.
    FOR_ALL(faceMap, oldFaceI) {
      // The face f will be at newFaceI (after morphing) and we want its
      // anchorPoint (= f[0]) to align with the anchorpoint for the
      // corresponding face on the other side.
      label newFaceI = faceMap[oldFaceI];
      const point& wantedAnchor = anchors0[newFaceI];
      rotation[newFaceI] = getRotation
      (
        pp.points(),
        pp[oldFaceI],
        wantedAnchor,
        tols[oldFaceI]
      );
      if (rotation[newFaceI] == -1) {
        SERIOUS_ERROR_IN
        (
          "cyclicPolyPatch::order(const primitivePatch&"
          ", labelList&, labelList&) const"
        )
        << "in patch " << name()
        << " : "
        << "Cannot find point on face " << pp[oldFaceI]
        << " with vertices "
        << IndirectList<point>(pp.points(), pp[oldFaceI])()
        << " that matches point " << wantedAnchor
        << " when matching the halves of processor patch " << name()
        << "Continuing with incorrect face ordering from now on!"
        << endl;
        return false;
      }
    }
    ownerPatchPtr_.clear();
    // Return false if no change neccesary, true otherwise.
    FOR_ALL(faceMap, faceI) {
      if (faceMap[faceI] != faceI || rotation[faceI] != 0) {
        return true;
      }
    }
    return false;
  }
}


void mousse::cyclicPolyPatch::write(Ostream& os) const
{
  coupledPolyPatch::write(os);
  if (!neighbPatchName_.empty()) {
    os.writeKeyword("neighbourPatch") << neighbPatchName_
      << token::END_STATEMENT << nl;
  }
  coupleGroup_.write(os);
  switch (transform()) {
    case ROTATIONAL:
    {
      os.writeKeyword("rotationAxis") << rotationAxis_
        << token::END_STATEMENT << nl;
      os.writeKeyword("rotationCentre") << rotationCentre_
        << token::END_STATEMENT << nl;
      break;
    }
    case TRANSLATIONAL:
    {
      os.writeKeyword("separationVector") << separationVector_
        << token::END_STATEMENT << nl;
      break;
    }
    case NOORDERING:
    {
      break;
    }
    default:
    {
      // no additional info to write
    }
  }
}
