#include <Grid/Grid.h>
#include "observable.h"
#include "init.h"



namespace Grid {
namespace QCD {

void run_hmc(const HMC_PARA &hmc_para) {

  typedef GenericHMCRunner<LeapFrog> HMCWrapper;
  // typedef GenericHMCRunner<ForceGradient> HMCWrapper;
  HMCWrapper TheHMC;

  TheHMC.Resources.AddFourDimGrid("gauge");

  // Checkpointer definition
  CheckpointerParameters CPparams;  
  CPparams.config_prefix = "ckpoint_lat";
  CPparams.rng_prefix = "ckpoint_rng";
  CPparams.saveInterval = hmc_para.saveInterval;
  CPparams.format = "IEEE64BIG";
  TheHMC.Resources.LoadNerscCheckpointer(CPparams);

  RNGModuleParameters RNGpar;
  RNGpar.serial_seeds = "1 2 3 4 5";
  RNGpar.parallel_seeds = "6 7 8 9 10";
  TheHMC.Resources.SetRNGSeeds(RNGpar);

  // Construct observables
  typedef PlaquetteMod<HMCWrapper::ImplPolicy> PlaqObs;
  TheHMC.Resources.AddObservable<PlaqObs>();
  typedef LinkTraceMod<HMCWrapper::ImplPolicy> LTObs;
  TheHMC.Resources.AddObservable<LTObs>();
  typedef MyTCMod<HMCWrapper::ImplPolicy> QObs; 
  TheHMC.Resources.AddObservable<QObs>(hmc_para.tc_para);

  /////////////////////////////////////////////////////////////

  WilsonGaugeActionR Wilson_action(hmc_para.beta);
  DBW2GaugeAction<PeriodicGimplR> DBW2_action(hmc_para.beta);

  ActionLevel<HMCWrapper::Field> Level1(1);
  if(hmc_para.action == "Wilson"){
    Level1.push_back(&Wilson_action);
  }
  else if(hmc_para.action == "DBW2"){
    Level1.push_back(&DBW2_action);
  }
  else {
    std::cout << "Action not available" << std::endl;
    return;
  }
  TheHMC.TheAction.push_back(Level1);

  // HMC
  TheHMC.Parameters.NoMetropolisUntil = hmc_para.Thermalizations;
  TheHMC.Parameters.Trajectories = hmc_para.Trajectories;
  TheHMC.Parameters.MD.MDsteps = hmc_para.mdSteps;
  TheHMC.Parameters.MD.trajL   = hmc_para.trajL;
  TheHMC.Parameters.StartingType = hmc_para.StartingType;
  TheHMC.Parameters.StartTrajectory = hmc_para.StartingTrajectory;

  std::cout << hmc_para << std::endl;

  // TheHMC.ReadCommandLine(argc, argv); // these can be parameters from file
  TheHMC.Run();  // no smearing
}

}}

using namespace std;
using namespace Grid;
using namespace Grid::QCD;

int main(int argc, char **argv) {

  Grid_init(&argc, &argv);
  GridLogLayout();

  HMC_PARA hmc_para;
  init(argc, argv, hmc_para);

  run_hmc(hmc_para);


  Grid_finalize();

} // main
