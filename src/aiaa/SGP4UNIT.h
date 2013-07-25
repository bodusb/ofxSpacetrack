#ifndef _sgp4unit_
#define _sgp4unit_
/*     ----------------------------------------------------------------
*
*                                 sgp4unit.h
*
*    this file contains the sgp4 procedures for analytical propagation
*    of a satellite. the code was originally released in the 1980 and 1986
*    spacetrack papers. a detailed discussion of the theory and history
*    may be found in the 2006 aiaa paper by vallado, crawford, hujsak,
*    and kelso.
*
*                            companion code for
*               fundamentals of astrodynamics and applications
*                                    2007
*                              by david vallado
*
*       (w) 719-573-2600, email dvallado@agi.com
*
*    current :
*              20 apr 07  david vallado
*                           misc fixes for constants
*    changes :
*              11 aug 06  david vallado
*                           chg lyddane choice back to strn3, constants, misc doc
*              15 dec 05  david vallado
*                           misc fixes
*              26 jul 05  david vallado
*                           fixes for paper
*                           note that each fix is preceded by a
*                           comment with "sgp4fix" and an explanation of
*                           what was changed
*              10 aug 04  david vallado
*                           2nd printing baseline working
*              14 may 01  david vallado
*                           2nd edition baseline
*                     80  norad
*                           original baseline
*       ----------------------------------------------------------------      */

#include "ofMain.h"
//#include <math.h>
//#include <stdio.h>

// -------------------------- structure declarations ----------------------------
typedef enum
{
  wgs72old,
  wgs72,
  wgs84
} gravconsttype;

typedef struct elsetrec
{
  long int  satnum;
  int       epochyr, epochtynumrev;
  int       error;
  char      init, method;

  /* Near Earth */
  int    isimp;
  double aycof  , con41  , cc1    , cc4      , cc5    , d2      , d3   , d4    ,
         delmo  , eta    , argpdot, omgcof   , sinmao , t       , t2cof, t3cof ,
         t4cof  , t5cof  , x1mth2 , x7thm1   , mdot   , nodedot, xlcof , xmcof ,
         nodecf;

  /* Deep Space */
  int    irez;
  double d2201  , d2211  , d3210  , d3222    , d4410  , d4422   , d5220 , d5232 ,
         d5421  , d5433  , dedt   , del1     , del2   , del3    , didt  , dmdt  ,
         dnodt  , domdt  , e3     , ee2      , peo    , pgho    , pho   , pinco ,
         plo    , se2    , se3    , sgh2     , sgh3   , sgh4    , sh2   , sh3   ,
         si2    , si3    , sl2    , sl3      , sl4    , gsto    , xfact , xgh2  ,
         xgh3   , xgh4   , xh2    , xh3      , xi2    , xi3     , xl2   , xl3   ,
         xl4    , xlamo  , zmol   , zmos     , atime  , xli     , xni;

  double a      , altp   , alta   , epochdays, jdsatepoch       , nddot , ndot  ,
         bstar  , rcse   , inclo  , nodeo    , ecco             , argpo , mo    ,
         no;
} elsetrec;

// --------------------------- function declarations ----------------------------
int sgp4init
       (
         gravconsttype whichconst,      const int satn,     const double epoch,
         const double xbstar, const double xecco, const double xargpo,
         const double xinclo, const double xmo,   const double xno,
         const double xnodeo,
         elsetrec& satrec
       );

int sgp4
       (
         gravconsttype whichconst,
         elsetrec& satrec, double tsince,
         double r[], double v[]
       );

double  gstime
       (
         double
       );

void getgravconst
      (
       gravconsttype,
       double&,
       double&,
       double&,
       double&,
       double&,
       double&,
       double&,
       double&
      );

#endif
