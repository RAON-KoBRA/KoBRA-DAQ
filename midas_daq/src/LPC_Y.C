double fA[10] = {

    4.12944e-08,3.54019e-06,0.000130308,0.00269882,0.0346664,0.287462,1.55662,5.46832,13.1247,14.6093

};

double fB[10] = {

   113.42,407.595,453.649,-58.5044,-533.835,-430.773,-82.1809,74.3565,60.8069,24.855

};

double gA[10] = {

  4.35343e-08,-3.72345e-06,0.000136693,-0.00282252,0.0361265,-0.298278,1.6065,-5.60478,13.3215,-14.7062

};

double gB[10] = {

    61.5808,-212.659,205.614,46.5305,-127.855,-95.8327,250.855,-188.37,80.6553,-26.2038

};



	double fhigh(double yy){

		return fA[0]*pow(yy,9) + fA[1]*pow(yy,8) + fA[2]*pow(yy,7) + fA[3]*pow(yy,6) + fA[4]*pow(yy,5) + fA[5]*pow(yy,4) + fA[6]*pow(yy,3) + fA[7]*pow(yy,2) + fA[8]*yy + fA[9];

	}

        double flow(double yy){

                return fB[0]*pow(yy,9) + fB[1]*pow(yy,8) + fB[2]*pow(yy,7) + fB[3]*pow(yy,6) + fB[4]*pow(yy,5) + fB[5]*pow(yy,4) + fB[6]*pow(yy,3) + fB[7]*pow(yy,2) + fB[8]*yy + fB[9];

        }

        double ghigh(double yy){

                return gA[0]*pow(yy,9) + gA[1]*pow(yy,8) + gA[2]*pow(yy,7) + gA[3]*pow(yy,6) + gA[4]*pow(yy,5) + gA[5]*pow(yy,4) + gA[6]*pow(yy,3) + gA[7]*pow(yy,2) + gA[8]*yy + gA[9];

        }
        double glow(double yy){

                return gB[0]*pow(yy,9) + gB[1]*pow(yy,8) + gB[2]*pow(yy,7) + gB[3]*pow(yy,6) + gB[4]*pow(yy,5) + gB[5]*pow(yy,4) + gB[6]*pow(yy,3) + gB[7]*pow(yy,2) + gB[8]*yy + gB[9];

        }

void LPC_Y(){

  #include <iostream>

	double diff1;
	double diff2;


  ifstream infile;
  infile.open("output00749.dat");

  TGraph* cor1 = new TGraph("cor1.dat");
  TGraph* cor2 = new TGraph("cor2.dat");

  TSpline3 *s1 = new TSpline3("cor1",cor1);
  TSpline3 *s2 = new TSpline3("cor2",cor2);

 /* TCanvas *c10 = new TCanvas("c10","c10",1);
  s1->Draw("APL");
  TCanvas *c20 = new TCanvas("c20","c20",1);
  s2->Draw("APL");
*/
  TH1D *PosY = new TH1D("PosY","All events",400,-100,100);
  TH1D *DoublehitY = new TH1D("DoublehitY","Double hit events",400,-100,100);
  TH1D *SinglehitY = new TH1D("SinglehitY","Single hit events",400,-100,100);
  TH1D *DoublehitYcor = new TH1D("DoublehitYcor","Double hit corrected",400,-100,100);
  TH1D *SinglehitYcor = new TH1D("SinglehitYcor","Single hit corected",400,-100,100);
  TH1D *PosYcor = new TH1D("PosYcor","All events corrected",400,-100,100);

  PosY->GetXaxis()->SetTitle("Y hit position (mm)");
  DoublehitY->GetXaxis()->SetTitle("Y hit position (mm)");
  SinglehitY->GetXaxis()->SetTitle("Y hit position (mm)");
  DoublehitYcor->GetXaxis()->SetTitle("Y hit position (mm)");
  SinglehitYcor->GetXaxis()->SetTitle("Y hit position (mm)");
  PosYcor->GetXaxis()->SetTitle("Y hit position (mm)");

  TH1D *DoublehitX = new TH1D("DoublehitX","DoublehitX",800,-200,200);
  TH1D *SinglehitX = new TH1D("SinglehitX","SinglehitX",800,-200,200);
  TH1D *PosXcor = new TH1D("PosXcor","PosXcor",800,-200,200);

  TH2D *Pos2Dcor = new TH2D("Pos2Dcor","Pos2Dcor",800,-200,200,400,-100,100);

  int tdc_value[32];

  double x1x2_0;
  double x1x2_1;
  double x3x4_0;
  double x3x4_1;
  double x5x6_0;
  double x5x6_1;
  double x7x8_0;
  double x7x8_1;

  double y1y2_0;
  double y1y2_1;
  double y3y4_0;
  double y3y4_1;

  double yposcor;    			// 에러수정사항

  for (int i=0; i<1e5; i++) {

    double difftotmin = 100;

    if (!infile) {
      break;//stops reading at end of file
    }

    for (int j=0; j<32; j++) {
      infile >> tdc_value[j];
//      cout << tdc_value[j] << endl;
    }

    int nyhit=0;//counts number of positions for each event
    int nxhit=0;//counts number of positions for each event

    x1x2_0 = 0.01522;//conversion gradient (mm/chan)
    x1x2_1 = 0.7914;//conversion intercept (mm)
    x3x4_0 = 0.01529;
    x3x4_1 = 0.3573;
    x5x6_0 = 0.01515;
    x5x6_1 = 0.5610;
    x7x8_0 = 0.01512;
    x7x8_1 = 0.4472;

    y1y2_0 = 0.01512;
    y1y2_1 = -0.2145;
    y3y4_0 = 0.01506;
    y3y4_1 = -0.1396;

    double ypos[2];//stores y positions for the 2 different segments
    double xpos[4];//stores x positions for the 4 different segments
	double yposd,yposd1,yposd2;
     int nyhit1 = 0;
//-----------------------------------------------
//For each Y segment, checks wether there is valid position hit
    if (tdc_value[8]>0 && tdc_value[9]>0 && tdc_value[8]-tdc_value[9]<5000 && tdc_value[8]-tdc_value[9]>-5000 && tdc_value[12]>0) {
      ypos[nyhit] = (tdc_value[8]-tdc_value[9]-16.61)*y1y2_0+y1y2_1-50.;
      PosY->Fill(ypos[nyhit],1);
      nyhit++;
    }

       if (tdc_value[10]>0 && tdc_value[11]>0 && tdc_value[10]-tdc_value[11]<5000 && tdc_value[10]-tdc_value[11]>-5000 && tdc_value[12]>0) {
      ypos[nyhit] = (tdc_value[10]-tdc_value[11]+2.674)*y3y4_0+y3y4_1+50.;
      PosY->Fill(ypos[nyhit],1);
      nyhit++;
      }


	if (tdc_value[8]>0 && tdc_value[9]>0 && tdc_value[8]-tdc_value[9]<5000 && tdc_value[8]-tdc_value[9]>-5000){
		if (tdc_value[10]>0 && tdc_value[11]>0 && tdc_value[10]-tdc_value[11]<5000 && tdc_value[10]-tdc_value[11]>-5000 && tdc_value[12]>0){

			yposd1 = (tdc_value[8]-tdc_value[9]-16.61)*y1y2_0+y1y2_1-50.;
			yposd2 = (tdc_value[10]-tdc_value[11]+2.674)*y3y4_0+y3y4_1+50.;
			if(-yposd1<yposd2) yposd = yposd2;
			else yposd = yposd1;

			nyhit1+=2;
			DoublehitY->Fill(yposd,1);

		}
	}

/*

  //For each X segment, checks wether there is valid position hit
    if (tdc_value[0]>0 && tdc_value[1]>0 && tdc_value[0]-tdc_value[1]<5000 && tdc_value[0]-tdc_value[1]>-5000 && tdc_value[12]>0) {
      xpos[nxhit] = (tdc_value[0]-tdc_value[1]+6.398)*x1x2_0+x1x2_1-150.;
      nxhit++;
    }
    if (tdc_value[2]>0 && tdc_value[3]>0 && tdc_value[2]-tdc_value[3]<5000 && tdc_value[2]-tdc_value[3]>-5000 && tdc_value[12]>0) {
      xpos[nxhit] = (tdc_value[2]-tdc_value[3]+5.481)*x3x4_0+x3x4_1-50.;
      nxhit++;
    }
    if (tdc_value[4]>0 && tdc_value[5]>0 && tdc_value[4]-tdc_value[5]<5000 && tdc_value[4]-tdc_value[5]>-5000 && tdc_value[12]>0) {
      xpos[nxhit] = (tdc_value[4]-tdc_value[5]+2.42)*x5x6_0+x5x6_1+50.;
      nxhit++;
    }
    if (tdc_value[6]>0 && tdc_value[7]>0 && tdc_value[6]-tdc_value[7]<5000 && tdc_value[6]-tdc_value[7]>-5000 && tdc_value[12]>0) {
      xpos[nxhit] = (tdc_value[6]-tdc_value[7]+6.685)*x7x8_0+x7x8_1+150.;
      nxhit++;
    }
    xposcor = xpos[0];

*/





//-----------------------------------------------
//If there are 2 hits in the Y segements, take a weighted average of their position
/*
ypos[0]=1.6;
ypos[1]=-1.6;
nyhit=2;
*/
 /*   if (nyhit>1) {
      DoublehitY->Fill(ypos[0],1);
      DoublehitY->Fill(ypos[1],1);
    }*/
    if (nyhit1>1) {

    //double yposcor;//stores corrected y position calculated from the 2 different segments		// 에러 수정 사항

/*      if(ypos[0] < -0.9 && ypos[0] > -6.){
        for (int k=0; k<1000; k++) {
	  yposcor = fhigh(ypos[0]);//->Eval(hitpos);
	}
	DoublehitYcor->Fill(yposcor,1);
	PosYcor->Fill(yposcor,1);
      }
      else if(ypos[0] > -0.9 && ypos[0] < - 0.3){
	for (int k=0; k<500; k++) {
	  yposcor = flow(ypos[0]);//->Eval(hitpos);
	}
	      DoublehitYcor->Fill(yposcor,1);
	      PosYcor->Fill(yposcor,1);
      }
      else if(ypos[0] > 0.9 && ypos[0] < 6.){
	for (int k=0; k<1000; k++) {
	  yposcor = ghigh(ypos[0]);//->Eval(hitpos);
	}
	DoublehitYcor->Fill(yposcor,1);
	PosYcor->Fill(yposcor,1);
      }
      else if(ypos[0] < 0.9 && ypos[0] >  0.3){
	for (int k=0; k<500; k++) {
	  yposcor = glow(ypos[0]);//->Eval(hitpos);
	}
	DoublehitYcor->Fill(yposcor,1);
	PosYcor->Fill(yposcor,1);
      }
*/
      if(yposd < -0.9 && yposd > -6.){
        for (int k=0; k<15; k++) {
          yposcor = fhigh(yposd);//->Eval(hitpos);
        }
        DoublehitYcor->Fill(yposcor,1);
        PosYcor->Fill(yposcor,1);
      }
      else if(yposd > -0.9 && yposd < - 0.3){
        for (int k=0; k<5; k++) {
          yposcor = flow(yposd);//->Eval(hitpos);
        }
              DoublehitYcor->Fill(yposcor,1);
              PosYcor->Fill(yposcor,1);
      }
      else if(yposd > 0.9 && yposd < 6.){
        for (int k=0; k<15; k++) {
          yposcor = ghigh(yposd);//->Eval(hitpos);
        }
        DoublehitYcor->Fill(yposcor,1);
        PosYcor->Fill(yposcor,1);
      }
      else if(yposd < 0.9 && yposd >  0.3){
        for (int k=0; k<5; k++) {
          yposcor = glow(yposd);//->Eval(hitpos);
        }
        DoublehitYcor->Fill(yposcor,1);
        PosYcor->Fill(yposcor,1);

      }
    }
  /*  for (int k=0; k<10000; k++) {

	// double hitpos = -5.+10./10000.*double(k);

	if(ypos[0] < -0.9) 			yposcor = fhigh(ypos[0]);//->Eval(hitpos);
	if(ypos[0] > -0.9 && ypos[0] < - 0.02) 	yposcor = flow(ypos[0]);//->Eval(hitpos);
        if(ypos[0] > 0.9) 			yposcor = ghigh(ypos[0]);//->Eval(hitpos);
        if(ypos[0] < 0.9 && ypos[0] >  0.02) 	yposcor = glow(ypos[0]);//->Eval(hitpos);
	
	if(ypos[1] < -0.9) 			yposcor = fhigh(ypos[1]);//->Eval(hitpos);
	if(ypos[1] > -0.9 && ypos[1] < - 0.02) 	yposcor = flow(ypos[1]);//->Eval(hitpos);
        if(ypos[1] > 0.9) 			yposcor = ghigh(ypos[1]);//->Eval(hitpos);
        if(ypos[1] < 0.9 && ypos[1] >  0.02) 	yposcor = glow(ypos[1]);//->Eval(hitpos);
*/	
//        double difftot = diff1*diff1+diff2*diff2;

	/*  if (difftot<difftotmin) {
          difftotmin=difftot;
          yposcor=hitpos;
	  }*/

	//	if(difftot<difftotmin){
	  //difftotmin=difftot;
	//	  if(hitpos < -0.9) 			yposcor = fhigh(hitpos)->Eval(hitpos);
	// if(hitpos > -0.9 && hitpos < - 0.02) 	yposcor = flow(hitpos)->Eval(hitpos);
	//  if(hitpos > 0.9) 			yposcor = ghigh(hitpos)->Eval(hitpos);
	//  if(hitpos < 0.9 && hitpos >  0.02) 	yposcor = glow(hitpos)->Eval(hitpos);
	  //	}

//      }

//      ypos[0] = (1.1*ypos[0]+1.3*ypos[1])/2.4;
  //    DoublehitYcor->Fill(yposcor,1);
    //  PosYcor->Fill(yposcor,1);
//      cout << ypos[0] << "\t" << ypos[1] << endl;
//      cout << yposcor << endl;

   // }
//-----------------------------------------------
//If there is a single hit in Y, shift events that are within 7.5mm of the segment edge towards the edge


    if (nyhit1==0) {
         SinglehitY->Fill(ypos[0],1);
	
   //   if (ypos[0]>-5.0 && ypos[0]<0){
   //     difftotmin=100;
      //        for (int k=0; k<1000; k++) {
      //  double hitpos = -5.+10./1000.*double(k);
  //        double diff1 = ypos[0] - s2->Eval(hitpos);
//          double difftot = diff1*diff1;
    //      if (difftot<difftotmin) {
      //      difftotmin=difftot;
        //    yposcor=hitpos;
	 if(ypos[0] < -0.9 && ypos[0] > -15.8){
        for (int k=0; k<30; k++) {
	  //  double hitpos = -5.+10./1000.*double(k);
	  yposcor = fhigh(ypos[0]);//->Eval(hitpos);
	}
	SinglehitYcor->Fill(yposcor,1);
	PosYcor->Fill(yposcor,1);
      }
	 else if(ypos[0] > -0.9 && ypos[0] < -0.25){
	for (int k=0; k<5; k++) {
	  // double hitpos = -5.+10./1000.*double(k);
	  yposcor = flow(ypos[0]);//->Eval(hitpos);
	}
	      SinglehitYcor->Fill(yposcor,1);
	      PosYcor->Fill(yposcor,1);
      }
      else if(ypos[0] > 0.9 && ypos[0] < 15.8){
	for (int k=0; k<30; k++) {
	  // double hitpos = -5.+10./1000.*double(k);
	  yposcor = ghigh(ypos[0]);//->Eval(hitpos);
	}
	SinglehitYcor->Fill(yposcor,1);
	PosYcor->Fill(yposcor,1);
      }
      else if(ypos[0] < 0.9 && ypos[0] >  0.25){
	for (int k=0; k<5; k++) {
	  // double hitpos = -5.+10./1000.*double(k);
	  yposcor = glow(ypos[0]);//->Eval(hitpos);
	}
	SinglehitYcor->Fill(yposcor,1);
	PosYcor->Fill(yposcor,1);
      }
      else {
	SinglehitYcor->Fill(ypos[0],1);
	PosYcor->Fill(ypos[0],1);
      }
	//        if(ypos[1] < -0.9 && ypos[1] > -7.)    yposcor = fhigh(ypos[1]);//->Eval(hitpos);
	// else if(ypos[1] > -0.9 && ypos[1] < - 0.1)  yposcor = flow(ypos[1]);//->Eval(hitpos);
        //else if(ypos[1] > 0.9 && ypos[1] < 7.)      yposcor = ghigh(ypos[1]);//->Eval(hitpos);
        //else if(ypos[1] < 0.9 && ypos[1] >  0.1)    yposcor = glow(ypos[1]);//->Eval(hitpos);
	//else yposcor = ypos[1];
         // }
	     // }
	  // SinglehitYcor->Fill(yposcor,1);
       // cout << ypos[0] << "\t" << yposcor << "\t" << difftotmin << endl;
	// }
   /*   else if (ypos[0]<5.0 && ypos[0]>0){
        difftotmin=100;
        for (int k=0; k<500; k++) {
          double hitpos = 0.+10./500.*double(k);
        //  double diff1 = ypos[0] - s1->Eval(hitpos);
        //  double difftot = diff1*diff1;
        //  if (difftot<difftotmin) {
         //   difftotmin=difftot;
         //   yposcor=hitpos;
	if(ypos[0] < -0.9)                      yposcor = fhigh(ypos[0]);//->Eval(hitpos);
        if(ypos[0] > -0.9 && ypos[0] < - 0.02)  yposcor = flow(ypos[0]);//->Eval(hitpos);
        if(ypos[0] > 0.9)                       yposcor = ghigh(ypos[0]);//->Eval(hitpos);
        if(ypos[0] < 0.9 && ypos[0] >  0.02)    yposcor = glow(ypos[0]);//->Eval(hitpos);

        if(ypos[1] < -0.9)                      yposcor = fhigh(ypos[1]);//->Eval(hitpos);
        if(ypos[1] > -0.9 && ypos[1] < - 0.02)  yposcor = flow(ypos[1]);//->Eval(hitpos);
        if(ypos[1] > 0.9)                       yposcor = ghigh(ypos[1]);//->Eval(hitpos);
        if(ypos[1] < 0.9 && ypos[1] >  0.02)    yposcor = glow(ypos[1]);//->Eval(hitpos);

          }
        }
        SinglehitYcor->Fill(yposcor,1);
       // cout << ypos[0] << "\t" << yposcor << "\t" << difftotmin << endl;
      }
      else {
        SinglehitYcor->Fill(ypos[0],1);
      }*/
    }

//-----------------------------------------------

  //  if (nyhit>0) {PosYcor->Fill(ypos[0],1);}
//    if (nxhit>0) {PosXcor->Fill(xpos[0],1);}
  //  if (nyhit>0 && nxhit>0) {Pos2Dcor->Fill(xpos[0],ypos[0],1);}

  }

  TCanvas *c0 = new TCanvas("c0","c0",1);
  PosY->Draw("COLZ");

  TCanvas *c1 = new TCanvas("c1","c1",1);
  SinglehitY->Draw("COLZ");

  TCanvas *c11 = new TCanvas("c11","c11",1);
  SinglehitYcor->Draw("COLZ");

  TCanvas *c2 = new TCanvas("c2","c2",1);
  DoublehitY->Draw("COLZ");

  TCanvas *c22 = new TCanvas("c22","c22",1);
  DoublehitYcor->Draw("COLZ");

  TCanvas *c3 = new TCanvas("c3","c3",1);
  PosYcor->Draw("COLZ");

/*  TCanvas *c4 = new TCanvas("c4","c4",1);
  SinglehitX->Draw("COLZ");

  TCanvas *c5 = new TCanvas("c5","c5",1);
  DoublehitX->Draw("COLZ");

  TCanvas *c6 = new TCanvas("c6","c6",1);
  PosXcor->Draw("COLZ");

  TCanvas *c7 = new TCanvas("c7","c7",1);
  Pos2Dcor->Draw("COLZ");
*/
}
