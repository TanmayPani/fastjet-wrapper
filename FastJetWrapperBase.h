#ifndef FastJetWrapperBase_h
#define FastJetWrapperBase_h

#include "fastjet/JetDefinition.hh"
#include "fastjet/AreaDefinition.hh"
#include "FastJetUtils.h"
#include "SelectorMaker.h"

class FastJetWrapperBase{
public:
    FastJetWrapperBase(bool debug = false);
    virtual ~FastJetWrapperBase();

    virtual void init();
    virtual void clear();

    fastjet::JetAlgorithm jetAlgorithm(){return jetAlgo;}
    fastjet::RecombinationScheme recombinationScheme(){return jetRecombScheme;}
    float R(){return jetReso;}
    fastjet::AreaType jetAreaType(){return areaType;}

    fastjet::JetDefinition* jetDefinition(){
        if(!jetDef)setJetDefinition();
        return jetDef.get();
    }
    fastjet::GhostedAreaSpec* ghostedAreaSpec(){
        if(!ghostSpec)setJetAreaDefinition();
        return ghostSpec.get();
    }
    fastjet::AreaDefinition* areaDefinition(){
        if(!areaDef)setJetAreaDefinition();
        return areaDef.get();
    }

    void setJetAlgorithm(fastjet::JetAlgorithm algo){jetAlgo = algo;}
    void setJetResolution(float R){jetReso = R;}
    void setJetRecombinationScheme(fastjet::RecombinationScheme scheme){jetRecombScheme = scheme;}
    void setJetDefinition();
    void setJetDefinition(fastjet::JetAlgorithm algo, float R, fastjet::RecombinationScheme reco);

    void setDoArea(bool doarea){doArea = doarea;}

    void setGhostedAreaSpec(float maxrap);
    void setGhostedAreaSpec(fastjet::Selector selector);
    void setAreaType(fastjet::AreaType type){areaType = type;}
    void setJetAreaDefinition();
    void setJetAreaDefinition(fastjet::AreaType areatype);

    virtual void printDescription();

    virtual void inputForClustering(int i, fastjet::PseudoJet& constit);
    virtual void inputForClustering(fastjet::PseudoJet& constit);
    virtual void inputForClustering(int i, float px, float py, float pz, float E);
    virtual void inputForClustering(float px, float py, float pz, float E);
    template <class T> inline void inputForClustering(int i, T & constit){
        fastjet::PseudoJet cons(constit);
        inputForClustering(i, cons);
    }

    void setFullEvent(const std::vector<fastjet::PseudoJet>& fEvent){fullEvent = fEvent;}

    std::vector<fastjet::PseudoJet> getFullEvent(){return fullEvent;}
    
    SelectorMaker* constituentCuts(){
        if(!constituentSelection){
            if(doFullDebug)std::cout << "Making constituentSelection" << std::endl;
            constituentSelection.reset(new SelectorMaker());
        }
        return constituentSelection.get();
    }
    SelectorMaker* jetCuts(){
        if(!jetSelection){
            if(doFullDebug)std::cout << "Making jetSelection" << std::endl;
            jetSelection.reset(new SelectorMaker());
        }
        return jetSelection.get();
    }

    void printPseudoJets(const std::vector<fastjet::PseudoJet>& jets, std::ostream& out = std::cout) const;
    void printJetVectors(const std::vector<JetVector>& jets, std::ostream& out = std::cout) const;

protected:
    fastjet::JetAlgorithm jetAlgo = fastjet::antikt_algorithm;
    float jetReso = 0.4;
    fastjet::RecombinationScheme jetRecombScheme = fastjet::E_scheme;
    std::unique_ptr<fastjet::JetDefinition> jetDef = nullptr;

    bool doArea = false;

    float ghostMaxRap = 1.2;
    fastjet::Selector ghostSelector;
    std::unique_ptr<fastjet::GhostedAreaSpec> ghostSpec = nullptr;

    fastjet::AreaType areaType = fastjet::active_area_explicit_ghosts;
    std::unique_ptr<fastjet::AreaDefinition> areaDef   = nullptr;

    std::unique_ptr<SelectorMaker> constituentSelection = nullptr;
    std::unique_ptr<SelectorMaker> jetSelection = nullptr; 

    std::vector<fastjet::PseudoJet> fullEvent;

    std::unique_ptr<fastjet::JetVectorMaker> jetVectorMaker = nullptr;
private:
    bool doFullDebug = false;
};

#endif