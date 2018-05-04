#ifndef ISING_CORE_ISING_PARAMETER_H_
#define ISING_CORE_ISING_PARAMETER_H_

#include <string>
#include <vector>

#include <include/rapidjson/document.h>

#include "ising-core/ising.h"

ISING_NAMESPACE_BEGIN

// Allow relaxed JSON syntax (comments and trailing commas).
const auto kJsonParseFlag = rapidjson::kParseCommentsFlag
                          + rapidjson::kParseTrailingCommasFlag;

// The settings file (JSON) may have the following keys:
//   "boundary"                      string ("periodic", "free")
//   "size.list"                     integer array
//   "temperature.list"              real-number array
//   "externalMagneticField.list"    real-number array
//   "size.span"                     object
//   "temperature.span"              object
//   "externalMagneticField.span"    object
//   "iterations"                    integer
//   "analysisEnsembleCount"         integer
//   "analysisEnsembleInterval"      integer
//   "repetitions"                   integer
//
//   [DEPRECATED]
//     "size"         integer
//     "xSize"        integer
//     "ySize"        integer
//     "beta.list"    real-number array
//     "beta.span"    object
//
// A "span" object may have the following values:
//   "begin" real-number
//   "end"   real-number
//   "step"  real-number

class Parameter
{
public:
    Parameter() = default;

    void ReadFromString(const std::string & settings);
    void ReadFromString(const char * settings);
    void ReadFromFile(const std::string & file_name);
    void ReadFromFile(const char * file_name);

    void Parse();

    BoundaryConditions       boundary_condition;
    LatticeSize              lattice_size;      // TODO: to be deprecated
    std::vector<LatticeSize> lattice_size_list;
    std::vector<double>      temperature_list;
    std::vector<double>      beta_list;         // TODO: to be deprecated
    std::vector<double>      magnetic_h_list;
    size_t                   iterations;
    size_t                   n_ensemble;
    size_t                   n_delta;
    size_t                   repetitions;

private:
    const size_t kDefaultIterations              = 1000;
    const size_t kDefaultIterationsEnsembleRatio = 10;
    const size_t kDefaultEnsembleInterval        = 1;
    const size_t kDefaultRepetitions             = 1;

    const double kDoubleTolerance = 1.0e-6;

    rapidjson::Document json_doc_;

    BoundaryConditions       ParseBoundaryCondition();
    LatticeSize              ParseLatticeSize();
    std::vector<LatticeSize> ParseLatticeSizeList();
    std::vector<double>      ParseTemperatureList();
    std::vector<double>      ParseBetaList();
    std::vector<double>      ParseMagneticFieldList();
    size_t                   ParseIterations();
    size_t                   ParseEnsembleCount();
    size_t                   ParseEnsembleInterval();
    size_t                   ParseRepetitions();
};

ISING_NAMESPACE_END

#endif
