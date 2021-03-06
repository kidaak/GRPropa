#include "grpropa/module/OutputTXT.h"
#include "grpropa/Units.h"

#include <stdio.h>

namespace grpropa {

TrajectoryOutput::TrajectoryOutput(std::string name) {
    setDescription("Trajectory output");
    fout.open(name.c_str());
    fout << "# D\tID\tE\tX\tY\tZ\tPx\tPy\tPz\n";
    fout << "#\n";
    fout << "# D           Trajectory length\n";
    fout << "# ID          Particle type (PDG MC numbering scheme)\n";
    fout << "# E           Energy [EeV]\n";
    fout << "# X, Y, Z     Position [Mpc]\n";
    fout << "# Px, Py, Pz  Heading (unit vector of momentum)\n";
    fout << "#\n";
}

TrajectoryOutput::~TrajectoryOutput() {
    fout.close();
}

void TrajectoryOutput::process(Candidate *c) const {
    char buffer[1024];
    size_t p = 0;

    p += sprintf(buffer + p, "%8.3f\t", c->getTrajectoryLength() / Mpc);
    p += sprintf(buffer + p, "%10i\t", c->current.getId());
    p += sprintf(buffer + p, "%.4g\t", c->current.getEnergy() / eV);
    Vector3d pos = c->current.getPosition() / Mpc;
    p += sprintf(buffer + p, "%8.8f\t%8.8f\t%8.8f\t", pos.x, pos.y, pos.z);
    const Vector3d &dir = c->current.getDirection();
    p += sprintf(buffer + p, "%8.5f\t%8.5f\t%8.5f\n", dir.x, dir.y, dir.z);

#pragma omp critical
    {
        fout.write(buffer, p);
        fout.flush();
    }
}

ConditionalOutput::ConditionalOutput(std::string fname, std::string cond) :
        condition(cond) {
    setDescription(
            "Conditional output, condition: " + cond + ", filename: " + fname);
    fout.open(fname.c_str());
    fout << "# D\tID\tID0\tE\tE0\tX\tY\tZ\tX0\tY0\tZ0\tPx\tPy\tPz\tP0x\tP0y\tP0z\tz\n";
    fout << "#\n";
    fout << "# D           Trajectory length [Mpc]\n";
    fout << "# ID          Particle type (PDG MC numbering scheme)\n";
    fout << "# E           Energy [EeV]\n";
    fout << "# X, Y, Z     Position [Mpc]\n";
    fout << "# Px, Py, Pz  Heading (unit vector of momentum)\n";
    fout << "# z           Current redshift\n";
    fout << "# Initial state: ID0, E0, ...\n";
    fout << "#\n";
}

ConditionalOutput::~ConditionalOutput() {
    fout.close();
}

void ConditionalOutput::process(Candidate *c) const {
    if (not (c->hasProperty(condition)))
        return;

    c->removeProperty(condition);

    char buffer[256];
    size_t p = 0;

    p += sprintf(buffer + p, "%8.3f\t", c->getTrajectoryLength() / Mpc);
    p += sprintf(buffer + p, "%10i\t", c->current.getId());
    p += sprintf(buffer + p, "%10i\t", c->source.getId());
    p += sprintf(buffer + p, "%.4g\t", c->current.getEnergy() / eV);
    p += sprintf(buffer + p, "%.4g\t", c->source.getEnergy() / eV);
    Vector3d pos = c->current.getPosition() / Mpc;
    p += sprintf(buffer + p, "%9.4f\t%9.4f\t%9.4f\t", pos.x, pos.y, pos.z);
    Vector3d ipos = c->source.getPosition() / Mpc;
    p += sprintf(buffer + p, "%9.4f\t%9.4f\t%9.4f\t", ipos.x, ipos.y, ipos.z);
    Vector3d dir = c->current.getDirection();
    p += sprintf(buffer + p, "%8.5f\t%8.5f\t%8.5f\t", dir.x, dir.y, dir.z);
    Vector3d idir = c->source.getDirection();
    p += sprintf(buffer + p, "%8.5f\t%8.5f\t%8.5f\t", idir.x, idir.y, idir.z);
    p += sprintf(buffer + p, "%1.3f\n", c->getRedshift());

#pragma omp critical
    {
        fout.write(buffer, p);
        fout.flush();
    }
}

TrajectoryOutput1D::TrajectoryOutput1D(std::string filename) {
    setDescription("TrajectoryOutput, filename: " + filename);
    fout.open(filename.c_str());
    fout << "#X\tID\tE\n";
    fout << "#\n";
    fout << "# X  Position [Mpc]\n";
    fout << "# ID Particle type\n";
    fout << "# E  Energy [EeV]\n";
}

TrajectoryOutput1D::~TrajectoryOutput1D() {
    fout.close();
}

void TrajectoryOutput1D::process(Candidate *c) const {
    char buffer[1024];
    size_t p = 0;
    p += sprintf(buffer + p, "%8.4f\t", c->current.getPosition().x / Mpc);
    p += sprintf(buffer + p, "%10i\t", c->current.getId());
    p += sprintf(buffer + p, "%.4g\n", c->current.getEnergy() / eV);
#pragma omp critical
    {
        fout.write(buffer, p);
        fout.flush();
    }
}

EventOutput1D::EventOutput1D(std::string filename) {
    setDescription("Conditional output, filename: " + filename);
    fout.open(filename.c_str());
    fout << "#ID\tE\tD\tID0\tE0\n";
    fout << "#\n";
    fout << "# ID  Particle type\n";
    fout << "# E   Energy [EeV]\n";
    fout << "# D   Comoving source distance [Mpc]\n";
    fout << "# ID0 Initial particle type\n";
    fout << "# E0  Initial energy [EeV]\n";
}

EventOutput1D::~EventOutput1D() {
    fout.close();
}

void EventOutput1D::process(Candidate *c) const {
    if (not (c->hasProperty("Detected")))
        return;

    c->removeProperty("Detected");

    char buffer[256];
    size_t p = 0;

    p += sprintf(buffer + p, "%10i\t", c->current.getId());
    p += sprintf(buffer + p, "%.4g\t", c->current.getEnergy() / eV);
    p += sprintf(buffer + p, "%9.4f\t", c->source.getPosition().x / Mpc);
    p += sprintf(buffer + p, "%10i\t", c->source.getId());
    p += sprintf(buffer + p, "%.4g\n", c->source.getEnergy() / eV);

#pragma omp critical
    {
        fout.write(buffer, p);
        fout.flush();
    }
}

} // namespace grpropa
