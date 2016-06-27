#pragma once

#include <mensura/core/ReaderPlugin.hpp>

#include <mensura/PECReader/GeneratorInfo.hpp>

#include <vector>


class PECInputData;


/**
 * \class PECGeneratorReader
 * \brief Reads generator-level information about a process from a PEC file
 * 
 * This plugin reads information about generate process, such as LHE-level event weights, input
 * state, etc. Other plugins should be used to access generator-level particles and jets.
 * 
 * This reader relies on PECInputData to get access to the input file. It should only be used with
 * simulated datasets.
 */
class PECGeneratorReader: public ReaderPlugin
{
public:
    /**
     * \brief Creates plugin with the given name
     * 
     * User is encouraged to keep the default name.
     */
    PECGeneratorReader(std::string const name = "Generator");
    
    /// Copy constructor
    PECGeneratorReader(PECGeneratorReader const &src) noexcept;
    
    /// Default move constructor
    PECGeneratorReader(PECGeneratorReader &&) = default;
    
    /// Assignment operator is deleted
    PECGeneratorReader &operator=(PECGeneratorReader const &) = delete;
    
public:
    /**
     * \brief Sets up reading of the tree with pile-up information
     * 
     * Reimplemented from Plugin.
     */
    virtual void BeginRun(Dataset const &dataset) override;
    
    /**
     * \brief Creates a newly configured clone
     * 
     * Implemented from Plugin.
     */
    virtual Plugin *Clone() const override;
    
    /**
     * \brief Returns alternative LHE-level weight with the given index
     * 
     * These weights are only available if they have been requested using method RequestAltWeights.
     * Otherwise an exception is thrown.
     */
    double GetAltWeight(unsigned index) const;
    
    /// Returns nominal LHE-level weight of the current event
    double GetNominalWeight() const;
    
    /// Returns number of available alternative weights
    unsigned GetNumAltWeights() const;
    
    /// Returns process ID (as written in LHE file) of the current event
    int GetProcessID() const;
    
    /// Specifies whether alternative LHE-level weights should be read
    void RequestAltWeights(bool on = true);
    
private:
    /**
     * \brief Reads pile-up information for a PEC file
     * 
     * Reimplemented from Plugin.
     */
    virtual bool ProcessEvent() override;
    
private:
    /// Name of the plugin that reads PEC files
    std::string inputDataPluginName;
    
    /// Non-owning pointer to a plugin that reads PEC files
    PECInputData const *inputDataPlugin;
    
    /// Flag showing whether alternative weights should be read
    bool readAltWeights;
    
    /// Name of the tree with generator information
    std::string treeName;
    
    /// Buffer to read the only branch of the tree
    pec::GeneratorInfo bfGenerator;
    
    /**
     * \brief An auxiliary pointer to the buffer
     * 
     * Need by ROOT to read the object from a tree.
     */
    decltype(bfGenerator) *bfGeneratorPointer;
};
