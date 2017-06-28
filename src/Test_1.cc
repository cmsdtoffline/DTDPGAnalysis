// system include files
#include <memory>
#include <fstream>
#include <string>
#include <vector>

// user include files
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/Common/interface/SortedCollection.h"
#include "DataFormats/HcalRecHit/interface/HORecHit.h"
#include "DataFormats/HcalDigi/interface/HOTriggerPrimitiveDigi.h"

#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "TFile.h"
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.


class Test_1: public edm::one::EDAnalyzer<edm::one::SharedResources> {
    public:
    
    explicit Test_1(const edm::ParameterSet&);
    ~Test_1();
    
    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


    private:
    
    virtual void beginJob() override;
    virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
    virtual void endJob() override;
    
    virtual void clear();
    virtual void fill_hoTP_variables(edm::Handle <edm::SortedCollection <HOTriggerPrimitiveDigi> > hotpHandle);
    
    // ----------member data ---------------------------
    
    int hoTPdigi_n;
    std::vector<int> hoTPdigi_bits;
    std::vector<int> hoTPdigi_iEta;
    std::vector<int> hoTPdigi_iPhi;
    std::vector<int> hoTPdigi_nSamples;
    std::vector<int> hoTPdigi_raw;
    std::vector<int> hoTPdigi_whichSampleTriggered;
    
    TTree *tree;
    
    edm::InputTag hoTPLabel;
    
    edm::EDGetTokenT <edm::SortedCollection <HOTriggerPrimitiveDigi, \
        edm::StrictWeakOrdering <HOTriggerPrimitiveDigi> > > tok_hoTP;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
Test_1::Test_1(const edm::ParameterSet& iConfig)
{
    //now do what ever initialization is needed
    
    usesResource("TFileService");
    
    edm::Service <TFileService> fs;
    tree = fs->make<TTree>("tree", "tree");
    
    
    tree->Branch("hoTPdigi_n", &hoTPdigi_n);
    tree->Branch("hoTPdigi_bits", &hoTPdigi_bits);
    tree->Branch("hoTPdigi_iEta", &hoTPdigi_iEta);
    tree->Branch("hoTPdigi_iPhi", &hoTPdigi_iPhi);
    tree->Branch("hoTPdigi_nSamples", &hoTPdigi_nSamples);
    tree->Branch("hoTPdigi_raw", &hoTPdigi_raw);
    tree->Branch("hoTPdigi_whichSampleTriggered", &hoTPdigi_whichSampleTriggered);
    
    
    hoTPLabel = iConfig.getParameter<edm::InputTag>("hoTPLabel");
    
    tok_hoTP = consumes <edm::SortedCollection <HOTriggerPrimitiveDigi, \
             edm::StrictWeakOrdering <HOTriggerPrimitiveDigi> > > (edm::InputTag(hoTPLabel));
}


Test_1::~Test_1()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void Test_1::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    clear();
    
    edm::Handle <edm::SortedCollection <HOTriggerPrimitiveDigi> > hotpHandle;
    iEvent.getByToken(tok_hoTP, hotpHandle);
    
    fill_hoTP_variables(hotpHandle);
    
    tree->Fill();
}
// ------------ method called once each job just before starting event loop  ------------
void 
Test_1::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
Test_1::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
Test_1::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}


void Test_1::fill_hoTP_variables(edm::Handle <edm::SortedCollection <HOTriggerPrimitiveDigi> > hotpHandle)
{
    for(edm::SortedCollection <HOTriggerPrimitiveDigi, \
        edm::StrictWeakOrdering <HOTriggerPrimitiveDigi> >::const_iterator hoTP = hotpHandle->begin();
        hoTP != hotpHandle->end(); ++hoTP)
    {
        if(!hoTP->bits())
        {
            continue;
        }
        
        hoTPdigi_n++;
        
        hoTPdigi_bits.push_back(hoTP->bits());
        hoTPdigi_iEta.push_back(hoTP->ieta());
        hoTPdigi_iPhi.push_back(hoTP->iphi());
        hoTPdigi_nSamples.push_back(hoTP->nsamples());
        hoTPdigi_raw.push_back(hoTP->raw());
        hoTPdigi_whichSampleTriggered.push_back(hoTP->whichSampleTriggered());
        
        //printf("hoTPdigi %d: bits %d, whSamp %d, data(whSamp) %d, mip %d \n", \
            hoTPdigi_n, \
            hoTP->bits(), \
            hoTP->whichSampleTriggered(), \
            hoTP->data(hoTP->whichSampleTriggered()), \
            (hoTP->bits()>>hoTP->whichSampleTriggered())&0x1);
    }
    
    printf("Number of HOTPs = %d \n", hoTPdigi_n);
}


void Test_1::clear()
{
    hoTPdigi_n = 0;
    hoTPdigi_bits.clear();
    hoTPdigi_iEta.clear();
    hoTPdigi_iPhi.clear();
    hoTPdigi_nSamples.clear();
    hoTPdigi_raw.clear();
    hoTPdigi_whichSampleTriggered.clear();
}

//define this as a plug-in
DEFINE_FWK_MODULE(Test_1);
