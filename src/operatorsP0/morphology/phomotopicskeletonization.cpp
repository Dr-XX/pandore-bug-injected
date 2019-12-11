/* -*- c-basic-offset: 3 -*-
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
 * @author Sébastien Fourey - 2004/02/05
 */

#include <stdlib.h>
#include <stdio.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file phomotopicskeletonization.cpp
 */

#define BYTE unsigned char

#define ADJ6  0
#define ADJ6P 1
#define ADJ18 2
#define ADJ26 3
#define CENTRE 13

#define FIN 50

#define MASK_CONFIG 0x07FFFFFF
#define MASK_CENTRE 0x00002000
#define MASK_26VOISINS 0x07FFDFFF
#define MASK_18VOISINS 0x2EBDEBA
#define MASK_6VOISINS  0x415410
#define MASK_6PVOISINS 0x415410

#define MASK_4  0x10
#define MASK_10 0x400
#define MASK_12 0x1000
#define MASK_14 0x4000
#define MASK_16 0x10000
#define MASK_22 0x400000

#define MASK_0 0x1
#define MASK_1 0x2
#define MASK_2 0x4
#define MASK_3 0x8

#define MASK_5 0x20
#define MASK_6 0x40
#define MASK_7 0x80
#define MASK_8 0x100
#define MASK_9 0x200

#define MASK_11 0x800
#define MASK_13 0x2000
#define MASK_15 0x8000

#define MASK_17 0x20000
#define MASK_18 0x40000
#define MASK_19 0x80000
#define MASK_20 0x100000
#define MASK_21 0x200000

#define MASK_23 0x800000
#define MASK_24 0x1000000
#define MASK_25 0x2000000
#define MASK_26 0x4000000

#define MASK_CUBE0 0x361B
#define MASK_CUBE1 0x6C36
#define MASK_CUBE2 0x1B0D8
#define MASK_CUBE3 0x361B0
#define MASK_CUBE4 0x6C3600
#define MASK_CUBE5 0xD86C00
#define MASK_CUBE6 0x361B000
#define MASK_CUBE7 0x6C36000

typedef unsigned long Config;

#define VOISIN(TABLEAU,X,Y,Z,DIRECTION) TABLEAU[(X)+dx_of[DIRECTION]][(Y)+dy_of[DIRECTION]][(Z)+dz_of[DIRECTION]]

char adj(char p1, char p2, char n_adj);
inline Config cnfComplementaire(Config c);
inline Config cnfAdd(Config c, char n);
char cnfExtremite(Config c);
int cnfCardinal(Config c);

Config cnfVGeodesique6(Config cnf);
Config cnfVGeodesique6P(Config cnf);
Config cnfVGeodesique18(Config cnf);
Config cnfVGeodesique26(Config cnf);

Config cnfVGeodesique6_3(Config cnf);
Config cnfVGeodesique6P_5(Config cnf);
Config cnfVGeodesique18_3(Config cnf);

char cnfUneC6C(const Config c);
char cnfUneC18C(const Config c);
char cnfUneC26C(const Config c);

char cnfUneC6C_FILE(const Config c);
char cnfUneC18C_FILE(const Config c);
char cnfUneC26C_FILE(const Config c);

char cnfUneC6C_FILE(const Config c);

char cnfUneCC_FILE(const Config c, const int liste_voisins[][32]);
char cnfDeuxCC_FILE(const Config c, const int liste_voisins[][32]);
char cnfNombreCC_FILE(const Config c, const int liste_voisins[][32]);

/*
 * Fonctions de test des caractersisations
 * locales de points simples, P-simples et surfaces.
 */

typedef char (*FonctionTestSimple) (Config);
typedef char (*FonctionTestPSimple) (Config, Config);
typedef char (*FonctionTestSurface) (Config);

char cnfSimple6(Config c);
char cnfSimple6P(Config c);
char cnfSimple18(Config c);
char cnfSimple26(Config c);

char cnfPSimple6(Config c, Config mask_p);
char cnfPSimple6P(Config c, Config mask_p);
char cnfPSimple18(Config c, Config mask_p);
char cnfPSimple26(Config c, Config mask_p);

char cnfExtremite6(Config c);
char cnfExtremite6P(Config c);
char cnfExtremite18(Config c);
char cnfExtremite26(Config c);

/* Retour: 3 ou 0 */
char cnfSurface6(Config c);
char cnfSurface6P(Config c);
char cnfSurface18(Config c);
char cnfSurface26(Config c);

/* Retour: 1 ou 0 */
char cnfIsthme6(Config c);
char cnfIsthme6P(Config c);
char cnfIsthme18(Config c);
char cnfIsthme26(Config c);

/* Retour: 1 ou 0 */
char cnfBord6(Config c);
char cnfBord6P(Config c);
char cnfBord18(Config c);
char cnfBord26(Config c);

/* Retour: 3 ou 0 */
char cnfJonctionSurface6(Config c);
char cnfJonctionSurface6P(Config c);
char cnfJonctionSurface18(Config c);
char cnfJonctionSurface26(Config c);

/* Retour: 1 ou 0 */
char cnfSurfaceBertrand6(Config c);
char cnfSurfaceBertrand6P(Config c);
char cnfSurfaceBertrand18(Config c);
char cnfSurfaceBertrand26(Config c);

/* Retour: 3 ou 1 ou 0 */
char cnfSurfaceBertrandOuIsthme6(Config c);
char cnfSurfaceBertrandOuIsthme6P(Config c);
char cnfSurfaceBertrandOuIsthme18(Config c);
char cnfSurfaceBertrandOuIsthme26(Config c);

/*  Retourne dans a et b les composantes 6-connexes du complementaire
    de cnf qui sont 6-adjacentes au point central, 
    si elles sont deux exactement, retourne VRAI (1)
    sinon retourne FAUX (0) */
char cnfLabel6(Config cnf, Config * axx, Config * bxx);
char cnfSurfaceForte26(Config c);
char cnfSurfaceForte26Ext6(Config c);
char cnfSurfaceForte26Ext18(Config c);
char cnfSurfaceForte26Ext26(Config c);


const char ORDRE_USUEL[4] = { 2, 3, 2, 1 };
const char LCHEMIN[4] = { 5, 7, 5, 3 };
const char ORDRE_USUEL_TESTS[4] = { 3, 5, 3, 1 };
const char N_BARRE[4] = { ADJ26, ADJ18, ADJ6P, ADJ6 };

const char Tab_Indices[3][3][3] = { {{0, 9, 18}, {3, 12, 21}, {6, 15, 24}},
{{1, 10, 19}, {4, 13, 22}, {7, 16, 25}},
{{2, 11, 20}, {5, 14, 23}, {8, 17, 26}}
};

const int dx_of[27] = { -1, -1, -1, -1, -1, -1, -1, -1, -1,
   0, 0, 0, 0, 0, 0, 0, 0, 0,
   1, 1, 1, 1, 1, 1, 1, 1, 1
};

const int dy_of[27] = { -1, -1, -1, 0, 0, 0, 1, 1, 1,
   -1, -1, -1, 0, 0, 0, 1, 1, 1,
   -1, -1, -1, 0, 0, 0, 1, 1, 1
};

const int dz_of[27] = { -1, 0, 1, -1, 0, 1, -1, 0, 1,
   -1, 0, 1, -1, 0, 1, -1, 0, 1,
   -1, 0, 1, -1, 0, 1, -1, 0, 1
};

const int delta[27][3] = {
   {-1, -1, -1}, {-1, -1, 0}, {-1, -1, 1}, {-1, 0, -1},
   {-1, 0, 0}, {-1, 0, 1}, {-1, 1, -1}, {-1, 1, 0}, {-1, 1, 1},
   {0, -1, -1}, {0, -1, 0}, {0, -1, 1},
   {0, 0, -1}, {0, 0, 0}, {0, 0, 1},
   {0, 1, -1}, {0, 1, 0}, {0, 1, 1}, {1, -1, -1}, {1, -1, 0},
   {1, -1, 1}, {1, 0, -1}, {1, 0, 0},
   {1, 0, 1}, {1, 1, -1}, {1, 1, 0}, {1, 1, 1}
};

/* Table des adjacences entre points du 26 voisinage */

const unsigned int bits_adj[4][32] = {
   {0x20a, 0x415, 0x822, 0x1051, 0x20aa, 0x4114, 0x8088, 0x10150, 0x200a0,
    0x41401,
    0x82a02, 0x104404, 0x20a208, 0x415410, 0x822820, 0x1011040, 0x202a080,
    0x4014100, 0x280200, 0x540400, 0x880800, 0x1441000, 0x2a82000,
    0x4504000, 0x2208000, 0x5410000, 0x2820000},
   {0x20a, 0x415, 0x822, 0x1051, 0x20aa, 0x4114, 0x8088, 0x10150, 0x200a0,
    0x41401,
    0x82a02, 0x104404, 0x20a208, 0x415410, 0x822820, 0x1011040, 0x202a080,
    0x4014100, 0x280200, 0x540400, 0x880800, 0x1441000, 0x2a82000,
    0x4504000, 0x2208000, 0x5410000, 0x2820000},
   {0x161a, 0x2e3d, 0x4c32, 0xb2d3, 0x175ef, 0x26996, 0x19098, 0x3a178,
    0x340b0,
    0x2c340b, 0x5c7a17, 0x986426, 0x165a659, 0x2ebdeba, 0x4d32d34,
    0x32130c8,
    0x742f1d0, 0x68161a0, 0x681600, 0xf42e00, 0xc84c00, 0x34cb200,
    0x7bd7400, 0x65a6800, 0x2619000, 0x5e3a000, 0x2c34000},
   {0x361a, 0x7e3d, 0x6c32, 0x1b6d3, 0x3ffef, 0x36d96, 0x1b098, 0x3f178,
    0x360b0,
    0x6c341b, 0xfc7a3f, 0xd86436, 0x36da6db, 0x7ffdfff, 0x6db2db6,
    0x36130d8,
    0x7e2f1f8, 0x6c161b0, 0x683600, 0xf47e00, 0xc86c00, 0x34db600,
    0x7bffe00, 0x65b6c00, 0x261b000, 0x5e3f000, 0x2c36000}
};

const Config Les6voisins[27] = {
   0x20a, 0x415, 0x822, 0x1051, 0xaa, 0x4114, 0x8088, 0x10150,
   0x200a0, 0x41401, 0x80a02, 0x104404, 0x208208, 0x415410,
   0x820820, 0x1011040, 0x2028080, 0x4014100, 0x280200, 0x540400, 0x880800,
   0x1441000, 0x2a80000, 0x4504000, 0x2208000, 0x5410000, 0x2820000
};

const Config Les18voisins[27] = {
   0x161a, 0xe3d, 0x4c32, 0x92d3, 0x155ef, 0x24996, 0x19098, 0x38178,
   0x340b0, 0x2c140b, 0x5c5a17, 0x984426, 0x1658659, 0x2ebdeba, 0x4d30d34,
   0x32110c8, 0x742d1d0, 0x68141a0, 0x681600, 0xf40e00, 0xc84c00,
   0x34c9200,
   0x7bd5400, 0x65a4800, 0x2619000, 0x5e38000, 0x2c34000
};

const Config Les26voisins[27] = {
   0x161a, 0x5e3d, 0x4c32, 0x196d3, 0x3dfef, 0x34d96, 0x19098, 0x3d178,
   0x340b0, 0x6c141b, 0xfc5a3f, 0xd84436, 0x36d86db, 0x7ffdfff, 0x6db0db6,
   0x36110d8, 0x7e2d1f8, 0x6c141b0, 0x681600, 0xf45e00, 0xc84c00,
   0x34d9600,
   0x7bfde00, 0x65b4c00, 0x2619000, 0x5e3d000, 0x2c34000
};

const int Liste6voisins[27][32] = { {1, 3, 9, 127},
{0, 2, 4, 10, 127},
{1, 5, 11, 127},
{0, 4, 6, 12, 127},
{1, 3, 5, 7, 127},
{2, 4, 8, 14, 127},
{3, 7, 15, 127},
{4, 6, 8, 16, 127},
{5, 7, 17, 127},
{0, 10, 12, 18, 127},
{1, 9, 11, 19, 127},
{2, 10, 14, 20, 127},
{3, 9, 15, 21, 127},
{4, 10, 12, 14, 16, 22, 127},
{5, 11, 17, 23, 127},
{6, 12, 16, 24, 127},
{7, 15, 17, 25, 127},
{8, 14, 16, 26, 127},
{9, 19, 21, 127},
{10, 18, 20, 22, 127},
{11, 19, 23, 127},
{12, 18, 22, 24, 127},
{19, 21, 23, 25, 127},
{14, 20, 22, 26, 127},
{15, 21, 25, 127},
{16, 22, 24, 26, 127},
{17, 23, 25, 127}
};

const int Liste18voisins[27][32] = {
   {1, 3, 4, 9, 10, 12, 127},
   {0, 2, 3, 4, 5, 9, 10, 11, 127},
   {1, 4, 5, 10, 11, 14, 127},
   {0, 1, 4, 6, 7, 9, 12, 15, 127},
   {0, 1, 2, 3, 5, 6, 7, 8, 10, 12, 14, 16, 127},
   {1, 2, 4, 7, 8, 11, 14, 17, 127},
   {3, 4, 7, 12, 15, 16, 127},
   {3, 4, 5, 6, 8, 15, 16, 17, 127},
   {4, 5, 7, 14, 16, 17, 127},
   {0, 1, 3, 10, 12, 18, 19, 21, 127},
   {0, 1, 2, 4, 9, 11, 12, 14, 18, 19, 20, 22, 127},
   {1, 2, 5, 10, 14, 19, 20, 23, 127},
   {0, 3, 4, 6, 9, 10, 15, 16, 18, 21, 22, 24, 127},
   {1, 3, 4, 5, 7, 9, 10, 11, 12, 14, 15, 16, 17, 19, 21, 22, 23, 25, 127},
   {2, 4, 5, 8, 10, 11, 16, 17, 20, 22, 23, 26, 127},
   {3, 6, 7, 12, 16, 21, 24, 25, 127},
   {4, 6, 7, 8, 12, 14, 15, 17, 22, 24, 25, 26, 127},
   {5, 7, 8, 14, 16, 23, 25, 26, 127},
   {9, 10, 12, 19, 21, 22, 127},
   {9, 10, 11, 18, 20, 21, 22, 23, 127},
   {10, 11, 14, 19, 22, 23, 127},
   {9, 12, 15, 18, 19, 22, 24, 25, 127},
   {10, 12, 14, 16, 18, 19, 20, 21, 23, 24, 25, 26, 127},
   {11, 14, 17, 19, 20, 22, 25, 26, 127},
   {12, 15, 16, 21, 22, 25, 127},
   {15, 16, 17, 21, 22, 23, 24, 26, 127},
   {14, 16, 17, 22, 23, 25, 127}
};

const int Liste26voisins[27][32] = {
   {1, 3, 4, 9, 10, 12, 127},
   {0, 2, 3, 4, 5, 9, 10, 11, 12, 14, 127},
   {1, 4, 5, 10, 11, 14, 127},
   {0, 1, 4, 6, 7, 9, 10, 12, 15, 16, 127},
   {0, 1, 2, 3, 5, 6, 7, 8, 9, 10, 11, 12, 14, 15, 16, 17, 127},
   {1, 2, 4, 7, 8, 10, 11, 14, 16, 17, 127},
   {3, 4, 7, 12, 15, 16, 127},
   {3, 4, 5, 6, 8, 12, 14, 15, 16, 17, 127},
   {4, 5, 7, 14, 16, 17, 127},
   {0, 1, 3, 4, 10, 12, 18, 19, 21, 22, 127},
   {0, 1, 2, 3, 4, 5, 9, 11, 12, 14, 18, 19, 20, 21, 22, 23, 127},
   {1, 2, 4, 5, 10, 14, 19, 20, 22, 23, 127},
   {0, 1, 3, 4, 6, 7, 9, 10, 15, 16, 18, 19, 21, 22, 24, 25, 127},
   {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 15, 16, 17, 18,
    19, 20, 21, 22, 23, 24, 25, 26, 127},
   {1, 2, 4, 5, 7, 8, 10, 11, 16, 17, 19, 20, 22, 23, 25, 26, 127},
   {3, 4, 6, 7, 12, 16, 21, 22, 24, 25, 127},
   {3, 4, 5, 6, 7, 8, 12, 14, 15, 17, 21, 22, 23, 24, 25, 26, 127},
   {4, 5, 7, 8, 14, 16, 22, 23, 25, 26, 127},
   {9, 10, 12, 19, 21, 22, 127},
   {9, 10, 11, 12, 14, 18, 20, 21, 22, 23, 127},
   {10, 11, 14, 19, 22, 23, 127},
   {9, 10, 12, 15, 16, 18, 19, 22, 24, 25, 127},
   {9, 10, 11, 12, 14, 15, 16, 17, 18, 19, 20, 21, 23, 24, 25, 26, 127},
   {10, 11, 14, 16, 17, 19, 20, 22, 25, 26, 127},
   {12, 15, 16, 21, 22, 25, 127},
   {12, 14, 15, 16, 17, 21, 22, 23, 24, 26, 127},
   {14, 16, 17, 22, 23, 25, 127}
};

const char taille_voisins[4] = { 6, 6, 19, 27 };


#define OBJ_TAILLE(O,X,Y,Z) X=O.tx; Y=O.ty; Z=O.tz
#define POBJ_TAILLE(O,X,Y,Z) X=O->tx; Y=O->ty; Z=O->tz
#define POBJ_ASPECT(O,X,Y,Z) X=O->aspectx; Y=O->aspecty; Z=O->aspectz
#define POBJ_CDG(O,X,Y,Z) X=O->cdg_x; Y=O->cdg_y; Z=O->cdg_z


#define POBJ_SETTAILLE(O,X,Y,Z) O->tx=X; O->ty=Y; O->tz=Z
#define POBJ_SETASPECT(O,X,Y,Z) O->aspectx=X; O->aspecty=Y; O->aspectz=Z
#define POBJ_SETCDG(O,X,Y,Z) O->cdg_x=X; O->cdg_y=Y; O->cdg_z=Z

#ifndef UShort
#define UShort unsigned short
#endif

#ifndef ULong
#define ULong unsigned long
#endif


#ifndef WORD
#define WORD unsigned int
#endif

#ifndef BYTE
#define BYTE unsigned char
#endif

#define NORD  0
#define SUD   1
#define EST   2
#define OUEST 3
#define HAUT  4
#define BAS   5
#define AUCUNE_DIR 6

#define DIR_NORD   1
#define DIR_SUD    2
#define DIR_EST    4
#define DIR_OUEST  8
#define DIR_HAUT  16
#define DIR_BAS   3

typedef struct _Objet3D {
   UShort tx, ty, tz;
   BYTE ***matrice;
   float cdg_x, cdg_y, cdg_z;
   float aspectx, aspecty, aspectz;
} Objet3D;



#define X 0
#define Y 1
#define Z 2

#define VECT_SUM(V,A,B) V[X]=A[X]+B[X]; V[Y]=A[Y]+B[Y]; V[Z]=A[Z]+B[Z]
#define VECT_DIFF(V,A,B) V[X]=A[X]-B[X]; V[Y]=A[Y]-B[Y]; V[Z]=A[Z]-B[Z]
#define VECT_EQUAL(A,B) A[X]==B[X] && A[Y]==B[Y] && A[Z]==B[Z]
#define VECT_DIV(A,B,D) A[X]=B[X]/D; A[Y]=B[Y]/D; A[Z]=B[Z]/D


typedef struct recpoint {
   UShort x, y, z;
   BYTE directions;
} RecPoint;

typedef struct listepoints {

   ULong nb_points;
   ULong taille_tableaux;

   ULong n_courant;
   BYTE tableau_courant;
   ULong courant;

   int tableau_dernier;
   ULong dernier;

   RecPoint **tableaux;
} ListePoints;


void objAllouer(Objet3D * objet, UShort tx, UShort ty, UShort tz);

void objFree(Objet3D * objet);
void objClear(Objet3D * objet);
void objInit(Objet3D * objet);
void objCopy(Objet3D * dest, Objet3D * src);

inline void objGetTaille(Objet3D * objet,
			 UShort * tx, UShort * ty, UShort * tz);

int Entete(FILE * fichier, char *chaine);

ListePoints lpCreate();
int lpInit(ListePoints * pliste);
int lpInitTailleTableaux(ListePoints * pliste, ULong n);
void lpFree(ListePoints * pliste);
int lpCopy(ListePoints * dest, ListePoints * src);

void lpEchange(ListePoints * liste, ULong i, ULong j);
void lpMelange(ListePoints * liste);


void lpDebut(ListePoints * liste);

RecPoint *lpSuivant(ListePoints * liste);
inline char lpFin(ListePoints * liste);
inline char lpVide(ListePoints * liste);
void lpRaz(ListePoints * liste);
void lpAjoute(ListePoints * liste, UShort, UShort, UShort, BYTE);

void EcrireDWord(FILE * fichier, unsigned long w);
unsigned long LireDWord(FILE * fichier);


inline int int_min(int a, int b);
inline int int_max(int a, int b);
inline float float_min(float a, float b);
inline float float_max(float a, float b);

enum { AXE_OX, AXE_OY, AXE_OZ };

BYTE objOrientation(Objet3D * objet, UShort x, UShort y, UShort z);

Config objGetConfig(Objet3D * objet, UShort x, UShort y, UShort z);

void objMarque6Bord(Objet3D * objet, BYTE valeur);
void objMarque6PBord(Objet3D * objet, BYTE valeur);
void objMarque18Bord(Objet3D * objet, BYTE valeur);
void objMarque26Bord(Objet3D * objet, BYTE valeur);

BYTE
objOrientation(Objet3D * objet, UShort x, UShort y, UShort z)
{
   BYTE res = 0;
   BYTE ***m;

   m = objet->matrice;

   if (!m[x][y + 1][z])
      res |= 1 << NORD;
   if (!m[x][y - 1][z])
      res |= 1 << SUD;
   if (!m[x + 1][y][z])
      res |= 1 << EST;
   if (!m[x - 1][y][z])
      res |= 1 << OUEST;
   if (!m[x][y][z + 1])
      res |= 1 << HAUT;
   if (!m[x][y][z - 1])
      res |= 1 << BAS;
   return res;
}

Config
objGetConfig(Objet3D * objet, UShort x, UShort y, UShort z)
{
   Config res = 0;
   BYTE ***matrice = objet->matrice;
   BYTE *p = matrice[x - 1][y - 1] + (z - 1);
   Config mask = 1;
   BYTE *origine = p;
   ULong inc_dy = (objet->tz + 2) - 2;
   ULong inc_dx = (objet->ty + 2) * (objet->tz + 2);;

   if (*p)
      res |= mask;
   p++;
   mask <<= 1;			/* _ */
   if (*p)
      res |= mask;
   p++;
   mask <<= 1;			/* _ */
   if (*p)
      res |= mask;
   p += inc_dy;
   mask <<= 1;			/* _ */
   if (*p)
      res |= mask;
   p++;
   mask <<= 1;			/* _ */
   if (*p)
      res |= mask;
   p++;
   mask <<= 1;			/* _ */
   if (*p)
      res |= mask;
   p += inc_dy;
   mask <<= 1;			/* _ */
   if (*p)
      res |= mask;
   p++;
   mask <<= 1;			/* _ */
   if (*p)
      res |= mask;
   p++;
   mask <<= 1;			/* _ */
   if (*p)
      res |= mask;
   p = origine + inc_dx;
   mask <<= 1;			/* _ */
   if (*p)
      res |= mask;
   p++;
   mask <<= 1;			/* _ */
   if (*p)
      res |= mask;
   p++;
   mask <<= 1;			/* _ */
   if (*p)
      res |= mask;
   p += inc_dy;
   mask <<= 1;			/* _ */
   if (*p)
      res |= mask;
   p++;
   mask <<= 1;			/* _ */
   if (*p)
      res |= mask;
   p++;
   mask <<= 1;			/* _ */
   if (*p)
      res |= mask;
   p += inc_dy;
   mask <<= 1;			/* _ */
   if (*p)
      res |= mask;
   p++;
   mask <<= 1;			/* _ */
   if (*p)
      res |= mask;
   p++;
   mask <<= 1;			/* _ */
   if (*p)
      res |= mask;
   p = origine + (inc_dx << 1);
   mask <<= 1;			/* _ */
   if (*p)
      res |= mask;
   p++;
   mask <<= 1;			/* _ */
   if (*p)
      res |= mask;
   p++;
   mask <<= 1;			/* _ */
   if (*p)
      res |= mask;
   p += inc_dy;
   mask <<= 1;			/* _ */
   if (*p)
      res |= mask;
   p++;
   mask <<= 1;			/* _ */
   if (*p)
      res |= mask;
   p++;
   mask <<= 1;			/* _ */
   if (*p)
      res |= mask;
   p += inc_dy;
   mask <<= 1;			/* _ */
   if (*p)
      res |= mask;
   p++;
   mask <<= 1;			/* _ */
   if (*p)
      res |= mask;
   p++;
   mask <<= 1;			/* _ */
   if (*p)
      res |= mask;

   return res;
}

void
objMarque26Bord(Objet3D * objet, BYTE valeur)
{
   UShort x, y, z;
   UShort tx, ty, tz;
   BYTE ***matrice = objet->matrice;
   BYTE *p;
   ULong diff_plan;

   POBJ_TAILLE(objet, tx, ty, tz);
   diff_plan = (ty + 2) * (tz + 2);

   for (z = 0; z < tz; z++)
      for (y = 1; y < ty; y++)
	 for (x = 1; x < tx; x++) {
	    p = matrice[x][y] + z;
	    if (*p) {
	       *p = 1;
	       if (!*(p + tz + 2))
		  matrice[x][y][z] = valeur;
	       else if (!*(p - (tz + 2)))
		  matrice[x][y][z] = valeur;
	       else if (!*(p + 1))
		  matrice[x][y][z] = valeur;
	       else if (!*(p - 1))
		  matrice[x][y][z] = valeur;
	       else if (!*(p - diff_plan))
		  matrice[x][y][z] = valeur;
	       else if (!*(p + diff_plan))
		  matrice[x][y][z] = valeur;
	    }
	 }

}


void
objMarque18Bord(Objet3D * objet, BYTE valeur)
{
   UShort x, y, z;
   UShort tx, ty, tz;
   BYTE ***matrice = objet->matrice;
   BYTE *p;
   ULong diff_plan;

   POBJ_TAILLE(objet, tx, ty, tz);
   diff_plan = (ty + 2) * (tz + 2);

   for (z = 0; z < tz; z++)
      for (y = 0; y < ty; y++)
	 for (x = 0; x < tx; x++) {
	    p = matrice[x][y] + z;
	    if (*p) {
	       *p = 1;
	       if (!*(p + tz + 2))
		  matrice[x][y][z] = valeur;
	       else if (!*(p - (tz + 2)))
		  matrice[x][y][z] = valeur;
	       else if (!*(p + 1))
		  matrice[x][y][z] = valeur;
	       else if (!*(p - 1))
		  matrice[x][y][z] = valeur;
	       else if (!*(p - diff_plan))
		  matrice[x][y][z] = valeur;
	       else if (!*(p + diff_plan))
		  matrice[x][y][z] = valeur;
	    }
	 }
}


void
objMarque6Bord(Objet3D * objet, BYTE valeur)
{
   UShort x, y, z;
   UShort tx, ty, tz;
   Config config;
   BYTE ***matrice = objet->matrice;

   POBJ_TAILLE(objet, tx, ty, tz);

   for (z = 0; z < tz; z++)
      for (y = 0; y < ty; y++)
	 for (x = 0; x < tx; x++)
	    if (matrice[x][y][z]) {
	       config = objGetConfig(objet, x, y, z);
	       if ((~config) & MASK_26VOISINS)
		  matrice[x][y][z] = valeur;
	    }

}


void
objMarque6PBord(Objet3D * objet, BYTE valeur)
{
   UShort x, y, z;
   UShort tx, ty, tz;
   Config config;
   BYTE ***matrice = objet->matrice;

   POBJ_TAILLE(objet, tx, ty, tz);

   for (z = 1; z < tz - 1; z++)
      for (y = 1; y < ty - 1; y++)
	 for (x = 1; x < tx - 1; x++)
	    if (matrice[x][y][z]) {
	       /*matrice[x][y][z]=1; */
	       config = objGetConfig(objet, x, y, z);
	       if ((~config) & MASK_18VOISINS)
		  matrice[x][y][z] = valeur;
	    }
}


void
EcrireDWord(FILE * fichier, unsigned long w)
{
   fputc(w / 0x1000000, fichier);
   fputc((w / 0x10000) & 0xFF, fichier);
   fputc((w / 0x100) & 0xFF, fichier);
   fputc(w & 0xFF, fichier);
}

unsigned long
LireDWord(FILE * fichier)
{

   long res = 0;

   res = fgetc(fichier) * 0x1000000;
   res += fgetc(fichier) * 0x10000;
   res += fgetc(fichier) * 0x100;
   res += fgetc(fichier);

   return (unsigned long) res;
}

int
int_min(int a, int b)
{
   return (a > b) ? b : a;
}

int
int_max(int a, int b)
{
   return (a < b) ? b : a;
}

float
float_min(float a, float b)
{
   return (a > b) ? b : a;
}

float
float_max(float a, float b)
{
   return (a > b) ? a : b;
}

int
Entete(FILE * fichier, char *chaine)
{
   char *p = chaine;

   while (*p) {
      if (*p == fgetc(fichier))
	 p++;
      else
	 return 0;
   }
   fgetc(fichier);
   return 1;
}

void
objInit(Objet3D * objet)
/* matrice=NULL pour reconnaitre qu'un objet est vide */
{
   objet->tx = objet->ty = objet->tz = 0;
   objet->aspectx = objet->aspecty = objet->aspectz = 1.0;
   objet->cdg_x = objet->cdg_y = objet->cdg_z = 0.0;
   objet->matrice = NULL;
}

/*  
 *    Allocation d'un tableau 3D. 
 *    Un bord est ajoute avec des valeurs nulles de  
 *    sorte que l'acces aux 26 voisins de tous les
 *    voxels soit possible sur tout le volume.
 *    Si un matrice pre-existe, elle est liberee. 
 */
void
objAllouer(Objet3D * objet, UShort tx, UShort ty, UShort tz)
{
   long x, k;
   BYTE ***matrice;

   objFree(objet);

   objet->tx = tx;
   objet->ty = ty;
   objet->tz = tz;
   matrice = (BYTE ***) calloc(tx + 2, sizeof(BYTE * *));
   if (!matrice) {
      perror("!!! objAllouer() : ALLOC IMPOSSIBLE ");
      exit(-1);
   }
   matrice++;

   matrice[-1] = (BYTE * *)calloc((tx + 2) * (ty + 2), sizeof(BYTE *));
   if (!matrice[-1]) {
      perror("!!! objAllouer() : ALLOC IMPOSSIBLE ");
      exit(-1);
   }

   (matrice[-1])++;
   matrice[-1][-1] =
       (BYTE *) calloc((tx + 2) * (ty + 2) * (tz + 2), sizeof(BYTE));

   if (!matrice[-1][-1]) {
      perror("!!! objAllouer() : ALLOC IMPOSSIBLE ");
      exit(-1);
   }
   (matrice[-1][-1])++;


   for (x = 0; x < tx + 1; x++)
      matrice[x] = matrice[x - 1] + ty + 2;

   for (k = 0; k < (ty + 2) * (tx + 2) - 1; k++)
      matrice[-1][k] = matrice[-1][k - 1] + tz + 2;

   objet->matrice = matrice;

   POBJ_SETCDG(objet, 0, 0, 0);
   POBJ_SETASPECT(objet, 0.0, 0.0, 0.0);
}


void
objFree(Objet3D * objet)
{
   if (!(objet->matrice))
      return;

   free((objet->matrice[-1][-1]) - 1);
   free((objet->matrice[-1]) - 1);
   free((objet->matrice) - 1);
   objet->matrice = NULL;
   objet->tx = objet->ty = objet->tz = 0;
}


void
objClear(Objet3D * objet)
{
   if (objet->matrice)
      memset(objet->matrice[-1][-1],
	     0,
	     (objet->tx + 2) * (objet->ty + 2) * (objet->tz +
						  2) * sizeof(BYTE));
}


void
objCopy(Objet3D * dest, Objet3D * src)
{
   UShort tx, ty, tz;

   POBJ_TAILLE(src, tx, ty, tz);

   if (src->tx == dest->tx && src->ty == dest->ty && src->tz == dest->tz)
      objClear(dest);
   else {
      objFree(dest);
      objAllouer(dest, tx, ty, tz);
   }

   memcpy(dest->matrice[-1][-1], src->matrice[-1][-1],
	  (tx + 2) * (ty + 2) * (tz + 2) * sizeof(BYTE));
}

unsigned int
ReadInt(FILE * fichier)
{
   unsigned int res = 0;
   res |= ((unsigned int) fgetc(fichier));
   res |= ((unsigned int) fgetc(fichier)) << 8;
   res |= ((unsigned int) fgetc(fichier)) << 16;
   res |= ((unsigned int) fgetc(fichier)) << 24;
   return res;
}

UShort
ReadShort(FILE * fichier)
{
   UShort res = 0;
   res |= ((unsigned int) fgetc(fichier));
   res |= ((unsigned int) fgetc(fichier)) << 8;
   return res;
}

ListePoints
lpCreate()
{
   ListePoints liste;
   int n;

   liste.taille_tableaux = 65530;
   liste.nb_points = liste.courant = liste.n_courant = 0;
   liste.tableau_courant = 0;
   liste.tableau_dernier = liste.dernier = 0;

   liste.tableaux = (RecPoint * *)calloc(256, sizeof(BYTE *));

   if (!(liste.tableaux)) {
      perror
	  ("!!! lpInit() : Impossible d'allouer un tableau de 256 pointeurs");
      liste.tableaux = NULL;
      return liste;
   }

   liste.tableaux[0] =
       (RecPoint *) calloc(liste.taille_tableaux, sizeof(RecPoint));

   if (!(liste.tableaux[0])) {
      perror
	  ("!!! lpInit() : Impossible d'allouer un tableau de 256 pointeurs");
      liste.tableaux[0] = 0;
      return liste;
   }

   for (n = 1; n < 256; n++) {
      liste.tableaux[n] = NULL;
   }

   return liste;
}


int
lpInit(ListePoints * liste)
{
   int n;

   liste->taille_tableaux = 65530;
   liste->nb_points = liste->courant = liste->n_courant = 0;
   liste->tableau_courant = 0;
   liste->tableau_dernier = liste->dernier = 0;

   liste->tableaux = (RecPoint * *)calloc(256, sizeof(BYTE *));
   liste->tableaux[0] =
       (RecPoint *) calloc(liste->taille_tableaux, sizeof(RecPoint));

   if (!(liste->tableaux) || !(liste->tableaux[0])) {
      perror
	  ("!!! lpInit() : Impossible d'allouer un tableau de 256 pointeurs");
      liste->taille_tableaux = 0;
      return 0;
   }

   for (n = 1; n < 256; n++) {
      liste->tableaux[n] = NULL;
   }

   return 1;
}

int
lpInitTailleTableaux(ListePoints * liste, ULong n)
{
   int i;
   liste->taille_tableaux = n;
   liste->nb_points = liste->courant = liste->n_courant = 0;
   liste->tableau_courant = 0;
   liste->tableau_dernier = liste->dernier = 0;

   liste->tableaux = (RecPoint * *)calloc(256, sizeof(BYTE *));
   liste->tableaux[0] =
       (RecPoint *) calloc(liste->taille_tableaux, sizeof(RecPoint));

   if (!liste->tableaux) {
      perror
	  ("!!! lpInit() : Impossible d'allouer un tableau de 256 pointeurs");
      liste->taille_tableaux = 0;
      return 0;
   }

   for (i = 1; i < 256; i++) {
      liste->tableaux[i] = NULL;
   }

   return 1;
}

int
lpCopy(ListePoints * dest, ListePoints * src)
{
   int i;

   if (src->taille_tableaux != dest->taille_tableaux) {
      for (i = 0; i < 255; i++) {
	 if (dest->tableaux[i])
	    free(dest->tableaux[i]);
	 dest->tableaux[i] = NULL;
      }
   } else {
      dest->taille_tableaux = src->taille_tableaux;
      dest->nb_points = src->nb_points;
      dest->courant = src->courant;
      dest->tableau_courant = src->tableau_courant;
      dest->dernier = src->dernier;
      dest->tableau_dernier = src->tableau_dernier;
      dest->n_courant = src->n_courant;
   }


   for (i = 0; i <= dest->tableau_dernier; i++) {
      if (!(dest->tableaux[i]))
	 dest->tableaux[i] =
	     (RecPoint *) calloc(dest->taille_tableaux, sizeof(RecPoint));

      if (!dest->tableaux[i]) {
	 perror("!!! lpCopy() : Erreur d'allocation ");
	 return 0;
      }
   }

   for (i = 0; i <= dest->tableau_dernier; i++)
      memcpy(dest->tableaux[i],
	     src->tableaux[i], dest->taille_tableaux * sizeof(RecPoint));

   return 1;
}

void
lpFree(ListePoints * liste)
{
   int i;
   RecPoint **tableaux = liste->tableaux;

   if (tableaux) {
      for (i = 0; i < 256; i++)
	 if (tableaux[i])
	    free(tableaux[i]);

      free(tableaux);
      liste->tableaux = NULL;
   }
}


void
lpDebut(ListePoints * liste)
{
   liste->n_courant = liste->courant = 0;
   liste->tableau_courant = 0;
}


void
lpRaz(ListePoints * liste)
{
   liste->nb_points = liste->courant = liste->n_courant = 0;
   liste->tableau_courant = 0;
   liste->tableau_dernier = liste->dernier = 0;
}

RecPoint *
lpSuivant(ListePoints * liste)
{
   RecPoint *p;

   if (liste->n_courant == liste->nb_points) {
      perror("!!! lpSuivant() : Fin de la liste depasee");
      printf(" %lu %lu \n", liste->n_courant, liste->nb_points);
      return NULL;
   }

   p = &(liste->tableaux[liste->tableau_courant][liste->courant]);
   (liste->courant)++;
   (liste->n_courant)++;
   if (liste->courant == liste->taille_tableaux) {
      liste->courant = 0;
      liste->tableau_courant++;
   }

   return p;
}

inline char
lpFin(ListePoints * liste)
{
   return (liste->n_courant == liste->nb_points);
}

inline char
lpVide(ListePoints * liste)
{
   return !(liste->nb_points);
}

void
lpAjoute(ListePoints * liste, UShort x, UShort y, UShort z,
	 BYTE directions)
{
   RecPoint *p;

   p = &(liste->tableaux[liste->tableau_dernier][liste->dernier]);
   p->x = x;
   p->y = y;
   p->z = z;
   p->directions = directions;

   liste->nb_points++;
   liste->dernier++;

   if (liste->tableau_dernier == 255
       && liste->dernier == liste->taille_tableaux) {
      perror("!!! lpAjoute() : Limite de tableau atteinte");
      return;
   }

   if (liste->dernier == liste->taille_tableaux) {
      liste->tableau_dernier++;
      if (!(liste->tableaux[liste->tableau_dernier])) {
	 liste->tableaux[liste->tableau_dernier] =
	     (RecPoint *) calloc(liste->taille_tableaux, sizeof(RecPoint));

	 if (!(liste->tableaux[liste->tableau_dernier])) {
	    perror
		(" !!! lpAjoute() : Impossible d'allouer un nouveau tableau");
	 }
      }

      liste->dernier = 0;
   }
}


void
lpEchange(ListePoints * liste, ULong i, ULong j)
{
   RecPoint *p_i;
   RecPoint *p_j;

   RecPoint tmp;

   p_i =
       (liste->tableaux[i / (liste->taille_tableaux)]) +
       (i % (liste->taille_tableaux));
   p_j =
       (liste->tableaux[j / (liste->taille_tableaux)]) +
       (j % (liste->taille_tableaux));

   tmp = *p_i;
   *p_i = *p_j;
   *p_j = tmp;
}

char
adj(char p1, char p2, char n_adj)
{
   char dx = abs(dx_of[(int)p1] - dx_of[(int)p2]);
   char dy = abs(dy_of[(int)p1] - dy_of[(int)p2]);
   char dz = abs(dz_of[(int)p1] - dz_of[(int)p2]);

   if (!(dx + dy + dz))
      return 0;

   if (dx > 1 || dy > 1 || dz > 1)
      return 0;

   if (dx + dy + dz == 1)
      return 1;			// Pour la 6,18,26-adj Ok

   if (n_adj == ADJ18) {
      if (dx + dy + dz == 2)
	 return 1;
      else
	 return 0;
   }
   if (n_adj == ADJ26)
      return 1;
   return 0;
}

inline Config
cnfComplementaire(Config c)
{
   return (~c) & MASK_CONFIG;
}

int
cnfCardinal(Config c)
{
   int res = 0;
   while (c) {
      if (c & 1)
	 res++;
      c >>= 1;
   }
   return res;
}

Config
cnfAlea()
{
   char n;
   Config res = 0;
   for (n = 0; n < 27; n++) {
      if ((rand() & 15) == 15)
	 res |= 1 << n;
   }
   return res;
}

Config
cnfAdd(Config c, char n)
{
   return c | (1 << n);
}

char
cnfExtremite(Config c)
{
   char deja = 0;
   char n;

   for (n = 0; n < 27; n++) {
      if (n != 13 && (c & (1 << n)) && deja)
	 return 0;
      if (n != CENTRE && (c & (1 << n)))
	 deja = 1;
   }
   return 1;
}

Config
cnfVGeodesique6(Config cnf)
{
   /* Voisinage geodesique d'ordre 2 */
   Config res = 0;
   Config ajout = 0;

   res = cnf & MASK_6VOISINS;

   if (res & MASK_4)
      ajout |= Les6voisins[4];
   if (res & MASK_10)
      ajout |= Les6voisins[10];
   if (res & MASK_12)
      ajout |= Les6voisins[12];
   if (res & MASK_14)
      ajout |= Les6voisins[14];
   if (res & MASK_16)
      ajout |= Les6voisins[16];
   if (res & MASK_22)
      ajout |= Les6voisins[22];

   res |= (cnf & ajout);

   return res;
}

Config
cnfVGeodesique6_3(Config cnf)
{
   /* Voisinage geodesique d'ordre 3 */
   Config res = 0;
   Config ajout = 0;
   int n;

   res = cnf & MASK_6VOISINS;

   if (res & MASK_4)
      ajout |= Les6voisins[4];
   if (res & MASK_10)
      ajout |= Les6voisins[10];
   if (res & MASK_12)
      ajout |= Les6voisins[12];
   if (res & MASK_14)
      ajout |= Les6voisins[14];
   if (res & MASK_16)
      ajout |= Les6voisins[16];
   if (res & MASK_22)
      ajout |= Les6voisins[22];

   res |= (cnf & ajout);

   ajout = 0;

   for (n = 0; n < 27; n++) {
      if (res & (1 << n))
	 ajout |= Les6voisins[n];
   }

   res |= (cnf & ajout);
   return res;
}

Config
cnfVGeodesique6P(Config cnf)
{
   /* Voisinage geodesique d'ordre 3 */
   Config res = 0;
   Config ajout = 0;
   int n;

   res = cnf & MASK_6VOISINS;

   if (res & MASK_4)
      ajout |= Les6voisins[4];
   if (res & MASK_10)
      ajout |= Les6voisins[10];
   if (res & MASK_12)
      ajout |= Les6voisins[12];
   if (res & MASK_14)
      ajout |= Les6voisins[14];
   if (res & MASK_16)
      ajout |= Les6voisins[16];
   if (res & MASK_22)
      ajout |= Les6voisins[22];

   res |= (cnf & ajout);

   ajout = 0;
   for (n = 0; n < 27; n++) {
      if (res & (1 << n))
	 ajout |= Les6voisins[n];
   }
   res |= (cnf & ajout);
   return res;
}

Config
cnfVGeodesique6P_5(Config cnf)
{
   /* Voisinage geodesique d'ordre 5 */
   Config res = 0, mask;

   static int file[32];
   int ordre;
   int *tete = file;
   int *queue = file;
   int *limite;
   const int *p_voisin;

   res = cnf & MASK_6VOISINS;
   if (res & MASK_4)
      *(queue++) = 4;
   if (res & MASK_10)
      *(queue++) = 10;
   if (res & MASK_12)
      *(queue++) = 12;
   if (res & MASK_14)
      *(queue++) = 14;
   if (res & MASK_16)
      *(queue++) = 16;		/* Si dans la file, alors aussi dans res */
   if (res & MASK_22)
      *(queue++) = 22;		/* VG d'ordre 1 dans la file */

   ordre = 2;
   limite = queue;
   while (ordre < 6 && tete < queue) {
      while (tete < limite) {
	 p_voisin = Liste6voisins[*tete];
	 while ((*p_voisin) != 127) {
	    mask = (1 << *p_voisin);
	    if ((mask & cnf) && !(res & mask)) {	/* Si pas marque */
	       *(queue++) = *p_voisin;	/* On enfile le voisins */
	       res |= mask;	/* On le marque comme deja vu */
	    }
	    p_voisin++;
	 }
	 tete++;
      }
      limite = queue;
      ordre++;
   }
   return res;
}

Config
cnfVGeodesique18(Config cnf)
{
   /* Voisinage geodesique d'ordre 3 */
   Config res = 0;
   Config ajout = 0;
   int n;

   res = cnf & MASK_18VOISINS;
   ajout = 0;

   for (n = 0; n < 27; n++) {
      if (res & (1 << n))
	 ajout |= Les18voisins[n];
   }

   res |= (cnf & ajout);
   ajout = 0;
   for (n = 0; n < 27; n++) {
      if (res & (1 << n))
	 ajout |= Les18voisins[n];
   }
   res |= (cnf & ajout);
   return res;
}

Config
cnfVGeodesique18_3(Config cnf)
{
   /* Voisinage geodesique d'ordre 3 */
   /* Je CROIS QU'UN ORDRE 2 EST GRANDEMENT SUFFISANT !!! */

   Config res = 0, mask;

   static int file[32];
   int ordre;
   int *tete = file;
   int *queue = file;
   int *limite;
   const int *p_voisin;

   res = 0;
   *(queue++) = 13;


   /*cnfAffiche3D(res);    */

   ordre = 1;
   limite = queue;
   while (ordre < 4 && tete < queue) {
      while (tete < limite) {
	 p_voisin = Liste18voisins[*tete];
	 while ((*p_voisin) != 127) {
	    mask = (1 << *p_voisin);
	    if ((mask & cnf) && !(res & mask)) {
	       *(queue++) = *p_voisin;
	       res |= mask;
	    }
	    p_voisin++;
	 }
	 tete++;
      }
      limite = queue;
      ordre++;
   }
   return res;
}

Config
cnfVGeodesique26(Config cnf)
{
   /* Voisinage geodesique d'ordre 1 */
   return cnf & MASK_26VOISINS;
}

char
cnfUneC6C(const Config c)
{
   Config non_vus, courant, prochain;
   int n;
   non_vus = MASK_26VOISINS & c;
   if (non_vus) {
      n = 0;
      while (!(non_vus & (1 << n)))
	 n++;
      prochain = 1 << n;
      non_vus ^= prochain;
      while (prochain && non_vus) {
	 courant = prochain;
	 prochain = 0;
	 n = 0;
	 while (courant) {
	    if (courant & 1)
	       prochain |= Les6voisins[n] & non_vus;
	    courant >>= 1;
	    n++;
	 }
	 non_vus ^= prochain;
      }
      if (!non_vus)
	 return 1;
   }
   return 0;
}

char
cnfUneC6C_FILE(const Config c)
{
   Config non_vus, mask;
   int n;
   unsigned char file[32];
   unsigned char *tete = file;
   unsigned char *queue = file;
   const int *p_voisin;

   non_vus = MASK_26VOISINS & c;
   if (non_vus) {
      n = 0;
      mask = 1;
      while (!(non_vus & mask)) {
	 n++;
	 mask <<= 1;
      }

      *queue = n;
      queue++;
      non_vus ^= mask;

      while (tete < queue) {
	 p_voisin = Liste6voisins[*tete];
	 while (*p_voisin != 127) {
	    mask = 1 << *p_voisin;
	    if (non_vus & mask) {
	       *queue = *p_voisin;
	       queue++;
	       non_vus ^= mask;
	    }
	    p_voisin++;
	 }
	 tete++;
      }
      if (!non_vus)
	 return 1;
      return 0;
   }
   return 0;
}

char
cnfUneC18C(const Config c)
{
   Config non_vus, courant, prochain;
   int n;
   non_vus = MASK_26VOISINS & c;

   if (non_vus) {
      n = 0;
      while (!(non_vus & (1 << n)))
	 n++;
      prochain = 1 << n;
      non_vus ^= prochain;

      while (prochain && non_vus) {
	 courant = prochain;
	 prochain = 0;
	 n = 0;
	 while (courant) {
	    if (courant & 1)
	       prochain |= Les18voisins[n] & non_vus;
	    courant >>= 1;
	    n++;
	 }
	 non_vus ^= prochain;
      }
      if (!non_vus)
	 return 1;
   }
   return 0;
}

char
cnfUneC18C_FILE(const Config c)
{
   Config non_vus, mask;
   int n;
   unsigned char file[32];
   unsigned char *tete = file;
   unsigned char *queue = file;
   const int *p_voisin;

   non_vus = MASK_26VOISINS & c;

   if (non_vus) {
      n = 0;
      mask = 1;
      while (!(non_vus & mask)) {
	 n++;
	 mask <<= 1;
      }

      *queue = n;
      queue++;
      non_vus ^= mask;
      while (tete < queue) {
	 p_voisin = Liste18voisins[*tete];
	 while (*p_voisin != 127) {
	    mask = 1 << *p_voisin;
	    if (non_vus & mask) {
	       *queue = *p_voisin;
	       queue++;
	       non_vus ^= mask;	/* Et on marque deja vu */
	    }
	    p_voisin++;
	 }
	 tete++;
      }
      if (!non_vus)
	 return 1;
      return 0;
   }
   return 0;
}

char
cnfUneC26C(const Config c)
{
   Config non_vus, courant, prochain;
   int n;
   non_vus = MASK_26VOISINS & c;

   if (non_vus) {
      n = 0;
      while (!(non_vus & (1 << n)))
	 n++;
      prochain = 1 << n;
      non_vus ^= prochain;

      while (prochain && non_vus) {
	 courant = prochain;
	 prochain = 0;
	 n = 0;
	 while (courant) {
	    if (courant & 1)
	       prochain |= Les26voisins[n] & non_vus;
	    courant >>= 1;
	    n++;
	 }
	 non_vus ^= prochain;
      }
      if (!non_vus)
	 return 1;
   }
   return 0;
}

char
cnfUneC26C_FILE(const Config c)
{
   Config non_vus, mask;
   int n;
   unsigned char file[32];
   unsigned char *tete = file;
   unsigned char *queue = file;
   const int *p_voisin;

   non_vus = MASK_26VOISINS & c;

   if (non_vus) {
      n = 0;
      mask = 1;
      while (!(non_vus & mask)) {
	 n++;
	 mask <<= 1;
      }

      *queue = n;
      queue++;
      non_vus ^= mask;

      while (tete < queue) {
	 p_voisin = Liste26voisins[*tete];
	 while (*p_voisin != 127) {
	    mask = 1 << *p_voisin;

	    if (non_vus & mask) {	/* Si pas deja vu, on enfile */
	       *queue = *p_voisin;
	       queue++;
	       non_vus ^= mask;	/* Et on marque deja vu */
	    }
	    p_voisin++;
	 }
	 tete++;
      }
      if (!non_vus)
	 return 1;
      return 0;
   }
   return 0;
}

char
cnfUneCC_FILE(const Config c, const int liste_voisins[][32])
{
   Config non_vus, mask;
   int n;
   unsigned char file[32];
   unsigned char *tete = file;
   unsigned char *queue = file;
   const int *p_voisin;

   non_vus = MASK_26VOISINS & c;

   if (non_vus) {
      n = 0;
      mask = 1;
      while (!(non_vus & mask)) {
	 n++;
	 mask <<= 1;
      }

      *queue = n;
      queue++;
      non_vus ^= mask;

      while (tete < queue) {
	 p_voisin = liste_voisins[*tete];
	 while (*p_voisin != 127) {
	    mask = 1 << *p_voisin;

	    if (non_vus & mask) {	/* Si pas deja vu, on enfile */
	       *queue = *p_voisin;
	       queue++;
	       non_vus ^= mask;	/* Et on marque deja vu */
	    }
	    p_voisin++;
	 }
	 tete++;
      }
      if (!non_vus)
	 return 1;
      return 0;
   }
   return 0;
}

char
cnfDeuxCC_FILE(const Config c, const int liste_voisins[][32])
{
   Config non_vus, mask;
   int n;
   unsigned char file[32];
   unsigned char *tete = file;
   unsigned char *queue = file;
   const int *p_voisin;

   non_vus = MASK_26VOISINS & c;

   if (non_vus) {
      n = 0;
      mask = 1;
      while (!(non_vus & mask)) {
	 n++;
	 mask <<= 1;
      }

      *queue = n;
      queue++;
      non_vus ^= mask;

      while (tete < queue) {
	 p_voisin = liste_voisins[*tete];
	 while (*p_voisin != 127) {
	    mask = 1 << *p_voisin;

	    if (non_vus & mask) {	/* Si pas deja vu, on enfile */
	       *queue = *p_voisin;
	       queue++;
	       non_vus ^= mask;	/* Et on marque deja vu */
	    }
	    p_voisin++;
	 }
	 tete++;
      }
      if (!non_vus)
	 return 0;		/* Il n'y a qu'un CC, echec */

      n = 0;
      mask = 1;
      while (!(non_vus & mask)) {
	 n++;
	 mask <<= 1;
      }
      tete = queue = file;
      *queue = n;
      queue++;
      non_vus ^= mask;

      while (tete < queue) {
	 p_voisin = liste_voisins[*tete];
	 while (*p_voisin != 127) {
	    mask = 1 << *p_voisin;

	    if (non_vus & mask) {	/* Si pas deja vu, on enfile */
	       *queue = *p_voisin;
	       queue++;
	       non_vus ^= mask;	/* Et on marque deja vu */
	    }
	    p_voisin++;
	 }
	 tete++;
      }

      if (!non_vus)
	 return 1;		/* On a tout efface en deux passes : Ok */
      return 0;
   }
   return 0;
}

char
cnfNombreCC_FILE(const Config c, const int liste_voisins[][32])
{
   Config non_vus, mask;
   int n = 0;
   char res = 0;
   unsigned char file[32];
   unsigned char *tete = file;
   unsigned char *queue = file;
   const int *p_voisin;

   non_vus = MASK_26VOISINS & c;

   while (non_vus) {
      n = 0;
      res++;
      tete = queue = file;
      mask = 1;
      while (!(non_vus & mask)) {
	 n++;
	 mask <<= 1;
      }
      *queue = n;
      queue++;
      non_vus ^= mask;
      while (tete < queue) {
	 p_voisin = liste_voisins[*tete];
	 while (*p_voisin != 127) {
	    mask = 1 << *p_voisin;

	    if (non_vus & mask) {	/* Si pas deja vu, on enfile */
	       *queue = *p_voisin;
	       queue++;
	       non_vus ^= mask;	/* Et on marque deja vu */
	    }
	    p_voisin++;
	 }
	 tete++;
      }
   }
   return res;
}

/* FONCTIONS DE TEST DES CARACTERISATIONS LOCALES DE POINTS SIMPLE ET  P-SIMPLES */

char
cnfSimple6(Config c)
{
   Config vg;

   vg = cnfVGeodesique6(c);
   if (!cnfUneC6C(vg))
      return 0;
   vg = cnfVGeodesique26((~c) & MASK_CONFIG);
   if (!cnfUneC26C(vg))
      return 0;

   return 1;
}

char
cnfSimple6P(Config c)
{

   Config vg;

   vg = cnfVGeodesique6P(c);
   if (!cnfUneC6C(vg))
      return 0;

   vg = cnfVGeodesique18((~c) & MASK_CONFIG);
   if (!cnfUneC18C(vg))
      return 0;
   return 1;
}

char
cnfSimple18(Config c)
{

   Config vg;

   vg = cnfVGeodesique18(c);
   if (!cnfUneC18C(vg))
      return 0;

   vg = cnfVGeodesique6P((~c) & MASK_CONFIG);
   if (!cnfUneC6C(vg))
      return 0;
   return 1;
}

char
cnfSimple26(Config c)
{

   Config vg;

   vg = cnfVGeodesique26(c);
   if (!cnfUneC26C(vg))
      return 0;

   vg = cnfVGeodesique6((~c) & MASK_CONFIG);
   if (!cnfUneC6C(vg))
      return 0;
   return 1;
}


/* ATTENTION mask_p doit etre "inclus" dans c */
char
cnfPSimple6(Config c, Config mask_p)
{
   Config XsansP = c ^ mask_p;
   Config test;
   int n;

   if (!cnfUneC6C(cnfVGeodesique6(XsansP)))
      return 0;

   if (!cnfUneC26C(cnfVGeodesique26((~c) & MASK_26VOISINS)))
      return 0;

   test = cnfVGeodesique6_3(XsansP);
   /* Tous les points de P, 6-voisins de x doivent etre adjacents a test */
   if ((mask_p & MASK_4) && !(test & Les6voisins[4]))
      return 0;
   if ((mask_p & MASK_10) && !(test & Les6voisins[10]))
      return 0;
   if ((mask_p & MASK_12) && !(test & Les6voisins[12]))
      return 0;
   if ((mask_p & MASK_14) && !(test & Les6voisins[14]))
      return 0;
   if ((mask_p & MASK_16) && !(test & Les6voisins[16]))
      return 0;
   if ((mask_p & MASK_22) && !(test & Les6voisins[22]))
      return 0;


   test = (~c) & MASK_26VOISINS;
   /* Tous les points de P, 26-voisins de x doivent etre adjacents a test */
   n = 0;
   while (n < 27) {
      if ((mask_p & (1 << n)) && !(test & Les26voisins[n]))
	 return 0;
      n++;
   }

   return 1;
}

/* ATTENTION mask_p doit etre "inclus" dans c */
char
cnfPSimple6P(Config c, Config mask_p)
{
   Config XsansP = c ^ mask_p;
   Config test;
   int n;
   const int *p_voisin;

   if (!cnfUneC6C(cnfVGeodesique6P(XsansP)))
      return 0;
   if (!cnfUneC18C(cnfVGeodesique18((~c) & MASK_26VOISINS)))
      return 0;

   test = cnfVGeodesique6P_5(XsansP);

   if ((mask_p & MASK_4) && !(test & Les6voisins[4]))
      return 0;
   if ((mask_p & MASK_10) && !(test & Les6voisins[10]))
      return 0;
   if ((mask_p & MASK_12) && !(test & Les6voisins[12]))
      return 0;
   if ((mask_p & MASK_14) && !(test & Les6voisins[14]))
      return 0;
   if ((mask_p & MASK_16) && !(test & Les6voisins[16]))
      return 0;
   if ((mask_p & MASK_22) && !(test & Les6voisins[22]))
      return 0;


   test = cnfVGeodesique18_3((~c) & MASK_26VOISINS);

   p_voisin = Liste18voisins[CENTRE];

   while ((*p_voisin != 127)) {
      n = *p_voisin;
      if ((mask_p & (1 << n)) && !(test & Les18voisins[n]))
	 return 0;
      p_voisin++;
   }

   return 1;
}


/* ATTENTION mask_p doit etre "inclus" dans c */
char
cnfPSimple18(Config c, Config mask_p)
{
   Config XsansP = c ^ mask_p;
   Config test;
   int n;
   const int *p_voisin;

   if (!cnfUneC18C(cnfVGeodesique18(XsansP)))
      return 0;
   if (!cnfUneC6C(cnfVGeodesique6P((~c) & MASK_26VOISINS)))
      return 0;

   test = cnfVGeodesique18_3(XsansP);

   /* Tous les points de P, 18-voisins de x doivent etre adjacents a test */
   p_voisin = Liste18voisins[CENTRE];
   while ((*p_voisin != 127)) {
      n = *p_voisin;
      if ((mask_p & (1 << n)) && !(test & Les18voisins[n]))
	 return 0;
      p_voisin++;
   }

   test = cnfVGeodesique6P_5((~c) & MASK_26VOISINS);

   /* Tous les points de P, 6-voisins de x doivent etre adjacents a test */
   if ((mask_p & MASK_4) && !(test & Les6voisins[4]))
      return 0;
   if ((mask_p & MASK_10) && !(test & Les6voisins[10]))
      return 0;
   if ((mask_p & MASK_12) && !(test & Les6voisins[12]))
      return 0;
   if ((mask_p & MASK_14) && !(test & Les6voisins[14]))
      return 0;
   if ((mask_p & MASK_16) && !(test & Les6voisins[16]))
      return 0;
   if ((mask_p & MASK_22) && !(test & Les6voisins[22]))
      return 0;

   return 1;
}

/* ATTENTION mask_p doit etre "inclus" dans c */
char
cnfPSimple26(Config c, Config mask_p)
{
   Config XsansP = c ^ mask_p;
   Config test;
   int n;

   if (!cnfUneC26C(cnfVGeodesique26(XsansP)))
      return 0;
   if (!cnfUneC6C(cnfVGeodesique6((~c) & MASK_26VOISINS)))
      return 0;

   test = XsansP & MASK_26VOISINS;

   /* Tous les points de P, 26-voisins de x doivent etre 26-adjacents a test */
   n = 0;
   while (n < 27) {
      if ((mask_p & (1 << n)) && !(test & Les26voisins[n]))
	 return 0;
      n++;
   }

   test = cnfVGeodesique6_3((~c) & MASK_26VOISINS);
   /* Tous les points de P, 6-voisins de x doivent etre 6-adjacents a test */
   if ((mask_p & MASK_4) && !(test & Les6voisins[4]))
      return 0;
   if ((mask_p & MASK_10) && !(test & Les6voisins[10]))
      return 0;
   if ((mask_p & MASK_12) && !(test & Les6voisins[12]))
      return 0;
   if ((mask_p & MASK_14) && !(test & Les6voisins[14]))
      return 0;
   if ((mask_p & MASK_16) && !(test & Les6voisins[16]))
      return 0;
   if ((mask_p & MASK_22) && !(test & Les6voisins[22]))
      return 0;

   return 1;
}


/* FONCTIONS DE TEST DES CARACTERISATIONS LOCALES DE SURFACES DISCRETES
   
   Les fonction de test renvoient un entier 
   bit 0 : point satisfaisant partielement la propriete 
   bit 1 : point satisfaisant la proprietes
*/

/* POINT DE SURFACE PRIMAIRE */
char
cnfSurface6(Config c)
{
   Config vg = cnfVGeodesique26(MASK_26VOISINS & ~c);
   if (cnfDeuxCC_FILE(vg, Liste26voisins))
      return 3;
   else
      return 0;
}

char
cnfSurface6P(Config c)
{
   Config vg = cnfVGeodesique18(MASK_26VOISINS & ~c);
   if (cnfDeuxCC_FILE(vg, Liste18voisins))
      return 3;
   else
      return 0;
}

char
cnfSurface18(Config c)
{
   Config vg = cnfVGeodesique6P(MASK_26VOISINS & ~c);
   if (cnfDeuxCC_FILE(vg, Liste6voisins))
      return 3;
   else
      return 0;
}

char
cnfSurface26(Config c)
{
   Config vg = cnfVGeodesique6(MASK_26VOISINS & ~c);
   if (cnfDeuxCC_FILE(vg, Liste6voisins))
      return 3;
   else
      return 0;
}

/* POINT ISTHME */
char
cnfIsthme6(Config c)
{
   Config vg = cnfVGeodesique6(c);
   if (cnfNombreCC_FILE(vg, Liste6voisins) >= 2)
      return 1;
   else
      return 0;
}

char
cnfIsthme6P(Config c)
{
   Config vg = cnfVGeodesique6P(c);
   if (cnfNombreCC_FILE(vg, Liste6voisins) >= 2)
      return 1;
   else
      return 0;
}

char
cnfIsthme18(Config c)
{
   Config vg = cnfVGeodesique18(c);
   if (cnfNombreCC_FILE(vg, Liste18voisins) >= 2)
      return 1;
   else
      return 0;
}

char
cnfIsthme26(Config c)
{
   Config vg = cnfVGeodesique26(c);
   if (cnfNombreCC_FILE(vg, Liste26voisins) >= 2)
      return 1;
   else
      return 0;
}

/* POINT BORD */
char
cnfBord6(Config c)
{
   if ((~c) & MASK_26VOISINS)
      return 1;
   else
      return 0;
}

char
cnfBord6P(Config c)
{
   if ((~c) & MASK_18VOISINS)
      return 1;
   else
      return 0;
}

char
cnfBord18(Config c)
{
   if ((~c) & MASK_6VOISINS)
      return 1;
   else
      return 0;
}

char
cnfBord26(Config c)
{
   if ((~c) & MASK_6VOISINS)
      return 1;
   else
      return 0;
}

/* POINTS DE JONCTIONS DE SURFACES */
char
cnfJonctionSurface6(Config c)
{
   if (cnfNombreCC_FILE
       (cnfVGeodesique26((~c) & MASK_26VOISINS), Liste26voisins) > 2
       && cnfUneC6C(cnfVGeodesique6(c)))
      return 3;
   return 0;
}

char
cnfJonctionSurface6P(Config c)
{
   if (cnfNombreCC_FILE
       (cnfVGeodesique18((~c) & MASK_26VOISINS), Liste18voisins) > 2
       && cnfUneC6C(cnfVGeodesique6P(c)))
      return 3;
   return 0;
}

char
cnfJonctionSurface18(Config c)
{
   if (cnfNombreCC_FILE
       (cnfVGeodesique6P((~c) & MASK_26VOISINS), Liste6voisins) > 2
       && cnfUneC18C(cnfVGeodesique18(c)))
      return 3;
   return 0;
}

char
cnfJonctionSurface26(Config c)
{
   if (cnfNombreCC_FILE
       (cnfVGeodesique6((~c) & MASK_26VOISINS), Liste6voisins) > 2
       && cnfUneC26C(cnfVGeodesique26(c)))
      return 3;
   return 0;
}


/* POINT SURFACE DE BERTRAND */
char
cnfSurfaceBertrand6(Config c)
{
   if (cnfNombreCC_FILE
       (cnfVGeodesique26((~c) & MASK_26VOISINS), Liste26voisins) >= 2)
      return 1;
   return 0;
}

char
cnfSurfaceBertrand6P(Config c)
{
   if (cnfNombreCC_FILE
       (cnfVGeodesique18((~c) & MASK_26VOISINS), Liste18voisins) >= 2)
      return 1;
   return 0;
}

char
cnfSurfaceBertrand18(Config c)
{
   if (cnfNombreCC_FILE
       (cnfVGeodesique6P((~c) & MASK_26VOISINS), Liste6voisins) >= 2)
      return 1;
   return 0;
}

char
cnfSurfaceBertrand26(Config c)
{
   if (cnfNombreCC_FILE
       (cnfVGeodesique6((~c) & MASK_26VOISINS), Liste6voisins) >= 2)
      return 1;
   return 0;
}

/* POINT SURFACE DE BERTRAND OU ISTHME */

char
cnfSurfaceBertrandOuIsthme6(Config c)
{
   char res = 0;
   if (cnfNombreCC_FILE
       (cnfVGeodesique26((~c) & MASK_26VOISINS), Liste26voisins) >= 2)
      res |= 2;
   if (cnfNombreCC_FILE(cnfVGeodesique6(c), Liste6voisins) >= 2)
      res |= 1;
   return res;
}

char
cnfSurfaceBertrandOuIsthme6P(Config c)
{
   char res = 0;
   if (cnfNombreCC_FILE
       (cnfVGeodesique18((~c) & MASK_26VOISINS), Liste18voisins) >= 2)
      res |= 2;
   if (cnfNombreCC_FILE(cnfVGeodesique6P(c), Liste6voisins) >= 2)
      res |= 1;
   return res;
}

char
cnfSurfaceBertrandOuIsthme18(Config c)
{
   char res = 0;
   if (cnfNombreCC_FILE
       (cnfVGeodesique6P((~c) & MASK_26VOISINS), Liste6voisins) >= 2)
      res |= 2;
   if (cnfNombreCC_FILE(cnfVGeodesique18(c), Liste18voisins) >= 2)
      res |= 1;
   return res;
}

char
cnfSurfaceBertrandOuIsthme26(Config c)
{
   char res = 0;
   if (cnfNombreCC_FILE
       (cnfVGeodesique6((~c) & MASK_26VOISINS), Liste6voisins) >= 2)
      res |= 2;
   if (cnfNombreCC_FILE(cnfVGeodesique26(c), Liste26voisins) >= 2)
      res |= 2;
   return res;
}


/*  Retourne dans a et b les composantes 6-connexes du complementaire
    de cnf qui sont 6-adjacentes au point central, 
    si elles sont deux exactement, retourne VRAI (1)
    sinon retourne FAUX (0) */
char
cnfLabel6(Config cnf, Config * axx, Config * bxx)
{
   Config non_vus, mask;
   unsigned char file[32];
   unsigned char *tete = file;
   unsigned char *queue = file;
   const int *p_voisin;

   non_vus = MASK_26VOISINS & (~cnf);	/* On examine le complementaire de cnf */

   *axx = *bxx = 0;

   /* On cherche Axx */
   p_voisin = Liste6voisins[CENTRE];

   *queue = 0;
   if (non_vus & MASK_4)
      *queue = 4;
   if (non_vus & MASK_10)
      *queue = 10;
   if (non_vus & MASK_12)
      *queue = 12;
   if (non_vus & MASK_14)
      *queue = 14;
   if (non_vus & MASK_16)
      *queue = 16;
   if (non_vus & MASK_22)
      *queue = 22;
   if (!(*queue))
      return 0;
   queue++;
   mask = 1 << *tete;
   non_vus ^= mask;
   (*axx) |= mask;
   while (tete < queue) {
      p_voisin = Liste6voisins[*tete];
      while (*p_voisin != 127) {
	 mask = 1 << (*p_voisin);

	 if (non_vus & mask) {	/* Si pas deja vu, on enfile */
	    *queue = *p_voisin;
	    queue++;
	    non_vus ^= mask;	/* Et on marque deja vu */
	    (*axx) |= mask;	/* Le sommet est place dans Axx */
	 }
	 p_voisin++;
      }
      tete++;
   }

   /* On cherche Bxx */
   tete = queue = file;
   *queue = 0;
   if (non_vus & MASK_4)
      *queue = 4;
   if (non_vus & MASK_10)
      *queue = 10;
   if (non_vus & MASK_12)
      *queue = 12;
   if (non_vus & MASK_14)
      *queue = 14;
   if (non_vus & MASK_16)
      *queue = 16;
   if (non_vus & MASK_22)
      *queue = 22;
   if (!(*queue))
      return 0;
   queue++;
   mask = 1 << *tete;
   non_vus ^= mask;
   (*bxx) |= mask;
   while (tete < queue) {
      p_voisin = Liste6voisins[*tete];
      while (*p_voisin != 127) {
	 mask = 1 << (*p_voisin);
	 if (non_vus & mask) {	/* Si pas deja vu, on enfile */
	    *queue = *p_voisin;
	    queue++;
	    non_vus ^= mask;	/* Et on marque deja vu */
	    (*bxx) |= mask;	/* Le sommet est place dans Bxx */
	 }
	 p_voisin++;
      }
      tete++;
   }

   if (*axx && *bxx && !(non_vus & Les6voisins[CENTRE]))
      return 1;

   return 0;
}


char
cnfSurfaceForte26(Config c)
{
   /* Xbarre a deux c6c 6adjacentes a x */
   Config axx, bxx;
   const int *pvoisin;
   char res = 0;
   Config vgAxx, vgBxx;

   if (cnfDeuxCC_FILE
       (cnfVGeodesique6((~c) & MASK_26VOISINS), Liste6voisins)) {
      res = 1;
   } else
      return 0;

   if (!cnfLabel6(c, &axx, &bxx))
      return res;
   if (!cnfDeuxCC_FILE
       (cnfVGeodesique6((~c) & MASK_26VOISINS), Liste6voisins))
      return res;

   vgAxx = axx & MASK_26VOISINS;
   if (!cnfUneCC_FILE(vgAxx, Liste26voisins))
      return res;
   vgBxx = bxx & MASK_26VOISINS;
   if (!cnfUneCC_FILE(vgBxx, Liste26voisins))
      return res;

   pvoisin = Liste26voisins[CENTRE];
   while ((*pvoisin) != 127) {
      if (c & (1 << *pvoisin)) {
	 if (!(vgAxx & Les26voisins[*pvoisin]))
	    return res;
	 if (!(vgBxx & Les26voisins[*pvoisin]))
	    return res;
      }
      pvoisin++;
   }

   /* Condition 4 */
   vgAxx = cnfVGeodesique6(axx);
   if (!cnfUneCC_FILE(vgAxx, Liste6voisins))
      return res;
   vgBxx = cnfVGeodesique6(bxx);
   if (!cnfUneCC_FILE(vgBxx, Liste6voisins))
      return res;

   vgAxx = cnfVGeodesique6_3(axx);
   vgBxx = cnfVGeodesique6_3(bxx);
   if ((c & MASK_4) && !(vgAxx & Les6voisins[4]))
      return res;
   if ((c & MASK_4) && !(vgBxx & Les6voisins[4]))
      return res;
   if ((c & MASK_10) && !(vgAxx & Les6voisins[10]))
      return res;
   if ((c & MASK_10) && !(vgBxx & Les6voisins[10]))
      return res;
   if ((c & MASK_12) && !(vgAxx & Les6voisins[12]))
      return res;
   if ((c & MASK_12) && !(vgBxx & Les6voisins[12]))
      return res;
   if ((c & MASK_14) && !(vgAxx & Les6voisins[14]))
      return res;
   if ((c & MASK_14) && !(vgBxx & Les6voisins[14]))
      return res;
   if ((c & MASK_16) && !(vgAxx & Les6voisins[16]))
      return res;
   if ((c & MASK_16) && !(vgBxx & Les6voisins[16]))
      return res;
   if ((c & MASK_22) && !(vgAxx & Les6voisins[22]))
      return res;
   if ((c & MASK_22) && !(vgBxx & Les6voisins[22]))
      return res;

   return 3;
}

char
cnfSurfaceForte26Ext6(Config c)
{
   if (cnfSurfaceForte26(c))
      return 3;
   if (cnfExtremite6(c))
      return 1;
   return 0;
}

char
cnfSurfaceForte26Ext18(Config c)
{
   if (cnfSurfaceForte26(c))
      return 3;
   if (cnfExtremite18(c))
      return 1;
   return 0;
}

char
cnfSurfaceForte26Ext26(Config c)
{
   if (cnfSurfaceForte26(c))
      return 3;
   if (cnfExtremite26(c))
      return 1;
   return 0;
}


/* FONCTION DE TEST DE POINT TERMINAUX EXTREMITES */

char
cnfExtremite6(Config c)
{
   c &= MASK_26VOISINS;

   if ((c & MASK_4) == c)
      return 1;
   if ((c & MASK_10) == c)
      return 1;
   if ((c & MASK_12) == c)
      return 1;
   if ((c & MASK_14) == c)
      return 1;
   if ((c & MASK_16) == c)
      return 1;
   if ((c & MASK_22) == c)
      return 1;
   return 0;
}

char
cnfExtremite6P(Config c)
{
   c &= MASK_26VOISINS;

   if ((c & MASK_4) == c)
      return 1;
   if ((c & MASK_10) == c)
      return 1;
   if ((c & MASK_12) == c)
      return 1;
   if ((c & MASK_14) == c)
      return 1;
   if ((c & MASK_16) == c)
      return 1;
   if ((c & MASK_22) == c)
      return 1;

   return 0;
}

char
cnfExtremite18(Config c)
{
   const int *p = Liste18voisins[CENTRE];
   int n = 0;

   while (*p != 127) {
      if (c & (1 << *p))
	 n++;
      p++;
   }

   if (n == 1)
      return 1;
   return 0;
}

char
cnfExtremite26(Config c)
{
   const int *p = Liste26voisins[CENTRE];
   int n = 0;
   while (*p != 127) {
      if (c & (1 << *p))
	 n++;
      p++;
   }


   if (n == 1)
      return 1;
   return 0;
}


void objMarque6BordListeBinarise(Objet3D * objet, ListePoints * bord);
void objMarque6BordListe(Objet3D * objet, ListePoints * bord);
void objMarque6BordDirListe(Objet3D * objet, ListePoints * bord,
			    int direction);

/*   nb_passes_max = 0  si non precise
     fichier_trace     : historique des suppressions
     ordre             : chaine permutation de NSEOHB */

size_t sqSequentiel(Objet3D * objet,
		    int nb_passes_max,
		    const char *ordre,
		    FonctionTestSimple fonction_test,
		    FonctionTestSurface test_surface, char binarise);


size_t sqMarquerPointsSurface(Objet3D * objet,
			      ListePoints * bord,
			      ListePoints * listesurf,
			      FonctionTestSurface test_surface);


char sqSansPointSimple(Objet3D * objet, FonctionTestSimple f_test);


/* Marque avec le bit 1 les voxels 6 voisins du fond de l'image */
void
objMarque6BordListe(Objet3D * objet, ListePoints * bord)
{
   size_t x, y, z;
   size_t tx, ty, tz;
   BYTE ***matrice = objet->matrice;
   char directions;
   size_t n = 0;

   POBJ_TAILLE(objet, tx, ty, tz);

   for (z = 0; z < tz; z++)
      for (y = 0; y < ty; y++)
	 for (x = 0; x < tx; x++)
	    if (matrice[x][y][z]) {
	       directions = objOrientation(objet, x, y, z);
	       if (directions) {
		  matrice[x][y][z] |= 2;
		  lpAjoute(bord, x, y, z, directions);
		  n++;
	       }
	    }
}


void
objMarque6BordListeBinarise(Objet3D * objet, ListePoints * bord)
{
   size_t x, y, z;
   size_t tx, ty, tz;
   BYTE ***matrice = objet->matrice;
   char directions;
   size_t n = 0;

   POBJ_TAILLE(objet, tx, ty, tz);

   for (z = 0; z < tz; z++)
      for (y = 0; y < ty; y++)
	 for (x = 0; x < tx; x++)
	    if (matrice[x][y][z]) {
	       matrice[x][y][z] = 1;
	       directions = objOrientation(objet, x, y, z);
	       if (directions) {
		  matrice[x][y][z] |= 2;
		  lpAjoute(bord, x, y, z, directions);
		  n++;
	       }
	    }
}


/* Marque avec le bit 1 les voxels 6 voisins du fond de l'image dans
   une certaine direction  */
void
objMarque6BordDirListe(Objet3D * objet, ListePoints * bord, int direction)
{
   size_t x, y, z;
   size_t tx, ty, tz;
   BYTE ***matrice = objet->matrice;
   size_t n = 0;

   POBJ_TAILLE(objet, tx, ty, tz);

   if (direction == NORD)
      for (z = 1; z < tz - 1; z++)
	 for (x = 1; x < tx - 1; x++) {
	    for (y = ty - 1; y && !(matrice[x][y][z]); y--) ;
	    if (y) {
	       matrice[x][y][z] |= 2;
	       lpAjoute(bord, x, y, z, 1 << NORD);
	       n++;
	    }
	 }

   if (direction == SUD)
      for (z = 1; z < tz - 1; z++)
	 for (x = 1; x < tx - 1; x++) {
	    for (y = 1; (y < ty - 1) && !(matrice[x][y][z]); y++) ;
	    if (y != ty - 1) {
	       matrice[x][y][z] |= 2;
	       lpAjoute(bord, x, y, z, 1 << SUD);
	       n++;
	    }
	 }

   if (direction == EST)
      for (z = 1; z < tz - 1; z++)
	 for (y = 1; y < ty - 1; y++) {

	    for (x = tx - 1; x && !(matrice[x][y][z]); x--) ;
	    if (x) {
	       matrice[x][y][z] |= 2;
	       lpAjoute(bord, x, y, z, 1 << EST);
	       n++;
	    }
	 }

   if (direction == OUEST)
      for (z = 1; z < tz - 1; z++)
	 for (y = 1; y < ty - 1; y++) {

	    for (x = 1; (x < tx - 1) && !(matrice[x][y][z]); x++) ;
	    if (x != tx - 1) {
	       matrice[x][y][z] |= 2;
	       lpAjoute(bord, x, y, z, 1 << OUEST);
	       n++;
	    }
	 }

   if (direction == HAUT)
      for (x = 1; x < tx - 1; x++)
	 for (y = 1; y < ty - 1; y++) {

	    for (z = tz - 1; z && !(matrice[x][y][z]); z--) ;
	    if (z) {
	       matrice[x][y][z] |= 2;
	       lpAjoute(bord, x, y, z, 1 << HAUT);
	       n++;
	    }
	 }
   if (direction == BAS)
      for (x = 1; x < tx - 1; x++)
	 for (y = 1; y < ty - 1; y++) {

	    for (z = 1; (z < tz - 1) && !(matrice[x][y][z]); z++) ;
	    if (z < tz - 1) {
	       matrice[x][y][z] |= 2;
	       lpAjoute(bord, x, y, z, 1 << BAS);
	       n++;
	    }
	 }
}


/* SQUELETTISATION EN SEQUENTIEL DIRECTIONEL */
size_t
sqSequentiel(Objet3D * objet,
	     int nb_passes_max,
	     const char *ordre,
	     FonctionTestSimple fonction_test,
	     FonctionTestSurface test_surface, char binarise)
{
   BYTE masque_reste[6];
   char ordre_defaut[6] = { 0, 1, 2, 3, 4, 5 };
   ListePoints bord, l2, listesurf;

   BYTE ***matrice;

   int nb_passes = 0;

   Config cnf;
   size_t nb_supp = 0, nb_supp_direction, old_supp;
   int direction;
   BYTE orientation;
   size_t x, y, z;
//   size_t tx, ty, tz;

   BYTE v;
   int n, n_dir;
   RecPoint *ppoint;
   BYTE mask_dir;
   char fini = 0;

   if (!ordre)
      ordre = ordre_defaut;

//   POBJ_TAILLE(objet, tx, ty, tz);
   matrice = objet->matrice;

   lpInit(&bord);
   lpInit(&l2);
   lpInit(&listesurf);

   masque_reste[5] = 0;

   for (n = 4; n >= 0; n--)
      masque_reste[n] = masque_reste[n + 1] | (1 << ordre[n + 1]);

   if (binarise)
      objMarque6BordListeBinarise(objet, &bord);
   else
      objMarque6BordListe(objet, &bord);

   while (!fini) {
      fini = 1;
      nb_passes++;

      old_supp = nb_supp;

      for (n_dir = 0; n_dir < 6; n_dir++) {
	 direction = ordre[n_dir];

	 /*             if (direction == AUCUNE_DIR)
	    mask_dir = 0;
	    else */
	 mask_dir = 1 << direction;

	 /* Marquage des points de surface */
	 if (test_surface)
	    sqMarquerPointsSurface(objet, &bord, &listesurf, test_surface);

	 lpDebut(&bord);
	 nb_supp_direction = 0;
	 while (!lpFin(&bord)) {
	    ppoint = lpSuivant(&bord);
	    x = ppoint->x;
	    y = ppoint->y;
	    z = ppoint->z;
	    orientation = ppoint->directions;
	    v = matrice[x][y][z];
	    if (v && !(v & 4) && (orientation & mask_dir)) {
	       cnf = objGetConfig(objet, x, y, z);
	       if (fonction_test(cnf)) {
		  nb_supp_direction++;
		  /* On enleve le point de l'objet */
		  matrice[x][y][z] = 0;

		  /* Ses 6-voisins deviennent points de bord */
		  if (matrice[x][y + 1][z] == 1) {
		     matrice[x][y + 1][z] |= 2;
		     lpAjoute(&l2, x, y + 1, z, 0);
		  }
		  if (matrice[x][y - 1][z] == 1) {
		     matrice[x][y - 1][z] |= 2;
		     lpAjoute(&l2, x, y - 1, z, 0);
		  }
		  if (matrice[x + 1][y][z] == 1) {
		     matrice[x + 1][y][z] |= 2;
		     lpAjoute(&l2, x + 1, y, z, 0);
		  }
		  if (matrice[x - 1][y][z] == 1) {
		     matrice[x - 1][y][z] |= 2;
		     lpAjoute(&l2, x - 1, y, z, 0);
		  }
		  if (matrice[x][y][z + 1] == 1) {
		     matrice[x][y][z + 1] |= 2;
		     lpAjoute(&l2, x, y, z + 1, 0);
		  }
		  if (matrice[x][y][z - 1] == 1) {
		     matrice[x][y][z - 1] |= 2;
		     lpAjoute(&l2, x, y, z - 1, 0);
		  }
	       } else
		  /* On ajoute a liste l2 si pas point de bord dans une direction suivante */
	       if (!(orientation & (masque_reste[n_dir])))
		  lpAjoute(&l2, x, y, z, 0);
	    }
	 }
	 nb_supp += nb_supp_direction;
      }

      /* Recopie bord = l2 et mise a jour des orientations */
      lpRaz(&bord);
      lpDebut(&l2);
      while (!lpFin(&l2)) {
	 ppoint = lpSuivant(&l2);
	 lpAjoute(&bord, ppoint->x, ppoint->y, ppoint->z,
		  objOrientation(objet, ppoint->x, ppoint->y, ppoint->z));
      }

      lpRaz(&l2);

      fini = 0;
      if (old_supp == nb_supp) {
	 fini = 1;
      }
      if (nb_passes == nb_passes_max)
	 fini = 1;
   }


   lpFree(&bord);
   lpFree(&l2);
   lpFree(&listesurf);
   return nb_supp;
}


size_t
sqMarquerPointsSurface(Objet3D * objet,
		       ListePoints * bord,
		       ListePoints * listesurf,
		       FonctionTestSurface test_surface)
{
   int m;
   BYTE ***matrice = objet->matrice;
   size_t pm = 0;
   char retour;
   size_t x, y, z;
   RecPoint *ppoint;
   Config cnf;
   BYTE valeur;
   size_t nbp;

   /* On met d'abord a jour la liste des points de surfaces et voisins connus */
   nbp = listesurf->nb_points;
   lpDebut(listesurf);
   while (!lpFin(listesurf) && nbp) {
      ppoint = lpSuivant(listesurf);
      x = ppoint->x;
      y = ppoint->y;
      z = ppoint->z;
      nbp--;

      if (matrice[x][y][z]) {
	 cnf = objGetConfig(objet, x, y, z);
	 if ((retour = test_surface(cnf)) & 2) {	/* On garde le point et ses voisins */
	    /* On marque ses voisins dans l'objet comme faisant partie du squelette */
	    for (m = 0; m < 27; m++) {
	       valeur = matrice[x + dx_of[m]][y + dy_of[m]][z + dz_of[m]];
	       if (valeur && !(valeur & 4)) {
		  matrice[x + dx_of[m]][y + dy_of[m]][z + dz_of[m]] |= 4;
		  lpAjoute(listesurf, x + dx_of[m], y + dy_of[m],
			   z + dz_of[m], 0);
		  pm++;
	       }
	    }
	 } else if (retour & 1) {	/* On marque le point comme point terminal */
	    valeur = matrice[x][y][z];
	    if (valeur && !(valeur & 4)) {
	       matrice[x][y][z] |= 4;
	       lpAjoute(listesurf, x, y, z, 0);
	       pm++;
	    }
	 }
      }
   }


   /* Puis on parcours le bord */
   lpDebut(bord);
   while (!lpFin(bord)) {
      ppoint = lpSuivant(bord);
      x = ppoint->x;
      y = ppoint->y;
      z = ppoint->z;

      if (matrice[x][y][z]) {
	 cnf = objGetConfig(objet, x, y, z);
	 if ((retour = test_surface(cnf)) & 2) {	/* Le point est un point de surface */
	    /* On marque ses voisins dans l'objet comme faisant partie du squelette */
	    for (m = 0; m < 27; m++) {
	       valeur = matrice[x + dx_of[m]][y + dy_of[m]][z + dz_of[m]];
	       if (valeur && !(valeur & 4)) {
		  matrice[x + dx_of[m]][y + dy_of[m]][z + dz_of[m]] |= 4;
		  lpAjoute(listesurf, x + dx_of[m], y + dy_of[m],
			   z + dz_of[m], 0);
		  pm++;
	       }
	    }
	 } else if (retour & 1) {
	    valeur = matrice[x][y][z];
	    if (valeur && !(valeur & 4)) {
	       matrice[x][y][z] |= 4;
	       lpAjoute(listesurf, x, y, z, 0);
	       pm++;
	    }

	 }
      }
   }

   return pm;
}


char
sqSansPointSimple(Objet3D * objet, FonctionTestSimple f_test)
{
   size_t tx, ty, tz;
   size_t x, y, z;
   BYTE ***matrice = objet->matrice;
   Config cnf;

   POBJ_TAILLE(objet, tx, ty, tz);


   for (x = 1; x < tx - 1; x++)
      for (y = 1; y < ty - 1; y++)
	 for (z = 1; z < tz - 1; z++) {
	    if (matrice[x][y][z]) {
	       cnf = objGetConfig(objet, x, y, z);
	       if (f_test(cnf))
		  return 0;
	    }
	 }
   return 1;
}

/**
 *  Squelettisation homotopique 3D d'un volume.  
 *  @param connexite Limite a 6 ou 26 pour respectivement (6,26) ou (26,6).
 */
Errc PHomotopicSkeletonization( const Img3duc &ims, Img3duc &imd, int connexite) {
   Objet3D objet;
   UShort x, y, z;
   UShort tx, ty, tz;

   char ordre_sq[6] = { NORD, SUD, EST, OUEST, HAUT, BAS };

   objInit(&objet);
   objAllouer(&objet, tx = (UShort)ims.Width(), ty = (UShort)ims.Height(), tz =
	      (UShort)ims.Depth());

   for (x = 0; x < tx; x++)
      for (y = 0; y < ty; y++)
	 for (z = 0; z < tz; z++) {
	    objet.matrice[x][y][z] = (ims[z][y][x] > 0) ? 1 : 0;
	 }


   switch (connexite) {
   case 6:


      sqSequentiel(&objet, 0,	// Nb passes
		   ordre_sq, cnfSimple6, NULL,	// Test surface
		   0);		// Binarise 

      break;
   case 26:
      sqSequentiel(&objet, 0, ordre_sq, cnfSimple26, NULL, 0);
      break;
   }


   for (x = 0; x < tx; x++)
      for (y = 0; y < ty; y++)
	 for (z = 0; z < tz; z++) {
	    imd[z][y][x] = (objet.matrice[x][y][z] > 0) ? 255 : 0;
	 }

   objFree(&objet);

   return SUCCESS;
}


#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s connexity [-m mask] [im_in|-] [im_out|-]"
#define	PARC	1
#define	FINC	1
#define	FOUTC	1
#define	MASK	1

int main( int argc, char *argv[] ) {
   Errc result;			// The result code of the execution.
   Pobject *mask;		// The region mask.
   Pobject *objin[FINC + 1];	// The input objects.
   Pobject *objs[FINC + 1];	// The source objects masked by the mask.
   Pobject *objout[FOUTC + 1];	// The output object.
   Pobject *objd[FOUTC + 1];	// The result object of the execution.
   char* parv[PARC + 1];	// The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout,
	    objd, parv, USAGE, MASK);

   switch (objs[0]->Type()) {
   case Po_Img3duc:{
	 Img3duc *const ims = (Img3duc *) objs[0];
	 objd[0] = new Img3duc(ims->Size());
	 Img3duc *const imd = (Img3duc *) objd[0];

	 result = PHomotopicSkeletonization(*ims, *imd, atoi(parv[0]));
      } break;
   default:
      PrintErrorFormat(objin, FINC);
      result = FAILURE;
   }

   if (result)
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
   Exit(result);
   return 0;
}

#endif
