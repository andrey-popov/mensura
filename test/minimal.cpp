#include <GenericEventSelection.hpp>
#include <Dataset.hpp>
#include <PECReader.hpp>
#include <BTagger.hpp>
#include <BTagEfficiencies.hpp>
#include <BTagScaleFactors.hpp>
#include <WeightBTag.hpp>
#include <TriggerSelection.hpp>

#include <iostream>
#include <memory>


using namespace std;


int main()
{
    // Define the b-tagging objects
    shared_ptr<BTagger const> bTagger(
     new BTagger(BTagger::Algorithm::CSV, BTagger::WorkingPoint::Tight));
    
    
    // Define the event selection
    GenericEventSelection sel(30., bTagger);
    sel.AddLeptonThreshold(Lepton::Flavour::Muon, 26.);
    sel.AddJetTagBin(2, 1);
    sel.AddJetTagBin(3, 1);
    sel.AddJetTagBin(3, 2);
    
    
    // Define datasets
    string const filePrefix("/gridgroup/cms/popov/PECData/2012Bravo/");
    double const brWlnu = 3 * 0.1080;
    list<Dataset> datasets;
    
    // ttbar
    datasets.emplace_back(Dataset({Dataset::Process::ttbar, Dataset::Process::ttSemilep},
     Dataset::Generator::MadGraph, Dataset::ShowerGenerator::Pythia));
    datasets.back().AddFile(filePrefix + "ttbar-semilep-mg-p1_53X.02.05_tTP_p1.root",
     234. * brWlnu * (1. - brWlnu) * 2, 24953451);
    datasets.back().AddFile(filePrefix + "ttbar-semilep-mg-p1_53X.02.05_tTP_p2.root",
     234. * brWlnu * (1. - brWlnu) * 2, 24953451);
    datasets.back().AddFile(filePrefix + "ttbar-semilep-mg-p1_53X.02.05_tTP_p3.root",
     234. * brWlnu * (1. - brWlnu) * 2, 24953451);
    datasets.back().AddFile(filePrefix + "ttbar-semilep-mg-p1_53X.02.05_tTP_p4.root",
     234. * brWlnu * (1. - brWlnu) * 2, 24953451);
    
     
    // Define the triggers
    list<TriggerRange> triggerRanges;
    triggerRanges.emplace_back(0, -1, "IsoMu24_eta2p1", 19.7e3, "IsoMu24_eta2p1");
    
    TriggerSelection triggerSel(triggerRanges);
    
    
    // Define reweighting for b-tagging
    BTagEfficiencies bTagEff("BTagEff_2012Bravo_v1.0.root", "in4_jPt30/");
    
    // Set a mapping from process codes to names of histograms with b-tagging efficiencies
    bTagEff.SetProcessLabel(Dataset::Process::ttSemilep, "ttbar-semilep");
    bTagEff.SetProcessLabel(Dataset::Process::ttchan, "t-tchan");
    bTagEff.SetProcessLabel(Dataset::Process::ttH, "ttH");
    bTagEff.SetProcessLabel(Dataset::Process::tHq, "tHq-nc");
    bTagEff.SetDefaultProcessLabel("ttbar-inc");
    
    BTagScaleFactors bTagSF(bTagger->GetAlgorithm());
    WeightBTag bTagReweighter(bTagger, bTagEff, bTagSF);
    
    
    // Perform dataset-specific initialization
    bTagReweighter.LoadPayload(datasets.front());
    
    
    // Build an instance of PECReader
    PECReader reader(datasets.front());
    reader.SetTriggerSelection(&triggerSel);
    reader.SetEventSelection(&sel);
    reader.SetBTagReweighter(&bTagReweighter);
    //reader.SetReadHardInteraction();
    reader.NextSourceFile();
    
    
    // Loop over few events
    for (unsigned i = 0; i < 10; ++i)
    {
        cout << "Event " << i << '\n';
        reader.NextEvent();
        
        cout << "Tight leptons' pts:";
        
        for (auto const &l: reader.GetLeptons())
            cout << " pt: " << l.Pt() << ", iso: " << l.RelIso() << ", dB: " << l.DB();
        
        cout << "\nAnalysis jets' pts:";
        
        for (auto const &j: reader.GetJets())
            cout << " " << j.Pt();
        
        cout << "\nAdditional jets' pts:";
        
        for (auto const &j: reader.GetAdditionalJets())
            cout << " " << j.Pt();
        
        cout << "\nNeutrino's pt: " << reader.GetNeutrino().Pt() << ", pz: " <<
         reader.GetNeutrino().P4().Pz();
        
        /*
        cout << "\nHard generator-level particles:";
        
        for (auto const &p: reader.GetHardGenParticles())
        {
            cout << "\n PDG ID: " << p.GetPdgId() <<
             "\n # mothers: " << p.GetMothers().size() <<
             "\n # daughters: " << p.GetDaughters().size();
        }
        */
        
        cout << "\nEvent weight: " << reader.GetCentralWeight();
        
        cout << "\n\n";
    }
    
    
    return 0;
}