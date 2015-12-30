// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "turbulence_fields.hpp"
#include "dictionary.hpp"
#include "turbulent_transport_model.hpp"
#include "turbulent_fluid_thermo_model.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(turbulenceFields, 0);
  template<>
  const char* NamedEnum<turbulenceFields::compressibleField, 8>::names[] =
  {
    "k",
    "epsilon",
    "mut",
    "muEff",
    "alphat",
    "alphaEff",
    "R",
    "devRhoReff"
  };
  const NamedEnum<turbulenceFields::compressibleField, 8>
    turbulenceFields::compressibleFieldNames_;
  template<>
  const char* NamedEnum<turbulenceFields::incompressibleField, 6>::names[] =
  {
    "k",
    "epsilon",
    "nut",
    "nuEff",
    "R",
    "devReff"
  };
  const NamedEnum<turbulenceFields::incompressibleField, 6>
    turbulenceFields::incompressibleFieldNames_;
  const word turbulenceFields::modelName = turbulenceModel::propertiesName;
}
// Protected Member Functions 
bool mousse::turbulenceFields::compressible()
{
  if (obr_.foundObject<compressible::turbulenceModel>(modelName))
  {
    return true;
  }
  else if (obr_.foundObject<incompressible::turbulenceModel>(modelName))
  {
    return false;
  }
  else
  {
    WarningIn("mousse::word& mousse::turbulenceFields::compressible() const")
      << "Turbulence model not found in database, deactivating";
    active_ = false;
  }
  return false;
}
// Constructors 
mousse::turbulenceFields::turbulenceFields
(
  const word& name,
  const objectRegistry& obr,
  const dictionary& dict,
  const bool loadFromFiles
)
:
  name_(name),
  obr_(obr),
  active_(true),
  fieldSet_()
{
  // Check if the available mesh is an fvMesh otherise deactivate
  if (isA<fvMesh>(obr_))
  {
    read(dict);
  }
  else
  {
    active_ = false;
    WarningIn
    (
      "turbulenceFields::turbulenceFields"
      "("
        "const word&, "
        "const objectRegistry&, "
        "const dictionary&, "
        "const bool"
      ")"
    )   << "No fvMesh available, deactivating " << name_
      << endl;
  }
}
// Destructor 
mousse::turbulenceFields::~turbulenceFields()
{}
// Member Functions 
void mousse::turbulenceFields::read(const dictionary& dict)
{
  if (active_)
  {
    fieldSet_.insert(wordList(dict.lookup("fields")));
    Info<< type() << " " << name_ << ": ";
    if (fieldSet_.size())
    {
      Info<< "storing fields:" << nl;
      forAllConstIter(wordHashSet, fieldSet_, iter)
      {
        Info<< "    " << modelName << ':' << iter.key() << nl;
      }
      Info<< endl;
    }
    else
    {
      Info<< "no fields requested to be stored" << nl << endl;
    }
  }
}
void mousse::turbulenceFields::execute()
{
  bool comp = compressible();
  if (!active_)
  {
    return;
  }
  if (comp)
  {
    const compressible::turbulenceModel& model =
      obr_.lookupObject<compressible::turbulenceModel>(modelName);
    forAllConstIter(wordHashSet, fieldSet_, iter)
    {
      const word& f = iter.key();
      switch (compressibleFieldNames_[f])
      {
        case cfK:
        {
          processField<scalar>(f, model.k());
          break;
        }
        case cfEpsilon:
        {
          processField<scalar>(f, model.epsilon());
          break;
        }
        case cfMut:
        {
          processField<scalar>(f, model.mut());
          break;
        }
        case cfMuEff:
        {
          processField<scalar>(f, model.muEff());
          break;
        }
        case cfAlphat:
        {
          processField<scalar>(f, model.alphat());
          break;
        }
        case cfAlphaEff:
        {
          processField<scalar>(f, model.alphaEff());
          break;
        }
        case cfR:
        {
          processField<symmTensor>(f, model.R());
          break;
        }
        case cfDevRhoReff:
        {
          processField<symmTensor>(f, model.devRhoReff());
          break;
        }
        default:
        {
          FatalErrorIn("void mousse::turbulenceFields::execute()")
            << "Invalid field selection" << abort(FatalError);
        }
      }
    }
  }
  else
  {
    const incompressible::turbulenceModel& model =
      obr_.lookupObject<incompressible::turbulenceModel>(modelName);
    forAllConstIter(wordHashSet, fieldSet_, iter)
    {
      const word& f = iter.key();
      switch (incompressibleFieldNames_[f])
      {
        case ifK:
        {
          processField<scalar>(f, model.k());
          break;
        }
        case ifEpsilon:
        {
          processField<scalar>(f, model.epsilon());
          break;
        }
        case ifNut:
        {
          processField<scalar>(f, model.nut());
          break;
        }
        case ifNuEff:
        {
          processField<scalar>(f, model.nuEff());
          break;
        }
        case ifR:
        {
          processField<symmTensor>(f, model.R());
          break;
        }
        case ifDevReff:
        {
          processField<symmTensor>(f, model.devReff());
          break;
        }
        default:
        {
          FatalErrorIn("void mousse::turbulenceFields::execute()")
            << "Invalid field selection" << abort(FatalError);
        }
      }
    }
  }
}
void mousse::turbulenceFields::end()
{
  if (active_)
  {
    execute();
  }
}
void mousse::turbulenceFields::timeSet()
{}
void mousse::turbulenceFields::write()
{}
