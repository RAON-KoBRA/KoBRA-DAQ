#include "Friend.h"
#include "Define.h"
Int_t Friend::TrackingNegative(THyphiEvent_Phys_new* const Event_Out,THyphiEvent_Phys_track *Event_Track_Out)
{
  std::string funcname = "TrackingNegative : ";
  int NumNewPi=0;
#ifdef DEBUGTRACKNEG    
  cout<<funcname<<__LINE__<<endl;
#endif

  const double min_tofs = GetTimeZero();
  const double ChiCutBg = 20; //2010 12 23
  const double ChiCutAlBg = 20;
  // const double ChiCutBg = 10;
  // const double ChiCutAlBg = 10;
	
// #ifdef EUNHEE
//   std::vector<int> Layer;
//   std::vector<int> Wire;
//   std::vector<TVector3> ChambPos;
//   ScanBDC(Event_Chamber,Layer,Wire,ChambPos);
//   std::vector<int> LayerQ;
//   std::vector<int> WireQ;
//   std::vector<TVector3> ChambPosQ;
//   ScanSDC(Event_Chamber,LayerQ,WireQ,ChambPosQ);
// #endif

  double x[4],y[4],z[4];
  double x_albg[5],y_albg[5],z_albg[5];

  double p1,p2,chi,rchi;
  double reso_aladin[4] = {ResTR0,ResTR1,ResTR2,ResTFW};

  double reso_aladin_bg[5] = {ResTR0,ResTR1,ResTR2,ResTFW,ResAladin};
  double xc[4],yc[4],zc[4];
  double p1xc,p2xc,p1yc,p2yc;
  double chixc,chiyc,rchixc,rchiyc;

  double reso_c[3] = {ResTR0,ResTR1,ResTR2};

  std::vector<TVector3> pos_al[4];
  std::vector<TVector3> pos_bg[4];
  std::vector<TVector2> best_bg_px;
  std::vector<TVector2> best_bg_py;
  std::vector<int> pos_bg_tr0veto;
	  
  std::vector<double> pos_bg_scx;
  std::vector<double> pos_bg_scy;
  std::vector<double> pos_bg_tx;
  std::vector<double> pos_bg_ty;
  std::vector<TVector3> pos_albg[5];
	  
  std::vector<int> best_index[5];
  std::vector<int> best_index_prin[6];
  std::vector<int> best_princoin;



  std::vector<double> time_bg;
  std::vector<double> ene_bg;
  std::vector<double> chi_bg;
  std::vector<int> id_bg;

  std::vector<double> time_albg;
  std::vector<double> chi_albg;
  std::vector<int> id_albg;
  int duplid[4][18];
  bool duplflag[18];
  
  for(int i=0;i<18;i++)
    { 
      for(int j=0;j<4;j++)
	{
	  duplid[j][i]=0;
	}
      duplflag[i]=false;
    }
  //  const double min_tofs = GetTimeZero();
  const double min_tofs_ene = GetTimeZeroBarE();



#ifdef WTR0N
  for(size_t tr0=0;tr0<TR0HitCond.Pos.size();tr0++)
#endif
    for(size_t tr1=0;tr1<TR1HitCond.Pos.size();tr1++)
      for(size_t tr2=0;tr2<TR2HitCond.Pos.size();tr2++)
	//for(size_t tr2=0;tr2<0;tr2++)
	{
#ifdef DEBUGTRACKNEG    
  cout<<funcname<<__LINE__<<endl;
#endif

		  
	  x[1]=TR1HitCond.Pos[tr1].X();
	  y[1]=TR1HitCond.Pos[tr1].Y();
	  z[1]=TR1HitCond.Pos[tr1].Z();
		  
	  x[2]=TR2HitCond.Pos[tr2].X();
	  y[2]=TR2HitCond.Pos[tr2].Y();
	  z[2]=TR2HitCond.Pos[tr2].Z();

#ifdef WTR0N
	  x[0]=TR0HitCond.Pos[tr0].X();
	  y[0]=TR0HitCond.Pos[tr0].Y();
	  z[0]=TR0HitCond.Pos[tr0].Z();
#else
	  double slopex = (x[2]-x[1])/(z[2]-z[1]);
	  double intx = x[2] - slopex*z[2];

	  double slopey = (y[2]-y[1])/(z[2]-z[1]);
	  double inty = y[2] - slopey*z[2];

	  z[0] = TR0zPos;
	  x[0] = slopex*z[0]+intx;
	  y[0] = slopey*z[0]+inty;

#endif

	  bool ChambCoin=false;
		  
	  for(int i=0;i<3;i++)
	    {
	      xc[i]=x[i];
	      yc[i]=y[i];
	      zc[i]=z[i];


	      x_albg[i]=x[i];
	      y_albg[i]=y[i];
	      z_albg[i]=z[i];

	    }
		  
	  MT_LinearFit_Chi2(3, zc, xc, reso_c,
			    &p1xc,&p2xc,&chixc,&rchixc);
	  MT_LinearFit_Chi2(3, zc, yc, reso_c,
			    &p1yc,&p2yc,&chiyc,&rchiyc);



	  if(rchixc>5 || rchiyc>5)continue;


#ifdef DEBUGTRACKNEG    
  cout<<funcname<<__LINE__<<endl;
#endif





	  bool TR0veto[2] = {false,false};
	  int TR0VetoS = 0 ;
	  // for(size_t tr0v=0;tr0v<TR0HitCond.Pos.size();tr0v++)
	  //   {


	  //     if(fabs(TR0HitCond.Pos[tr0v].X() - x[0])<1.0)TR0veto[0]=true;
	  //     if(fabs(TR0HitCond.Pos[tr0v].Y() - y[0])<1.0)TR0veto[1]=true;
			
	  //   }

	  // if(TR0veto[0] && TR0veto[1])TR0VetoS=1;
	  //#endif


		  
#ifdef DEBUGTRACKNEG    
  cout<<funcname<<__LINE__<<endl;
#endif

	  h_track_tpos_bg->Fill(p1xc*TargetPosZ+p2xc,p1yc*TargetPosZ+p2yc);
#ifdef DEBUGTRACKNEG    
  cout<<funcname<<__LINE__<<endl;
#endif

	  if(fabs(p1yc*AladinExitZ+p2yc)>
	     Cut_AladinXYAbs[1] ||
	     fabs(p1xc*AladinEntranceZ+p2xc)>
	     Cut_AladinXYAbs[0])
	    {
	      continue;
	    }
#ifdef DEBUGTRACKNEG    
  cout<<funcname<<__LINE__<<endl;
#endif
	  double x_sc = p1xc*PrinzPos+p2xc;

	  int usedid_p[6]={-10,-10,-10,-10,-10,-10};
#ifdef PRINNEGATIVE
	  

	  bool coin[6] ={false,false,false,false,false,false};
	  
	  ChambCoin = CheckChambHitIdP(p1xc,p2xc,p1yc,p2yc,
				       min_tofs,Event_Out,usedid_p,coin);
	  
	  //if(!ChambCoin)continue;
	  
	  int NLayerCount=0;
	  if(coin[0]||coin[1])NLayerCount++;
	  if(coin[2]||coin[3])NLayerCount++;
	  if(coin[4]||coin[5])NLayerCount++;

	  //	  if(NLayerCount<2)continue;
	  
#endif

// 	  if(fabs(p1yc)<0.01)continue;
// 	  if(fabs(p1xc)<0.01)continue;

#ifdef DEBUGTRACKNEG    
  cout<<funcname<<__LINE__<<endl;
#endif

	  bool FoundAtTofp = false;
	  for(size_t j=0;j<TofPlusBarHitCond.Pos.size();j++)
	    {
	      double posy_tofp = TofPlusBarHitCond.Pos[j].Y();
		      
	      double calu_at_tofp = p1yc*TofPlusBarHitCond.Pos[j].Z()+p2yc;
	      if(fabs(posy_tofp-calu_at_tofp)<25)
		FoundAtTofp=true;
	    }

	  //if(FoundAtTofp)continue;

		
	  for(size_t j=0;j<TofPlusPlusHitCond.Pos.size();j++)
	    {
	      //if(bar_veto[j])continue;
		    
	      x[3] = TofPlusPlusHitCond.Pos[j].X();
	      y[3] = TofPlusPlusHitCond.Pos[j].Y();
	      z[3] = TofPlusPlusHitCond.Pos[j].Z();
	      /*
	      double zshift = 105*TMath::Sin(22.8*TMath::DegToRad());
	      double zminedge = z[3] + zshift/2.0;
	      
	      double xshift = 105*TMath::Cos(22.8*TMath::DegToRad());
	      double xminedge = x[3] - xshift/2.0;
	      double best_dist = 9999;
	      double best_x = 0;
	      double best_z = 0;
	      double best_y;
	      //	      cout<<"init "<<x[3]<<" "<<z[3]<<endl;
	      int selected=0;
	      for(int seg = 0;seg<10;seg++)
		{
		  double segz = zminedge-zshift/10*(seg+0.5);
		  double segx = xminedge+xshift/10*(seg+0.5);

		  double postrack = p1yc*segz+p2yc;
		  double disttemp =fabs(y[3]-postrack);
		  //		  cout<<seg<<" "<<segx<<" "<<segz<<" "<<disttemp<<endl;
		  if(disttemp<best_dist)
		    {
		      best_dist = disttemp;
		      best_x = segx;
		      best_z = segz;
		      best_y = postrack;
		      selected = seg;
		    }
		}
// 	      if(selected>0&&selected<9)
// 	      cout<<"selected "<<selected<<endl;

	      x[3] = best_x;
	      y[3] = best_y;
	      z[3] = best_z;

	      */

	      MT_LinearFit_Chi2(4, z, y, reso_aladin,
				&p1,&p2,&chi,&rchi);


	      h_chi_bg->Fill(rchi);
	      //	      cout<<"Chi ="<<rchi<<endl;
	      if(rchi<ChiCutBg)
		{
		  //		  cout<<"if Chi ="<<rchi<<endl;
		  for(int pl=0;pl<4;pl++)
		    {
		      //		      cout<<"for Chi ="<<rchi<<endl;
		      pos_bg[pl].push_back(TVector3(x[pl],
						    y[pl],
						    z[pl]));	
		    }

		  best_index[0].push_back((int)TR1HitCond.Id[tr1].X());
		  best_index[1].push_back((int)TR1HitCond.Id[tr1].Y());
		  best_index[2].push_back((int)TR2HitCond.Id[tr2].X());
		  best_index[3].push_back((int)TR2HitCond.Id[tr2].Y());
		  best_index[4].push_back((int)TofPlusPlusHitCond.Id[j].Y());
		  for(int prinpl=0;prinpl<6;prinpl++)
		    {
		      best_index_prin[prinpl].push_back(usedid_p[prinpl]);
		    }
		  best_princoin.push_back(ChambCoin);
		  pos_bg_tr0veto.push_back(TR0VetoS);
		  pos_bg_scx.push_back(x_sc);
		  pos_bg_scy.push_back(p1*PrinzPos+p2);
			
		  pos_bg_tx.push_back(p1xc*TargetPosZ+p2xc);
		  pos_bg_ty.push_back(p1yc*TargetPosZ+p2yc);
			
		  best_bg_px.push_back(TVector2(p1xc,p2xc));
		  best_bg_py.push_back(TVector2(p1yc,p2yc));
			
		  time_bg.push_back(TofPlusPlusHitCond.Id[j].X());
		  ene_bg.push_back(TofPlusPlusHitCond.Wid[j].X());
		  id_bg.push_back((int)TofPlusPlusHitCond.Wid[j].Y());
		  chi_bg.push_back(rchi);
			

		}
		    
	    }
#ifdef DEBUGTRACKNEG    
  cout<<funcname<<__LINE__<<endl;
#endif


	  // for(size_t j=0;j<TofAladinHitCond.Pos.size();j++)
	  //   for(size_t k=0;k<TofPlusPlusHitCond.Pos.size();k++)
	  //     {
		      
	  // 	x_albg[3] = TofPlusPlusHitCond.Pos[k].X();
	  // 	y_albg[3] = TofPlusPlusHitCond.Pos[k].Y();
	  // 	z_albg[3] = TofPlusPlusHitCond.Pos[k].Z();

	  // 	x_albg[4] = TofAladinHitCond.Pos[j].X();
	  // 	y_albg[4] = TofAladinHitCond.Pos[j].Y();
	  // 	z_albg[4] = TofAladinHitCond.Pos[j].Z();

	  // 	// 		      if( (x_albg[3]-x_albg[4]) >-650 &&
	  // 	// 			  (x_albg[3]-x_albg[4]) <100 )
	  // 	{
			  
	  // 	  MT_LinearFit_Chi2(4, z_albg, y_albg, reso_aladin_bg,
	  // 			    &p1,&p2,&chi,&rchi);
	  // 	  h_chi_albg->Fill(rchi);
			  
	  // 	  if(rchi<ChiCutAlBg)
	  // 	    {
	  // 	      for(int pl=0;pl<5;pl++)
	  // 		{
	  // 		  pos_albg[pl].
	  // 		    push_back(TVector3(x_albg[pl],
	  // 				       y_albg[pl],
	  // 				       z_albg[pl]));		
	  // 		}
	  // 	      time_albg.push_back(TofPlusPlusHitCond.Id[k].X());
	  // 	      id_albg.push_back((int)TofPlusPlusHitCond.Wid[k].Y());
	  // 	      chi_albg.push_back(rchi+rchixc);
	  // 	    }
	  // 	}
	  //     }
	}//loop for tr0,1,2


#ifdef DEBUGTRACKNEG    
  cout<<funcname<<__LINE__<<endl;
#endif
  if(pos_al[0].size()>0)
    h_CombAlside[0]->Fill(pos_al[0].size());
  if(pos_bg[0].size()>0)
    h_CombAlside[1]->Fill(pos_bg[0].size());



#ifdef DEBUGTRACKNEG    
  cout<<" pos_bg "<<pos_bg[0].size()<<endl;
#endif

  double best_rho_bg[18];
  double best_poq_bg[18];
  double best_beta_bg[18];
  double best_ene_bg[18];
  double best_temp_bg[18];
  double best_mass_bg[18];
  double best_charge[18];
  double best_pos_scx[18];
  double best_pos_scy[18];
  double best_rho_bg_new[18];
  double best_beta_bg_new[18];
  double best_pos_bg_y[18];
  double best_pos_bg_x[18];
  double best_pos_bg_x_atAl[18];
  double best_postx_bg[18];
  double best_posty_bg[18];

  double best_momx_bg[18];
  double best_momy_bg[18];
  double best_momz_bg[18];

  double best_p1x_bg[18];
  double best_p2x_bg[18];

  double best_p1y_bg[18];
  double best_p2y_bg[18];



  double best_rho_albg[18];
  double best_beta_albg[18];
  double best_temp_albg[18];

  bool best_track_queen[18];
  bool best_track_prin[18];
  int  best_coin_tofs[18];


  //	  int best_bar_bg=0;
  for(int i=0;i<18;i++)
    {
      best_rho_bg[i]=0;
      best_poq_bg[i]=0;
      best_beta_bg[i]=0;
      best_ene_bg[i]=0;
      best_temp_bg[i]=100;
      best_mass_bg[i]=0;
      best_charge[i]=0;
      best_pos_scx[i]=999;
      best_pos_scy[i]=-999;
      best_rho_bg_new[i]=0;
      best_beta_bg_new[i]=0;
      best_pos_bg_y[i]=-9999;
      best_pos_bg_x[i]=-9999;
      best_pos_bg_x_atAl[i]=-9999;
      best_postx_bg[i]=999;
      best_posty_bg[i]=-999;
      
      best_momx_bg[i]=-9999;
      best_momy_bg[i]=-9999;
      best_momz_bg[i]=-9999;

      best_p1x_bg[i] = -9999;
      best_p2x_bg[i] = -9999;
      best_p1y_bg[i] = -9999;
      best_p2y_bg[i] = -9999;

	      


      best_rho_albg[i]=0;
      best_beta_albg[i]=0;
      best_temp_albg[i]=100;

      best_track_queen[i]=false;
      best_track_prin[i]=false;
      best_coin_tofs[i]=0;
    }
  //  cout<<"Candidate = "<<pos_bg[0].size()<<endl;
  for(size_t comb=0;comb<pos_bg[0].size();comb++)
    {
      //      cout<<"loop  "<<comb<<" "<<__LINE__<<endl;
      double pos_bfm1p[3]; //tr1
      double pos_bfm2p[3]; //tr2
	      
      double pos_afm2[3]; //bigtof
      double pos_afm1new[3]={0,0,0}; //tofp
      double pos_afm2new[3]={0,0,0}; //aladin
	      
      MT_Input_to_3Vector(pos_bg[1][comb].X(),
			  pos_bg[1][comb].Y(),
			  pos_bg[1][comb].Z(),pos_bfm1p);
	      
      MT_Input_to_3Vector(pos_bg[2][comb].X(),
			  pos_bg[2][comb].Y(),
			  pos_bg[2][comb].Z(),pos_bfm2p);
	      
	      
      MT_Input_to_3Vector(pos_bg[3][comb].X(),
			  pos_bg[3][comb].Y(),
			  pos_bg[3][comb].Z(),pos_afm2);


      double center_temp[2] ={0.0, 0.0};
      double pos_ent_temp[2]={0.0, 0.0};
      double pos_exi_temp[2]={0.0, 0.0};
	      
      double pos_exi_temp2[3]={0.0,0.0, 0.0};
	      
      //double center[3] ={0.0, 0.0, 0.0};
      double pos_ent[3]={0.0, 0.0, 0.0};
      double pos_exi[3]={0.0, 0.0, 0.0};
	      
      double ini_rho;//, ini_p;
	      
	      
//       ini_rho = GetRho(pos_bfm1p[0],pos_bfm1p[2],
// 		       pos_bfm2p[0],pos_bfm2p[2],
// 		       pos_afm2[0],pos_afm2[2],
// 		       0,pos_ent_temp,pos_exi_temp,
// 		       center_temp);

      double length_xyz=0;
      ini_rho = GetRho(-1,pos_bfm1p[0],pos_bfm1p[2],pos_bfm2p[0],pos_bfm2p[2],
		       pos_afm2[0],pos_afm2[1],pos_afm2[2],length_xyz,
		       pos_ent_temp,pos_exi_temp,center_temp);//m

      if(ini_rho<0)continue;
      //      cout<<"loop  "<<comb<<" "<<__LINE__<<endl;
      //      cout<<"loop  "<<__LINE__<<endl;
// #ifdef RK
      
//       double temp_mom[2]={ini_rho*0.3*0.7447*0.85,ini_rho*0.3*0.7447*1.1};//MeV
//       double temp_p[3];
//       double min_diff_tofp=999999.;
//       double p_RK[3]={0,0,0};
//       double pos_RK[3]={0,0,0};
	      
//       prop->SetMaxZ(pos_afm2[2]/10.);//cm
//       prop->SetMaxR(10000);
//       prop->SetMaxStep(1);
	      
//       int ip_min = 999;
	      
	      
//       double vecin_temp[3];
//       for(int an=0;an<3;an++)
// 	{
// 	  vecin_temp[an] = pos_bfm2p[an]-pos_bfm1p[an];
// 	}
	      
//       double vecin_abs_temp =
// 	MT_AbsoluteValueOfVector(3,vecin_temp);
	      
//       double unit[3];
//       for(int an=0;an<3;an++)unit[an] = vecin_temp[an]/vecin_abs_temp;
	      
	      
//       int ipcount=0;
//       double length_rk=0;
//       for(int ip=(int)temp_mom[0];ip<(int)temp_mom[1];ip++)
// 	{
// 	  ip+=1;
// 	  temp_p[0]=ip/1000.*unit[0];//GeV
// 	  temp_p[1]=ip/1000.*unit[1];
// 	  temp_p[2]=ip/1000.*unit[2];
		  
// 	  rc = new TEveRecTrack();
		  
		  
// 	  rc->fV.Set(pos_bfm2p[0]/10,pos_bfm2p[1]/10,pos_bfm2p[2]/10);
// 	  rc->fP.Set(temp_p[0],temp_p[1],temp_p[2]);
// 	  rc->fSign = -1;
		  
// 	  track = new TEveTrack(rc, prop);
// 	  track->MakeTrack(); //big tof
		  
// 	  double xl=-999999.9;
// 	  double yl=-999999.9;
// 	  double zl=-999999.9;
// 	  h_rk_lastpoint->Fill(track->GetLastPoint());
// 	  if(track->GetLastPoint()>5000){
// 	    delete rc;
// 	    delete track;
// 	    continue;
// 	  }		  
		  
// 	  track->GetPoint(track->GetLastPoint(),xl,yl,zl);//cm
		  
// 	  xl *= 10.;//mm
// 	  yl *= 10.;	      
// 	  zl *= 10.;	      
// 	  // 	      cout<<"last "<<track->GetLastPoint()<<endl;
// 	  // 	      cout<<"("<<xl<<","<<yl<<","<<zl<<")"<<endl;//in TOF+ 
// 	  // 	      cout<<"charge "<<track->GetCharge()<<endl;
// 	  // 	      cout<<track->GetEndMomentum()[0]<<" "<<track->GetEndMomentum()[1]<<" "<<track->GetEndMomentum()[2]<<endl;
		  
// 	  double diff_tofp = fabs(xl-pos_afm2[0]);
// 	  if(diff_tofp<min_diff_tofp)
// 	    {
// 	      ip_min = ip-(int)temp_mom[0];
// 	      min_diff_tofp = diff_tofp;
// 	      p_RK[0] = temp_p[0];
// 	      p_RK[1] = temp_p[1];
// 	      p_RK[2] = temp_p[2];
		      
// 	      pos_RK[0] = xl;
// 	      pos_RK[1] = yl;
// 	      pos_RK[2] = zl;
// 	      length_rk=track->GetLastPoint();
// 	      //lastpoint
// 	    }
// 	  delete rc;
// 	  delete track;
// 	}
//       if(length_rk<=10)
// 	{
// 	  cout<<"sss "<<endl;
// 	  continue;
// 	}
//       double length_xyz_rk = Length_TOFs_TR2(pos_bfm1p[0],pos_bfm1p[1],pos_bfm1p[2],pos_bfm2p[0],pos_bfm2p[1],pos_bfm2p[2]) + length_rk*10;
//       double p_RK_abs =sqrt(p_RK[0]*p_RK[0]+p_RK[1]*p_RK[1]+p_RK[2]*p_RK[2]);

//       double xq[5],yq[5],zq[5];
	      
//       for(int iq=0;iq<5;iq++)
// 	{
// 	  xq[iq]= -9999.;
// 	  yq[iq]= -9999.;				  
// 	  zq[iq]= -9999.;
		  
// 	  prop->SetMaxZ((QueenzPos+QueenPlOffs[iq])/10.);//cm
// 	  prop->SetMaxR(10000);
// 	  prop->SetMaxStep(1);
		  
// 	  rc = new TEveRecTrack();
		  
// 	  rc->fV.Set(pos_bfm2p[0]/10,pos_bfm2p[1]/10,pos_bfm2p[2]/10);
// 	  rc->fP.Set(p_RK[0],p_RK[1],p_RK[2]);
// 	  rc->fSign = -1;
		  
// 	  track = new TEveTrack(rc, prop);
// 	  track->MakeTrack();
		  
// 	  track->GetPoint(track->GetLastPoint(),xq[iq],yq[iq],zq[iq]);
// 	  delete rc;
// 	  delete track;
		  
// 	}
//       for(int iq=0;iq<5;iq++)
// 	{
// 	  xq[iq] *=10.0;
// 	  yq[iq] *=10.0;
// 	  zq[iq] *=10.0;
// 	}
//       bool hoge2rk = false;
//       //hoge2rk=CheckChambHitQueenIdPH(xq,yq,Event_Chamber);
//       //	      if(!hoge2rk)continue;

// #endif


      bool positive_flag = false;

      MT_Line line_rec;
      line_rec.From_Points(pos_bfm1p,pos_bfm2p);
	      
      pos_ent[2]=pos_ent_temp[1];
      pos_exi_temp2[2]=pos_exi_temp[1];
	      
      if(line_rec.Get_XY_atZ(pos_ent)==1)
	{
	  if(line_rec.Get_XY_atZ(pos_exi_temp2)==1)
	    {
		      
	      pos_exi[0] = pos_exi_temp[0];
	      pos_exi[1] = pos_exi_temp2[1];
	      pos_exi[2] = pos_exi_temp[1];
		      
	      MT_Line line_ins;
	      line_ins.From_Points(pos_ent,pos_exi);
		      
	      double inter[3]={0.0, 0.0, 0.0};
	      inter[2]=center_temp[1];
	      if(line_ins.Get_XY_atZ(inter)==1)
		{
		  double center[3]={0,0,0,};
		  center[0] = center_temp[0];
		  center[1] = inter[1];
		  center[2] = center_temp[1];
			  

		  MT_Line line_afm;
		  line_afm.From_Points(pos_exi,pos_afm2);
			  
			  

		  //cout<<"loop  "<<__LINE__<<endl;
		  //		  cout<<"loop  "<<comb<<" "<<__LINE__<<endl;


#ifdef DEBUGTRACKNEG    
		  cout<<funcname<<" line "<<__LINE__<<endl;
#endif



		  double qpos[3] = {0,0,0};
			  
		  qpos[2] = QueenzPos;


		  bool CoinQ[5] = {false,false,false,false,false};
		  double slopexb = (pos_bfm1p[0]-pos_bfm2p[0])/(pos_bfm1p[2]-pos_bfm2p[2]);


		  double slopex = (pos_afm2[0]-pos_exi[0])/(pos_afm2[2]-pos_exi[2]);
		  double intx = pos_afm2[0] - slopex*pos_afm2[2];
			  
		  double slopey = (pos_afm2[1]-pos_exi[1])/(pos_afm2[2]-pos_exi[2]);
		  double inty = pos_afm2[1] - slopey*pos_afm2[2];



		   
		   double vec_in[2];
		   vec_in[0] = pos_afm2[0]-pos_exi[0];
		   vec_in[1] = pos_afm2[2]-pos_exi[2];
		   double pos_1[2];
		   pos_1[0] = pos_exi[0];
		   pos_1[1] = pos_exi[2];
		   
		   double pos_al[2];
		   pos_al[0] = 2355.32 + (1187.5 - 25*(1-1))*TMath::Cos(23.5*TMath::DegToRad());
		   pos_al[1] = 5155.86 - (1187.5 - 25*(1-1))*TMath::Sin(23.5*TMath::DegToRad());

		   double pos_al2[2];
		   pos_al2[0] = 2355.32 + (1187.5 - 25*(100-1))*TMath::Cos(23.5*TMath::DegToRad());
		   pos_al2[1] = 5155.86 - (1187.5 - 25*(100-1))*TMath::Sin(23.5*TMath::DegToRad());
		   
		   double vec_al[2];
		   vec_al[0] = pos_al2[0]-pos_al[0];
		   vec_al[1] = pos_al2[1]-pos_al[1];
		   double pos_atAl[2];
		   MT_Intersection_of_Lines_2D(pos_1,vec_in,pos_al,vec_al,pos_atAl);



		  //cout<<slopexb<<endl;
			  
		  if(line_afm.Get_XY_atZ(qpos)==1)
		    {

		      //		      cout<<"loop  "<<comb<<" "<<__LINE__<<endl;


// 		      qpos[2] = QueenzPos+QueenPlOffs[0];
// 		      if(line_afm.Get_XY_atZ(qpos)==1)
// 			{
// 			  //if(CoinQ[4])
// 			  for(int cam=0;cam<Event_Out->n_CaMx ;cam++)
// 			    {
// 			      THyphiHit_new temp_cham_hit;
// 			      Event_Out->Get_Hit(cam,"CaMx",temp_cham_hit);
// 			      double id_cal = GetXidQ(qpos[0],qpos[1]);
// 			      for(size_t ss =0;ss<temp_cham_hit.Id_x.size();ss++)
// 				{
// 				  //double dtu = temp_cham.TDC_LE[ss];
// 				  //if(dtu>120 &&dtu<160)
// 				  {
					      
// 				    h_track_id_coin_que_x_bt->Fill(temp_cham_hit.Id_x[ss],id_cal);
// 				    if(slopexb<-0.2)
// 				      h_track_id_coin_que_x_bt_angle->Fill(temp_cham_hit.Id_x[ss],id_cal);
// 				    if(fabs(temp_cham_hit.Id_x[ss]-id_cal)<2.5)CoinQ[0] = true;
// 				  }
// 				}
				      
// 			    }
// 			}
// #ifdef DEBUGTRACKNEG    
// 		  cout<<funcname<<" line "<<__LINE__<<endl;
// #endif
			      
// 		      qpos[2] = QueenzPos+QueenPlOffs[1];
// 		      if(line_afm.Get_XY_atZ(qpos)==1)
// 			{
// 			  //if(CoinQ[4])
// 			  for(int cam=0;cam<Event_Out->n_CaMxp ;cam++)
// 			    {
// 			      THyphiHit_new temp_cham_hit;
// 			      Event_Out->Get_Hit(cam,"CaMxp",temp_cham_hit);
// 			      double id_cal = GetXpidQ(qpos[0],qpos[1]);
// 			      for(size_t ss =0;ss<temp_cham_hit.Id_x.size();ss++)
// 				{
// 				  //double dtu = temp_cham.TDC_LE[ss];
// 				  //if(dtu>120 &&dtu<160)
// 				  {
// 				    h_track_id_coin_que_xp_bt->Fill(temp_cham_hit.Id_x[ss],id_cal);
// 				    if(slopexb<-0.2)
// 				      h_track_id_coin_que_xp_bt_angle->Fill(temp_cham_hit.Id_x[ss],id_cal);
// 				    if(fabs(temp_cham_hit.Id_x[ss]-id_cal)<2.5)CoinQ[1] = true;
// 				  }
// 				}
				      
// 			    }
// 			}

// #ifdef DEBUGTRACKNEG    
// 		  cout<<funcname<<" line "<<__LINE__<<endl;
// #endif

// 		      qpos[2] = QueenzPos+QueenPlOffs[2];
// 		      if(line_afm.Get_XY_atZ(qpos)==1)
// 			{
// 			  //if(CoinQ[0] && CoinQ[1] )
// 			  // 				    if(CoinQ[4])
// 			  for(int cam=0;cam<Event_Out->n_CaMy ;cam++)
// 			    {
// 			      THyphiHit_new temp_cham_hit;
// 			      Event_Out->Get_Hit(cam,"CaMy",temp_cham_hit);
// 			      double id_cal = GetYidQ(qpos[0],qpos[1]);
// 			      for(size_t ss =0;ss<temp_cham_hit.Id_x.size();ss++)
// 				{
// 				  //double dtu = temp_cham.TDC_LE[ss];
// 				  //if(dtu>120 &&dtu<160)
// 				  {
					      
// 				    h_track_id_coin_que_y_bt->Fill(temp_cham_hit.Id_x[ss],id_cal);
// 				    if(slopexb<-0.2)
// 				      h_track_id_coin_que_y_bt_angle->Fill(temp_cham_hit.Id_x[ss],id_cal);
// 				    if(fabs(temp_cham_hit.Id_x[ss]-id_cal)<2.5)CoinQ[2] = true;
// 				  }
// 				}
				      
// 			    }
// 			}
// #ifdef DEBUGTRACKNEG    
// 		  cout<<funcname<<" line "<<__LINE__<<endl;
// #endif

// 		      qpos[2] = QueenzPos+QueenPlOffs[3];
// 		      if(line_afm.Get_XY_atZ(qpos)==1)
// 			{
// 			  //if(CoinQ[0] && CoinQ[1])
// 			  // 				    if(CoinQ[4])
// 			  for(int cam=0;cam<Event_Out->n_CaMyp ;cam++)
// 			    {
// 			      THyphiHit_new temp_cham_hit;
// 			      Event_Out->Get_Hit(cam,"CaMyp",temp_cham_hit);
// 			      double id_cal = GetYpidQ(qpos[0],qpos[1]);
// 			      for(size_t ss =0;ss<temp_cham_hit.Id_x.size();ss++)
// 				{
// 				  //double dtu = temp_cham.TDC_LE[ss];
// 				  //if(dtu>120 &&dtu<160)
// 				  {
// 				    h_track_id_coin_que_yp_bt->Fill(temp_cham_hit.Id_x[ss],id_cal);
// 				    if(slopexb<-0.2)
// 				      h_track_id_coin_que_yp_bt_angle->Fill(temp_cham_hit.Id_x[ss],id_cal);
// 				    if(fabs(temp_cham_hit.Id_x[ss]-id_cal)<2.5)CoinQ[3] = true;
// 				  }
// 				}
				      
// 			    }
// 			}
// #ifdef DEBUGTRACKNEG    
// 		      cout<<funcname<<" line "<<__LINE__<<endl;
// #endif

// 		      qpos[2] = QueenzPos+QueenPlOffs[4];
// 		      if(line_afm.Get_XY_atZ(qpos)==1)
// 			{
// 			  //  if(CoinQ[0] && CoinQ[1] && CoinQ[2]&&CoinQ[3])
// 			  //if(n_CaM_u<4)
// 			  for(int cam=0;cam<Event_Out->n_CaMu ;cam++)
// 			    {
// 			      THyphiHit_new temp_cham_hit;
// 			      Event_Out->Get_Hit(cam,"CaMu",temp_cham_hit);
// 			      double id_cal = GetUidQ(qpos[0],qpos[1]);
// 			      for(size_t ss =0;ss<temp_cham_hit.Id_x.size();ss++)
// 				{
// 				  //double dtu = temp_cham.TDC_LE[ss];
// 				  //if(dtu>120 &&dtu<160)
// 				  {
// 				    h_track_id_coin_que_u_bt->Fill(temp_cham_hit.Id_x[ss],id_cal);
// 				    if(slopexb<-0.2)
// 				      h_track_id_coin_que_u_bt_angle->Fill(temp_cham_hit.Id_x[ss],id_cal);
// 				    if(fabs(temp_cham_hit.Id_x[ss]-id_cal)<2.5)CoinQ[4] = true;
// 				  }
// 				}
					  
// 			    }
				  
// 			}






//		      cout<<"loop  "<<comb<<" "<<__LINE__<<endl;


		    }
#ifdef DEBUGTRACKNEG    
		  cout<<funcname<<" line "<<__LINE__<<endl;
#endif
			      
			      


			  
		  // 			  double slopex = (pos_afm2[0]-pos_exi[0])/(pos_afm2[2]-pos_exi[2]);
		  // 			  double intx = pos_afm2[0] - slopex*pos_afm2[2];
			  
		  // 			  double slopey = (pos_afm2[1]-pos_exi[1])/(pos_afm2[2]-pos_exi[2]);
		  // 			  double inty = pos_afm2[1] - slopey*pos_afm2[2];
			  

		  //lkkkkkkkkkkk
		  //BigChambCoin=CheckChambHitQueenIdP(qpos,Event_Chamber);
		  //  			      if(!BigChambCoin)continue;


		  bool hoge=false;
		  if(CoinQ[0] && CoinQ[1] &&CoinQ[2]&&CoinQ[3] && CoinQ[4])
		    {
		      hoge=true;
		    }
		  // 			  cout<<line_afm2.slope[0]/line_afm2.slope[2]<<" "<<slopex<<endl;
		  // 			  cout<<line_afm2.slope[1]/line_afm2.slope[2]<<" "<<slopey<<endl;
		  // 			  cout<<line_afm2.intercept[0]-line_afm2.intercept[2]*
		  // 			    line_afm2.slope[0]/line_afm2.slope[2]<<" "<<intx<<endl;
		  // 			  cout<<line_afm2.intercept[1]-line_afm2.intercept[2]*
		  // 			    line_afm2.slope[1]/line_afm2.slope[2]<<" "<<inty<<endl;


		  // 			  double slopex = (pos_afm2[0]-pos_exi[0])/(pos_afm2[2]-pos_exi[2]);
		  // 			  double intx = pos_afm2[0] - slopex*pos_afm2[2];
			  
		  // 			  double slopey = (pos_afm2[1]-pos_exi[1])/(pos_afm2[2]-pos_exi[2]);
		  // 			  double inty = pos_afm2[1] - slopey*pos_afm2[2];








		  //		  cout<<"loop  "<<comb<<" "<<__LINE__<<endl;








			  
		  // 			  bool hoge2=CheckChambHitQueenIdP(slopex,intx,
		  // 							   slopey,inty,
		  // 							   Event_Chamber);
			  
		  //			  cout<<hoge<<hoge2<<endl;

		  //if(!CoinQ[0] ||! CoinQ[1] || !CoinQ[2]||!CoinQ[3] || !CoinQ[4])continue;


		  //if(!hoge2)continue;
		  
			  
		  double posx[5],posz[5];
		  bool coin[6] ={false,false,false,false,false,false};
		  int usedid_q[5]={-1,-1,-1,-1,-1};
		  std::vector<int> Id_x;
		  std::vector<int> Id_xp;
		  std::vector<int> Id_y;
		  std::vector<int> Id_yp;
		  std::vector<int> Id_u;
		  bool hoge2=CheckChambHitQueenIdPHP(-1,slopex,intx,
						     slopey,inty,
						     Event_Out,posx,posz,coin,usedid_q,
						     Id_x,Id_xp,Id_y,Id_yp,Id_u);


		  // bool hoge_coin=CheckChambHitQueenIdPHP(-3,slopex,intx,
 		  //  					 slopey,inty,
 		  //  					 Event_Out,posx,posz,coin,usedid_q);

		  //if(!coin[0]&&!coin[1]&&!coin[2]&&!coin[3])continue;


#ifdef DEBUGTRACKNEG    
		  cout<<funcname<<" line "<<__LINE__<<endl;
#endif
			  

 		  int nqueen=0;
		  //		  cout<<"loop  "<<comb<<" "<<__LINE__<<endl;
 		  if(coin[0]||coin[1])nqueen++;
 		  if(coin[2]||coin[3])nqueen++;
// 		  if(coin[4])nqueen++;
 		  if(nqueen<2)continue;
//		  cout<<"loop  "<<comb<<" "<<__LINE__<<endl;

			  
//		  if(!hoge2)continue;

#ifdef NEGWCHAMB		  
		  double xx[3],yy[3],zz[3],zzy[3];
		  double xx_reso[3] = {9.0/sqrt(8.0),9.0/sqrt(8.0),105/sqrt(8.0)};
		  double p1aft,p2aft;
		  double chiaft,rchiaft;
		  xx[0] = posx[0];
		  zz[0] = posz[0];
			
		  xx[1] = posx[1];
		  zz[1] = posz[1];
			
		  yy[0] = posx[2];
		  zzy[0] = posz[2];
		  yy[1] = posx[3];
		  zzy[1] = posz[3];
			
			


		  xx[2] = pos_afm2[0];
		  yy[2] = pos_afm2[1];
		  zz[2] = pos_afm2[2];
		  zzy[2] = pos_afm2[2];

		  MT_LinearFit_Chi2(3, zz, xx,xx_reso,
				    &p1aft,&p2aft,&chiaft,&rchiaft);
		  pos_afm1new[0] = p1aft*zz[0]+p2aft;
		  pos_afm1new[2] = zz[0];

		  pos_afm2new[0] = p1aft*zz[2]+p2aft;
		  pos_afm2new[2] = zz[2];
		  double ini_rho_new = GetRho(pos_bfm1p[0],pos_bfm1p[2],
					      pos_bfm2p[0],pos_bfm2p[2],
					      pos_afm1new[0],pos_afm1new[2],
					      pos_afm2new[0],pos_afm2new[2],
					      0,pos_ent_temp,pos_exi_temp,
					      center_temp);
			
		  //		  cout<<"loop  "<<comb<<" "<<__LINE__<<endl;

		  double length1 = sqrt(pos_ent_temp[0]*pos_ent_temp[0]+pos_ent_temp[1]*pos_ent_temp[1]);

		  double length3 = sqrt((pos_exi_temp[0]-pos_afm2new[0])*
					(pos_exi_temp[0]-pos_afm2new[0])+
					(pos_exi_temp[1]-pos_afm2new[2])*
					(pos_exi_temp[1]-pos_afm2new[2]));
			  
		  double vec1new[2],vec2new[2];
		  vec1new[0] = pos_ent_temp[0]-center_temp[0];
		  vec1new[1] = pos_ent_temp[1]-center_temp[1];
		  vec2new[0] = pos_exi_temp[0]-center_temp[0];
		  vec2new[1] = pos_exi_temp[1]-center_temp[1];

		  double costheta =  (vec1new[0]*vec2new[0]+vec1new[1]*vec2new[1])/sqrt(vec1new[0]*vec1new[0]+vec1new[1]*vec1new[1])/
		    sqrt(vec2new[0]*vec2new[0]+vec2new[1]*vec2new[1]);

		  double theta =acos(costheta);
		  double length2=ini_rho_new*theta;
		  double length_xz_new = length1+length2+length3;
		  double length_xyz_new = sqrt(length_xz_new*length_xz_new + pos_afm2[1]*pos_afm2[1]);
			  
#else 
		  double ini_rho_new = 0;
		  double length_xyz_new = 0;
#endif
		  // 			ini_rho =ini_rho_new;
		  // 			length_xyz = length_xyz_new;

#ifdef DEBUGTRACKNEG    
		  cout<<funcname<<" line "<<__LINE__<<endl;
#endif

		  double naiseki_s=0.0;
		  double abs1_s=0.0;
		  double abs2_s=0.0;
		  double plength_s[3]={0.0,0.0,0.0};
			  
		  for(int an=0;an<3;an++)
		    {
		      naiseki_s += (pos_ent[an]-center[an]) *
			(pos_exi[an]-center[an]);
			      
		      abs1_s += (pos_ent[an]-center[an])*
			(pos_ent[an]-center[an]);
			      
		      abs2_s += (pos_exi[an]-center[an])*
			(pos_exi[an]-center[an]);
			      
		      plength_s[0] +=pos_ent[an]*pos_ent[an];
		      plength_s[2] +=(pos_afm2[an]-pos_exi[an])*
			(pos_afm2[an]-pos_exi[an]);
			      
		    }
			  
		  abs1_s = sqrt(abs1_s);
		  abs2_s = sqrt(abs2_s);
		  plength_s[0] = sqrt(plength_s[0]);
		  plength_s[2] = sqrt(plength_s[2]);
			  
		  //		  cout<<"loop  "<<comb<<" "<<__LINE__<<endl;			  
			      
		  double inan_s = TMath::ACos(naiseki_s/(abs1_s*abs2_s));
		  plength_s[1] = ini_rho*inan_s;
		  //cout<<abs1<<" abs2="<<abs2<<" ian"<<inan<<" ini_rho"<<ini_rho<<endl;
		  //cout<<"Lenght 1="<<plength[0]<<" 2="<<plength[1]<<" 3="<<plength[2]<<endl;
			  
			  
			  
		  //double PathLength_s = plength_s[0]+plength_s[1]+plength_s[2];
			  
			  
			  
		  //			      double beta_s= PathLength_s/time_bg[comb]/300.;
		  //double beta_s= length_xyz/time_bg[comb]/300.;
#ifdef RK
		  double beta_s= length_xyz_rk/time_bg[comb]/300.;
#else
		  double beta_s= length_xyz/time_bg[comb]/300.;
#endif
		  double beta_s_new= length_xyz_new/time_bg[comb]/300.;
		  //kkkkkkkkkkkkkk
		  h_rho_beta_bg[id_bg[comb]]->Fill(ini_rho/1000.,beta_s);
		  h_rho_beta_bg_all->Fill(ini_rho/1000.,beta_s);

#ifdef DEBUGTRACKNEG    
		  cout<<funcname<<" line "<<__LINE__<<endl;
#endif
		  
		  double ini_s=0;
		  MT_RhoToMomentum(Field_Strength,ini_rho,
				   1,&ini_s);
		  double ini_s_new=0;
		  MT_RhoToMomentum(Field_Strength,ini_rho_new,
				   1,&ini_s_new);
		  double recmass_bg=-100.0;
		  double recmass_bg_new=-100.0;
		  //			      if(beta_s>1)beta_s=0.999999;
		  //			      {
		  double gamma_s = 1./sqrt(1.-beta_s*beta_s);
		  double gamma_s_new = 1./sqrt(1.-beta_s_new*beta_s_new);
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
		  double betag=beta_s*gamma_s;

		  double ini_s_tot_new =ini_s_new * ratio;
		  double betag_new=beta_s_new*gamma_s_new;
			  
		  double vec_unit[3];
		  for(int an=0;an<3;an++)
		    {
		      vec_unit[an] = vecin[an]/vecin_abs;
		    }
			  
		  recmass_bg = ini_s_tot/betag;
		  recmass_bg_new = ini_s_tot_new/betag_new;
		  //recmass_bg = p_RK_abs/betag;
		  //			      cout<<pos_afm2[0]<<"   "<<recmass_bg<<" "<<betag<<" "<<beta_s<<" "<<ini_s_tot<<" "<<time_bg[comb]<<endl;
			  
		  h_pos_recmass_bg->Fill(pos_afm2[0],recmass_bg);
			  
#ifdef DEBUGTRACKNEG    
		  cout<<funcname<<" line "<<__LINE__<<endl;
#endif
			  
			  
		  //			      }
		  double RhoMM = ini_rho/1000.;
		  double BetaHigh = 0.747*0.3*RhoMM/sqrt(0.747*RhoMM*0.747*0.3*0.3*RhoMM+
							 0.138*0.138)+0.05;
			  
		  double BetaLow = 0.747*0.3*RhoMM/sqrt(0.747*RhoMM*0.747*0.3*0.3*RhoMM+0.238*0.238)-0.05;
			  
			  
#ifdef DEBUGTRACKNEG    
		  cout<<funcname<<" line "<<__LINE__<<endl;
#endif
			  
		  //		  cout<<"loop  "<<comb<<" "<<__LINE__<<endl;			  
			  
#ifdef RK			  
		  h_rk_comppi[0] ->Fill(p_RK[0],ini_s_tot*vec_unit[0]);
		  //cout<<p_RK[0]<<" "<<ini_s_tot*vec_unit[0]<<endl;
		  h_rk_comppi[1] ->Fill(p_RK[1],ini_s_tot*vec_unit[1]);
		  h_rk_comppi[2] ->Fill(p_RK[2],ini_s_tot*vec_unit[2]);
		  h_rk_comppi[3] ->Fill(p_RK_abs,ini_s_tot);
		  h_rk_comppi[4] ->Fill(p_RK_abs/ini_s_tot,ini_s_tot);
		  //			      h_rk_comppi[5] ->Fill(p_RK_abs/ini_s_tot,(int)best_tofp_tof_id[comb2]);
#endif			  
			  
		  //			  if(beta_s>BetaLow && beta_s<BetaHigh)
#ifdef DEBUGTRACKNEG    
		  cout<<funcname<<" line "<<__LINE__<<" "<<recmass_bg<<endl;
#endif



		  //		  cout<<"loop  "<<comb<<" "<<__LINE__<<endl;

			      
		      THyphiHit_track temp_track;
		      
		      temp_track.px =ini_s_tot*vec_unit[0];
		      temp_track.py =ini_s_tot*vec_unit[1];
		      temp_track.pz =ini_s_tot*vec_unit[2];
		      temp_track.dE=0;
		      temp_track.tof =time_bg[comb];
		      temp_track.beta = beta_s;
		      temp_track.path_length = length_xyz;
		      if(beta_s<1.0)
			temp_track.mass = recmass_bg;
		      else
			temp_track.mass = 0;
		      temp_track.rchi_x = 0;
		      temp_track.rchi_y = chi_bg[comb];

		      temp_track.Pos1x = pos_bfm1p[0];
		      temp_track.Pos1y = pos_bfm1p[1];
		      temp_track.Pos1z = pos_bfm1p[2];
		      temp_track.Index_1x =best_index[0][comb];
		      temp_track.Index_1y =best_index[1][comb];
		      
		      temp_track.Pos2x = pos_bfm2p[0];
		      temp_track.Pos2y = pos_bfm2p[1];
		      temp_track.Pos2z = pos_bfm2p[2];
		      temp_track.Index_2x =best_index[2][comb];
		      temp_track.Index_2y =best_index[3][comb];
		      
		      temp_track.Postx = pos_afm2[0];
		      temp_track.Posty = pos_afm2[1];
		      temp_track.Postz = pos_afm2[2];
		      temp_track.Index_tof =best_index[4][comb];
		      temp_track.TofBarId =id_bg[comb];
		      
		      temp_track.Index_CbMx  = best_index_prin[0][comb];
		      temp_track.Index_CbMxp = best_index_prin[1][comb];
		      temp_track.Index_CbMu  = best_index_prin[2][comb];
		      temp_track.Index_CbMup = best_index_prin[3][comb];
		      temp_track.Index_CbMv  = best_index_prin[4][comb];
		      temp_track.Index_CbMvp = best_index_prin[5][comb];
		      
		      temp_track.Index_CaMx  = usedid_q[0];
		      temp_track.Index_CaMxp = usedid_q[1];
		      temp_track.Index_CaMy  = usedid_q[2];
		      temp_track.Index_CaMyp = usedid_q[3];
		      temp_track.Index_CaMu  = usedid_q[4];
		      
		      temp_track.V_Index_CaMx  = Id_x;
		      temp_track.V_Index_CaMxp = Id_xp;
		      temp_track.V_Index_CaMy  = Id_y;
		      temp_track.V_Index_CaMyp = Id_yp;
		      temp_track.V_Index_CaMu  = Id_u;
		      
		      Event_Track_Out->Add_Track(temp_track,"Pi");

		      NumNewPi++;
		      //		      cout<<"loop  "<<comb<<" "<<__LINE__<<endl;
		  if(recmass_bg>0.05 && recmass_bg<0.5)
		    {
		      //if(PionTrack.size()>200)continue;


#ifdef DEBUGTRACKNEG    
		      cout<<funcname<<" line "<<__LINE__<<" "<<recmass_bg<<endl;
#endif



#ifdef RK
		      PionTrack.push_back(TVector3(p_RK[0],
						   p_RK[1],
						   p_RK[2]));
#else

		      PionTrack.push_back(TVector3(ini_s_tot*vec_unit[0],
						   ini_s_tot*vec_unit[1],
						   ini_s_tot*vec_unit[2]));
#endif			      
#ifdef DEBUGTRACKNEG    
		  cout<<funcname<<" line "<<__LINE__<<" "<<recmass_bg<<endl;
#endif


		      PionTrackPosX.push_back(TVector2(best_bg_px[comb].X(),best_bg_px[comb].Y()));
		      PionTrackPosY.push_back(TVector2(best_bg_py[comb].X(),best_bg_py[comb].Y()));
		      PionTrackChi.push_back(chi_bg[comb]);
#ifdef DEBUGTRACKNEG    
		  cout<<funcname<<" line "<<__LINE__<<endl;
#endif
		      
#ifdef DEBUGTRACKNEG    
		  cout<<funcname<<" line "<<__LINE__<<endl;
#endif

			      
// 		      //if(NumNewPi<200)
// 		      if(NumNewPi>200) continue;
// 		      {
// #ifdef INVMASSNEW				
// 			Inv.P1xS[NumNewPi] = best_bg_px[comb].X();
// 			Inv.P2xS[NumNewPi] = best_bg_px[comb].Y();
// 			Inv.P1yS[NumNewPi] = best_bg_py[comb].X();
// 			Inv.P2yS[NumNewPi] = best_bg_py[comb].Y();

// 			Inv.TR0VetoS[NumNewPi] = pos_bg_tr0veto[comb];
// #ifdef RK
// 			Inv.rhoS[NumNewPi] = p_RK_abs;//ini_s_tot;
// #else
// 			Inv.rhoS[NumNewPi] = ini_s_tot;
// 			Inv.rhoSnew[NumNewPi] = ini_s_tot_new;
// #endif

// 			Inv.betaS[NumNewPi] = beta_s;
// 			Inv.betaSnew[NumNewPi] = beta_s_new;
// 			Inv.chi2S[NumNewPi] = chi_bg[comb];
// 			Inv.Pos_bfm1Sx[NumNewPi] = pos_bfm1p[0];
// 			Inv.Pos_bfm1Sy[NumNewPi] = pos_bfm1p[1];
// 			Inv.Pos_bfm1Sz[NumNewPi] = pos_bfm1p[2];
// 			/////////////////
// 			Inv.Pos_bfm2Sx[NumNewPi] = pos_bfm2p[0];
// 			Inv.Pos_bfm2Sy[NumNewPi] = pos_bfm2p[1];
// 			Inv.Pos_bfm2Sz[NumNewPi] = pos_bfm2p[2];
// 			/////////////////
// 			Inv.Pos_afm1Sx[NumNewPi] =0;
// 			Inv.Pos_afm1Sy[NumNewPi] =0;
// 			Inv.Pos_afm1Sz[NumNewPi] =0;
// 			/////////////////
// 			Inv.Pos_afm2Sx[NumNewPi] = pos_afm2[0];
// 			Inv.Pos_afm2Sy[NumNewPi] = pos_afm2[1];
// 			Inv.Pos_afm2Sz[NumNewPi] = pos_afm2[2];
// 			/////////////////
				  
// #ifdef RK
// 			Inv.MomSx[NumNewPi]=p_RK[0];
// 			Inv.MomSy[NumNewPi]=p_RK[1];
// 			Inv.MomSz[NumNewPi]=p_RK[2];
// #else
// 			Inv.MomSx[NumNewPi]=ini_s_tot*vec_unit[0];
// 			Inv.MomSy[NumNewPi]=ini_s_tot*vec_unit[1];
// 			Inv.MomSz[NumNewPi]=ini_s_tot*vec_unit[2];

// #endif //RK
// 			Inv.MomSxnew[NumNewPi]=ini_s_tot_new*vec_unit[0];
// 			Inv.MomSynew[NumNewPi]=ini_s_tot_new*vec_unit[1];
// 			Inv.MomSznew[NumNewPi]=ini_s_tot_new*vec_unit[2];

// 			Inv.Ind1xS[NumNewPi] = best_index[0][comb];
// 			Inv.Ind1yS[NumNewPi] = best_index[1][comb];
// 			Inv.Ind2xS[NumNewPi] = best_index[2][comb];
// 			Inv.Ind2yS[NumNewPi] = best_index[3][comb];
// 			Inv.IndTofS[NumNewPi]= best_index[4][comb];
// 			Inv.PrinCoinS[NumNewPi] = best_princoin[comb];
// #endif // INVMASSNEW
// 		      }
		  //NumNewPi++;
		    }
			      

#ifdef DEBUGTRACKNEG    
		      cout<<funcname<<" line "<<__LINE__<<" "<<recmass_bg<<endl;
#endif


		  
		  if(chi_bg[comb]<best_temp_bg[id_bg[comb]])
		    {
		      best_temp_bg[id_bg[comb]]=chi_bg[comb];
		      best_rho_bg[id_bg[comb]]=ini_rho;
		      best_rho_bg_new[id_bg[comb]]=ini_rho_new;
#ifdef RK				  
		      best_poq_bg[id_bg[comb]]=p_RK_abs;//ini_s_tot;
#else
		      best_poq_bg[id_bg[comb]]=ini_s_tot;
#endif //RK

		      best_beta_bg[id_bg[comb]]=beta_s;
		      best_beta_bg_new[id_bg[comb]]=beta_s_new;
		      best_ene_bg[id_bg[comb]]=ene_bg[comb];
		      best_mass_bg[id_bg[comb]] = recmass_bg;
		      best_pos_scx[id_bg[comb]] = pos_bg_scx[comb];
		      best_pos_scy[id_bg[comb]] = pos_bg_scy[comb];
		      best_postx_bg[id_bg[comb]] = pos_bg_tx[comb];
		      best_posty_bg[id_bg[comb]] = pos_bg_ty[comb];
#ifdef RK
		      best_momx_bg[id_bg[comb]] = p_RK[0];
		      best_momy_bg[id_bg[comb]] = p_RK[1];
		      best_momz_bg[id_bg[comb]] = p_RK[2];
#else
		      best_momx_bg[id_bg[comb]] = ini_s_tot*vec_unit[0];
		      best_momy_bg[id_bg[comb]] = ini_s_tot*vec_unit[1];
		      best_momz_bg[id_bg[comb]] = ini_s_tot*vec_unit[2];
#endif //RK
		      best_p1x_bg[id_bg[comb]] = best_bg_px[comb].X();
		      best_p2x_bg[id_bg[comb]] = best_bg_px[comb].Y();
		      best_p1y_bg[id_bg[comb]] = best_bg_py[comb].X();
		      best_p2y_bg[id_bg[comb]] = best_bg_py[comb].Y();

		      best_pos_bg_y[id_bg[comb]]=pos_afm2[1];
		      best_pos_bg_x[id_bg[comb]]=pos_afm2[0];
		      best_pos_bg_x_atAl[id_bg[comb]]= pos_atAl[0];
		      best_track_queen[id_bg[comb]]=hoge2;
		      best_track_prin[id_bg[comb]]=best_princoin[comb];
		      if(positive_flag)
			best_charge[id_bg[comb]]=1;
		      else
			best_charge[id_bg[comb]]=0;
		      // best_bar_bg=id_bg[comb];

		      double cutfuncl = TimeZeroBar*3.0-18.0;
		      double cutfunch = TimeZeroBar*3.0-12.0;
		      
		      if(best_bg_px[comb].Y()>cutfuncl &&best_bg_px[comb].Y()<cutfunch)
			best_coin_tofs[id_bg[comb]]=1;
		      else
			best_coin_tofs[id_bg[comb]]=0;

		      for(int index=0;index<4;index++)
			{
			  duplid[index][id_bg[comb]] = best_index[index][comb];
			}

		      for(int index=0;index<4;index++)
			{
			  duplid[index][id_bg[comb]] = best_index[index][comb];
			}




		    }
		  //			      cout<<ini_rho<<" "<<beta_s<<endl;
		  //			      h_inv_rho_pos[1]->Fill(ini_rho_s_old/1000.,pos_afm2s[0]);
#ifdef DEBUGTRACKNEG    
		  cout<<funcname<<" line "<<__LINE__<<endl;
#endif
		}
	    }
	}
      //	    }



    }//loop for pos_bg




 // for(int i=0;i<18;i++)
 //    {
 //      //      cout<<funcname<<__LINE__<<endl;
 //    if(best_rho_bg[i]>0)
 // 	{

	  
 // 	  {	      
 // 	    THyphiHit_new temp_x_hit;
 // 	    Event_Out->Get_Hit(duplid[0][i],"Tr1x",temp_x_hit);
 // 	    int usedid_min = temp_x_hit.Id_x[0];
 // 	    int usedid_max = temp_x_hit.Id_x[ temp_x_hit.Id_x.size()-1  ];
 // 	    for(int nh=0;nh<Event_Out->n_Tr1x;nh++)
 // 	      {
 // 		if(nh!=duplid[0][i])
 // 		  {
 // 		    Event_Out->Get_Hit(nh,"Tr1x",temp_x_hit);
 // 		    int tempid_min = temp_x_hit.Id_x[0];
 // 		    int tempid_max = temp_x_hit.Id_x[ temp_x_hit.Id_x.size()-1 ];
 // 		    if(abs(tempid_min - usedid_min)<3 || abs(tempid_max - usedid_max)<3 ||
 // 		       abs(tempid_min - usedid_max)<3 || abs(tempid_max - usedid_min)<3)
 // 		      duplflag[i]=true;
 // 		  }
		  
 // 	      }
 // 	  }   
 // 	  //	  cout<<funcname<<__LINE__<<endl;
 // 	  {	      
 // 	    THyphiHit_new temp_x_hit;
 // 	    Event_Out->Get_Hit(duplid[2][i],"Tr2x",temp_x_hit);
 // 	    int usedid_min = temp_x_hit.Id_x[0];
 // 	    int usedid_max = temp_x_hit.Id_x[ temp_x_hit.Id_x.size()-1  ];
 // 	    for(int nh=0;nh<Event_Out->n_Tr2x;nh++)
 // 	      {
 // 		if(nh!=duplid[2][i])
 // 		  {
 // 		    Event_Out->Get_Hit(nh,"Tr2x",temp_x_hit);
 // 		    int tempid_min = temp_x_hit.Id_x[0];
 // 		    int tempid_max = temp_x_hit.Id_x[ temp_x_hit.Id_x.size()-1 ];
 // 		    if(abs(tempid_min - usedid_min)<3 || abs(tempid_max - usedid_max)<3||
 // 		       abs(tempid_min - usedid_max)<3 || abs(tempid_max - usedid_min)<3)
 // 		      duplflag[i]=true;
 // 		  }
		  
 // 	      }
 // 	  }   

 // 	  //	  cout<<funcname<<__LINE__<<endl;
 // 	  {	      
 // 	    THyphiHit_new temp_y_hit;
 // 	    Event_Out->Get_Hit(duplid[1][i],"Tr1y",temp_y_hit);
 // 	    int usedid_min = temp_y_hit.Id_y[0];
 // 	    int usedid_max = temp_y_hit.Id_y[ temp_y_hit.Id_x.size()-1  ];
 // 	    for(int nh=0;nh<Event_Out->n_Tr1y;nh++)
 // 	      {
 // 		if(nh!=duplid[1][i])
 // 		  {
 // 		    Event_Out->Get_Hit(nh,"Tr1y",temp_y_hit);
 // 		    int tempid_min = temp_y_hit.Id_y[0];
 // 		    int tempid_max = temp_y_hit.Id_y[ temp_y_hit.Id_y.size()-1 ];
 // 		    if(abs(tempid_min - usedid_min)<3 || abs(tempid_max - usedid_max)<3||
 // 		       abs(tempid_min - usedid_max)<3 || abs(tempid_max - usedid_min)<3)
 // 		      duplflag[i]=true;
 // 		  }
		  
 // 	      }
 // 	  }   
 // 	  //cout<<funcname<<__LINE__<<endl;
 // 	  {	      
 // 	    THyphiHit_new temp_y_hit;
 // 	    Event_Out->Get_Hit(duplid[3][i],"Tr2y",temp_y_hit);
 // 	    int usedid_min = temp_y_hit.Id_y[0];
 // 	    int usedid_max = temp_y_hit.Id_y[ temp_y_hit.Id_x.size()-1  ];
 // 	    for(int nh=0;nh<Event_Out->n_Tr2y;nh++)
 // 	      {
 // 		if(nh!=duplid[3][i])
 // 		  {
 // 		    Event_Out->Get_Hit(nh,"Tr2y",temp_y_hit);
 // 		    int tempid_min = temp_y_hit.Id_y[0];
 // 		    int tempid_max = temp_y_hit.Id_y[ temp_y_hit.Id_y.size()-1 ];
 // 		    if(abs(tempid_min - usedid_min)<3 || abs(tempid_max - usedid_max)<3||
 // 		       abs(tempid_min - usedid_max)<3 || abs(tempid_max - usedid_min)<3)
 // 		      duplflag[i]=true;
 // 		  }
		  
 // 	      }
 // 	  }   
	      
 // 	}
 //    }









  for(int i=0;i<18;i++)
    if(best_rho_bg[i]>0)
      {

	h_rho_beta_bg_v[0]->Fill(best_rho_bg[i]/1000.,best_beta_bg[i]);
	h_mass_bg_best_v[0]->Fill(best_mass_bg[i]);
	h_mass_bg_best_bar_v[0]->Fill(best_mass_bg[i],i);
	if(best_coin_tofs[i]==1)
	  {
	    h_rho_beta_bg_v[1]->Fill(best_rho_bg[i]/1000.,best_beta_bg[i]);
	    h_mass_bg_best_v[1]->Fill(best_mass_bg[i]);
	    h_mass_bg_best_bar_v[1]->Fill(best_mass_bg[i],i);
	  }
	if(!duplflag[i])
	  {
	    h_rho_beta_bg_v[2]->Fill(best_rho_bg[i]/1000.,best_beta_bg[i]);
	    h_mass_bg_best_v[2]->Fill(best_mass_bg[i]);
	    h_mass_bg_best_bar_v[2]->Fill(best_mass_bg[i],i);
	  }
	if(!duplflag[i]&&best_coin_tofs[i]==1)
	  {
	    h_rho_beta_bg_v[3]->Fill(best_rho_bg[i]/1000.,best_beta_bg[i]);
	    h_mass_bg_best_v[3]->Fill(best_mass_bg[i]);
	    h_mass_bg_best_bar_v[3]->Fill(best_mass_bg[i],i);
	  }

	h_rho_beta_bg_best[i]->Fill(best_rho_bg[i]/1000.,best_beta_bg[i]);
	h_rho_beta_bg_best_all->Fill(best_rho_bg[i]/1000.,best_beta_bg[i]);
	h_rho_beta_bg_best_all_new->Fill(best_rho_bg_new[i]/1000.,best_beta_bg_new[i]);
	h_poq_beta_bg_best_all->Fill(best_poq_bg[i],best_beta_bg[i]);

	
	for(int j=0;j<Event_Out->n_TofAladin;j++)
	  {
	    THyphiHit_new temp_aladin;
	    Event_Out->Get_Hit(j,"TofAladin",temp_aladin);
	    h_PosCor_LandAladinXX_aftTrack->Fill(best_pos_bg_x[i],temp_aladin.x);

	    h_PosCor_LandAladinXX_aftTrack_atAl->Fill(best_pos_bg_x_atAl[i],temp_aladin.x);
	    h_PosCor_LandAladinXX_aftTrack_atAl1D->Fill(best_pos_bg_x_atAl[i]-temp_aladin.x);
	    if(best_track_queen[i])
	      {
		h_PosCor_LandAladinXX_aftTrack_wc->Fill(best_pos_bg_x[i],temp_aladin.x);
		h_PosCor_LandAladinXX_aftTrack_atAl1D_wc->Fill(best_pos_bg_x_atAl[i]-temp_aladin.x);
		if(best_track_prin[i])
		  {
		    h_PosCor_LandAladinXX_aftTrack_wc_wp->Fill(best_pos_bg_x[i],temp_aladin.x);
		    h_PosCor_LandAladinXX_aftTrack_atAl1D_wc_wp->Fill(best_pos_bg_x_atAl[i]-temp_aladin.x);
		  }

	      }
	    h_PosCor_LandAladinYY_aftTrack->Fill(best_pos_bg_y[i],temp_aladin.y);
	    //if(temp_aladin.x>best_pos_bg_x[i]*5.0/4.0-250 && temp_aladin.x<best_pos_bg_x[i]*5.0/4.0)
	    if(fabs(best_pos_bg_x_atAl[i]-temp_aladin.x)<100)
	      {
		h_PosCor_LandAladinXX_aftTrack_cut1->Fill(best_pos_bg_x[i],temp_aladin.x);
		h_rho_beta_bg_best_all_alcoin1->Fill(best_rho_bg[i]/1000.,best_beta_bg[i]);
		if(best_track_queen[i])
		  {
		  h_rho_beta_bg_best_all_alcoin1_wc->Fill(best_rho_bg[i]/1000.,best_beta_bg[i]);
		  if(best_track_prin[i])
		    {
		      h_rho_beta_bg_best_all_alcoin1_wc_wp->Fill(best_rho_bg[i]/1000.,best_beta_bg[i]);
		    }
		  }





		
		if(best_coin_tofs[i]==1)
		  {
		    h_rho_beta_bg_v[4]->Fill(best_rho_bg[i]/1000.,best_beta_bg[i]);
		    h_mass_bg_best_v[4]->Fill(best_mass_bg[i]);
		    h_mass_bg_best_bar_v[4]->Fill(best_mass_bg[i],i);
		  }
		if(!duplflag[i])
		  {
		    h_rho_beta_bg_v[5]->Fill(best_rho_bg[i]/1000.,best_beta_bg[i]);
		    h_mass_bg_best_v[5]->Fill(best_mass_bg[i]);
		    h_mass_bg_best_bar_v[5]->Fill(best_mass_bg[i],i);
		  }
		if(!duplflag[i]&&best_coin_tofs[i]==1)
		  {
		    h_rho_beta_bg_v[6]->Fill(best_rho_bg[i]/1000.,best_beta_bg[i]);
		    h_mass_bg_best_v[6]->Fill(best_mass_bg[i]);
		    h_mass_bg_best_bar_v[6]->Fill(best_mass_bg[i],i);
		  }

		if(!duplflag[i]&&best_coin_tofs[i]==1&&best_track_prin[i])
		  {
		    h_rho_beta_bg_v[7]->Fill(best_rho_bg[i]/1000.,best_beta_bg[i]);
		    h_mass_bg_best_v[7]->Fill(best_mass_bg[i]);
		    h_mass_bg_best_bar_v[7]->Fill(best_mass_bg[i],i);
		  }

		h_rho_beta_bg_v[8]->Fill(best_rho_bg[i]/1000.,best_beta_bg[i]);
		h_mass_bg_best_v[8]->Fill(best_mass_bg[i]);
		h_mass_bg_best_bar_v[8]->Fill(best_mass_bg[i],i);
		if(best_track_prin[i])
		  {
		    h_rho_beta_bg_v[9]->Fill(best_rho_bg[i]/1000.,best_beta_bg[i]);
		    h_mass_bg_best_v[9]->Fill(best_mass_bg[i]);
		    h_mass_bg_best_bar_v[9]->Fill(best_mass_bg[i],i);
		  }


	      }
	    //else if(temp_aladin.x>best_pos_bg_x[i]*5.0/4.0+200 && temp_aladin.x<best_pos_bg_x[i]*6.0/4.0+500)
	    if(best_pos_bg_x_atAl[i]-temp_aladin.x>-700 &&best_pos_bg_x_atAl[i]-temp_aladin.x<-200 )
	      {
		h_PosCor_LandAladinXX_aftTrack_cut2->Fill(best_pos_bg_x[i],temp_aladin.x);
		h_rho_beta_bg_best_all_alcoin2->Fill(best_rho_bg[i]/1000.,best_beta_bg[i]);
		if(best_track_queen[i])
		  {
		    h_rho_beta_bg_best_all_alcoin2_wc->Fill(best_rho_bg[i]/1000.,best_beta_bg[i]);
		  if(best_track_prin[i])
		    {
		      h_rho_beta_bg_best_all_alcoin2_wc_wp->Fill(best_rho_bg[i]/1000.,best_beta_bg[i]);
		    }
		  }
	      }
	      
	  }
	// 		if(best_charge[i]==1)
	// 		  {
	// 		    h_rho_beta_bg_best_positive[i]->Fill(best_rho_bg[i]/1000.,best_beta_bg[i]);
	// 		    h_rho_beta_bg_best_all_positive->Fill(best_rho_bg[i]/1000.,best_beta_bg[i]);
	// 		  }
	//		h_rho_ene_bg_best[i]->Fill(best_rho_bg[i]/1000.,best_ene_bg[i]);
	h_mass_bg_best_all->Fill(best_mass_bg[i]);
	// 		if(best_beta_bg[i]>0.9)
	// 		  {
	// 		    PionTrack.push_back(TVector3(best_momx_bg[i],
	// 						 best_momy_bg[i],
	// 						 best_momz_bg[i]));
	// 		    PionTrackPosX.push_back(TVector2(best_p1x_bg[i],best_p2x_bg[i]));
	// 		    PionTrackPosY.push_back(TVector2(best_p1y_bg[i],best_p2y_bg[i]));
	// 		    PionTrackChi.push_back(best_temp_bg[i]);
	// 		  }
	h_recmass_targetpos_bg->Fill(best_postx_bg[i],best_posty_bg[i]);

	//  		for(int eunhee=0;eunhee<ChambPos.size();eunhee++)
	//  		  {
	//  		    h_arect_prinx->Fill(best_pos_scx[i],ChambPos[eunhee].X());
	//  		    h_arect_priny->Fill(best_pos_scy[i],ChambPos[eunhee].Y());
	// 		    if(fabs(best_pos_scx[i]-ChambPos[eunhee].X())<5)
	// 		      h_arect_priny_x->Fill(best_pos_scy[i],ChambPos[eunhee].Y());
	//  		  }
	//		cout<<Event_Chamber->n_CbM<<endl;
// 	for(int CbM=0;CbM<Event_Chamber->n_CbM;CbM++)
// 	  {
// 	    temp_cham.Clear();
// 	    THyphiHit_new temp_cham_hit;
// 	    Event_Chamber->Get_Channel(CbM,"CbM",temp_cham);
// 	    //		    cout<<" "<<CbM<<endl;
// 	    h_arect_prinx_old->Fill(best_pos_scx[i],temp_cham.X);
// 	    h_arect_priny_old->Fill(best_pos_scy[i],temp_cham.Y);
// 	    if(fabs(best_pos_scx[i]-temp_cham.X)<5)
// 	      h_arect_priny_x_old->Fill(best_pos_scy[i],temp_cham.Y);
		    
// 	  }



      }





  //	  if(pos_albg[0].size()==1)
  //for(size_t comb=0;comb<pos_albg[0].size();comb++)
  for(size_t comb=0;comb<0;comb++)
    //	  for(size_t comb=0;comb<0;comb++)
    {

      double pos_bfm1p[3]; //tr1
      double pos_bfm2p[3]; //tr2
	      
      double pos_afm2[3]; //bigtof
      double pos_afm1[3]; //aladin
	      
      MT_Input_to_3Vector(pos_albg[1][comb].X(),
			  pos_albg[1][comb].Y(),
			  pos_albg[1][comb].Z(),pos_bfm1p);
	      
      MT_Input_to_3Vector(pos_albg[2][comb].X(),
			  pos_albg[2][comb].Y(),
			  pos_albg[2][comb].Z(),pos_bfm2p);
	      
      // 3 : BigTof
      // 4 : aladin
      MT_Input_to_3Vector(pos_albg[4][comb].X(),
			  pos_albg[3][comb].Y(),
			  pos_albg[4][comb].Z(),pos_afm2);
      // 	      MT_Input_to_3Vector(pos_albg[3][comb].X(),
      // 				  pos_albg[3][comb].Y(),
      // 				  pos_albg[3][comb].Z(),pos_afm2);

      MT_Input_to_3Vector(pos_albg[4][comb].X(),
			  pos_albg[4][comb].Y(),
			  pos_albg[4][comb].Z(),pos_afm1);


      double center_temp[2] ={0.0, 0.0};
      double pos_ent_temp[2]={0.0, 0.0};
      double pos_exi_temp[2]={0.0, 0.0};
	      
      double pos_exi_temp2[3]={0.0,0.0, 0.0};
	      
      //double center[3] ={0.0, 0.0, 0.0};
      double pos_ent[3]={0.0, 0.0, 0.0};
      double pos_exi[3]={0.0, 0.0, 0.0};
	      
      double ini_rho;//, ini_p;
	      
	      
	      
      //	   if(pos_bfm2p[0]-pos_bfm1p[0]<0.0)continue;
	      
      //    	      ini_rho = GetRho(pos_bfm1p[0],pos_bfm1p[2],
      //   			       pos_bfm2p[0],pos_bfm2p[2],
      //  			       pos_afm2[0],pos_afm2[2],
      //   			       pos_afm1[0],pos_afm1[2],
      //   			       0,pos_ent_temp,pos_exi_temp,
      //   			       center_temp);

//       ini_rho = GetRho(pos_bfm1p[0],pos_bfm1p[2],
// 		       pos_bfm2p[0],pos_bfm2p[2],
// 		       pos_afm2[0],pos_afm2[2],
// 		       0,pos_ent_temp,pos_exi_temp,
// 		       center_temp);

      double length_xyz=0;
      ini_rho = GetRho(-1,pos_bfm1p[0],pos_bfm1p[2],pos_bfm2p[0],pos_bfm2p[2],
		       pos_afm2[0],pos_afm2[1],pos_afm2[2],length_xyz,
		       pos_ent_temp,pos_exi_temp,
 		       center_temp,pos_albg[3][comb].Z());//m
      // 	      ini_rho = GetRho(-1,pos_bfm1p[0],pos_bfm1p[2],pos_bfm2p[0],pos_bfm2p[2],
      // 			       pos_afm2[0],pos_afm2[1],pos_afm2[2],length_xyz);//m

      //bool BigChambCoin = false;
	      
      MT_Line line_rec;
      line_rec.From_Points(pos_bfm1p,pos_bfm2p);
	      
      pos_ent[2]=pos_ent_temp[1];
      pos_exi_temp2[2]=pos_exi_temp[1];
	      
      if(line_rec.Get_XY_atZ(pos_ent)==1)
	{
	  if(line_rec.Get_XY_atZ(pos_exi_temp2)==1)
	    {
		      
	      pos_exi[0] = pos_exi_temp[0];
	      pos_exi[1] = pos_exi_temp2[1];
	      pos_exi[2] = pos_exi_temp[1];
		      
	      MT_Line line_ins;
	      line_ins.From_Points(pos_ent,pos_exi);
		      
	      double inter[3]={0.0, 0.0, 0.0};
	      inter[2]=center_temp[1];
	      if(line_ins.Get_XY_atZ(inter)==1)
		{


		  double center[3]={0,0,0,};
		  center[0] = center_temp[0];
		  center[1] = inter[1];
		  center[2] = center_temp[1];

#ifdef DEBUGTRACKNEG    
		  cout<<funcname<<" line "<<__LINE__<<endl;
#endif


		  bool coinFlag[4]={false,false,false,false};			  

		  MT_Line line_afm;
		  line_afm.From_Points(pos_exi,pos_afm2);
		  double qpos[3] = {0,0,0};
			  


// 		  qpos[2] = QueenzPos+QueenPlOffs[0];
// 		  if(line_afm.Get_XY_atZ(qpos)==1)
// 		    {
// 		      for(int cam=0;cam<Event_Out->n_CaMx ;cam++)
// 			{
// 			  THyphiHit_new temp_cham_hit;
// 			  Event_Out->Get_Hit(cam,"CaMx",temp_cham_hit);
// 			  double id_cal = GetXidQ(qpos[0],qpos[1]);
// 			  for(size_t ss =0;ss<temp_cham_hit.Id_x.size();ss++)
// 			    {
// 			      h_track_id_coin_que_x_albt[trig_f]->Fill(temp_cham_hit.Id_x[ss],id_cal);
// 			      if(fabs(temp_cham_hit.Id_x[ss]-id_cal)<3)
// 				coinFlag[0]=true;
// 			    }
			  
// 			}
// 		    }
// 		  qpos[2] = QueenzPos+QueenPlOffs[1];
// 		  if(line_afm.Get_XY_atZ(qpos)==1)
// 		    {
// 		      for(int cam=0;cam<Event_Out->n_CaMxp ;cam++)
// 			{
// 			  THyphiHit_new temp_cham_hit;
// 			  Event_Out->Get_Hit(cam,"CaMxp",temp_cham_hit);
// 			  double id_cal = GetXpidQ(qpos[0],qpos[1]);
// 			  for(size_t ss =0;ss<temp_cham_hit.Id_x.size();ss++)
// 			    {
// 			      h_track_id_coin_que_xp_albt[trig_f]->Fill(temp_cham_hit.Id_x[ss],id_cal);
// 			      if(fabs(temp_cham_hit.Id_x[ss]-id_cal)<3)
// 				coinFlag[1]=true;
			      
// 			    }
			  
			  
// 			}
// 		    }
// 		  qpos[2] = QueenzPos+QueenPlOffs[2];
// 		  if(line_afm.Get_XY_atZ(qpos)==1)
// 		    {

// 		      for(int cam=0;cam<Event_Out->n_CaMy ;cam++)
// 			{
// 			  THyphiHit_new temp_cham_hit;
// 			  Event_Out->Get_Hit(cam,"CaMy",temp_cham_hit);
// 			  double id_cal = GetYidQ(qpos[0],qpos[1]);
// 			  for(size_t ss =0;ss<temp_cham_hit.Id_x.size();ss++)
// 			    {
// 			      h_track_id_coin_que_y_albt[trig_f]->Fill(temp_cham_hit.Id_x[ss],id_cal);
// 			      if(fabs(temp_cham_hit.Id_x[ss]-id_cal)<3)
// 				coinFlag[2]=true;

// 			    }

				  
// 			}
// 		    }
// 		  qpos[2] = QueenzPos+QueenPlOffs[3];
// 		  if(line_afm.Get_XY_atZ(qpos)==1)
// 		    {

// 		      for(int cam=0;cam<Event_Out->n_CaMyp ;cam++)
// 			{
// 			  THyphiHit_new temp_cham_hit;
// 			  Event_Out->Get_Hit(cam,"CaMyp",temp_cham_hit);
// 			  double id_cal = GetYpidQ(qpos[0],qpos[1]);
// 			  for(size_t ss =0;ss<temp_cham_hit.Id_x.size();ss++)
// 			    {
// 			      h_track_id_coin_que_yp_albt[trig_f]->Fill(temp_cham_hit.Id_x[ss]*1.02,id_cal);
// 			      if(fabs(temp_cham_hit.Id_x[ss]-id_cal)<3)
// 				coinFlag[3]=true;

// 			    }
				  
// 			}
// 		    }
// 		  qpos[2] = QueenzPos+QueenPlOffs[0];
// 		  if(line_afm.Get_XY_atZ(qpos)==1)
// 		    {

// 		      if(coinFlag[2]&&coinFlag[3]&&coinFlag[0])
// 			for(int cam=0;cam<Event_Out->n_CaMxp ;cam++)
// 			  {
// 			    THyphiHit_new temp_cham_hit;
// 			    Event_Out->Get_Hit(cam,"CaMxp",temp_cham_hit);
// 			    double id_cal = GetXidQ(qpos[0],qpos[1]);
// 			    for(size_t ss =0;ss<temp_cham_hit.Id_x.size();ss++)
// 			      {
// 				h_track_id_coin_que_xp_albt_coin->Fill(temp_cham_hit.Id_x[ss],id_cal);

// 			      }
				    
// 			  }
// 		    }
// 		  qpos[2] = QueenzPos+QueenPlOffs[1];
// 		  if(line_afm.Get_XY_atZ(qpos)==1)
// 		    {

// 		      if(coinFlag[2]&&coinFlag[3]&&coinFlag[1])
// 			for(int cam=0;cam<Event_Out->n_CaMx ;cam++)
// 			  {
// 			    THyphiHit_new temp_cham_hit;
// 			    Event_Out->Get_Hit(cam,"CaMx",temp_cham_hit);
// 			    double id_cal = GetXidQ(qpos[0],qpos[1]);
// 			    for(size_t ss =0;ss<temp_cham_hit.Id_x.size();ss++)
// 			      {
// 				h_track_id_coin_que_x_albt_coin->Fill(temp_cham_hit.Id_x[ss],id_cal);
// 			      }
				    
// 			  }
// 		    }
// 		  qpos[2] = QueenzPos+QueenPlOffs[2];
// 		  if(line_afm.Get_XY_atZ(qpos)==1)
// 		    {


// 		      if(coinFlag[0]&&coinFlag[1]&&coinFlag[2])
// 			for(int cam=0;cam<Event_Out->n_CaMyp ;cam++)
// 			  {
// 			    THyphiHit_new temp_cham_hit;
// 			    Event_Out->Get_Hit(cam,"CaMyp",temp_cham_hit);
// 			    double id_cal = GetYpidQ(qpos[0],qpos[1]);
// 			    for(size_t ss =0;ss<temp_cham_hit.Id_x.size();ss++)
// 			      {
// 				h_track_id_coin_que_yp_albt_coin->Fill(temp_cham_hit.Id_x[ss],id_cal);

// 			      }
				    
// 			  }
// 		    }
// 		  qpos[2] = QueenzPos+QueenPlOffs[3];
// 		  if(line_afm.Get_XY_atZ(qpos)==1)
// 		    {

// 		      if(coinFlag[0]&&coinFlag[1]&&coinFlag[3])
// 			for(int cam=0;cam<Event_Out->n_CaMy ;cam++)
// 			  {
// 			    THyphiHit_new temp_cham_hit;
// 			    Event_Out->Get_Hit(cam,"CaMy",temp_cham_hit);
// 			    double id_cal = GetYidQ(qpos[0],qpos[1]);
// 			    for(size_t ss =0;ss<temp_cham_hit.Id_x.size();ss++)
// 			      {
// 				h_track_id_coin_que_y_albt_coin->Fill(temp_cham_hit.Id_x[ss],id_cal);
// 			      }
				    
// 			  }

// 		    }


#ifdef DEBUGTRACKNEG    
		  cout<<funcname<<" line "<<__LINE__<<endl;
#endif









		      double slopex = (pos_afm2[0]-pos_exi[0])/(pos_afm2[2]-pos_exi[2]);
		      double intx = pos_afm2[0] - slopex*pos_afm2[2];
			      
		      double slopey = (pos_afm2[1]-pos_exi[1])/(pos_afm2[2]-pos_exi[2]);
		      double inty = pos_afm2[1] - slopey*pos_afm2[2];
		      bool coin[6] ={false,false,false,false,false,false};
		      double posx[5],posz[5];
		      int usedid_q[5]={-1,-1,-1,-1,-1};
		      // bool hoge2=CheckChambHitQueenIdPHP(-2,slopex,intx,
		      // 					 slopey,inty,
		      // 					 Event_Out,posx,posz,coin,usedid_q);
		      // bool hoge_coin=CheckChambHitQueenIdPHP(-4,slopex,intx,
		      // 					     slopey,inty,
		      // 					     Event_Out,posx,posz,coin,usedid_q);
			      
			      
			      
			      
			      

		      
		      //if(!hoge2)continue;
		      //lkkkkkkkkkkk
		      double naiseki_s=0.0;
		      double abs1_s=0.0;
		      double abs2_s=0.0;
		      double plength_s[3]={0.0,0.0,0.0};
			      
		      for(int an=0;an<3;an++)
			{
			  naiseki_s += (pos_ent[an]-center[an]) *
			    (pos_exi[an]-center[an]);
				  
			  abs1_s += (pos_ent[an]-center[an])*
			    (pos_ent[an]-center[an]);
				  
			  abs2_s += (pos_exi[an]-center[an])*
			    (pos_exi[an]-center[an]);
				  
			  plength_s[0] +=pos_ent[an]*pos_ent[an];
			  plength_s[2] +=(pos_afm2[an]-pos_exi[an])*
			    (pos_afm2[an]-pos_exi[an]);
				  
			}
			      
		      abs1_s = sqrt(abs1_s);
		      abs2_s = sqrt(abs2_s);
		      plength_s[0] = sqrt(plength_s[0]);
		      plength_s[2] = sqrt(plength_s[2]);
			      
			      
			      
		      double inan_s = TMath::ACos(naiseki_s/(abs1_s*abs2_s));
		      plength_s[1] = ini_rho*inan_s;
		      //		       cout<<abs1<<" abs2="<<abs2<<" ian"<<inan<<" ini_rho"<<ini_rho<<endl;
		      //cout<<"Lenght 1="<<plength[0]<<" 2="<<plength[1]<<" 3="<<plength[2]<<endl;
			      
			      
			      
		      //double PathLength_s = plength_s[0]+plength_s[1]+plength_s[2];
			      
			      
			      
		      //double beta_s= PathLength_s/time_albg[comb]/300.;
		      double beta_s= length_xyz/time_albg[comb]/300.;
		      //kkkkkkkkkkkkkk
		      h_rho_beta_albg[id_albg[comb]]->Fill(ini_rho/1000.,beta_s);

		      if(chi_albg[comb]<best_temp_albg[id_albg[comb]])
			{
			  best_temp_albg[id_albg[comb]]=chi_albg[comb];
			  best_rho_albg[id_albg[comb]]=ini_rho;
			  best_beta_albg[id_albg[comb]]=beta_s;
			  // best_bar_bg=id_bg[comb];
			}


		      //		    }

		}
	    }
	}



    }//loop for pos_albg
  for(int i=0;i<18;i++)
    if(best_rho_albg[i]>0)
      {
	h_rho_beta_albg_best[i]->Fill(best_rho_albg[i]/1000.,best_beta_albg[i]);
	h_rho_beta_albg_best_all->Fill(best_rho_albg[i]/1000.,best_beta_albg[i]);
      }

  return  NumNewPi;//PionTrack.size();
	  

}
