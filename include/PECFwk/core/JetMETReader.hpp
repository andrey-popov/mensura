#pragma once

#include <PECFwk/core/ReaderPlugin.hpp>

#include <PECFwk/core/PhysicsObjects.hpp>

#include <vector>


/**
 * \class JetMETReader
 * \brief An abstract base class for a reader plugin that provides collection jets and MET
 * 
 * Reconstructed jets and MET are expected to be fully corrected.
 */
class JetMETReader: public ReaderPlugin
{
public:
    /**
     * \brief Creates plugin with the given name
     * 
     * User is encouraged to keep the default name.
     */
    JetMETReader(std::string const name = "JetMET");
    
    /// Default copy constructor
    JetMETReader(JetMETReader const &) = default;
    
    /// Default move constructor
    JetMETReader(JetMETReader &&) = default;
    
    /// Default assignment operator
    JetMETReader &operator=(JetMETReader const &) = default;
    
    /// Trivial destructor
    virtual ~JetMETReader() noexcept;
    
public:
    /// Returns collection of corrected jets in the current event
    std::vector<Jet> const &GetJets() const;
    
    /// Returns corrected MET in the current event
    MET const &GetMET() const;
    
protected:
    /// Collection of (corrected) jets in the current event
    std::vector<Jet> jets;
    
    /// Corrected MET in the current event
    MET met;
};
