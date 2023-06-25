#define FastJetWrapperBase_cxx

#include "FastJetWrapperBase.h"

using namespace fastjet;
using namespace std;

FastJetWrapperBase::FastJetWrapperBase(bool debug){
    doFullDebug = debug;
    if(doFullDebug)cout<<"************** FastJetWrapperBase::FastJetWrapperBase() *****************"<<endl;

    jetVectorMaker.reset(new JetVectorMaker(R()));
}

FastJetWrapperBase::~FastJetWrapperBase(){

}

void FastJetWrapperBase::init(){
    if(doFullDebug)cout<<"************** FastJetWrapperBase::init() *****************"<<endl;
    if(!jetDef)setJetDefinition();

    if(constituentSelection)constituentSelection->makeSelector();
    if(jetSelection)jetSelection->makeSelector();

    if(doArea){
        if(!ghostSpec){
            if(constituentSelection)setGhostedAreaSpec(constituentSelection->getGeometricCuts());
            else setGhostedAreaSpec(ghostMaxRap);
        }
        if(ghostSpec && !areaDef)setJetAreaDefinition();
    }
    
}

void FastJetWrapperBase::clear(){
    if(doFullDebug)cout<<"*********** FastJetWrapperBase::clear() ***********"<<endl;

    if(!fullEvent.empty())fullEvent.clear();
    if(jetVectorMaker)jetVectorMaker.reset();
}

void FastJetWrapperBase::setJetDefinition(){
    if(doFullDebug)cout<<"************** FastJetWrapperBase::setJetDefinition() *****************"<<endl;
    jetDef.reset(new JetDefinition(jetAlgo, jetReso, jetRecombScheme, Best));
}

void FastJetWrapperBase::setJetDefinition(JetAlgorithm algo, float R, RecombinationScheme reco){
    setJetAlgorithm(algo);
    setJetResolution(R);
    setJetRecombinationScheme(reco);
    setJetDefinition();
}

void FastJetWrapperBase::setGhostedAreaSpec(float maxrap){
    if(doFullDebug)cout<<"************** FastJetWrapperBase::setGhostedAreaSpec("<<maxrap<<") *****************"<<endl;
    ghostMaxRap = maxrap;
    ghostSpec.reset(new GhostedAreaSpec(ghostMaxRap));
    doArea = true;
}

void FastJetWrapperBase::setGhostedAreaSpec(Selector sel){
    if(doFullDebug)cout<<"************** FastJetWrapperBase::setGhostedAreaSpec("<<sel.description()<<") *****************"<<endl;
    ghostSelector = sel;
    ghostSpec.reset(new GhostedAreaSpec(sel));
    doArea = true;
}

void FastJetWrapperBase::setJetAreaDefinition(){
    if(doFullDebug)cout<<"************** FastJetWrapperBase::setJetAreaDefinition() *****************"<<endl;
    if(!ghostSpec)
        throw Error("FastJetWrapperBase::setJetAreaDefinition() ghostSpec not set!");
    areaDef.reset(new fastjet::AreaDefinition(areaType, *ghostSpec));
}

void FastJetWrapperBase::setJetAreaDefinition(fastjet::AreaType areatype){
    setAreaType(areatype);
    setJetAreaDefinition();
}

void FastJetWrapperBase::printDescription(){
    if(jetDef)cout<<"Jet definition details: "<<jetDef->description()<<endl;
    if(areaDef)cout<<"Area definition details: "<<areaDef->description()<<endl;
    if(constituentSelection)cout<<"Constituent cuts: "<<constituentSelection->description()<<endl;
    if(jetSelection)cout<<"Jet cuts: "<<jetSelection->description()<<endl;
}

void FastJetWrapperBase::inputForClustering(fastjet::PseudoJet& constit){
    fullEvent.push_back(constit);
}

void FastJetWrapperBase::inputForClustering(int i, fastjet::PseudoJet& constit){
    constit.set_user_index(i);
    fullEvent.push_back(constit);
}

void FastJetWrapperBase::inputForClustering(float px, float py, float pz, float E){
    PseudoJet constit(px, py, pz, E);
    fullEvent.push_back(constit);
}

void FastJetWrapperBase::inputForClustering(int i, float px, float py, float pz, float E){
    PseudoJet constit(px, py, pz, E);
    inputForClustering(i, constit);
}

void FastJetWrapperBase::printPseudoJets(const vector<PseudoJet>& jets, ostream& out) const {
    out<<"Number of PseudoJets: "<<jets.size()<<endl;
    unsigned int ijet = 0;
    for(auto& jet : jets){
        out<<"PseudoJet #: "<<++ijet<<" pt: "<<jet.pt()<<" eta: "<<jet.eta()<<" phi: "<<jet.phi()<<endl;
        if(!jet.has_constituents())continue;
        out<<"constituents: "<<endl;
        unsigned int icst = 0;
        for(auto& cst : jet.constituents()){
            out<<" . constituent #: "<<++icst <<"("<<cst.user_index()<<")"<<" pt: "<<cst.pt()<<" eta: "<<cst.eta()<<" phi: "<<cst.phi()<<endl;
        }
    }
}

void FastJetWrapperBase::printJetVectors(const vector<JetVector> & jets, ostream& out) const {
    cout<<"Number of JetVectors: "<<jets.size()<<endl;
    unsigned int ijet = 0;
    for(auto& jet : jets){
        cout<<"JetVector: "<<++ijet<<" ";
        jet.print(out);
    }
}



