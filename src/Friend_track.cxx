#include "TFile.h"
#include "TClonesArray.h"
#include "TH2.h"
#include "TLine.h"
#include "TTree.h"
#include "TBenchmark.h"
#include "TRandom.h"
#include "Friend.h"
#include "TFriendElement.h"
#ifndef DEG2RAD
#define DEG2RAD 4.*atan(1.0)/180.
#endif
#ifndef RAD2DEG
#define RAD2DEG 180./4./atan(1.0)
#endif
#include <math.h>
#include "TMath.h"
#include "TMacro.h"
#include "TSystem.h"
#include "TCanvas.h"


#include "TEveTrackPropagator.h"
#include "TEveTrack.h"
#include "TEveVSDStructs.h"
#include "TEveManager.h"
#include "TEveViewer.h"
#include "TSystem.h"
#include "TGLViewer.h"

#include "TEveViewer.h"
#include "TEvePointSet.h"
#include "Riostream.h"
#include "TString.h"
#include "TObjString.h"
#include "TRandom2.h"
#include "TMath.h"
#include "TVector3.h"
#include "time.h"
#include "TTimeStamp.h"
//TEveTrackPropagator* g_prop;//=0; 

Friend::Friend(const char* namefile1,const char* namefile2,
				   const char* out):nameoutfile(out)
{
}
Friend::Friend(const char* namefile1,
	       const char* namefile2,
	       const char* namefile3,
	       const char* out):nameoutfile(out)
{
}

//Friend::Friend(const char* out,const char* outtreename,int iii):nameoutfile(out)
//{
//}
Friend::Friend(const char* namefile1,
	       const char* namefile2,
	       const char* namefile3,
	       const char* out,
	       const char* out2,int iii):nameoutfile(out)
{

  const std::string funcname = "[Friend::Friend]:default: for simple " ;

  MaxEvent = iii;
  std::ifstream TofList(namefile1);
  std::ifstream FiberList(namefile2);
  std::ifstream ChamberList(namefile3);


  nameoutfile2 = out2;

  std::cout<<funcname<<" Tree Output File="<<nameoutfile<<std::endl; 
  std::cout<<funcname<<" Histogram Output File ="<<nameoutfile2<<std::endl;


  
  if(!CreateHistogram())exit(1);
  
  chain_fib = new TChain("EventsFiber");
  chain_tof = new TChain("EventsTof");
  chain_cha = new TChain("EventsChamber");
  std::string infile;


  while(!TofList.eof())
    {
      TofList >> infile;
      if(!TofList.eof())
	{
	chain_tof->Add(infile.c_str());
	cout<<infile<<endl;
	}
    }
  TofList.close();

  while(!FiberList.eof()){
    FiberList >> infile;
    if(!FiberList.eof())
      {
    chain_fib->Add(infile.c_str());
	cout<<infile<<endl;
      }
  }
  FiberList.close();

  while(!ChamberList.eof()){
    ChamberList >> infile;
    if(!ChamberList.eof())
      {
      chain_cha->Add(infile.c_str());
	cout<<infile<<endl;
      }
  }
  ChamberList.close();


  
  cout<<" fiber "<< chain_fib->GetEntries()<<endl;
  cout<<" chamber "<< chain_cha->GetEntries()<<endl;
  cout<<" tof "<< chain_tof->GetEntries()<<endl;
  if( (chain_tof->GetEntries() !=chain_fib->GetEntries()) ||
      (chain_tof->GetEntries() !=chain_cha->GetEntries()) ||
      (chain_cha->GetEntries() !=chain_fib->GetEntries()) )
    {
      cout<<"------------------------------------------------"<<endl;
      cout<<" Entries for each file is not same !!!!!!"<<endl;      
      cout<<"------------------------------------------------"<<endl;
      exit(-1);
    }

  Tin = chain_fib;
  Tin->AddFriend(chain_tof);
  Tin->AddFriend(chain_cha);


//   TTimeStamp ff;
//   UInt_t yy,mm,dd;
//   Now=ff.GetDate(true,0,&yy,&mm,&dd);
//   cout<<"Now "<<Now<<endl;

#ifdef TREEOUT
  if(!CreateTree())exit(1);
#endif
  
  
}

Friend::Friend(const char* namefile1,
	       const char* out,
	       const char* out2,int iii):nameoutfile(out)
{

  const std::string funcname = "[Friend::Friend]:Merged : " ;

  MaxEvent = iii;
  std::ifstream MergedList(namefile1);

  nameoutfile2 = out2;

  std::cout<<funcname<<" Tree Output File="<<nameoutfile<<std::endl; 
  std::cout<<funcname<<" Histogram Output File ="<<nameoutfile2<<std::endl;


  
  if(!CreateHistogram())exit(1);
  
  chain_merged = new TChain("EventsPhysNew");

  std::string infile;


  while(!MergedList.eof())
    {
      MergedList >> infile;
      if(!MergedList.eof())
	{
	chain_merged->Add(infile.c_str());
	cout<<infile<<endl;
	}
    }
  MergedList.close();


  
  cout<<" Merged "<< chain_merged->GetEntries()<<endl;

  Tin = chain_merged;
//   TTimeStamp ff;
//   UInt_t yy,mm,dd;
//   Now=ff.GetDate(true,0,&yy,&mm,&dd);
//   cout<<"Now "<<Now<<endl;
  
#ifdef TREEOUT
  if(!CreateTreeTrack())exit(1);
#endif
  
  
}

Friend::Friend(const char* namefile1,
	       const char* out,int iii):nameoutfile(out)
{

  const std::string funcname = "[Friend::Friend]:Track : " ;

  MaxEvent = iii;
  std::ifstream TrackList(namefile1);

  nameoutfile2 = out;
  std::cout<<funcname<<" Histogram Output File ="<<nameoutfile2<<std::endl;

  outfile2=0;

  
  if(!CreateHistogram())exit(1);
  
  chain_merged = new TChain("EventsPhysNew");
  chain_track = new TChain("EventsPhysTrack");

  std::string infile;

  cout<<"List file "<<namefile1<<endl;
  
  while(!TrackList.eof())
    {
      TrackList >> infile;
      if(!TrackList.eof())
	{
	cout<<infile<<endl;
	chain_merged->Add(infile.c_str());
	chain_track->Add(infile.c_str());

	}
    }
  TrackList.close();


  
  cout<<" Merged "<< chain_merged->GetEntries()<<endl;
  cout<<" Track "<< chain_track->GetEntries()<<endl;

  Tin2 = chain_merged;
  Tin = chain_track;
  

}
Bool_t Friend::WriteFile()
{ 

  char tempfile[300];
  //  std::string host=gSystem->HostName();
  sprintf(tempfile,"Temp_OutFile_%i_%i_%s_%s",Now,HMS,HostName.c_str(),nameoutfile2.Data());
  
  TFile *ff = new TFile(tempfile,"RECREATE");
  cout<<"File = "<<tempfile<<endl;
  ff->cd();

  h_Progress->Write();
  h_StepProc->Write();
  htp_TimeOfFlightPos->Write();
  htp_TimeOfFlightPosT->Write();
  htp_TimeOfFlightPosB->Write();
  h_xy_tofplus->Write();
  h_xy_plusplus->Write();
  htp_Cl2Cor->Write();
  htp_Cl2CorT->Write();
  htp_Cl2CorTQ->Write();
  htp_Cl2CorB->Write();
  htp_Cl2CorBQ->Write();
  htp_HitPatTopT->Write();
  htp_HitPatTopT_Mul->Write();
  htp_HitPatTopQ->Write();
  htp_HitPatBotT->Write();
  htp_HitPatBotT_Mul->Write();
  htp_HitPatBotQ->Write();
  htp_HitPatTDC->Write();
  htp_HitPatAll->Write();
  htp_HitPatAll_Mul->Write();

  for(int i=0;i<31;i++)
    {
      htp_TimeDiffCl_T[i]->Write();
      htp_TimeDiffCl_B[i]->Write();
    }


  for(int i=0;i<4;i++)
    {
      h_PosCor_LandAladin[i]->Write();
    }

  h_PosCor_LandAladinXX[0]->Write();
  h_PosCor_LandAladinXX[1]->Write();
  h_PosCor_LandAladinXX_time[0]->Write();
  htpp_Hit2Cor->Write();
  htpp_TimeOfFlight->Write();
  htpp_TimeOfFlightPos->Write();
  for(int i=0;i<10;i++)
    {
      hts_min_tofsE[i]->Write();
    }

#ifdef CALIBCHECK
  for(int i=0;i<10;i++)
    {
      h_TOF_S[i]->Write();
    }
#endif

#ifdef NEGTRACK
  for(int i=0;i<10;i++)
    {
      h_rho_beta_bg_v[i]->Write();
      h_mass_bg_best_v[i]->Write();
      h_mass_bg_best_bar_v[i]->Write();
    }


  h_PosCor_LandAladinXX_aftTrack->Write();
  h_PosCor_LandAladinXX_aftTrack_wc->Write();
  h_PosCor_LandAladinXX_aftTrack_wc_wp->Write();
  h_PosCor_LandAladinXX_aftTrack_atAl->Write();
  h_PosCor_LandAladinXX_aftTrack_atAl1D->Write();
  h_PosCor_LandAladinXX_aftTrack_atAl1D_wc->Write();
  h_PosCor_LandAladinXX_aftTrack_atAl1D_wc_wp->Write();
  h_PosCor_LandAladinXX_aftTrack_cut1->Write();
  h_PosCor_LandAladinXX_aftTrack_cut2->Write();
  h_PosCor_LandAladinYY_aftTrack->Write();
  h_rho_beta_bg_best_all->Write();
  h_rho_beta_bg_best_all_alcoin1->Write();
  h_rho_beta_bg_best_all_alcoin2->Write();
  h_rho_beta_bg_best_all_alcoin1_wc->Write();
  h_rho_beta_bg_best_all_alcoin2_wc->Write();
  h_rho_beta_bg_best_all_alcoin1_wc_wp->Write();
  h_rho_beta_bg_best_all_alcoin2_wc_wp->Write();
  h_rho_beta_albg_best_all->Write();
#endif

#ifdef POSTRACK
  h_mom_new_cor->Write();
  h_mom_new_diff->Write();
  h_shift_angle->Write();
  h_rho_beta_all_Z1->Write();
  h_rho_beta_all_Z1_tofs->Write();
  h_rho_beta_all_Z1_new->Write();
  h_rho_beta_all_Z1_tofs_new->Write();
  h_rho_beta_all_Z1_tofs_new_r->Write();
  h_rho_beta_all_Z1_tofs_new_l->Write();
  h_rho_beta_all_Z1_tofs_new_m->Write();

  h_track_id_coin_que_x_tp->Write();
  h_track_id_coin_que_x_tp_coin->Write();
  h_track_id_coin_que_xp_tp->Write();
  h_track_id_coin_que_xp_tp_coin->Write();
  h_track_id_coin_que_y_tp->Write();
  h_track_id_coin_que_y_tp_coin->Write();
  h_track_id_coin_que_yp_tp->Write();
  h_track_id_coin_que_yp_tp_coin->Write();
  h_track_id_coin_que_u_tp->Write();
  h_track_id_coin_que_u_tp_coin->Write();

  for(int i=0;i<4;i++)
    {
      h_recmass_best[i]->Write();
    }
  for(int i=0;i<32;i++){
    h_recmass_best_E[i]->Write();
    h_recmass_best_T[i]->Write();
    h_rho_beta_best[i]->Write();
    h_rho_beta_best_r[i]->Write();
    h_rho_beta_best_l[i]->Write();
    h_rho_beta_best_m[i]->Write();
  }
  h_recmass_best_posx[0]->Write();
  h_recmass_best_proton->Write();
  h_rho_beta_all_Z1_proton->Write();
  h_recmass_best_tofs_proton->Write();
  h_recmass_best_new->Write();
  h_recmass_best_tofs->Write();
  h_recmass_best_tofs_new->Write();
  h_recmass_best_tr0->Write();
  h_rho_beta_all_Z1_tofs_proton->Write();
  h_recmass_best_tofsZ2->Write();
  h_recmass_best_tofs_newZ2->Write();
  h_recmass_best_tofsZ2_rho->Write();
  h_recmass_tofs->Write();
  h_recmass_best_tofsbar->Write();
  for(int i=0;i<10;i++)
    {
      h_recmass_best_tofstime[i]->Write();
      h_recmass_best_tofsene[i]->Write();
    }
  for(int i=0;i<5;i++)
    {
      h_recmass_best_v[i]->Write();
    }

  h_recmass_best_p1x->Write();
  h_recmass_best_p2x->Write();

  h_recmass_best_p1y->Write();
  h_recmass_best_p2y->Write();
  h_recmass_best_chi->Write();
  h_recmass_best_chi_bar->Write();
  h_PathLengthPos->Write();
  h_PathLengthMass->Write();
  h_pq_E_best_all->Write();
  h_pq_E_best_all_tofs->Write();
#endif

#ifdef POSRESFIB    	
  for(int i=0;i<3;i++)
    {
      h_PosResX[i]->Write();
      h_PosResY[i]->Write();
      h_PosResX_angle[i]->Write();
      h_PosResY_angle[i]->Write();
      h_PosResX_pos[i]->Write();
      h_PosResY_pos[i]->Write();
    }
#endif


#ifdef TRACKCHECK     
  for(int i=0;i<18;i++)
    {
      h_track_bg_Id[i]->Write();
      h_track_bg_Id_best[i]->Write();
      h_track_bg_Id_diff_time[i]->Write();
      h_track_bg_Id_timecut[i]->Write();
    }
  for(int i=0;i<32;i++)
    {
      h_track_tofp_Id_best[i]->Write();
      h_track_tofp_Id_best_resd[i]->Write();
//       h_track_tofp_Id[i]->Write();
      h_track_tofp_Id1D[i]->Write();
      h_track_tofp_Id1D_c[i]->Write();
    }
  h_track_id_coin_que_x_tp->Write();
  h_track_id_coin_que_xp_tp->Write();
  h_track_id_coin_que_y_tp->Write();
  h_track_id_coin_que_yp_tp->Write();
  h_track_id_coin_que_u_tp->Write();
  
  h_track_id_coin_que_x_tp_coin->Write();
  h_track_id_coin_que_xp_tp_coin->Write();
  h_track_id_coin_que_y_tp_coin->Write();
  h_track_id_coin_que_yp_tp_coin->Write();
  h_track_id_coin_que_u_tp_coin->Write();

  h_track_id_coin_que_x_bt->Write();
  h_track_id_coin_que_xp_bt->Write();
  h_track_id_coin_que_y_bt->Write();
  h_track_id_coin_que_yp_bt->Write();
  h_track_id_coin_que_u_bt->Write();

  h_track_id_coin_que_x_bt_coin->Write();
  h_track_id_coin_que_xp_bt_coin->Write();
  h_track_id_coin_que_y_bt_coin->Write();
  h_track_id_coin_que_yp_bt_coin->Write();
  h_track_id_coin_que_u_bt_coin->Write();

  h_track_id_coin_que_x_albt->Write();
  h_track_id_coin_que_xp_albt->Write();
  h_track_id_coin_que_y_albt->Write();
  h_track_id_coin_que_yp_albt->Write();
  h_track_id_coin_que_u_albt->Write();

  h_track_id_coin_que_x_albt_coin->Write();
  h_track_id_coin_que_xp_albt_coin->Write();
  h_track_id_coin_que_y_albt_coin->Write();
  h_track_id_coin_que_yp_albt_coin->Write();
  h_track_id_coin_que_u_albt_coin->Write();
#endif




  h_newinv_mass_l_all->Write();
  for(int i=0;i<20;i++)
    {
      h_newinv_mass_l_v[i]->Write();
    }
  for(int i=0;i<10;i++)
    {
      h_newinv_mass_a3p_v[i]->Write();
      h_newinv_mass_a4p_v[i]->Write();
      h_newinv_secz_cut[i]->Write();
      h_newinv_seczx_cut[i]->Write();

    }
  h_newinv_secz->Write();
  h_newinv_secz_mompi->Write();
  h_newinv_secz_momp->Write();
  h_newinv_seczx->Write();
  h_newinv_seczy->Write();
  h_newinv_dist->Write();
  h_newinv_mass_l->Write();
  h_newinv_mass_l_kin->Write();
  h_newinv_mass_l_tx->Write();
  h_newinv_mass_l_ty->Write();
  h_newinv_mass_l_mompi->Write();
  h_newinv_mass_l_momp->Write();
  h_newinv_mass_l_ydis->Write();
  h_newinv_mass_l_xdis->Write();
  h_newinv_mass_l_tpbar->Write();
  h_newinv_mass_l_bgbar->Write();

  h_newinv_mass_l2->Write();
  h_newinv_mass_l_pmass->Write();
  h_newinv_mass_l_pimass->Write();
  h_newinv_mass_l_pmass->Write();
  h_newinv_pmass->Write();
  h_newinv_pimass->Write();
  h_newinv_mass_dist_l->Write();
  h_newinv_mass_secz_l->Write();
  h_newinv_dist_secz_l->Write();


  h_newinv_secz_a3p->Write();
  h_newinv_dist_a3p->Write();
  h_newinv_mass_a3p->Write();
  h_newinv_mass_dist_a3p->Write();
  h_newinv_mass_secz_a3p->Write();

 h_newinv_secz_a4p->Write();
  h_newinv_dist_a4p->Write();
  h_newinv_mass_a4p->Write();
  h_newinv_mass_dist_a4p->Write();
  h_newinv_mass_secz_a4p->Write();

  delete ff;
  cout<<"Histogram written in "<<tempfile<<endl;
  return true;
}


Bool_t Friend::CreateTree()
{
  std::string funcname = "Friend::CreateTree";
  cout<<funcname<<endl;

   outfile2 = new TFile(nameoutfile,"RECREATE");
   outfile2->cd();
   outfile2->SetCompressionLevel(5);
   std::cout<<" file for tree"<<std::endl;
   Event_Out = 0;
   Tout = new TTree("EventsPhysNew","Events");
   Tout->Branch("THyphiEvent_Phys_new",&Event_Out,64000,2);
   Tout->SetAutoSave(10000000);
   Tout->SetMaxTreeSize(1000000000); //1 GB
   std::cout<<" THyphiEvent_Phys_new"<<std::endl;
   Event_Track_Out = 0;
   Tout_Track = new TTree("EventsPhysTrack","Events");
   Tout_Track->Branch("THyphiEvent_Phys_track",&Event_Track_Out,64000,2);
   Tout_Track->SetAutoSave(10000000);

   std::cout<<" THyphiEvent_Phys_track"<<std::endl;
   //Tout_Track->SetMaxTreeSize(500000000); //1 GB
   
   //   if(!WriteSourceCode(outfile2))return false;
   //   outfile2->Write();
   return true;;

}

Bool_t Friend::CreateTreeTrack()
{
  std::string funcname = "Friend::CreateTreeTrack ";
  cout<<funcname<<endl;

   outfile2 = new TFile(nameoutfile,"RECREATE");
   outfile2->cd();
   outfile2->SetCompressionLevel(5);

   Event_Track_Out = 0;
   Tout_Track = new TTree("EventsPhysTrack","Events");
   Tout_Track->Branch("THyphiEvent_Phys_track",&Event_Track_Out,64000,2);
   //    Tout_Track->SetAutoSave(10000000);
   //    Tout_Track->SetMaxTreeSize(1000000000); //1 GB
   //Tout_Track->SetMaxTreeSize(1000000); //1 GB
   
   //   if(!WriteSourceCode(outfile2))return false;
   //   outfile2->Write();
   return true;;

}

Bool_t Friend::WriteSourceCode(TFile *file)
{

  file->cd();
  //const char *dirname = ".";
  const char *dirname = gSystem->pwd();
  char *slash = (char *)strrchr(dirname,'/');
  char *locdir;
  
  if(slash)locdir = slash+1;
  else locdir = (char *)dirname;
  
  printf("processing dir %s\n",dirname);
  
  TDirectory *savdir = gDirectory;
  TDirectory *adir = savdir->mkdir(locdir);
  adir->cd();
  
  void *dirp = gSystem->OpenDirectory(dirname);
  if(!dirp)return false;
  
  char *direntry;
  Long_t id,size,flags,modtime;
  while((direntry=(char *)gSystem->GetDirEntry(dirp)))
    {
      TString afile = Form("%s/%s",dirname,direntry);
      gSystem->GetPathInfo(afile,&id,&size,&flags,&modtime);
      //      cout<<modtime<<endl;
      if(direntry[0]=='.')continue;
      if(!strcmp(direntry,"CVS"))continue;
      if(!strcmp(direntry,"htmldoc"))continue;
      if(strstr(direntry,"root/include"))continue;
      if(strstr(direntry,"G__"))continue;
      if(strstr(direntry,".root"))continue;
      if(strstr(direntry,"~") )continue;
      
      if(strstr(direntry,".C") ||
	 strstr(direntry,".cxx")||
	 strstr(direntry,".h") ||
	 strstr(direntry,"EventClass/")
	 //strstr(direntry,".dat")
	 )
	{
	  cout<<direntry<<endl;     
	  TMacro *m=new TMacro(afile);
	  m->Write(direntry);
	  delete m;
	}
      else
	{
	  if(flags!=3) continue;
	  //	 importdir(afile);	   
	}
      
    }
  cout<<"finished"<<endl;
  gSystem->FreeDirectory(dirp);
  savdir->cd();
  //  file->Write();

//   time_t now = time(NULL);
  
//   cout<<"time = "<<ctime(&now)<<endl;
  return true;
}

Bool_t Friend::FillStepProc(std::string step)
{
  h_StepProc->Fill(step.c_str(),1);
  return true;
}

Bool_t Friend::FillProgress(double prog)
{
  h_Progress->Fill(prog);
  return true;
}
Bool_t Friend::WriteHistogram(int all)
{
  std::string funcname = "Friend::WriteHistogram()";
  cout<<funcname;
  outfile->cd();

  outfile->Write("",TObject::kOverwrite);

  if(all>0)
    outfile->Close();
  cout<<" done "<<endl;
  return true;
}

Bool_t Friend::WriteTree()
{
  std::string funcname = "Friend::WriteTree()";
  cout<<funcname;
  outfile2 = Tout->GetCurrentFile();
  outfile2->cd();
  Tout->Write();
  Tout_Track->Write();
  cout<<" cd ";
  outfile2->Write();
  cout<<" Write ";
  outfile2->Close();
  cout<<" Close "<<endl;
  return true;
}


Friend::~Friend()
{
  const std::string funcname = "[Friend::~Friend] :";

  
  WriteHistogram(1);
  delete outfile;

#ifdef TREEOUT
  outfile2 = Tout->GetCurrentFile();
  if(outfile2)
    {
      //if(!WriteSourceCode(outfile2))return false;
      
      WriteSourceCode(outfile2);
      WriteTree();
      delete outfile2;
    }
#endif
  if (!T) delete T;
  if(!Event_Out) delete Event_Out;
  if(!Event_Track_Out) delete Event_Track_Out;
  if(!Event_Tof) delete   Event_Tof;
  if(!Event_Fiber) delete Event_Fiber;
  if(!Event_Chamber) delete Event_Chamber;
  //  if(!Event_Phys) delete Event_Phys;
  if(!Tout)delete Tout;
  if(!Tout_Track)delete Tout_Track;


  std::cout<<funcname<<" Delete instance"<<std::endl;
}

Bool_t Friend::InitLoop()
{
  string funcname = "InitLoop : ";

#ifdef RK  
  if(Init_RK()!=true) return false;;
#endif  


  Event_Tof = 0;
  Event_Fiber = 0;
  Event_Chamber = 0;

  Tin->SetBranchAddress("THyphiEvent_TOF",&Event_Tof);
  Tin->SetBranchAddress("THyphiEvent_Fiber",&Event_Fiber);
  Tin->SetBranchAddress("THyphiEvent_Chamber",&Event_Chamber);
  
  nentries = Tin->GetEntries();
  nb=0;
  NE =0;
  
  if(MaxEvent)
    {
      if(MaxEvent<nentries)
 	NE = MaxEvent;
      else
 	NE =nentries;
    }
  else
    {
      NE=nentries;
    }
  
  cout<<"analyze "<<NE<<" events"<<endl;
  en_temp=0;
  en_temp1=0;

  return true;
}
Bool_t Friend::InitLoopMerged()
{
  string funcname = "InitLoopMerged : ";

#ifdef RK  
  if(Init_RK()!=true) return false;
#endif  


  Event_Out = 0;
  Tin->SetBranchAddress("THyphiEvent_Phys_new",&Event_Out);
  
  nentries = Tin->GetEntries();
  nb=0;
  NE =0;
  
  if(MaxEvent)
    {
      if(MaxEvent<nentries)
 	NE = MaxEvent;
      else
 	NE =nentries;
    }
  else
    {
      NE=nentries;
    }
  
  cout<<funcname<<"analyze "<<NE<<" events"<<endl;
  en_temp=0;
  en_temp1=0;
  return true;
}
Bool_t Friend::InitLoopTrack()
{
  string funcname = "InitLoopTrack : ";

  Event_Track_Out = 0;
  Tin->SetBranchAddress("THyphiEvent_Phys_track",&Event_Track_Out);
  nentries = Tin->GetEntries();

  Event_Out = 0;
  Tin2->SetBranchAddress("THyphiEvent_Phys_new",&Event_Out);


  nb=0;
  NE =0;
  
  if(MaxEvent)
    {
      if(MaxEvent<nentries)
 	NE = MaxEvent;
      else
 	NE =nentries;
    }
  else
    {
      NE=nentries;
    }
  
  cout<<funcname<<"analyze "<<NE<<" events"<<endl;
  en_temp=0;
  en_temp1=0;
  return true;
}
Bool_t Friend::RawAnalysis(int en)
{
  string funcname = "RawAnalysis : ";
  NHits();

  const int trigger_type=Event_Tof->trigger_type;

  trig_f = 0;
  if(trigger_type == 1)trig_f = 0;
  else if(trigger_type == 10)trig_f = 1;
  else if(trigger_type == 20)trig_f = 2;
  else if(trigger_type == 30)trig_f = 3;

  h_trigger->Fill(trigger_type);
  h_trigger->Fill(trig_f+50);
  
  if(en%1000==0) en_temp++;
  if(en%100000==0)en_temp1++;
  
  HitPat();

#ifdef FIBERSTUDY
  FiberStudy();
#endif

#ifdef TOFSSTUDY
  TofStartStudy();
#endif

#ifdef TOFPSTUDY
  TofPlusStudy();
#endif

#ifdef TOFPPSTUDY
  TofPlusPlusStudy();
#endif
  return true;
}//Bool_t Friend::RawAnalysis(int en)

Bool_t Friend::Proc(int en)
{
  string funcname = "Proc : ";

#ifdef TREEOUT
  Event_Out->Clear();
  Event_Out->fEventHeader = Event_Fiber->fEventHeader;
  Event_Out->trigger_type = Event_Fiber->trigger_type;
#endif

  if(Event_Fiber->fEventHeader.GetNameFileOrigin()!=
     Event_Tof->fEventHeader.GetNameFileOrigin())
    {
      cout<<"Not Syncro "<<endl;
    }

  if(Event_Fiber->fEventHeader.GetNameFileOrigin()!=
     Event_Chamber->fEventHeader.GetNameFileOrigin())
    {
      cout<<"Not Syncro "<<endl;

    }

  if(Event_Fiber->fEventHeader.GetNameFileOrigin()!=
     Event_Tof->fEventHeader.GetNameFileOrigin())
    {
      cout<<"Not Syncro "<<endl;
    }

  if(Event_Fiber->fEventHeader.GetNameFileOrigin()!=
     Event_Chamber->fEventHeader.GetNameFileOrigin())
    {
      cout<<"Not Syncro "<<endl;

    }
  

  for(int i=0;i<Event_Fiber->n_TR0_x;i++)
    {
      THyphiFiberClusterChannel temp_x;
      Event_Fiber->Get_Channel(i,"TR0_x",temp_x);
      THyphiHit_new temp;
      double temp_xx = 0.0;
      int temp_xx_size = 0;
      for(size_t xx =0;xx<temp_x.TDC_LE.size();xx++)
	{
	  if(temp_x.TDC_LE[xx]>0)
	    {
	      temp_xx += temp_x.TDC_LE[xx];
	      temp_xx_size++;
	    }
	}
      temp.t = temp_xx/temp_xx_size;
      temp.x=temp_x.Position;
      temp.z=temp_x.Z;
      temp.E=temp_x.E;
      double temp_time=0;
      for(size_t cl=0;cl<temp_x.TDC_LE.size();cl++)
	{
	  if(temp_x.TDC_LE[cl]>0)
	    {
	      temp_time+=temp_x.TDC_LE[cl];
	    }
	}
      temp.t = temp_time/temp_x.TDC_LE.size();
      temp.Id_x = temp_x.Id;
      temp.Le_x=temp_x.TDC_LE;
      temp.Ene_x=temp_x.QDC;
      temp.Wid_x=temp_x.Width;
      Event_Out->Add_Hit(temp,"Tr0x");
    }
#ifdef DEBUG
  cout<<"TR0x ";
#endif
  for(int i=0;i<Event_Fiber->n_TR0_y;i++)
    {
      THyphiFiberClusterChannel temp_y;
      Event_Fiber->Get_Channel(i,"TR0_y",temp_y);
      THyphiHit_new temp;
      double temp_yy = 0.0;
      int temp_yy_size = 0;
      
      for(size_t yy =0;yy<temp_y.TDC_LE.size();yy++)
	{
	  if(temp_y.TDC_LE[yy]>0)
	    {
	      
	      temp_yy += temp_y.TDC_LE[yy];
	      temp_yy_size++;
	    }
	}	    
      temp.t = temp_yy/temp_yy_size;
      temp.y=temp_y.Position;
      temp.z=temp_y.Z;
      temp.E=temp_y.E;
      temp.Id_y = temp_y.Id;
      temp.Le_y=temp_y.TDC_LE;
      temp.Ene_y=temp_y.QDC;
      temp.Wid_y=temp_y.Width;
      Event_Out->Add_Hit(temp,"Tr0y");
    }
#ifdef DEBUG
  cout<<"TR0y ";
#endif
  
  for(int i=0;i<Event_Fiber->n_TR1_x;i++)
    {
      THyphiFiberClusterChannel temp_x;
      Event_Fiber->Get_Channel(i,"TR1_x",temp_x);
      
      THyphiHit_new temp;
      double temp_xx = 0.0;
      int temp_xx_size = 0;
      for(size_t xx =0;xx<temp_x.TDC_LE.size();xx++)
	{
	  if(temp_x.TDC_LE[xx]>0)
	    {
	      temp_xx += temp_x.TDC_LE[xx];
	      temp_xx_size++;
	    }
	}
      temp.t = temp_xx/temp_xx_size;
      temp.x=temp_x.Position;
      temp.z=temp_x.Z;
      temp.E=temp_x.E;
      temp.Id_x = temp_x.Id;
      temp.Le_x=temp_x.TDC_LE;
      temp.Wid_x=temp_x.Width;
      Event_Out->Add_Hit(temp,"Tr1x");
    }
#ifdef DEBUG
  cout<<"TR1x ";
#endif
  
  for(int i=0;i<Event_Fiber->n_TR1_y;i++)
    {
      THyphiFiberClusterChannel temp_y;
      Event_Fiber->Get_Channel(i,"TR1_y",temp_y);
      
      THyphiHit_new temp;
      double temp_yy = 0.0;
      int temp_yy_size = 0;
      
      for(size_t yy =0;yy<temp_y.TDC_LE.size();yy++)
	{
	  if(temp_y.TDC_LE[yy]>0)
	    {
	      
	      temp_yy += temp_y.TDC_LE[yy];
	      temp_yy_size++;
	    }
	}	    
      temp.t = temp_yy/temp_yy_size;
      temp.y=temp_y.Position;
      temp.z=temp_y.Z;
      temp.E=temp_y.E;
      temp.Id_y = temp_y.Id;
      temp.Le_y=temp_y.TDC_LE;
      temp.Wid_y=temp_y.Width;
      Event_Out->Add_Hit(temp,"Tr1y");
    }
#ifdef DEBUG
  cout<<"TR1y ";
#endif
  
  for(int i=0;i<Event_Fiber->n_TR2_x;i++)
    {
      THyphiFiberClusterChannel temp_x;
      Event_Fiber->Get_Channel(i,"TR2_x",temp_x);
      
      THyphiHit_new temp;
      double temp_xx = 0.0;
      int temp_xx_size = 0;
      for(size_t xx =0;xx<temp_x.TDC_LE.size();xx++)
	{
	  if(temp_x.TDC_LE[xx]>0)
	    {
	      temp_xx += temp_x.TDC_LE[xx];
	      temp_xx_size++;
	    }
	}
      temp.t = temp_xx/temp_xx_size;
      temp.x=temp_x.Position;
      temp.z=temp_x.Z;
      temp.E=temp_x.E;
      temp.Id_x = temp_x.Id;
      temp.Le_x=temp_x.TDC_LE;
      temp.Wid_x=temp_x.Width;
      Event_Out->Add_Hit(temp,"Tr2x");
    }
#ifdef DEBUG
  cout<<"TR2x ";
#endif
  
  for(int i=0;i<Event_Fiber->n_TR2_y;i++)
    {
      THyphiFiberClusterChannel temp_y;
      Event_Fiber->Get_Channel(i,"TR2_y",temp_y);
      
      THyphiHit_new temp;
      double temp_yy = 0.0;
      int temp_yy_size = 0;
      
      for(size_t yy =0;yy<temp_y.TDC_LE.size();yy++)
	{
	  if(temp_y.TDC_LE[yy]>0)
	    {
	      
	      temp_yy += temp_y.TDC_LE[yy];
	      temp_yy_size++;
	    }
	}	    
      temp.t = temp_yy/temp_yy_size;
      
      temp.y=temp_y.Position;
      temp.z=temp_y.Z;
      temp.E=temp_y.E;
      temp.Id_y = temp_y.Id;
      temp.Le_y=temp_y.TDC_LE;
      temp.Wid_y=temp_y.Width;
      Event_Out->Add_Hit(temp,"Tr2y");
    }
#ifdef DEBUG
  cout<<"TR2y ";
#endif
  
#ifdef DEBUG
  cout<<"Filling Fibers done"<<endl;
#endif
  
  

#ifdef DEBUG
  cout<<" CbM"<<endl;
  cout<<"Event_Chamber->n_TR2_x ="<<Event_Chamber->n_CbM_x<<endl;
  cout<<"Event_Chamber->n_TR2_y ="<<Event_Chamber->n_CbM_xp<<endl;
#endif
  
  //Fill Chambers
  for(int i=0;i<Event_Chamber->n_CbM_x;i++)
    {
      THyphiChamberChannel temp_cham;
      Event_Chamber->Get_Channel(i,"CbM_x",temp_cham);
      THyphiHit_new temp;
      for(size_t k=0;k<temp_cham.Id.size();k++)
	{
	  temp.z =PrinzPos+PrinPlOffs[0];
	  temp.Id_x.push_back(temp_cham.Id[k]);
	  temp.Le_x.push_back(temp_cham.TDC_LE[k]);
	  Event_Out->Add_Hit(temp,"CbMx");
	  
	}
    }
  for(int i=0;i<Event_Chamber->n_CbM_xp;i++)
    {
      THyphiChamberChannel temp_cham;
      Event_Chamber->Get_Channel(i,"CbM_xp",temp_cham);
      THyphiHit_new temp;
      for(size_t k=0;k<temp_cham.Id.size();k++)
	{
	  temp.z =PrinzPos+PrinPlOffs[1];
	  temp.Id_x.push_back(temp_cham.Id[k]);
	  temp.Le_x.push_back(temp_cham.TDC_LE[k]);
	  Event_Out->Add_Hit(temp,"CbMxp");
	  
	}
    }
  
#ifdef DEBUG
  cout<<"Event_Chamber->n_CbM_u ="<<Event_Chamber->n_CbM_u<<endl;
  cout<<"Event_Chamber->n_CbM_up="<<Event_Chamber->n_CbM_up<<endl;
#endif
  
  for(int i=0;i<Event_Chamber->n_CbM_u;i++)
    {
      THyphiChamberChannel temp_cham;
      Event_Chamber->Get_Channel(i,"CbM_u",temp_cham);
      THyphiHit_new temp;
      for(size_t k=0;k<temp_cham.Id.size();k++)
	{
	  temp.z =PrinzPos+PrinPlOffs[2];
	  temp.Id_x.push_back(temp_cham.Id[k]);
	  temp.Le_x.push_back(temp_cham.TDC_LE[k]);
	  Event_Out->Add_Hit(temp,"CbMu");
	  
	}
    }
  for(int i=0;i<Event_Chamber->n_CbM_up;i++)
    {
      THyphiChamberChannel temp_cham;
      Event_Chamber->Get_Channel(i,"CbM_up",temp_cham);
      THyphiHit_new temp;
      for(size_t k=0;k<temp_cham.Id.size();k++)
	{
	  temp.z =PrinzPos+PrinPlOffs[3];
	  temp.Id_x.push_back(temp_cham.Id[k]);
	  temp.Le_x.push_back(temp_cham.TDC_LE[k]);
	  Event_Out->Add_Hit(temp,"CbMup");
	  
	}
    }
#ifdef DEBUG
  cout<<"Event_Chamber->n_CbM_v ="<<Event_Chamber->n_CbM_v<<endl;
  cout<<"Event_Chamber->n_CbM_vp="<<Event_Chamber->n_CbM_vp<<endl;
#endif
  
  
  for(int i=0;i<Event_Chamber->n_CbM_v;i++)
    {
      THyphiChamberChannel temp_cham;
      Event_Chamber->Get_Channel(i,"CbM_v",temp_cham);
      THyphiHit_new temp;
      for(size_t k=0;k<temp_cham.Id.size();k++)
	{
	  temp.z =PrinzPos+PrinPlOffs[4];
	  temp.Id_x.push_back(temp_cham.Id[k]);
	  temp.Le_x.push_back(temp_cham.TDC_LE[k]);
	  Event_Out->Add_Hit(temp,"CbMv");
	  
	}
    }
  for(int i=0;i<Event_Chamber->n_CbM_vp;i++)
    {
      THyphiChamberChannel temp_cham;
      Event_Chamber->Get_Channel(i,"CbM_vp",temp_cham);
      THyphiHit_new temp;
      for(size_t k=0;k<temp_cham.Id.size();k++)
	{
	  temp.z =PrinzPos+PrinPlOffs[5];
	  temp.Id_x.push_back(temp_cham.Id[k]);
	  temp.Le_x.push_back(temp_cham.TDC_LE[k]);
	  Event_Out->Add_Hit(temp,"CbMvp");
	  
	}
    }
  
#ifdef DEBUG
  cout<<"Filling Small chamber plane done"<<endl;
#endif

#ifdef DEBUG
  cout<<" CaM"<<endl;
  cout<<"Event_Chamber->n_CaM_x ="<<Event_Chamber->n_CaM_x<<endl;
  cout<<"Event_Chamber->n_CaM_xp="<<Event_Chamber->n_CaM_xp<<endl;
#endif
  
  for(int i=0;i<Event_Chamber->n_CaM_x;i++)
    {
      THyphiChamberChannel temp_cham;
      Event_Chamber->Get_Channel(i,"CaM_x",temp_cham);
      THyphiHit_new temp;
      for(size_t k=0;k<temp_cham.Id.size();k++)
	{
	  temp.z =QueenZpos+QueenPlOffs[0];
	  temp.Id_x.push_back(temp_cham.Id[k]);
	  temp.Le_x.push_back(temp_cham.TDC_LE[k]);
	  Event_Out->Add_Hit(temp,"CaMx");
	  
	}
    }
  for(int i=0;i<Event_Chamber->n_CaM_xp;i++)
    {
      THyphiChamberChannel temp_cham;
      Event_Chamber->Get_Channel(i,"CaM_xp",temp_cham);
      THyphiHit_new temp;
      for(size_t k=0;k<temp_cham.Id.size();k++)
	{
	  temp.z =QueenZpos+QueenPlOffs[1];
	  temp.Id_x.push_back(temp_cham.Id[k]);
	  temp.Le_x.push_back(temp_cham.TDC_LE[k]);
	  Event_Out->Add_Hit(temp,"CaMxp");
	  
	}
    }
#ifdef DEBUG
  cout<<"Event_Chamber->n_CaM_y ="<<Event_Chamber->n_CaM_y<<endl;
  cout<<"Event_Chamber->n_CaM_yp="<<Event_Chamber->n_CaM_yp<<endl;
  cout<<"Event_Chamber->n_CaM_u ="<<Event_Chamber->n_CaM_u<<endl;
#endif
  
  for(int i=0;i<Event_Chamber->n_CaM_y;i++)
    {
      THyphiChamberChannel temp_cham;
      Event_Chamber->Get_Channel(i,"CaM_y",temp_cham);
      THyphiHit_new temp;
      for(size_t k=0;k<temp_cham.Id.size();k++)
	{
	  temp.z =QueenZpos+QueenPlOffs[2];
	  temp.Id_x.push_back(temp_cham.Id[k]);
	  temp.Le_x.push_back(temp_cham.TDC_LE[k]);
	  Event_Out->Add_Hit(temp,"CaMy");
	  
	}
    }
  for(int i=0;i<Event_Chamber->n_CaM_xp;i++)
    {
      THyphiChamberChannel temp_cham;
      Event_Chamber->Get_Channel(i,"CaM_yp",temp_cham);
      THyphiHit_new temp;
      for(size_t k=0;k<temp_cham.Id.size();k++)
	{
	  temp.z =QueenZpos+QueenPlOffs[3];
	  temp.Id_x.push_back(temp_cham.Id[k]);
	  temp.Le_x.push_back(temp_cham.TDC_LE[k]);
	  Event_Out->Add_Hit(temp,"CaMyp");
	  
	}
    }
  for(int i=0;i<Event_Chamber->n_CaM_u;i++)
    {
      THyphiChamberChannel temp_cham;
      Event_Chamber->Get_Channel(i,"CaM_u",temp_cham);
      THyphiHit_new temp;
      for(size_t k=0;k<temp_cham.Id.size();k++)
	{
	  temp.z =QueenZpos+QueenPlOffs[4];
	  temp.Id_x.push_back(temp_cham.Id[k]);
	  temp.Le_x.push_back(temp_cham.TDC_LE[k]);
	  Event_Out->Add_Hit(temp,"CaMu");
	  
	}
    }
  
  for(int i=0;i<Event_Tof->n_TOFStartBar;i++)
    {                                                 
      THyphiTofBar tofbar_bar;
      Event_Tof->Get_Bar(i,"TOF_Start",tofbar_bar); 
      //double tdiff = tofbar_bar.TDC_t-tofbar_bar.TDC_b;
	{
	  double new_top=0;
	  double new_bot=0;
	  double posy_new=0;
	  double min_tofs=0;
	  if(!GetCalib("TOFS",tofbar_bar,new_top,new_bot,min_tofs,posy_new))continue;
	  THyphiHit_new temp;
	  temp.x = tofbar_bar.Position_x;
	  temp.y = tofbar_bar.Position_y;
	  temp.z = tofbar_bar.Position_z;
	  temp.E = tofbar_bar.Energy_high;
	  temp.t = min_tofs;
	  temp.t_bot = new_bot;
	  temp.t_top = new_top;
	  temp.Id_x.push_back(tofbar_bar.Id_bar);
	  temp.Id_y.push_back(tofbar_bar.Id_bar);
	  temp.PID = 0;
	  Event_Out->Add_Hit(temp,"TofStart");
	}
    }

  
  for(int i=0;i<Event_Tof->n_TOFPlusBar;i++)
    {
      THyphiTofBar tofbar_bar;
      Event_Tof->Get_Bar(i,"TOF_Plus",tofbar_bar); 
      double new_top=0;
      double new_bot=0;
      double posy_new=0;
      double time=0;
      if(!GetCalib("TOFP",tofbar_bar,new_top,new_bot,time,posy_new))continue;
      double NewE=0;
      int pid=0;
      if(!GetPID(tofbar_bar,NewE,pid))continue;
      
      double theta_tofp = 1.3*TMath::DegToRad();
      double x_rot_center = xxx;
      double z_rot_center = TofpzPos + zzz;

      double temp_x_rel_to_rot_center = tofbar_bar.Position_x + TofpXOffsBar[tofbar_bar.Id_bar-1] - x_rot_center;
      double temp_z_rel_to_rot_center = TofpzPos + TofpZOffsBar[tofbar_bar.Id_bar-1] - z_rot_center;
      double temp_x_after_rot = temp_x_rel_to_rot_center*cos(theta_tofp) - temp_z_rel_to_rot_center*sin(theta_tofp);
      double temp_z_after_rot = temp_x_rel_to_rot_center*sin(theta_tofp) + temp_z_rel_to_rot_center*cos(theta_tofp);

      THyphiHit_new temp;
      temp.x = temp_x_after_rot +x_rot_center ;
      temp.y = posy_new + ClYOffs[tofbar_bar.Id_bar-1] ;
      temp.z = temp_z_after_rot + z_rot_center;




      //      THyphiHit_new temp;
      //temp.x = tofbar_bar.Position_x + TofpXOffsBar[tofbar_bar.Id_bar-1];
      //temp.y = posy_new + ClYOffs[tofbar_bar.Id_bar-1] ;
      //temp.z = TofpzPos +TofpZOffsBar[tofbar_bar.Id_bar-1]+
      //	0.703*(tofbar_bar.Id_bar-1+0.5) ;
      temp.E = NewE;
      temp.t = time;
      temp.t_bot = new_bot;
      temp.t_top = new_top;
      temp.Id_x.push_back(tofbar_bar.Id_bar);
      temp.Id_y.push_back(tofbar_bar.Id_bar);
      temp.PID = pid;
      
      Event_Out->Add_Hit(temp,"TofPlusBar");
    }
#ifdef DEBUG
  cout<<"Event_Tof->n_TOFAladinBar="<<Event_Tof->n_TOFAladinBar<<endl;
#endif

  for(int i=0;i<Event_Tof->n_TOFAladinBar;i++)
    {

      THyphiTofBar tofbar_bar;
      Event_Tof->Get_Bar(i,"TOF_Aladin",tofbar_bar); 
//       double new_top=0;
//       double new_bot=0;
//       double time=0;
//       double posy_new=0;
      
      THyphiHit_new temp;
      temp.x = tofbar_bar.Position_x;
      temp.y = tofbar_bar.Position_y;
      temp.z = tofbar_bar.Position_z;
      temp.E = tofbar_bar.Energy_high;
      temp.t = (tofbar_bar.time_t+tofbar_bar.time_b)/2*0.1;
      temp.t_top = tofbar_bar.time_t * 0.1;
      temp.t_bot = tofbar_bar.time_b * 0.1;
      temp.Id_x.push_back(tofbar_bar.Id_bar);
      temp.Id_y.push_back(tofbar_bar.Id_bar);
      Event_Out->Add_Hit(temp,"TofAladin");
      
    }
#ifdef DEBUG
  cout<<"Event_Tof->n_TOFPlusPlusBar="<<Event_Tof->n_TOFPlusPlusBar<<endl;
#endif
  
  for(int i=0;i<Event_Tof->n_TOFPlusPlusBar;i++)
    {
      THyphiTofBar tofbar_bar;
      Event_Tof->Get_Bar(i,"TOF_PlusPlus",tofbar_bar); 
      double new_top=0;
      double new_bot=0;
      double time=0;
      double posy_new=0;
      if(!GetCalib("TOFPP",tofbar_bar,new_top,new_bot,time,posy_new))continue;

      THyphiHit_new temp;
      temp.x = tofbar_bar.Position_x+75.0;
      temp.y = posy_new;
      if(tofbar_bar.Id_bar>18)
	temp.z = 4868.32  - (tofbar_bar.Position_x-1679.26)/TMath::Cos(22.8*TMath::DegToRad())
	  *TMath::Sin(22.8*TMath::DegToRad());// - 
      else
	temp.z = tofbar_bar.Position_z;
      temp.E = tofbar_bar.Energy_high;
      temp.t = time;
      temp.t_top = new_top;
      temp.t_bot = new_bot;
      temp.Id_x.push_back(tofbar_bar.Id_bar);
      temp.Id_y.push_back(tofbar_bar.Id_bar);
      Event_Out->Add_Hit(temp,"TofPlusPlus");
    }






  //TOFPlusClustering
  {
    Init_ClusterPalam();
    for(int i=0;i<Event_Tof->n_TOFPlusBar;i++)
      {
	THyphiTofBar tofbar_bar;
	Event_Tof->Get_Bar(i,"TOF_Plus",tofbar_bar);
	double new_top=0;
	double new_bot=0;
	double posy_new=0;
	double time=0;
	if(!GetCalib("TOFP",tofbar_bar,new_top,new_bot,time,posy_new))continue;
	double NewE=0;
	int pid=0;
	if(!GetPID(tofbar_bar,NewE,pid))continue;

	i_hit_bar[tofbar_bar.Id_bar-1] = 1;
	x_bar[tofbar_bar.Id_bar-1] = tofbar_bar.Position_x + TofpXOffsBar[tofbar_bar.Id_bar-1];
	y_bar[tofbar_bar.Id_bar-1] =posy_new;// tofbar_bar.Position_y;
	z_bar[tofbar_bar.Id_bar-1] =TofpzPos +TofpZOffsBar[tofbar_bar.Id_bar-1]+
	  0.703*(tofbar_bar.Id_bar-1+0.5) ;
	energy_low_bar[tofbar_bar.Id_bar-1] = tofbar_bar.Energy_low;
	energy_high_bar[tofbar_bar.Id_bar-1] = NewE;
	tdc_t_bar[tofbar_bar.Id_bar-1] = time;
	tdc_b_bar[tofbar_bar.Id_bar-1] = time;
	i_hit_pid[tofbar_bar.Id_bar-1] = pid;
	
      }
       

    double cut_y = 280.0;

    //int n_tofp_temp = 0;
    for(int i=0;i<31;i++)
      {
	if(i_hit_pid[i]>0)
	  {

	    double charge_temp; 
	    if(i_hit_pid[i]==1) charge_temp = 1.0;
	    if(i_hit_pid[i]==2) charge_temp = 1.5;
	    if(i_hit_pid[i]==4) charge_temp = 2.0;
	    if(i_hit_pid[i]==9) charge_temp = 3.0;
	      {
		if(i_hit_bar[i] == 1 && i_hit_bar[i+1] == 1)
		  {


		    double new_time = tdc_t_bar[i];
		    int new_pid =  i_hit_pid[i];

		    THyphiHit_new temp;
		    temp.x = (x_bar[i]+x_bar[i+1])/2.0;
		    temp.y = y_bar[i] + ClYOffs[i+32];
		    temp.z = (z_bar[i]+z_bar[i+1])/2.0;
		    temp.E = energy_high_bar[i];
		    temp.t = new_time;
		    temp.Id_x.push_back(i+32);
		    temp.Id_x.push_back(i+1+32);
		    temp.PID = new_pid;
		    Event_Out->Add_Hit(temp,"TofPlus");


		    temp.Clear();
		    new_time = tdc_t_bar[i+1];
		    new_pid =  i_hit_pid[i+1];

		    temp.x = (x_bar[i]+x_bar[i+1])/2.0;
		    temp.y = y_bar[i+1]+ ClYOffs[i+64];
		    temp.z = (z_bar[i]+z_bar[i+1])/2.0;
		    temp.E = energy_high_bar[i+1];
		    temp.t = new_time;
		    temp.Id_x.push_back(i+64);
		    temp.Id_x.push_back(i+1+64);
		    temp.PID = new_pid;
		    Event_Out->Add_Hit(temp,"TofPlus");

// 		    temp.Clear();
// 		    new_time = tdc_t_bar[i];
// 		    new_pid =  i_hit_pid[i];

// 		    temp.x = (x_bar[i]+x_bar[i+1])/2.0;
// 		    temp.y = (y_bar[i]+y_bar[i+1])/2.0;
// 		    temp.z = (z_bar[i]+z_bar[i+1])/2.0;
// 		    temp.E = energy_high_bar[i];
// 		    temp.t = new_time;
// 		    temp.Id_x.push_back(i+96);
// 		    temp.Id_x.push_back(i+1+96);
// 		    temp.PID = new_pid;
// 		    Event_Out->Add_Hit(temp,"TofPlus");

// 		    temp.Clear();
// 		    new_time = tdc_t_bar[i+1];
// 		    new_pid =  i_hit_pid[i+1];

// 		    temp.x = (x_bar[i]+x_bar[i+1])/2.0;
// 		    temp.y = (y_bar[i]+y_bar[i+1])/2.0;
// 		    temp.z = (z_bar[i]+z_bar[i+1])/2.0;
// 		    temp.E = energy_high_bar[i+1];
// 		    temp.t = new_time;
// 		    temp.Id_x.push_back(i+128);
// 		    temp.Id_x.push_back(i+1+128);
// 		    temp.PID = new_pid;
// 		    Event_Out->Add_Hit(temp,"TofPlus");



		  }
	      }
	  }
      }
  }//TOFPlusClustering
  return true;
} //Bool_t Friend::Proc(int en)

Bool_t Friend::FillHitCond(THyphiEvent_Phys_new* const Event_Out,THyphiEvent_Phys_track *Event_Track_Out)
{
  string funcname = "FillHitCond : ";
  ClearHitCond();
  //  cout<<"FillHitcond "<<__LINE__<<endl;
  Event_Track_Out->Clear();
  Event_Track_Out->fEventHeader = Event_Out->fEventHeader;
  Event_Track_Out->trigger = Event_Out->trigger_type;

  // if(Event_Out->n_Tr1x>20)return false;
  // if(Event_Out->n_Tr1y>20)return false;
  
  // if(Event_Out->n_Tr2x>20)return false;
  // if(Event_Out->n_Tr2y>20)return false;

  // if(Event_Out->n_CaMx==0 &&Event_Out->n_CaMxp==0)return false;
  // if(Event_Out->n_CaMy==0 &&Event_Out->n_CaMyp==0)return false;

  // if(Event_Out->n_TofPlusPlus==0)return false;

  //  for(int i=0;i<Event_Out->n_Tr0x;i++)
  //    {
  //  	THyphiHit_new temp_x;
  //  	Event_Out->Get_Hit(i,"Tr0x",temp_x);
  	
  //  	THyphiHit_new temp;
  //  	temp.t = temp_x.t;
  //  	temp.x = temp_x.x;
  //  	temp.z = temp_x.z;
  //  	temp.E = temp_x.E;
  //  	temp.Id_x = temp_x.Id_x;
  //  	temp.Le_x=temp_x.Le_x;
  //  	temp.Ene_x=temp_x.Ene_x;
  //  	temp.Wid_x=temp_x.Wid_x;
  //  	Event_Track_Out->Add_TR0(temp,"TR0x");
  //    }

  //  for(int i=0;i<Event_Out->n_Tr0y;i++)
  //    {
  //  	THyphiHit_new temp_y;
  //  	Event_Out->Get_Hit(i,"Tr0y",temp_y);
  	
  //  	THyphiHit_new temp;
  //  	temp.t = temp_y.t;
  //  	temp.y = temp_y.y;
  //  	temp.z = temp_y.z;
  //  	temp.E = temp_y.E;
  //  	temp.Id_y = temp_y.Id_y;
  //  	temp.Le_y=temp_y.Le_y;
  //  	temp.Ene_y=temp_y.Ene_y;
  //  	temp.Wid_y=temp_y.Wid_y;
  //  	Event_Track_Out->Add_TR0(temp,"TR0y");
  //    }


  //  if(Event_Out->n_Tr0x<5&&Event_Out->n_Tr0y<5)
#ifdef WOCL

  cout<<"FillHitcond wocl "<<__LINE__<<endl;
  double erliest_tr0y=-9999;
  for(int i=0;i<Event_Out->n_Tr0x;i++)
    for(int j=0;j<Event_Out->n_Tr0y;j++)
      {
	THyphiHit_new temp_x_hit;
	THyphiHit_new temp_y_hit;

	Event_Out->Get_Hit(i,"Tr0x",temp_x_hit);
	Event_Out->Get_Hit(j,"Tr0y",temp_y_hit);

	if(temp_x_hit.Le_x.size()>15)return false;
	if(temp_y_hit.Le_y.size()>15)return false;

	for(size_t xx =0;xx<temp_x_hit.Le_x.size();xx++)
	  {	      
	    for(size_t yy =0;yy<temp_y_hit.Le_y.size();yy++)
	      {	      
	      
		if(temp_x_hit.Le_x[xx]>=134 &&temp_x_hit.Le_x[xx]<=142 &&
		   temp_y_hit.Le_y[yy]>=137 &&temp_y_hit.Le_y[yy]<=146)
		  if(temp_x_hit.Le_x[xx] - temp_y_hit.Le_y[yy] >= Cut_WidDiff[0][0] &&
		     temp_x_hit.Le_x[xx] - temp_y_hit.Le_y[yy]  <= Cut_WidDiff[0][1])
		    {
		      double posx = d_pos_Fiber[0][temp_x_hit.Id_x[xx]]+posoff[0];
		      double posy = d_pos_Fiber[1][temp_y_hit.Id_y[yy]]+posoff[1];

		      TR0HitCond.Pos.push_back(TVector3(posx,
							posy,
							TR0zPos));
		      
		      TR0HitCond.Id.push_back(TVector2(i,j));
		      
		      TR0HitCond.Wid.push_back(TVector2(temp_x_hit.Ene_x[xx],temp_y_hit.Ene_y[yy]));
		      TR0HitCondTime.push_back(TVector2(temp_x_hit.Le_x[xx],temp_y_hit.Le_y[yy]));
		      TR0HitCondSize.push_back(TVector2(temp_x_hit.Le_x.size(),temp_y_hit.Le_y.size()));
		      // h_WidCor[0]->Fill(wid_x,wid_y);
		      // h_xy_diff_wid[0]->Fill( ( wid_x - wid_y)/((wid_x+wid_y)/2));
		    }
	      }
	  } 
      }
          
#ifdef DEBUG_DETAIL
  cout<<funcname<<" line "<<__LINE__<<endl;
#endif 
      

      
  h_status_new->Fill(4);

  //  if(Event_Out->n_Tr1x<5&&Event_Out->n_Tr1y<5)  
  for(int i=0;i<Event_Out->n_Tr1x;i++)
    for(int j=0;j<Event_Out->n_Tr1y;j++)
      {
	THyphiHit_new temp_x_hit;
	THyphiHit_new temp_y_hit;

	Event_Out->Get_Hit(i,"Tr1x",temp_x_hit);
	Event_Out->Get_Hit(j,"Tr1y",temp_y_hit);

	if(temp_x_hit.Le_x.size()>15)return false;
	if(temp_y_hit.Le_y.size()>15)return false;

	for(size_t xx =0;xx<temp_x_hit.Le_x.size();xx++)
	  {	      
	    for(size_t yy =0;yy<temp_y_hit.Le_y.size();yy++)
	      {	      
	      
		if(temp_x_hit.Le_x[xx]>=134 &&temp_x_hit.Le_x[xx]<=142 &&
		   temp_y_hit.Le_y[yy]>=137 &&temp_y_hit.Le_y[yy]<=146)
		  if(temp_x_hit.Le_x[xx] - temp_y_hit.Le_y[yy] >= Cut_WidDiff[1][0] &&
		     temp_x_hit.Le_x[xx] - temp_y_hit.Le_y[yy] <= Cut_WidDiff[1][1] &&
		     temp_x_hit.Wid_x[xx]>0 && temp_y_hit.Wid_y[yy]>0)
		    {	   
		      double posx = d_pos_Fiber[2][temp_x_hit.Id_x[xx]]+posoff[2];
		      double posy = d_pos_Fiber[3][temp_y_hit.Id_y[yy]]+posoff[3];


		      TR1HitCond.Pos.push_back(TVector3(posx,//+0.59*(rand.Rndm(0)-0.5)+TR1xOffs,
							posy,//+0.59*(rand.Rndm(0)-0.5)+TR1yOffs,
							TR1zPos));
		      TR1HitCond.Id.push_back(TVector2(i,j));
		      TR1HitCond.Wid.push_back(TVector2(temp_x_hit.Wid_x[xx],temp_y_hit.Wid_y[yy]));
		      TR1HitCondTime.push_back(TVector2(temp_x_hit.Le_x[xx],temp_y_hit.Le_y[yy]));
		      TR1HitCondSize.push_back(TVector2(temp_x_hit.Le_x.size(),temp_y_hit.Le_y.size()));
		      // h_xy_diff_wid[1]->Fill(wid_x - wid_y);
		      // h_WidCor[1]->Fill(wid_x,wid_y);
		  
		    }
	      }
	  }
      }
  if(TR1HitCond.Pos.size()<4)return false;
  if(TR1HitCond.Pos.size()>900)return false;

#ifdef DEBUG_DETAIL
  cout<<funcname<<" line "<<__LINE__<<endl;
#endif 
      
    
  //  if(Event_Out->n_Tr2x<5&&Event_Out->n_Tr2y<5)        
  for(int i=0;i<Event_Out->n_Tr2x;i++)
    for(int j=0;j<Event_Out->n_Tr2y;j++)
      {
	THyphiHit_new temp_x_hit;
	THyphiHit_new temp_y_hit;

	Event_Out->Get_Hit(i,"Tr2x",temp_x_hit);
	Event_Out->Get_Hit(j,"Tr2y",temp_y_hit);

	if(temp_x_hit.Le_x.size()>15)return false;
	if(temp_y_hit.Le_y.size()>15)return false;

#ifdef DEBUG_DETAIL
	cout<<funcname<<" line "<<__LINE__<<endl;
#endif 
	for(size_t xx =0;xx<temp_x_hit.Le_x.size();xx++)
	  {	      
	    for(size_t yy =0;yy<temp_y_hit.Le_y.size();yy++)
	      {	      
	  	   

		if(temp_x_hit.Le_x[xx]>=137 &&temp_x_hit.Le_x[xx]<=145 &&
		   temp_y_hit.Le_y[yy]>=135 &&temp_y_hit.Le_y[yy]<=144)
		  if(temp_x_hit.Le_x[xx] - temp_y_hit.Le_y[yy] >= Cut_WidDiff[2][0] &&
		     temp_x_hit.Le_x[xx] - temp_y_hit.Le_y[yy] <= Cut_WidDiff[2][1] &&
		     temp_y_hit.Wid_y[yy]>0/*wid_x>0 && wid_y>0*/)
		    {
		      double posx = d_pos_Fiber[4][temp_x_hit.Id_x[xx]]+posoff[4];
		      double posy = d_pos_Fiber[5][temp_y_hit.Id_y[yy]]+posoff[5];
		      TR2HitCond.Pos.push_back(TVector3(posx,
							posy,
							TR2zPos));
		      TR2HitCond.Id.push_back(TVector2(i,j));
		      TR2HitCond.Wid.push_back(TVector2(0,temp_y_hit.Wid_y[yy]));
		      TR2HitCondTime.push_back(TVector2(temp_x_hit.Le_x[xx],temp_y_hit.Le_y[yy]));
		      TR2HitCondSize.push_back(TVector2(temp_x_hit.Le_x.size(),temp_y_hit.Le_y.size()));
		      // h_WidCor[2]->Fill(wid_x,wid_y);
		      // //h_xy_diff_wid[2]->Fill( ( wid_x - wid_y)/((wid_x+wid_y)/2));
		      // h_xy_diff_wid[2]->Fill( wid_x - wid_y);

		    }
	      }
	  }
      }
  if(TR2HitCond.Pos.size()<4)return false;
  if(TR2HitCond.Pos.size()>9000)return false;
#else // with clustering

  //  cout<<"FillHitcond with clustering "<<__LINE__<<endl;

  double erliest_tr0y=-9999;
  for(int i=0;i<Event_Out->n_Tr0x;i++)
    for(int j=0;j<Event_Out->n_Tr0y;j++)
      {
	THyphiHit_new temp_x_hit;
	THyphiHit_new temp_y_hit;

	Event_Out->Get_Hit(i,"Tr0x",temp_x_hit);
	Event_Out->Get_Hit(j,"Tr0y",temp_y_hit);
#ifdef CLSIZE
	// // 	    if(temp_x.Id.size()>5)continue;
	// // 	    if(temp_y.Id.size()>5)continue;
#endif	    
	double temp_xx = 0.0;
	double temp_yy = 0.0;
	int temp_xx_size = 0;
	int temp_yy_size = 0;
	double wid_x=0;
	double wid_y=0;
	for(size_t xx =0;xx<temp_x_hit.Le_x.size();xx++)
	  {
	    if(temp_x_hit.Le_x[xx]>0)
	      {
		//wid_x += temp_x.Width[xx];
		temp_xx += temp_x_hit.Le_x[xx];
		temp_xx_size++;
	      }
	  }
	for(size_t yy =0;yy<temp_y_hit.Le_y.size();yy++)
	  {
	    if(temp_y_hit.Le_y[yy]>0)
	      {
		if(temp_y_hit.Le_y[yy]>erliest_tr0y)
		  erliest_tr0y=temp_y_hit.Le_y[yy];

		//wid_y += temp_y.Width[yy];
		temp_yy += temp_y_hit.Le_y[yy];
		temp_yy_size++;
	      }
	  }	    
	h_xy_diff[0]->Fill(temp_xx/temp_xx_size - temp_yy/temp_yy_size);

	    
	if(temp_xx/temp_xx_size>=128 &&temp_xx/temp_xx_size<=135 &&
	   temp_yy/temp_yy_size>=132 &&temp_yy/temp_yy_size<=137)
	      
	  if(temp_xx/temp_xx_size - temp_yy/temp_yy_size >= Cut_WidDiff[0][0] &&
	     temp_xx/temp_xx_size - temp_yy/temp_yy_size <= Cut_WidDiff[0][1] //&&
	     /*wid_x>0 && wid_y>0*/)
	    {
		  
// 	      THyphiHit_track temp;
// 	      temp.x = temp_x_hit.x;
// 	      temp.y = temp_y_hit.y;
// 	      temp.z = TR0zPos;
// 	      temp.E = (temp_x_hit.E + temp_y_hit.E)/2.0;
// 	      temp.Index_x = i;
// 	      temp.Index_y = j;
// 	      Event_Track_Out->Add_Hit(temp,"Tr0");


	      TR0HitCond.Pos.push_back(TVector3(temp_x_hit.x,
						temp_y_hit.y,
						TR0zPos));
		      
	      TR0HitCond.Id.push_back(TVector2(i,j));
		      
	      TR0HitCond.Wid.push_back(TVector2(temp_x_hit.E,temp_y_hit.E));
	      TR0HitCondTime.push_back(TVector2(temp_xx/temp_xx_size,temp_yy/temp_yy_size));
	      TR0HitCondSize.push_back(TVector2(temp_xx_size,temp_yy_size));
	      h_WidCor[0]->Fill(wid_x,wid_y);
	      h_xy_diff_wid[0]->Fill( ( wid_x - wid_y)/((wid_x+wid_y)/2));
	    }
	    
      }
          
#ifdef DEBUG_DETAIL
  cout<<funcname<<" line "<<__LINE__<<endl;
#endif 
      

      
  h_status_new->Fill(4);
          
  //  if(Event_Out->n_Tr1x<5&&Event_Out->n_Tr1y<5)  
  for(int i=0;i<Event_Out->n_Tr1x;i++)
    for(int j=0;j<Event_Out->n_Tr1y;j++)
      {
	THyphiHit_new temp_x_hit;
	THyphiHit_new temp_y_hit;

	Event_Out->Get_Hit(i,"Tr1x",temp_x_hit);
	Event_Out->Get_Hit(j,"Tr1y",temp_y_hit);
#ifdef CLSIZE
	if(temp_x_hit.Id_x.size()>MaxClSize)continue;
	if(temp_y_hit.Id_y.size()>MaxClSize)continue;
#endif	    
	    
	double temp_xx = 0.0;
	double temp_yy = 0.0;
	int temp_xx_size = 0;
	int temp_yy_size = 0;

	double wid_x=0;
	double wid_y=0;

	for(size_t xx =0;xx<temp_x_hit.Le_x.size();xx++)
	  {

	    if(temp_x_hit.Le_x[xx]>0 && temp_x_hit.Le_x[xx]<250)
	      {
		wid_x += temp_x_hit.Wid_x[xx];
		temp_xx += temp_x_hit.Le_x[xx];
		temp_xx_size++;
	      }
	  }
	for(size_t yy =0;yy<temp_y_hit.Le_y.size();yy++)
	  {
	    if(temp_y_hit.Le_y[yy]>0&&temp_y_hit.Le_y[yy]<250)
	      {
		wid_y += temp_y_hit.Wid_y[yy];
		temp_yy += temp_y_hit.Le_y[yy];
		temp_yy_size++;
	      }
	  }	    
	h_xy_diff[1]->Fill(temp_xx/temp_xx_size - temp_yy/temp_yy_size);
	wid_x = wid_x/temp_xx_size;
	wid_y = wid_y/temp_yy_size;
	      
	if(temp_xx/temp_xx_size>=134 &&temp_xx/temp_xx_size<=142 &&
	   temp_yy/temp_yy_size>=137 &&temp_yy/temp_yy_size<=146)
	  if(temp_xx/temp_xx_size - temp_yy/temp_yy_size >= Cut_WidDiff[1][0] &&
	     temp_xx/temp_xx_size - temp_yy/temp_yy_size <= Cut_WidDiff[1][1] &&
	     wid_x>0 && wid_y>0)
	    {	   
// 	      THyphiHit_track temp;
// 	      temp.x = temp_x_hit.x;
// 	      temp.y = temp_y_hit.y;
// 	      temp.z = TR1zPos;
// 	      temp.E = (temp_x_hit.E + temp_y_hit.E)/2.0;
// 	      temp.Index_x = i;
// 	      temp.Index_y = j;
// 	      Event_Track_Out->Add_Hit(temp,"Tr1");


		  
	      TR1HitCond.Pos.push_back(TVector3(temp_x_hit.x,//+0.59*(rand.Rndm(0)-0.5)+TR1xOffs,
						temp_y_hit.y,//+0.59*(rand.Rndm(0)-0.5)+TR1yOffs,
						TR1zPos));
	      TR1HitCond.Id.push_back(TVector2(i,j));
	      TR1HitCond.Wid.push_back(TVector2(wid_x,wid_y));
	      TR1HitCondTime.push_back(TVector2(temp_xx/temp_xx_size,temp_yy/temp_yy_size));
	      TR1HitCondSize.push_back(TVector2(temp_xx_size,temp_yy_size));
	      h_xy_diff_wid[1]->Fill(wid_x - wid_y);
	      h_WidCor[1]->Fill(wid_x,wid_y);
		  
	    }
      }
#ifdef DEBUG_DETAIL
  cout<<funcname<<" line "<<__LINE__<<endl;
#endif 
  //  cout<<"FillHitcond with clustering "<<__LINE__<<endl;
    
  //  if(Event_Out->n_Tr2x<5&&Event_Out->n_Tr2y<5)        
  for(int i=0;i<Event_Out->n_Tr2x;i++)
    for(int j=0;j<Event_Out->n_Tr2y;j++)
      {
	THyphiHit_new temp_x_hit;
	THyphiHit_new temp_y_hit;

	Event_Out->Get_Hit(i,"Tr2x",temp_x_hit);
	Event_Out->Get_Hit(j,"Tr2y",temp_y_hit);
#ifdef DEBUG_DETAIL
  cout<<funcname<<" line "<<__LINE__<<endl;
#endif 

#ifdef CLSIZE
	if(temp_x_hit.Id_x.size()>MaxClSize)continue;
	if(temp_y_hit.Id_y.size()>MaxClSize)continue;
#endif	    
	double temp_xx = 0.0;
	double temp_yy = 0.0;
	int temp_xx_size = 0;
	int temp_yy_size = 0;

	double wid_x=0;
	double wid_y=0;
	for(size_t xx =0;xx<temp_x_hit.Le_x.size();xx++)
	  {

	    if(temp_x_hit.Le_x[xx]>0 && temp_x_hit.Le_x[xx]<250)
	      {
		//wid_x += temp_x_hit.Wid_x[xx];
		temp_xx += temp_x_hit.Le_x[xx];
		temp_xx_size++;
	      }
	  }
	for(size_t yy =0;yy<temp_y_hit.Le_y.size();yy++)
	  {
	    if(temp_y_hit.Le_y[yy]>0&&temp_y_hit.Le_y[yy]<250)
	      {
		wid_y += temp_y_hit.Wid_y[yy];
		temp_yy += temp_y_hit.Le_y[yy];
		temp_yy_size++;
	      }
	  }	    
	h_xy_diff[2]->Fill(temp_xx/temp_xx_size - temp_yy/temp_yy_size);
	   
	//wid_x = wid_x/temp_xx_size;
	wid_y = wid_y/temp_yy_size;
	  	   

	if(temp_xx/temp_xx_size>=137 &&temp_xx/temp_xx_size<=145 &&
	   temp_yy/temp_yy_size>=135 &&temp_yy/temp_yy_size<=144)
	  if(temp_xx/temp_xx_size - temp_yy/temp_yy_size >= Cut_WidDiff[2][0] &&
	     temp_xx/temp_xx_size - temp_yy/temp_yy_size <= Cut_WidDiff[2][1] &&
	     wid_y>0/*wid_x>0 && wid_y>0*/)
	    {


// 	      THyphiHit_track temp;
// 	      temp.x = temp_x_hit.x;
// 	      temp.y = temp_y_hit.y;
// 	      temp.z = TR1zPos;
// 	      temp.E = (temp_x_hit.E + temp_y_hit.E)/2.0;
// 	      temp.Index_x = i;
// 	      temp.Index_y = j;
// 	      Event_Track_Out->Add_Hit(temp,"Tr2");


	      TR2HitCond.Pos.push_back(TVector3(temp_x_hit.x,
						temp_y_hit.y,
						TR2zPos));
	      TR2HitCond.Id.push_back(TVector2(i,j));
	      TR2HitCond.Wid.push_back(TVector2(temp_x_hit.E,temp_y_hit.E));
	      TR2HitCondTime.push_back(TVector2(temp_xx/temp_xx_size,temp_yy/temp_yy_size));
	      TR2HitCondSize.push_back(TVector2(temp_xx_size,temp_yy_size));
	      h_WidCor[2]->Fill(wid_x,wid_y);
	      //h_xy_diff_wid[2]->Fill( ( wid_x - wid_y)/((wid_x+wid_y)/2));
	      h_xy_diff_wid[2]->Fill( wid_x - wid_y);

	    }
      }
#endif

  double min_tofs =-9999.;
  //double min_tofs_bar =0;
  int min_tofs_bar_id =0;
  int min_tofs_bar_id_E =0;
  //  cout<<"FillHitcond  "<<__LINE__<<endl;
#ifdef DEBUG_DETAIL
  cout<<funcname<<" line "<<__LINE__<<endl;
#endif
  int FindSecondLi=0;
  double min_tofs_temp[10];
  double min_e_temp[10];
  for(int i=0;i<10;i++)min_tofs_temp[i]=-999.;
  //  if(Event_Out->n_TofStart!=2)return false;
  for(int i=0;i<Event_Out->n_TofStart;i++)
    {

       THyphiHit_new temp_tof;
      Event_Out->Get_Hit(i,"TofStart",temp_tof);
      //	  cout<<"come here ff "<<tofbar_bar.Energy_high<<" "<<EneStCutTemp[tofbar_bar.Id_bar-1][1]<<endl;
      {
	//if(temp_tof.Id_x[0]==1)cout<<"come 1"<<endl;
 	if(temp_tof.E>EneStCutTemp[temp_tof.Id_x[0]-1][1])
 	  {
 	    FindSecondLi++;
 	  }

	double tdiff = temp_tof.t_top-temp_tof.t_bot;
	//	if(temp_tof.Id_x[0]==1)cout<<"come 2 "<<tdiff<<" "<<temp_tof.E<<endl;
  	if(/*tofbar_bar.Id_bar>=5 &&tofbar_bar.Id_bar<=7 &&*/
  	   temp_tof.E>EneStCutTemp[temp_tof.Id_x[0]-1][0]&&
  	   temp_tof.E<EneStCutTemp[temp_tof.Id_x[0]-1][1]
  	   )
	      
// 	  if(1// (tofbar_bar.TDC_t+tofbar_bar.TDC_b)/2.0*0.043 > min_tofs &&
// 	     //fabs(tofbar_bar.TDC_t-tofbar_bar.TDC_b)<100
// 	     )
#ifdef TOFSTDCCUT
	    if(tdiff>TofsTdcCut[temp_tof.Id_x[0]-1][0] &&
	       tdiff<TofsTdcCut[temp_tof.Id_x[0]-1][1] )
#endif

	    {
	      
	      //    if(temp_tof.Id_x[0]==1)cout<<"come 2"<<endl;
//   	      if(temp_tof.t_top>temp_tof.t_bot)
//   		min_tofs_temp[temp_tof.Id_x[0]-1] = temp_tof.t_top;
//   	      else
//   		min_tofs_temp[temp_tof.Id_x[0]-1] = temp_tof.t_bot;
	      min_tofs_temp[temp_tof.Id_x[0]-1] = temp_tof.t;
	      min_e_temp[temp_tof.Id_x[0]-1] = temp_tof.E;
// 	      min_tofs_bar = temp_tof.x;
// 	      min_tofs_bar_id = temp_tof.Id_x[0];
	    }
	    
      }
    }
  //  cout<<"FillHitcond with clustering "<<__LINE__<<endl;
#ifdef DEBUG

  cout<<funcname<<" line "<<__LINE__<<" "<<FindSecondLi<<endl;
  
#endif

  //  if(FindSecondLi>0)return false;
  h_status_new->Fill(5);

  double comp_temp = -99999;
  for(int i=0;i<10;i++)
    {
      if(min_tofs_temp[i]>0)
	{
	  if(min_tofs_temp[i]>comp_temp)
	    {
	      comp_temp = min_tofs_temp[i];
	      min_tofs_bar_id =i+1;
	      min_tofs_bar_id_E =min_e_temp[i];
	    }

	}


    }

  min_tofs = comp_temp; // Latest Timing is taken as Time Zero




#ifdef DEBUG

    cout<<funcname<<" line "<<__LINE__<<"min_tofs ="<<min_tofs<<" "<<endl;
  
#endif
    //  cout<<"FillHitcond findsec "<<__LINE__<<endl;
  if(FindSecondLi>0) return false;
  //  cout<<"FillHitcond findsec "<<__LINE__<<endl;
  // if(min_tofs_bar_id==4 || min_tofs_bar_id==7 || min_tofs_bar_id==9|| min_tofs_bar_id==10 )
  //   return false;

  //cout<<min_tofs<<endl;
  SetTimeZero(min_tofs);

  SetTimeZeroBar(min_tofs_bar_id);
  SetTimeZeroBarE(min_tofs_bar_id_E);
  hts_min_tofs->Fill(TimeZero);      
  hts_min_tofsE[(int)min_tofs_bar_id-1]->Fill(TimeZero,min_tofs_bar_id_E);
  
  Event_Track_Out-> TofStartBar=min_tofs_bar_id;
  Event_Track_Out-> TofStartBarTime=min_tofs;

  //if(min_tofs <0)return false;
  //  cout<<"FillHitcond "<<__LINE__<<endl;
  if(min_tofs<38)return false;
  //  cout<<"FillHitcond "<<__LINE__<<endl;
  if(min_tofs>48)return false;
  //  cout<<"FillHitcond "<<__LINE__<<endl;
//   if(min_tofs<43)
//     SetTimeZero(38);
//   else
//     SetTimeZero(48);
//   if(erliest_tr0y<0)return false;
  //min_tofs = (-1.0)*(erliest_tr0y-134)*2.5+45;
  //SetTimeZero(newtime);
//  min_tofs = (-1.0)*min_tofs+90.0;
//  min_tofs = 45.0;
  SetTimeZero(min_tofs);
  h_status_new->Fill(6);

  //VECTOR FOR TOF
      
  int NumLi=0;

  //#ifdef EVTOF
      
  int neig_hit[34];
  for(int i=0;i<34;i++)
    neig_hit[i]=0;

  for(int i=0;i<Event_Out->n_TofPlusBar;i++)
    {
       THyphiHit_new temp_tof;
      Event_Out->Get_Hit(i,"TofPlusBar",temp_tof);
      neig_hit[temp_tof.Id_x[0]]=1;
    }

  for(int i=0;i<Event_Out->n_TofPlusBar;i++)
    {
       THyphiHit_new temp_tof;
      Event_Out->Get_Hit(i,"TofPlusBar",temp_tof);
	  
      //double posy_new =-999;
#ifdef ONEHITTOFP
      if(neig_hit[temp_tof.Id_x[0]-1]==0 &&neig_hit[temp_tof.Id_x[0]+1]==0 )
#endif
	{
	  if(temp_tof.t_bot>0&&temp_tof.t_top>0)//&&
	    {
	      h_xy_tofplus->Fill(temp_tof.x,temp_tof.y-meandiff[temp_tof.Id_x[0]-1]);
	      htp_TimeOfFlightPos->Fill(temp_tof.x,temp_tof.t-min_tofs);
	      htp_TimeOfFlightPosT->Fill(temp_tof.x,temp_tof.t_top-min_tofs+50);
	      htp_TimeOfFlightPosB->Fill(temp_tof.x,temp_tof.t_bot-min_tofs+50);
	      //	      cout<<temp_tof.t_bot<<" "<<temp_tof.t_top<<endl;
	      htp_TimeOfFlightPosTofp->Fill(temp_tof.x,temp_tof.t);
	      htp_TimeOfFlightPosMin->Fill(temp_tof.x,min_tofs);

//  	      if(temp_tof.t-min_tofs>30)continue;
//  	      if(temp_tof.t-min_tofs<16.1)continue;

//  	      if(time-min_tofs>30)continue;
//  	      if(time-min_tofs<14)continue;

	      if(temp_tof.PID ==9)NumLi++;
	      if(temp_tof.PID>10)
		{
		  NumLi++;
		  continue;
		}
	      {
		double tofptime=temp_tof.t;
		TofPlusBarHitCond.Pos.
		  push_back(TVector3(temp_tof.x,
				     temp_tof.y,//-meandiff[temp_tof.Id_x[0]-1],
				     temp_tof.z));
		TofPlusBarHitCond.Id.push_back(TVector2(tofptime-min_tofs,
							temp_tof.PID));
		TofPlusBarHitCond.Wid.push_back(TVector2(temp_tof.E,temp_tof.Id_x[0]-1));//Because TofPlus Clusterd Id start from 1!!!!!! not 0.
		//cout<<temp_tof.t-min_tofs<<endl;

		    
		TofPlusBarHitCondEneHigh.push_back(temp_tof.E);
		TofPlusBarHitCondIndex.push_back(i);		    
	      }
	    }
	      
	      
	}
    }



  //  if(TofPlusBarHitCond.Pos.size()<1) return false;
  htp_NumLi->Fill(NumLi);






  for(int i=0;i<Event_Out->n_TofPlusPlus;i++)
    {
       THyphiHit_new temp_tof;
      Event_Out->Get_Hit(i,"TofPlusPlus",temp_tof);
      if(temp_tof.Id_x[0]>=1 &&temp_tof.Id_x[0]<=18 )
	{      
	  if(temp_tof.t-min_tofs<24 &&temp_tof.t-min_tofs>16
	     &&fabs(temp_tof.y+AddTofppBarOffsPos[temp_tof.Id_x[0]-1])<600)
	    {
	      TofPlusPlusHitCond.Pos.
		push_back(TVector3(temp_tof.x,
				   temp_tof.y,//+AddTofppBarOffsPos[temp_tof.Id_x[0]-1],
				   temp_tof.z));

	      TofPlusPlusHitCond.Id.push_back(TVector2(temp_tof.t-min_tofs,i));
	      TofPlusPlusHitCond.Wid.push_back(TVector2(temp_tof.E,temp_tof.Id_x[0]-1));
	    }
	  
	  h_xy_plusplus->Fill(temp_tof.x,temp_tof.y+AddTofppBarOffsPos[temp_tof.Id_x[0]-1]);

	  //	  cout<<temp_tof.Id_x[0]<<" "<<temp_tof.y+AddTofppBarOffsPos[temp_tof.Id_x[0]-1]<<endl;


	  htpp_TimeOfFlight ->Fill(temp_tof.t-min_tofs);
	  //	  htpp_TimeOfFlightE ->Fill(temp_tof.t-min_tofs,temp_tof.E);
	  htpp_TimeOfFlightPos ->Fill(temp_tof.x,temp_tof.t-min_tofs);

	}      
    }
  if(TofPlusPlusHitCond.Pos.size()<1)return false;
  

#ifdef WITHALADIN
    
  for(int i=0;i<Event_Out->n_TofAladin;i++)
    {
      THyphiHit_new temp_tof;
      Event_Out->Get_Hit(i,"TofAladin",temp_tof);
      {

	//double time = (tofbar_bar.TDC_t+tofbar_bar.TDC_b)/2.0*0.1;
	double time = temp_tof.t;
	hta_TimeOfFlight ->Fill(min_tofs-time);
	// 	      htpp_TimeOfFlightE ->Fill(tofbar.t-min_tofs,tofbar.E);
	// 	      htpp_TimeOfFlightPos ->Fill(tofbar.x,tofbar.t-min_tofs);
	// 	      if(tofbar.t-min_tofs > 5 && tofbar.t-min_tofs < 40)
	hta_TimeOfFlightE ->Fill(min_tofs-time,temp_tof.E);
	hta_TimeOfFlightPos ->Fill(temp_tof.x,min_tofs-time);
	//	      if(tofbar.t-min_tofs > 5 && tofbar.t-min_tofs < 40)
	// 	      if(tofbar_bar.time_t>1200 &&tofbar_bar.time_t<1800 && 
	// 		 tofbar_bar.time_b>1200 &&tofbar_bar.time_b<1800 )
	if(temp_tof.x>1200&&temp_tof.x<3200&&
	   temp_tof.y>-1000&&temp_tof.y<1000)
	  {
	    TofAladinHitCond.Pos.
	      push_back(TVector3(temp_tof.x,
				 temp_tof.y,
				 temp_tof.z));
		  
	    TofAladinHitCond.Id.push_back(TVector2(i,0));
	  }
	h_xy_aladin->Fill(temp_tof.x,temp_tof.y);
      }
    }
#endif


#ifdef DEBUG_DETAIL
  cout<<funcname<<" line "<<__LINE__<<endl;
#endif

  //TOFPlusClustering
  for(int i=0;i<Event_Out->n_TofPlus;i++)
    {
       THyphiHit_new temp_tof;
      Event_Out->Get_Hit(i,"TofPlus",temp_tof);    
      TofPlusBarClHitCond.Pos.
	push_back(TVector3(temp_tof.x,temp_tof.y,temp_tof.z));
      TofPlusBarClHitCond.Id.push_back(TVector2(temp_tof.t-min_tofs,
						temp_tof.PID));
      TofPlusBarClHitCond.Wid.push_back(TVector2(temp_tof.E, temp_tof.Id_x[0])); //Because TofPlus Clusterd Id start from 0!!!!!! not 1.
      TofPlusBarClHitCondEneHigh.push_back(temp_tof.E);
      TofPlusBarClHitCondIndex.push_back(i);
    }
  
  for(size_t tofpcl=0;tofpcl<TofPlusBarClHitCond.Pos.size();tofpcl++)
    {
      
      h_hitpat_tofpcl->Fill(TofPlusBarClHitCond.Pos[tofpcl].X());
    }

  

   for(int tofpcl=0;tofpcl<TofPlusBarClHitCond.Pos.size();tofpcl++)
     {

       TofPlusBarHitCond.Pos.push_back(TofPlusBarClHitCond.Pos[tofpcl]);
       TofPlusBarHitCond.Id.push_back(TofPlusBarClHitCond.Id[tofpcl]);
       TofPlusBarHitCond.Wid.push_back( TofPlusBarClHitCond.Wid[tofpcl]);
    
       TofPlusBarHitCondEneHigh.push_back(TofPlusBarClHitCondEneHigh[tofpcl]);
       TofPlusBarHitCondIndex.push_back( TofPlusBarClHitCondIndex[tofpcl]);
     }







  return true;
}//Bool_t Friend::FillHitCond(THyphiEvent_Phys_new *Event_Out,THyphiEvent_Phys_track *Event_Track_Out)


Bool_t Friend::HitCondAna(int en,THyphiEvent_Phys_new* const Event_Out)
{
  string funcname = "HitCondAn : ";
#ifdef CALIBCHECK
  CalibCheck(en,Event_Out);
#endif

#ifdef TRACKCHECK
  TrackCheck(en,Event_Out);
#endif


#ifdef POSRESFIB    	
 {
  const double min_tofs = GetTimeZero();
  const double timezerobar = GetTimeZeroBar();

  std::vector<double> calctr0x;
  std::vector<double> calctr0y;

  std::vector<double> calctr1x;
  std::vector<double> calctr1y;

  std::vector<double> calctr2x;
  std::vector<double> calctr2y;
  if(Event_Out->n_Tr0x<5&&Event_Out->n_Tr0y<5&&
     Event_Out->n_Tr1x<5&&Event_Out->n_Tr1y<5)
  for(size_t tr0=0;tr0<TR0HitCond.Pos.size();tr0++)
    {
      for(size_t tr1=0;tr1<TR1HitCond.Pos.size();tr1++)
	{
	  double x[3],y[3],z[3];
	  //double reso_tofp[3]={ResTR1,ResTR2,ResTOFp};

	  x[0]=TR0HitCond.Pos[tr0].X();
	  y[0]=TR0HitCond.Pos[tr0].Y();
	  z[0]=TR0HitCond.Pos[tr0].Z();
		
	  x[1]=TR1HitCond.Pos[tr1].X();
	  y[1]=TR1HitCond.Pos[tr1].Y();
	  z[1]=TR1HitCond.Pos[tr1].Z();

	  double p1x = (x[1]-x[0])/(z[1]-z[0]);
	  double p2x = x[1] - p1x*z[1];
	  
	  double p1y = (y[1]-y[0])/(z[1]-z[0]);
	  double p2y = y[1] - p1y*z[1];

	  int usedid[6]={0,0,0,0,0,0};
	  bool coin[6] ={false,false,false,false,false,false};
	  bool cc = CheckChambHitIdP(p1x,p2x,p1y,p2y,min_tofs,Event_Out,usedid,coin);

	  if(!cc)continue;

	  double cutfuncl = timezerobar*3.0-18.0;
	  double cutfunch = timezerobar*3.0-12.0;

	  if(p2x<cutfuncl|| p2x>cutfunch)continue;
	  if(fabs(p2x)>20 || fabs(p2y)>20)continue;

	  bool tofpcoin = false;
	  for(int tofp=0;tofp<Event_Out->n_TofPlusBar;tofp++)		
	    {			
	      THyphiHit_new temp_tof;
	      Event_Out->Get_Hit(tofp,"TofPlusBar",temp_tof);
	      {
		if(fabs(temp_tof.y - (p1y*temp_tof.z+p2y))<25.0)
		  tofpcoin = true;
	      }
	    }



	  if(!tofpcoin)continue;

	  double posx[5],posz[5];
	  bool hoge2=CheckChambHitQueenIdPHP(100,p1x,p2x,
					     p1y,p2y,
					     Event_Out,posx,posz,coin);


// 	  if(!coin[2])continue;
// 	  if(!coin[3])continue;
	  double cx= p1x*TR2zPos+p2x;
	  double cy= p1y*TR2zPos+p2y;
	  bool x_same=false;
	  bool y_same=false;
	  for(int same=0;same<calctr2x.size();same++)
	    {
	      if(fabs(cx-calctr2x[same])<0.001)
		x_same=true;
	    }
	  for(int same=0;same<calctr2y.size();same++)
	    {
	      if(fabs(cy-calctr2y[same])<0.001)
		y_same=true;
	    }
	  if(!x_same)
	    for(int i=0;i<Event_Out->n_Tr2x;i++)
 	  {
 	    THyphiHit_new temp_x_hit;
 	    Event_Out->Get_Hit(i,"Tr2x",temp_x_hit);

	    

	      {
		h_PosResX[2]->Fill(temp_x_hit.x - cx );
		h_PosResX_angle[2]->Fill(temp_x_hit.x-cx,p1x);
		//cout<<temp_x_hit.x<< " "<<cx<<endl;
		for(int cl=0;cl<temp_x_hit.Id_x.size();cl++)
		  {
		    h_PosResX_pos[2]->Fill(temp_x_hit.x - cx,temp_x_hit.Id_x[cl]);
		  }
		calctr2x.push_back(cx);
		
	      }
 	  }
	  if(!y_same)
         for(int i=0;i<Event_Out->n_Tr2y;i++)
	   {
	     THyphiHit_new temp_x_hit;
	     Event_Out->Get_Hit(i,"Tr2y",temp_x_hit);
	     bool x_same=false;
	     
	     {
		 h_PosResY[2]->Fill(temp_x_hit.y - cy );
		 h_PosResY_angle[2]->Fill(temp_x_hit.y-cy,p1y);
		 //cout<<temp_x_hit.y<< " "<<cy<<endl;
		for(int cl=0;cl<temp_x_hit.Id_y.size();cl++)
		  {
		    h_PosResY_pos[2]->Fill(temp_x_hit.y - cy,temp_x_hit.Id_y[cl]);
		  }

		 calctr2y.push_back(cy);
		 
	       }
	   }

//  	  for(size_t tr2=0;tr2<TR2HitCond.Pos.size();tr2++)
//  	    {
	      
//  	      double mx=TR2HitCond.Pos[tr2].X();
//  	      double my=TR2HitCond.Pos[tr2].Y();
//  	      double mz=TR2HitCond.Pos[tr2].Z();
	      

// 	      {
// 		bool x_same=false;
// 		bool y_same=false;
// 		for(int same=0;same<calctr2x.size();same++)
// 		  {
// 		    if(fabs(cx-calctr2x[same])<0.001)
// 		      x_same=true;
// 		  }
		
// 		if(!x_same)
// 		  {
// 		    h_PosResX[2]->Fill(mx - cx );
// 		    h_PosResX_angle[2]->Fill(mx-cx,p1x);

// 		    calctr2x.push_back(cx);
// 		  }

// 		for(int same=0;same<calctr2y.size();same++)
// 		  {
// 		    if(fabs(cy-calctr2y[same])<0.001)
// 		      y_same=true;
// 		  }
// 		if(!y_same)
// 		  {
// 		    h_PosResY[2]->Fill(my - cy);
// 		    h_PosResY_angle[2]->Fill(my-cy,p1y);
// 		    calctr2y.push_back(cy);

// 		  }

		
// 	      }
	      


//	    }


	}
    }

  









  if(Event_Out->n_Tr1x<5&&Event_Out->n_Tr1y<5&&
     Event_Out->n_Tr2x<5&&Event_Out->n_Tr2y<5)
  for(size_t tr1=0;tr1<TR1HitCond.Pos.size();tr1++)
    {
      for(size_t tr2=0;tr2<TR2HitCond.Pos.size();tr2++)
	{
	  double x[3],y[3],z[3];
	  //double reso_tofp[3]={ResTR1,ResTR2,ResTOFp};

	  x[0]=TR1HitCond.Pos[tr1].X();
	  y[0]=TR1HitCond.Pos[tr1].Y();
	  z[0]=TR1HitCond.Pos[tr1].Z();
		
	  x[1]=TR2HitCond.Pos[tr2].X();
	  y[1]=TR2HitCond.Pos[tr2].Y();
	  z[1]=TR2HitCond.Pos[tr2].Z();

	  double p1x = (x[1]-x[0])/(z[1]-z[0]);
	  double p2x = x[1] - p1x*z[1];
	  
	  double p1y = (y[1]-y[0])/(z[1]-z[0]);
	  double p2y = y[1] - p1y*z[1];

	  int usedid[6]={0,0,0,0,0,0};
	  bool coin[6] ={false,false,false,false,false,false};
	  bool cc = CheckChambHitIdP(p1x,p2x,p1y,p2y,min_tofs,Event_Out,usedid,coin);

	  if(!cc)continue;

	  double cutfuncl = timezerobar*3.0-18.0;
	  double cutfunch = timezerobar*3.0-12.0;

	  if(p2x<cutfuncl|| p2x>cutfunch)continue;
	  if(fabs(p2x)>20 || fabs(p2y)>20)continue;
	  bool tofpcoin = false;
	  for(int tofp=0;tofp<Event_Out->n_TofPlusBar;tofp++)		
	    {			
	      THyphiHit_new temp_tof;
	      Event_Out->Get_Hit(tofp,"TofPlusBar",temp_tof);
	      {
		if(fabs(temp_tof.y - (p1y*temp_tof.z+p2y))<25.0)
		  tofpcoin = true;
	      }
	    }
	  
	  

	  if(!tofpcoin)continue;
	  double posx[5],posz[5];
	  bool hoge2=CheckChambHitQueenIdPHP(100,p1x,p2x,
					     p1y,p2y,
					     Event_Out,posx,posz,coin);


// 	  if(!coin[2])continue;
// 	  if(!coin[3])continue;
	  double cx= p1x*TR0zPos+p2x;
	  double cy= p1y*TR0zPos+p2y;
	  bool x_same=false;
	  bool y_same=false;
	  for(int same=0;same<calctr0x.size();same++)
	    {
	      if(fabs(cx-calctr0x[same])<0.001)
		x_same=true;
	    }
	  
	  for(int same=0;same<calctr0y.size();same++)
	    {
	      if(fabs(cy-calctr0y[same])<0.001)
		y_same=true;
	    }
	  if(!x_same)
         for(int i=0;i<Event_Out->n_Tr0x;i++)
 	  {
 	    THyphiHit_new temp_x_hit;
 	    Event_Out->Get_Hit(i,"Tr0x",temp_x_hit);
 

	      {
		h_PosResX[0]->Fill(temp_x_hit.x - cx );
		h_PosResX_angle[0]->Fill(temp_x_hit.x-cx,p1x);
		for(int cl=0;cl<temp_x_hit.Id_x.size();cl++)
		  {
		    h_PosResX_pos[0]->Fill(temp_x_hit.x - cx,temp_x_hit.Id_x[cl]);
		  }

		calctr0x.push_back(cx);
		
	      }
 	  }
	  if(!y_same)
         for(int i=0;i<Event_Out->n_Tr0y;i++)
 	  {
 	    THyphiHit_new temp_x_hit;
 	    Event_Out->Get_Hit(i,"Tr0y",temp_x_hit);
	    
	      {
		h_PosResY[0]->Fill(temp_x_hit.y - cy );
		h_PosResY_angle[0]->Fill(temp_x_hit.y-cy,p1y);
		for(int cl=0;cl<temp_x_hit.Id_y.size();cl++)
		  {
		    h_PosResY_pos[0]->Fill(temp_x_hit.y - cy,temp_x_hit.Id_y[cl]);
		  }

		calctr0y.push_back(cy);
		
	      }
 	  }



// 	  calctr0x.push_back(cx);
// 	  calctr0y.push_back(cy);

//  	  for(size_t tr0=0;tr0<TR0HitCond.Pos.size();tr0++)
//  	    {

//  	      double mx=TR0HitCond.Pos[tr0].X();
//  	      double my=TR0HitCond.Pos[tr0].Y();
//  	      double mz=TR0HitCond.Pos[tr0].Z();

// // 	      double cx= p1x*mz+p2x;
// // 	      double cy= p1y*mz+p2y;

// // 	      h_PosResX[0]->Fill(mx - cx );
// // 	      h_PosResY[0]->Fill(my - cy);
// // 	      if(fabs(my-cy)<10)
// // 		h_PosResX_angle[0]->Fill(mx-cx,p1x);

// // 	      if(fabs(mx-cx)<10)
// 	      {
// 		bool x_same=false;
// 		bool y_same=false;
// 		for(int same=0;same<calctr0x.size();same++)
// 		  {
// 		    if(fabs(cx-calctr0x[same])<0.001)
// 		      x_same=true;
// 		  }
		
// 		if(!x_same)
// 		  {
// 		    h_PosResX[0]->Fill(mx - cx );
// 		    h_PosResX_angle[0]->Fill(mx-cx,p1x);
// 		    calctr0x.push_back(cx);
// 		  }

// 		for(int same=0;same<calctr0y.size();same++)
// 		  {
// 		    if(fabs(cy-calctr0y[same])<0.001)
// 		      y_same=true;
// 		  }
// 		if(!y_same)
// 		  {
// 		    h_PosResY[0]->Fill(my - cy);
// 		    h_PosResY_angle[0]->Fill(my-cy,p1y);
// 		    calctr0y.push_back(cy);

// 		  }

		
// 	      }
// // 		h_PosResY_angle[0]->Fill(my-cy,p1y);


//  	    }

	}
    }




  if(Event_Out->n_Tr0x<5&&Event_Out->n_Tr0y<5&&
     Event_Out->n_Tr2x<5&&Event_Out->n_Tr2y<5)
  for(size_t tr0=0;tr0<TR0HitCond.Pos.size();tr0++)
    {
      for(size_t tr2=0;tr2<TR2HitCond.Pos.size();tr2++)
	{
	  double x[3],y[3],z[3];
	  //double reso_tofp[3]={ResTR1,ResTR2,ResTOFp};

	  x[0]=TR0HitCond.Pos[tr0].X();
	  y[0]=TR0HitCond.Pos[tr0].Y();
	  z[0]=TR0HitCond.Pos[tr0].Z();
		
	  x[1]=TR2HitCond.Pos[tr2].X();
	  y[1]=TR2HitCond.Pos[tr2].Y();
	  z[1]=TR2HitCond.Pos[tr2].Z();

	  double p1x = (x[1]-x[0])/(z[1]-z[0]);
	  double p2x = x[1] - p1x*z[1];
	  
	  double p1y = (y[1]-y[0])/(z[1]-z[0]);
	  double p2y = y[1] - p1y*z[1];

	  int usedid[6]={0,0,0,0,0,0};
	  bool coin[6] ={false,false,false,false,false,false};
	  bool cc = CheckChambHitIdP(p1x,p2x,p1y,p2y,min_tofs,Event_Out,usedid,coin);

	  if(!cc)continue;

	  double cutfuncl = timezerobar*3.0-18.0;
	  double cutfunch = timezerobar*3.0-12.0;

	  if(p2x<cutfuncl|| p2x>cutfunch)continue;
	  if(fabs(p2x)>20 || fabs(p2y)>20)continue;

	  bool tofpcoin = false;
	  for(int tofp=0;tofp<Event_Out->n_TofPlusBar;tofp++)		
	    {			
	      THyphiHit_new temp_tof;
	      Event_Out->Get_Hit(tofp,"TofPlusBar",temp_tof);
	      {
		if(fabs(temp_tof.y - (p1y*temp_tof.z+p2y))<25.0)
		  tofpcoin = true;
	      }
	    }
	  double posx[5],posz[5];
	  bool hoge2=CheckChambHitQueenIdPHP(100,p1x,p2x,
					     p1y,p2y,
					     Event_Out,posx,posz,coin);


// 	  if(!coin[2])continue;
// 	  if(!coin[3])continue;
	  
	  

	  if(!tofpcoin)continue;
	  double cx= p1x*TR1zPos+p2x;
	  double cy= p1y*TR1zPos+p2y;
	  bool x_same=false;
	  bool y_same=false;
	  for(int same=0;same<calctr1x.size();same++)
	    {
	      if(fabs(cx-calctr1x[same])<0.001)
		x_same=true;
	    }
	  for(int same=0;same<calctr1y.size();same++)
	    {
	      if(fabs(cy-calctr1y[same])<0.001)
		y_same=true;
	    }

	  if(!x_same)
         for(int i=0;i<Event_Out->n_Tr1x;i++)
 	  {
 	    THyphiHit_new temp_x_hit;
 	    Event_Out->Get_Hit(i,"Tr1x",temp_x_hit);
	    

	      {
		h_PosResX[1]->Fill(temp_x_hit.x - cx );
		h_PosResX_angle[1]->Fill(temp_x_hit.x-cx,p1x);
		for(int cl=0;cl<temp_x_hit.Id_x.size();cl++)
		  {
		    h_PosResX_pos[1]->Fill(temp_x_hit.x - cx,temp_x_hit.Id_x[cl]);
		  }

		calctr1x.push_back(cx);
		
	      }
 	  }
	  if(!y_same)
         for(int i=0;i<Event_Out->n_Tr1y;i++)
 	  {
 	    THyphiHit_new temp_x_hit;
 	    Event_Out->Get_Hit(i,"Tr1y",temp_x_hit);
	      {
		h_PosResY[1]->Fill(temp_x_hit.y - cy );
		h_PosResY_angle[1]->Fill(temp_x_hit.y-cy,p1y);
		for(int cl=0;cl<temp_x_hit.Id_y.size();cl++)
		  {
		    h_PosResY_pos[1]->Fill(temp_x_hit.y - cy,temp_x_hit.Id_y[cl]);
		  }
		
		calctr1y.push_back(cy);
		
	      }
 	  }


// 	  calctr1x.push_back(cx);
// 	  calctr1y.push_back(cy);

 	  // for(size_t tr1=0;tr1<TR1HitCond.Pos.size();tr1++)
//  	    {

//  	      double mx=TR1HitCond.Pos[tr1].X();
//  	      double my=TR1HitCond.Pos[tr1].Y();
//  	      double mz=TR1HitCond.Pos[tr1].Z();

// // 	      double cx= p1x*mz+p2x;
// // 	      double cy= p1y*mz+p2y;

// // 	      h_PosResX[1]->Fill(mx - cx );
// // 	      h_PosResY[1]->Fill(my - cy);
// // 	      if(fabs(my-cy)<10)
// // 		h_PosResX_angle[1]->Fill(mx-cx,p1x);

// // 	      if(fabs(mx-cx)<10)
// // 		h_PosResY_angle[1]->Fill(my-cy,p1y);
// 	      {
// 		bool x_same=false;
// 		bool y_same=false;
// 		for(int same=0;same<calctr1x.size();same++)
// 		  {
// 		    if(fabs(cx-calctr1x[same])<0.001)
// 		      x_same=true;
// 		  }
		
// 		if(!x_same)
// 		  {
// 		    h_PosResX[1]->Fill(mx - cx );
// 		    h_PosResX_angle[1]->Fill(mx-cx,p1x);

// 		    calctr1x.push_back(cx);
// 		  }

// 		for(int same=0;same<calctr1y.size();same++)
// 		  {
// 		    if(fabs(cy-calctr1y[same]) < 0.001)
// 		      y_same=true;
// 		 } 
// 		if(!y_same)
// 		  {
// 		    h_PosResY[1]->Fill(my - cy);
// 		    h_PosResY_angle[1]->Fill(my-cy,p1y);
// 		    calctr1y.push_back(cy);

// 		  }

// 	      }
//  	    }
	  
	  
	}
    }

//   std::sort(calctr2x.begin(),calctr2x.end());
//   std::vector<double>::iterator end_ite_1=std::unique(calctr2x.begin(),calctr2x.end());
//   calctr2x.erase(end_ite_tr2x,calctr2x.end());
//   for(size_t c=0;c<calctr2x.size();c++)
//     {
//         for(int i=0;i<Event_Out->n_Tr2x;i++)
// 	  {
// 	    THyphiHit_new temp_x_hit;
// 	    Event_Out->Get_Hit(i,"Tr2x",temp_x_hit);
// 	    h_PosResX[2]->Fill(temp_x_hit.x - calctr2x[c] );
// 	  }
//     }





 }
#endif

 {
   int cond_id[18];
   for(int i=0;i<18;i++)
     {
       cond_id[i]=0;
     }

   for(int i=0;i<Event_Out->n_TofPlusPlus;i++)
     {
       
       THyphiHit_new temp_plusplus;
       Event_Out->Get_Hit(i,"TofPlusPlus",temp_plusplus);
       if(temp_plusplus.Id_x[0]<19)
	 {
	   cond_id[temp_plusplus.Id_x[0]-1] = 1;
	 }
     }
   for(int i=0;i<18;i++)
     {
       if(cond_id[i]==1)
	 {
	   for(int j=i+1;j<18;j++)
	     {
	       if(cond_id[j]==1)
		 {
		   htpp_Hit2Cor->Fill(i,j);

		 }
	     }
	 }
     }
   
  
 }



  for(int i=0;i<Event_Out->n_TofPlusPlus;i++)
    {
      
      THyphiHit_new temp_plusplus;
      Event_Out->Get_Hit(i,"TofPlusPlus",temp_plusplus);
      
        for(int j=0;j<Event_Out->n_TofAladin;j++)
	  {
	    THyphiHit_new temp_aladin;
	    Event_Out->Get_Hit(i,"TofAladin",temp_aladin);
	    
	    if(temp_plusplus.x>0&&temp_aladin.x>0)
	      {
		if(temp_plusplus.Id_x[0]<19)
		  {
		    h_PosCor_LandAladinXX[0]->Fill(temp_plusplus.x,temp_aladin.x);
		    h_PosCor_LandAladinXX_time[0]->Fill(5.0/4.0*temp_plusplus.x-100-temp_aladin.x,temp_plusplus.t-TimeZero);

		    h_PosCor_LandAladin[0]->Fill(temp_plusplus.Id_x[0],temp_aladin.Id_x[0]);
		    h_PosCor_LandAladin[2]->Fill(temp_plusplus.y,temp_aladin.y);

		  }
		else 
		  {
		    h_PosCor_LandAladinXX[1]->Fill(temp_plusplus.x,temp_aladin.x);
		    //h_PosCor_LandAladinXX_time[1]->Fill(temp_plusplus.x,temp_aladin.x);
		    h_PosCor_LandAladin[1]->Fill(temp_plusplus.Id_x[0],temp_aladin.Id_x[0]);
		    h_PosCor_LandAladin[3]->Fill(temp_plusplus.y,temp_aladin.y);

		  }
	      }
	  }

    }


  return true;












  if(TofPlusBarHitCond.Pos.size()>0)
    h_status_new->Fill(7);



  const int NhitBg=TofPlusPlusHitCond.Pos.size();
  bool bar_veto[NhitBg];
  double bar_veto_chi[NhitBg];
  double bar_veto_mass[NhitBg];
  double bar_veto_mass_p[NhitBg];
  for(int nh = 0;nh<NhitBg;nh++)
    {
      bar_veto[nh]=false;
      bar_veto_chi[nh]=9999;
      bar_veto_mass[nh]=-1;
      bar_veto_mass_p[nh]=-1;
    }
      
      





#ifdef BGPRE

  { // Big TOF Bar Veto
    double x[4],y[4],z[4];
    double p1=0;double p2=0;
    double reso_aladin[4] = {0.59,0.59,0.59,105.0};
    double chi=0;double rchi=0;
    for(size_t tr0=0;tr0<TR0HitCond.Pos.size();tr0++)
      for(size_t tr1=0;tr1<TR1HitCond.Pos.size();tr1++)
	for(size_t tr2=0;tr2<TR2HitCond.Pos.size();tr2++)
	  for(size_t j=0;j<TofPlusPlusHitCond.Pos.size();j++)
	    {

	      x[0]=TR0HitCond.Pos[tr0].X();
	      y[0]=TR0HitCond.Pos[tr0].Y();
	      z[0]=TR0HitCond.Pos[tr0].Z();

	      x[1]=TR1HitCond.Pos[tr1].X();
	      y[1]=TR1HitCond.Pos[tr1].Y();
	      z[1]=TR1HitCond.Pos[tr1].Z();
	    
	      x[2]=TR2HitCond.Pos[tr2].X();
	      y[2]=TR2HitCond.Pos[tr2].Y();
	      z[2]=TR2HitCond.Pos[tr2].Z();

	    
	      x[3] = TofPlusPlusHitCond.Pos[j].X();
	      y[3] = TofPlusPlusHitCond.Pos[j].Y();
	      z[3] = TofPlusPlusHitCond.Pos[j].Z();

	      MT_LinearFit_Chi2(4, z, y, reso_aladin,
				&p1,&p2,&chi,&rchi);




	      //h_chi_pre_s->Fill(rchi);



	      if(rchi<1)
		{

		  double pos_bfm1p[3]; //tr1
		  double pos_bfm2p[3]; //tr2
		
		  double pos_afm2[3]; //bigtof
		  MT_Input_to_3Vector(x[1],
				      y[1],
				      z[1],pos_bfm1p);
		
		  MT_Input_to_3Vector(x[2],
				      y[2],
				      z[2],pos_bfm2p);
		
		
		  MT_Input_to_3Vector(x[3],
				      y[3],
				      z[3],pos_afm2);

		  double length_xyz=0;
		  double ini_rho = GetRho(-1,pos_bfm1p[0],pos_bfm1p[2],pos_bfm2p[0],pos_bfm2p[2],
					  pos_afm2[0],pos_afm2[1],pos_afm2[2],length_xyz);//m






		  //cout<<ini_rho_p<<endl;
		  double beta_s= length_xyz/TofPlusPlusHitCond.Id[j].X()/300.;
		  double gamma_s = 1./sqrt(1.-beta_s*beta_s);
		  double betag=beta_s*gamma_s;

		  double ini_s=0;
		  MT_RhoToMomentum(Field_Strength,ini_rho,
				   1,&ini_s);

		  double vecin[3];
		  double vecin_para[2];
		  for(int an=0;an<3;an++)
		    {
		      vecin[an] = pos_bfm2p[an]-pos_bfm1p[an];
		    }
		  vecin_para[0] = vecin[0];
		  vecin_para[1] = vecin[2];
		
		  double vecin_abs =
		    MT_AbsoluteValueOfVector(3,vecin);
		
		  double vecin_abs_para =
		    MT_AbsoluteValueOfVector(2,vecin_para);
		
		  double ratio = vecin_abs/vecin_abs_para;
		  double ini_s_tot =ini_s * ratio;
		
		  double vec_unit[3];
		  for(int an=0;an<3;an++)
		    {
		      //  vec_unit[an] = vecin[an]/vecin_abs;
		      vec_unit[an] = vecin[an]/vecin_abs;
		    }
		  double recmass_bg=-100.0;
		  recmass_bg = ini_s_tot/betag;

		  // 		double recmass_p=-100.0;
		  // 		recmass_p = ini_p_tot/betag;
		  //		h_recmass_inv_s_pre->Fill(recmass_bg);
		  //  		if(recmass_bg>PionMassCut[0] &&recmass_bg<PionMassCut[1])
		  //  		  {
		  //  		    bar_veto[j]=true;
		  //  		  }
		  if(rchi<bar_veto_chi[j])
		    {
		      bar_veto_chi[j] = rchi;
		      bar_veto_mass[j] = recmass_bg;
		    }

		}

	    }
    int vetocount=0;
    for(int nh=0;nh<NhitBg;nh++)
      {
	if(bar_veto_mass[nh]>0)
	  {
	    h_recmass_s_pre->Fill(bar_veto_mass[nh]);
	    if(bar_veto_mass[nh]>0.1 &&bar_veto_mass[nh]<0.22)
	      {
		bar_veto[nh]=true;
		vetocount++;
	      }
	  }
      }
    //      if(NhitBg>0)
    //        cout<<vetocount<<" "<<NhitBg<<endl;
    if(vetocount==NhitBg)return false;
  }
#endif


#ifdef DEBUG
  cout<<funcname<<" line "<<__LINE__<<endl;
#endif
      
  return true;      
}//Bool_t Friend::HitCondAna(int en,THyphiEvent_Phys_new* const Event_Out)    
  
//   h_NumPcand  ->Fill(ProtonTrack.size());
//   h_NumPipcand->Fill(PipTrack.size());
//   h_NumDcand  ->Fill(DeuteronTrack.size());
//   h_NumPimcand->Fill(PionTrack.size());
//   {
//     if( PionTrack.size()>0 &&( AlphaTrack.size()>0 || Alpha3Track.size()>0) )
//       cout<<"Proton = "<<ProtonTrack.size()<<" Pi- ="<<PionTrack.size()<<" He3 ="<<Alpha3Track.size()<<" He4 ="<<AlphaTrack.size()<<endl;
//     double x_pr[3],y_pr[3],z_pr[3];
//     double x_al[3],y_al[3],z_al[3];
//     double x_pi[3],y_pi[3],z_pi[3];
	
//     double temp_best_chi_l = 999;
//     double best_mass_l=-999;
//     double best_secz_l=-999;
//     double best_dist_l=-999;
	
//     double temp_best_chi_l2 = 999;
//     double best_mass_l2=-999;
//     double best_secz_l2=-999;
//     double best_dist_l2=-999;

//     double temp_best_chi_k = 999;
//     double best_mass_k=-9990;
//     double best_secz_k=-9990;
//     double best_dist_k=-9990;

	  
    
//     for(size_t pr=0;pr<PipTrack.size();pr++)
//       {
// 	for(size_t pi=0;pi<PionTrack.size();pi++)
// 	  {
		  

// 	    //		  cout<<"come here"<<endl;

// 	    z_pr[0] = TR1zPos;
// 	    x_pr[0] = PipTrackPosX[pr].X()*z_pr[0]+PipTrackPosX[pr].Y();
// 	    y_pr[0] = PipTrackPosY[pr].X()*z_pr[0]+PipTrackPosY[pr].Y();

// 	    z_pr[1] = TR2zPos;
// 	    x_pr[1] = PipTrackPosX[pr].X()*z_pr[1]+PipTrackPosX[pr].Y();
// 	    y_pr[1] = PipTrackPosY[pr].X()*z_pr[1]+PipTrackPosY[pr].Y();



// 	    z_pi[0] = TR1zPos;
// 	    x_pi[0] = PionTrackPosX[pi].X()*z_pi[0]+PionTrackPosX[pi].Y();
// 	    y_pi[0] = PionTrackPosY[pi].X()*z_pi[0]+PionTrackPosY[pi].Y();

// 	    z_pi[1] = TR2zPos;
// 	    x_pi[1] = PionTrackPosX[pi].X()*z_pi[1]+PionTrackPosX[pi].Y();
// 	    y_pi[1] = PionTrackPosY[pi].X()*z_pi[1]+PionTrackPosY[pi].Y();


// 	    if(x_pr[0] == x_pi[0])continue;
// 	    if(x_pr[1] == x_pi[1])continue;
		  
// 	    if(y_pr[0] == y_pi[0])continue;
// 	    if(y_pr[1] == y_pi[1])continue;
		    


// 	    double vec_p1[3],vec_p2[3];
// 	    MT_Input_to_3Vector(x_pr[0],
// 				y_pr[0],
// 				z_pr[0],vec_p1);
		  
// 	    MT_Input_to_3Vector(x_pr[1],
// 				y_pr[1],
// 				z_pr[1],vec_p2);



// 	    double vec_pi1[3],vec_pi2[3];
// 	    MT_Input_to_3Vector(x_pi[0],
// 				y_pi[0],
// 				z_pi[0],vec_pi1);
		  
// 	    MT_Input_to_3Vector(x_pi[1],
// 				y_pi[1],
// 				z_pi[1],vec_pi2);


		  
// 	    MT_Line line_proton;
// 	    line_proton.From_Points(vec_p1,vec_p2);

// 	    MT_Line line_pion;
// 	    line_pion.From_Points(vec_pi1,vec_pi2);
		  
// 	    double vec5[3];
// 	    double dist_h=
// 	      MT_Distance_of_Lines(&line_proton,&line_pion,vec5);

// 	    if(dist_h<0.01)continue;

// 	    h_newinv_secz_k->Fill(vec5[2]);
// 	    h_newinv_dist_k->Fill(dist_h);




// 	    double mom_pi[3],mom_proton[3];
// 	    mom_proton[0] = PipTrack[pr].X();
// 	    mom_proton[1] = PipTrack[pr].Y();
// 	    mom_proton[2] = PipTrack[pr].Z();

// 	    mom_pi[0] = PionTrack[pi].X();
// 	    mom_pi[1] = PionTrack[pi].Y();
// 	    mom_pi[2] = PionTrack[pi].Z();




// 	    // 		  double InvMass_L = MT_InvariantMass(M_pi, mom_pi,
// 	    // 						      M_proton, mom_proton);

// 	    double InvMass_K = MT_InvariantMass(M_pi, mom_pi,
// 						M_pi, mom_proton);



		  

		  
// 	    // 		  double comp_chi =PipTrackChi[pr]+PionTrackChi[pi];
		  
// 	    // 		  if(comp_chi<temp_best_chi_k)
// 	    // 		    {
// 	    // 		      temp_best_chi_k = comp_chi;
// 	    // 		      best_mass_k = InvMass_K;
// 	    // 		      best_secz_k = vec5[2];
// 	    // 		      //best_mass_l = InvMass_L;
// 	    // 		      best_dist_k = dist_h;
// 	    // 		    }
		  
		  
// 	    if(vec5[2]>-100 && vec5[2]<100)
// 	      if(dist_h<temp_best_chi_k)
// 		{
// 		  temp_best_chi_k = dist_h;
// 		  best_mass_k = InvMass_K;
// 		  best_secz_k = vec5[2];
// 		  best_dist_k = dist_h;
// 		}
		  
		  
		  
		  
		  
		  
		  
// 	  }
	      
//       }
	  

//     double temp_best_chi_2d = 999;
//     double best_mass_2d=-999;
//     double best_dist_2d=-999;


//     if(DeuteronTrack.size()>=2)
//       for(size_t d1=0;d1<DeuteronTrack.size();d1++)
// 	{
// 	  for(size_t d2=d1+1;d2<DeuteronTrack.size();d2++)
// 	    {
// 	      //cout<<DeuteronTrackTOF[d1]<< DeuteronTrackTOF[d2]<<endl;
// 	      if(DeuteronTrackTOF[d1] == DeuteronTrackTOF[d2])continue;
		    
// 	      z_pr[0] = TR1zPos;
// 	      x_pr[0] = DeuteronTrackPosX[d1].X()*z_pr[0]+DeuteronTrackPosX[d1].Y();
// 	      y_pr[0] = DeuteronTrackPosY[d1].X()*z_pr[0]+DeuteronTrackPosY[d1].Y();
		    
// 	      z_pr[1] = TR2zPos;
// 	      x_pr[1] = DeuteronTrackPosX[d1].X()*z_pr[1]+DeuteronTrackPosX[d1].Y();
// 	      y_pr[1] = DeuteronTrackPosY[d1].X()*z_pr[1]+DeuteronTrackPosY[d1].Y();
		    
		    
		    
// 	      z_pi[0] = TR1zPos;
// 	      x_pi[0] = DeuteronTrackPosX[d2].X()*z_pi[0]+DeuteronTrackPosX[d2].Y();
// 	      y_pi[0] = DeuteronTrackPosY[d2].X()*z_pi[0]+DeuteronTrackPosY[d2].Y();
		    
// 	      z_pi[1] = TR2zPos;
// 	      x_pi[1] = DeuteronTrackPosX[d2].X()*z_pi[1]+DeuteronTrackPosX[d2].Y();
// 	      y_pi[1] = DeuteronTrackPosY[d2].X()*z_pi[1]+DeuteronTrackPosY[d2].Y();
		    
		    
		    
		    
// 	      double vec_p1[3],vec_p2[3];
// 	      MT_Input_to_3Vector(x_pr[0],
// 				  y_pr[0],
// 				  z_pr[0],vec_p1);
		    
// 	      MT_Input_to_3Vector(x_pr[1],
// 				  y_pr[1],
// 				  z_pr[1],vec_p2);
		    
		    
		    
// 	      double vec_pi1[3],vec_pi2[3];
// 	      MT_Input_to_3Vector(x_pi[0],
// 				  y_pi[0],
// 				  z_pi[0],vec_pi1);
		    
// 	      MT_Input_to_3Vector(x_pi[1],
// 				  y_pi[1],
// 				  z_pi[1],vec_pi2);
		    
		    
		    
// 	      MT_Line line_proton;
// 	      line_proton.From_Points(vec_p1,vec_p2);
		    
// 	      MT_Line line_pion;
// 	      line_pion.From_Points(vec_pi1,vec_pi2);
		    
// 	      double vec5[3];
// 	      double dist_h=
// 		MT_Distance_of_Lines(&line_proton,&line_pion,vec5);
		    
// 	      if(dist_h<0.01)continue;
		    
// 	      h_newinv_secz_2d->Fill(vec5[2]);
// 	      h_newinv_dist_2d->Fill(dist_h);
		    
		    
		    
		    
// 	      double mom_pi[3],mom_proton[3];
// 	      mom_proton[0] = DeuteronTrack[d1].X();
// 	      mom_proton[1] = DeuteronTrack[d1].Y();
// 	      mom_proton[2] = DeuteronTrack[d1].Z();
		    
// 	      mom_pi[0] = DeuteronTrack[d2].X();
// 	      mom_pi[1] = DeuteronTrack[d2].Y();
// 	      mom_pi[2] = DeuteronTrack[d2].Z();
		    
		    
		    
		    
// 	      // 		  double InvMass_L = MT_InvariantMass(M_pi, mom_pi,
// 	      // 						      M_proton, mom_proton);
		    
// 	      double InvMass_2D = MT_InvariantMass(1.87503, mom_pi,
// 						   1.87503, mom_proton);
		    
		    
		    
		    
		    
		    
// 	      double comp_chi =DeuteronTrackChi[d1]+DeuteronTrackChi[d2];
		    
// 	      if(comp_chi<temp_best_chi_2d)
// 		{
// 		  temp_best_chi_2d = comp_chi;
// 		  best_mass_2d = InvMass_2D;
// 		  best_dist_2d = dist_h;
// 		}
		    
// 	    }
		
// 	}
	  
	  
	  
	  

//     //  	  double temp_best_chi_2p = 999;
//     // 	  double best_mass_2p=-999;
//     // 	  double best_dist_2p=-999;


//     if(ProtonTrack.size()>=2)
//       for(size_t d1=0;d1<ProtonTrack.size();d1++)
// 	{
// 	  for(size_t d2=d1+1;d2<ProtonTrack.size();d2++)
// 	    {
		    
// 	      if(ProtonTrackTOF[d1] == ProtonTrackTOF[d2])continue;

		    
// 	      z_pr[0] = TR1zPos;
// 	      x_pr[0] = ProtonTrackPosX[d1].X()*z_pr[0]+ProtonTrackPosX[d1].Y();
// 	      y_pr[0] = ProtonTrackPosY[d1].X()*z_pr[0]+ProtonTrackPosY[d1].Y();
		    
// 	      z_pr[1] = TR2zPos;
// 	      x_pr[1] = ProtonTrackPosX[d1].X()*z_pr[1]+ProtonTrackPosX[d1].Y();
// 	      y_pr[1] = ProtonTrackPosY[d1].X()*z_pr[1]+ProtonTrackPosY[d1].Y();
		    
		    
		    
// 	      z_pi[0] = TR1zPos;
// 	      x_pi[0] = ProtonTrackPosX[d2].X()*z_pi[0]+ProtonTrackPosX[d2].Y();
// 	      y_pi[0] = ProtonTrackPosY[d2].X()*z_pi[0]+ProtonTrackPosY[d2].Y();
		    
// 	      z_pi[1] = TR2zPos;
// 	      x_pi[1] = ProtonTrackPosX[d2].X()*z_pi[1]+ProtonTrackPosX[d2].Y();
// 	      y_pi[1] = ProtonTrackPosY[d2].X()*z_pi[1]+ProtonTrackPosY[d2].Y();
		    
		    


// 	      if(x_pr[0] == x_pi[0])continue;
// 	      if(x_pr[1] == x_pi[1])continue;

// 	      if(y_pr[0] == y_pi[0])continue;
// 	      if(y_pr[1] == y_pi[1])continue;
		    
		    
// 	      double vec_p1[3],vec_p2[3];
// 	      MT_Input_to_3Vector(x_pr[0],
// 				  y_pr[0],
// 				  z_pr[0],vec_p1);
		    
// 	      MT_Input_to_3Vector(x_pr[1],
// 				  y_pr[1],
// 				  z_pr[1],vec_p2);
		    
		    
		    
// 	      double vec_pi1[3],vec_pi2[3];
// 	      MT_Input_to_3Vector(x_pi[0],
// 				  y_pi[0],
// 				  z_pi[0],vec_pi1);
		    
// 	      MT_Input_to_3Vector(x_pi[1],
// 				  y_pi[1],
// 				  z_pi[1],vec_pi2);
		    
		    
		    
// 	      MT_Line line_proton;
// 	      line_proton.From_Points(vec_p1,vec_p2);
		    
// 	      MT_Line line_pion;
// 	      line_pion.From_Points(vec_pi1,vec_pi2);
		    
// 	      double vec5[3];
// 	      double dist_h=
// 		MT_Distance_of_Lines(&line_proton,&line_pion,vec5);
		    
// 	      //		    if(dist_h<0.01)continue;
		    
// 	      //		    cout<<" Sec Z = ("<<vec5[0]<<","<<vec5[1]<<","<<vec5[2]<<")  Dist = "<<dist_h<<endl;
// 	      h_newinv_secz_2p->Fill(vec5[2]);
// 	      h_newinv_dist_2p->Fill(dist_h);
		    
		    
		    
		    
// 	      // 		    double mom_pi[3],mom_proton[3];
// 	      // 		    mom_proton[0] = ProtonTrack[d1].X();
// 	      // 		    mom_proton[1] = ProtonTrack[d1].Y();
// 	      // 		    mom_proton[2] = ProtonTrack[d1].Z();
		    
// 	      // 		    mom_pi[0] = ProtonTrack[d2].X();
// 	      // 		    mom_pi[1] = ProtonTrack[d2].Y();
// 	      // 		    mom_pi[2] = ProtonTrack[d2].Z();
		    
		    
		    
		    
// 	      // 		    // 		  double InvMass_L = MT_InvariantMass(M_pi, mom_pi,
// 	      // 		    // 						      M_proton, mom_proton);
		    
// 	      // 		    double InvMass_2D = MT_InvariantMass(1.87503, mom_pi,
// 	      // 							 1.87503, mom_proton);
		    
		    
		    
		    
		    
		    
// 	      // 		    double comp_chi =DeuteronTrackChi[d1]+DeuteronTrackChi[d2];
		    
// 	      // 		    if(comp_chi<temp_best_chi_2d)
// 	      // 		      {
// 	      // 			temp_best_chi_2d = comp_chi;
// 	      // 			best_mass_2d = InvMass_2D;
// 	      // 			best_dist_2d = dist_h;
// 	      // 		      }
		    
// 	    }
		
// 	}
	  


//     double temp_best_chi_ad = 999;
//     double best_mass_ad=-999;
//     double best_dist_ad=-999;
//     double best_secz_ad=-999;
//     int best_tofs_ad=0;
//     double best_pos_tofs[2] = {999,999};

//     double best_kene_ad=-999;
//     for(size_t d1=0;d1<AlphaTrack.size();d1++)
//       {
// 	for(size_t d2=0;d2<DeuteronTrack.size();d2++)
// 	  {
		    
// 	    //if(AlphaTrackTOF[d1] == DeuteronTrackTOF[d2])continue;
		    
// 	    z_pr[0] = TR1zPos;
// 	    x_pr[0] = AlphaTrackPosX[d1].X()*z_pr[0]+AlphaTrackPosX[d1].Y();
// 	    y_pr[0] = AlphaTrackPosY[d1].X()*z_pr[0]+AlphaTrackPosY[d1].Y();
		    
// 	    z_pr[1] = TR2zPos;
// 	    x_pr[1] = AlphaTrackPosX[d1].X()*z_pr[1]+AlphaTrackPosX[d1].Y();
// 	    y_pr[1] = AlphaTrackPosY[d1].X()*z_pr[1]+AlphaTrackPosY[d1].Y();
		    
		    
		    
// 	    z_pi[0] = TR1zPos;
// 	    x_pi[0] = DeuteronTrackPosX[d2].X()*z_pi[0]+DeuteronTrackPosX[d2].Y();
// 	    y_pi[0] = DeuteronTrackPosY[d2].X()*z_pi[0]+DeuteronTrackPosY[d2].Y();
		    
// 	    z_pi[1] = TR2zPos;
// 	    x_pi[1] = DeuteronTrackPosX[d2].X()*z_pi[1]+DeuteronTrackPosX[d2].Y();
// 	    y_pi[1] = DeuteronTrackPosY[d2].X()*z_pi[1]+DeuteronTrackPosY[d2].Y();
		    
		    
// 	    // 		    if(fabs(x_pr[0] - x_pi[0])<0.2)continue;
// 	    // 		    if(fabs(x_pr[1] - x_pi[1])<0.2)continue;

// 	    // 		    if(fabs(y_pr[0] - y_pi[0])<0.2)continue;
// 	    // 		    if(fabs(y_pr[1] - y_pi[1])<0.2)continue;
		    
		    
		    
// 	    double vec_p1[3],vec_p2[3];
// 	    MT_Input_to_3Vector(x_pr[0],
// 				y_pr[0],
// 				z_pr[0],vec_p1);
		    
// 	    MT_Input_to_3Vector(x_pr[1],
// 				y_pr[1],
// 				z_pr[1],vec_p2);
		    
		    
		    
// 	    double vec_pi1[3],vec_pi2[3];
// 	    MT_Input_to_3Vector(x_pi[0],
// 				y_pi[0],
// 				z_pi[0],vec_pi1);
		    
// 	    MT_Input_to_3Vector(x_pi[1],
// 				y_pi[1],
// 				z_pi[1],vec_pi2);
		    
		    
		    
// 	    MT_Line line_proton;
// 	    line_proton.From_Points(vec_p1,vec_p2);
		    
// 	    MT_Line line_pion;
// 	    line_pion.From_Points(vec_pi1,vec_pi2);
		    
// 	    double vec5[3];
// 	    double dist_h=
// 	      MT_Distance_of_Lines(&line_proton,&line_pion,vec5);
		    
// 	    //if(dist_h<0.01)continue;
		    
// 	    h_newinv_secz_ad->Fill(vec5[2]);
// 	    h_newinv_dist_ad->Fill(dist_h);
		    
		    
		    
		    
// 	    double mom_pi[3],mom_proton[3];
// 	    mom_proton[0] = AlphaTrack[d1].X();
// 	    mom_proton[1] = AlphaTrack[d1].Y();
// 	    mom_proton[2] = AlphaTrack[d1].Z();
		    
// 	    mom_pi[0] = DeuteronTrack[d2].X();
// 	    mom_pi[1] = DeuteronTrack[d2].Y();
// 	    mom_pi[2] = DeuteronTrack[d2].Z();
		    
		    
		    
// 	    double ini_mom[3];
// 	    double mom_li[3];
// 	    for(int com = 0;com<3;com++)
// 	      {
// 		ini_mom[com] = vec5[com]+mom_proton[com]+mom_pi[com];
// 		mom_li[com] =mom_proton[com]+mom_pi[com];
// 	      }






// 	    MT_Line line_ini;
// 	    line_ini.From_Points(vec5,ini_mom);

// 	    double tofs[3];
// 	    tofs[2]=TofszPos;
// 	    if(line_ini.Get_XY_atZ(tofs)==1)
// 	      {
// 		h_newinv_LiFrom->Fill(TimeZeroBar,tofs[0]);
			
// 	      }

		    

// 	    // 		  double InvMass_L = MT_InvariantMass(M_pi, mom_pi,
// 	    // 						      M_proton, mom_proton);
		    
// 	    double InvMass_AD = MT_InvariantMass(1.87503, mom_pi,
// 						 4.0026*0.931478, mom_proton);
		    
		    
		    
		    
// 	    double mom_li_tot = MT_AbsoluteValueOfVector(3,mom_li);
// 	    double ene_li = sqrt(mom_li_tot*mom_li_tot+InvMass_AD*InvMass_AD)-InvMass_AD;

		    
// 	    h_newinv_mass_ad_all->Fill(InvMass_AD);
// 	    h_newinv_kene_ad_all->Fill(ene_li/6.0);
// 	    double cutfuncl = (double)TimeZeroBar*3.0-18.0;
// 	    double cutfunch = (double)TimeZeroBar*3.0-12.0;
// 	    int cuttofs = 0;
// 	    if(tofs[0]>cutfuncl && tofs[0]<cutfunch)
// 	      {
// 		h_newinv_mass_ad_all_tofs->Fill(InvMass_AD);
// 		cuttofs = 1;
// 	      }

// 	    double comp_chi =AlphaTrackChi[d1]+DeuteronTrackChi[d2];

// 	    if(comp_chi<temp_best_chi_ad)
// 	      {
// 		temp_best_chi_ad = comp_chi;
// 		best_mass_ad = InvMass_AD;
// 		best_dist_ad = dist_h;
// 		best_secz_ad = vec5[2];
// 		best_tofs_ad=cuttofs;
// 		best_pos_tofs[0] = TimeZeroBar;
// 		best_pos_tofs[1] = tofs[0];
// 		best_kene_ad=ene_li;
// 	      }
		    
// 	  }
		
//       }
	  
	    
	  
	  
	  
	  
	  
	  
//     if(best_mass_k>0)
//       {
// 	if(best_secz_k<100 &&best_secz_k>-200)
// 	  h_newinv_mass_k->Fill(best_mass_k);
// 	h_newinv_mass_dist_k->Fill(best_mass_k,best_dist_k);
// 	//cout<<"Kaon Mass =============================================== "<<best_mass_k<<endl;
//       }
// //     if(best_mass_l>0)
// //       {
// // 	int fill=0;
// // 	if(best_secz_l>60 &&best_secz_l<250 && best_dist_l<2)
// // 	  {
// // 	    h_newinv_mass_l->Fill(best_mass_l);
// // 	    fill=1;
// // 	  }
	    
// // 	h_newinv_mass_secz_l->Fill(best_mass_l,best_secz_l);
// // 	h_newinv_mass_dist_l->Fill(best_mass_l,best_dist_l);
// // 	cout<<en<<" Lambda Mass ========================== "<<best_mass_l<<" Z="<<best_secz_l<<" dist="<<best_dist_l<<" fill "<<fill;
// // 	if(best_mass_l>1.110 && best_mass_l<1.1200)
// // 	  cout<<"        //////////////////////////////  "<<endl;
// // 	else
// // 	  cout<<endl;
// //       }

// //     if(best_mass_l2>0)
// //       {
// // 	int fill=0;
// // 	if(best_secz_l2>60 &&best_secz_l2<250 && best_dist_l2<3)
// // 	  {
// // 	    h_newinv_mass_l2->Fill(best_mass_l2);
// // 	    fill=1;
// // 	  }
	      
// // 	h_newinv_mass_secz_l2->Fill(best_mass_l2,best_secz_l2);
// // 	h_newinv_mass_dist_l2->Fill(best_mass_l2,best_dist_l2);

// // 	cout<<en<<" Lambda Mass ========================== "<<best_mass_l2<<" Z="<<best_secz_l2<<" dist="<<best_dist_l2<<"fill "<<fill;
// // 	if(best_mass_l2>1.110 && best_mass_l2<1.1200)
// // 	  cout<<"        ////////////////////////////// "<<endl;
// // 	else
// // 	  cout<<endl;
// //       }
//     // 	  if(best_mass_l>0 || best_mass_k>0)
//     //  	    cout<<"Proton Candidata = "<<ProtonTrack.size()<<" Pi+ candidate"<<PipTrack.size()<<" Pion Candidate ="<<PionTrack.size()<<endl;


//     if(best_mass_2d>0)
//       {
// 	h_newinv_mass_2d->Fill(best_mass_2d);
// 	h_newinv_mass_dist_2d->Fill(best_mass_2d,best_dist_2d);
// 	//cout<<"2D Mass ========================== "<<best_mass_2d<<endl;
//       }

//     if(best_mass_ad>0)
//       {
// 	h_newinv_mass_ad->Fill(best_mass_ad);
// 	h_newinv_kene_ad->Fill(best_kene_ad/6.0);
// 	if(best_tofs_ad==1)
// 	  {
// 	    h_newinv_mass_ad_tofs->Fill(best_mass_ad);
// 	    h_newinv_mass_ad_tofs_secz->Fill(best_mass_ad,best_secz_ad);
// 	  }
// 	h_newinv_mass_dist_ad->Fill(best_mass_ad,best_dist_ad);
// 	h_newinv_secz_ad_best->Fill(best_secz_ad);
// 	h_newinv_dist_ad_best->Fill(best_dist_ad);
// 	h_newinv_LiFrom_best->Fill(best_pos_tofs[0],best_pos_tofs[1]);
// 	//	      cout<<"AD Mass ========================== "<<best_mass_ad<<endl;
//       }

	  

//     double temp_best_chi_a3p = 999;
//     double best_mass_a3p=-999;
//     double best_dist_a3p=-999;
//     double best_secz_a3p=-999;
//     for(size_t pr=0;pr<Alpha3Track.size();pr++)
//       {
// 	for(size_t pi=0;pi<PionTrack.size();pi++)
// 	  {
// 	    if(Alpha3TrackTR0[pr]!=1)continue;

// 	    //		  cout<<"come here"<<endl;

// 	    z_pr[0] = TR1zPos;
// 	    x_pr[0] = Alpha3TrackPosX[pr].X()*z_pr[0]+Alpha3TrackPosX[pr].Y();
// 	    y_pr[0] = Alpha3TrackPosY[pr].X()*z_pr[0]+Alpha3TrackPosY[pr].Y();

// 	    z_pr[1] = TR2zPos;
// 	    x_pr[1] = Alpha3TrackPosX[pr].X()*z_pr[1]+Alpha3TrackPosX[pr].Y();
// 	    y_pr[1] = Alpha3TrackPosY[pr].X()*z_pr[1]+Alpha3TrackPosY[pr].Y();

// 	    z_pr[2] = TR0zPos;
// 	    x_pr[2] = Alpha3TrackPosX[pr].X()*z_pr[2]+Alpha3TrackPosX[pr].Y();
// 	    y_pr[2] = Alpha3TrackPosY[pr].X()*z_pr[2]+Alpha3TrackPosY[pr].Y();



// 	    z_pi[0] = TR1zPos;
// 	    x_pi[0] = PionTrackPosX[pi].X()*z_pi[0]+PionTrackPosX[pi].Y();
// 	    y_pi[0] = PionTrackPosY[pi].X()*z_pi[0]+PionTrackPosY[pi].Y();

// 	    z_pi[1] = TR2zPos;
// 	    x_pi[1] = PionTrackPosX[pi].X()*z_pi[1]+PionTrackPosX[pi].Y();
// 	    y_pi[1] = PionTrackPosY[pi].X()*z_pi[1]+PionTrackPosY[pi].Y();
		  
// 	    z_pi[2] = TR0zPos;
// 	    x_pi[2] = PionTrackPosX[pi].X()*z_pi[2]+PionTrackPosX[pi].Y();
// 	    y_pi[2] = PionTrackPosY[pi].X()*z_pi[2]+PionTrackPosY[pi].Y();
		  

// 	    if(fabs(x_pr[0] - x_pi[0])<0.1)continue;
// 	    if(fabs(x_pr[1] - x_pi[1])<0.1)continue;
// 	    if(fabs(x_pr[2] - x_pi[2])<0.1)continue;
		  
// 	    if(fabs(y_pr[0] - y_pi[0])<0.1)continue;
// 	    if(fabs(y_pr[1] - y_pi[1])<0.1)continue;
// 	    if(fabs(y_pr[2] - y_pi[2])<0.1)continue;


// 	    double vec_p1[3],vec_p2[3];
// 	    MT_Input_to_3Vector(x_pr[0],
// 				y_pr[0],
// 				z_pr[0],vec_p1);
		  
// 	    MT_Input_to_3Vector(x_pr[1],
// 				y_pr[1],
// 				z_pr[1],vec_p2);



// 	    double vec_pi1[3],vec_pi2[3];
// 	    MT_Input_to_3Vector(x_pi[0],
// 				y_pi[0],
// 				z_pi[0],vec_pi1);
		  
// 	    MT_Input_to_3Vector(x_pi[1],
// 				y_pi[1],
// 				z_pi[1],vec_pi2);


		  
// 	    MT_Line line_proton;
// 	    line_proton.From_Points(vec_p1,vec_p2);

// 	    MT_Line line_pion;
// 	    line_pion.From_Points(vec_pi1,vec_pi2);
		  
// 	    double vec5[3];
// 	    double dist_h=
// 	      MT_Distance_of_Lines(&line_proton,&line_pion,vec5);

// 	    if(dist_h<0.001)continue;

// 	    h_newinv_secz_a3p->Fill(vec5[2]);
// 	    h_newinv_dist_a3p->Fill(dist_h);




// 	    double mom_pi[3],mom_proton[3];
// 	    mom_proton[0] = Alpha3Track[pr].X();
// 	    mom_proton[1] = Alpha3Track[pr].Y();
// 	    mom_proton[2] = Alpha3Track[pr].Z();

// 	    mom_pi[0] = PionTrack[pi].X();
// 	    mom_pi[1] = PionTrack[pi].Y();
// 	    mom_pi[2] = PionTrack[pi].Z();




// 	    double InvMass_L = MT_InvariantMass(M_pi, mom_pi,
// 						M_alpha3, mom_proton);


		  
// 	    double comp_chi =Alpha3TrackChi[pr]+PionTrackChi[pi]+dist_h;
		  
		  
// 	    //if(vec5[2]>60 && vec5[2]<390 &&dist_h<2)
// 	    if(comp_chi<temp_best_chi_a3p)
// 	      {
// 		temp_best_chi_a3p = comp_chi;
// 		best_mass_a3p = InvMass_L;
// 		best_secz_a3p = vec5[2];
// 		best_dist_a3p = dist_h;
// 	      }
		  
		  
		  
		  
		  
		  
		  
		  
// 	  }
	      
//       }



//     if(best_mass_a3p>0)
//       {
// 	if(best_secz_a3p<400 &&best_secz_a3p>60&&best_dist_a3p<3)
// 	  h_newinv_mass_a3p->Fill(best_mass_a3p);
// 	h_newinv_mass_secz_a3p->Fill(best_mass_a3p,best_secz_a3p);
// 	h_newinv_mass_dist_a3p->Fill(best_mass_a3p,best_dist_a3p);
// 	cout<<"H3L  ========================== "<<best_mass_a3p<<endl;
//       }




//     double temp_best_chi_a4p = 999;
//     double best_mass_a4p=-999;
//     double best_dist_a4p=-999;
//     double best_secz_a4p=-999;
//     for(size_t pr=0;pr<AlphaTrack.size();pr++)
//       {
// 	for(size_t pi=0;pi<PionTrack.size();pi++)
// 	  {
// 	    if(AlphaTrackTR0[pr]!=1)continue;

// 	    //		  cout<<"come here"<<endl;

// 	    z_pr[0] = TR1zPos;
// 	    x_pr[0] = AlphaTrackPosX[pr].X()*z_pr[0]+AlphaTrackPosX[pr].Y();
// 	    y_pr[0] = AlphaTrackPosY[pr].X()*z_pr[0]+AlphaTrackPosY[pr].Y();

// 	    z_pr[1] = TR2zPos;
// 	    x_pr[1] = AlphaTrackPosX[pr].X()*z_pr[1]+AlphaTrackPosX[pr].Y();
// 	    y_pr[1] = AlphaTrackPosY[pr].X()*z_pr[1]+AlphaTrackPosY[pr].Y();

// 	    z_pr[2] = TR0zPos;
// 	    x_pr[2] = AlphaTrackPosX[pr].X()*z_pr[2]+AlphaTrackPosX[pr].Y();
// 	    y_pr[2] = AlphaTrackPosY[pr].X()*z_pr[2]+AlphaTrackPosY[pr].Y();



// 	    z_pi[0] = TR1zPos;
// 	    x_pi[0] = PionTrackPosX[pi].X()*z_pi[0]+PionTrackPosX[pi].Y();
// 	    y_pi[0] = PionTrackPosY[pi].X()*z_pi[0]+PionTrackPosY[pi].Y();

// 	    z_pi[1] = TR2zPos;
// 	    x_pi[1] = PionTrackPosX[pi].X()*z_pi[1]+PionTrackPosX[pi].Y();
// 	    y_pi[1] = PionTrackPosY[pi].X()*z_pi[1]+PionTrackPosY[pi].Y();
		  
// 	    z_pi[2] = TR0zPos;
// 	    x_pi[2] = PionTrackPosX[pi].X()*z_pi[2]+PionTrackPosX[pi].Y();
// 	    y_pi[2] = PionTrackPosY[pi].X()*z_pi[2]+PionTrackPosY[pi].Y();
		  

// 	    if(fabs(x_pr[0] - x_pi[0])<0.1)continue;
// 	    if(fabs(x_pr[1] - x_pi[1])<0.1)continue;
// 	    //if(fabs(x_pr[2] - x_pi[2])<0.1)continue;
		  
// 	    if(fabs(y_pr[0] - y_pi[0])<0.1)continue;
// 	    if(fabs(y_pr[1] - y_pi[1])<0.1)continue;
// 	    //if(fabs(y_pr[2] - y_pi[2])<0.1)continue;


// 	    double vec_p1[3],vec_p2[3];
// 	    MT_Input_to_3Vector(x_pr[0],
// 				y_pr[0],
// 				z_pr[0],vec_p1);
		  
// 	    MT_Input_to_3Vector(x_pr[1],
// 				y_pr[1],
// 				z_pr[1],vec_p2);



// 	    double vec_pi1[3],vec_pi2[3];
// 	    MT_Input_to_3Vector(x_pi[0],
// 				y_pi[0],
// 				z_pi[0],vec_pi1);
		  
// 	    MT_Input_to_3Vector(x_pi[1],
// 				y_pi[1],
// 				z_pi[1],vec_pi2);


		  
// 	    MT_Line line_proton;
// 	    line_proton.From_Points(vec_p1,vec_p2);

// 	    MT_Line line_pion;
// 	    line_pion.From_Points(vec_pi1,vec_pi2);
		  
// 	    double vec5[3];
// 	    double dist_h=
// 	      MT_Distance_of_Lines(&line_proton,&line_pion,vec5);

// 	    if(dist_h<0.001)continue;

// 	    h_newinv_secz_a4p->Fill(vec5[2]);
// 	    h_newinv_dist_a4p->Fill(dist_h);




// 	    double mom_pi[3],mom_proton[3];
// 	    mom_proton[0] = AlphaTrack[pr].X();
// 	    mom_proton[1] = AlphaTrack[pr].Y();
// 	    mom_proton[2] = AlphaTrack[pr].Z();

// 	    mom_pi[0] = PionTrack[pi].X();
// 	    mom_pi[1] = PionTrack[pi].Y();
// 	    mom_pi[2] = PionTrack[pi].Z();




// 	    double InvMass_L = MT_InvariantMass(M_pi, mom_pi,
// 						M_alpha4, mom_proton);


		  
// 	    double comp_chi =AlphaTrackChi[pr]+PionTrackChi[pi]+dist_h;
		  
		  
// 	    //if(vec5[2]>60 && vec5[2]<390 &&dist_h<2)
// 	    if(comp_chi<temp_best_chi_a4p)
// 	      {
// 		temp_best_chi_a4p = comp_chi;
// 		best_mass_a4p = InvMass_L;
// 		best_secz_a4p = vec5[2];
// 		best_dist_a4p = dist_h;
// 	      }
		  
		  
		  
		  
		  
		  
		  
		  
// 	  }
	      
//       }



//     if(best_mass_a4p>0)
//       {
// 	if(best_secz_a4p<400 &&best_secz_a4p>60&&best_dist_a4p<3)
// 	  h_newinv_mass_a4p->Fill(best_mass_a4p);
// 	h_newinv_mass_secz_a4p->Fill(best_mass_a4p,best_secz_a4p);
// 	h_newinv_mass_dist_a4p->Fill(best_mass_a4p,best_dist_a4p);
// 	cout<<"H4L  ========================== "<<best_mass_a4p<<endl;
//       }








//     //He5L
//     double temp_best_chi_a4pp = 999;
//     double best_mass_a4pp=-999;
//     double best_dist_a4pp=-999;
//     double best_secz_a4pp=-999;
//     for(size_t pr=0;pr<ProtonTrack.size();pr++)
//       for(size_t al=0;al<AlphaTrack.size();al++)
// 	{
// 	  for(size_t pi=0;pi<PionTrack.size();pi++)
// 	    {
// 	      //if(AlphaTrackTR0[al]!=1)continue;
		    
// 	      z_al[0] = TR1zPos;
// 	      x_al[0] = AlphaTrackPosX[al].X()*z_al[0]+AlphaTrackPosX[al].Y();
// 	      y_al[0] = AlphaTrackPosY[al].X()*z_al[0]+AlphaTrackPosY[al].Y();
		    
// 	      z_al[1] = TR2zPos;
// 	      x_al[1] = AlphaTrackPosX[al].X()*z_al[1]+AlphaTrackPosX[al].Y();
// 	      y_al[1] = AlphaTrackPosY[al].X()*z_al[1]+AlphaTrackPosY[al].Y();
		    
// 	      z_al[2] = TR0zPos;
// 	      x_al[2] = AlphaTrackPosX[al].X()*z_al[2]+AlphaTrackPosX[al].Y();
// 	      y_al[2] = AlphaTrackPosY[al].X()*z_al[2]+AlphaTrackPosY[al].Y();
		    
		    
// 	      z_pr[0] = TR1zPos;
// 	      x_pr[0] = ProtonTrackPosX[pr].X()*z_pr[0]+ProtonTrackPosX[pr].Y();
// 	      y_pr[0] = ProtonTrackPosY[pr].X()*z_pr[0]+ProtonTrackPosY[pr].Y();
		    
// 	      z_pr[1] = TR2zPos;
// 	      x_pr[1] = ProtonTrackPosX[pr].X()*z_pr[1]+ProtonTrackPosX[pr].Y();
// 	      y_pr[1] = ProtonTrackPosY[pr].X()*z_pr[1]+ProtonTrackPosY[pr].Y();
		    
// 	      z_pr[2] = TR0zPos;
// 	      x_pr[2] = ProtonTrackPosX[pr].X()*z_pr[2]+ProtonTrackPosX[pr].Y();
// 	      y_pr[2] = ProtonTrackPosY[pr].X()*z_pr[2]+ProtonTrackPosY[pr].Y();
		    
		    
// 	      z_pi[0] = TR1zPos;
// 	      x_pi[0] = PionTrackPosX[pi].X()*z_pi[0]+PionTrackPosX[pi].Y();
// 	      y_pi[0] = PionTrackPosY[pi].X()*z_pi[0]+PionTrackPosY[pi].Y();
		    
// 	      z_pi[1] = TR2zPos;
// 	      x_pi[1] = PionTrackPosX[pi].X()*z_pi[1]+PionTrackPosX[pi].Y();
// 	      y_pi[1] = PionTrackPosY[pi].X()*z_pi[1]+PionTrackPosY[pi].Y();
		    
// 	      z_pi[2] = TR0zPos;
// 	      x_pi[2] = PionTrackPosX[pi].X()*z_pi[2]+PionTrackPosX[pi].Y();
// 	      y_pi[2] = PionTrackPosY[pi].X()*z_pi[2]+PionTrackPosY[pi].Y();
		    

// 	      if(fabs(x_al[0] - x_pi[0])<0.1)continue;
// 	      if(fabs(x_al[1] - x_pi[1])<0.1)continue;
// 	      if(fabs(x_pr[0] - x_pi[0])<0.1)continue;
// 	      if(fabs(x_pr[1] - x_pi[1])<0.1)continue;
// 	      if(fabs(x_al[0] - x_pr[0])<0.1)continue;
// 	      if(fabs(x_al[1] - x_pr[1])<0.1)continue;

// 	      if(fabs(y_al[0] - y_pi[0])<0.1)continue;
// 	      if(fabs(y_al[1] - y_pi[1])<0.1)continue;
		    
// 	      if(fabs(y_pr[0] - y_pi[0])<0.1)continue;
// 	      if(fabs(y_pr[1] - y_pi[1])<0.1)continue;

// 	      if(fabs(y_al[0] - y_pr[0])<0.1)continue;
// 	      if(fabs(y_al[1] - y_pr[1])<0.1)continue;
// 	      //if(fabs(y_al[2] - y_pi[2])<0.1)continue;
		    

// 	      double vec_a1[3],vec_a2[3];
// 	      MT_Input_to_3Vector(x_al[0],
// 				  y_al[0],
// 				  z_al[0],vec_a1);
		  
// 	      MT_Input_to_3Vector(x_al[1],
// 				  y_al[1],
// 				  z_al[1],vec_a2);


// 	      double vec_p1[3],vec_p2[3];
// 	      MT_Input_to_3Vector(x_pr[0],
// 				  y_pr[0],
// 				  z_pr[0],vec_p1);
		  
// 	      MT_Input_to_3Vector(x_pr[1],
// 				  y_pr[1],
// 				  z_pr[1],vec_p2);



// 	      double vec_pi1[3],vec_pi2[3];
// 	      MT_Input_to_3Vector(x_pi[0],
// 				  y_pi[0],
// 				  z_pi[0],vec_pi1);
		  
// 	      MT_Input_to_3Vector(x_pi[1],
// 				  y_pi[1],
// 				  z_pi[1],vec_pi2);



		  
// 	      MT_Line line_proton;
// 	      line_proton.From_Points(vec_p1,vec_p2);

// 	      MT_Line line_al;
// 	      line_proton.From_Points(vec_a1,vec_a2);

// 	      MT_Line line_pion;
// 	      line_pion.From_Points(vec_pi1,vec_pi2);
		  


// 	      double vtx_D1D2[3]={0.,0.,0.};
// 	      double vtx_D2D3[3]={0.,0.,0.};
// 	      double vtx_D1D3[3]={0.,0.,0.};

		  
// 	      double dist_D1D2,dist_D2D3,dist_D1D3,dist_vtxD1D2_vtxD2D3,dist_vtxD1D2_vtxD1D3,dist_vtxD2D3_vtxD1D3;
// 	      dist_D1D2 = MT_Distance_of_Lines(&line_proton,&line_pion,vtx_D1D2);
// 	      dist_D2D3 = MT_Distance_of_Lines(&line_pion,&line_al,vtx_D2D3);
// 	      dist_D1D3 = MT_Distance_of_Lines(&line_proton,&line_al,vtx_D1D3);



// 	      dist_vtxD1D2_vtxD2D3 = TMath::Sqrt((vtx_D1D2[0]-vtx_D2D3[0])*(vtx_D1D2[0]-vtx_D2D3[0])+
// 						 (vtx_D1D2[1]-vtx_D2D3[1])*(vtx_D1D2[1]-vtx_D2D3[1])+
// 						 (vtx_D1D2[2]-vtx_D2D3[2])*(vtx_D1D2[2]-vtx_D2D3[2]));
		  
// 	      dist_vtxD1D2_vtxD1D3 = TMath::Sqrt((vtx_D1D2[0]-vtx_D1D3[0])*(vtx_D1D2[0]-vtx_D1D3[0])+
// 						 (vtx_D1D2[1]-vtx_D1D3[1])*(vtx_D1D2[1]-vtx_D1D3[1])+
// 						 (vtx_D1D2[2]-vtx_D1D3[2])*(vtx_D1D2[2]-vtx_D1D3[2]));
		  
// 	      dist_vtxD2D3_vtxD1D3 = TMath::Sqrt((vtx_D2D3[0]-vtx_D1D3[0])*(vtx_D2D3[0]-vtx_D1D3[0])+
// 						 (vtx_D2D3[1]-vtx_D1D3[1])*(vtx_D2D3[1]-vtx_D1D3[1])+
// 						 (vtx_D2D3[2]-vtx_D1D3[2])*(vtx_D2D3[2]-vtx_D1D3[2]));
		  
		  
// 	      double semi_per = 0.5*(dist_vtxD1D2_vtxD2D3+dist_vtxD1D2_vtxD1D3+dist_vtxD2D3_vtxD1D3);
// 	      double area_vtx = TMath::Sqrt(semi_per*(semi_per-dist_vtxD1D2_vtxD2D3)*(semi_per-dist_vtxD1D2_vtxD1D3)*(semi_per-dist_vtxD2D3_vtxD1D3));
		  
// 	      double vtx_Centroid[3]={vtx_D1D2[0]+vtx_D2D3[0]+vtx_D1D3[0],vtx_D1D2[1]+vtx_D2D3[1]+vtx_D1D3[1],vtx_D1D2[2]+vtx_D2D3[2]+vtx_D1D3[2]};
// 	      vtx_Centroid[0]/=3.;vtx_Centroid[1]/=3.;vtx_Centroid[2]/=3.;


// 	      double mom_pi[3],mom_proton[3],mom_al[3];
// 	      mom_proton[0] = ProtonTrack[pr].X();
// 	      mom_proton[1] = ProtonTrack[pr].Y();
// 	      mom_proton[2] = ProtonTrack[pr].Z();

// 	      mom_al[0] = AlphaTrack[al].X();
// 	      mom_al[1] = AlphaTrack[al].Y();
// 	      mom_al[2] = AlphaTrack[al].Z();

// 	      mom_pi[0] = PionTrack[pi].X();
// 	      mom_pi[1] = PionTrack[pi].Y();
// 	      mom_pi[2] = PionTrack[pi].Z();

// 	      double Area_cut[2];
// 	      Area_cut[0]=0.02;
// 	      Area_cut[1]=6.;

// 	      //		  if (area_vtx < Area_cut[0] && vtx_Centroid[2]>Area_cut[1]) 
// 	      {
		      
// 		double InvMass_a4pp = MT_InvariantMass(M_pi,mom_pi,M_alpha4,mom_al,M_proton,mom_proton);

// 		//double InvMass_L = MT_InvariantMass(M_pi, mom_pi,
// 		//	M_alpha4, mom_proton);

// 		h_newinv_secz_a4pp->Fill( vtx_Centroid[2]);
// 		h_newinv_dist_a4pp->Fill(area_vtx);
		      
// 		//cout<<en<<" come here  3 "<<InvMass_a4pp<<" "<<area_vtx<<" "<<vtx_Centroid[2]<<endl;
// 		double comp_chi =AlphaTrackChi[al]+PionTrackChi[pi]+area_vtx;
		      
		      
// 		//if(vtx_Centroid[2]>60 && vtx_Centroid[2]<390 &&area_vtx<5)
// 		if(comp_chi<temp_best_chi_a4pp)
// 		  {
// 		    temp_best_chi_a4pp = comp_chi;
// 		    best_mass_a4pp = InvMass_a4pp;
// 		    best_secz_a4pp =  vtx_Centroid[2];
// 		    best_dist_a4pp = area_vtx;
// 		  }
		      
// 	      }
		  
// 	    }
	      
// 	}



//     if(best_mass_a4pp>0)
//       {
// 	if(best_secz_a4pp<390 &&best_secz_a4pp>60&&best_dist_a4pp<5)
// 	  h_newinv_mass_a4pp->Fill(best_mass_a4pp);
// 	h_newinv_mass_secz_a4pp->Fill(best_mass_a4pp,best_secz_a4pp);
// 	h_newinv_mass_dist_a4pp->Fill(best_mass_a4pp,best_dist_a4pp);
// 	cout<<"He5L  ========================== "<<best_mass_a4pp<<" Z="<<best_secz_a4pp<<" dist="<<best_dist_a4pp<<endl;
//       }

//   }
  
// #ifdef DEBUG_DETAIL
//   cout<<funcname<<" line "<<__LINE__<<endl;
// #endif
//}




// double Friend::GetRho(double x_TR1, double z_TR1,
// 		      double x_TR2, double z_TR2,
// 		      double x_TOF, double z_TOF,
// 		      int Direction,double *pos_ent,double *pos_exi,
// 		      double *center)
// {
//   //Direction==1 : positive charged particle--> center[0] should be  negative
//   //Direction==0 : negative charged particle--> center[0] should be  positive


//   double a = (z_TR1-z_TR2)/(x_TR1-x_TR2+0.000000001);
//   double b = -a*x_TR1 + z_TR1;

//   //  cout << "a and b " << a << " " << b << endl;

//   double pos_TR1[2],pos_TR2[2];
//   pos_TR1[0] = x_TR1;
//   pos_TR1[1] = z_TR1;
//   pos_TR2[0] = x_TR2;
//   pos_TR2[1] = z_TR2;
//   double vec_in[2];
//   vec_in[0] = pos_TR2[0] - pos_TR1[0];
//   vec_in[1] = pos_TR2[1] - pos_TR1[1];


//   double pos_magnet_corner1[2],pos_magnet_corner2[2],pos_magnet_corner3[2],pos_magnet_corner4[2];
//   pos_magnet_corner1[0] = corner1_angle[0];
//   pos_magnet_corner1[1] = corner1_angle[2];
//   pos_magnet_corner2[0] = corner2_angle[0];
//   pos_magnet_corner2[1] = corner2_angle[2];
//   pos_magnet_corner3[0] = corner3_angle[0];
//   pos_magnet_corner3[1] = corner3_angle[2];
//   pos_magnet_corner4[0] = corner4_angle[0];
//   pos_magnet_corner4[1] = corner4_angle[2];
//   double vec_magnet_in[2], vec_magnet_out[2];
//   vec_magnet_in[0] = pos_magnet_corner2[0] - pos_magnet_corner1[0];
//   vec_magnet_in[1] = pos_magnet_corner2[1] - pos_magnet_corner1[1];
//   vec_magnet_out[0] = pos_magnet_corner4[0] - pos_magnet_corner3[0];
//   vec_magnet_out[1] = pos_magnet_corner4[1] - pos_magnet_corner3[1];

//   double pos_1[2];

//   MT_Intersection_of_Lines_2D(pos_TR2,vec_in,pos_magnet_corner1,vec_magnet_in,pos_1);


//   double pos_2[2];

//   MT_Intersection_of_Lines_2D(pos_TR2,vec_in,pos_magnet_corner3,vec_magnet_out,pos_2);


//   //  cout << pos_1[0] << " " << pos_1[1] << " " << pos_2[0] << " " << pos_2[1] << endl; 

//   //  h_int_ent ->Fill(pos_1[0],pos_1[1]);

//   double pos_0[2];
//   pos_0[0] = x_TOF;
//   pos_0[1] = z_TOF;

//   //  cout << pos_0[0] << " " << pos_0[1] << endl;

  
//   double pos_3_selected[2];
//   double vec_03_selected[2];


//   double length1 =0.0;
//   double length2=0.0;
    
//   double min = 999999999999999.9;

//   //  cout << pos_1[1] << " " << pos_2[1] << endl;

//   //  int interval = (pos_2[1]-pos_1[1])/1.0;
//   int interval = (int)( (pos_2[1]-pos_1[1])/10 );

//   //  cout << "interval = " << interval << endl;
//   double pos_3[2]={0.0,0.0};
//   for(int i=0;i<interval;i++)
//     {
//       double t = pos_1[1]+ i*10;
//       double pos_t[2];
//       pos_t[0] = (t-b)/a;
//       pos_t[1] = t;

//       //      cout << pos_t[0] << " " << pos_t[1] << endl;


//       double vec_t0[2];
//       vec_t0[0] = pos_0[0] - pos_t[0];
//       vec_t0[1] = pos_0[1] - pos_t[1]; 


      
//       MT_Intersection_of_Lines_2D(pos_0,vec_t0,pos_magnet_corner3,vec_magnet_out,pos_3);

//       //      cout << pos_3[0] << " " << pos_3[1] << endl;

//       length1 = (pos_1[0]-pos_t[0])*(pos_1[0]-pos_t[0]) + (pos_1[1]-pos_t[1])*(pos_1[1]-pos_t[1]);
//       length2 = (pos_3[0]-pos_t[0])*(pos_3[0]-pos_t[0]) + (pos_3[1]-pos_t[1])*(pos_3[1]-pos_t[1]);

//       //cout << sqrt(length1) << " " << sqrt(length2) << endl;

//       double test = fabs(sqrt(length1) - sqrt(length2));

//       //      if(pos_t[1]<pos_1[1] || pos_t[1] > pos_2[1]) continue;

//       if(test<min)
// 	{
// 	  min = test;
// 	  pos_3_selected[0] = pos_3[0];
// 	  pos_3_selected[1] = pos_3[1];
// 	  vec_03_selected[0] = pos_3[0] - pos_0[0];
// 	  vec_03_selected[1] = pos_3[1] - pos_0[1];
// 	}
//     }

//   //  double center[2];

//   if(MT_Center_of_Incircle(pos_1,vec_in,pos_3_selected,vec_03_selected,center)==1)
//     {

// //       h_int_ent ->Fill(pos_1[0],pos_1[1]);
// //       h_int_ent ->Fill(pos_3_selected[0],pos_3_selected[1]);
// //       h_int_ent ->Fill(vec_03_selected[0],vec_03_selected[1]);


//       //      cout << "pos_3 (x,z) = " << pos_3_selected[0] << " " << pos_3_selected[1] << endl;
//       //      cout << "center (x,z) = " << center[0] << " " << center[1] << endl;
//       double rho = sqrt((pos_1[0]-center[0])*(pos_1[0]-center[0]) + 
// 			(pos_1[1]-center[1])*(pos_1[1]-center[1]));
// //       double rho2 = sqrt((pos_3_selected[0]-center[0])*(pos_3_selected[0]-center[0]) + 
// // 			 (pos_3_selected[1]-center[1])*(pos_3_selected[1]-center[1]));

//       //      cout<<"rho="<<rho<<" rho2="<<rho2<<"dis="<<rho-rho2<<endl;
//       //      cout << " ******** Test for GetRho : " << rho << " " << min << endl;
//       //      if(center[1]<0)return -1;



//       for(int pp=0;pp<2;pp++)
// 	{
// 	  pos_ent[pp]=pos_1[pp];
// 	  pos_exi[pp]=pos_3_selected[pp];
// 	}

// //         if(pos_exi[0]>corner3_angle[0] ||
// //   	 pos_exi[0]<corner4_angle[0])return -1;


//        if(Direction==1)
//  	{
//  	  if(center[0]>0)return -1;
//  	}
//        else if(Direction==0)
//  	{
// 	  //	  cout<<center[0]<<endl;
//  	  if(center[0]<0)//return -1;
// 	    {
// 	      rho = (-1.0)*rho;
// 	    }
//  	}

      


// //       if(pos_exi[1]>2600 && pos_exi[1]<2800 )
// // 	{
// // 	  cout<<"kkkkkkkkkkkkkkkkkkKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK"<<endl;
// // 	}
// //       if(center[1]<0)
// //  	{
// //  	  cout <<"length 1 ="<<length1 << " length 2=" << length2 << endl;
// //  	  cout << "pos_3 ("<<pos_3[0] << ", " << pos_3[1] <<")"<< endl;
// //  	  cout << "interval = " << interval << endl;
// //  	  cout << "pos_1 (x,z) = " << pos_1[0] << " " << pos_1[1] << endl;
// //  	  cout << "min = " << min  << endl;
// //  	  cout << "pos_3 (x,z) = " << pos_3_selected[0] << " " << pos_3_selected[1] << endl;
// //  	  cout << "center (x,z) = " << center[0] << " " << center[1] << endl;
// //  	  cout << " ******** Test for GetRho : " << rho << " " << min << endl;
// //  	}

//       return rho;
      

//     }

//   return -99999;

// }





double Friend::GetRho(double x_TR1, double z_TR1,
		      double x_TR2, double z_TR2,
		      double x_TOF1, double z_TOF1,
		      double x_TOF2, double z_TOF2,
		      int Direction,double *pos_ent,double *pos_exi,
		      double *center)
  {
  //Direction==1 : positive charged particle--> center[0] should be  negative
  //Direction==0 : negative charged particle--> center[0] should be  positive


//    double a = (z_TR1-z_TR2)/(x_TR1-x_TR2+0.000000001);
//    double b = -a*x_TR1 + z_TR1;

  //  cout << "a and b " << a << " " << b << endl;

  double pos_TR1[2],pos_TR2[2];
  pos_TR1[0] = x_TR1;
  pos_TR1[1] = z_TR1;
  pos_TR2[0] = x_TR2;
  pos_TR2[1] = z_TR2;

  double pos_TOF1[2],pos_TOF2[2];
  pos_TOF1[0] = x_TOF1;
  pos_TOF1[1] = z_TOF1;

  pos_TOF2[0] = x_TOF2;
  pos_TOF2[1] = z_TOF2;


  double vec_in[2];
  vec_in[0] = pos_TR2[0] - pos_TR1[0];
  vec_in[1] = pos_TR2[1] - pos_TR1[1];

  double vec_out[2];
  vec_out[0] = pos_TOF2[0] - pos_TOF1[0];
  vec_out[1] = pos_TOF2[1] - pos_TOF1[1];


//   double pos_magnet_corner1[2],pos_magnet_corner2[2],pos_magnet_corner3[2],pos_magnet_corner4[2];
//   pos_magnet_corner1[0] = corner1_angle[0];
//   pos_magnet_corner1[1] = corner1_angle[2];
//   pos_magnet_corner2[0] = corner2_angle[0];
//   pos_magnet_corner2[1] = corner2_angle[2];

//   pos_magnet_corner3[0] = corner3_angle[0];
//   pos_magnet_corner3[1] = corner3_angle[2];
//   pos_magnet_corner4[0] = corner4_angle[0];
//   pos_magnet_corner4[1] = corner4_angle[2];
//   double vec_magnet_in[2], vec_magnet_out[2];
//   vec_magnet_in[0] = pos_magnet_corner2[0] - pos_magnet_corner1[0];
//   vec_magnet_in[1] = pos_magnet_corner2[1] - pos_magnet_corner1[1];
//   vec_magnet_out[0] = pos_magnet_corner4[0] - pos_magnet_corner3[0];
//   vec_magnet_out[1] = pos_magnet_corner4[1] - pos_magnet_corner3[1];

  double pos_1[2];
  MT_Intersection_of_Lines_2D(pos_TR2,vec_in,pos_magnet_corner1,vec_magnet_in,pos_1);


  double pos_2[2];
  MT_Intersection_of_Lines_2D(pos_TOF1,vec_out,pos_magnet_corner3,vec_magnet_out,pos_2);


  //  cout << pos_1[0] << " " << pos_1[1] << " " << pos_2[0] << " " << pos_2[1] << endl; 

  //  h_int_ent ->Fill(pos_1[0],pos_1[1]);

//   double pos_0[2];
//   pos_0[0] = x_TOF;
//   pos_0[1] = z_TOF;

//   //  cout << pos_0[0] << " " << pos_0[1] << endl;

  
//   double pos_3_selected[2];
//   double vec_03_selected[2];


//   double length1 =0.0;
//   double length2=0.0;
    
//   double min = 999999999999999.9;

//   //  cout << pos_1[1] << " " << pos_2[1] << endl;

//   //  int interval = (pos_2[1]-pos_1[1])/1.0;
//   int interval = (int)( (pos_2[1]-pos_1[1])/10 );

//   //  cout << "interval = " << interval << endl;
//   double pos_3[2]={0.0,0.0};
//   for(int i=0;i<interval;i++)
//     {
//       double t = pos_1[1]+ i*10;
//       double pos_t[2];
//       pos_t[0] = (t-b)/a;
//       pos_t[1] = t;

//       //      cout << pos_t[0] << " " << pos_t[1] << endl;


//       double vec_t0[2];
//       vec_t0[0] = pos_0[0] - pos_t[0];
//       vec_t0[1] = pos_0[1] - pos_t[1]; 


      
//       MT_Intersection_of_Lines_2D(pos_0,vec_t0,pos_magnet_corner3,vec_magnet_out,pos_3);

//       //      cout << pos_3[0] << " " << pos_3[1] << endl;

//       length1 = (pos_1[0]-pos_t[0])*(pos_1[0]-pos_t[0]) + (pos_1[1]-pos_t[1])*(pos_1[1]-pos_t[1]);
//       length2 = (pos_3[0]-pos_t[0])*(pos_3[0]-pos_t[0]) + (pos_3[1]-pos_t[1])*(pos_3[1]-pos_t[1]);

//       //      cout << sqrt(length1) << " " << sqrt(length2) << endl;

//       double test = fabs(sqrt(length1) - sqrt(length2));

//       //      if(pos_t[1]<pos_1[1] || pos_t[1] > pos_2[1]) continue;

//       if(test<min)
// 	{
// 	  min = test;
// 	  pos_3_selected[0] = pos_3[0];
// 	  pos_3_selected[1] = pos_3[1];
// 	  vec_03_selected[0] = pos_3[0] - pos_0[0];
// 	  vec_03_selected[1] = pos_3[1] - pos_0[1];
// 	}
//     }

//   //  double center[2];

  if(MT_Center_of_Incircle(pos_1,vec_in,pos_2,vec_out,center)==1)
    {

//       h_int_ent ->Fill(pos_1[0],pos_1[1]);
//       h_int_ent ->Fill(pos_3_selected[0],pos_3_selected[1]);
//       h_int_ent ->Fill(vec_03_selected[0],vec_03_selected[1]);


      //      cout << "pos_3 (x,z) = " << pos_3_selected[0] << " " << pos_3_selected[1] << endl;
      //      cout << "center (x,z) = " << center[0] << " " << center[1] << endl;
      double rho = sqrt((pos_1[0]-center[0])*(pos_1[0]-center[0]) + 
			(pos_1[1]-center[1])*(pos_1[1]-center[1]));
      double rho2 = sqrt((pos_2[0]-center[0])*(pos_2[0]-center[0]) + 
			 (pos_2[1]-center[1])*(pos_2[1]-center[1]));

//       h_inv_rho1_rho2->Fill(rho/1000.0 ,rho2/1000.0);
//       h_inv_rho1_rho2_diff->Fill(rho/1000.0-rho2/1000.0);
      //      cout<<rho<<" - "<<rho2<<endl;

      if(fabs(rho-rho2)>200)return -1;

      double rho_mean = (rho+rho2)/2;
      //      cout<<"rho="<<rho<<" rho2="<<rho2<<"dis="<<rho-rho2<<endl;
      //      cout << " ******** Test for GetRho : " << rho << " " << min << endl;
      //      if(center[1]<0)return -1;

       if(Direction==1)
 	{
 	  if(center[0]>0)return -1;
 	}
       else if(Direction==0)
 	{
 	  if(center[0]<0)return -1;
 	}

      for(int pp=0;pp<2;pp++)
	{
	  pos_ent[pp]=pos_1[pp];
	  pos_exi[pp]=pos_2[pp];
	}



//       if(pos_exi[1]>2600 && pos_exi[1]<2800 )
// 	{
// 	  cout<<"kkkkkkkkkkkkkkkkkkKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK"<<endl;
// 	}
//       if(center[1]<0)
//  	{
//  	  cout <<"length 1 ="<<length1 << " length 2=" << length2 << endl;
//  	  cout << "pos_3 ("<<pos_3[0] << ", " << pos_3[1] <<")"<< endl;
//  	  cout << "interval = " << interval << endl;
//  	  cout << "pos_1 (x,z) = " << pos_1[0] << " " << pos_1[1] << endl;
//  	  cout << "min = " << min  << endl;
//  	  cout << "pos_3 (x,z) = " << pos_3_selected[0] << " " << pos_3_selected[1] << endl;
//  	  cout << "center (x,z) = " << center[0] << " " << center[1] << endl;
//  	  cout << " ******** Test for GetRho : " << rho << " " << min << endl;
//  	}
      return rho_mean;
      

    }

  return -1;

}



double Friend::Length_TOFs_TR2(double x_TR1, double y_TR1, double z_TR1, double x_TR2, double y_TR2, double z_TR2)
{

  double posxz_TR1[2],posxz_TR2[2];
  posxz_TR1[0] = x_TR1;
  posxz_TR1[1] = z_TR1;
  posxz_TR2[0] = x_TR2;
  posxz_TR2[1] = z_TR2;

  double vecxz_in[2];
  vecxz_in[0] = posxz_TR2[0] - posxz_TR1[0];
  vecxz_in[1] = posxz_TR2[1] - posxz_TR1[1];

  double posxz_ts[2];
  double posxz_TOF_start[2] = {0.0,-50.0};
  double vecxz_TOF_start[2] = {1.0,0.0};
      
  MT_Intersection_of_Lines_2D(posxz_TR2,vecxz_in,posxz_TOF_start,vecxz_TOF_start,posxz_ts);
      
  //cout << pos_ts[0] << " " << pos_ts[1] << endl;
  
      
  double lengthxz_TOFs_TR2 = sqrt((posxz_TR2[0]-posxz_ts[0])*(posxz_TR2[0]-posxz_ts[0]) + (posxz_TR2[1]-posxz_ts[1])*(posxz_TR2[1]-posxz_ts[1]));
  //cout << length_in << endl;





  double posyz_TR1[2],posyz_TR2[2];
  posyz_TR1[0] = y_TR1;
  posyz_TR1[1] = z_TR1;
  posyz_TR2[0] = y_TR2;
  posyz_TR2[1] = z_TR2;

  double vecyz_in[2];
  vecyz_in[0] = posyz_TR2[0] - posyz_TR1[0];
  vecyz_in[1] = posyz_TR2[1] - posyz_TR1[1];

  double posyz_ts[2];
  double posyz_TOF_start[2] = {0.0,-50.0};
  double vecyz_TOF_start[2] = {1.0,0.0};
      
  MT_Intersection_of_Lines_2D(posyz_TR2,vecyz_in,posyz_TOF_start,vecyz_TOF_start,posyz_ts);
      
  //cout << pos_ts[0] << " " << pos_ts[1] << endl;
  
      
  double lengthy_TOFs_TR2 = sqrt((posyz_TR2[0]-posyz_ts[0])*(posyz_TR2[0]-posyz_ts[0]));
  //cout << length_in << endl;




  double length = sqrt(lengthxz_TOFs_TR2*lengthxz_TOFs_TR2 + lengthy_TOFs_TR2*lengthy_TOFs_TR2);

  return length;

}



Double_t Friend::GetRho(int sign, double x_TR1, double z_TR1, double x_TR2, double z_TR2,
			double x_TOF, double y_TOF, double z_TOF, double &length_xyz,
			double *pos_ent,double *pos_exi,double *center)
{

  double a = (z_TR1-z_TR2)/(x_TR1-x_TR2+0.000000001);
  double b = -a*x_TR1 + z_TR1;

  //  cout << "a and b " << a << " " << b << endl;

  double pos_TR1[2],pos_TR2[2];
  pos_TR1[0] = x_TR1;
  pos_TR1[1] = z_TR1;
  pos_TR2[0] = x_TR2;
  pos_TR2[1] = z_TR2;
  double vec_in[2];
  vec_in[0] = pos_TR2[0] - pos_TR1[0];
  vec_in[1] = pos_TR2[1] - pos_TR1[1];


//   double pos_magnet_corner1[2],pos_magnet_corner2[2],pos_magnet_corner3[2],pos_magnet_corner4[2];
//   pos_magnet_corner1[0] = corner1_angle[0];
//   pos_magnet_corner1[1] = corner1_angle[2];
//   pos_magnet_corner2[0] = corner2_angle[0];
//   pos_magnet_corner2[1] = corner2_angle[2];
//   pos_magnet_corner3[0] = corner3_angle[0];
//   pos_magnet_corner3[1] = corner3_angle[2];
//   pos_magnet_corner4[0] = corner4_angle[0];
//   pos_magnet_corner4[1] = corner4_angle[2];
//   double vec_magnet_in[2], vec_magnet_out[2];
//   vec_magnet_in[0] = pos_magnet_corner2[0] - pos_magnet_corner1[0];
//   vec_magnet_in[1] = pos_magnet_corner2[1] - pos_magnet_corner1[1];
//   vec_magnet_out[0] = pos_magnet_corner4[0] - pos_magnet_corner3[0];
//   vec_magnet_out[1] = pos_magnet_corner4[1] - pos_magnet_corner3[1];

  double pos_1[2];

  MT_Intersection_of_Lines_2D(pos_TR2,vec_in,pos_magnet_corner1,vec_magnet_in,pos_1);


  double pos_2[2];

  MT_Intersection_of_Lines_2D(pos_TR2,vec_in,pos_magnet_corner3,vec_magnet_out,pos_2);







  //  cout << pos_1[0] << " " << pos_1[1] << " " << pos_2[0] << " " << pos_2[1] << endl; 



  double pos_0[2];
  pos_0[0] = x_TOF;
  pos_0[1] = z_TOF;


  //  cout << pos_0[0] << " " << pos_0[1] << endl;

  
  double pos_3_selected[2];
  double vec_03_selected[2];
    
  double min = 999999999999999.9;
  double min2 = 999999999999999.9;

  double temp1=0;
  double temp2=0;
  //  cout << pos_1[1] << " " << pos_2[1] << endl;

  //  int interval = int((pos_2[1]-pos_1[1])/1.0);

  //  cout << "interval = " << interval << endl;

  double x_exit, z_exit;


  //  for(int i=0;i<interval;i++)
  for(int i=730;i<810;i++)
    {
      double t = pos_1[1]+ i*1.0;
      double pos_t[2];
      pos_t[0] = (t-b)/a;
      pos_t[1] = t;

      //      cout << pos_t[0] << " " << pos_t[1] << endl;


      double vec_t0[2];
      vec_t0[0] = pos_0[0] - pos_t[0];
      vec_t0[1] = pos_0[1] - pos_t[1]; 

      double pos_3[2];
      
      MT_Intersection_of_Lines_2D(pos_0,vec_t0,pos_magnet_corner3,vec_magnet_out,pos_3);

      //      cout << pos_3[0] << " " << pos_3[1] << endl;

      double length1 = (pos_1[0]-pos_t[0])*(pos_1[0]-pos_t[0]) + (pos_1[1]-pos_t[1])*(pos_1[1]-pos_t[1]);
      double length2 = (pos_3[0]-pos_t[0])*(pos_3[0]-pos_t[0]) + (pos_3[1]-pos_t[1])*(pos_3[1]-pos_t[1]);

      //      cout << sqrt(length1) << " " << sqrt(length2) << endl;

      double test = fabs(sqrt(length1) - sqrt(length2));
      
      double test2 = fabs(sqrt(length1)/sqrt(length2)-1.0);
      //      cout<<test2<<endl;

      //      if(pos_t[1]<pos_1[1] || pos_t[1] > pos_2[1]) continue;

      if(test<min)
	{
	  min = test;
	  pos_3_selected[0] = pos_3[0];
	  pos_3_selected[1] = pos_3[1];
	  vec_03_selected[0] = pos_3[0] - pos_0[0];
	  vec_03_selected[1] = pos_3[1] - pos_0[1];

	  x_exit = pos_3[0];
	  z_exit = pos_3[1];
	  temp1=i;
	}

      if(test2<min2)
	{
	  min2 = test2;
// 	  pos_3_selected[0] = pos_3[0];
// 	  pos_3_selected[1] = pos_3[1];
// 	  vec_03_selected[0] = pos_3[0] - pos_0[0];
// 	  vec_03_selected[1] = pos_3[1] - pos_0[1];

// 	  x_exit = pos_3[0];
// 	  z_exit = pos_3[1];
	  temp2=i;
	}
    }
//   if(temp1!=temp2)
//     cout<<"******************************************************"<<temp1<<" "<<temp2<<endl;
  

//   if(temp1<759 )
//      cout<<"******************************************************"<<temp1<<" "<<temp2<<endl;
  
//   if(temp1>785 )
//     cout<<"*************************************************"<<temp1<<" "<<temp2<<endl;


//   if(x_exit>pos_magnet_corner3[0] || x_exit<pos_magnet_corner4[0]) return -1;


  double vec_magnet_ep[2], vec_magnet_em[2];
  vec_magnet_ep[0] = pos_magnet_corner3[0] - pos_magnet_corner1[0];
  vec_magnet_ep[1] = pos_magnet_corner3[1] - pos_magnet_corner1[1];
  vec_magnet_em[0] = pos_magnet_corner4[0] - pos_magnet_corner2[0];
  vec_magnet_em[1] = pos_magnet_corner4[1] - pos_magnet_corner2[1];


  double aa_ep =  vec_magnet_ep[0]/ vec_magnet_ep[1];
  double bb_ep =  pos_magnet_corner3[0]-aa_ep*pos_magnet_corner3[1];

  double aa_em =  vec_magnet_em[0]/ vec_magnet_em[1];
  double bb_em =  pos_magnet_corner4[0]-aa_em*pos_magnet_corner4[1];








  //  double center[2];

  if(MT_Center_of_Incircle(pos_1,vec_in,pos_3_selected,vec_03_selected,center)==1)
    {

      if(sign==0) return -1;
      if(sign>0) 
	{
	  if(center[0]>0.0) return -1;

	}
      if(sign<0)
	{
	  if(center[0]<0.0) return -1;
	}

       for(int pp=0;pp<2;pp++)
 	{
 	  pos_ent[pp]=pos_1[pp];
 	  pos_exi[pp]=pos_3_selected[pp];
 	}


      //      cout << "pos_3 (x,z) = " << pos_3_selected[0] << " " << pos_3_selected[1] << endl;
      //      cout << "center (x,z) = " << center[0] << " " << center[1] << endl;
      double rho = sqrt((pos_1[0]-center[0])*(pos_1[0]-center[0]) + (pos_1[1]-center[1])*(pos_1[1]-center[1]));
      //      cout << " ******** Test for GetRho : " << rho << " " << min << endl;

//       double rho2 = sqrt((pos_3_selected[0]-center[0])*(pos_3_selected[0]-center[0]) + 
// 			 (pos_3_selected[1]-center[1])*(pos_3_selected[1]-center[1]));

      //      cout<<rho<<" === "<<rho2<<endl;


      double dist_ep = fabs(center[0]-aa_ep*center[1]-bb_ep)/sqrt(1+aa_ep*aa_ep);
      double dist_em = fabs(center[0]-aa_em*center[1]-bb_em)/sqrt(1+aa_em*aa_em);

      if(sign>0) 
	{
	  if(rho>dist_ep) return -1;

	}
      if(sign<0)
	{
	  if(rho>dist_em) return -1;
	}


      double pos_ts[2];
      double pos_TOF_start[2] = {0.0,-50.0};
      double vec_TOF_start[2] = {1.0,0.0};
      
      MT_Intersection_of_Lines_2D(pos_TR2,vec_in,pos_TOF_start,vec_TOF_start,pos_ts);
      
      //cout << pos_ts[0] << " " << pos_ts[1] << endl;
      
      
      double length_in = sqrt((pos_1[0]-pos_ts[0])*(pos_1[0]-pos_ts[0]) + (pos_1[1]-pos_ts[1])*(pos_1[1]-pos_ts[1]));
      //cout << length_in << endl;
      double length_out = sqrt((pos_0[0]-pos_3_selected[0])*(pos_0[0]-pos_3_selected[0]) + (pos_0[1]-pos_3_selected[1])*(pos_0[1]-pos_3_selected[1]));
      //      cout << length_out << endl;

      double vec_1[2],vec_2[2];
      vec_1[0] = pos_1[0] - center[0];
      vec_1[1] = pos_1[1] - center[1];
      vec_2[0] = pos_3_selected[0] - center[0];
      vec_2[1] = pos_3_selected[1] - center[1];
      double costheta = (vec_1[0]*vec_2[0]+vec_1[1]*vec_2[1])/sqrt(vec_1[0]*vec_1[0]+vec_1[1]*vec_1[1])/sqrt(vec_2[0]*vec_2[0]+vec_2[1]*vec_2[1]);
      double theta = acos(costheta);
      double length_magnet = rho * theta;
      //      cout << length_magnet << endl;
      double length_xz = length_in + length_magnet + length_out;
      
      length_xyz = sqrt(length_xz*length_xz + y_TOF*y_TOF);



      return rho;


    }

  return -1;

}

Double_t Friend::GetRho(int sign, double x_TR1, double z_TR1, double x_TR2, double z_TR2,
			double x_TOF, double y_TOF, double z_TOF, double &length_xyz,
			double *pos_ent,double *pos_exi,double *center,double z_Bg)
{

  double a = (z_TR1-z_TR2)/(x_TR1-x_TR2+0.000000001);
  double b = -a*x_TR1 + z_TR1;

  //  cout << "a and b " << a << " " << b << endl;

  double pos_TR1[2],pos_TR2[2];
  pos_TR1[0] = x_TR1;
  pos_TR1[1] = z_TR1;
  pos_TR2[0] = x_TR2;
  pos_TR2[1] = z_TR2;
  double vec_in[2];
  vec_in[0] = pos_TR2[0] - pos_TR1[0];
  vec_in[1] = pos_TR2[1] - pos_TR1[1];


//   double pos_magnet_corner1[2],pos_magnet_corner2[2],pos_magnet_corner3[2],pos_magnet_corner4[2];
//   pos_magnet_corner1[0] = corner1_angle[0];
//   pos_magnet_corner1[1] = corner1_angle[2];
//   pos_magnet_corner2[0] = corner2_angle[0];
//   pos_magnet_corner2[1] = corner2_angle[2];
//   pos_magnet_corner3[0] = corner3_angle[0];
//   pos_magnet_corner3[1] = corner3_angle[2];
//   pos_magnet_corner4[0] = corner4_angle[0];
//   pos_magnet_corner4[1] = corner4_angle[2];
//   double vec_magnet_in[2], vec_magnet_out[2];
//   vec_magnet_in[0] = pos_magnet_corner2[0] - pos_magnet_corner1[0];
//   vec_magnet_in[1] = pos_magnet_corner2[1] - pos_magnet_corner1[1];
//   vec_magnet_out[0] = pos_magnet_corner4[0] - pos_magnet_corner3[0];
//   vec_magnet_out[1] = pos_magnet_corner4[1] - pos_magnet_corner3[1];

  double pos_1[2];

  MT_Intersection_of_Lines_2D(pos_TR2,vec_in,pos_magnet_corner1,vec_magnet_in,pos_1);


  double pos_2[2];

  MT_Intersection_of_Lines_2D(pos_TR2,vec_in,pos_magnet_corner3,vec_magnet_out,pos_2);







  //  cout << pos_1[0] << " " << pos_1[1] << " " << pos_2[0] << " " << pos_2[1] << endl; 



  double pos_0[2];
  pos_0[0] = x_TOF;
  pos_0[1] = z_TOF;


  //  cout << pos_0[0] << " " << pos_0[1] << endl;

  
  double pos_3_selected[2];
  double vec_03_selected[2];
    
  double min = 999999999999999.9;
  double min2 = 999999999999999.9;

  double temp1=0;
  double temp2=0;
  //  cout << pos_1[1] << " " << pos_2[1] << endl;

  //  int interval = int((pos_2[1]-pos_1[1])/1.0);

  //  cout << "interval = " << interval << endl;

  double x_exit, z_exit;


  //  for(int i=0;i<interval;i++)
  for(int i=730;i<810;i++)
    {
      double t = pos_1[1]+ i*1.0;
      double pos_t[2];
      pos_t[0] = (t-b)/a;
      pos_t[1] = t;

      //      cout << pos_t[0] << " " << pos_t[1] << endl;


      double vec_t0[2];
      vec_t0[0] = pos_0[0] - pos_t[0];
      vec_t0[1] = pos_0[1] - pos_t[1]; 

      double pos_3[2];
      
      MT_Intersection_of_Lines_2D(pos_0,vec_t0,pos_magnet_corner3,vec_magnet_out,pos_3);

      //      cout << pos_3[0] << " " << pos_3[1] << endl;

      double length1 = (pos_1[0]-pos_t[0])*(pos_1[0]-pos_t[0]) + (pos_1[1]-pos_t[1])*(pos_1[1]-pos_t[1]);
      double length2 = (pos_3[0]-pos_t[0])*(pos_3[0]-pos_t[0]) + (pos_3[1]-pos_t[1])*(pos_3[1]-pos_t[1]);

      //      cout << sqrt(length1) << " " << sqrt(length2) << endl;

      double test = fabs(sqrt(length1) - sqrt(length2));
      
      double test2 = fabs(sqrt(length1)/sqrt(length2)-1.0);
      //      cout<<test2<<endl;

      //      if(pos_t[1]<pos_1[1] || pos_t[1] > pos_2[1]) continue;

      if(test<min)
	{
	  min = test;
	  pos_3_selected[0] = pos_3[0];
	  pos_3_selected[1] = pos_3[1];
	  vec_03_selected[0] = pos_3[0] - pos_0[0];
	  vec_03_selected[1] = pos_3[1] - pos_0[1];

	  x_exit = pos_3[0];
	  z_exit = pos_3[1];
	  temp1=i;
	}

      if(test2<min2)
	{
	  min2 = test2;
// 	  pos_3_selected[0] = pos_3[0];
// 	  pos_3_selected[1] = pos_3[1];
// 	  vec_03_selected[0] = pos_3[0] - pos_0[0];
// 	  vec_03_selected[1] = pos_3[1] - pos_0[1];

// 	  x_exit = pos_3[0];
// 	  z_exit = pos_3[1];
	  temp2=i;
	}
    }
//   if(temp1!=temp2)
//     cout<<"******************************************************"<<temp1<<" "<<temp2<<endl;
  

//   if(temp1<759 )
//      cout<<"******************************************************"<<temp1<<" "<<temp2<<endl;
  
//   if(temp1>785 )
//     cout<<"*************************************************"<<temp1<<" "<<temp2<<endl;


//   if(x_exit>pos_magnet_corner3[0] || x_exit<pos_magnet_corner4[0]) return -1;


  double vec_magnet_ep[2], vec_magnet_em[2];
  vec_magnet_ep[0] = pos_magnet_corner3[0] - pos_magnet_corner1[0];
  vec_magnet_ep[1] = pos_magnet_corner3[1] - pos_magnet_corner1[1];
  vec_magnet_em[0] = pos_magnet_corner4[0] - pos_magnet_corner2[0];
  vec_magnet_em[1] = pos_magnet_corner4[1] - pos_magnet_corner2[1];


  double aa_ep =  vec_magnet_ep[0]/ vec_magnet_ep[1];
  double bb_ep =  pos_magnet_corner3[0]-aa_ep*pos_magnet_corner3[1];

  double aa_em =  vec_magnet_em[0]/ vec_magnet_em[1];
  double bb_em =  pos_magnet_corner4[0]-aa_em*pos_magnet_corner4[1];








  //  double center[2];

  if(MT_Center_of_Incircle(pos_1,vec_in,pos_3_selected,vec_03_selected,center)==1)
    {

      if(sign==0) return -1;
      if(sign>0) 
	{
	  if(center[0]>0.0) return -1;

	}
      if(sign<0)
	{
	  if(center[0]<0.0) return -1;
	}
      for(int pp=0;pp<2;pp++)
 	{
 	  pos_ent[pp]=pos_1[pp];
 	  pos_exi[pp]=pos_3_selected[pp];
 	}



      //      cout << "pos_3 (x,z) = " << pos_3_selected[0] << " " << pos_3_selected[1] << endl;
      //      cout << "center (x,z) = " << center[0] << " " << center[1] << endl;
      double rho = sqrt((pos_1[0]-center[0])*(pos_1[0]-center[0]) + (pos_1[1]-center[1])*(pos_1[1]-center[1]));
      //      cout << " ******** Test for GetRho : " << rho << " " << min << endl;

//       double rho2 = sqrt((pos_3_selected[0]-center[0])*(pos_3_selected[0]-center[0]) + 
// 			 (pos_3_selected[1]-center[1])*(pos_3_selected[1]-center[1]));

      //      cout<<rho<<" === "<<rho2<<endl;


      double dist_ep = fabs(center[0]-aa_ep*center[1]-bb_ep)/sqrt(1+aa_ep*aa_ep);
      double dist_em = fabs(center[0]-aa_em*center[1]-bb_em)/sqrt(1+aa_em*aa_em);

      if(sign>0) 
	{
	  if(rho>dist_ep) return -1;

	}
      if(sign<0)
	{
	  if(rho>dist_em) return -1;
	}


      double pos_ts[2];
      double pos_TOF_start[2] = {0.0,-50.0};
      double vec_TOF_start[2] = {1.0,0.0};

      double pos_bg[2];
      double pos_TOFbg[2] = {0.0,z_Bg};
      double vec_TOFbg[2] = {1.0,0.0};
      
      MT_Intersection_of_Lines_2D(pos_TR2,vec_in,pos_TOF_start,vec_TOF_start,pos_ts);

      MT_Intersection_of_Lines_2D(pos_3_selected,vec_03_selected,pos_TOFbg,vec_TOFbg,pos_bg);
      
      //cout << pos_ts[0] << " " << pos_ts[1] << endl;
      
      
      double length_in = sqrt((pos_1[0]-pos_ts[0])*(pos_1[0]-pos_ts[0]) + (pos_1[1]-pos_ts[1])*(pos_1[1]-pos_ts[1]));
      //cout << length_in << endl;
      //double length_out = sqrt((pos_0[0]-pos_3_selected[0])*(pos_0[0]-pos_3_selected[0]) + (pos_0[1]-pos_3_selected[1])*(pos_0[1]-pos_3_selected[1]));
      double length_out = sqrt((pos_bg[0]-pos_3_selected[0])*(pos_bg[0]-pos_3_selected[0]) + (pos_bg[1]-pos_3_selected[1])*(pos_bg[1]-pos_3_selected[1]));
      //      cout << length_out << endl;

      double vec_1[2],vec_2[2];
      vec_1[0] = pos_1[0] - center[0];
      vec_1[1] = pos_1[1] - center[1];
      vec_2[0] = pos_3_selected[0] - center[0];
      vec_2[1] = pos_3_selected[1] - center[1];
      double costheta = (vec_1[0]*vec_2[0]+vec_1[1]*vec_2[1])/sqrt(vec_1[0]*vec_1[0]+vec_1[1]*vec_1[1])/sqrt(vec_2[0]*vec_2[0]+vec_2[1]*vec_2[1]);
      double theta = acos(costheta);
      double length_magnet = rho * theta;
      //      cout << length_magnet << endl;
      double length_xz = length_in + length_magnet + length_out;
      
      length_xyz = sqrt(length_xz*length_xz + y_TOF*y_TOF);



      return rho;


    }

  return -1;

}


Bool_t Friend::AnalysisStat()
{
  h_status->Fill(1); // all events

  //Fiber Requirement
#ifdef INVMASS

  int n_TR0x_temp = Event_Fiber->n_TR0_x;
  int n_TR0y_temp = Event_Fiber->n_TR0_y;
  
  int n_TR1x_temp = Event_Fiber->n_TR1_x;
  int n_TR1y_temp = Event_Fiber->n_TR1_y;
  
  int n_TR2x_temp = Event_Fiber->n_TR2_x;
  int n_TR2y_temp = Event_Fiber->n_TR2_y;
  

  if(n_TR0x_temp==0 || n_TR0y_temp==0 ||
     n_TR1x_temp<2 || n_TR1y_temp<2 ||
     n_TR2x_temp<2 || n_TR2y_temp<2 )return false;
#endif

//   if(/*n_TR0x_temp>5 || n_TR0y_temp>5 ||*/
//       n_TR1x_temp>8 || n_TR1y_temp>8 ||
//       n_TR2x_temp>8 || n_TR2y_temp>8 )return false;
  
  
  h_status->Fill(2); // after fiber cut

  //  if(Event_Tof->n_TOFPlusBar==0) return false;

  //  if(Event_Tof->n_TOFPlusBar>10) return false;
  //  if(Event_Tof->n_TOFPlusBar>4) return false;

  // 	 Event_Phys->n_TofPlus>2) continue;
  //      if(Event_Tof->n_TOFPlusBar>3)continue;
  h_status->Fill(3); //after tofplus cut
  //  if(Event_Tof->n_TOFStartBar>2) return false;



  
  //  if(Event_Tof->n_TOFPlusPlusBar!=1)return false

  //  if(Event_Phys->n_TofAladin==1)return false;
  
  h_status->Fill(4); // after tof aladin cut

#ifdef INVMASS
  if(Event_Tof->n_TOFPlusPlusBar>4)return false;
  if(Event_Tof->n_TOFPlusPlusBar==0)return false;

#endif

  //  if(Event_Tof->n_TOFPlusPlusBar>4)return false;

  h_status->Fill(5); // after tof plupplus cut


  //       if(n_tofplus==0 || n_aladin ==0 )continue;


  //  if(Event_Chamber->n_CbM==0 ||Event_Chamber->n_CbM>1000 )return false;

  h_status->Fill(6); // after small chamber cut



  //  if(Event_Chamber->n_CaM==0 ||Event_Chamber->n_CaM>1000)return false;

  h_status->Fill(7); // after big chamber cut

  //  if(Event_Tof->n_TOFStartBar!=1) return false;

  h_status->Fill(8); // after big chamber cut
  
  return true;

}      
Bool_t Friend::AnalysisStatMerged()
{
  h_status->Fill(1); // all events

  //Fiber Requirement

//   int n_TR0x_temp = Event_Out->n_Tr0x;
//   int n_TR0y_temp = Event_Out->n_Tr0y;
  
//   int n_TR1x_temp = Event_Out->n_Tr1x;
//   int n_TR1y_temp = Event_Out->n_Tr1y;
  
//   int n_TR2x_temp = Event_Out->n_Tr2x;
//   int n_TR2y_temp = Event_Out->n_Tr2y;
  
// #ifdef INVMASS
//   if(n_TR0x_temp==0 || n_TR0y_temp==0 ||
//      n_TR1x_temp<2 || n_TR1y_temp<2 ||
//      n_TR2x_temp<2 || n_TR2y_temp<2 )return false;
// #endif

//   if(/*n_TR0x_temp>5 || n_TR0y_temp>5 ||*/
//       n_TR1x_temp>8 || n_TR1y_temp>8 ||
//       n_TR2x_temp>8 || n_TR2y_temp>8 )return false;
  
  
  h_status->Fill(2); // after fiber cut

  //  if(Event_Tof->n_TOFPlusBar==0) return false;

  //  if(Event_Tof->n_TOFPlusBar>10) return false;
  //  if(Event_Tof->n_TOFPlusBar>4) return false;

  // 	 Event_Phys->n_TofPlus>2) continue;
  //      if(Event_Tof->n_TOFPlusBar>3)continue;
  h_status->Fill(3); //after tofplus cut
  if(Event_Out->n_TofStart>2) return false;



  
  //  if(Event_Tof->n_TOFPlusPlusBar!=1)return false

  //  if(Event_Phys->n_TofAladin==1)return false;
  
  h_status->Fill(4); // after tof aladin cut

#ifdef INVMASS

  if(Event_Out->n_TofPlusPlus>4)return false;
  if(Event_Out->n_TOFPlusPlus==0)return false;
#endif


  //  if(Event_Tof->n_TOFPlusPlusBar>4)return false;

  h_status->Fill(5); // after tof plupplus cut


  //       if(n_tofplus==0 || n_aladin ==0 )continue;


  //  if(Event_Chamber->n_CbM==0 ||Event_Chamber->n_CbM>1000 )return false;

  h_status->Fill(6); // after small chamber cut



  //  if(Event_Chamber->n_CaM==0 ||Event_Chamber->n_CaM>1000)return false;

  h_status->Fill(7); // after big chamber cut

  //  if(Event_Tof->n_TOFStartBar!=1) return false;

  h_status->Fill(8); // after big chamber cut
  
  return true;

}      

void Friend::NHits()
{

  int n_TR0x_temp = Event_Fiber->n_TR0_x;
  int n_TR0y_temp = Event_Fiber->n_TR0_y;

  int n_TR1x_temp = Event_Fiber->n_TR1_x;
  int n_TR1y_temp = Event_Fiber->n_TR1_y;
  
  int n_TR2x_temp = Event_Fiber->n_TR2_x;
  int n_TR2y_temp = Event_Fiber->n_TR2_y;

  int n_tofstart_temp = Event_Tof->n_TOFStartBar;
  int n_tofplus_temp = Event_Tof->n_TOFPlusBar;
  //  int n_aladin_temp  = Event_Tof->n_TOFAladinBar;
  int n_tofplusplus_temp  = Event_Tof->n_TOFPlusPlusBar;
  
  int n_CbM_x_temp  = Event_Chamber->n_CbM_x;
  int n_CbM_xp_temp = Event_Chamber->n_CbM_xp;
  int n_CbM_u_temp  = Event_Chamber->n_CbM_u;
  int n_CbM_up_temp = Event_Chamber->n_CbM_up;
  int n_CbM_v_temp  = Event_Chamber->n_CbM_v;
  int n_CbM_vp_temp = Event_Chamber->n_CbM_vp;
  
  int n_CbM_temp = Event_Chamber->n_CbM;
  
  int n_CaM_x_temp  = Event_Chamber->n_CaM_x;
  int n_CaM_xp_temp = Event_Chamber->n_CaM_xp;
  int n_CaM_y_temp  = Event_Chamber->n_CaM_y;
  int n_CaM_yp_temp = Event_Chamber->n_CaM_yp;
  int n_CaM_u_temp  = Event_Chamber->n_CaM_u;
  
  int n_CaM_temp = Event_Chamber->n_CaM;

  h_NHits->GetXaxis()->SetBinLabel(1,"TR0x");
  h_NHits->GetXaxis()->SetBinLabel(100,"TR0y");
  h_NHits->GetXaxis()->SetBinLabel(200,"TR1x");
  h_NHits->GetXaxis()->SetBinLabel(500,"TR1y");
  h_NHits->GetXaxis()->SetBinLabel(700,"TR2x");
  h_NHits->GetXaxis()->SetBinLabel(1200,"TR2y");
  
  h_NHits->GetXaxis()->SetBinLabel(1400,"CbM_x");
  h_NHits->GetXaxis()->SetBinLabel(1500,"CbM_xp");
  h_NHits->GetXaxis()->SetBinLabel(1600,"CbM_u");
  h_NHits->GetXaxis()->SetBinLabel(1700,"CbM_up");
  h_NHits->GetXaxis()->SetBinLabel(1800,"CbM_v");
  h_NHits->GetXaxis()->SetBinLabel(1900,"CbM_vp");
  
  h_NHits->GetXaxis()->SetBinLabel(2000,"CaM_x");
  h_NHits->GetXaxis()->SetBinLabel(2200,"CaM_xp");
  h_NHits->GetXaxis()->SetBinLabel(2400,"CaM_y");
  h_NHits->GetXaxis()->SetBinLabel(2500,"CaM_yp");
  h_NHits->GetXaxis()->SetBinLabel(2600,"CaM_u");
  
  h_NHits->GetXaxis()->SetBinLabel(2700,"Tof+");
  //      h_NHits->GetXaxis()->SetBinLabel(2800,"Aladin");
  h_NHits->GetXaxis()->SetBinLabel(2800,"Tofplusplus");
  
  h_NHits->GetXaxis()->SetBinLabel(3000," CbM");
  
  h_NHits->GetXaxis()->SetBinLabel(3300," CaM");
  h_NHits->GetXaxis()->SetBinLabel(3500," Tof_Start");
  
  
  if(n_TR0x_temp>0)h_NHits->Fill(n_TR0x_temp);
  if(n_TR0y_temp>0)h_NHits->Fill(100+n_TR0y_temp);
  if(n_TR1x_temp>0)h_NHits->Fill(200+n_TR1x_temp);
  if(n_TR1y_temp>0)h_NHits->Fill(500+n_TR1y_temp);
  if(n_TR2x_temp>0)h_NHits->Fill(700+n_TR2x_temp);
  if(n_TR2y_temp>0)h_NHits->Fill(1200+n_TR2y_temp);

  if(n_CbM_x_temp>0) h_NHits->Fill(1400+n_CbM_x_temp);
  if(n_CbM_xp_temp>0)h_NHits->Fill(1500+n_CbM_xp_temp);
  if(n_CbM_u_temp>0) h_NHits->Fill(1600+n_CbM_u_temp);
  if(n_CbM_up_temp>0)h_NHits->Fill(1700+n_CbM_up_temp);
  if(n_CbM_v_temp>0) h_NHits->Fill(1800+n_CbM_v_temp);
  if(n_CbM_vp_temp>0)h_NHits->Fill(1900+n_CbM_vp_temp);
  
  
  if(n_CaM_x_temp>0) h_NHits->Fill(2000+n_CaM_x_temp);
  if(n_CaM_xp_temp>0)h_NHits->Fill(2200+n_CaM_xp_temp);
  if(n_CaM_y_temp>0) h_NHits->Fill(2400+n_CaM_y_temp);
  if(n_CaM_yp_temp>0)h_NHits->Fill(2500+n_CaM_yp_temp);
  if(n_CaM_u_temp>0) h_NHits->Fill(2600+n_CaM_u_temp);

  //      if(n_tofplus>0)
  h_NHits->Fill(2700+n_tofplus_temp);
  //      if(n_aladin>0)
  //      h_NHits->Fill(2800+n_aladin);
  h_NHits->Fill(2800+n_tofplusplus_temp);

  if(n_CbM_temp>0)h_NHits->Fill(3000+n_CbM_temp);
  if(n_CaM_temp>0)h_NHits->Fill(3300+n_CaM_temp);

  h_NHits->Fill(3500+n_tofstart_temp);
  


  
}


Bool_t Friend::GetCoeff(int Det,int id, double *CoeTop, double *CoeBot)
{

  if(Det==0) // TOF Start
    {
      CoeTop[0]=TDCCalib[4][id][0];
      CoeTop[1]=TDCCalib[4][id][1];
      CoeTop[2]=TDCCalib[4][id][2];

      CoeBot[0]=TDCCalib[4][id+16][0];
      CoeBot[1]=TDCCalib[4][id+16][1];
      CoeBot[2]=TDCCalib[4][id+16][2];

    }
  else if( Det==1) // Tof Plus
    {

      int module_top = IdToTdcModTp[1][id]; // top
      int ch_top   = IdToTdcChTp[1][id];;
      //      cout<<"top module = "<<module_top<<" ch="<<ch_top<<" "<<TDCCalib[module_top][ch_top][0]<<endl;
      CoeTop[0]=TDCCalib[module_top][ch_top][0];
      CoeTop[1]=TDCCalib[module_top][ch_top][1];
      CoeTop[2]=TDCCalib[module_top][ch_top][2];

      int module_bot = IdToTdcModTp[0][id]; // bottom
      int ch_bot   = IdToTdcChTp[0][id];
      //      cout<<"Bot module = "<<module_bot<<" ch="<<ch_bot<<" "<<TDCCalib[module_bot][ch_bot][0]<<endl;
      CoeBot[0]=TDCCalib[module_bot][ch_bot][0];
      CoeBot[1]=TDCCalib[module_bot][ch_bot][1];
      CoeBot[2]=TDCCalib[module_bot][ch_bot][2];
    }
  else if(Det==2) //Big Tof
    {
      int module_top = IdToTdcModBg[1][id]; // top
      int ch_top   = IdToTdcChBg[1][id];;

      CoeTop[0]=TDCCalib[module_top+2][ch_top][0];
      CoeTop[1]=TDCCalib[module_top+2][ch_top][1];
      CoeTop[2]=TDCCalib[module_top+2][ch_top][2];

      int module_bot = IdToTdcModBg[0][id]; // bottom
      int ch_bot   = IdToTdcChBg[0][id];

      CoeBot[0]=TDCCalib[module_bot+2][ch_bot][0];
      CoeBot[1]=TDCCalib[module_bot+2][ch_bot][1];
      CoeBot[2]=TDCCalib[module_bot+2][ch_bot][2];
    }
  else
    {
      cout<<"No detector specified "<<endl;
    }
  return true;
}





void Friend::Add()
{
//   TList *friends = Tin->GetListOfFriends();

//   const  Int_t nentries = (Int_t)(Tin->GetEntries());
//   const  Int_t nfriends = friends->GetSize();
//   const  Int_t ntrees = nfriends +1;

//   cout << "nfriends = " << nfriends << endl;
//   cout << "nentries = " << nentries << endl;
//   TTree   *tree_cond[nfriends+1];

//   TTree *newtree = 0;
//   //  TTree *tmptree = 0;

//   outfile->cd();

//   TFriendElement *fe = 0;  
//   fe = (TFriendElement*)friends->At(0);

//   tree_cond[0] = fe->GetParentTree();
// #ifdef DEBUG
//   cout << "tree_cond 0 = " << tree_cond[0]->GetName() << endl;
// #endif

//   for (Int_t j=0; j<nfriends; j++) {
//     fe = (TFriendElement*)friends->At(j);
//     tree_cond[j+1] = fe->GetTree();
// #ifdef DEBUG
//     cout << "tree_cond "<< j+1 <<" =" << tree_cond[j+1]->GetName() << endl;
// #endif
//   }
  
  
//   for (Int_t j=0; j<ntrees; j++) {
//     newtree = tree_cond[j]->CloneTree(-1);
// #ifdef DEBUG
//     TString name  = tree_cond[j]->GetName();
//     cout<<"tree_cond"<<j<<name.Data()<<endl;
// #endif

//     for (Int_t i=0; i<nentries; i++) {
      
//       tree_cond[j]->GetEntry(i);
//        newtree->Fill();
//      }
     
//      newtree->Write();
//   }  
   
//   //  Tin = newtree;
}
