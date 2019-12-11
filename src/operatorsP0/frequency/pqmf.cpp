/* -*- c-basic-offset: 3; mode:c++ -*-
 *
 * Copyright (c), GREYC.
 * All rights reserved
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of the GREYC, nor the name of its
 *     contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 *
 * 
 * For more information, refer to:
 * https://clouard.users.greyc.fr/Pandore
 */

/**
 * @author Ludovic SOLTYS - 02-02-2003
 * @author Jalal Fadili - 2005-03-18
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file pqmf.h
 * Operator source: Edition de fichiers collection de filtres QMF
 * References:
 *   The books by Daubechies and Wickerhauser.
 */
float norm( float *x, int size ) {
   float nrm = 0;
   
   for (int i = 1; i <= size; i++) {
      nrm += (x[i] * x[i]);
   }
   
   return (float)sqrt(nrm);
}

Errc PQmf( char *name, char *ordre, Collection &c ) {
   int size = 0, i;
   static float *f;
   float normf;
   
   int order = atoi(ordre);
   
   if (strcmp(name, "beylkin") == 0) {
      size = 18;
      static float h[19] = { 0.0f, .099305765374f, .424215360813f, .699825214057f, 
			     .449718251149f, -.110927598348f, -.264497231446f, 
			     .026900308804f, .155538731877f, -.017520746267f, 
			     -.088543630623f, .019679866044f, .042916387274f, 
			     -.017460408696f, -.014365807969f, .010040411845f, 
			     .001484234782f, -.002736031626f, .000640485329f };
      
      f = h;
   }
   
   if (strcmp(name, "coiflet") == 0) {
      size = order * 6;
      if (order == 1) {
	 static float h[7] = { 0.0f, 	.038580777748f, -.126969125396f, -.077161555496f, 
			       .607491641386f, .745687558934f, .226584265197f };
	 f = h;
      }
      
      
      if (order == 2) {
	 static float h[13] = { 0.0f, 	.016387336463f, -.041464936782f, -.067372554722f, 
				.386110066823f, .812723635450f, .417005184424f, 
				-.076488599078f, -.059434418646f, .023680171947f, 
				.005611434819f, -.001823208871f, -.000720549445f };
	 f = h;
      }
      
      
      if (order == 3) {
	 static float h[19] = { 0.0f, 	-.003793512864f, .007782596426f, .023452696142f, 
				-.065771911281f, -.061123390003f, .405176902410f, 
				.793777222626f, .428483476378f, -.071799821619f, 
				-.082301927106f, .034555027573f, .015880544864f, 
				-.009007976137f, -.002574517688f, .001117518771f, 
				.000466216960f, -.000070983303f, -.000034599773f };
	 f = h;
      }
      
      if (order == 4) {
	 static float h[25] = { 0.0f, 	.000892313668f, -.001629492013f, -.007346166328f, 
				.016068943964f, .026682300156f, -.081266699680f, 
				-.056077313316f, .415308407030f, .782238930920f, 
				.434386056491f, -.066627474263f, -.096220442034f, 
				.039334427123f, .025082261845f, -.015211731527f, 
				-.005658286686f, .003751436157f, .001266561929f, 
				-.000589020757f, -.000259974552f, .000062339034f, 
				.000031229876f, -.000003259680f, -.000001784985f };
	 f = h;		
      }
	 
      if (order == 5) {
	 static float h[31] = { 0.0f, 	-.000212080863f, .000358589677f, .002178236305f, 
				-.004159358782f, -.010131117538f, .023408156762f, 
				.028168029062f, -.091920010549f, -.052043163216f, 
				.421566206729f, .774289603740f, .437991626228f, 
				-.062035963906f, -.105574208706f, .041289208741f, 
				.032683574283f, -.019761779012f, -.009164231153f, 
				.006764185419f, .002433373209f, -.001662863769f, 
				-.000638131296f, .000302259520f, .000140541149f, 
				-.000041340484f, -.000021315014f, .000003734597f, 
				.000002063806f, -.000000167408f, -.000000095158f };
			
	 f = h;
      }
		
   }
	 
 
   if (strcmp(name, "daubechies") == 0) {
      size = order;
      
      if (order == 4) {
	 static float h[5] = { 0.0f,  .482962913145f, .836516303738f, .224143868042f, -.129409522551f };
	 f = h;
      }
	 
      if (order == 6) {
	 static float h[7] = { 0.0f, 	.332670552950f, .806891509311f, .459877502118f, 
			       -.135011020010f, -.085441273882f, .035226291882f };
	 f = h;
      }
	 
      if (order == 8) {
	 static float h[9] = { 0.0f, 	.230377813309f, .714846570553f, .630880767930f,
			       -.027983769417f, -.187034811719f,	.030841381836f, 
			       .032883011667f, -.010597401785f };
	 f = h;
      }
	 
      if (order == 10) {
	 static float h[11] = { 0.0f, 	.160102397974f, .603829269797f, .724308528438f, 
				.138428145901f, -.242294887066f, -.032244869585f, 
				.077571493840f, -.006241490213f, -.012580751999f, 
				.003335725285f };
	 f = h;
      }
	 
      if (order == 12) {
	 static float h[13] = { 0.0f, 	.111540743350f, .494623890398f, .751133908021f, 
				.315250351709f, -.226264693965f, -.129766867567f, 
				.097501605587f, .027522865530f, -.031582039317f, 
				.000553842201f, .004777257511f, -.001077301085f };
	 f = h;
      }
	 
      if (order == 14) {
	 static float h[15] = { 0.0f, 	.077852054085f, .396539319482f, .729132090846f, 
				.469782287405f, -.143906003929f, -.224036184994f, 
				.071309219267f, .080612609151f, -.038029936935f, 
				-.016574541631f, .012550998556f, .000429577973f, 
				-.001801640704f, .000353713800f };
	 f = h;
      }
	 
      if (order == 16) {
	 static float h[17] = { 0.0f, 	.054415842243f, .312871590914f, .675630736297f, 
				.585354683654f, -.015829105256f, -.284015542962f, 
				.000472484574f, .128747426620f, -.017369301002f, 
				-.044088253931f, .013981027917f, .008746094047f, 
				-.004870352993f, -.000391740373f, .000675449406f, 
				-.000117476784f };
	 f = h;
      }
	 
      if (order == 18) {
	 static float h[19] = { 0.0f, 	.038077947364f, .243834674613f, .604823123690f, 
				.657288078051f, .133197385825f, -.293273783279f, 
				-.096840783223f, .148540749338f, .030725681479f, 
				-.067632829061f, .000250947115f, .022361662124f, 
				-.004723204758f, -.004281503682f, .001847646883f, 
				.000230385764f, -.000251963189f, .000039347320f };
	 f = h;
      }
	 
      if (order == 20) {
	 static float h[21] = { 0.0f, 	.026670057901f, .188176800078f, .527201188932f, 
				.688459039454f, .281172343661f, -.249846424327f, 
				-.195946274377f, .127369340336f, .093057364604f, 
				-.071394147166f, -.029457536822f, .033212674059f, 
				.003606553567f, -.010733175483f, .001395351747f, 
				.001992405295f, -.000685856695f, -.000116466855f, 
				.000093588670f, -.000013264203f };
	 f = h;
      }

		
   }
	 
 
   if (strcmp(name, "symmlet") == 0) {
      size = order * 2;

      if (order == 4) {
	 static float h[9] = { 0.0f, 	-.107148901418f, -.041910965125f, .703739068656f, 
			       1.136658243408f, .421234534204f, -.140317624179f, 
			       -.017824701442f, .045570345896f };
	 f = h;

      }
			 
      if (order == 5) {
	 static float h[11] = { 0.0f, 	.038654795955f, .041746864422f, -.055344186117f, 
				.281990696854f, 1.023052966894f, .896581648380f, 
				.023478923136f, -.247951362613f, -.029842499869f, 
				.027632152958f };
	 f = h;
      }
	 
      if (order == 6) {
	 static float h[13] = { 0.0f, 	.021784700327f, .004936612372f, -.166863215412f, 
				-.068323121587f, .694457972958f, 1.113892783926f, 
				.477904371333f, -.102724969862f, -.029783751299f, 
				.063250562660f, .002499922093f, -.011031867509f };
	 f = h;
      }
	 
      if (order == 7) {
	 static float h[15] = { 0.0f, 	.003792658534f, -.001481225915f, -.017870431651f, 
				.043155452582f, .096014767936f, -.070078291222f, 
				.024665659489f, .758162601964f, 1.085782709814f, 
				.408183939725f, -.198056706807f, -.152463871896f, 
				.005671342686f, .014521394762f };
	 f = h;
      }
	 
      if (order == 8) {
	 static float h[17] = { 0.0f, 	.002672793393f, -.000428394300f, -.021145686528f, 
				.005386388754f, .069490465911f, -.038493521263f, 
				-.073462508761f, .515398670374f, 1.099106630537f, 
				.680745347190f, -.086653615406f, -.202648655286f, 
				.010758611751f, .044823623042f, -.000766690896f,  
				-.004783458512f };
	 f = h;
      }
	 
      if (order == 9) {
	 static float h[19] = { 0.0f, 	.001512487309f, -.000669141509f, -.014515578553f, 
				.012528896242f, .087791251554f, -.025786445930f, 
				-.270893783503f, .049882830959f, .873048407349f, 
				1.015259790832f, .337658923602f, -.077172161097f, 
				.000825140929f, .042744433602f, -.016303351226f, 
				-.018769396836f, .000876502539f, .001981193736f };
	 f = h;
      }
	 
      if (order == 10) {
	 static float h[21] = { 0.0f, 	.001089170447f, .000135245020f, -.012220642630f, 
				-.002072363923f, .064950924579f, .016418869426f, 
				-.225558972234f, -.100240215031f, .667071338154f, 
				1.088251530500f, .542813011213f, -.050256540092f, 
				-.045240772218f, .070703567550f, .008152816799f, 
				-.028786231926f, -.001137535314f, .006495728375f, 
				.000080661204f, -.000649589896f };
	 f = h;
      }

   }
 
   if  (strcmp(name, "vaidyanathan") == 0) {
      size = 24;
      static float h[25] = { 0.0f, 		-.000062906118f, .000343631905f, -.000453956620f, 
			     -.000944897136f, .002843834547f,	.000708137504f, 
			     -.008839103409f, .003153847056f, .019687215010f, 
			     -.014853448005f, -.035470398607f, .038742619293f, 
			     .055892523691f, -.077709750902f, -.083928884366f, 
			     .131971661417f, .135084227129f, -.194450471766f, 
			     -.263494802488f, .201612161775f, .635601059872f, 
			     .572797793211f, .250184129505f, .045799334111f };
		
      f = h;
   }
 
   if (strcmp(name, "battle") == 0) {
      if (order == 1) {
	 size = 12;
	 static float g[13] = { 0.0f, 	0.578163f, 0.280931f, -0.0488618f, 
				-0.0367309f, 0.012003f, 0.00706442f, 
				-0.00274588f, -0.00155701f, 0.000652922f, 
				0.000361781f, -0.000158601f, -0.0000867523f };
	 static float h[13];
	 int s = -1;
		
	 for (int k = 0; k < size; k++) {
	    h[size - k] = 1 / s * g[k];
	    s = -s;
	 }
	 f = h;
      }
	 
      if (order == 3) {
	 size = 21;
	 static float g[22] = { 0.0f, 	0.541736f, 0.30683f, -0.035498f,	
				-0.0778079f, 0.0226846f, 0.0297468f, 
				-0.0121455f, -0.0127154f, 0.00614143f, 
				0.00579932f, -0.00307863f, -0.00274529f, 
				0.00154624f, 0.00133086f, -0.000780468f, 
				-0.00065562f, 0.000395946f, 0.000326749f, 
				-0.000201818f, -0.000164264f, 0.000103307f };
	 static float h[22];
	 int s = -1;
		
	 for (int k = 0; k < size; k++) {
	    h[size - k]= 1 / s * g[k];
	    s = -s;
	 }
	 f = h;
      }
	 
      if (order == 5) {
	 size = 30;
	 static float g[31] = { 0.0f, 	0.528374f, 0.312869f, -0.0261771f, 
				-0.0914068f, 0.0208414f, 0.0433544f, 
				-0.0148537f, -0.0229951f, 0.00990635f, 
				0.0128754f, -0.00639886f, -0.00746848f, 
				0.00407882f, 0.00444002f, -0.00258816f, 
				-0.00268646f, 0.00164132f, 0.00164659f, 
				-0.00104207f, -0.00101912f, 0.000662836f, 
				0.000635563f, -0.000422485f, -0.000398759f, 
				0.000269842f, 0.000251419f, -0.000172685f, 
				-0.000159168f, 0.000110709f, 0.000101113f };
	 static float h[31];
	 int s = -1;
		
	 for (int k = 0; k < size; k++) {
	    h[size-k] = 1 / s * g[k];
	    s = -s;
	 }
	 f = h;
      }
				
   }

   // le filtre "Haar" est choisi filtre par défaut
   if (strcmp(name, "haar") == 0 || size == 0) {
      size = 2;
      static float h[3] = { 0.0f,  0.70710678f, 0.70710678f };
      f = h;
#if defined _WIN32 || defined WIN32
      name = _strdup("haar");
#else
      name = strdup("haar");
#endif
   }

   normf = norm(f, size);
   for (i = 1; i <= size; i++) {
      f[i] = f[i] / normf;
   }
   c.SETARRAY(name, static float, f, size + 1);

   c.SETARRAY("filtre", Char, name, 15);

   return SUCCESS;
}


//#define MAIN
#ifdef MAIN

#define USAGE   "usage: %s filter_name order [col_out|-]"
#define PARC    2
#define FINC    0
#define FOUTC   1
#define MASK    0

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject *mask;              // The mask.
   Pobject *objin[FINC + 1];   // The input objects.
   Pobject *objs[FINC + 1];    // The source objects masked by mask.
   Pobject *objout[FOUTC + 1]; // The output object unmasked by mask.
   Pobject *objd[FOUTC + 1];   // The result object.
   char* parv[PARC + 1];       // The input parameters.
   
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   objd[0] = new Collection;
   Collection * const coll = (Collection*)objd[0];
   result = PQmf(parv[0], parv[1], *coll);
   
   if (result) {
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
   }
   Exit(result);
   return 0;
}

#endif
