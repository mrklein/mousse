// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::diameterModels::IATEsource
// Description
//   IATE (Interfacial Area Transport Equation) bubble diameter model
//   run-time selectable sources.
// SourceFiles
//   iat_esource.cpp
#ifndef iat_esource_hpp_
#define iat_esource_hpp_
#include "iate.hpp"
#include "mathematical_constants.hpp"
namespace mousse
{
namespace diameterModels
{
class IATEsource
{
protected:
  // Protected data
    //- Reference to the IATE this source applies to
    const IATE& iate_;
public:
  //- Runtime type information
  TYPE_NAME("IATEsource");
  // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      IATEsource,
      dictionary,
      (
        const IATE& iate,
        const dictionary& dict
      ),
      (iate, dict)
    );
  //- Class used for the read-construction of
  //  PtrLists of IATE sources
  class iNew
  {
    const IATE& iate_;
  public:
    iNew(const IATE& iate)
    :
      iate_{iate}
    {}
    autoPtr<IATEsource> operator()(Istream& is) const
    {
      word type{is};
      dictionary dict{is};
      return IATEsource::New(type, iate_, dict);
    }
  };
  // Constructors
    IATEsource(const IATE& iate)
    :
      iate_{iate}
    {}
    autoPtr<IATEsource> clone() const
    {
      NOT_IMPLEMENTED("autoPtr<IATEsource> clone() const");
      return autoPtr<IATEsource>{NULL};
    }
  // Selectors
    static autoPtr<IATEsource> New
    (
      const word& type,
      const IATE& iate,
      const dictionary& dict
    );
  //- Destructor
  virtual ~IATEsource()
  {}
  // Member Functions
    const phaseModel& phase() const
    {
      return iate_.phase();
    }
    const twoPhaseSystem& fluid() const
    {
      return iate_.phase().fluid();
    }
    const phaseModel& otherPhase() const
    {
      return phase().otherPhase();
    }
    scalar phi() const
    {
      return 1.0/(36*constant::mathematical::pi);
    }
    //- Return the bubble relative velocity
    tmp<volScalarField> Ur() const;
    //- Return the bubble turbulent velocity
    tmp<volScalarField> Ut() const;
    //- Return the bubble Reynolds number
    tmp<volScalarField> Re() const;
    //- Return the bubble drag coefficient
    tmp<volScalarField> CD() const;
    //- Return the bubble Morton number
    tmp<volScalarField> Mo() const;
    //- Return the bubble Eotvos number
    tmp<volScalarField> Eo() const;
    //- Return the bubble Webber number
    tmp<volScalarField> We() const;
    virtual tmp<volScalarField> R() const = 0;
};
}  // namespace diameterModels
}  // namespace mousse
#endif
