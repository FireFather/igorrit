
#include "RobboLito.h"
#include <string.h>

#define MASSIMO_ANNO 256
#define MASSIMO_ABISSO 256

#define NUMERO(x,y)\
  ((( (x) - ANNI) & (MASSIMO_ANNO - 1)) * MASSIMO_ABISSO\
   + (MASSIMO_ABISSO - (y) ))

void
anno_incremento ()
{
  ANNI += 1;
  if (ANNI == MASSIMO_ANNO)
    ANNI = 0;
}

static uint64 zobrist_dimensione = 0x400000;
static logico zobrist_inizializzazione_pietra = 0;

void
zobrist_libero ()
{
  memset (elenco_zobrist, 0, zobrist_dimensione * sizeof (tipo_zobrist));
  memset (elenco_zobrist_pv, 0, 0x10000 * sizeof (tipo_zobrist_pv));
  ANNI = 0;
}

int inizializzazione_zobrist (int mb)
{
  uint64 ull1 = 1;
  uint32 bsr_mb = BSR (mb);
  uint64 pow2bsrmb = ull1 << bsr_mb;
  uint64 Mb = (pow2bsrmb << 20);
  ANNI = 0;
  zobrist_dimensione = Mb / sizeof (tipo_zobrist);
  if (zobrist_dimensione > 0x100000000)
    zobrist_dimensione = 0x100000000;
  mb = (zobrist_dimensione * sizeof (tipo_zobrist)) >> 20;
  elenco_celare = zobrist_dimensione - 4;
  if (zobrist_inizializzazione_pietra)
    free (elenco_zobrist);
  zobrist_inizializzazione_pietra = VERO;
  POSIX_MEMALIGN (elenco_zobrist, 64, zobrist_dimensione * sizeof (tipo_zobrist));
  zobrist_libero ();
  return mb;
}

void
Zobrist_basso_totale (tipo_posizione *POSIZIONE, int mossa, int abisso, int valu)
{
  int ABISSO, i, k = DINAMICO->Zobrist & elenco_celare;
  tipo_zobrist *tr;
  int massimo = 0, w = 0;
  mossa &= 0x7fff;
  for (i = 0; i < 4; i++)
    {
      tr = elenco_zobrist + (k + i);
      if ((tr->zobrist ^ (DINAMICO->Zobrist >> 32)) == 0
	  && (!tr->abisso_basso || zobrist_totale (tr))
	  && tr->abisso_basso <= abisso)
	{
	  tr->abisso_basso = abisso;
	  tr->mossa = mossa;
	  tr->valu_bassa = valu;
	  tr->anni = ANNI;
	  tr->pietras |= pietra_bassa | pietra_totale;
	  return;
	}
      ABISSO = MASSIMO (tr->abisso_basso, tr->abisso_alto);
      if (NUMERO (tr->anni, ABISSO) > massimo)
	{
	  massimo = NUMERO (tr->anni, ABISSO);
	  w = i;
	}
    }
  tr = elenco_zobrist + (k + w);
  tr->zobrist = (DINAMICO->Zobrist >> 32);
  tr->abisso_alto = 0;
  tr->valu_alta = 0;
  tr->abisso_basso = abisso;
  tr->mossa = mossa;
  tr->valu_bassa = valu;
  tr->anni = ANNI;
  tr->pietras = pietra_bassa | pietra_totale;
  return;
}

void
Zobrist_alto_tagliato (tipo_posizione *POSIZIONE, int abisso, int valu)
{
  int ABISSO, i, k = DINAMICO->Zobrist & elenco_celare;
  tipo_zobrist *tr;
  int massimo = 0, w = 0;
  for (i = 0; i < 4; i++)
    {
      tr = elenco_zobrist + (k + i);
      if (!(tr->zobrist ^ (DINAMICO->Zobrist >> 32))
	  && (!tr->abisso_alto || zobrist_tagliato (tr))
	  && tr->abisso_alto <= abisso)
	{
	  tr->abisso_alto = abisso;
	  tr->valu_alta = valu;
	  tr->anni = ANNI;
	  tr->pietras |= pietra_alta | pietra_tagliata;
	  return;
	}
      ABISSO = MASSIMO (tr->abisso_basso, tr->abisso_alto);
      if (NUMERO (tr->anni, ABISSO) > massimo)
	{
	  massimo = NUMERO (tr->anni, ABISSO);
	  w = i;
	}
    }
  tr = elenco_zobrist + (k + w);
  tr->zobrist = (DINAMICO->Zobrist >> 32);
  tr->abisso_basso = 0;
  tr->mossa = 0;
  tr->valu_bassa = 0;
  tr->abisso_alto = abisso;
  tr->valu_alta = valu;
  tr->anni = ANNI;
  tr->pietras = pietra_alta | pietra_tagliata;
  return;
}

void
Zobrist_basso (uint64 Z, int mossa, int abisso, int valu)
{
  int ABISSO, i, k = Z & elenco_celare;
  tipo_zobrist *tr;
  int massimo = 0, w = 0;
  mossa &= 0x7fff;
  for (i = 0; i < 4; i++)
    {
      tr = elenco_zobrist + (k + i);
      if (!(tr->zobrist ^ (Z >> 32)) && !zobrist_preciso (tr)
	  && tr->abisso_basso <= abisso)
	{
	  tr->abisso_basso = abisso;
	  tr->mossa = mossa;
	  tr->valu_bassa = valu;
	  tr->anni = ANNI;
	  tr->pietras |= pietra_bassa;
	  tr->pietras &= ~pietra_totale;
	  return;
	}
      ABISSO = MASSIMO (tr->abisso_basso, tr->abisso_alto);
      if (NUMERO (tr->anni, ABISSO) > massimo)
	{
	  massimo = NUMERO (tr->anni, ABISSO);
	  w = i;
	}
    }
  tr = elenco_zobrist + (k + w);
  tr->zobrist = (Z >> 32);
  tr->abisso_alto = 0;
  tr->valu_alta = 0;
  tr->abisso_basso = abisso;
  tr->mossa = mossa;
  tr->valu_bassa = valu;
  tr->anni = ANNI;
  tr->pietras = pietra_bassa;
  return;
}

void
Zobrist_alto (uint64 Z, int abisso, int valu)
{
  int ABISSO, i, k = Z & elenco_celare;
  tipo_zobrist *tr;
  int massimo = 0, w = 0;
  for (i = 0; i < 4; i++)
    {
      tr = elenco_zobrist + (k + i);
      if (!(tr->zobrist ^ (Z >> 32)) && !zobrist_preciso (tr)
	  && tr->abisso_alto <= abisso)
	{
	  tr->abisso_alto = abisso;
	  tr->valu_alta = valu;
	  tr->anni = ANNI;
	  tr->pietras |= pietra_alta;
	  tr->pietras &= ~pietra_tagliata;
	  return;
	}
      ABISSO = MASSIMO (tr->abisso_basso, tr->abisso_alto);
      if (NUMERO (tr->anni, ABISSO) > massimo)
	{
	  massimo = NUMERO (tr->anni, ABISSO);
	  w = i;
	}
    }
  tr = elenco_zobrist + (k + w);
  tr->zobrist = (Z >> 32);
  tr->abisso_basso = 0;
  tr->mossa = 0;
  tr->valu_bassa = 0;
  tr->abisso_alto = abisso;
  tr->valu_alta = valu;
  tr->anni = ANNI;
  tr->pietras = pietra_alta;
  return;
}

static void
pv_zobrist (tipo_posizione *POSIZIONE, int mossa, int abisso, int valu)
{
  int i, k = DINAMICO->Zobrist & elenco_celare_pv;
  tipo_zobrist_pv *tr;
  int w = 0, massimo = 0;
  for (i = 0; i < 4; i++)
    {
      tr = elenco_zobrist_pv + (k + i);
      if (tr->zobrist == DINAMICO->Zobrist)
	{
	  tr->abisso = abisso;
	  tr->valu = valu;
	  tr->mossa = mossa;
	  tr->anni = ANNI;
	  return;
	}
      if (NUMERO (tr->anni, tr->abisso) > massimo)
	{
	  massimo = NUMERO (tr->anni, tr->abisso);
	  w = i;
	}
    }
  tr = elenco_zobrist_pv + (k + w);
  tr->zobrist = DINAMICO->Zobrist;
  tr->abisso = abisso;
  tr->mossa = mossa;
  tr->valu = valu;
  tr->anni = ANNI;
}

void
Zobrist_preciso
 (tipo_posizione *POSIZIONE, int mossa, int abisso, int valu, int PIETRAS)
{
  int ABISSO, i, j, k = DINAMICO->Zobrist & elenco_celare;
  tipo_zobrist *tr;
  int massimo = 0, w = 0;
  mossa &= 0x7fff;
  pv_zobrist (POSIZIONE, mossa, abisso, valu);
  for (i = 0; i < 4; i++)
    {
      tr = elenco_zobrist + (k + i);
      if ((tr->zobrist ^ (DINAMICO->Zobrist >> 32)) == 0
	  && MASSIMO (tr->abisso_alto, tr->abisso_basso) <= abisso)
	{
	  tr->abisso_alto = tr->abisso_basso = abisso;
	  tr->mossa = mossa;
	  tr->valu_alta = tr->valu_bassa = valu;
	  tr->anni = ANNI;
	  tr->pietras = PIETRAS;
	  for (j = i + 1; j < 4; j++)
	    {
	      tr = elenco_zobrist + (k + j);
	      if ((tr->zobrist ^ (DINAMICO->Zobrist >> 32)) == 0
		  && MASSIMO (tr->abisso_alto, tr->abisso_basso) <= abisso)
		{
		  memset (tr, 0, 16);
		  tr->anni = ANNI ^ (MASSIMO_ANNO / 2);
		}
	    }
	  return;
	}
      ABISSO = MASSIMO (tr->abisso_basso, tr->abisso_alto);
      if (NUMERO (tr->anni, ABISSO) > massimo)
	{
	  massimo = NUMERO (tr->anni, ABISSO);
	  w = i;
	}
    }
  tr = elenco_zobrist + (k + w);
  tr->zobrist = (DINAMICO->Zobrist >> 32);
  tr->abisso_alto = tr->abisso_basso = abisso;
  tr->mossa = mossa;
  tr->valu_alta = tr->valu_bassa = valu;
  tr->anni = ANNI;
  tr->pietras = PIETRAS;
  return;
}
