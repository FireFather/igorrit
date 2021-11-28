#ifndef BUILD_cerca_fine
#define BUILD_cerca_fine
#include "RobboLito.h"
#include "cerca_fine.c"
#include "bianco.h"
#else
#include "nero.h"
#endif

#define RETURN(x) return(x)
int
mio_cerca_fine (tipo_posizione *POSIZIONE, int VALU, int abisso)
{
  int valu, i, k = DINAMICO->Zobrist & elenco_celare, v, valu_migliore;
  uint32 TEMP, mossa, mossa_tras = 0, tras_abisso, mossa_abisso = 0;
  uint64 targa;
  tipo_lista_mossa LISTA[256], *lista, *p, *q;
  tipo_dinamico *POSIZIONE0 = DINAMICO;
  tipo_zobrist *tr;
  //YUSUF128 H[1];

  if (VALU < -VALU_MATTO + 1)
    RETURN (-VALU_MATTO + 1);
  if (VALU > VALU_MATTO - 1)
    RETURN (VALU_MATTO - 1);

  ripetizione_verificare;
  occupato5_verificare;

  for (i = 0; i < 4; i++)
    {
      tr = elenco_zobrist + (k + i);
      HYATT_HASH();
      if ((tr->zobrist ^ (DINAMICO->Zobrist >> 32)) == 0)
	{
	  if (tr->abisso_basso)
	    {
	      valu = zobrist_valu_bassa (tr);
	      if (valu >= VALU)
		RETURN (valu);
	    }
	  if (tr->abisso_alto)
	    {
	      valu = zobrist_valu_alta (tr);
	      if (valu < VALU)
		RETURN (valu);
	    }
	  tras_abisso = tr->abisso_basso;
	  mossa = tr->mossa;
	  if (mossa && tras_abisso > mossa_abisso)
	    {
	      mossa_abisso = tras_abisso;
	      mossa_tras = mossa;
	    }
	}
    }

  valu_migliore = POSIZIONE0->valu + valu_tempo2;
  if (valu_migliore >= VALU)
    RETURN (valu_migliore);
  v = VALU - pedone_prugna;
  targa = tuo_occupato;
  if (v > valu_migliore)
    {
      v = VALU - minore_prugna;
      targa ^= bitbordo_tuo_pedone;
      if (v > valu_migliore)
	{
	  targa ^= (bitbordo_tuo_cavallo | bitbordo_tuo_alfiere);
	  v = VALU - torre_prugna;
	  if (v > valu_migliore)
	    targa ^= bitbordo_tuo_torre;
	}
      if (bitbordo_tuo_pedone & mio_attaco)
	valu_migliore += pedone_prugna;
    }

  lista = mio_cattura (POSIZIONE, LISTA, targa);
  p = LISTA;
  while (p->mossa)
    {
      if ((p->mossa & 0x7fff) == mossa_tras)
	p->mossa |= 0xffff0000;
      p++;
    }
  p = LISTA;
  while (p->mossa)
    {
      mossa = p->mossa;
      q = ++p;
      while (q->mossa)
	{
	  if (mossa < q->mossa)
	    {
	      TEMP = q->mossa;
	      q->mossa = mossa;
	      mossa = TEMP;
	    }
	  q++;
	}
      if (!SEE_facile (mossa) && (mossa & 0x7fff) != mossa_tras &&
	  quadretto_fisso[DI (mossa)] & ~mio_xray && !mio_SEE (POSIZIONE, mossa))
	continue;
      mossa &= 0x7fff;
      FARE (POSIZIONE, mossa);
      chiamata_valu_pigra (VALU, VALU, valu_pigro, mossa);
      if (mossa_illegale)
	{
	  ANNULLA (POSIZIONE, mossa);
	  continue;
	}
      if (mossa_scacco)
	v = -tuo_cerca_fine_scacco (POSIZIONE, 1 - VALU, abisso - 1);
      else
	v = -tuo_cerca_fine (POSIZIONE, 1 - VALU, abisso - 1);
      ANNULLA (POSIZIONE, mossa);
      CHECK_HALT ();
      if (v <= valu_migliore)
	continue;
      valu_migliore = v;
      if (v >= VALU)
	{
	  Zobrist_basso (DINAMICO->Zobrist, mossa, 1, v);
	  RETURN (v);
	}
    }
  if (abisso >= -1 && POSIZIONE0->valu >= VALU - (100 + (12 << (abisso + 4))))
    {
      lista = mio_scacco_zitto (POSIZIONE, LISTA, targa);
      for (i = 0; i < lista - LISTA; i++)
	{
	  mossa = LISTA[i].mossa;
	  mossa &= 0x7fff;
	  FARE (POSIZIONE, mossa);
	  chiamata_valu_pigra (VALU, VALU, valu_pigro, mossa);
	  if (mossa_illegale)
	    {
	      ANNULLA (POSIZIONE, mossa);
	      continue;
	    }
	  v = -tuo_cerca_fine_scacco (POSIZIONE, 1 - VALU, abisso - 1);
	  ANNULLA (POSIZIONE, mossa);
	  CHECK_HALT ();
	  if (v <= valu_migliore)
	    continue;
	  valu_migliore = v;
	  if (v >= VALU)
	    {
	      Zobrist_basso (DINAMICO->Zobrist, mossa, 1, v);
	      RETURN (v);
	    }
	}
    }
  Zobrist_alto (DINAMICO->Zobrist, 1, valu_migliore);
  RETURN (valu_migliore);
}

int
mio_cerca_fine_scacco (tipo_posizione *POSIZIONE, int VALU, int abisso)
{
  int ignorato, valu, i, k = DINAMICO->Zobrist & elenco_celare,
    v, valu_migliore, tras_abisso, mossa_abisso = 0;
  tipo_zobrist *tr;
  uint64 targa;
  tipo_lista_mossa LISTA[256], *lista, *p, *q;
  tipo_dinamico *POSIZIONE0;
  uint32 mossa, TEMP, mossa_tras = 0;
  //YUSUF128 H[1];

  POSIZIONE0 = DINAMICO;
  if (VALU < -VALU_MATTO + 1)
    RETURN (-VALU_MATTO + 1);
  if (VALU > VALU_MATTO - 1)
    RETURN (VALU_MATTO - 1);

  ripetizione_verificare;
  occupato5_verificare;
  for (i = 0; i < 4; i++)
    {
      tr = elenco_zobrist + (k + i);
      HYATT_HASH();
      if ((tr->zobrist ^ (DINAMICO->Zobrist >> 32)) == 0)
	{
	  if (tr->abisso_basso)
	    {
	      valu = zobrist_valu_bassa (tr);
	      if (valu >= VALU)
		RETURN (valu);
	    }
	  if (tr->abisso_alto)
	    {
	      valu = zobrist_valu_alta (tr);
	      if (valu < VALU)
		RETURN (valu);
	    }
	  tras_abisso = tr->abisso_basso;
	  mossa = tr->mossa;
	  if (mossa && tras_abisso > mossa_abisso)
	    {
	      mossa_abisso = tras_abisso;
	      mossa_tras = mossa;
	    }
	}
    }
  valu_migliore = HEIGHT (POSIZIONE) - VALU_MATTO;
  targa = 0xffffffffffffffff;
  if (POSIZIONE0->valu + scacco_prugna < VALU)
    {
      valu_migliore = POSIZIONE0->valu + scacco_prugna;
      v = VALU - 200;
      targa = tuo_occupato;
      if (v > valu_migliore)
	{
	  targa ^= bitbordo_tuo_pedone;
	  v = VALU - 500;
	  valu_migliore += 200;
	  if (v > valu_migliore)
	    targa ^= (bitbordo_tuo_cavallo | bitbordo_tuo_alfiere);
	}
    }

  lista = mio_evasione (POSIZIONE, LISTA, targa);
  if ((lista - LISTA) > 1)
    abisso--;
  p = LISTA;
  while (p->mossa)
    {
      if ((p->mossa & 0x7fff) == mossa_tras)
	p->mossa |= 0xfff00000;
      p++;
    }
  p = LISTA;
  ignorato = 0;
  while (p->mossa)
    {
      mossa = p->mossa;
      q = ++p;
      while (q->mossa)
	{
	  if (mossa < q->mossa)
	    {
	      TEMP = q->mossa;
	      q->mossa = mossa;
	      mossa = TEMP;
	    }
	  q++;
	}
      if (INTERROGATIVO_INTERPOSIZIONE (mossa) && VALU > -25000
	  && (mossa & 0x7fff) != mossa_tras && !mio_SEE (POSIZIONE, mossa))
	{
	  ignorato++;
	  continue;
	}
      if (POSIZIONE->qu[AI (mossa)] == 0 && (mossa & 0x6000) == 0
	  && (mossa & 0x7fff) != mossa_tras && mio_nullo
	  && MASSIMO_POSIZIONALE (mossa) + POSIZIONE0->valu < VALU + 25
	  && VALU > -25000)
	{
	  ignorato++;
	  continue;
	}
      mossa &= 0x7fff;
      FARE (POSIZIONE, mossa);
      chiamata_valu_pigra (VALU, VALU, valu_pigro, mossa);
      if (mossa_illegale)
	{
	  ANNULLA (POSIZIONE, mossa);
	  continue;
	}
      if (mossa_scacco)
	v = -tuo_cerca_fine_scacco (POSIZIONE, 1 - VALU, abisso);
      else
	v = -tuo_cerca_fine (POSIZIONE, 1 - VALU, abisso);
      ANNULLA (POSIZIONE, mossa);
      CHECK_HALT ();
      if (v <= valu_migliore)
	continue;
      valu_migliore = v;
      if (v >= VALU)
	{
	  Zobrist_basso (DINAMICO->Zobrist, mossa, 1, v);
	  RETURN (v);
	}
    }
  if (ignorato && valu_migliore < -25000)
    valu_migliore = VALU - 1;
  Zobrist_alto (DINAMICO->Zobrist, 1, valu_migliore);
  RETURN (valu_migliore);
}
