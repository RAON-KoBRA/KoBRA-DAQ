//#include "Inspect_RawNew.h"
//#include "Inspect_Detector.h"
//#include "TofAnalysis.h"
//#include "FiberAnalysis.h"
//#include "ChamberAnalysis.h"


#include "DefParameters.h"

#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "TBranch.h"
#include "TString.h"
#include "TClonesArray.h"
#include <cstdlib>
#include "TChain.h"
#include "TFile.h"


//#include "THyphiEvent_RawNew.h"
#include "THyphiEvent_TOF.h"
#include "THyphiEvent_Fiber.h"
#include "THyphiEvent_Chamber.h"
#include "THyphiEvent_Phys.h"




#include <getopt.h>
#include <cstdlib>
#include "Riostream.h"
#include "TString.h"
#include "TObjString.h"
#include "TRandom2.h"
#include "TMath.h"

#include "MathematicalTools.h"

// Type of the analysis 

#define Lambda
//#define Hypernuclei
//#define NLambda

//#define HypernuclearTrigger
#define Reaction3Trigger


//#define MassScan

#define TofPlusEnergyHisto

#define Normal
//#define Mixed

//#define Chamber


#define Beam


//#define WithTR0
#define WithoutTR0


TH2F  *h_xy_target;
TH2F  *h_xy_magnet;

TH1F  *h_dis_TR0;
TH1F  *h_dis_tracks;
TH1F  *h_z_sec;

TH1F  *h_z_sec_selected;
TH1F  *h_z_sec_selected_bg;
TH1F  *h_z_sec_selected_sub;

TH1F  *h_energy_initial;
TH1F  *h_energy_initial_bg;
TH1F  *h_energy_initial_sub;

TH2F  *h_xy_magnet_primary;
TH2F  *h_xy_magnet_secondary;


TH2F  *h_xy_tofplus;
TH2F  *h_xy_tofplus_bar;
TH2F  *h_xy_aladin;

TH1F  *h_invmass;

#ifdef MassScan
TH1F  *h_invmass_mass_scan[31];
#endif

TH1F  *h_res_y_tofplus;
TH1F  *h_res_y_aladin;

TH1F  *h_res_y_tofplus_selected;
TH1F  *h_res_y_aladin_selected;

TH1F  *h_angle;
TH1F  *h_n_sec;

#ifdef TofPlusEnergyHisto
TH1F *h_energy_tofplus[32];
#endif

TH2F  *h_invmass_angle;

TH2F  *h_pp_ppi;
TH2F  *h_pp_ppi_mf;

TH1F  *h_dif_pmf;
TH1F  *h_cos_bb;

TH2F  *h_cosbb_difp;

TH1F  *h_n_selected;
TH2F  *h_n_selected_invmass;

TH1F *h_time_fiber;
TH1F *h_time_chamber;

TH2F  *h_xy_princess;
TH2F  *h_xy_queen;

TH1F *h_chi2;

TH1F *h_n_tr;

TH2F  *h_n_tofstart_n_selected;

TH1F *h_tofplus_time;
TH1F *h_tofplus_cluster_energy;
TH1F *h_aladin_tdc;

TH1F *h_TOF_tofplus;
TH2F *h_x_TOF_tofplus;


TH1F *h_energy_beam;

TH1F *h_tofstart_time;

TH1F *h_decay;
TH1F *h_decay_bg;
TH1F *h_decay_sub;

TH2F *h_pid_tof;

void WriteHisto();

void exit_magnet_primary(double *center, double rho, double *point1, double *point2, double *point_exit, double &slope);
void exit_magnet_secondary(double *center, double rho, double *point1, double *point2, double *point_exit, double &slope);


double GetRho(double x_TR1, double z_TR1, double x_TR2, double z_TR2, double x_TOF, double z_TOF);

double corner1_angle[3];
double corner2_angle[3];
double corner3_angle[3];
double corner4_angle[3];

TH2F  *h_nx_TR0;
TH2F  *h_ny_TR0;
TH2F  *h_nx_TR1;
TH2F  *h_ny_TR1;
TH2F  *h_nx_TR2;
TH2F  *h_ny_TR2;

TH2F  *h_xy_TR0;
TH2F  *h_xy_TR1;
TH2F  *h_xy_TR2;

TH1F *h_tdif_TR0;
TH1F *h_tdif_TR1;
TH1F *h_tdif_TR2;


TH1F  *h_width_TR0x;
TH1F  *h_width_TR0y;
TH1F  *h_width_TR1x;
TH1F  *h_width_TR1y;
TH1F  *h_width_TR2x;
TH1F  *h_width_TR2y;

TH1F  *h_cluster_size_TR0x;
TH1F  *h_cluster_size_TR0y;
TH1F  *h_cluster_size_TR1x;
TH1F  *h_cluster_size_TR1y;
TH1F  *h_cluster_size_TR2x;
TH1F  *h_cluster_size_TR2y;

TH1F *h_QDC_TR0x;
TH1F *h_QDC_TR0y;




int main() 
{


  //  TFile *outfile = new TFile("/misc/tsaito/test.root","RECREATE");

  h_xy_target = new TH2F("xy_target","xy_target",300,-300,300,300,-300,300);
  h_xy_magnet = new TH2F("xy_magnet","xy_magnet",200,-1000,1000,200,-500,500);
  h_dis_TR0 = new TH1F("dis_TR0","dis_TR0",500,0.0,100);
  h_dis_tracks = new TH1F("dis_tracks","dis_tracks",500,0.0,100);
  h_z_sec = new TH1F("z_sec","z_sec",1000,-1000.0,1000);

  h_z_sec_selected = new TH1F("z_sec_selected","z_sec_selected",100,0.0,1000);
  h_z_sec_selected_bg = new TH1F("z_sec_selected_bg","z_sec_selected_bg",100,0.0,1000);
  h_z_sec_selected_sub = new TH1F("z_sec_selected_sub","z_sec_selected_sub",100,0.0,1000);

  h_energy_initial = new TH1F("energy_initial","energy_initial",300,0.0,3.0);
  h_energy_initial_bg = new TH1F("energy_initial_bg","energy_initial_bg",300,0.0,3.0);
  h_energy_initial_sub = new TH1F("energy_initial_sub","energy_initial_sub",300,0.0,3.0);

  h_xy_magnet_primary = new TH2F("xy_magnet_primary","xy_magnet_primary",200,-1000,1000,200,-500,500);
  h_xy_magnet_secondary = new TH2F("xy_magnet_secondary","xy_magnet_secondary",200,-1000,1000,200,-500,500);


  h_xy_tofplus = new TH2F("xy_tofplus","xy_tofplus",250,-1000,1000,200,-1000,1000);
  h_xy_tofplus_bar = new TH2F("xy_tofplus_bar","xy_tofplus_bar",250,-1000,1000,200,-1000,1000);
  h_xy_aladin = new TH2F("xy_aladin","xy_aladin",500,0,3000,200,-1000,1000);

  h_invmass = new TH1F("invmass","invmass",50000,0.0,5.0);

  h_res_y_tofplus = new TH1F("res_y_tofplus","res_y_tofplus",1000,0.0,1000.0);
  h_res_y_aladin = new TH1F("res_y_aladin","res_y_aladin",1000,0.0,1000.0);

  h_res_y_tofplus_selected = new TH1F("res_y_tofplus_selected","res_y_tofplus_selected",1000,0.0,1000.0);
  h_res_y_aladin_selected = new TH1F("res_y_aladin_selected","res_y_aladin_selected",1000,0.0,1000.0);

  h_angle = new TH1F("angle","angle",1000,0.0,100.0);
  h_n_sec = new TH1F("n_sec","n_sec",100,0.0,100.0);

  char txt[100];

#ifdef TofPlusEnergyHisto
  for(int i=0;i<32;i++)
    {
      sprintf(txt,"energy_tofplus_%i.root",i+1);
      if(i+1<10) sprintf(txt,"energy_tofplus_0%i.root",i+1);
      h_energy_tofplus[i] = new TH1F(txt,txt,4000,0.0,4000);
    }
#endif

  sprintf(txt,"invmass_angle");
  h_invmass_angle = new TH2F(txt,txt,400,1.0,1.4,100,0.0,20.0);

  sprintf(txt,"pp_ppi");
  h_pp_ppi = new TH2F(txt,txt,200,0.0,10.0,200,0.0,10.0);

  sprintf(txt,"pp_ppi_mf");
  h_pp_ppi_mf = new TH2F(txt,txt,200,0.0,10.0,200,0.0,10.0);

  sprintf(txt,"dif_pmf");
  h_dif_pmf = new TH1F(txt,txt,1000,0.0,0.5);

  sprintf(txt,"cos_bb");
  h_cos_bb = new TH1F(txt,txt,200,-1.0,1.0);

  sprintf(txt,"cosbb_difp");
  h_cosbb_difp = new TH2F(txt,txt,200,-1.0,1.0,500,0.0,11.0);

  sprintf(txt,"n_selected");
  h_n_selected = new TH1F(txt,txt,500,0.0,500.0);

  sprintf(txt,"n_selected_invmass");
  h_n_selected_invmass = new TH2F(txt,txt,500,0.0,500,500,1.0,2.0);

  sprintf(txt,"chi2");
  h_chi2 = new TH1F(txt,txt,5000,0.0,500.0);

  sprintf(txt,"decay");
  h_decay = new TH1F(txt,txt,1000,0.0,10.0);
  sprintf(txt,"decay_bg");
  h_decay_bg = new TH1F(txt,txt,1000,0.0,10.0);
  sprintf(txt,"decay_sub");
  h_decay_sub = new TH1F(txt,txt,1000,0.0,10.0);



#ifdef MassScan
  sprintf(txt,"invmass_mass_scan_1.100");
  h_invmass_mass_scan[0] = new TH1F(txt,txt,50000,0.0,5.0);
  sprintf(txt,"invmass_mass_scan_1.103");
  h_invmass_mass_scan[1] = new TH1F(txt,txt,50000,0.0,5.0);
  sprintf(txt,"invmass_mass_scan_1.106");
  h_invmass_mass_scan[2] = new TH1F(txt,txt,50000,0.0,5.0);
  sprintf(txt,"invmass_mass_scan_1.109");
  h_invmass_mass_scan[3] = new TH1F(txt,txt,50000,0.0,5.0);
  sprintf(txt,"invmass_mass_scan_1.112");
  h_invmass_mass_scan[4] = new TH1F(txt,txt,50000,0.0,5.0);
  sprintf(txt,"invmass_mass_scan_1.115");
  h_invmass_mass_scan[5] = new TH1F(txt,txt,50000,0.0,5.0);
  sprintf(txt,"invmass_mass_scan_1.118");
  h_invmass_mass_scan[6] = new TH1F(txt,txt,50000,0.0,5.0);
  sprintf(txt,"invmass_mass_scan_1.121");
  h_invmass_mass_scan[7] = new TH1F(txt,txt,50000,0.0,5.0);
  sprintf(txt,"invmass_mass_scan_1.124");
  h_invmass_mass_scan[8] = new TH1F(txt,txt,50000,0.0,5.0);
  sprintf(txt,"invmass_mass_scan_1.127");
  h_invmass_mass_scan[9] = new TH1F(txt,txt,50000,0.0,5.0);
  sprintf(txt,"invmass_mass_scan_1.130");
  h_invmass_mass_scan[10] = new TH1F(txt,txt,50000,0.0,5.0);
  sprintf(txt,"invmass_mass_scan_1.133");
  h_invmass_mass_scan[11] = new TH1F(txt,txt,50000,0.0,5.0);
  sprintf(txt,"invmass_mass_scan_1.136");
  h_invmass_mass_scan[12] = new TH1F(txt,txt,50000,0.0,5.0);
  sprintf(txt,"invmass_mass_scan_1.139");
  h_invmass_mass_scan[13] = new TH1F(txt,txt,50000,0.0,5.0);
  sprintf(txt,"invmass_mass_scan_1.142");
  h_invmass_mass_scan[14] = new TH1F(txt,txt,50000,0.0,5.0);
  sprintf(txt,"invmass_mass_scan_1.145");
  h_invmass_mass_scan[15] = new TH1F(txt,txt,50000,0.0,5.0);
  sprintf(txt,"invmass_mass_scan_1.148");
  h_invmass_mass_scan[16] = new TH1F(txt,txt,50000,0.0,5.0);
  sprintf(txt,"invmass_mass_scan_1.151");
  h_invmass_mass_scan[17] = new TH1F(txt,txt,50000,0.0,5.0);
  sprintf(txt,"invmass_mass_scan_1.154");
  h_invmass_mass_scan[18] = new TH1F(txt,txt,50000,0.0,5.0);
  sprintf(txt,"invmass_mass_scan_1.157");
  h_invmass_mass_scan[19] = new TH1F(txt,txt,50000,0.0,5.0);
  sprintf(txt,"invmass_mass_scan_1.160");
  h_invmass_mass_scan[20] = new TH1F(txt,txt,50000,0.0,5.0);
  sprintf(txt,"invmass_mass_scan_1.163");
  h_invmass_mass_scan[21] = new TH1F(txt,txt,50000,0.0,5.0);
  sprintf(txt,"invmass_mass_scan_1.166");
  h_invmass_mass_scan[22] = new TH1F(txt,txt,50000,0.0,5.0);
  sprintf(txt,"invmass_mass_scan_1.169");
  h_invmass_mass_scan[23] = new TH1F(txt,txt,50000,0.0,5.0);
  sprintf(txt,"invmass_mass_scan_1.172");
  h_invmass_mass_scan[24] = new TH1F(txt,txt,50000,0.0,5.0);
  sprintf(txt,"invmass_mass_scan_1.175");
  h_invmass_mass_scan[25] = new TH1F(txt,txt,50000,0.0,5.0);
  sprintf(txt,"invmass_mass_scan_1.178");
  h_invmass_mass_scan[26] = new TH1F(txt,txt,50000,0.0,5.0);
  sprintf(txt,"invmass_mass_scan_1.181");
  h_invmass_mass_scan[27] = new TH1F(txt,txt,50000,0.0,5.0);
  sprintf(txt,"invmass_mass_scan_1.184");
  h_invmass_mass_scan[28] = new TH1F(txt,txt,50000,0.0,5.0);
  sprintf(txt,"invmass_mass_scan_1.187");
  h_invmass_mass_scan[29] = new TH1F(txt,txt,50000,0.0,5.0);
  sprintf(txt,"invmass_mass_scan_1.190");
  h_invmass_mass_scan[30] = new TH1F(txt,txt,50000,0.0,5.0);
#endif

  sprintf(txt,"time_fiber");
  h_time_fiber = new TH1F(txt,txt,100000,0.0,100000.0);
  sprintf(txt,"time_chamber");
  h_time_chamber = new TH1F(txt,txt,100000,0.0,100000.0);


  sprintf(txt,"xy_princess");
  h_xy_princess = new TH2F(txt,txt,500,-2000.0,2000,500,-1000.0,1000.0);

  sprintf(txt,"xy_queen");
  h_xy_queen = new TH2F(txt,txt,500,-2000.0,2000,500,-1000.0,1000.0);

  sprintf(txt,"n_tr");
  h_n_tr = new TH1F(txt,txt,6000,0.0,6000.0);

  sprintf(txt,"nx_TR0");
  h_nx_TR0 = new TH2F(txt,txt,100,0.0,100,500,-200.0,200.0);
  sprintf(txt,"ny_TR0");
  h_ny_TR0 = new TH2F(txt,txt,100,0.0,100,500,-200.0,200.0);
  sprintf(txt,"nx_TR1");
  h_nx_TR1 = new TH2F(txt,txt,100,0.0,100,500,-200.0,200.0);
  sprintf(txt,"ny_TR1");
  h_ny_TR1 = new TH2F(txt,txt,100,0.0,100,500,-200.0,200.0);
  sprintf(txt,"nx_TR2");
  h_nx_TR2 = new TH2F(txt,txt,100,0.0,100,500,-200.0,200.0);
  sprintf(txt,"ny_TR2");
  h_ny_TR2 = new TH2F(txt,txt,100,0.0,100,500,-200.0,200.0);

  sprintf(txt,"xy_TR0");
  h_xy_TR0 = new TH2F(txt,txt,500,-200.0,200,500,-200.0,200.0);
  sprintf(txt,"xy_TR1");
  h_xy_TR1 = new TH2F(txt,txt,500,-200.0,200,500,-200.0,200.0);
  sprintf(txt,"xy_TR2");
  h_xy_TR2 = new TH2F(txt,txt,500,-200.0,200,500,-200.0,200.0);


  sprintf(txt,"n_tofstart_n_selected");
  h_n_tofstart_n_selected = new TH2F(txt,txt,10,0.0,10,500,0.0,500.0);

  sprintf(txt,"tofplus_time");
  h_tofplus_time = new TH1F(txt,txt,1000,-500.0,500.0);

  sprintf(txt,"tofplus_cluster_energy");
  h_tofplus_cluster_energy = new TH1F(txt,txt,4000,0.0,4000.0);

  sprintf(txt,"aladin_tdc");
  h_aladin_tdc = new TH1F(txt,txt,4000,0.0,4000.0);

  sprintf(txt,"TOF_tpfplus");
  h_TOF_tofplus = new TH1F(txt,txt,2000,-100.0,100.0);

  sprintf(txt,"x_TOF_tpfplus");
  h_x_TOF_tofplus = new TH2F(txt,txt,250,-1000,1000,200,-50.0,50.0);

  sprintf(txt,"energy_beam");
  h_energy_beam = new TH1F(txt,txt,5000,0.0,50.0);

  sprintf(txt,"tofstart_time");
  h_tofstart_time = new TH1F(txt,txt,1000,-500.0,500.0);

  sprintf(txt,"tdif_TR0");
  h_tdif_TR0 = new TH1F(txt,txt,400,-200.0,200.0);

  sprintf(txt,"tdif_TR1");
  h_tdif_TR1 = new TH1F(txt,txt,400,-200.0,200.0);

  sprintf(txt,"tdif_TR2");
  h_tdif_TR2 = new TH1F(txt,txt,400,-200.0,200.0);

  sprintf(txt,"pid_tpf");
  h_pid_tof = new TH2F(txt,txt,10,0,10,600,-30.0,30.0);






  sprintf(txt,"width_TR0x");
  h_width_TR0x = new TH1F(txt,txt,200,-100,100);
  sprintf(txt,"width_TR0y");
  h_width_TR0y = new TH1F(txt,txt,200,-100,100);
  sprintf(txt,"width_TR1x");
  h_width_TR1x = new TH1F(txt,txt,200,-100,100);
  sprintf(txt,"width_TR1y");
  h_width_TR1y = new TH1F(txt,txt,200,-100,100);
  sprintf(txt,"width_TR2x");
  h_width_TR2x = new TH1F(txt,txt,200,-100,100);
  sprintf(txt,"width_TR2y");
  h_width_TR2y = new TH1F(txt,txt,200,-100,100);

  sprintf(txt,"cluster_size_TR0x");
  h_cluster_size_TR0x = new TH1F(txt,txt,500,0,500);
  sprintf(txt,"cluster_size_TR0y");
  h_cluster_size_TR0y = new TH1F(txt,txt,500,0,500);
  sprintf(txt,"cluster_size_TR1x");
  h_cluster_size_TR1x = new TH1F(txt,txt,500,0,500);
  sprintf(txt,"cluster_size_TR1y");
  h_cluster_size_TR1y = new TH1F(txt,txt,500,0,500);
  sprintf(txt,"cluster_size_TR2x");
  h_cluster_size_TR2x = new TH1F(txt,txt,500,0,500);
  sprintf(txt,"cluster_size_TR2y");
  h_cluster_size_TR2y = new TH1F(txt,txt,500,0,500);




  sprintf(txt,"QDC_TR0x");
  h_QDC_TR0x = new TH1F(txt,txt,4000,0,4000);
  sprintf(txt,"QDC_TR0y");
  h_QDC_TR0y = new TH1F(txt,txt,4000,0,4000);





  // magnet four corner calculation
  

  double B = 1.18; // T
	      
  //  double aladin_length = 1.4; // m
  //  double aladin_length = 1.6; // m

  double field_length_upstream = 0.75;
  double field_length_downstream = 0.80;



  double corner1[3] = {1.56/2.0*1000,0.0,-field_length_upstream*1000};
  double corner2[3] = {-1.56/2.0*1000,0.0,-field_length_upstream*1000};
  double corner3[3] = {1.56/2.0*1000,0.0,+field_length_downstream*1000};
  double corner4[3] = {-1.56/2.0*1000,0.0,+field_length_downstream*1000};
  


  //  double angle = -5.6/180.0*3.1419;
  double angle = -7.6/180.0*3.1419;
  
  corner1_angle[0] = corner1[0]*cos(angle) + corner1[2]*sin(angle);
  corner1_angle[1] = corner1[1];
  corner1_angle[2] = -corner1[0]*sin(angle) + corner1[2]*cos(angle) + 2380.0;
  
  corner2_angle[0] = corner2[0]*cos(angle) + corner2[2]*sin(angle);
  corner2_angle[1] = corner2[1];
  corner2_angle[2] = -corner2[0]*sin(angle) + corner2[2]*cos(angle) + 2380.0;
  
  corner3_angle[0] = corner3[0]*cos(angle) + corner3[2]*sin(angle);
  corner3_angle[1] = corner3[1];
  corner3_angle[2] = -corner3[0]*sin(angle) + corner3[2]*cos(angle) + 2380.0;
  
  corner4_angle[0] = corner4[0]*cos(angle) + corner4[2]*sin(angle);
  corner4_angle[1] = corner4[1];
  corner4_angle[2] = -corner4[0]*sin(angle) + corner4[2]*cos(angle) + 2380.0;
  
  
  
  cout << corner1[0] << " " << corner1[1] << " " << corner1[2] +2380<< endl;
  cout << corner1_angle[0] << " " << corner1_angle[1] << " " << corner1_angle[2] << endl;
  cout << corner2[0] << " " << corner2[1] << " " << corner2[2] +2380<< endl;
  cout << corner2_angle[0] << " " << corner2_angle[1] << " " << corner2_angle[2] << endl;
  cout << corner3[0] << " " << corner3[1] << " " << corner3[2] +2380<< endl;
  cout << corner3_angle[0] << " " << corner3_angle[1] << " " << corner3_angle[2] << endl;
  cout << corner4[0] << " " << corner4[1] << " " << corner4[2] +2380<< endl;
  cout << corner4_angle[0] << " " << corner4_angle[1] << " " << corner4_angle[2] << endl;
  
		
  MT_Line line_magnet1;
  line_magnet1.From_Points(corner1_angle,corner2_angle);

  MT_Line line_magnet2;
  line_magnet2.From_Points(corner3_angle,corner4_angle);








  /////////


  THyphiEvent_Fiber *event = 0;

  THyphiFiberClusterChannel cluster;

  THyphiEvent_TOF *event_Tof = 0;

  THyphiTofBar tofbar;

  THyphiEvent_Chamber *event_chamber = 0;

  THyphiChamberChannel chamber;

  THyphiEvent_Phys *event_phys = 0;

  THyphiHit hit;



  TChain *chain_fiber = new TChain("EventsFiber");

  chain_fiber->Add("/data.local2/HypHI_trees/minbias_highbeam_1119_EventTree_Raw_off_Fiber.root");        
  




  TChain *chain_phys = new TChain("EventsPhys");
  TChain *chain_tof = new TChain("EventsTof");

  
  chain_tof->Add("/data.local2/HypHI_trees/minbias_highbeam_1119_EventTree_Raw_off_TOF.root");            


  chain_phys->Add("/data.local2/HypHI_trees/minbias_highbeam_1119_EventTree_Raw_off_TOF.root");            


#ifdef Chamber
  TChain *chain_chamber = new TChain("EventsChamber");


  chain_chamber->Add("/data.local2/HypHI_trees/minbias_highbeam_1119_EventTree_Raw_off_Chamber.root");            
#endif
  //  chain_fiber->Show(1);
  //  chain_tof->Show(1);
  /*  
  chain_tof->Print();
  cout<<";;;;;;;;;;;;;;;;;;;;;"<<endl;
  chain_fiber->Print();
  cout<<";;;;;;;;;;;;;;;;;;;;;"<<endl;
  chain_chamber->Print();
  */



  TChain *t;
  if(chain_fiber->GetEntries() != chain_tof->GetEntries()) exit(1);

  if(chain_fiber->GetEntries()!= chain_phys->GetEntries() ) exit(1);


  t = chain_fiber;
  t->AddFriend(chain_tof);
  t->AddFriend(chain_phys);
#ifdef Chamber
  t->AddFriend(chain_chamber);
#endif

  TRandom2 rand;





  t->SetBranchAddress("THyphiEvent_Fiber",&event);
  t->SetBranchAddress("THyphiEvent_TOF",&event_Tof);
  t->SetBranchAddress("THyphiEvent_Phys",&event_phys);

#ifdef Chamber
  t->SetBranchAddress("THyphiEvent_Chamber",&event_chamber);
#endif

  Long64_t nentries = (Int_t)t->GetEntries();


  cout << "Total event number = " << nentries << endl;

  int n_fiber_100=0;
  int n_chamber_100_x=0;
  int n_chamber_100_u=0;
  int n_chamber_100_v=0;
  int n_timer=0;


  int n_filled=0;
  int n_attention=0;

#ifdef Normal
  for(Long64_t neve=0;neve<nentries;neve++)
#endif
#ifdef Mixed
  for(Long64_t neve=0;neve<nentries-2;neve++)
#endif
    //  for(int neve=0;neve<10000;neve++)
    {

      if((neve%25000)==0) 
	{
	  cout << "@@@@@@@@@@@@@@@@@@@@@@@@@ " << neve+1 << " events done! $$$$$$$$$$$$$$$$$$$$$$$$  " << n_attention << " / " << n_filled << endl;
	  WriteHisto();
	}

      t->GetEntry(neve);
      

      //Tof data

      int n_tofplus = event_phys->n_TofPlus;
      int n_tofplus_bar = event_Tof->n_TOFPlusBar;
      int n_aladin = event_Tof->n_TOFAladinBar;
      int n_tofstart = event_phys->n_TofStart;





      double cos_aladin = cos(20.0/180.0*3.14159);


      vector<double> x_tofplus;
      vector<double> y_tofplus;
      vector<double> z_tofplus;
      vector<double> x_aladin;
      vector<double> y_aladin;
      vector<double> z_aladin;

      vector<double> dis_y_tofplus;
      vector<double> dis_y_aladin;

      vector<double> energy_tofplus;
      vector<int> id_tofplus;

      vector<double> time_tofstart;


      vector<double> x_tofplus_9;
      vector<double> y_tofplus_9;
      vector<double> z_tofplus_9;
      vector<int> id_tofplus_9;


      int temp_n_tofstart = 0;

      /*
      
      for(int i=0;i<n_tofstart;i++) // TOF_start
	{
	  THyphiHit *temp = static_cast<THyphiHit*>(event_phys->TofStart-> At(i));
	  h_tofstart_time->Fill(temp->t);

	  //	  if(temp->t>-12.0 && temp->t<4.0)
	  if(temp->t>5 && temp->t<15.0)  //reaction3
	    {
	      time_tofstart.push_back(temp->t);
	      temp_n_tofstart++;
	    }
	}

      n_tofstart = temp_n_tofstart;
      

      if(n_tofstart==0 || n_tofstart>2) continue;

      double tof_ref;

      if(n_tofstart==1)
	{
	  tof_ref = time_tofstart[0];
	}
  
      if(n_tofstart==2)
	{
	  if(time_tofstart[0]<time_tofstart[1]) 
	    {
	  
	      tof_ref=time_tofstart[0];
	    }

	  else
	    {
	       
	      tof_ref=time_tofstart[1];
	    }
	}
      */
      
      double offset_tof = 20.0;
      //      double offset_tof = -25.0;





      int temp_n_tofplus = 0;



      for(int i=0;i<n_tofplus;i++) // Analyzing proton 
	{
	  THyphiHit *temp = static_cast<THyphiHit*>(event_phys->TofPlus-> At(i));
	  double xxx = temp->x + offset_tof+15.0/2.0*(rand.Rndm()-0.5);
	  
	  double yyy = temp->y;
	  h_xy_tofplus->Fill(temp->x, yyy);
	  if(xxx>-250.0+offset_tof && xxx<-220.0+offset_tof) yyy = temp->y-150.0; 


	  h_tofplus_time->Fill(temp->t);
	  h_tofplus_cluster_energy->Fill(temp->E);

	  //	  if(temp->t > 5.0 && temp->t < 20.0) 
	  if(temp->t > -3 && temp->t < 8.0) // reaction 3
	    {

	      if(1/*temp->x <= -305.5*/)
		{
		  //		  double tof = temp->t - tof_ref;

		  //		  h_pid_tof->Fill(temp->PID,tof);

		  if(temp->PID==1)
		    {
		      


		      
		      //		      h_TOF_tofplus->Fill(tof);
		      //		      h_x_TOF_tofplus->Fill(xxx,tof);
		      
		      //		      if(tof>8.0 && tof<22.0)
		      //		      if(tof>10.0 && tof<23.0) //reaction 3
		      if(1/*tof>10.0 && tof<25.0*/) //reaction 3
			{
			  x_tofplus.push_back(xxx);
			  y_tofplus.push_back(yyy);
		 	  z_tofplus.push_back(temp->z);
			  id_tofplus.push_back(i);
			  
			  
			  temp_n_tofplus++;
			}
		    }
		}
	    }
	}

      n_tofplus = temp_n_tofplus;





#ifdef Beam

      int temp_n_tofplus_9 = 0;

      n_tofplus = event_phys->n_TofPlus;



      for(int i=0;i<n_tofplus;i++) // Analyzing PID = 9 
	{
	  THyphiHit *temp = static_cast<THyphiHit*>(event_phys->TofPlus-> At(i));
	  double xxx = temp->x + offset_tof+15.0/2.0*(rand.Rndm()-0.5);

	  double yyy = temp->y;
	  if(xxx>-250.0+offset_tof && xxx<-220.0+offset_tof) yyy = temp->y; 


	  if(temp->t > -3.0 && temp->t < 8.0)
	    {
	      if(1/*temp->x <= -305.5*/)
		{
		  //		  cout << temp->PID << endl;
		  if(temp->PID==9)
		    {
		      //		      double tof = temp->t - tof_ref;
		      if(1/*tof>10.0 && tof<25.0*/)
			{



			  x_tofplus_9.push_back(xxx);
			  y_tofplus_9.push_back(yyy);
			  z_tofplus_9.push_back(temp->z);
			  id_tofplus_9.push_back(i);
			  
			  
			  temp_n_tofplus_9++;
			}
		    }
		}
	    }
	}


      //      cout << temp_n_tofplus_9 << endl;
#endif






      for(int i=0;i<n_tofplus_bar;i++) // TOF+ bar information
	{
	  event_Tof->Get_Bar(i,"TOF_Plus",tofbar);

	  h_xy_tofplus_bar->Fill(tofbar.Position_x, tofbar.Position_y);
#ifdef TofPlusEnergyHisto
	  h_energy_tofplus[tofbar.Id_bar-1]->Fill(tofbar.Energy_high);
#endif
	}




  
      
      //      cout << " n_tofplus = " << n_tofplus << endl;

 
      int temp_n_aladin = 0;

      for(int i=0;i<n_aladin;i++) // Analyzing pion side
	{
	  event_Tof->Get_Bar(i,"TOF_Aladin",tofbar);
	  h_aladin_tdc->Fill(tofbar.TDC_b);
	  //	  if(tofbar.TDC_t>1000 && tofbar.TDC_t<2000 && tofbar.TDC_b>700 && tofbar.TDC_b<1600) 
	  if(tofbar.TDC_t>1200 && tofbar.TDC_t<2500 && tofbar.TDC_b>1200 && tofbar.TDC_b<2000) // reaction3 
	    {
	      if(tofbar.Id_bar<80)
		{

		  x_aladin.push_back(tofbar.Position_x + offset_tof + 25.0/2.0*cos_aladin*(rand.Rndm()-0.5));
		  y_aladin.push_back(tofbar.Position_y+100.0);
		  z_aladin.push_back(tofbar.Position_z);
		  //		  cout << i << " " << x_aladin[temp_n_aladin] << endl;
		  temp_n_aladin++;
		}
	      //	      h_xy_aladin->Fill(tofbar.Position_x + offset_tof + 25.0/2.0*cos_aladin*(rand.Rndm()-0.5),tofbar.Position_y+100);
	    }
	}
      n_aladin = temp_n_aladin;
      

      






      //      cout << n_tofplus << " " << n_aladin << " " << n_tofstart << endl;

#ifdef Mixed
      t->GetEntry(neve+1);
      //cout << " Mixed Event" << endl;
#endif

      //Fiber data 

      vector<double> x[3],y[3],z[3];
      vector<int> id_x[3],id_y[3];



      vector<double> time_TR[6];

      vector<double> width[6];

      vector<double> QDC_sum_TR0[2];

      int n[3][2];




      double offset_fiber = 0.0;

      double offset_upstream = 0.0;


      n[0][0] = event-> n_TR0_x;
      n[0][1] = event-> n_TR0_y;
      h_n_tr->Fill(n[0][0]);
      h_n_tr->Fill(n[0][1]+1000);


      for(int i=0;i<n[0][0];i++)
	{
	  event->Get_Channel(i,"TR0_x",cluster);
	  x[0].push_back(1*(cluster.Position + offset_fiber + 0.83*(rand.Rndm()-0.5)));
	  z[0].push_back(cluster.Z+offset_upstream);
	  id_x[0].push_back(i);
	  width[0].push_back(cluster.E);

	  double temp_time=0;
	  int temp_clustersize=0;
	  
	  double sum_QDC=0.0;
	  for(int j=0;j<cluster.TDC_LE.size();j++)
	    {
	      if(cluster.TDC_LE[j]>0.0) 
		{
		  temp_time+=cluster.TDC_LE[j];
		  if(cluster.QDC[j]!=-999) sum_QDC += cluster.QDC[j];
		  temp_clustersize++;
		}
	    }
	  time_TR[0].push_back(temp_time/temp_clustersize);

	  h_nx_TR0->Fill(n[0][0],x[0][i]);

	  h_cluster_size_TR0x->Fill(cluster.TDC_LE.size());

	  QDC_sum_TR0[0].push_back(sum_QDC);

	}

      
      for(int i=0;i<n[0][1];i++)
	{
	  event->Get_Channel(i,"TR0_y",cluster);
	  y[0].push_back(cluster.Position + 0.83*(rand.Rndm()-0.5));
	  z[0].push_back(cluster.Z+offset_upstream);
	  id_y[0].push_back(i);
	  width[1].push_back(cluster.E);

	  double temp_time=0;
	  int temp_clustersize=0;

	  double sum_QDC=0.0;

	  for(int j=0;j<cluster.TDC_LE.size();j++)
	    {
	      if(cluster.TDC_LE[j]>0.0) 
		{
		  temp_time+=cluster.TDC_LE[j];
		  if(cluster.QDC[j]!=-999) sum_QDC += cluster.QDC[j];
		  temp_clustersize++;
		}
	    }
	  time_TR[1].push_back(temp_time/temp_clustersize);

	  h_ny_TR0->Fill(n[0][1],y[0][i]);

	  h_cluster_size_TR0y->Fill(cluster.TDC_LE.size());

	  QDC_sum_TR0[1].push_back(sum_QDC);

	}


      n[1][0] = event-> n_TR1_x;
      n[1][1] = event-> n_TR1_y;
      h_n_tr->Fill(n[1][0]+2000);
      h_n_tr->Fill(n[1][1]+3000);

      for(int i=0;i<n[1][0];i++)
	{
	  event->Get_Channel(i,"TR1_x",cluster);
	  x[1].push_back(1*(cluster.Position + offset_fiber + 0.83*(rand.Rndm()-0.5)));
	  z[1].push_back(cluster.Z+offset_upstream);
	  id_x[1].push_back(i);

	  h_width_TR1x->Fill(cluster.E);
	  width[2].push_back(cluster.E);


	  double temp_time=0;
	  int temp_clustersize=0;
	  for(int j=0;j<cluster.TDC_LE.size();j++)
	    {
	      if(cluster.TDC_LE[j]>0.0) 
		{
		  temp_time+=cluster.TDC_LE[j];
		  temp_clustersize++;
		}
	    }
	  time_TR[2].push_back(temp_time/temp_clustersize);

	  h_nx_TR1->Fill(n[1][0],x[1][i]);

	  h_cluster_size_TR1x->Fill(cluster.TDC_LE.size());


	}



      int n_temp=0;

      for(int i=0;i<n[1][1];i++)
	{
	  
	  event->Get_Channel(i,"TR1_y",cluster);
	  if(cluster.Position>30.0) continue;
	  y[1].push_back(cluster.Position + 0.83*(rand.Rndm()-0.5));
	  z[1].push_back(cluster.Z+offset_upstream);
	  id_y[1].push_back(i);

	  h_width_TR1y->Fill(cluster.E);

	  width[3].push_back(cluster.E);

	  double temp_time=0;
	  int temp_clustersize=0;
	  for(int j=0;j<cluster.TDC_LE.size();j++)
	    {
	      if(cluster.TDC_LE[j]>0.0) 
		{
		  temp_time+=cluster.TDC_LE[j];
		  temp_clustersize++;
		}
	    }
	  time_TR[3].push_back(temp_time/temp_clustersize);

	  h_ny_TR1->Fill(n[1][1],y[1][i]);

	  h_cluster_size_TR1y->Fill(cluster.TDC_LE.size());



	  n_temp++;
	}

      n[1][1] = n_temp;

      /*
#ifdef Mixed
      t->GetEntry(neve+2);
      //cout << " Mixed Event" << endl;
#endif
      */

      n[2][0] = event-> n_TR2_x;
      n[2][1] = event-> n_TR2_y;
      h_n_tr->Fill(n[2][0]+4000);
      h_n_tr->Fill(n[2][1]+5000);

      n_temp=0;
      for(int i=0;i<n[2][0];i++)
	{
	  event->Get_Channel(i,"TR2_x",cluster);
	  if(cluster.Position>55.0 && cluster.Position<60.0) continue;
	  
	  x[2].push_back(1*(cluster.Position + offset_fiber + 0.83*(rand.Rndm()-0.5)));
	  z[2].push_back(cluster.Z+offset_upstream);
	  id_x[2].push_back(i);

	  h_width_TR2x->Fill(cluster.E);

	  width[4].push_back(cluster.E);


	  double temp_time=0;
	  int temp_clustersize=0;
	  for(int j=0;j<cluster.TDC_LE.size();j++)
	    {
	      if(cluster.TDC_LE[j]>0.0) 
		{
		  temp_time+=cluster.TDC_LE[j];
		  temp_clustersize++;
		}
	    }
	  time_TR[4].push_back(temp_time/temp_clustersize);

	  h_nx_TR2->Fill(n[2][0],x[2][i]);


	  h_cluster_size_TR2x->Fill(cluster.TDC_LE.size());



	  n_temp++;
	}
      n[2][0] = n_temp;

      for(int i=0;i<n[2][1];i++)
	{
	  event->Get_Channel(i,"TR2_y",cluster);
	  y[2].push_back(cluster.Position + 0.83*(rand.Rndm()-0.5));
	  z[2].push_back(cluster.Z+offset_upstream);
	  id_y[2].push_back(i);

	  h_width_TR2y->Fill(cluster.E);

	  width[5].push_back(cluster.E);


	  double temp_time=0;
	  int temp_clustersize=0;
	  for(int j=0;j<cluster.TDC_LE.size();j++)
	    {
	      if(cluster.TDC_LE[j]>0.0) 
		{
		  temp_time+=cluster.TDC_LE[j];
		  temp_clustersize++;
		}
	    }
	  time_TR[5].push_back(temp_time/temp_clustersize);

	  h_ny_TR2->Fill(n[2][1],y[2][i]);

	  h_cluster_size_TR2y->Fill(cluster.TDC_LE.size());


	}







      for(int i=0;i<n[0][0];i++)
	{
	  for(int j=0;j<n[0][1];j++)
	    {
	      h_xy_TR0->Fill(x[0][i],y[0][j]);
	      h_tdif_TR0->Fill(time_TR[0][i]-time_TR[1][j]);
	    }
	}
      
      for(int i=0;i<n[1][0];i++)
	{
	  for(int j=0;j<n[1][1];j++)
	    {
	      h_xy_TR1->Fill(x[1][i],y[1][j]);
	      h_tdif_TR1->Fill(time_TR[2][i]-time_TR[3][j]);
	    }
	}
      
      for(int i=0;i<n[2][0];i++)
	{
	  for(int j=0;j<n[2][1];j++)
	    {
	      h_xy_TR2->Fill(x[2][i],y[2][j]);
	      h_tdif_TR2->Fill(time_TR[4][i]-time_TR[5][j]);
	    }
	}
      









      //      cout << n[0][0] << " " << n[0][1] << " " << n[1][0] << " " << n[1][1] << " " << n[2][0] << " " << n[2][1] << endl; 



      //      cout << "yeah 1" << endl;

#ifdef Chamber

      // Chamber data

      int n_princess = event_chamber->n_CbM;
      int n_queen = event_chamber->n_CaM;


      vector<double> x_princess;
      vector<double> y_princess;

      int num_chamber_hit = event_chamber->n_CbM;

      for(int i=0;i< num_chamber_hit;i++)
	{
	  // if(temp)temp->Clear();
	  THyphiChamberChannel *temp =
	    static_cast<THyphiChamberChannel*>(event_chamber->CbM-> At(i));
	  //	  event_chamber->GetChannel(i,"CbM",chamber);

	  x_princess.push_back(temp->X);
	  y_princess.push_back(temp->Y);

	  h_xy_princess->Fill(temp->X, temp->Y);

	  //	  cout << x_princess[i] << " " << y_princess[i] << endl;

	}



      vector<double> x_queen;
      vector<double> y_queen;

      for(int i=0;i< n_queen;i++)
	{
	  // if(temp)temp->Clear();
	  THyphiChamberChannel *temp =
	    static_cast<THyphiChamberChannel*>(event_chamber->CaM-> At(i));
	  //	  event_chamber->GetChannel(i,"CbM",chamber);

	  x_queen.push_back(temp->X);
	  y_queen.push_back(temp->Y);

	  h_xy_queen->Fill(temp->X, temp->Y);

	  //	  cout << "Queen " << x_queen[i] << " " << y_queen[i] << endl;

	}


#endif



      double cond_tofplus_y = 40.0;
      double cond_aladin_y = 50.0;



#ifdef Beam

      // Checking BEAMS

      
#ifdef WithTR0
      if(n[0][0]>2) continue;
      if(n[0][1]>2) continue;
#endif



      if(n[1][0]!=1) continue;
      if(n[1][1]!=1) continue;
      if(n[2][0]!=1) continue;
      if(n[2][1]!=1) continue;



      if(temp_n_tofplus_9!=0 && temp_n_tofplus_9==1)
	{

	  int n_good=0;
	  
	  vector<double> pos1_temp[3];
	  vector<double> pos2_temp[3];
	  vector<double> pos_tofplus[3];


	  int flag_TR0x[500] = {0};
	  int flag_TR0y[500] = {0};




#ifdef WithTR0	  
	  for(int i0=0;i0<n[0][0];i0++)  // TR0x
	    {
	      for(int j0=0;j0<n[0][1];j0++) //TR1y
		{
#endif
		  for(int i1=0;i1<n[1][0];i1++)  // TR1x
		    {
		      for(int j1=0;j1<n[1][1];j1++) //TR1y
			{
			  for(int i2=0;i2<n[2][0];i2++)  //TR2x
			    {
			      for(int j2=0;j2<n[2][1];j2++) // TR2y
				{
			      
#ifdef WithTR0
				  if(time_TR[0][i0]-time_TR[1][j0]<-7.0) continue;
				  if(time_TR[0][i0]-time_TR[1][j0]>2.0) continue;
#endif				  
				  
				  if(time_TR[2][i1]-time_TR[3][j1]<-8.0) continue;
				  if(time_TR[2][i1]-time_TR[3][j1]>0.0) continue;
				  if(time_TR[4][i2]-time_TR[5][j2]<-8.0) continue;
				  if(time_TR[4][i2]-time_TR[5][j2]>0.0) continue;
		      
				  
				  /*				  
				  if(width[2][i1]<0.0 ) continue;
				  if(width[3][j1]<0.0 ) continue;
				  if(width[5][j2]<0.0 ) continue;
				  */

				  


#ifdef WithTR0
				  				  if(QDC_sum_TR0[0][i0]!=0 && flag_TR0x[i0]==0)
				    {
				      h_QDC_TR0x->Fill(QDC_sum_TR0[0][i0]);
				      flag_TR0x[i0]=1;
				      //				      cout << "QDC_sum_TR0x : " << QDC_sum_TR0[0][i0] << endl;   
				    }

				  
				  if(QDC_sum_TR0[1][j0]!=0 && flag_TR0y[j0]==0)
				    {
				      h_QDC_TR0y->Fill(QDC_sum_TR0[1][j0]);
				      flag_TR0y[j0]=1;
				      //				      cout << "QDC_sum_TR0y : " << QDC_sum_TR0[1][j0] << endl;   
				    }



				  if(QDC_sum_TR0[0][i0]<300.0 && QDC_sum_TR0[1][j0]<300.0) continue;
				  //if(QDC_sum_TR0[0][i0]<500.0) continue;
#endif

				  //				  if(fabs(x[0][i0])>10.0 || fabs(y[0][j0])>10.0) continue;
				  //				  if(fabs(x[1][i1])>50.0 || fabs(y[1][j1])>50.0) continue;
				  //				  if(fabs(x[2][i2])>50.0 || fabs(y[2][j2])>50.0) continue;





				  double vec1[3],vec2[3];
				  MT_Input_to_3Vector(x[1][i1],y[1][j1],z[1][i1],vec1);
				  MT_Input_to_3Vector(x[2][i2],y[2][j2],z[2][i2],vec2);
				  
				  MT_Line line;
				  line.From_Points(vec1,vec2);
				  
				  
				  double y_temp[4];
				  double z_temp[4];
				  double reso[4];
				  

#ifdef WithTR0
				  y_temp[0] = y[0][j0];
				  z_temp[0] = z[0][i0];
				  y_temp[1] = vec1[1];
				  z_temp[1] = vec1[2];
				  y_temp[2] = vec2[1];
				  z_temp[2] = vec2[2];
#endif
				  

				  
#ifdef WithoutTR0
				  y_temp[0] = vec1[1];
				  z_temp[0] = vec1[2];
				  y_temp[1] = vec2[1];
				  z_temp[1] = vec2[2];
#endif

				  				  
#ifdef WithTR0
				  reso[0] = 1.0;
				  reso[1] = 1.0;
				  reso[2] = 1.0;
#endif		  
				  
#ifdef WithoutTR0
				  reso[0] = 1.0;
				  reso[1] = 1.0;
#endif

				  double tpos[3];
				  int cond_aladin_entrance=0;
				  tpos[2]= 1500.0;//ALADiN entrance position
				  if(line.Get_XY_atZ(tpos)==1)
				    {
				      if(fabs(tpos[1])<250.0 && fabs(tpos[0])<760.0)
					{
					  cond_aladin_entrance=1;
					}
				    }
				  
				  if(cond_aladin_entrance==0) continue;
			      
				  
				  int cond_target=0;
				  
				  tpos[2]= offset_upstream;//target position, make it wider for proton from lambda
				  if(line.Get_XY_atZ(tpos)==1)
				    {
				      if(fabs(tpos[0])<20.0 && fabs(tpos[1])<20.0)
					{
					  cond_target=1;
					}
				    }
			      
				  if(cond_target==0) continue;
				  

#ifdef Chamber

				  /*
				  int cond_chamber = 0;
				  tpos[2]= 550.0-26.0;;//princess position
				  if(line.Get_XY_atZ(tpos)==1)
				    {
				      if(fabs(tpos[0]>15) && fabs(tpos[1]>15)  )
					{
					  
					  for(int i=0;i<num_chamber_hit;i++)
					    {
					      //				  cout << x_princess[i]-tpos[0] << " " << y_princess[i]-tpos[1] << endl;
					      
					      if(fabs(x_princess[i]-tpos[0])<20 && fabs(y_princess[i]-tpos[1])<20) 
						{
						  cond_chamber++;
						}
					    }
					}
				    }
				  
				  if(cond_chamber==0) continue;
				  */
					    
				  int cond_chamber_queen = 0;
				  tpos[2]= 3543.0;//queen position
				  if(line.Get_XY_atZ(tpos)==1)
				    {
				      //			      cout << "Pos " << y_queen[i] << " " << tpos[1] << endl;
				      for(int i=0;i<n_queen;i++)
					{
					  if(fabs(y_queen[i]-tpos[1])<25) 
					    {
					      cond_chamber_queen++;
					    }
					}
				    }
				  
				  if(cond_chamber_queen==0) continue;
				  
#endif

				  
				  
				  //Linear fit
				  
				  int n_tofplus_y = 0;
				  for(int iii=0;iii<temp_n_tofplus_9;iii++)
				    {
				    
				      
#ifdef WithTR0
				      y_temp[3] = y_tofplus_9[iii];
				      z_temp[3] = z_tofplus_9[iii];
				      reso[3] = cond_tofplus_y;
#endif		      

#ifdef WithoutTR0
				      y_temp[2] = y_tofplus_9[iii];
				      z_temp[2] = z_tofplus_9[iii];
				      reso[2] = cond_tofplus_y;
#endif
				    
				    
				      double p1;
				      double p2;
				      double chi;
				      double rchi;
				      

				      //cout << MT_LinearFit(3,z_temp,y_temp,&p1,&p2) << endl;

#ifdef WithTR0
		      if(MT_LinearFit_Chi2(4,z_temp,y_temp,reso,&p1,&p2,&chi,&rchi)==1)
#endif

#ifdef WithoutTR0
			if(MT_LinearFit_Chi2(3,z_temp,y_temp,reso,&p1,&p2,&chi,&rchi)==1)
#endif
					{
					  if(chi<1.0 )
					    {
					      //					  cout << i1 << " " << j1 << " " << i2 << " " << j2 << endl;
					      //					  cout << chi << " " << rchi << endl;
					      
					  
					      pos1_temp[0].push_back(x[1][i1]);
					      pos1_temp[1].push_back(y[1][j1]);
					      pos1_temp[2].push_back(z[1][i1]);
					      
					      pos2_temp[0].push_back(x[2][i2]);
					      pos2_temp[1].push_back(y[2][j2]);
					      pos2_temp[2].push_back(z[2][i2]);
					      
					      
					      pos_tofplus[0].push_back(x_tofplus_9[iii]);
					      pos_tofplus[1].push_back(y_tofplus_9[iii]);
					      pos_tofplus[2].push_back(z_tofplus_9[iii]);
					      
					      
					      n_good++;
					  
					  
					  
					    }
					}
				    }
				  
				}
			    }
			}
		      
		    }
#ifdef WithTR0
		}
	    }
#endif

	  
	  //	  if(n_good>3) continue;
	  //      cout << "**************" << endl;
	  for(int i=0;i<n_good;i++)
	    {
	      
	  
	      double mass_primary, charge_primary;
	      mass_primary = 0.93828*6.015;
	      charge_primary = 3.0;
      
	      double vector[3];
	      vector[0] = pos2_temp[0][i] - pos1_temp[0][i];
	      vector[1] = pos2_temp[1][i] - pos1_temp[1][i];
	      vector[2] = pos2_temp[2][i] - pos1_temp[2][i];

	      double length = sqrt(vector[0]*vector[0] + vector[1]*vector[1] + vector[2]*vector[2]);
	      double length_para = sqrt(vector[0]*vector[0] + vector[2]*vector[2]);

	      
	      double rho_primary = GetRho(pos1_temp[0][i],pos1_temp[2][i],pos2_temp[0][i],pos2_temp[2][i],pos_tofplus[0][i],pos_tofplus[2][i])/1000.0;
	      double p_primary_para = B*rho_primary*3.0/10.0*charge_primary;
	      double ratio = length/length_para;
	      double p_primary = ratio * p_primary_para; 
	      double E_kin_primary = sqrt(p_primary*p_primary + mass_primary*mass_primary)-mass_primary;
	      E_kin_primary = E_kin_primary/6.015;
	      
	      cout << "Beam information : " << p_primary_para << " " << p_primary << " " << E_kin_primary << endl;
	      
	      h_energy_beam->Fill(E_kin_primary);
	      
	    }



	}

#endif





      continue;





   

      if(n_tofplus==0) continue;
      if(n_aladin==0) continue;

      //      cout << n_tofplus << " " << n_aladin << endl;


      int test_n = 1;
      int test_mul=1;
      for(int i=0;i<3;i++)
	{
	  for(int j=0;j<2;j++)
	    {
	      test_n = test_n*n[i][j];
	      if(n[i][j]<2) test_mul=0;
	    }
	}
      //      cout << "yeah  " << endl;


      if(test_n==0) continue;
      if(test_mul==0) continue;


      //      if(fabs(n[1][0]-n[1][1])>3) continue;
      //      if(fabs(n[2][0]-n[2][1])>3) continue;
      //      if(fabs(n[1][0]-n[2][0])>3) continue;
      //      if(fabs(n[1][1]-n[2][1])>3) continue;

      /*
      if(n[0][0]>n[1][0]) continue;
      if(n[0][1]>n[1][1]) continue;
      if(n[0][0]>n[2][0]) continue;
      if(n[0][1]>n[2][1]) continue;
      */

      
      /*
      if(n[0][0]>10) continue;
      if(n[0][1]>10) continue;
      if(n[1][0]>20) continue;
      if(n[1][1]>20) continue;
      if(n[2][0]>20) continue;
      if(n[2][1]>20) continue;
      */
      


























      vector<double> x_TR1_primary;
      vector<double> y_TR1_primary;
      vector<double> z_TR1_primary;

      vector<double> x_TR2_primary;
      vector<double> y_TR2_primary;
      vector<double> z_TR2_primary;

      vector<int> id_TR1x_primary;
      vector<int> id_TR1y_primary;
      vector<int> id_TR2x_primary;
      vector<int> id_TR2y_primary;


      vector<int> id_tofplus_primary;

      int n_primary = 0;

      int n_matched = 0;


#ifdef Chamber
      if(neve%100==0)
	{
	  
	  h_time_fiber->Fill(n_timer,n_fiber_100);
	  h_time_chamber->Fill(n_timer,n_chamber_100_x);
	  h_time_chamber->Fill(n_timer+10000,n_chamber_100_u);
	  h_time_chamber->Fill(n_timer+20000,n_chamber_100_v);


	  n_fiber_100 = 0;
	  n_chamber_100_x = 0;
	  n_chamber_100_u = 0;
	  n_chamber_100_v = 0;
	  n_timer++;
	}

      n_fiber_100 = n_fiber_100 + n[0][0] + n[0][1] + n[1][0] + n[1][1] + n[2][0] + n[2][1];
      //      n_chamber_100 = n_chamber_100 + n_princess;
      n_chamber_100_x = n_chamber_100_x + event_chamber->n_CbM_x + event_chamber->n_CbM_xp;
      n_chamber_100_u = n_chamber_100_u + event_chamber->n_CbM_u + event_chamber->n_CbM_up;
      n_chamber_100_v = n_chamber_100_v + event_chamber->n_CbM_v + event_chamber->n_CbM_vp;

#endif


      for(int i1=0;i1<n[1][0];i1++)  // TR1x
	{
	  for(int j1=0;j1<n[1][1];j1++) //TR1y
	    {
	      for(int i2=0;i2<n[2][0];i2++)  //TR2x
		{
		  for(int j2=0;j2<n[2][1];j2++) // TR2y
		    {

		      if(time_TR[2][i1]-time_TR[3][j1]<-8.0) continue;
		      if(time_TR[2][i1]-time_TR[3][j1]>-0.0) continue;
		      if(time_TR[4][i2]-time_TR[5][j2]<-8.0) continue;
		      if(time_TR[4][i2]-time_TR[5][j2]>2.0) continue;

		      /*
		      if(width[2][i1]<0.0 || width[2][i1]>15.0) continue;
		      if(width[3][j1]<0.0 || width[3][j1]>15.0) continue;
		      if(width[5][j2]<0.0 || width[5][j2]>15.0) continue;
		      */
		      
		      double vec1[3],vec2[3];
		      MT_Input_to_3Vector(x[1][i1],y[1][j1],z[1][i1],vec1);
		      MT_Input_to_3Vector(x[2][i2],y[2][j2],z[2][i2],vec2);

		      MT_Line line;
		      line.From_Points(vec1,vec2);


		      double y_temp[3];
		      double z_temp[3];
		      double reso[3];

		      y_temp[0] = vec1[1];
		      z_temp[0] = vec1[2];
		      y_temp[1] = vec2[1];
		      z_temp[1] = vec2[2];

		      reso[0] = 1.0;
		      reso[1] = 1.0;


		      double tpos[3];

		      /*
		      int cond_target=0;
		      
		      tpos[2]= 26.0+offset_upstream;//target position, make it wider for proton from lambda
		      if(line.Get_XY_atZ(tpos)==1)
			{
			  h_xy_target->Fill(tpos[0],tpos[1]);
			  if(fabs(tpos[0])<40.0 && fabs(tpos[1])<40.0)
			    {
			      cond_target=1;
			    }
			}
		      
		      if(cond_target==0) continue;
		      */


		      int cond_aladin_entrance=0;
		      tpos[2]= 1500.0;//ALADiN entrance position
		      if(line.Get_XY_atZ(tpos)==1)
			{
			  h_xy_magnet->Fill(tpos[0],tpos[1]);
			  if(fabs(tpos[1])<250.0 && fabs(tpos[0])<760.0)
			    //			  if(fabs(tpos[1])<100.0 && fabs(tpos[0])<200.0)
			    {
			      cond_aladin_entrance=1;
			    }
			}

		      if(cond_aladin_entrance==0) continue;


#ifdef Chamber
		      /*
		      int cond_chamber = 0;
		      tpos[2]= 550.0-26.0;;//princess position
		      if(line.Get_XY_atZ(tpos)==1)
			{
			  if(1)
			    {

			      for(int i=0;i<num_chamber_hit;i++)
				{
				  //				  cout << x_princess[i]-tpos[0] << " " << y_princess[i]-tpos[1] << endl;
				  
				  if(fabs(x_princess[i]-tpos[0])<50 && fabs(y_princess[i]-tpos[1])<50) 
				    {
				      cond_chamber++;
				    }
				}
			    }
			}
		      


		      if(cond_chamber==0) continue;
		      */

#endif




		      //Linear fit

		      int n_tofplus_y = 0;
		      for(int iii=0;iii<n_tofplus;iii++)
			{
			  tpos[2]= z_tofplus[iii]; //TOF+ wall Z
			  if(line.Get_XY_atZ(tpos)==1)			    
			    {
			      y_temp[2] = y_tofplus[iii];
			      z_temp[2] = z_tofplus[iii];
			      reso[2] = cond_tofplus_y;
			     
			      
			      double p1;
			      double p2;
			      double chi;
			      double rchi;
			      //			      cout << MT_LinearFit(3,z_temp,y_temp,&p1,&p2) << endl;
			      if(MT_LinearFit_Chi2(3,z_temp,y_temp,reso,&p1,&p2,&chi,&rchi)==1)
				{
				  h_chi2->Fill(chi);
				  //				  cout << chi << " " << rchi << endl;
				  if(chi<1.0)
				    {

				      h_xy_magnet_primary->Fill(tpos[0],tpos[1]);

				      x_TR1_primary.push_back(x[1][i1]);
				      y_TR1_primary.push_back(y[1][j1]);
				      z_TR1_primary.push_back(z[1][i1]);
				      
				      x_TR2_primary.push_back(x[2][i2]);
				      y_TR2_primary.push_back(y[2][j2]);
				      z_TR2_primary.push_back(z[2][i2]);
				      
				      id_tofplus_primary.push_back(iii);
				      
				      
				      id_TR1x_primary.push_back(i1);
				      id_TR1y_primary.push_back(j1);
				      id_TR2x_primary.push_back(i2);
				      id_TR2y_primary.push_back(j2);
				      
				      n_primary++;
				      
				    }


				}
			    }
			}

		    }
		}
	    }
	    
	}
    

      if(n_primary==0) continue;
      //      cout << "n_primary = " << n_primary << endl;
      


      //      cout << "Primary scanned" << endl;

      /*
      cout << n_princess << " " << n_queen << endl;
      cout << "****************** " << num_chamber_hit << " " << n_matched << endl;

      for(int i=0;i<x_x.size();i++)
	{
	  cout << x_x[i] << " " << y_xu[i] << endl;
	}
      */




      vector<double> x_TR1_secondary;
      vector<double> y_TR1_secondary;
      vector<double> z_TR1_secondary;

      vector<double> x_TR2_secondary;
      vector<double> y_TR2_secondary;
      vector<double> z_TR2_secondary;

      vector<int> id_TR1x_secondary;
      vector<int> id_TR1y_secondary;
      vector<int> id_TR2x_secondary;
      vector<int> id_TR2y_secondary;

      vector<int> id_aladin;

      int n_secondary = 0;


      for(int i1=0;i1<n[1][0];i1++) //TR1x
	{
	  for(int j1=0;j1<n[1][1];j1++) //TR1y
	    {
	      for(int i2=0;i2<n[2][0];i2++) //TR2x
		{
		  for(int j2=0;j2<n[2][1];j2++) //TR2y 
		    {

		      if(time_TR[2][i1]-time_TR[3][j1]<-8.0) continue;
		      if(time_TR[2][i1]-time_TR[3][j1]>-0.0) continue;
		      if(time_TR[4][i2]-time_TR[5][j2]<-8.0) continue;
		      if(time_TR[4][i2]-time_TR[5][j2]>2.0) continue;

		      /*
		      if(width[2][i1]<0.0 || width[2][i1]>15.0) continue;
		      if(width[3][j1]<0.0 || width[3][j1]>15.0) continue;
		      if(width[5][j2]<0.0 || width[5][j2]>15.0) continue;
		      */

		      /*
		      if(x[1][i1]>0.0) continue;
		      if(x[2][i2]>0.0) continue;
		      */

		      //		      if(x[1][i1]<x[2][i2]) continue;



		      double vec1[3],vec2[3];
		      MT_Input_to_3Vector(x[1][i1],y[1][j1],z[1][i1],vec1);
		      MT_Input_to_3Vector(x[2][i2],y[2][j2],z[2][i2],vec2);

		      MT_Line line; //secondary
		      line.From_Points(vec1,vec2);


		      double y_temp[3];
		      double z_temp[3];
		      double reso[3];

		      y_temp[0] = vec1[1];
		      z_temp[0] = vec1[2];
		      y_temp[1] = vec2[1];
		      z_temp[1] = vec2[2];


		      reso[0] = 1.0;
		      reso[1] = 1.0;




		      double tpos[3];
		      int cond_aladin_entrance=0;
		      tpos[2]= 1500.0;//ALADiN entrance position
		      if(line.Get_XY_atZ(tpos)==1)
			{
			  if(fabs(tpos[1])<250.0 && fabs(tpos[0])<760.0)
			    //			  if(fabs(tpos[1])<250.0 && tpos[0]<-0.0)
			    {
			      cond_aladin_entrance=1;
			    }
			}

		      if(cond_aladin_entrance==0) continue;



#ifdef Chamber
		      
		      
		      int cond_chamber = 0;
		      tpos[2]= 550.0-26.0;;//princess position
		      if(line.Get_XY_atZ(tpos)==1)
			{
			  if(1)
			    {

			      for(int i=0;i<num_chamber_hit;i++)
				{
				  //				  cout << x_princess[i]-tpos[0] << " " << y_princess[i]-tpos[1] << endl;
				  
				  if(fabs(x_princess[i]-tpos[0])<50 && fabs(y_princess[i]-tpos[1])<50) 
				    {
				      cond_chamber++;
				    }
				}
			    }
			}
		      


		      if(cond_chamber==0) continue;
		      
		      //		      cout << "Princess : " << cond_chamber << endl;
		      








		      int cond_chamber_queen = 0;
		      tpos[2]= 3543.0;//queen position
		      if(line.Get_XY_atZ(tpos)==1)
			{
			  //			      cout << "Pos " << y_queen[i] << " " << tpos[1] << endl;
			  for(int i=0;i<n_queen;i++)
			    {
			      if(fabs(y_queen[i]-tpos[1])<50) 
				{
				  cond_chamber_queen++;
				}
			    }
			}
 
		      if(cond_chamber_queen==0) continue;




#endif

		      //secondary check with ALADiN on y
		      for(int iii=0;iii<n_aladin;iii++)
			{
			  double pos_temp[3];
			  pos_temp[2]= z_aladin[iii]; //ALADiN wall Z
			  if(line.Get_XY_atZ(pos_temp)==1)			    
			    {
			      y_temp[2] = y_aladin[iii];
			      z_temp[2] = z_aladin[iii];
			      reso[2] = cond_aladin_y;
			      
			      
			      double p1;
			      double p2;
			      double chi;
			      double rchi;
			      //			      cout << MT_LinearFit(3,z_temp,y_temp,&p1,&p2) << endl;
			      if(MT_LinearFit_Chi2(3,z_temp,y_temp,reso,&p1,&p2,&chi,&rchi)==1)
				{

				  //				  if(chi<1) cout << chi << endl;

				  //				  h_chi2->Fill(chi);
				  //				  cout << chi << " " << rchi << endl;
				  if(chi<1.0)
				    {
				      h_xy_magnet_secondary->Fill(tpos[0],tpos[1]);


				      x_TR1_secondary.push_back(x[1][i1]);
				      y_TR1_secondary.push_back(y[1][j1]);
				      z_TR1_secondary.push_back(z[1][i1]);
				      
				      x_TR2_secondary.push_back(x[2][i2]);
				      y_TR2_secondary.push_back(y[2][j2]);
				      z_TR2_secondary.push_back(z[2][i2]);
				      
				      id_aladin.push_back(iii);
				      
				      
				      id_TR1x_secondary.push_back(i1);
				      id_TR1y_secondary.push_back(j1);
				      id_TR2x_secondary.push_back(i2);
				      id_TR2y_secondary.push_back(j2);
				      
				      
				      n_secondary++;
				    }
				}
			    }
			}
		    }
		}
	    }
	}

      if(n_secondary==0) continue;
      cout << "n_primary = " << n_primary << endl;
      cout << "n_secondary = " << n_secondary << endl;


      
      
      vector<double> x_TR1_primary_selected;
      vector<double> y_TR1_primary_selected;
      vector<double> z_TR1_primary_selected;
      
      vector<double> x_TR2_primary_selected;
      vector<double> y_TR2_primary_selected;
      vector<double> z_TR2_primary_selected;
      
      vector<double> x_TR1_secondary_selected;
      vector<double> y_TR1_secondary_selected;
      vector<double> z_TR1_secondary_selected;
      
      vector<double> x_TR2_secondary_selected;
      vector<double> y_TR2_secondary_selected;
      vector<double> z_TR2_secondary_selected;
      
      vector<int> id_tofplus_selected;
      vector<int> id_aladin_selected;
      
      vector<int> id_TR1x_primary_selected;
      vector<int> id_TR1y_primary_selected;
      vector<int> id_TR2x_primary_selected;
      vector<int> id_TR2y_primary_selected;

      vector<int> id_TR1x_secondary_selected;
      vector<int> id_TR1y_secondary_selected;
      vector<int> id_TR2x_secondary_selected;
      vector<int> id_TR2y_secondary_selected;

      

      int n_selected = 0;
      
      vector<double> x_sec_selected;
      vector<double> y_sec_selected;
      vector<double> z_sec_selected;

      vector<double> dis_selected;

      vector<double> theta;

      for(int i=0;i<n_primary;i++)
	{
	  for(int j=0;j<n_secondary;j++)
	    {
	
	      //	      if(id_TR1x_primary[i]==id_TR1x_secondary[j] && id_TR1y_primary[i]==id_TR1y_secondary[j]) continue;
	      //	      if(id_TR2x_primary[i]==id_TR2x_secondary[j] && id_TR2y_primary[i]==id_TR2y_secondary[j]) continue;
	      if(id_TR1x_primary[i]==id_TR1x_secondary[j] || id_TR1y_primary[i]==id_TR1y_secondary[j]) continue;
	      if(id_TR2x_primary[i]==id_TR2x_secondary[j] || id_TR2y_primary[i]==id_TR2y_secondary[j]) continue;
	      
	      if(id_TR1x_primary[i]==id_TR1x_secondary[j] && id_TR2x_primary[i]==id_TR2x_secondary[j]) continue;
	      if(id_TR1y_primary[i]==id_TR1y_secondary[j] && id_TR2y_primary[i]==id_TR2y_secondary[j]) continue;




      
	      double vec1[3],vec2[3];  //secondary
	      MT_Input_to_3Vector(x_TR1_secondary[j],y_TR1_secondary[j],z_TR1_secondary[j],vec1);
	      MT_Input_to_3Vector(x_TR2_secondary[j],y_TR2_secondary[j],z_TR2_secondary[j],vec2);

	      
	      MT_Line line; //secondary
	      line.From_Points(vec1,vec2);

	      double vec3[3],vec4[3]; //primary
	      MT_Input_to_3Vector(x_TR1_primary[i],y_TR1_primary[i],z_TR1_primary[i],vec3);
	      MT_Input_to_3Vector(x_TR2_primary[i],y_TR2_primary[i],z_TR2_primary[i],vec4);

	      MT_Line line2; //primary
	      line2.From_Points(vec3,vec4);
	      

	      double vec5[3];
	      double dis = MT_Distance_of_Lines(&line,&line2,vec5);

	      //	      cout << vec1[0] << " " << vec2[0] << " " << vec3[0] << " " << vec4[0] << endl; 

	      //	      cout << "dis = " << dis << endl;


	      
	      h_z_sec->Fill(vec5[2]);
			  
	      
	      
	      //	      if(vec5[2]>100 && vec5[2]<300) // secondary vertex position cut
	      if(vec5[2]>0. && vec5[2]<350) // secondary vertex position cut
		{ 
		  //		  cout << " yeah 1 " << endl; 

		  //		  if(vec5[0]>-90.0 && vec5[0]<45.0 && fabs(vec5[1])<38.0) // decay volume cut
		  if(1/*vec5[0]>-40.0 && vec5[0]<40.0 && fabs(vec5[1])<40.0*/) // decay volume cut
		    { 
		      h_dis_tracks->Fill(dis);
		      if(dis<7.0) // track distance cut
			{
			  //			  cout << " yeah 2 " << endl; 
			  double length34 = sqrt((vec4[0]-vec3[0])*(vec4[0]-vec3[0]) + (vec4[1]-vec3[1])*(vec4[1]-vec3[1]) + (vec4[2]-vec3[2])*(vec4[2]-vec3[2]));
			  double length12 = sqrt((vec2[0]-vec1[0])*(vec2[0]-vec1[0]) + (vec2[1]-vec1[1])*(vec2[1]-vec1[1]) + (vec2[2]-vec1[2])*(vec2[2]-vec1[2]));
			  double sp = (vec4[0]-vec3[0])*(vec2[0]-vec1[0]) + (vec4[1]-vec3[1])*(vec2[1]-vec1[1]) + (vec4[2]-vec3[2])*(vec2[2]-vec1[2]);
					      
			  double costheta = sp/length34/length12;
			  double theta_temp = acos(costheta)/3.14159*180;
			  
			  h_angle->Fill(theta_temp);
			  
			  if(1/*theta_temp<10.0 && theta_temp > 1.0*/) 
			    {
			      
			      theta.push_back(acos(costheta)/3.14159*180);
			      
			      //					      cout << " angle = " << theta << " distance = " << dis << endl; 
			      
			      
			      
			      
			      x_TR1_primary_selected.push_back(vec3[0]);
			      y_TR1_primary_selected.push_back(vec3[1]);
			      z_TR1_primary_selected.push_back(vec3[2]);
			      x_TR2_primary_selected.push_back(vec4[0]);
			      y_TR2_primary_selected.push_back(vec4[1]);
			      z_TR2_primary_selected.push_back(vec4[2]);
			      
			      x_TR1_secondary_selected.push_back(vec1[0]);
			      y_TR1_secondary_selected.push_back(vec1[1]);
			      z_TR1_secondary_selected.push_back(vec1[2]);
			      x_TR2_secondary_selected.push_back(vec2[0]);
			      y_TR2_secondary_selected.push_back(vec2[1]);
			      z_TR2_secondary_selected.push_back(vec2[2]);
			      
			      id_tofplus_selected.push_back(id_tofplus_primary[i]);
			      id_aladin_selected.push_back(id_aladin[j]);
			      
			      //			  cout << id_aladin_selected[0] << endl;
			      
			      x_sec_selected.push_back(vec5[0]);
			      y_sec_selected.push_back(vec5[1]);
			      z_sec_selected.push_back(vec5[2]);
			      
			      dis_selected.push_back(dis);
			      
			      id_TR1x_primary_selected.push_back(id_TR1x_primary[i]);
			      id_TR1y_primary_selected.push_back(id_TR1y_primary[i]);
			      id_TR2x_primary_selected.push_back(id_TR2x_primary[i]);
			      id_TR2y_primary_selected.push_back(id_TR2y_primary[i]);
			      
			      id_TR1x_secondary_selected.push_back(id_TR1x_secondary[j]);
			      id_TR1y_secondary_selected.push_back(id_TR1y_secondary[j]);
			      id_TR2x_secondary_selected.push_back(id_TR2x_secondary[j]);
			      id_TR2y_secondary_selected.push_back(id_TR2y_secondary[j]);
			      
			      n_selected++;
			    }       
			}
		      
		    }
		}
	    }
	}
      
      //      if(n_selected!=0) cout << "n_selected = " << n_selected << endl;
      if(n_selected!=0) cout << "*************************** n_selected = " << n_selected << endl;

      h_n_sec->Fill(n_selected);

      h_n_tofstart_n_selected->Fill(n_tofstart,n_selected);



      
#ifdef MassScan
      for(int jjj=-5;jjj<26;jjj++) // mass scan
	{
#endif

	  vector<int> id_TR1x_used;
	  vector<int> id_TR1y_used;
	  vector<int> id_TR2x_used;
	  vector<int> id_TR2y_used;
	  
	  
	  
	  int n_used=0;
	  
	  int n_final=0;
	  
	  vector<double> inv_mass_final;
	  vector<double> dis_final;
	  vector<double> z_sec_final;

	  vector<int> id_iter_final;
	  vector<double> dif_p;
	  vector<double> energy_initial_final;

	  for(int lll=0;lll<n_selected;lll++)
	    {
	      
	      
	      double vec_TR1_primary[3];
	      double vec_TR2_primary[3];
	      double vec_TR1_secondary[3];
	      double vec_TR2_secondary[3];
	      
	      vec_TR1_primary[0] = x_TR1_primary_selected[lll];
	      vec_TR1_primary[1] = y_TR1_primary_selected[lll];
	      vec_TR1_primary[2] = z_TR1_primary_selected[lll];
	      vec_TR2_primary[0] = x_TR2_primary_selected[lll];
	      vec_TR2_primary[1] = y_TR2_primary_selected[lll];
	      vec_TR2_primary[2] = z_TR2_primary_selected[lll];
	      
	      vec_TR1_secondary[0] = x_TR1_secondary_selected[lll];
	      vec_TR1_secondary[1] = y_TR1_secondary_selected[lll];
	      vec_TR1_secondary[2] = z_TR1_secondary_selected[lll];
	      vec_TR2_secondary[0] = x_TR2_secondary_selected[lll];
	      vec_TR2_secondary[1] = y_TR2_secondary_selected[lll];
	      vec_TR2_secondary[2] = z_TR2_secondary_selected[lll];
	      
	      
	      double length_primary = sqrt((vec_TR2_primary[0]-vec_TR1_primary[0])*(vec_TR2_primary[0]-vec_TR1_primary[0]) + (vec_TR2_primary[1]-vec_TR1_primary[1])*(vec_TR2_primary[1]-vec_TR1_primary[1]) + (vec_TR2_primary[2]-vec_TR1_primary[2])*(vec_TR2_primary[2]-vec_TR1_primary[2]));

	      double vec_unit_primary[3];
	      vec_unit_primary[0] = (vec_TR2_primary[0]-vec_TR1_primary[0])/length_primary;
	      vec_unit_primary[1] = (vec_TR2_primary[1]-vec_TR1_primary[1])/length_primary;
	      vec_unit_primary[2] = (vec_TR2_primary[2]-vec_TR1_primary[2])/length_primary;
	      
	      double length_primary_para = sqrt(vec_unit_primary[0]*vec_unit_primary[0] + vec_unit_primary[2]*vec_unit_primary[2]);
	      


	      double length_secondary = sqrt((vec_TR2_secondary[0]-vec_TR1_secondary[0])*(vec_TR2_secondary[0]-vec_TR1_secondary[0]) + (vec_TR2_secondary[1]-vec_TR1_secondary[1])*(vec_TR2_secondary[1]-vec_TR1_secondary[1]) + (vec_TR2_secondary[2]-vec_TR1_secondary[2])*(vec_TR2_secondary[2]-vec_TR1_secondary[2]));
	      
	      double vec_unit_secondary[3];
	      vec_unit_secondary[0] = (vec_TR2_secondary[0]-vec_TR1_secondary[0])/length_secondary;
	      vec_unit_secondary[1] = (vec_TR2_secondary[1]-vec_TR1_secondary[1])/length_secondary;
	      vec_unit_secondary[2] = (vec_TR2_secondary[2]-vec_TR1_secondary[2])/length_secondary;
	      double length_secondary_para = sqrt(vec_unit_secondary[0]*vec_unit_secondary[0] + vec_unit_secondary[2]*vec_unit_secondary[2]);
	
	      //	      cout << length_secondary_para << endl;
	      
	      
	      double pos_magnet[3];
	      
	      MT_Line line_primary;
	      line_primary.From_Points(vec_TR1_primary,vec_TR2_primary);
	      pos_magnet[2]= 1500.0;//ALADiN entrance position
	      if(line_primary.Get_XY_atZ(pos_magnet)==1)
		{
		  //		  h_xy_magnet_primary->Fill(pos_magnet[0],pos_magnet[1]);
		}
	      
	      pos_magnet[2]= 1500.0;//ALADiN entrance position
	      MT_Line line_secondary;
	      line_secondary.From_Points(vec_TR1_secondary,vec_TR2_secondary);
	      if(line_secondary.Get_XY_atZ(pos_magnet)==1)
		{
		  //		  h_xy_magnet_secondary->Fill(pos_magnet[0],pos_magnet[1]);
		}
	      
	      
	      
	      ////////// Primary

	      double mass_primary, charge_primary;
#ifdef Lambda
	      mass_primary = 0.938;
	      charge_primary = 1.0;
#endif
#ifdef Hypernuclei
	      mass_primary = 0.938*4;
	      charge_primary = 2.0;
#endif
#ifdef NLambda
	      mass_primary = 0.938*2;
	      charge_primary = 1.0;
#endif
	      
	      double x_tofplus_selected = x_tofplus[id_tofplus_selected[lll]];
	      double z_tofplus_selected = z_tofplus[id_tofplus_selected[lll]];
	      
	      
	      double rho_primary = GetRho(vec_TR1_primary[0],vec_TR1_primary[2],vec_TR2_primary[0],vec_TR2_primary[2],x_tofplus_selected,z_tofplus_selected)/1000.0;
	      double p_primary_para = B*rho_primary*3.0/10.0*charge_primary;
	      double p_primary = p_primary_para/length_primary_para;
	      double E_kin_primary = sqrt(p_primary*p_primary + mass_primary*mass_primary)-mass_primary;
	      
	      //	  cout << E_kin_primary << endl;

	      
	      ////////// Secondary
	      
	      double mass_secondary = 0.1396;
	      double charge_secondary=1.0;
	      
	      double x_aladin_selected = x_aladin[id_aladin_selected[lll]];
	      double z_aladin_selected = z_aladin[id_aladin_selected[lll]];
	      
	      double rho_secondary = GetRho(vec_TR1_secondary[0],vec_TR1_secondary[2],vec_TR2_secondary[0],vec_TR2_secondary[2],x_aladin_selected,z_aladin_selected)/1000.0;
	      double p_secondary_para = B*rho_secondary*3.0/10.0*charge_secondary;
	      double p_secondary = p_secondary_para/length_secondary_para;
	      double E_kin_secondary = sqrt(p_secondary*p_secondary + mass_secondary*mass_secondary)-mass_secondary;
	      
	      //	      cout << "Primary   " << p_primary_para << " " << p_primary << endl;
	      //	      cout << "Secondary " << p_secondary_para << " " << p_secondary << endl;

	      
	      //	      	  cout <<rho_primary << " " << rho_secondary << endl;
	      //	      	  cout <<E_kin_primary << " " << E_kin_secondary << endl;
	      
	      
	      if(rho_primary>0 && rho_secondary>0)
		{
		  
		  
		  

		  double momentum_primary[3];
		  momentum_primary[0] = vec_unit_primary[0]*p_primary;
		  momentum_primary[1] = vec_unit_primary[1]*p_primary;
		  momentum_primary[2] = vec_unit_primary[2]*p_primary;
		  double E_primary = sqrt(p_primary*p_primary + mass_primary*mass_primary);
		  
		  double momentum_secondary[3];
		  momentum_secondary[0] = vec_unit_secondary[0]*p_secondary;
		  momentum_secondary[1] = vec_unit_secondary[1]*p_secondary;
		  momentum_secondary[2] = vec_unit_secondary[2]*p_secondary;
		  double E_secondary = sqrt(p_secondary*p_secondary + mass_secondary*mass_secondary);
		  
		  double momentum_lambda[3];
		  momentum_lambda[0] = momentum_primary[0] + momentum_secondary[0];
		  momentum_lambda[1] = momentum_primary[1] + momentum_secondary[1];
		  momentum_lambda[2] = momentum_primary[2] + momentum_secondary[2];
		  
		  double inv_mass = (E_kin_primary + mass_primary + E_kin_secondary + mass_secondary)*(E_kin_primary + mass_primary + E_kin_secondary + mass_secondary) - ((momentum_primary[0]+momentum_secondary[0])*(momentum_primary[0]+momentum_secondary[0])+(momentum_primary[1]+momentum_secondary[1])*(momentum_primary[1]+momentum_secondary[1])+(momentum_primary[2]+momentum_secondary[2])*(momentum_primary[2]+momentum_secondary[2]));
		  inv_mass = sqrt(inv_mass);
		  
		  
		  
		  
		  
		  double pos0[3],pos1[3];
		  pos0[0] = x_sec_selected[lll];
		  pos0[1] = y_sec_selected[lll];
		  pos0[2] = z_sec_selected[lll];
		  
		  pos1[0] = pos0[0] + momentum_lambda[0];
		  pos1[1] = pos0[1] + momentum_lambda[1];
		  pos1[2] = pos0[2] + momentum_lambda[2];
		  
		  MT_Line line; //initial product trajectory
		  line.From_Points(pos0,pos1);
		  
		  double tpos[3];
		  int cond_target=0;
		  tpos[2]= 26.0+offset_upstream;//target position cut for summed momentum
		  if(line.Get_XY_atZ(tpos)==1)
		    {
		      h_xy_target->Fill(tpos[0],tpos[1]);
		      if(fabs(tpos[0])<20.0 && fabs(tpos[1])<20.0)
			{
			  cond_target=1;
			}
		    }
		   
		  if(cond_target==0) continue;
		  


#ifdef Hypernuclei

		  int cond_TR0 = 0;

		  tpos[2]= 43.5 + 26.0;//TR0 position, nothing for Lambda, ONLY FOR HYPERNUCLEII
		  if(line.Get_XY_atZ(tpos)==1)
		    {
		      for(int i0=0;i0<n[0][0];i0++)
			{
			  for(int j0=0;j0<n[0][1];j0++)
			    {
			      
			      if(time_TR[0][i0]-time_TR[1][j0]<-7.0) continue;
			      if(time_TR[0][i0]-time_TR[1][j0]>2.0) continue;


			      double distance = sqrt((tpos[0]-x[0][i0])*(tpos[0]-x[0][i0]) + (tpos[1]-y[0][j0])*(tpos[1]-y[0][j0])); 
			      
			      h_dis_TR0->Fill(distance);
			      
			      if(distance<2.0)
				{
				  cond_TR0=1;
				}
			    }
			}
		    }
		  if(cond_TR0==0) continue;

#endif    

#ifdef Lambda
		  /*
		  int cond_TR0 = 0;

		  tpos[2]= 43.5 + 26.0;//TR0 position, nothing for Lambda, 
		  if(line.Get_XY_atZ(tpos)==1)
		    {
		      for(int i0=0;i0<n[0][0];i0++)
			{
			  for(int j0=0;j0<n[0][1];j0++)
			    {
			      
			      if(time_TR[0][i0]-time_TR[1][j0]<-7.0) continue;
			      if(time_TR[0][i0]-time_TR[1][j0]>2.0) continue;


			      double distance = sqrt((tpos[0]-x[0][i0])*(tpos[0]-x[0][i0]) + (tpos[1]-y[0][j0])*(tpos[1]-y[0][j0])); 
			      
			      h_dis_TR0->Fill(distance);
			      
			      if(distance<3.0)
				{
				  cond_TR0=1;
				}
			    }
			}
		    }
		  if(cond_TR0==1) continue;
		  */
#endif    


		  double mass_initial;
#ifdef Lambda
		  mass_initial = 1.115;
#endif
#ifdef MassScan
		  mass_initial = 1.115 + jjj*0.003;
#endif
#ifdef Hypernuclei
		  mass_initial = 0.938*3 + 1.115; 
#endif
#ifdef NLambda
		  mass_initial = 0.938 + 1.115; 
#endif
		  
		  double p_initial = sqrt(momentum_lambda[0]*momentum_lambda[0] + momentum_lambda[1]*momentum_lambda[1] + momentum_lambda[2]*momentum_lambda[2]);
		  double E_initial = sqrt(p_initial*p_initial + mass_initial*mass_initial);
		  double gamma = E_initial/mass_initial;
		  double beta = p_initial/E_initial;
		  double energy_initial = sqrt((momentum_primary[0]+momentum_secondary[0])*(momentum_primary[0]+momentum_secondary[0])+(momentum_primary[1]+momentum_secondary[1])*(momentum_primary[1]+momentum_secondary[1])+(momentum_primary[2]+momentum_secondary[2])*(momentum_primary[2]+momentum_secondary[2])+mass_initial*mass_initial) - mass_initial;
		  



#ifdef Lambda
		  //if(energy_initial > 2.5) continue;
#endif

#ifdef Hypernuclei
		  if(energy_initial > 2.5*4) continue;
#endif

#ifdef NLambda
		  if(energy_initial > 2.5*2) continue;
#endif

		  h_n_selected->Fill(n_selected);

		  //		  dif_p.push_back(diff);

		  energy_initial_final.push_back(energy_initial);
		  
		  inv_mass_final.push_back(inv_mass);
		  dis_final.push_back(dis_selected[lll]);
		  id_iter_final.push_back(lll);
		  z_sec_final.push_back(z_sec_selected[lll]);

		  n_final++;
		  
		  
		}
	    }
	  
	  if(n_final>0) cout << "+++++++++++++++++++++++++++++ n_final = " << n_final << endl;
	  
	  //	  if(n_final>20) continue;

	  for(int i=0;i<n_final;i++)
	    {
	      n_filled++;
	      if(inv_mass_final[i]>1.11 && inv_mass_final[i]<1.12)
		{
		  n_attention++;
		  cout << "----------------------- inv mass = " << inv_mass_final[i] << " dis = " << dis_final[i] << " z = " << z_sec_final[i] << "  <-----Attentiion " << n_attention << " / " << n_filled << endl;
		}
	      else
		{
		  cout << "----------------------- inv mass = " << inv_mass_final[i] << " dis = " << dis_final[i] << " z = " << z_sec_final[i] << endl;
		}
	      
	      h_invmass->Fill(inv_mass_final[i]);
	      if(inv_mass_final[i]>1.110 && inv_mass_final[i]<1.120) 
		{
		  h_energy_initial->Fill(energy_initial_final[i]);
		  h_z_sec_selected->Fill(z_sec_final[i]);

		  h_energy_initial_sub->Fill(energy_initial_final[i]);
		  h_z_sec_selected_sub->Fill(z_sec_final[i]);

		  double gamma_final = (energy_initial_final[i] + inv_mass_final[i])/inv_mass_final[i];
		  double beta_final = sqrt(1.0 - 1.0/gamma_final/gamma_final);

		  cout << gamma_final << " " << beta_final << endl;

		  double time = (z_sec_final[i] - 26.0)/beta_final/gamma_final/300.0;
		  cout << time << endl;

		  h_decay->Fill(time);
		  h_decay_sub->Fill(time);

		}
	      if(inv_mass_final[i]>1.120 && inv_mass_final[i]<1.140) 
		{
		  h_energy_initial_bg->Fill(energy_initial_final[i]);
		  h_z_sec_selected_bg->Fill(z_sec_final[i]);

		  h_energy_initial_sub->Fill(energy_initial_final[i],-0.5);
		  h_z_sec_selected_sub->Fill(z_sec_final[i],-0.5);


		  double gamma_final = (energy_initial_final[i] + inv_mass_final[i])/inv_mass_final[i];
		  double beta_final = sqrt(1.0 - 1.0/gamma_final/gamma_final);

		  cout << "sub " << gamma_final << " " << beta_final << endl;

		  double time = (z_sec_final[i] - 26.0)/beta_final/gamma_final/300.0;
		  cout << "sub " << time << endl;

		  h_decay_bg->Fill(time);
		  h_decay_sub->Fill(time,-0.5);

		}
	    }
	  
#ifdef MassScan	  
	}
#endif

    }

  WriteHisto();

}


void WriteHisto()
{
#ifdef Lambda 
#ifdef HypernuclearTrigger
  
#ifdef Mixed  
  TFile *outfile = new TFile("test_mixed.root","RECREATE");
#endif

#ifdef Normal
  TFile *outfile = new TFile("test.root","RECREATE");
#endif

  //  TFile *outfile = new TFile("test_queen.root","RECREATE");
#endif
#endif

#ifdef Hypernuclei
#ifdef HypernuclearTrigger
  TFile *outfile = new TFile("test_4H.root","RECREATE");
#endif
#endif

#ifdef NLambda
#ifdef HypernuclearTrigger
  TFile *outfile = new TFile("test_NLambda.root","RECREATE");
#endif
#endif

#ifdef Lambda 
#ifdef Reaction3Trigger
#ifdef Normal
  TFile *outfile = new TFile("test_reaction3.root","RECREATE");
#endif

#ifdef Mixed  
  TFile *outfile = new TFile("test_reaction3_mixed.root","RECREATE");
#endif


#endif
#endif

#ifdef Hypernuclei
#ifdef Reaction3Trigger
  TFile *outfile = new TFile("test_4H_reaction3.root","RECREATE");
#endif
#endif

#ifdef NLambda
#ifdef Reaction3Trigger
  TFile *outfile = new TFile("test_NLambda_reaction3.root","RECREATE");
#endif
#endif

#ifdef MassScan
#ifdef Lambda
  TFile *outfile = new TFile("test_MassScan.root","RECREATE");
#endif
#endif

  outfile->cd();
  h_xy_target->Write();
  h_xy_magnet->Write();
  h_dis_TR0->Write();
  h_dis_tracks->Write();
  h_z_sec->Write();

  h_xy_magnet_primary->Write();
  h_xy_magnet_secondary->Write();

  h_z_sec_selected->Write();
  h_z_sec_selected_bg->Write();
  h_z_sec_selected_sub->Write();

  h_xy_tofplus->Write();
  h_xy_tofplus_bar->Write();
  h_xy_aladin->Write();

  h_invmass->Write();
  h_energy_initial->Write();
  h_energy_initial_bg->Write();
  h_energy_initial_sub->Write();

  h_res_y_tofplus->Write();
  h_res_y_aladin->Write();

  h_res_y_tofplus_selected->Write();
  h_res_y_aladin_selected->Write();

  h_angle->Write();
  h_n_sec->Write();
  
#ifdef TofPlusEnergyHisto
  for(int i=0;i<32;i++)
    {
      h_energy_tofplus[i]->Write();
    }
#endif

  h_invmass_angle->Write();
  h_pp_ppi->Write();
  h_pp_ppi_mf->Write();

  h_dif_pmf->Write();
  h_cos_bb->Write();
  h_cosbb_difp->Write();

  h_n_selected->Write();
  h_n_selected_invmass->Write();

#ifdef MassScan
  for(int i=0;i<31;i++)
    {
      h_invmass_mass_scan[i]->Write();
    } 
#endif 

  h_time_fiber->Write();
  h_time_chamber->Write();



  h_xy_princess->Write();
  h_xy_queen->Write();

  h_chi2->Write();
  h_n_tr->Write();

  h_nx_TR0->Write();
  h_ny_TR0->Write();
  h_nx_TR1->Write();
  h_ny_TR1->Write();
  h_nx_TR2->Write();
  h_ny_TR2->Write();

  h_xy_TR0->Write();
  h_xy_TR1->Write();
  h_xy_TR2->Write();


  h_n_tofstart_n_selected->Write();

  h_tofplus_time->Write();

  h_tofplus_cluster_energy->Write();
  h_aladin_tdc->Write();

  h_TOF_tofplus->Write();
  h_x_TOF_tofplus->Write();

  h_energy_beam->Write();

  h_tofstart_time->Write();

  h_tdif_TR0->Write();
  h_tdif_TR1->Write();
  h_tdif_TR2->Write();


  h_decay->Write();
  h_decay_bg->Write();
  h_decay_sub->Write();

  h_pid_tof->Write();


  h_width_TR0x->Write();
  h_width_TR0y->Write();
  h_width_TR1x->Write();
  h_width_TR1y->Write();
  h_width_TR2x->Write();
  h_width_TR2y->Write();

  h_cluster_size_TR0x->Write();
  h_cluster_size_TR0y->Write();
  h_cluster_size_TR1x->Write();
  h_cluster_size_TR1y->Write();
  h_cluster_size_TR2x->Write();
  h_cluster_size_TR2y->Write();


  h_QDC_TR0x->Write();
  h_QDC_TR0y->Write();



  delete outfile;

}


double GetRho(double x_TR1, double z_TR1, double x_TR2, double z_TR2, double x_TOF, double z_TOF)
{

  double a = (z_TR1-z_TR2)/(x_TR1-x_TR2);
  double b = -a*x_TR1 + z_TR1;

  double pos_TR1[2],pos_TR2[2];
  pos_TR1[0] = x_TR1;
  pos_TR1[1] = z_TR1;
  pos_TR2[0] = x_TR2;
  pos_TR2[1] = z_TR2;
  double vec_in[2];
  vec_in[0] = pos_TR2[0] - pos_TR1[0];
  vec_in[1] = pos_TR2[1] - pos_TR1[1];


  double pos_magnet_corner1[2],pos_magnet_corner2[2],pos_magnet_corner3[2],pos_magnet_corner4[2];
  pos_magnet_corner1[0] = corner1_angle[0];
  pos_magnet_corner1[1] = corner1_angle[2];
  pos_magnet_corner2[0] = corner2_angle[0];
  pos_magnet_corner2[1] = corner2_angle[2];
  pos_magnet_corner3[0] = corner3_angle[0];
  pos_magnet_corner3[1] = corner3_angle[2];
  pos_magnet_corner4[0] = corner4_angle[0];
  pos_magnet_corner4[1] = corner4_angle[2];
  double vec_magnet_in[2], vec_magnet_out[2];
  vec_magnet_in[0] = pos_magnet_corner2[0] - pos_magnet_corner1[0];
  vec_magnet_in[1] = pos_magnet_corner2[1] - pos_magnet_corner1[1];
  vec_magnet_out[0] = pos_magnet_corner4[0] - pos_magnet_corner3[0];
  vec_magnet_out[1] = pos_magnet_corner4[1] - pos_magnet_corner3[1];

  double pos_1[2];

  MT_Intersection_of_Lines_2D(pos_TR2,vec_in,pos_magnet_corner1,vec_magnet_in,pos_1);

  double pos_2[2];

  MT_Intersection_of_Lines_2D(pos_TR2,vec_in,pos_magnet_corner3,vec_magnet_out,pos_2);

  double pos_0[2];
  pos_0[0] = x_TOF;
  pos_0[1] = z_TOF;

  
  double pos_3_selected[2];
  double vec_03_selected[2];
    
  double min = 999999999999999.9;

  for(int i=0;i<300;i++)
    {
      double t = 1000.0 + i*10.0;
      double pos_t[2];
      pos_t[0] = (t-b)/a;
      pos_t[1] = t;

      double vec_t0[2];
      vec_t0[0] = pos_0[0] - pos_t[0];
      vec_t0[1] = pos_0[1] - pos_t[1]; 

      double pos_3[2];
      
      MT_Intersection_of_Lines_2D(pos_0,vec_t0,pos_magnet_corner3,vec_magnet_out,pos_3);

      double length1 = (pos_1[0]-pos_t[0])*(pos_1[0]-pos_t[0]) + (pos_1[1]-pos_t[1])*(pos_1[1]-pos_t[1]);
      double length2 = (pos_3[0]-pos_t[0])*(pos_3[0]-pos_t[0]) + (pos_3[1]-pos_t[1])*(pos_3[1]-pos_t[1]);
      double test = fabs(sqrt(length1) - sqrt(length2));

      if(pos_t[1]<pos_1[1] || pos_t[1] > pos_2[1]) continue;

      if(test<min)
	{
	  min = test;
	  pos_3_selected[0] = pos_3[0];
	  pos_3_selected[1] = pos_3[1];
	  vec_03_selected[0] = pos_3[0] - pos_0[0];
	  vec_03_selected[1] = pos_3[1] - pos_0[1];
	}
    }

  double center[2];

  if(MT_Center_of_Incircle(pos_1,vec_in,pos_3_selected,vec_03_selected,center)==1)
    {

      //      cout << "pos_3 (x,z) = " << pos_3_selected[0] << " " << pos_3_selected[1] << endl;
      //      cout << "center (x,z) = " << center[0] << " " << center[1] << endl;
      double rho = sqrt((pos_1[0]-center[0])*(pos_1[0]-center[0]) + (pos_1[1]-center[1])*(pos_1[1]-center[1]));
      //      cout << " ******** Test for GetRho : " << rho << " " << min << endl;
      return rho;


    }

  return -1;

}




void exit_magnet_primary(double *center, double rho, double *point1, double *point2, double *point_exit, double &slope)
{

  double point1_temp[2];
  double point2_temp[2];
  point1_temp[0] = point1[0] - center[0];
  point1_temp[1] = point1[1] - center[1];
  point2_temp[0] = point2[0] - center[0];
  point2_temp[1] = point2[1] - center[1];

  double a = (point1_temp[1]-point2_temp[1])/(point1_temp[0]-point2_temp[0]);
  double b = -point1_temp[0]*a + point1_temp[1];

  double solution[2];
  solution[0] = (-a*b + sqrt(-b*b+rho*rho+a*a*rho*rho))/(1+a*a);
  solution[1] = b - a*a*b/(1+a*a) + a*sqrt(-b*b+rho*rho+a*a*rho*rho)/(1+a*a);

  slope = -solution[0]/sqrt(rho*rho-solution[0]*solution[0]);

  point_exit[0] = solution[0] + center[0];
  point_exit[1] = solution[1] + center[1];

  //  cout << point_exit[0] << " " << point_exit[1] << endl;


}

void exit_magnet_secondary(double *center, double rho, double *point1, double *point2, double *point_exit, double &slope)
{

  double point1_temp[2];
  double point2_temp[2];
  point1_temp[0] = point1[0] - center[0];
  point1_temp[1] = point1[1] - center[1];
  point2_temp[0] = point2[0] - center[0];
  point2_temp[1] = point2[1] - center[1];

  double a = (point1_temp[1]-point2_temp[1])/(point1_temp[0]-point2_temp[0]);
  double b = -point1_temp[0]*a + point1_temp[1];

  double solution[2];
  solution[0] = (-a*b - sqrt(-b*b+rho*rho+a*a*rho*rho))/(1+a*a);
  solution[1] = b - a*a*b/(1+a*a) - a*sqrt(-b*b+rho*rho+a*a*rho*rho)/(1+a*a);

  //  slope = -solution[0]/sqrt(rho*rho-solution[0]*solution[0]);
  slope = +solution[0]/sqrt(rho*rho-solution[0]*solution[0]);

  point_exit[0] = solution[0] + center[0];
  point_exit[1] = solution[1] + center[1];

  //  cout << point_exit[0] << " " << point_exit[1] << endl;

}

