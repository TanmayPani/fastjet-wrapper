#define JetMaker_cxx

#include "JetMaker.h"

using namespace fastjet;
using namespace std;

JetMaker::JetMaker(bool debug, bool dofulldebug) : FastJetWrapperBase(dofulldebug){
    doDebug = debug;
    if(doDebug)cout<<"************* JetMaker::JetMaker() *****************"<<endl;
}

JetMaker::~JetMaker(){
     
}

void JetMaker::clear(){
    if(doDebug)cout<<"*********** JetMaker::clear() ***********"<<endl;
    FastJetWrapperBase::clear();
    if(!chargedEvent.empty())chargedEvent.clear();
    if(!neutralEvent.empty())neutralEvent.clear();
    if(!fullJets.empty())fullJets.clear();
    if(!chargedJets.empty())chargedJets.clear();
    //if(jetVectorMaker)jetVectorMaker.reset();
    if(CS_Area)CS_Area.reset();  
    if(CS)CS.reset();
}

void JetMaker::inputForClustering(int i, fastjet::PseudoJet& cst){
    FastJetWrapperBase::inputForClustering(i, cst);
    if(i > -1){
        if(doDebug)
            cout<<"Adding charged: "<<i<<" pt: "<<cst.pt()<<" eta: "<<cst.eta()<<" phi: "<<cst.phi()<<endl;
        chargedEvent.push_back(cst);
    }
    else if(i < -1){
        if(doDebug)
            cout<<"Adding neutral: "<<i<<" pt: "<<cst.pt()<<" eta: "<<cst.eta()<<" phi: "<<cst.phi()<<endl;
        neutralEvent.push_back(cst);
    }else{
        if(doDebug)
            cout<<"Unknown: "<<i<<" pt: "<<cst.pt()<<" eta: "<<cst.eta()<<" phi: "<<cst.phi()<<endl;
    }
}

void JetMaker::inputForClustering(int i, float px, float py, float pz, float E){
    if(doDebug)
        cout<<"Provided with: "<<i<<" px: "<<px<<" py: "<<py<<" pz: "<<pz<<" E: "<<E<<endl;
    PseudoJet constit(px, py, pz, E);
    inputForClustering(i, constit);
}


void JetMaker::getJets(vector<PseudoJet>& fullEvt, vector<JetVector>& Jets){
    jetVectorMaker.reset(new JetVectorMaker(R(), fullEvt));
    vector<PseudoJet> constituents;
    if(constituentSelection) constituents = (*constituentSelection)(fullEvt);
    else constituents = fullEvt;

    if(doDebug){
        cout<<"*****JetMaker::getJets()*****"<<endl;
        cout<<"Number of constituents: "<<constituents.size()<<"out of entries: "<<fullEvt.size()<<endl;   
        printPseudoJets(constituents);
    }

    vector<PseudoJet> jets;
    if(!doArea){
        CS.reset(new ClusterSequence(constituents, *jetDef));
        if(doDebug)
            cout<<"Ran ClusterSequence... "<<CS->n_particles()<<" were provided "<<CS->unclustered_particles().size()<<" remain unclustered"<<endl;
        jets = CS->inclusive_jets(JetPtMin);
    }else{
        CS_Area.reset(new ClusterSequenceArea(constituents, *jetDef, *areaDef));
        if(doDebug)
            cout<<"Ran ClusterSequenceArea... "<<CS_Area->n_particles()<<" were provided "<<CS_Area->unclustered_particles().size()<<" remain unclustered"<<endl;
        jets = CS_Area->inclusive_jets(JetPtMin);
    }

    if(doDebug){
        cout<<"Got inclusive_jets... with minimum pt "<<JetPtMin<<endl;
        printPseudoJets(jets); 
    }

    if(jetSelection)Jets = (*jetVectorMaker)(sorted_by_pt((*jetSelection)(jets)));
    else Jets = (*jetVectorMaker)(sorted_by_pt(jets));

    if(doDebug){
        cout<<"Converted vector<fastjet::PseudoJet> into sorted vector<JetVector:PseudoJet> ... "<<endl;
        printJetVectors(Jets); 
    }

}

vector<JetVector> JetMaker::getFullJets(){
    if(doDebug)cout<<"*****JetMaker::getFullJets()*****"<<endl;
    getJets(fullEvent, fullJets);
    return fullJets;
}

vector<JetVector> JetMaker::getChargedJets(){
    if(doDebug)cout<<"*****JetMaker::getChargedJets()*****"<<endl;
    getJets(chargedEvent, chargedJets);
    return chargedJets;
}
