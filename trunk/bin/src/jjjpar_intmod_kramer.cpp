//------------------------------------------------------------------------------------------------
//routine mcalc for kramers doublet
//------------------------------------------------------------------------------------------------
Vector & jjjpar::kramer (double & T, Vector & gjmbH, double & lnZ, double & U)
{ /*on input
    ABC(1...3)  A,M,Ci....saturation moment/gJ[MU_B] of groundstate doublet in a.b.c direction
    gJ		lande factor
    T		temperature[K]
    gjmbH	vector of effective field [meV]
  on output    
    J		single ion momentum vector <J>
    Z		single ion partition function
    U		single ion magnetic energy
*/
  double alpha, betar, betai, lambdap,lambdap_K_BT, lambdap2, expp, expm, np, nm;
  double nennerp, nennerm, jap, jam, jbp, jbm, jcp, jcm,Z;
  double alpha_lambdap,alphaplambdap,alphaxlambdap;

  static Vector Jret(1,3);
  
  alpha = ABC[2] * gjmbH[2];
  betar = -ABC[1] * gjmbH[1];
  betai = -ABC[3] * gjmbH[3];
  lambdap2 = alpha * alpha + betar * betar + betai * betai;
  lambdap = sqrt (lambdap2);
  lambdap_K_BT=lambdap/K_B/T;
  if (lambdap_K_BT>700){lambdap_K_BT=700;}
  if (lambdap_K_BT<-700){lambdap_K_BT=-700;}
  expm = exp (lambdap_K_BT);
  expp = 1/expm; //=exp (-lambdap_K_BT);
  Z = expp + expm;
  lnZ=log(Z);
  np = expp / Z;
  nm = expm / Z;
  U=lambdap*(np-nm); // energy

//  nennerp = (alpha - lambdap) * (alpha - lambdap) + betar * betar + betai * betai;
//  nennerm = (alpha + lambdap) * (alpha + lambdap) + betar * betar + betai * betai;
    alphaxlambdap=alpha*lambdap;
    alpha_lambdap=alpha-lambdap;
    alphaplambdap=alpha+lambdap;
    nennerp=  2.0*(-alphaxlambdap+lambdap2);    
    nennerm=  2.0*(alphaxlambdap+lambdap2);    

  if (nennerp > SMALL)
    {
      jap = -ABC[1] * 2.0 * betar * (alpha_lambdap) / nennerp;
//      jbp = M * ((alpha_lambdap) * (alpha_lambdap) - (betar * betar + betai * betai)) / nennerp;
      jbp = ABC[2] * (2.0 * alpha*alpha_lambdap) / nennerp;
      jcp = -2.0 * ABC[3] * betai * (alpha_lambdap) / nennerp;
    }
  else
    {
      jap = 0;
      if (alpha * alpha > SMALL)
	{
	  jbp = -copysign (ABC[2], alpha);
	}
      else
	{
	  jbp = ABC[2];
	}
      jcp = 0;
    }

  if (nennerm > SMALL)
    {
      jam = -ABC[1] * 2.0 * betar * (alphaplambdap) / nennerm;
//      jbm = M * ((alpha + lambdap) * (alpha + lambdap) - (betar * betar + betai * betai)) / nennerm;
      jbm = ABC[2] * (2.0 * alpha*alphaplambdap) / nennerm;
      jcm = -2.0 * ABC[3] * betai * (alphaplambdap) / nennerm;
    }
  else
    {
      jam = 0;
      if (alpha * alpha > SMALL)
	{
	  jbm = copysign (ABC[2], alpha);
	}
      else
	{
	  jbm = -ABC[2];
	}
      jcm = 0;
    }

  Jret[1] = np * jap + nm * jam;
  Jret[2] = np * jbp + nm * jbm;
  Jret[3] = np * jcp + nm * jcm;
//  printf ("Ha=%g Hb=%g Hc=%g Ja=%g Jb=%g Jc=%g \n", 
//     gjmbH[1]/MU_B/gjJ, gjmbH[2]/MU_B/gjJ, gjmbH[3]/MU_B/gjJ, J[1], J[2], J[3]);
return Jret;
}

int jjjpar::kramerdm(int & transitionnumber,double & T,Vector & gjmbH,ComplexMatrix & mat,float & delta)
{ 
  /*on input
    transitionnumber ... number of transition to be computed - meaningless for kramers doublet, because there is only 1 transition
    ABC[i]	saturation moment/gJ[MU_B] of groundstate doublet in a.b.c direction
    gJ		lande factor
    T		temperature[K]
    gjmbH	vector of effective field [meV]
  on output    
    delta	splitting of kramers doublet [meV]
    mat(i,j)	<-|(Ji-<Ji>)|+><+|(Jj-<Jj>|-> tanh(delta/2kT)
*/
  double alpha, betar, betai, lambdap,lambdap_K_BT, lambdap2, expp, expm, np, nm;
  double nennerp, nennerm, nenner;
  complex<double> ja,jb,jc,i(0,1), jap, jam, jbp, jbm, jcp, jcm;
  double alpha_lambdap,alphaplambdap,alphaxlambdap;
  double Z;
  double lnz,u;
  
  static Vector Jret(1,3);
  // clalculate thermal expectation values (needed for quasielastic scattering)
  Jret=kramer(T,gjmbH,lnz,u);
  int pr;
  pr=1;
  if (transitionnumber<0) {pr=0;transitionnumber*=-1;}

  alpha = ABC[2]* gjmbH[2];
  betar = -ABC[1] * gjmbH[1];
  betai = -ABC[3] * gjmbH[3];
  lambdap2 = alpha * alpha + betar * betar + betai * betai;
  lambdap = sqrt (lambdap2);


  
  lambdap_K_BT=lambdap/K_B/T;
  if (lambdap_K_BT>700){lambdap_K_BT=700;}
  if (lambdap_K_BT<-700){lambdap_K_BT=-700;}
  expm = exp (lambdap_K_BT);
  expp = 1/expm; //=exp (-lambdap_K_BT);
  Z = expp + expm;
  np = expp / Z;
  nm = expm / Z;

    alphaxlambdap=alpha*lambdap;
    alpha_lambdap=alpha-lambdap;
    alphaplambdap=alpha+lambdap;
    nennerp=  2.0*(-alphaxlambdap+lambdap2);    
    nennerm=  2.0*(alphaxlambdap+lambdap2);    


if (transitionnumber==2)
{ delta=2*lambdap; //set delta !!!


    nenner=sqrt(nennerp*nennerm);

  if (nenner > SMALL)
    {
      ja = -ABC[1] * 2.0*(alpha * betar+i * betai * lambdap) / nenner;
      jb = -ABC[2] * 2.0 * (betar*betar+betai*betai) / nenner;
      jc = -ABC[3] * 2.0*(alpha*betai -i *betar*lambdap) / nenner;
    }
  else
    {
      if (alpha > SMALL)
	{ja = ABC[1];  // <-| is the ground state
  	 jb = 0;
         jc = -i*ABC[3];
	}
      else
	{ja = ABC[1];  // <+| is the ground state
  	 jb = 0;
         jc = i*ABC[3]; 	
	}
    }
 if (delta>SMALL)
 {// now lets calculate mat
 mat(1,1)=ja*conj(ja)*(nm-np);
 mat(1,2)=ja*conj(jb)*(nm-np);
 mat(1,3)=ja*conj(jc)*(nm-np);
 mat(2,1)=jb*conj(ja)*(nm-np);
 mat(2,2)=jb*conj(jb)*(nm-np);
 mat(2,3)=jb*conj(jc)*(nm-np);
 mat(3,1)=jc*conj(ja)*(nm-np);
 mat(3,2)=jc*conj(jb)*(nm-np);
 mat(3,3)=jc*conj(jc)*(nm-np); 
 }else
 {// quasielastic scattering needs epsilon * nm / KT ....
 mat(1,1)=ja*conj(ja)*nm/K_B/T;
 mat(1,2)=ja*conj(jb)*nm/K_B/T;
 mat(1,3)=ja*conj(jc)*nm/K_B/T;
 mat(2,1)=jb*conj(ja)*nm/K_B/T;
 mat(2,2)=jb*conj(jb)*nm/K_B/T;
 mat(2,3)=jb*conj(jc)*nm/K_B/T;
 mat(3,1)=jc*conj(ja)*nm/K_B/T;
 mat(3,2)=jc*conj(jb)*nm/K_B/T;
 mat(3,3)=jc*conj(jc)*nm/K_B/T; 
 }
}
else
{ delta=-SMALL; // transition within the same level
  if (nennerp > SMALL)
    {
      jap = -ABC[1] * 2.0 * betar * (alpha_lambdap) / nennerp;
//      jbp = M * ((alpha_lambdap) * (alpha_lambdap) - (betar * betar + betai * betai)) / nennerp;
      jbp = ABC[2] * (2.0 * alpha*alpha_lambdap) / nennerp;
      jcp = -2.0 * ABC[3] * betai * (alpha_lambdap) / nennerp;
    }
  else
    {
      jap = 0;
      if (alpha * alpha > SMALL)
	{
	  jbp = -copysign (ABC[2], alpha);
	}
      else
	{
	  jbp = -ABC[2];
	}
      jcp = 0;
    }

  if (nennerm > SMALL)
    {
      jam = -ABC[1] * 2.0 * betar * (alphaplambdap) / nennerm;
//      jbm = M * ((alpha + lambdap) * (alpha + lambdap) - (betar * betar + betai * betai)) / nennerm;
      jbm = ABC[2] * (2.0 * alpha*alphaplambdap) / nennerm;
      jcm = -2.0 * ABC[3] * betai * (alphaplambdap) / nennerm;
    }
  else
    {
      jam = 0;
      if (alpha * alpha > SMALL)
	{
	  jbm = copysign (ABC[2], alpha);
	}
      else
	{
	  jbm = ABC[2];
	}
      jcm = 0;
    }
 if (transitionnumber==1)
 {// now lets calculate mat
 mat(1,1)=(jam-Jret(1))*(jam-Jret(1))*nm/K_B/T;
 mat(1,2)=(jam-Jret(1))*(jbm-Jret(2))*nm/K_B/T;
 mat(1,3)=(jam-Jret(1))*(jcm-Jret(3))*nm/K_B/T;
 mat(2,1)=(jbm-Jret(2))*(jam-Jret(1))*nm/K_B/T;
 mat(2,2)=(jbm-Jret(2))*(jbm-Jret(2))*nm/K_B/T;
 mat(2,3)=(jbm-Jret(2))*(jcm-Jret(3))*nm/K_B/T;
 mat(3,1)=(jcm-Jret(3))*(jam-Jret(1))*nm/K_B/T;
 mat(3,2)=(jcm-Jret(3))*(jbm-Jret(2))*nm/K_B/T;
 mat(3,3)=(jcm-Jret(3))*(jcm-Jret(3))*nm/K_B/T;
 }else{
 // now lets calculate mat
 mat(1,1)=(jap-Jret(1))*(jap-Jret(1))*np/K_B/T;
 mat(1,2)=(jap-Jret(1))*(jbp-Jret(2))*np/K_B/T;
 mat(1,3)=(jap-Jret(1))*(jcp-Jret(3))*np/K_B/T;
 mat(2,1)=(jbp-Jret(2))*(jap-Jret(1))*np/K_B/T;
 mat(2,2)=(jbp-Jret(2))*(jbp-Jret(2))*np/K_B/T;
 mat(2,3)=(jbp-Jret(2))*(jcp-Jret(3))*np/K_B/T;
 mat(3,1)=(jcp-Jret(3))*(jap-Jret(1))*np/K_B/T;
 mat(3,2)=(jcp-Jret(3))*(jbp-Jret(2))*np/K_B/T;
 mat(3,3)=(jcp-Jret(3))*(jcp-Jret(3))*np/K_B/T;
 }
}
if (pr==1) printf ("delta=%4.6g meV\n",delta);

return 3; // kramers doublet has always exactly one transition + 2 levels (quasielastic scattering)!
}
/**************************************************************************/
