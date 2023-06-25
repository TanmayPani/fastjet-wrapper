//Wrapper class to use background estimator and background subtractor

#ifndef JetBackgroundMaker_h
#define JetBackgroundMaker_h

#include "FastJetWrapperBase.h"

#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/Selector.hh"
#include "fastjet/tools/JetMedianBackgroundEstimator.hh"
#include "fastjet/tools/GridMedianBackgroundEstimator.hh"
#include "fastjet/tools/Subtractor.hh"
#include "fastjet/contrib/ConstituentSubtractor.hh"
//#include "fastjet/contrib/GenericSubtractor.hh"

class JetBackgroundMaker : public FastJetWrapperBase{
    public:
        JetBackgroundMaker(bool debug = false, bool fulldebug = false); 
        ~JetBackgroundMaker() override;

        void init() override;
        void clear() override;

        void printDescription() override{
            std::cout<<"Jet median background details: "<<jetMedBgE->description()<<std::endl;
        }

        void setInputJets(std::vector<fastjet::PseudoJet>& j);

        void setEstimator();
        void estimateBackground();
        float getRho() const {return _rho;}
        float getRho(const fastjet::PseudoJet& jet) const;
        float getSigma() const {return _sigma;}
        float getSigma(const fastjet::PseudoJet& jet) const;
         
        fastjet::PseudoJet getBackgroundSubtractedJet(fastjet::PseudoJet& jet);

    private:
        bool doDebug = false;
        
        std::unique_ptr<fastjet::JetMedianBackgroundEstimator> jetMedBgE = nullptr;
        std::unique_ptr<fastjet::contrib::ConstituentSubtractor> constitSub = nullptr;

        float _rho = 0;
        float _sigma = 0;
};
#endif
