#ifndef SelectorMaker_h
#define SelectorMaker_h

#include "FastJetSelectors.h"
#include <string>
#include <map>
#include <vector>
#include <functional>

class SelectorMaker{
public:
    SelectorMaker();

    void setCut(const std::string& var, const std::string& type, float val);
    void setRange(const std::string& var, float min, float max);
    //void rejectNHardestJets(const unsigned int n){cutList.push_back(!fastjet::SelectorNHardest(n)  ) ;}
    //void rejectPureGhosts()                      {cutList.push_back(!fastjet::SelectorIsPureGhost()) ;}

    void makeSelector();

    std::vector<fastjet::PseudoJet> apply(const std::vector<fastjet::PseudoJet>& jets) const {return (cutList.back())(jets);}

    bool operator()(const fastjet::PseudoJet & jet) const {return (cutList.back())(jet);};
    std::vector<fastjet::PseudoJet> operator()(const std::vector<fastjet::PseudoJet> & jets) const {return apply(jets); };

    fastjet::Selector getGeometricCuts(){return geomCutList.back();}
    fastjet::Selector getSelector(){return cutList.back();}

    void rejectNHardestJets(const unsigned int n){
        fastjet::Selector newCut = !fastjet::SelectorNHardest(n);
        cutList.push_back(newCut);
    }
    void rejectPureGhosts(){
        fastjet::Selector newCut = !fastjet::SelectorIsPureGhost();
        cutList.push_back(newCut);
    }

    std::string description() {return cutList.back().description();}

private:
    std::vector<fastjet::Selector> cutList;
    std::vector<fastjet::Selector> geomCutList;

    unsigned int ncuts = 0;
    bool areSelectorsMade = false;

    static std::map<std::string, std::function<fastjet::Selector(float)> > cuts;
    static std::map<std::string, std::function<fastjet::Selector(float, float)> > rangeCuts;
};

#endif