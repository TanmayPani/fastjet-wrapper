#ifndef FastJetSelectors_h
#define FastJetSelectors_h

#include "fastjet/config.h"   // will allow a test for FJ3
#include "fastjet/Selector.hh"

using namespace std;

FASTJET_BEGIN_NAMESPACE

Selector SelectorUserIndexNonNegative(bool isTrue);

Selector SelectorEtaPhiCircle(const float & radius);

Selector SelectorEtaPhiAnnulus(const float & radius_in, const float & radius_out);

//Selector CombineSelectors(std::vector<Selector> cutList);

FASTJET_END_NAMESPACE

#endif