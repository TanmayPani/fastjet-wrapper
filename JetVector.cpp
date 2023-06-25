#define JetVector_cxx

#include "JetVector.h"

using namespace fastjet;
using namespace std;

JetVector::JetVector(){
    setJetConeDefinition();
}

JetVector::JetVector(double px, double py, double pz, double E, float rad) : 
PseudoJet(px, py, pz, E) {
    _radius = rad;
    setJetConeDefinition();
}

JetVector::JetVector(const PseudoJet& p, float rad) : PseudoJet(p) {
    _radius = rad;
    setJetConeDefinition();
    if(p.has_constituents()){
        setConstituents(p.constituents());
    }
}

JetVector::JetVector(const JetVector& j) : PseudoJet(j) {
    _radius = j._radius;
    _chargedConstituents = j._chargedConstituents;
    _neutralConstituents = j._neutralConstituents;
    _chargedConstituentsInCone = j._chargedConstituentsInCone;
    _neutralConstituentsInCone = j._neutralConstituentsInCone;
    jetCone = j.jetCone;
    chargedCone = j.chargedCone;
    neutralCone = j.neutralCone;
}

JetVector::~JetVector(){

}

void JetVector::setJetConeDefinition(){
    jetCone = SelectorEtaPhiCircle(_radius);
    jetCone.set_reference(*this);
    chargedCone = SelectorUserIndexNonNegative(true)  && jetCone;
    neutralCone = SelectorUserIndexNonNegative(false) && jetCone;
}

void JetVector::setRadius(float rad){
    _radius = rad;
    setJetConeDefinition();
}

void JetVector::setConstituents(const vector<PseudoJet>& constituents){
    _chargedConstituents = sorted_by_pt(chargedCone(constituents));
    _neutralConstituents = sorted_by_pt(neutralCone(constituents));
}

void JetVector::setConstituentsInCone(const vector<PseudoJet>& constituents){
    _chargedConstituentsInCone = sorted_by_pt(chargedCone(constituents));
    _neutralConstituentsInCone = sorted_by_pt(neutralCone(constituents));
}

void JetVector::setVector(const PseudoJet& p){
    this->reset(p);
    if(p.has_constituents()){
        setConstituents(p.constituents());
    }
    setJetConeDefinition();
}

void JetVector::setVector(const JetVector& j){
    this->reset(j);
    _chargedConstituents = j._chargedConstituents;
    _neutralConstituents = j._neutralConstituents;
    _chargedConstituentsInCone = j._chargedConstituentsInCone;
    _neutralConstituentsInCone = j._neutralConstituentsInCone;
    jetCone = j.jetCone;
    chargedCone = j.chargedCone;
    neutralCone = j.neutralCone;
}

void JetVector::setVector(double px, double py, double pz, double e){
    this->reset(px, py, pz, e);
    setJetConeDefinition();
}

vector<PseudoJet> JetVector::getConstituents(){
    if(this->has_constituents()){
        return sorted_by_pt(this->constituents());
    }
    vector<PseudoJet> constituents(_chargedConstituents);
    constituents.insert(constituents.end(), _neutralConstituents.begin(), _neutralConstituents.end());
    return sorted_by_pt(constituents);
}

vector<PseudoJet> JetVector::getConstituentsInCone(){
    vector<PseudoJet> constituents(_chargedConstituentsInCone);
    constituents.insert(constituents.end(), _neutralConstituentsInCone.begin(), _neutralConstituentsInCone.end());
    return sorted_by_pt(constituents);
}

float JetVector::getNeutralPtFraction(){
    float pt = this->pt();
    float neutralpt = 0.0;
    for(PseudoJet& p : _neutralConstituents){
        neutralpt += p.pt();
    }
    return neutralpt/pt;
}

float JetVector::getDeltaR(float eta, float phi){
    float deta = eta - this->eta();
    float dphi = abs(phi - this->phi());

    if(dphi > pi)dphi = twopi - dphi;

    return sqrt(deta*deta + dphi*dphi);
}

float JetVector::getDeltaR(const PseudoJet& p){
    return getDeltaR(p.eta(), p.phi());
}

vector<float> JetVector::getMomentumProfile(unsigned int nbins, float maxradius, float minpt){
    vector<float> profile(nbins, 0.0);
    float dr = maxradius/nbins;
    float r = dr/2.0;

    Selector minptcut = SelectorPtMin(minpt);
    vector<PseudoJet> constituents = minptcut(getConstituentsInCone());

    for(unsigned int i = 0; i < nbins; i++){
        float rin = r - dr/2.0;
        float rout = r + dr/2.0;
        Selector annulus = SelectorEtaPhiAnnulus(rin, rout);
        annulus.set_reference(*this); 
        vector<PseudoJet> _annulus = annulus(constituents);
        for(PseudoJet& p : _annulus){
            profile[i] += p.pt();
        }
        r += dr;
    }
    return profile;
}

vector<float> JetVector::getChargedMomentumProfile(unsigned int nbins, float maxradius, float minpt){
    vector<float> profile(nbins, 0.0);
    float dr = maxradius/nbins;
    float r = dr/2.0;

    Selector minptcut = SelectorPtMin(minpt);
    vector<PseudoJet> constituents = minptcut(_chargedConstituentsInCone);

    for(unsigned int i = 0; i < nbins; i++){
        float rin = r - dr/2.0;
        float rout = r + dr/2.0;
        Selector annulus = SelectorEtaPhiAnnulus(rin, rout);
        annulus.set_reference(*this); 
        vector<PseudoJet> _annulus = annulus(constituents);
        for(PseudoJet& p : _annulus){
            profile[i] += p.pt();
        }
        r += dr;
    }
    return profile;
}

float JetVector::getAngularity(float kappa, float beta, bool doFullNorm){
    float sum = 0.0;
    float norm = 0.0;
    for(PseudoJet& p : getConstituents()){
        sum += pow(p.pt(), kappa)*pow(getDeltaR(p), beta);
        norm += p.pt();
    }
    if(doFullNorm){return sum/pow(norm, kappa);}
    return sum/pow(pt(), kappa);
}

float JetVector::getChargedAngularity(float kappa, float beta, bool doFullNorm){
    float sum = 0.0;
    float norm = 0.0;
    for(PseudoJet& p : _chargedConstituents){
        sum += pow(p.pt(), kappa)*pow(getDeltaR(p), beta);
        norm += p.pt();
    }
    if(doFullNorm){return sum/pow(norm, kappa);}
    return sum/pow(pt(), kappa);
}

float JetVector::getAngularityInCone(float kappa, float beta, bool doFullNorm){
    float sum = 0.0;
    float norm = 0.0;
    for(PseudoJet& p : getConstituentsInCone()){
        sum += pow(p.pt(), kappa)*pow(getDeltaR(p), beta);
        norm += p.pt();
    }
    if(doFullNorm){return sum/pow(norm, kappa);}
    return sum/pow(pt(), kappa);
}

void JetVector::print(ostream& out) const {
    out<<" pt: "<<pt()<<" eta: "<<eta()<<" phi: "<<phi()<<endl;
    out<<" #charged constituents: "<<endl;
    unsigned int icst = 0;
    for(auto& cst : _chargedConstituents)
        out<<" . "<<++icst<<"("<<cst.user_index()<<"): pt: "<<cst.pt()<<" eta: "<<cst.eta()<<" phi: "<<cst.phi()<<endl;
    out<<" #neutral constituents: "<<endl;
    icst = 0;
    for(auto& cst : _neutralConstituents)
        out<<" . "<<++icst<<"("<<cst.user_index()<<"): pt: "<<cst.pt()<<" eta: "<<cst.eta()<<" phi: "<<cst.phi()<<endl;
    out<<" #charged constituents in cone: "<<endl;
    icst = 0;
    for(auto& cst : _chargedConstituentsInCone)
        out<<" . "<<++icst<<"("<<cst.user_index()<<"): pt: "<<cst.pt()<<" eta: "<<cst.eta()<<" phi: "<<cst.phi()<<endl; 
    out<<" #neutral constituents in cone: "<<endl;
    icst = 0;
    for(auto& cst : _neutralConstituentsInCone)
        out<<" . "<<++icst<<"("<<cst.user_index()<<"): pt: "<<cst.pt()<<" eta: "<<cst.eta()<<" phi: "<<cst.phi()<<endl;
}




