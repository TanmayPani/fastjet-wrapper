#ifndef FastJetUtils_h
#define FastJetUtils_h

#include "JetVector.h"
#include "fastjet/FunctionOfPseudoJet.hh"

FASTJET_BEGIN_NAMESPACE

class JetVectorMaker : public FunctionOfPseudoJet<JetVector>{
public:
    JetVectorMaker(float r){radius = r;}
    JetVectorMaker(float r, std::vector<PseudoJet> fe){radius = r; fullEvent = fe;}
    virtual ~JetVectorMaker(){}

    void setFullEvent(std::vector<PseudoJet> fe){fullEvent = fe;}

    virtual JetVector result(const PseudoJet& jet) const{
        JetVector jv(jet, radius);
        if(!fullEvent.empty())jv.setConstituentsInCone(fullEvent);
        return jv;
    }
private:
    float radius = 0.4;
    std::vector<PseudoJet> fullEvent;
};

FASTJET_END_NAMESPACE

#endif