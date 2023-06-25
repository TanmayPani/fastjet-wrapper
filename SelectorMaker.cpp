#define SelectorMaker_cxx

#include "SelectorMaker.h"

using namespace fastjet;

using namespace std;

map<string, function<Selector(float)> > SelectorMaker::cuts = {
    {"PtMax",     [](float value){return SelectorPtMax    (value); }}, {"PtMin",     [](float value){return SelectorPtMin    (value); }},
    {"EtMax",     [](float value){return SelectorEtMax    (value); }}, {"EtMin",     [](float value){return SelectorEtMin    (value); }},
    {"EMax",      [](float value){return SelectorEMax     (value); }}, {"EMin",      [](float value){return SelectorEMin     (value); }},
    {"EtaMax",    [](float value){return SelectorEtaMax   (value); }}, {"EtaMin",    [](float value){return SelectorEtaMin   (value); }},
    {"AbsEtaMax", [](float value){return SelectorAbsEtaMax(value); }}, {"AbsEtaMin", [](float value){return SelectorAbsEtaMin(value); }},
    {"RapMax",    [](float value){return SelectorRapMax   (value); }}, {"RapMin",    [](float value){return SelectorRapMin   (value); }},
    {"AbsRapMax", [](float value){return SelectorAbsRapMax(value); }}, {"AbsRapMin", [](float value){return SelectorAbsRapMin(value); }}
};

map<string, function<Selector(float, float)> > SelectorMaker::rangeCuts = {
    {"Pt",     [](float value1, float value2){return SelectorPtRange    (value1, value2); }}, 
    {"Eta",    [](float value1, float value2){return SelectorEtaRange   (value1, value2); }}, 
    {"Rap",    [](float value1, float value2){return SelectorRapRange   (value1, value2); }}, 
    {"AbsEta", [](float value1, float value2){return SelectorAbsEtaRange(value1, value2); }}, 
    {"AbsRap", [](float value1, float value2){return SelectorAbsRapRange(value1, value2); }}
};

SelectorMaker::SelectorMaker(){

}

void SelectorMaker::setCut(const string& var, const string& type, float val) {
    string selname = var+type;
    auto sel = cuts.find(selname);
    if(sel != cuts.end()) {
        Selector current = sel->second(val);
        cutList.push_back(current);
        ncuts++;
        if(current.is_geometric()) geomCutList.push_back(current);

    } else {
        throw invalid_argument("Invalid selector type: " + selname);
    }
}

void SelectorMaker::setRange(const string& var, float _min, float _max) {
    string selname = var;
    auto sel = rangeCuts.find(selname); 
    if(sel != rangeCuts.end()) {
        Selector current = sel->second(_min, _max);
        cutList.push_back(current);
        ncuts++;
        if(current.is_geometric())geomCutList.push_back(current);
        
    } else {
        throw invalid_argument("Invalid selector type: " + selname + "Range");
    }
}

void SelectorMaker::makeSelector() {
    if(geomCutList.empty()){
        geomCutList.push_back(SelectorIdentity());
        if(cutList.empty()) {
            cutList.push_back(SelectorIdentity());
            return;
        } 
    }

    Selector combined = cutList[0];

    for(int i = 1; i < ncuts; i++){
        combined &= cutList[i];
        //cout<<cutList[i].description()<<endl;
    }

    cutList.push_back(combined);

    Selector geomCombined = geomCutList[0];

    for(int i = 1; i < geomCutList.size(); i++){
        geomCombined &= geomCutList[i];
    }

    geomCutList.push_back(geomCombined);
}


