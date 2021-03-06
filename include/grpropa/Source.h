#ifndef GRPROPA_SOURCE_H
#define GRPROPA_SOURCE_H

#include "grpropa/Candidate.h"
#include "grpropa/Grid.h"

#include <vector>

namespace grpropa {

/**
 @class SourceFeature
 @brief Abstract base class cosmic ray source features
 */
class SourceFeature: public Referenced {
protected:
    std::string description;
public:
    virtual void prepareParticle(ParticleState& particle) const;
    virtual void prepareCandidate(Candidate& candidate) const;
    std::string getDescription() const;
};

/**
 @class Source
 @brief General cosmic ray source

 This class is a container for source features.
 The source prepares a new candidate by passing it to all its source features
 to be modified accordingly.
 */
class Source: public Referenced {
    std::vector<ref_ptr<SourceFeature> > features;
public:
    void add(SourceFeature* feature);
    ref_ptr<Candidate> getCandidate() const;
    std::string getDescription() const;
};

/**
 @class SourceList
 @brief List of cosmic ray sources of individual lumosities.

 The SourceList is a source itself. It can be used if several sources are
 needed in one simulation.
 */
class SourceList: public Source {
    std::vector<ref_ptr<Source> > sources;
    std::vector<double> cdf;
public:
    void add(Source* source, double weight = 1);
    ref_ptr<Candidate> getCandidate() const;
};

/**
 @class SourceParticleType
 @brief Particle type at the source
 */
class SourceParticleType: public SourceFeature {
    int id;
public:
    SourceParticleType(int id);
    void prepareParticle(ParticleState &particle) const;
    void setDescription();
};

/**
 @class SourceMultipleParticleTypes
 @brief Multiple particle types with individual relative abundances
 */
class SourceMultipleParticleTypes: public SourceFeature {
    std::vector<int> particleTypes;
    std::vector<double> cdf;
public:
    SourceMultipleParticleTypes();
    void add(int id, double weight = 1);
    void prepareParticle(ParticleState &particle) const;
    void setDescription();
};

/**
 @class SourceEnergy
 @brief Sets the initial energy to a given value
 */
class SourceEnergy: public SourceFeature {
    double E;
public:
    SourceEnergy(double energy);
    void prepareParticle(ParticleState &particle) const;
    void setDescription();
};

/**
 @class SourcePowerLawSpectrum
 @brief Particle energy following a power law spectrum
 */
class SourcePowerLawSpectrum: public SourceFeature {
    double Emin;
    double Emax;
    double index;
public:
    SourcePowerLawSpectrum(double Emin, double Emax, double index);
    void prepareParticle(ParticleState &particle) const;
    void setDescription();
};

/**
 @class SourcePosition
 @brief Position of a point source
 */
class SourcePosition: public SourceFeature {
    Vector3d position; /**< Source position */
public:
    SourcePosition(Vector3d position);
    SourcePosition(double d);
    void prepareParticle(ParticleState &state) const;
    void setDescription();
};

/**
 @class SourceMultiplePositions
 @brief Multiple point source positions with individual luminosities
 */
class SourceMultiplePositions: public SourceFeature {
    std::vector<Vector3d> positions;
    std::vector<double> cdf;
public:
    SourceMultiplePositions();
    void add(Vector3d position, double weight = 1);
    void prepareParticle(ParticleState &particle) const;
    void setDescription();
};

/**
 @class SourceUniformSphere
 @brief Uniform random source positions inside a sphere
 */
class SourceUniformSphere: public SourceFeature {
    Vector3d center;
    double radius;
public:
    SourceUniformSphere(Vector3d center, double radius);
    void prepareParticle(ParticleState &particle) const;
    void setDescription();
};

/**
 @class SourceUniformShell
 @brief Uniform random source positions on a sphere
 */
class SourceUniformShell: public SourceFeature {
    Vector3d center;
    double radius;
public:
    SourceUniformShell(Vector3d center, double radius);
    void prepareParticle(ParticleState &particle) const;
    void setDescription();
};

/**
 @class SourceUniformBox
 @brief Uniform random source positions inside a box
 */
class SourceUniformBox: public SourceFeature {
    Vector3d origin;
    Vector3d size;
public:
    /** Constructor
     @param origin  lower box corner
     @param size    upper box corner
     */
    SourceUniformBox(Vector3d origin, Vector3d size);
    void prepareParticle(ParticleState &particle) const;
    void setDescription();
};

/**
 @class SourceUniform1D
 @brief 1D-Positions from a uniform source distribution in an expanding universe

 This source property sets random x-coordinates according to a uniform source
 distribution in a given comoving distance interval.
 This is done by drawing a light travel distance from a flat distribution and
 converting to a comoving distance.
 */
class SourceUniform1D: public SourceFeature {
    double minD; // minimum light travel distance
    double maxD; // maximum light travel distance
    bool withCosmology;
public:
    /** Constructor
     @param minD    minimum comoving distance
     @param maxD    maximum comoving distance
     @param withCosmology   specify if universe expanding
     */
    SourceUniform1D(double minD, double maxD, bool withCosmology=true);
    void prepareParticle(ParticleState& particle) const;
    void setDescription();
};

/**
 @class SourceDensityGrid
 @brief Random source positions from a density grid
 */
class SourceDensityGrid: public SourceFeature {
    ref_ptr<ScalarGrid> grid;
public:
    SourceDensityGrid(ref_ptr<ScalarGrid> densityGrid);
    void prepareParticle(ParticleState &particle) const;
    void setDescription();
};

/**
 @class SourceDensityGrid1D
 @brief Random source positions from a 1D density grid
 */
class SourceDensityGrid1D: public SourceFeature {
    ref_ptr<ScalarGrid> grid;
public:
    SourceDensityGrid1D(ref_ptr<ScalarGrid> densityGrid);
    void prepareParticle(ParticleState &particle) const;
    void setDescription();
};

/**
 @class SourceIsotropicEmission
 @brief Isotropic emission from a source
 */
class SourceIsotropicEmission: public SourceFeature {
public:
    SourceIsotropicEmission();
    void prepareParticle(ParticleState &particle) const;
    void setDescription();
};

/**
 @class SourceDirection
 @brief Emission in a discrete direction
 */
class SourceDirection: public SourceFeature {
    Vector3d direction;
public:
    SourceDirection(Vector3d direction = Vector3d(-1, 0, 0));
    void prepareParticle(ParticleState &particle) const;
    void setDescription();
};

/**
 @class SourceEmissionCone
 @brief Uniform random emission inside a cone
 */
class SourceEmissionCone: public SourceFeature {
    Vector3d direction;
    double aperture;
public:
    SourceEmissionCone(Vector3d direction, double aperture);
    void prepareParticle(ParticleState &particle) const;
    void setDescription();
};

/**
 @class SourceRedshift
 @brief Discrete redshift (time of emission)
 */
class SourceRedshift: public SourceFeature {
    double z;
public:
    SourceRedshift(double z);
    void prepareCandidate(Candidate &candidate) const;
    void setDescription();
};

/**
 @class SourceUniformRedshift
 @brief Uniform redshift distribution (time of emission)
 */
class SourceUniformRedshift: public SourceFeature {
    double zmin, zmax;
public:
    SourceUniformRedshift(double zmin, double zmax);
    void prepareCandidate(Candidate &candidate) const;
    void setDescription();
};

/**
 @class SourceRedshift1D
 @brief Redshift according to the distance to 0

 This source property sets the redshift according to the distance to 0.
 It must be added after a position setting source property.
 */
class SourceRedshift1D: public SourceFeature {
public:
    SourceRedshift1D();
    void prepareCandidate(Candidate &candidate) const;
    void setDescription();
};


}// namespace grpropa

#endif // GRPROPA_SOURCE_H
