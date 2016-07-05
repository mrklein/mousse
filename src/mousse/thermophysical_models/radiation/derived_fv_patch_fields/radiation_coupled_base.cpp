// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "radiation_coupled_base.hpp"
#include "vol_fields.hpp"
#include "mapped_patch_base.hpp"
#include "fv_patch_field_mapper.hpp"
#include "radiation_model.hpp"
#include "absorption_emission_model.hpp"


// Static Member Data 
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(radiationCoupledBase, 0);

}


namespace mousse
{
  template<>
  const char* mousse::NamedEnum
  <
    mousse::radiationCoupledBase::emissivityMethodType,
    2
  >::names[] =
  {
    "solidRadiation",
    "lookup"
  };
}

const mousse::NamedEnum<mousse::radiationCoupledBase::emissivityMethodType, 2>
  mousse::radiationCoupledBase::emissivityMethodTypeNames_;


// Constructors 
mousse::radiationCoupledBase::radiationCoupledBase
(
  const fvPatch& patch,
  const word& calculationType,
  const scalarField& emissivity
)
:
  patch_{patch},
  method_{emissivityMethodTypeNames_[calculationType]},
  emissivity_{emissivity}
{}


mousse::radiationCoupledBase::radiationCoupledBase
(
  const fvPatch& patch,
  const word& calculationType,
  const scalarField& emissivity,
  const fvPatchFieldMapper& mapper
)
:
  patch_{patch},
  method_{emissivityMethodTypeNames_[calculationType]},
  emissivity_{emissivity, mapper}
{}


mousse::radiationCoupledBase::radiationCoupledBase
(
  const fvPatch& patch,
  const dictionary& dict
)
:
  patch_{patch},
  method_{emissivityMethodTypeNames_.read(dict.lookup("emissivityMode"))}
{
  switch (method_) {
    case SOLIDRADIATION:
      {
        if (!isA<mappedPatchBase>(patch_.patch())) {
          FATAL_IO_ERROR_IN
          (
            "radiationCoupledBase::radiationCoupledBase\n"
            "(\n"
            "    const fvPatch& p,\n"
            "    const dictionary& dict\n"
            ")\n",
            dict
          )
          << "\n    patch type '" << patch_.type()
          << "' not type '" << mappedPatchBase::typeName << "'"
          << "\n    for patch " << patch_.name()
          << exit(FatalIOError);
        }
        emissivity_ = scalarField{patch_.size(), 0.0};
      }
      break;
    case LOOKUP:
      {
        if (!dict.found("emissivity")) {
          FATAL_IO_ERROR_IN
          (
            "radiationCoupledBase::radiationCoupledBase\n"
            "(\n"
            "    const fvPatch& p,\n"
            "    const dictionary& dict\n"
            ")\n",
            dict
          )
          << "\n    emissivity key does not exist for patch "
          << patch_.name()
          << exit(FatalIOError);
        } else {
          emissivity_ = scalarField{"emissivity", dict, patch_.size()};
        }
      }
    break;
  }
}


// Destructor   
mousse::radiationCoupledBase::~radiationCoupledBase()
{}


// Member Functions 
mousse::scalarField mousse::radiationCoupledBase::emissivity() const
{
  switch (method_) {
    case SOLIDRADIATION:
      {
        // Get the coupling information from the mappedPatchBase
        const mappedPatchBase& mpp =
          refCast<const mappedPatchBase>(patch_.patch());
        const polyMesh& nbrMesh = mpp.sampleMesh();
        const radiation::radiationModel& radiation =
          nbrMesh.lookupObject<radiation::radiationModel>
          (
            "radiationProperties"
          );
        const fvMesh& nbrFvMesh = refCast<const fvMesh>(nbrMesh);
        const fvPatch& nbrPatch =
          nbrFvMesh.boundary()[mpp.samplePolyPatch().index()];
        scalarField
          emissivity
          {
            radiation.absorptionEmission().e()().boundaryField()[nbrPatch.index()]
          };
        mpp.distribute(emissivity);
        return emissivity;
      }
      break;
    case LOOKUP:
      {
        // return local value
        return emissivity_;
      }
    default:
      {
        FATAL_ERROR_IN
        (
          "radiationCoupledBase::emissivity(const scalarField&)"
        )
        << "Unimplemented method " << method_ << endl
        << "Please set 'emissivity' to one of "
        << emissivityMethodTypeNames_.toc()
        << exit(FatalError);
      }
      break;
  }
  return scalarField{0};
}


void mousse::radiationCoupledBase::autoMap
(
  const fvPatchFieldMapper& m
)
{
  emissivity_.autoMap(m);
}


void mousse::radiationCoupledBase::rmap
(
  const fvPatchScalarField& ptf,
  const labelList& addr
)
{
  const radiationCoupledBase& mrptf =
    refCast<const radiationCoupledBase>(ptf);
  emissivity_.rmap(mrptf.emissivity_, addr);
}


void mousse::radiationCoupledBase::write(Ostream& os) const
{
  os.writeKeyword("emissivityMode") << emissivityMethodTypeNames_[method_]
    << token::END_STATEMENT << nl;
  emissivity_.writeEntry("emissivity", os);
}

