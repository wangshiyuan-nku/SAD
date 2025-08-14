void ApertSet(double z, double& rx, double& ry, double& dx){
  double Absz=fabs(z);
  if(Absz<0.715){
    rx=0.0315;
    ry=0.0315;
  } 
  else if(Absz>=0.715&&Absz<1.2){
    rx=0.0315+(Absz-0.715)*0.0484536;
    ry=0.0315+(Absz-0.715)*0.0484536;
  }
  else if(Absz>=1.2&&Absz<1.89){
    rx=0.055;
    ry=0.055;
  }
  else if(Absz>=1.89&&Absz<1.924){
    rx=0.055+(Absz-1.89)*0.1470588;
    ry=0.055+(Absz-1.89)*0.1470588;
  }
  else if(Absz>=1.924&&Absz<2.129){
    rx=0.06+(Absz-1.924)*0.1578947;
    ry=0.06+(Absz-1.924)*0.1578947;
  }
  else if(z>=2.129&&z<2.21){
    rx=0.032;
    ry=0.032;
  }
  if(z>0){
    if(Absz>=2.21&&Absz<3.058){
      rx=0.032+(Absz-2.21)*0.0159198;
      ry=0.032+(Absz-2.21)*0.0159198;
    }
    else if(Absz>=3.058&&Absz<3.12){
      rx=0.0455+(Absz-3.058)*0.0532258;
      ry=0.0455+(Absz-3.058)*0.0532258;
    }
    else if(Absz>=3.12&&Absz<12.0){
      rx=0.05;
      ry=0.05;
    }
  } 
  else if(z<0){
    if(Absz>=2.21&&Absz<3.058){
      rx=0.032+(Absz-2.21)*0.01533019;
      ry=0.032+(Absz-2.21)*0.01533019;
    }
    else if(Absz>=3.058&&Absz<3.12){
      rx=0.045+(Absz-3.058)*0.08064516;
      ry=0.045+(Absz-3.058)*0.08064516;
    }
    else if(Absz>=3.12&&Absz<12.0){
      rx=0.05;
      ry=0.05;
    }
  }
  if(Absz>=12.0){
    rx=0.055;
    ry=0.028;
  }

  //collimators
  double SetColApt(double, double, double, double);
  double Lcol=0.335,  Lcolcen=0.125, Kxytrans=0.1777143;
  double Zcol,Rmincol,DisfromCol;
  Zcol=-7.6; Rmincol=0.028; DisfromCol=fabs(z-Zcol); //R4OCV02
  if(DisfromCol<=Lcol/2) ry=SetColApt(Lcolcen,Rmincol,Kxytrans,DisfromCol);
  Zcol=-8.2; Rmincol=0.035; DisfromCol=fabs(z-Zcol); //R4OCH02
  if(DisfromCol<=Lcol/2) rx=SetColApt(Lcolcen,Rmincol,Kxytrans,DisfromCol);
  Zcol=-11;  Rmincol=0.035; DisfromCol=fabs(z-Zcol); //R4OCH04
  if(DisfromCol<=Lcol/2) rx=SetColApt(Lcolcen,Rmincol,Kxytrans,DisfromCol);
  Zcol=11;   Rmincol=0.035; DisfromCol=fabs(z-Zcol); //R3ICH04
  if(DisfromCol<=Lcol/2) rx=SetColApt(Lcolcen,Rmincol,Kxytrans,DisfromCol);
  Zcol=-27;  Rmincol=0.031; DisfromCol=fabs(z-Zcol); //R3ICH08
  if(DisfromCol<=Lcol/2) rx=SetColApt(Lcolcen,Rmincol,Kxytrans,DisfromCol);
  Zcol=-46.8; Rmincol=0.026;DisfromCol=fabs(z-Zcol); //R4OCH14
  if(DisfromCol<=Lcol/2) rx=SetColApt(Lcolcen,Rmincol,Kxytrans,DisfromCol);
  Zcol=27.5; Rmincol=0.03;  DisfromCol=fabs(z-Zcol); //R3ICH08
  if(DisfromCol<=Lcol/2) rx=SetColApt(Lcolcen,Rmincol,Kxytrans,DisfromCol);
  Zcol=-50.1; Rmincol=0.015;DisfromCol=fabs(z-Zcol); //R4OCV15
  if(DisfromCol<=Lcol/2) ry=SetColApt(Lcolcen,Rmincol,Kxytrans,DisfromCol);
  Zcol=-64.8; Rmincol=0.014;DisfromCol=fabs(z-Zcol); //R4OCV16
  if(DisfromCol<=Lcol/2) ry=SetColApt(Lcolcen,Rmincol,Kxytrans,DisfromCol);
  Zcol=28.5; Rmincol=0.015; DisfromCol=fabs(z-Zcol); //R3ICV09
  if(DisfromCol<=Lcol/2) ry=SetColApt(Lcolcen,Rmincol,Kxytrans,DisfromCol);

  //dx
  if(Absz<0.909) dx=Absz*0.011;
  else if(Absz>=0.909&&Absz<1.509) dx=(Absz-0.909)*0.01854+0.01;
  else if(Absz>=1.509&&Absz<2.129) dx=(Absz-1.509)*0.026+0.021;
  else dx=0;  
}

double SetColApt(double Lcolcen, double Rmincol, double Kxytrans, double DisfromCol){
  if(DisfromCol<=Lcolcen) return Rmincol;
  else return Kxytrans*(DisfromCol-Lcolcen/2)+Rmincol;
}

void TransCoord(bool bGeneElec,double xsad,double ysad, double zsad, double &xG4, double &yG4, double &zG4, double& pxG4, double& pyG4, double& pzG4, double& EG4){
  double E0=1890,me=0.511;
  double p0=sqrt(E0*E0-me*me);

  double rx,ry,dx;
  ApertSet(zsad,rx,ry,dx); 
  double theta=0,k1,b1;
  if(fabs(zsad)>=2.129){  //after seperate
    if(zsad<=-2.129&&zsad>-12.0){
      theta=0.026;
      k1=theta; b1=0.018385;
    } else if(zsad>=2.129&&zsad<2.3){
      theta=0.02655;
      k1=theta; b1=-0.019237;
    } else if(zsad>=2.3&&zsad<2.9){
      theta=0.045;
      k1=theta; b1=-0.061638;
    } else if(zsad>=2.9&&zsad<12.0){
      theta=0.0656;
      k1=theta; b1=-0.121355;
    }
    double cost=cos(theta);
    double sint=sin(theta);

    double deno = sqrt(xsad*xsad/(rx*rx)+ysad*ysad/(ry*ry));
    double x1 = xsad/deno;
    double y1 = ysad/deno;
    double z1 = zsad; 
    double x_midline,z_midline;
    x_midline=z1*k1+b1;;
    z_midline=z1;;
    xG4 = -x1*cost+x_midline;
    yG4 = y1;
    zG4 = x1*sint+z_midline;
   
    double px_cm=pxG4*p0;
    double py_cm=pyG4*p0;
    double pz_cm=(1+pzG4)*p0;
    pxG4= -px_cm*cost+pz_cm*sint;
    pyG4= py_cm;
    pzG4= px_cm*sint+pz_cm*cost;
    EG4 = sqrt(pxG4*pxG4+pyG4*pyG4+pzG4*pzG4+me*me);
  } 
  else{
    double deno = sqrt(xsad*xsad/(rx*rx)+ysad*ysad/(ry*ry));
    xG4 = xsad/deno;
    yG4 = ysad/deno;
    zG4 = zsad;

    if(fabs(zsad)>=1.009) theta=0.0239;
    else theta=0.011;
    double cost=cos(theta);
    double sint=sin(theta);
    double px_cm=pxG4*p0;
    double py_cm=pyG4*p0;
    double pz_cm=(1+pzG4)*p0;
    pxG4= px_cm*cost+pz_cm*sint;
    pyG4= py_cm;
    pzG4= -px_cm*sint+pz_cm*cost;
    EG4 = sqrt(pxG4*pxG4+pyG4*pyG4+pzG4*pzG4+me*me); 
  }
  if(bGeneElec){
    zG4=-1*zG4;
    pzG4=-1*pzG4;
  }
}
