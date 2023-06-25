#ifndef JetVector_h
#define JetVector_h

#include "fastjet/PseudoJet.hh"
#include "FastJetSelectors.h"
#include <vector>

class JetVector : public fastjet::PseudoJet {
public:
    JetVector();
    JetVector(double px, double py, double pz, double e, float rad);
    JetVector(const fastjet::PseudoJet& pj, float rad);
    JetVector(const JetVector& v);

    template <class T> JetVector(const T & v, float rad) 
    : PseudoJet(v), _radius(rad) {setJetConeDefinition();}

    virtual ~JetVector();

    void setJetConeDefinition();
    void setRadius(float rad);
    void setConstituents(const std::vector<fastjet::PseudoJet>& pjv);
    void setConstituentsInCone(const std::vector<fastjet::PseudoJet>& pjv);

    void setVector(const fastjet::PseudoJet& pj);
    void setVector(double px, double py, double pz, double e);
    void setVector(const JetVector& v);
    template <class T> inline void setVector(const T& v){
        reset(v);
        setJetConeDefinition();
    }

    std::vector<fastjet::PseudoJet> getConstituents();
    std::vector<fastjet::PseudoJet> getChargedConstituents(){return _chargedConstituents;}
    std::vector<fastjet::PseudoJet> getNeutralConstituents(){return _neutralConstituents;}

    unsigned int nChargedConstituents(){return _chargedConstituents.size();}
    unsigned int nNeutralConstituents(){return _neutralConstituents.size();}
    unsigned int nConstituents(){return _chargedConstituents.size() + _neutralConstituents.size();}

    fastjet::PseudoJet getChargedConstituent(unsigned int i){
        if(i >= _chargedConstituents.size())
            throw fastjet::Error("JetVector::getChargedConstituent, Index out of range!");
        return _chargedConstituents[i];
    }

    fastjet::PseudoJet getNeutralConstituent(unsigned int i){
        if(i >= _neutralConstituents.size())
            throw fastjet::Error("JetVector::getNeutralConstituent, Index out of range!");
        return _neutralConstituents[i];
    }

    std::vector<fastjet::PseudoJet> getConstituentsInCone();
    std::vector<fastjet::PseudoJet> getChargedConstituentsInCone(){return _chargedConstituentsInCone;}
    std::vector<fastjet::PseudoJet> getNeutralConstituentsInCone(){return _neutralConstituentsInCone;}

    float getNeutralPtFraction();

    float getDeltaR(float eta, float phi);
    float getDeltaR(const fastjet::PseudoJet& v);
    template<class T> inline float getDeltaR(const T& v){
        PseudoJet p(v);
        return getDeltaR(p);
    }

    float getRadius() const {return _radius;}
    std::vector<float> getMomentumProfile(unsigned int nBins, float maxRadius, float minPt = 0.0);
    std::vector<float> getChargedMomentumProfile(unsigned int nBins, float maxRadius, float minPt = 0.0);

    float getAngularity(float kappa, float beta, bool doFullNorm = false);
    float getChargedAngularity(float kappa, float beta, bool doFullNorm = false);
    float getAngularityInCone(float kappa, float beta, bool doFullNorm = false);

    void print(std::ostream& out = std::cout) const;

private:
    float _radius = 0.4;
    std::vector<fastjet::PseudoJet> _chargedConstituents;
    std::vector<fastjet::PseudoJet> _neutralConstituents;
    std::vector<fastjet::PseudoJet> _chargedConstituentsInCone;
    std::vector<fastjet::PseudoJet> _neutralConstituentsInCone;
    fastjet::Selector jetCone;
    fastjet::Selector chargedCone;
    fastjet::Selector neutralCone;
  
};

#endif