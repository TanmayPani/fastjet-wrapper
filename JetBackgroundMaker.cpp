#define JetBackgroundMaker_cxx

#include "JetBackgroundMaker.h"

using namespace fastjet;
using namespace std;

JetBackgroundMaker::JetBackgroundMaker(bool debug, bool dofulldebug): FastJetWrapperBase(dofulldebug){
    doDebug = debug;
    if(doDebug)cout<<"************* JetBackgroundMaker::JetBackgroundMaker() *****************"<<endl;
    setDoArea(true);
}

JetBackgroundMaker::~JetBackgroundMaker(){
     
}

void JetBackgroundMaker::init(){
    FastJetWrapperBase::init();
    setEstimator();
}

void JetBackgroundMaker::setEstimator(){
    jetMedBgE.reset(new JetMedianBackgroundEstimator(jetSelection->getSelector(), *jetDef, *areaDef));
}

void JetBackgroundMaker::estimateBackground(){
    if(doDebug)cout<<"*********** JetBackgroundMaker::estimateBackground() ***********"<<endl;
    if(!jetMedBgE){
        throw Error("Cant do background estimation without a properly set estimator!");
    }

    if(doDebug){
        cout<<"Setting particles . . ."<<endl;
        printPseudoJets(fullEvent);
    }

    jetMedBgE->set_particles(fullEvent);
    _rho = jetMedBgE->rho();
    _sigma = jetMedBgE->sigma();
    if(doDebug)cout<<"Rho: "<<_rho<<" Sigma: "<<_sigma<<endl;

    constitSub.reset(new contrib::ConstituentSubtractor(jetMedBgE.get()));
    constitSub->set_common_bge_for_rho_and_rhom(true);
    if(doDebug)cout<<"Set Constituent background subtractor: "<<constitSub->description()<<endl;
}

float JetBackgroundMaker::getRho(const PseudoJet& jet) const {
    return jetMedBgE->rho(jet);
}

float JetBackgroundMaker::getSigma(const PseudoJet& jet) const {
    return jetMedBgE->sigma(jet);
}

PseudoJet JetBackgroundMaker::getBackgroundSubtractedJet(PseudoJet& jet){
    return (*constitSub)(jet);
}

void JetBackgroundMaker::clear(){
    FastJetWrapperBase::clear();
    if(constitSub)constitSub.reset();
}