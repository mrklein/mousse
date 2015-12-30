// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
template<class SourcePatch, class TargetPatch>
mousse::autoPtr<mousse::AMIMethod<SourcePatch, TargetPatch> >
mousse::AMIMethod<SourcePatch, TargetPatch>::New
(
  const word& methodName,
  const SourcePatch& srcPatch,
  const TargetPatch& tgtPatch,
  const scalarField& srcMagSf,
  const scalarField& tgtMagSf,
  const faceAreaIntersect::triangulationMode& triMode,
  const bool reverseTarget,
  const bool requireMatch
)
{
  if (debug)
  {
    Info<< "Selecting AMIMethod " << methodName << endl;
  }
  typename componentsConstructorTable::iterator cstrIter =
    componentsConstructorTablePtr_->find(methodName);
  if (cstrIter == componentsConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "AMIMethod<SourcePatch, TargetPatch>::New"
      "("
        "const word&, "
        "const SourcePatch&, "
        "const TargetPatch&, "
        "const scalarField&, "
        "const scalarField&, "
        "const faceAreaIntersect::triangulationMode&, "
        "const bool, "
        "const bool"
      ")"
    )   << "Unknown AMIMethod type "
      << methodName << nl << nl
      << "Valid AMIMethod types are:" << nl
      << componentsConstructorTablePtr_->sortedToc() << exit(FatalError);
  }
  return autoPtr<AMIMethod<SourcePatch, TargetPatch> >
  (
    cstrIter()
    (
      srcPatch,
      tgtPatch,
      srcMagSf,
      tgtMagSf,
      triMode,
      reverseTarget,
      requireMatch
    )
  );
}
