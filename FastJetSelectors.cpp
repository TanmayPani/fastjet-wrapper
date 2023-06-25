#define FastJetSelectors_cxx

#include "FastJetSelectors.h"

FASTJET_BEGIN_NAMESPACE // place the code here inside the FJ namespace

class SW_UserIndexNonNegative : public SelectorWorker{
public:
   SW_UserIndexNonNegative(bool isTrue) : _isTrue(isTrue) {};

   virtual bool pass(const PseudoJet & jet) const {
        //if (jet.is_pure_ghost())return false;
         if (jet.user_index() > -1) { return  _isTrue; }
        else if (jet.user_index() < -1) { return !_isTrue; }
        else{
            //throw Error("Cannot apply this selector if user index is not set by the user (fastjet's default user index is -1)");
            return false;
        }
   }

   virtual string description() const {return "non-negative user_index";}

private:
    bool _isTrue;
}; 

Selector SelectorUserIndexNonNegative(bool isTrue) {
    return Selector(new SW_UserIndexNonNegative(isTrue));
}

class SW_EtaPhiCircle : public SelectorWorker{
public:
    SW_EtaPhiCircle(const float &radius) : _is_initialised(false), _radius2(radius*radius){};

    /// returns true if the worker takes a reference jet
    virtual bool takes_reference() const { return true;}

    /// sets the reference jet
    virtual void set_reference(const PseudoJet &centre){
        _is_initialised = true;
        _reference = centre;
    }

    /// return a copy of the current object
    virtual SelectorWorker* copy(){ return new SW_EtaPhiCircle(*this);}

    virtual bool pass(const PseudoJet & jet) const {
        // make sure the centre is initialised
        if (! _is_initialised)
            throw Error("To use a SelectorCircle (or any selector that requires a reference), you first have to call set_reference(...)");
  
        double deta = jet.eta()-_reference.eta();
        double dphi = abs(jet.phi()-_reference.phi());
        if(dphi > pi) {dphi = twopi - dphi;}
        double dr2 = deta*deta + dphi*dphi;
        return dr2 <= _radius2;
    } 

    /// returns a description of the worker
    virtual string description() const {
        ostringstream ostr;
        ostr << "distance from the centre <= " << sqrt(_radius2);
        return ostr.str();
    }

    /// returns the rapidity range for which it may return "true"
    virtual void get_rapidity_extent(float & rapmin, float & rapmax) const{
        // make sure the centre is initialised
        if (! _is_initialised)
            throw Error("To use a SelectorEtaPhiCircle (or any selector that requires a reference), you first have to call set_reference(...)");
      
        rapmax = _reference.eta()+sqrt(_radius2);
        rapmin = _reference.eta()-sqrt(_radius2);
    }

    virtual bool is_geometric() const { return true; }    ///< implies a finite area
    virtual bool has_finite_area() const { return true; } ///< regardless of the reference 
    virtual bool has_known_area() const { return true; }  ///< the area is analytically known
    virtual double known_area() const { return pi * _radius2; }

private:
    PseudoJet _reference;
    float _radius2;
    bool _is_initialised;
}; 

Selector SelectorEtaPhiCircle(const float & radius) {
    return Selector(new SW_EtaPhiCircle(radius));
}

class SW_EtaPhiAnnulus : public SelectorWorker{
public:
    SW_EtaPhiAnnulus(const float &radius_in, const float &radius_out) 
    : _is_initialised(false), _radius_in2(radius_in*radius_in), _radius_out2(radius_out*radius_out){};

    /// returns true if the worker takes a reference jet
    virtual bool takes_reference() const { return true;}

    /// sets the reference jet
    virtual void set_reference(const PseudoJet &centre){
        _is_initialised = true;
        _reference = centre;
    }

    /// return a copy of the current object
    virtual SelectorWorker* copy(){ return new SW_EtaPhiAnnulus(*this);}

    virtual bool pass(const PseudoJet & jet) const {
        // make sure the centre is initialised
        if (! _is_initialised)
            throw Error("To use a SelectorAnnulus (or any selector that requires a reference), you first have to call set_reference(...)");
  
        double deta = jet.eta()-_reference.eta();
        double dphi = abs(jet.phi()-_reference.phi());
        if(dphi > pi) {dphi = twopi - dphi;}
        double dr2 = deta*deta + dphi*dphi;
        return (dr2 <= _radius_out2) && (dr2 >= _radius_in2);
    } 

    /// returns a description of the worker
    virtual string description() const {
        ostringstream ostr;
        ostr << sqrt(_radius_in2) << " <= distance from the centre <= " << sqrt(_radius_out2);
        return ostr.str();
    }

    /// returns the rapidity range for which it may return "true"
    virtual void get_rapidity_extent(float & rapmin, float & rapmax) const{
        // make sure the centre is initialised
        if (! _is_initialised)
            throw Error("To use a SelectorEtaPhiCircle (or any selector that requires a reference), you first have to call set_reference(...)");
      
        rapmax = _reference.eta()+sqrt(_radius_out2);
        rapmin = _reference.eta()-sqrt(_radius_out2);
    }

    virtual bool is_geometric() const { return true; }    ///< implies a finite area
    virtual bool has_finite_area() const { return true; } ///< regardless of the reference 
    virtual bool has_known_area() const { return true; }  ///< the area is analytically known
    virtual double known_area() const { return pi * (_radius_out2-_radius_in2); }

private:
    PseudoJet _reference;
    float _radius_in2, _radius_out2;
    bool _is_initialised;
}; 

Selector SelectorEtaPhiAnnulus(const float & radius_in, const float & radius_out) {
    return Selector(new SW_EtaPhiAnnulus(radius_in, radius_out));
}

//class SW_CombineSelectors : public SelectorWorker {
//public:
//    SW_CombineSelectors(vector<Selector>& cutList) : list(cutList) {};
//    
//private:
//    vector<Selector> list;
//}

FASTJET_END_NAMESPACE