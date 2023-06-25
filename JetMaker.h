#ifndef JetMaker_h
#define JetMaker_h

#include "FastJetWrapperBase.h"
#include "fastjet/ClusterSequenceArea.hh"
//#include <iostream>

class JetMaker : public FastJetWrapperBase{
public:
    JetMaker(bool debug = false, bool fulldebug = false); 
    virtual ~JetMaker() override;

    virtual void clear() override;

    virtual void inputForClustering(int i, fastjet::PseudoJet& constit)     override;
    virtual void inputForClustering(int i, float px, float py, float pz, float E) override;
    template <class T> inline void inputForClustering2(int i, T& constit){
        fastjet::PseudoJet cons(constit);
        inputForClustering(i, cons);
    }

    void setJetPtMin(float pt){JetPtMin = pt;}

    void getJets(vector<fastjet::PseudoJet>& fevent, vector<JetVector>& jets);
    std::vector<JetVector> getFullJets(); //returns clustered vector<PseudoJet> of full jets by running ClusterSequence
    std::vector<JetVector> getChargedJets(); //returns clustered vector<PseudoJet> of charged jets by running ClusterSequence

    std::vector<JetVector> full_jets(){//returns clustered vector<PseudoJet> of full jets
        if(fullJets.empty())getJets(fullEvent, fullJets);
        return fullJets;
    } 
    std::vector<JetVector> charged_jets(){//returns clustered vector<PseudoJet> of charged jets
        if(chargedJets.empty())getJets(chargedEvent, chargedJets);
        return chargedJets;
    } 
      
private:
    bool doDebug = false;

    float JetPtMin = 8.0;

    std::vector<fastjet::PseudoJet> chargedEvent;
    std::vector<fastjet::PseudoJet> neutralEvent;

    std::unique_ptr<fastjet::ClusterSequenceArea> CS_Area = nullptr;
	std::unique_ptr<fastjet::ClusterSequence> CS = nullptr;

    std::vector<JetVector> fullJets;
    std::vector<JetVector> chargedJets;

};


#endif
