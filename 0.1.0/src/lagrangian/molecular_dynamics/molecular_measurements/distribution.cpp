// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "distribution.hpp"
#include "ofstream.hpp"
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(distribution, 0);
}
// Static Member Functions
void mousse::distribution::write
(
  const fileName& file,
  const List<Pair<scalar> >& pairs
)
{
  OFstream os(file);
  FOR_ALL(pairs, i)
  {
    os  << pairs[i].first() << ' ' << pairs[i].second() << nl;
  }
}
// Constructors 
mousse::distribution::distribution()
:
  Map<label>(),
  binWidth_(1)
{}
mousse::distribution::distribution(const scalar binWidth)
:
  Map<label>(),
  binWidth_(binWidth)
{}
mousse::distribution::distribution(const distribution& d)
:
  Map<label>(static_cast< Map<label> >(d)),
  binWidth_(d.binWidth())
{}
// Destructor 
mousse::distribution::~distribution()
{}
// Member Functions 
mousse::label mousse::distribution::totalEntries() const
{
  label sumOfEntries = 0;
  FOR_ALL_CONST_ITER(Map<label>, *this, iter)
  {
    sumOfEntries += iter();
    if (sumOfEntries < 0)
    {
      WARNING_IN("label distribution::totalEntries()")
        << "Accumulated distribution values total has become negative: "
        << "sumOfEntries = " << sumOfEntries
        << ". This is most likely to be because too many samples "
        << "have been added to the bins and the label has 'rolled "
        << "round'. Try distribution::approxTotalEntries which "
        << "returns a scalar." << endl;
      sumOfEntries = -1;
      break;
    }
  }
  return sumOfEntries;
}
mousse::scalar mousse::distribution::approxTotalEntries() const
{
  scalar sumOfEntries = 0;
  FOR_ALL_CONST_ITER(Map<label>, *this, iter)
  {
    sumOfEntries += scalar(iter());
  }
  return sumOfEntries;
}
mousse::scalar mousse::distribution::mean() const
{
  scalar runningSum = 0;
  scalar totEnt = approxTotalEntries();
  List<label> keys = toc();
  FOR_ALL(keys,k)
  {
    label key = keys[k];
    runningSum +=
      (0.5 + scalar(key))
     *binWidth_
     *scalar((*this)[key])
     /totEnt;
  }
  return runningSum;
}
mousse::scalar mousse::distribution::median()
{
  // From:
  // http://mathworld.wolfram.com/StatisticalMedian.html
  // The statistical median is the value of the distribution variable
  // where the cumulative distribution = 0.5.
  scalar median = 0.0;
  scalar runningSum = 0.0;
  List<Pair<scalar> > normDist(normalised());
  if (normDist.size())
  {
    if (normDist.size() == 1)
    {
      median = normDist[0].first();
    }
    else if
    (
      normDist.size() > 1
    && normDist[0].second()*binWidth_ > 0.5
    )
    {
      scalar xk = normDist[1].first();
      scalar xkm1 = normDist[0].first();
      scalar Sk =
        (normDist[0].second() + normDist[1].second())*binWidth_;
      scalar Skm1 = normDist[0].second()*binWidth_;
      median = (0.5 - Skm1)*(xk - xkm1)/(Sk - Skm1) + xkm1;
    }
    else
    {
      label lastNonZeroIndex = 0;
      FOR_ALL(normDist,nD)
      {
        if (runningSum + (normDist[nD].second()*binWidth_) > 0.5)
        {
          scalar xk = normDist[nD].first();
          scalar xkm1 = normDist[lastNonZeroIndex].first();
          scalar Sk = runningSum + (normDist[nD].second()*binWidth_);
          scalar Skm1 = runningSum;
          median = (0.5 - Skm1)*(xk - xkm1)/(Sk - Skm1) + xkm1;
          break;
        }
        else if (normDist[nD].second() > 0.0)
        {
          runningSum += normDist[nD].second()*binWidth_;
          lastNonZeroIndex = nD;
        }
      }
    }
  }
  return median;
}
void mousse::distribution::add(const scalar valueToAdd)
{
  iterator iter(this->begin());
  label n = label(valueToAdd/binWidth_) - label(neg(valueToAdd/binWidth_));
  iter = find(n);
  if (iter == this->end())
  {
    this->insert(n,1);
  }
  else
  {
    (*this)[n]++;
  }
  if ((*this)[n] < 0)
  {
    FATAL_ERROR_IN("distribution::add(const scalar valueToAdd)")
      << "Accumulated distribution value has become negative: "
      << "bin = " << (0.5 + scalar(n)) * binWidth_
      << ", value = " << (*this)[n]
      << ". This is most likely to be because too many samples "
      << "have been added to a bin and the label has 'rolled round'"
      << abort(FatalError);
  }
}
void mousse::distribution::add(const label valueToAdd)
{
  add(scalar(valueToAdd));
}
void mousse::distribution::insertMissingKeys()
{
  iterator iter(this->begin());
  List<label> keys = toc();
  sort(keys);
  if (keys.size())
  {
    for (label k = keys[0]; k < keys.last(); k++)
    {
      iter = find(k);
      if (iter == this->end())
      {
        this->insert(k,0);
      }
    }
  }
}
mousse::List<mousse::Pair<mousse::scalar> > mousse::distribution::normalised()
{
  scalar totEnt = approxTotalEntries();
  insertMissingKeys();
  List<label> keys = toc();
  sort(keys);
  List<Pair<scalar> > normDist(size());
  FOR_ALL(keys,k)
  {
    label key = keys[k];
    normDist[k].first() = (0.5 + scalar(key))*binWidth_;
    normDist[k].second() = scalar((*this)[key])/totEnt/binWidth_;
  }
  if (debug)
  {
    Info<< "totEnt: " << totEnt << endl;
  }
  return normDist;
}
mousse::List<mousse::Pair<mousse::scalar> > mousse::distribution::normalisedMinusMean()
{
  return normalisedShifted(mean());
}
mousse::List<mousse::Pair<mousse::scalar> > mousse::distribution::normalisedShifted
(
  scalar shiftValue
)
{
  List<Pair<scalar> > oldDist(normalised());
  List<Pair<scalar> > newDist(oldDist.size());
  FOR_ALL(oldDist,u)
  {
    oldDist[u].first() -= shiftValue;
  }
  scalar lowestOldBin = oldDist[0].first()/binWidth_ - 0.5;
  label lowestNewKey = label
  (
    lowestOldBin + 0.5*sign(lowestOldBin)
  );
  scalar interpolationStartDirection =
    sign(scalar(lowestNewKey) - lowestOldBin);
  label newKey = lowestNewKey;
  if (debug)
  {
    Info<< shiftValue
      << nl << lowestOldBin
      << nl << lowestNewKey
      << nl << interpolationStartDirection
      << endl;
    scalar checkNormalisation = 0;
    FOR_ALL(oldDist, oD)
    {
      checkNormalisation += oldDist[oD].second()*binWidth_;
    }
    Info<< "Initial normalisation = " << checkNormalisation << endl;
  }
  FOR_ALL(oldDist,u)
  {
    newDist[u].first() = (0.5 + scalar(newKey)) * binWidth_;
    if (interpolationStartDirection < 0)
    {
      if (u == 0)
      {
        newDist[u].second() =
          (0.5 + scalar(newKey))*oldDist[u].second()
         - oldDist[u].second()
            *(oldDist[u].first() - binWidth_)/ binWidth_;
      }
      else
      {
        newDist[u].second() =
          (0.5 + scalar(newKey))
         *(oldDist[u].second() - oldDist[u-1].second())
         +
          (
            oldDist[u-1].second()*oldDist[u].first()
           - oldDist[u].second()*oldDist[u-1].first()
          )
          /binWidth_;
      }
    }
    else
    {
      if (u == oldDist.size() - 1)
      {
        newDist[u].second() =
          (0.5 + scalar(newKey))*-oldDist[u].second()
         + oldDist[u].second()*(oldDist[u].first() + binWidth_)
         /binWidth_;
      }
      else
      {
        newDist[u].second() =
          (0.5 + scalar(newKey))
         *(oldDist[u+1].second() - oldDist[u].second())
         +
          (
            oldDist[u].second()*oldDist[u+1].first()
           - oldDist[u+1].second()*oldDist[u].first()
          )
         /binWidth_;
      }
    }
    newKey++;
  }
  if (debug)
  {
    scalar checkNormalisation = 0;
    FOR_ALL(newDist, nD)
    {
      checkNormalisation += newDist[nD].second()*binWidth_;
    }
    Info<< "Shifted normalisation = " << checkNormalisation << endl;
  }
  return newDist;
}
mousse::List<mousse::Pair<mousse::scalar> > mousse::distribution::raw()
{
  insertMissingKeys();
  List<label> keys = toc();
  sort(keys);
  List<Pair<scalar> > rawDist(size());
  FOR_ALL(keys,k)
  {
    label key = keys[k];
    rawDist[k].first() = (0.5 + scalar(key))*binWidth_;
    rawDist[k].second() = scalar((*this)[key]);
  }
  return rawDist;
}
// Member Operators 
void mousse::distribution::operator=(const distribution& rhs)
{
  // Check for assignment to self
  if (this == &rhs)
  {
    FATAL_ERROR_IN("distribution::operator=(const distribution&)")
      << "Attempted assignment to self"
      << abort(FatalError);
  }
  Map<label>::operator=(rhs);
  binWidth_ = rhs.binWidth();
}
// Friend Operators 
mousse::Ostream& mousse::operator<<(Ostream& os, const distribution& d)
{
  os  << d.binWidth_
    << static_cast<const Map<label>&>(d);
  // Check state of Ostream
  os.check
  (
    "Ostream& operator<<(Ostream&, "
    "const distribution&)"
  );
  return os;
}
