/*==========================================================================*
*
*                            Unterprogramm: walk2
*                       Berechnung eines Zeitschritts
*
* Input:
*
*    nxmax        : Anzahl der Knoten in X-Richtung
*    icyc      : Zeitzyklus-Nummer
*    along, aq    : longitudinale bzw. transversale Dispersivit�t
*    de        : Zeitschrittweite =delt
*    vx[i]  : Geschwindigkeitskomponente in X-Richtung
*    dx[i]     : Knotenabst�nde in X-Richtung
*
*    ir[i][j]: ir-Maske - Transportmodell, enth�lt Randbedingungen
*
* Output:
*
*    partx[i]       : Koordinaten des Partikels i
*    npmax        : Anzahl Partikel im Modell
*
* 16.11.92
*==========================================================================*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "gwheader.h"

#define SQRT12 3.464101615
#define randinv 1.0/RAND_MAX
double gasdev( int *idum);

#ifdef __unix
extern "C"
 void walk2_(int& npmax,int& nxmax,int& ncyc,double& along,double& aquer,double dm[NCNODEX+2]
		   ,double& texe,double dx[NCNODEX+2],double vx[NCNODEX+2]
		   ,double partx[NCPMAX],double partxo[NCPMAX],double& xmaxr,double& xminr
		   ,double partic[NCBASIS+NCCOMPL][NCPMAX],double bn[NCNODEX][NCBASIS]
		   ,double cn[NCNODEX][NCCOMPL],int partib[NCNODEX],int& ibpstart,double x[NCNODEX]
		   ,double bo[NCNODEX][NCBASIS],double co[NCNODEX][NCCOMPL],int& m1,int& m2)
#else
 void walk2(int npmax,int nxmax,int ncyc,double along,double aquer,double dm[NCNODEX+2]
		   ,double texe,double dx[NCNODEX+2],double vx[NCNODEX+2]
		   ,double partx[NCPMAX],double partxo[NCPMAX],double xmaxr,double xminr
		   ,double partic[NCBASIS+NCCOMPL][NCPMAX],double bn[NCNODEX][NCBASIS]
		   ,double cn[NCNODEX][NCCOMPL],int partib[NCNODEX],int ibpstart,double x[NCNODEX]
		   ,double bo[NCNODEX][NCBASIS],double co[NCNODEX][NCCOMPL],int m1,int m2)
#endif
{
{
        double  slong,xlaenge, xxmin, xxmax;
        double dabs(),dpx, vpx,Z1,vabs;
        int  ip,ipa;
/*      float gasdef(idum);*/
        int idum, iknx;

        idum=1;
        ip = 0;
        ipa =0;
/*      printf("walk %d %d %d %g %g %g %g %g %g\n",ibpstart,nxmax,ncyc,along,*dm,texe,xmaxr, xminr);
      printf("walk %d %d %d %g %g %g %g %g %g\n",*ibpstart,*nymax,*ncyc,*along,*dm,*texe,*ymaxr, *yminr);

        for (i=0; i<=(*nxmax)-1; i++) {
                printf("i dx vx %d %f %f \n", i, dx[i], vx[i] );
        }
*/
        xlaenge = xmaxr - xminr + 2 * dx[1];
        xxmin = xminr - dx[1];
        xxmax = xmaxr + dx[1];     /*  + dx[1]; */

        do  {  /*  teilchenloop  */
           iknx= (int ) ( (partx[ip]+dx[1])  / dx[2])  ;

dpx=dx[1]-(partx[ip]-x[iknx]);
vpx=vx[iknx];
vabs=sqrt(vpx*vpx);
if(vabs == 0.)  vabs=1.e-30; /* keine division durch 0 */
/* neu 23 05 95 */

           if (along == 0. && aquer == 0. && dm[iknx] == 0.)  {              /* disp + diff =0  */
             slong=0.0;
           }
           else {                                                     /* wenigstens eine Dispersivitaet > 0 */

              Z1=(double) rand()*randinv -0.5 ;
              slong = 2.*Z1* sqrt(6.* (along*vpx  + dm[iknx]) * texe);
	   /*   slong= (double ) gasdev(idum) * sqrt(2. *( along * vpx + dm[iknx]) * texe); longitu. Weg x' */

           }

           partx[ip] +=  vpx *  texe +  slong;  /* neue position der teilchen = konv. Anteil + disp. Anteil x */

           if(partx[ip]  >=   xxmax  ){  /*keine diffusion ueber 'rechten rand' - const. conc   */

             partx[ip] -=  xlaenge ;
             for (ipa=0; ipa< m1; ipa++) {
                partic[ipa][ip]=bn[0][ipa]/(double) ibpstart ; /*division durch 50 partikelx in randbox */
             }
             for (ipa=0; ipa< m2; ipa++) {
               partic[ipa+ m1][ip]=cn[0][ipa]/(double) ibpstart;
             }
           }
           if(partx[ip] <= xxmin) {
               partxo[ip] =  partx[ip] ;
               partx[ip] += xlaenge   ;
               for (ipa=0; ipa< m1; ipa++) {
                  partic[ipa][ip]=bo[nxmax-1][ipa]/(double)  ibpstart; /* -1 durch -2 ersetzt, 090796 */
               }
               for (ipa=0; ipa< m2; ipa++) {
                  partic[ipa+ m1][ip]=co[nxmax-1][ipa]/(double) ibpstart; /*  -1 durch -2 ersetzt, 090796 */
               }
            }
        ++ip;
        }   while (ip<= npmax-1  );      /* ende teilchenloop */
}

}

#include <math.h>
#include <stdlib.h>
#define randinv 1.0/RAND_MAX

double gasdev( int *idum)
/*int *idum;*/
/* returns a normally distributed deviate with  zero mean and unit variance */

{
        int iset=0;
        double gset;
        double fac,r,v1,v2;
        gset=0.;
        if (iset ==0) {
                do {
                        v1=2.0*rand(/*idum*/)*randinv - 1.0;
                        v2=2.0*rand(/*idum*/)*randinv - 1.0;
                        r=v1*v1+v2*v2;
                } while( r >= 1.0 || r ==0 );
                fac = sqrt(-2.0*log(r)/r);
                gset=v1*fac;
                iset=1;
                return v2*fac;
        } else {
                iset=1;
                return gset;
        }
}