// g++ GF_para.cc -lboost_program_options
#include <stdlib.h>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

namespace Grid {
namespace QCD {

struct HMC_PARA{
  std::string StartingType;
  int StartingTrajectory;
  int Thermalizations;
  int Trajectories;
  int mdSteps;
  double trajL;
  int saveInterval;

  std::string action;
  double beta;

  MyTC_para tc_para;
};

std::ostream& operator<<(std::ostream& out, const HMC_PARA &HMC_para)
{
  out << "StartingType: " << HMC_para.StartingType << std::endl;
  out << "StartingTrajectory: " << HMC_para.StartingTrajectory << std::endl;
  out << "Thermalizations: " << HMC_para.Thermalizations << std::endl;
  out << "Trajectories: " << HMC_para.Trajectories << std::endl;
  out << "mdSteps: " << HMC_para.mdSteps << std::endl;
  out << "trajL: " << HMC_para.trajL << std::endl;
  out << "saveInterval: " << HMC_para.saveInterval << std::endl;

  out << "action: " << HMC_para.action << std::endl;
  out << "beta: " << HMC_para.beta << std::endl;


  return out;
}





void init(int argc, char **argv, HMC_PARA &hmc_para)
{
  po::options_description desc("HMC options");
  desc.add_options()("help", "help message")
                    ("StartingType", po::value<std::string>(&hmc_para.StartingType)->default_value("ColdStart"), "Stariing configuration. It can be HotStart, ColdStart, TepidStart, or CheckpointStart.")
                    ("StartingTrajectory", po::value<int>(&hmc_para.StartingTrajectory), "If StartingType is CheckpointStart, ckpoint_lat.xx and ckpoint_rng.xx corresponding to StartingTrajectory will be loaded.")
                    ("Thermalizations", po::value<int>(&hmc_para.Thermalizations)->default_value(0), "Number of trajectories without Metropolis test.")
                    ("Trajectories", po::value<int>(&hmc_para.Trajectories)->default_value(0), "Number of trajectories after those without Metropolis test. p.s. At the moment Metropolis is disabled")
                    ("mdSteps", po::value<int>(&hmc_para.mdSteps)->default_value(20), "Number of MD steps within each trajectory.")
                    ("trajL", po::value<double>(&hmc_para.trajL)->default_value(1.0), "Trajectory length.")
                    ("saveInterval", po::value<int>(&hmc_para.saveInterval)->default_value(50), "Save interval for checker pointers.")
                    ("action", po::value<std::string>(&hmc_para.action)->default_value("Wilson"), "Action name; available choices: Wilson, GF_Wilson, DBW2, GF_DBW2")
                    ("beta", po::value<double>(&hmc_para.beta)->default_value(5.6), "beta")

                    ("TC.step_size", po::value<double>(&hmc_para.tc_para.step_size)->default_value(1.0), "")
                    ("TC.adaptiveErrorTolerance", po::value<double>(&hmc_para.tc_para.adaptiveErrorTolerance)->default_value(2e-6), "")
                    ("TC.maxTau", po::value<double>(&hmc_para.tc_para.maxTau)->default_value(3.0), "")
                    ("TC.TrajectoryStart", po::value<int>(&hmc_para.tc_para.TrajectoryStart)->default_value(20))
                    ("TC.TrajectoryInterval", po::value<int>(&hmc_para.tc_para.TrajectoryInterval)->default_value(1))
                    ("TC.topoChargeOutFile", po::value<std::string>(&hmc_para.tc_para.topoChargeOutFile)->default_value("topoCharge.txt"))
                    ("TC.saveSmearField", po::value<bool>(&hmc_para.tc_para.saveSmearField)->default_value(false))
                    ("TC.smearFieldFilePrefix", po::value<std::string>(&hmc_para.tc_para.smearFieldFilePrefix)->default_value("ckpoint_lat_smear"))
                    ;

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc).allow_unregistered().run(), vm); // allow additional command line options
  po::store(po::parse_config_file<char>("hmc.ini", desc), vm);
  po::notify(vm);


  // std::cout << "HMC_MOMENTUM_DENOMINATOR: " << HMC_MOMENTUM_DENOMINATOR << std::endl;
// #ifndef CPS_MD_TIME
//   std::cout << "zyd Warning: there is a discrepancy in evolution time between cps and old version Grid." << std::endl;
//   std::cout << "In terms of cps, your trajectory length is " <<  hmc_para.trajL << std::endl;
//   hmc_para.trajL = hmc_para.trajL * std::sqrt(2);
//   std::cout << "In terms of old version Grid, your trajectory length is " <<  hmc_para.trajL << std::endl;
// #endif

  if(vm.count("help")) {
    std::cout << desc << std::endl;
    exit(0);
  }

}



}}
