/*============================================================================
  WCSLIB 8.4 - an implementation of the FITS WCS standard.
  Copyright (C) 1995-2024, Mark Calabretta

  This file is part of WCSLIB.

  WCSLIB is free software: you can redistribute it and/or modify it under the
  terms of the GNU Lesser General Public License as published by the Free
  Software Foundation, either version 3 of the License, or (at your option)
  any later version.

  WCSLIB is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
  more details.

  You should have received a copy of the GNU Lesser General Public License
  along with WCSLIB.  If not, see http://www.gnu.org/licenses.

  Author: Mark Calabretta, Australia Telescope National Facility, CSIRO,
     and: Michael Droetboom, Space Telescope Science Institute
  http://www.atnf.csiro.au/people/Mark.Calabretta
  $Id: twcs.c,v 8.4 2024/10/28 13:56:17 mcalabre Exp $
*=============================================================================
*
* twcs tests wcss2p() and wcsp2s() for closure on an oblique 2-D slice through
* a 4-D image with celestial, spectral and logarithmic coordinate axes.
*
*---------------------------------------------------------------------------*/

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <wcslib.h>
#include <wcsconfig_tests.h>


#define NELEM 9

void parser(struct wcsprm *);
int  check_error(struct wcsprm *, int, int, char *);
int  test_errors();

// Reporting tolerance.
const double tol = 1.0e-10;


// In real life these would be encoded as FITS header keyrecords.
const int NAXIS = 4;
const double CRPIX[4] =  {  513.0,  0.0,  0.0,  0.0};
const double PC[4][4] = {{    1.1,  0.0,  0.0,  0.0},
                         {    0.0,  1.0,  0.0,  0.1},
                         {    0.0,  0.0,  1.0,  0.0},
                         {    0.0,  0.2,  0.0,  1.0}};
const double CDELT[4] =  {-9.635265432e-6, 1.0, 0.1, -1.0};

char CTYPE[4][9] = {"WAVE-F2W", "XLAT-BON", "TIME-LOG", "XLON-BON"};

const double CRVAL[4] = {0.214982042, -30.0, 1.0, 150.0};
const double LONPOLE  = 150.0;
const double LATPOLE  = 999.0;
const double RESTFRQ  =   1.42040575e9;
const double RESTWAV  =   0.0;

int NPV = 3;
struct pvcard PV[3];		// Projection parameters are set in main().

int itest = 0;

int main()

{
  int ver[3];
  printf("WCSLIB version number: %s", wcslib_version(ver));
  printf(" (%d,%d,%d)\n\n", ver[0], ver[1], ver[2]);

  printf("Testing closure of WCSLIB world coordinate transformation "
         "routines (twcs.c)\n"
         "----------------------------------------------------------"
         "-----------------\n");

  // List status return messages.
  int status;
  printf("\nList of wcs status return values:\n");
  for (status = 1; status <= 13; status++) {
    printf("%4d: %s.\n", status, wcs_errmsg[status]);
  }

  printf("\nSize of data types (bytes):\n");
  printf("           char:%5"MODZ"u\n", sizeof(char));
  printf("      short int:%5"MODZ"u\n", sizeof(short int));
  printf("            int:%5"MODZ"u\n", sizeof(int));
  printf("       long int:%5"MODZ"u\n", sizeof(long int));
  printf("  long long int:%5"MODZ"u\n", sizeof(long long int));
  printf("          float:%5"MODZ"u\n", sizeof(float));
  printf("         double:%5"MODZ"u\n", sizeof(double));
  printf("         char *:%5"MODZ"u\n", sizeof(char *));
  printf("   char (*)[72]:%5"MODZ"u\n", sizeof(char (*)[72]));
  printf("          int *:%5"MODZ"u\n", sizeof(int *));
  printf("        float *:%5"MODZ"u\n", sizeof(float *));
  printf("       double *:%5"MODZ"u\n", sizeof(double *));
  printf("struct  dpkey *:%5"MODZ"u\n", sizeof(struct dpkey *));
  printf("struct pvcard *:%5"MODZ"u\n", sizeof(struct pvcard *));
  printf("struct pscard *:%5"MODZ"u\n", sizeof(struct pscard *));

  printf("\nSize of structs (bytes/ints):\n");

  char ok[] = "", mismatch[] = " (WARNING, mismatch)";
  char *s = (sizeof(struct auxprm) == sizeof(int)*AUXLEN) ? ok : mismatch;
  printf("         auxprm:%5"MODZ"u /%4"MODZ"u%s\n", sizeof(struct auxprm),
         AUXLEN, s);

  s = (sizeof(struct celprm) == sizeof(int)*CELLEN) ? ok : mismatch;
  printf("         celprm:%5"MODZ"u /%4"MODZ"u%s\n", sizeof(struct celprm),
         CELLEN, s);

  s = (sizeof(struct disprm) == sizeof(int)*DISLEN) ? ok : mismatch;
  printf("         disprm:%5"MODZ"u /%4"MODZ"u%s\n", sizeof(struct disprm),
         DISLEN, s);

  s = (sizeof(struct dpkey)  == sizeof(int)*DPLEN)  ? ok : mismatch;
  printf("          dpkey:%5"MODZ"u /%4"MODZ"u%s\n", sizeof(struct dpkey),
         DPLEN, s);

  s = (sizeof(struct fitskey) == sizeof(int)*KEYLEN) ? ok : mismatch;
  printf("        fitskey:%5"MODZ"u /%4"MODZ"u%s\n", sizeof(struct fitskey),
         KEYLEN, s);

  s = (sizeof(struct fitskeyid) == sizeof(int)*KEYIDLEN) ? ok : mismatch;
  printf("      fitskeyid:%5"MODZ"u /%4"MODZ"u%s\n", sizeof(struct fitskeyid),
         KEYIDLEN, s);

  s = (sizeof(struct linprm) == sizeof(int)*LINLEN) ? ok : mismatch;
  printf("         linprm:%5"MODZ"u /%4"MODZ"u%s\n", sizeof(struct linprm),
         LINLEN, s);

  s = (sizeof(struct prjprm) == sizeof(int)*PRJLEN) ? ok : mismatch;
  printf("         prjprm:%5"MODZ"u /%4"MODZ"u%s\n", sizeof(struct prjprm),
         PRJLEN, s);

  s = (sizeof(struct pscard) == sizeof(int)*PSLEN)  ? ok : mismatch;
  printf("         pscard:%5"MODZ"u /%4"MODZ"u%s\n", sizeof(struct pscard),
         PSLEN, s);

  s = (sizeof(struct pvcard) == sizeof(int)*PVLEN)  ? ok : mismatch;
  printf("         pvcard:%5"MODZ"u /%4"MODZ"u%s\n", sizeof(struct pvcard),
         PVLEN, s);

  s = (sizeof(struct spcprm) == sizeof(int)*SPCLEN) ? ok : mismatch;
  printf("         spcprm:%5"MODZ"u /%4"MODZ"u%s\n", sizeof(struct spcprm),
         SPCLEN, s);

  s = (sizeof(struct spxprm) == sizeof(int)*SPXLEN) ? ok : mismatch;
  printf("         spxprm:%5"MODZ"u /%4"MODZ"u%s\n", sizeof(struct spxprm),
         SPXLEN, s);

  s = (sizeof(struct tabprm) == sizeof(int)*TABLEN) ? ok : mismatch;
  printf("         tabprm:%5"MODZ"u /%4"MODZ"u%s\n", sizeof(struct tabprm),
         TABLEN, s);

  s = (sizeof(struct wcserr) == sizeof(int)*ERRLEN) ? ok : mismatch;
  printf("         wcserr:%5"MODZ"u /%4"MODZ"u%s\n", sizeof(struct wcserr),
         ERRLEN, s);

  s = (sizeof(struct wcsprm) == sizeof(int)*WCSLEN) ? ok : mismatch;
  printf("         wcsprm:%5"MODZ"u /%4"MODZ"u%s\n", sizeof(struct wcsprm),
         WCSLEN, s);

  printf("\n");


  // Set the PVi_ma keyvalues for the longitude axis.
  //------------------------------------------------------------
  // For test purposes, these are set so that the fiducial
  // native coordinates are at the native pole, i.e. so that
  // (phi0,theta0) = (0,90), but without any fiducial offset,
  // i.e. iwith PVi_0a == 0 (by default).
  //------------------------------------------------------------
  PV[0].i = 4;			// Longitude is on axis 4.
  PV[0].m = 1;			// Parameter number 1.
  PV[0].value =  0.0;		// Fiducial native longitude.

  PV[1].i = 4;			// Longitude is on axis 4.
  PV[1].m = 2;			// Parameter number 2.
  PV[1].value = 90.0;		// Fiducial native latitude.

  // Set the PVi_m keyvaluess for the latitude axis.
  PV[2].i = 2;			// Latitude is on axis 2.
  PV[2].m = 1;			// Parameter number 1.
  PV[2].value = -30.0;		// PVi_1.


  // The following routine simulates the actions of a FITS header parser.
  struct wcsprm *wcs = malloc(sizeof(struct wcsprm));
  wcs->flag = -1;
  parser(wcs);

  // Test wcsenq() - note that the struct is unset.
  int nFail0 = 0;
  if (!wcsenq(wcs, WCSENQ_MEM)) {
    nFail0++;
    printf("Unexpected result from wcsenq(): memory.\n");
  }

  // Enable error messaging.
  wcserr_enable(1);

  // Put the struct in bypass mode (as a test).
  wcs->flag = 1;
  if (wcsset(wcs)) {
    wcsperr(wcs, "");
  }

  // Tidy up the wcsprm struct.
  wcstrim(wcs);

  // Test wcsenq().
  if (!wcsenq(wcs, WCSENQ_SET | WCSENQ_BYP)) {
    nFail0++;
    printf("Unexpected result from wcsenq(): set and bypass.\n");
  }

  if (!wcsenq(wcs, WCSENQ_CHK)) {
    nFail0++;
    printf("Unexpected result from wcsenq(): checksum (a).\n");
  }

  // Further test for wcsenq() - an inconsequential change.
  wcs->latpole = 999.001;
  if (wcsenq(wcs, WCSENQ_CHK)) {
    nFail0++;
    printf("Unexpected result from wcsenq(): checksum (b).\n");
  }


  if (nFail0) printf("\n");
  printf("Reporting tolerance %5.1g pixel.\n", tol);

  // Initialize non-celestial world coordinates.
  double time = 1.0;
  double freq = 1.42040595e9 - 180.0 * 62500.0;
  double world1[361][NELEM], world2[361][NELEM];
  for (int k = 0; k < 361; k++) {
    world1[k][0] = 0.0;
    world1[k][1] = 0.0;
    world1[k][2] = 0.0;
    world1[k][3] = 0.0;

    world1[k][2] = time;
    time *= 1.01;

    world1[k][wcs->spec] = 2.99792458e8 / freq;
    freq += 62500.0;
  }

  int nFail1 = 0, stat[361];
  double residmax = 0.0;
  double img[361][NELEM], pixel1[361][NELEM], pixel2[361][NELEM];
  double phi[361], theta[361];
  for (int lat = 90; lat >= -90; lat--) {
    double lat1 = (double)lat;

    int k, lng;
    for (lng = -180, k = 0; lng <= 180; lng++, k++) {
      double lng1 = (double)lng;

      world1[k][wcs->lng] = lng1;
      world1[k][wcs->lat] = lat1;
    }

    if (wcss2p(wcs, 361, NELEM, world1[0], phi, theta, img[0], pixel1[0],
               stat)) {
      printf("  At wcss2p#1 with lat1 == %f\n", lat1);
      wcsperr(wcs, "  ");
      continue;
    }

    if (wcsp2s(wcs, 361, NELEM, pixel1[0], img[0], phi, theta, world2[0],
               stat)) {
      printf("  At wcsp2s with lat1 == %f\n", lat1);
      wcsperr(wcs, "  ");
      continue;
    }

    if (wcss2p(wcs, 361, NELEM, world2[0], phi, theta, img[0], pixel2[0],
               stat)) {
      printf("  At wcss2p#2 with lat1 == %f\n", lat1);
      wcsperr(wcs, "  ");
      continue;
    }

    for (k = 0; k < 361; k++) {
      double resid = 0.0;
      for (int i = 0; i < NAXIS; i++) {
        double r = pixel2[k][i] - pixel1[k][i];
        resid += r*r;
      }

      resid = sqrt(resid);
      if (resid > residmax) residmax = resid;

      if (resid > tol) {
        nFail1++;
        printf("\nClosure error:\n"
               "world1:%18.12f%18.12f%18.12f%18.12f\n"
               "pixel1:%18.12f%18.12f%18.12f%18.12f\n"
               "world2:%18.12f%18.12f%18.12f%18.12f\n"
               "pixel2:%18.12f%18.12f%18.12f%18.12f\n",
          world1[k][0], world1[k][1], world1[k][2], world1[k][3],
          pixel1[k][0], pixel1[k][1], pixel1[k][2], pixel1[k][3],
          world2[k][0], world2[k][1], world2[k][2], world2[k][3],
          pixel2[k][0], pixel2[k][1], pixel2[k][2], pixel2[k][3]);
       }
    }
  }

  printf("wcsp2s/wcss2p: Maximum closure residual = %.1e pixel.\n", residmax);


  // The wcsprm is set in bypass mode (above), so wcsset() should return
  // immediately and hence not fail.
  int nFail2 = 0;
  wcs->pv[2].value = UNDEFINED;
  if ((status = wcsset(wcs))) {
    nFail2 = 1;
    printf("\nBypass mode failure in wcsset().\n");
  }


  // Test wcserr and wcsprintf() as well.
  wcsprintf_set(stdout);
  wcsprintf("\nIGNORE messages marked with 'OK', they test wcserr "
    "(and wcsprintf):\n");

  // Test 1 - take it out of bypass mode, so now wcsset() should fail.
  int nFail3 = 0;
  wcs->pv[2].value = UNDEFINED;
  wcs->flag = 0;
  status = wcsset(wcs);
  nFail3 += check_error(wcs, status, WCSERR_BAD_PARAM,
                        "Invalid parameter value");

  nFail3 += test_errors();


  printf("\n");
  int success = 1;
  if (nFail0) {
    success = 0;
    printf("FAIL: Unexpected result%s from wcsenq().\n",
           (nFail0 = 1) ? "" : "s");
  }

  if (nFail1) {
    success = 0;
    printf("FAIL: %d closure residuals exceed reporting tolerance.\n",
      nFail1);
  }

  if (nFail2) {
    success = 0;
    printf("FAIL: Bypass mode failure in wcsset().\n");
  }

  if (nFail3) {
    success = 0;
    printf("FAIL: %d error messages differ from that expected.\n", nFail3);
  }

  if (success) {
    printf("PASS: All enquiry results returned as expected.\n");
    printf("PASS: All closure residuals are within reporting tolerance.\n");
    printf("PASS: wcsset() in bypass mode operated as expected.\n");
    printf("PASS: All error messages reported as expected.\n");
  }


  // Clean up.
  wcsfree(wcs);
  free(wcs);

  return nFail0 + nFail1 + nFail2 + nFail3;
}

//----------------------------------------------------------------------------

void parser(wcs)

struct wcsprm *wcs;

{
  // In practice a parser would read the FITS header until it encountered
  // the NAXIS keyword which must occur near the start, before any of the
  // WCS keywords.  It would then use wcsini() to allocate memory for
  // arrays in the wcsprm struct and set default values.  In this
  // simulation the header keyvalues are set as global variables.
  wcsini(1, NAXIS, wcs);


  // Now the parser scans the FITS header, identifying WCS keywords and
  // loading their values into the appropriate elements of the wcsprm
  // struct.

  for (int j = 0; j < NAXIS; j++) {
    wcs->crpix[j] = CRPIX[j];
  }

  double *pcij = wcs->pc;
  for (int i = 0; i < NAXIS; i++) {
    for (int j = 0; j < NAXIS; j++) {
      *(pcij++) = PC[i][j];
    }
  }

  for (int i = 0; i < NAXIS; i++) {
    wcs->cdelt[i] = CDELT[i];
  }

  for (int i = 0; i < NAXIS; i++) {
    strcpy(wcs->ctype[i], &CTYPE[i][0]);
  }

  for (int i = 0; i < NAXIS; i++) {
    wcs->crval[i] = CRVAL[i];
  }

  wcs->lonpole = LONPOLE;
  wcs->latpole = LATPOLE;

  wcs->restfrq = RESTFRQ;
  wcs->restwav = RESTWAV;

  wcs->npv = NPV;
  for (int i = 0; i < NPV; i++) {
    wcs->pv[i] = PV[i];
  }

  return;
}

//----------------------------------------------------------------------------

int check_error(struct wcsprm *wcs, int status, int exstatus, char *exmsg)
{
  wcsprintf("\nTest %d...\n", ++itest);

  const char *errmsg = (status ? (wcs->err)->msg : "");
  if (status == exstatus && strcmp(errmsg, exmsg) == 0) {
    wcsperr(wcs, "OK: ");
    wcsprintf("...succeeded.\n");
  } else {
    wcsprintf("Expected error %d: '%s', got\n", exstatus, exmsg);
    wcsperr(wcs, "");
    wcsprintf("...failed.\n");
    return 1;
  }

  return 0;
}

//----------------------------------------------------------------------------

int test_errors()

{
  int nFail = 0, status;
  struct wcsprm wcs;

  // Test 2.
  wcs.flag = -1;
  status = wcsini(1, -32, &wcs);
  nFail += check_error(&wcs, status, WCSERR_MEMORY,
             "naxis must not be negative (got -32)");

  // Test 3.
  wcs.flag = 0;
  status = wcsini(1, 2, &wcs);
  nFail += check_error(&wcs, status, WCSERR_SUCCESS, "");

  // Test 4.
  const char *(multiple_cubeface[2]) = {"CUBEFACE", "CUBEFACE"};
  for (int i = 0; i < 2; i++) {
    strcpy(wcs.ctype[i], &multiple_cubeface[i][0]);
  }
  status = wcsset(&wcs);
  nFail += check_error(&wcs, status, WCSERR_BAD_CTYPE,
             "Multiple CUBEFACE axes (in CTYPE1 and CTYPE2)");

  // Test 5.
  const char *(projection_code[2])   = {"RA---FOO", "DEC--BAR"};
  wcs.flag = 0;
  status = wcsini(1, 2, &wcs);
  for (int i = 0; i < 2; i++) {
    strcpy(wcs.ctype[i], &projection_code[i][0]);
  }
  status = wcsset(&wcs);
  nFail += check_error(&wcs, status, WCSERR_BAD_CTYPE,
             "Unrecognized projection code (FOO in CTYPE1)");

  // Test 6.
  const char *(unmatched[2])         = {"RA---TAN", "FREQ-LOG"};
  wcs.flag = 0;
  status = wcsini(1, 2, &wcs);
  for (int i = 0; i < 2; i++) {
    strcpy(wcs.ctype[i], &unmatched[i][0]);
  }
  status = wcsset(&wcs);
  nFail += check_error(&wcs, status, WCSERR_BAD_CTYPE,
             "Unmatched celestial axes");

  wcsfree(&wcs);

  return nFail;
}
