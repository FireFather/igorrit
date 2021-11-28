#ifndef BUILD_cerca_fine_pv
#define BUILD_cerca_fine_pv
#include "RobboLito.h"
#include "cerca_fine_pv.c"
#include "bianco.h"
#else
#include "nero.h"
#endif

#define RETURN(x) return(x)

int
mio_pv_cerca_fine (tipo_posizione *POSIZIONE, int BASSO, int ALTO, int abisso)
{
  int i;
  uint32 mossa_buona = 0, mossa_tras = 0, mossa, cattura_brutta[64],
    tras_abisso, mossa_abisso = 0;
  int valu_migliore, valu;
  uint64 targa;
  tipo_lista_mossa LISTA[256], *lista, *p, *q;
  int TEMP, v;
  tipo_dinamico *POSIZIONE0 = DINAMICO;
  int k = DINAMICO->Zobrist & elenco_celare;
  int cb = 0;
  tipo_zobrist *tr;
  //YUSUF128 H[1];

  ripetizione_verificare;
  if (ALTO < -VALU_MATTO)
    RETURN (-VALU_MATTO);
  if (BASSO > VALU_MATTO)
    RETURN (VALU_MATTO);
  for (i = 0; i < 4; i++)
    {
      tr = elenco_zobrist + (k + i);
      HYATT_HASH();
      if ((tr->zobrist ^ (DINAMICO->Zobrist >> 32)) == 0)
	{
	  if (zobrist_preciso (tr))
	    {
	      valu = zobrist_valu_alta (tr);
	      RETURN (valu);
	    }
	  if (tr->abisso_basso)
	    {
	      valu = zobrist_valu_bassa (tr);
	      if (valu >= ALTO)
		RETURN (valu);
	    }
	  if (tr->abisso_alto)
	    {
	      valu = zobrist_valu_alta (tr);
	      if (valu <= BASSO)
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
  targa = tuo_occupato;
  if (valu_migliore >= ALTO)
    {
      RETURN (valu_migliore);
    }
  else if (valu_migliore > BASSO)
    BASSO = valu_migliore;
  else
    {
      if (valu_migliore < BASSO - pedone_prugna)
	{
	  targa ^= bitbordo_tuo_pedone;
	  if (valu_migliore < BASSO - minore_prugna)
	    {
	      targa ^= (bitbordo_tuo_cavallo | bitbordo_tuo_alfiere);
	      if (valu_migliore < BASSO - torre_prugna)
		targa ^= bitbordo_tuo_torre;
	    }
	  valu_migliore += pedone_prugna;
	}
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
      if (SEE_facile (mossa) || (mossa & 0x7fff) == mossa_tras
	  || mio_SEE (POSIZIONE, mossa))
	{
	  mossa &= 0x7fff;
	  FARE (POSIZIONE, mossa);
	  chiamata_valu_piena (mossa);
	  if (mossa_illegale)
	    {
	      ANNULLA (POSIZIONE, mossa);
	      continue;
	    }
	  if (mossa_scacco)
	    v = -tuo_pv_cerca_fine_scacco (POSIZIONE, -ALTO, -BASSO, abisso - 1);
	  else
	    v = -tuo_pv_cerca_fine (POSIZIONE, -ALTO, -BASSO, abisso - 1);
	  ANNULLA (POSIZIONE, mossa);
	  CHECK_HALT ();
	  if (v <= valu_migliore)
	    continue;
	  valu_migliore = v;
	  if (v <= BASSO)
	    continue;
	  BASSO = v;
	  mossa_buona = mossa;
	  if (v >= ALTO)
	    {
	      Zobrist_basso (DINAMICO->Zobrist, mossa, 1, v);
	      RETURN (v);
	    }
	}
      else
	cattura_brutta[cb++] = mossa;
    }
  if (abisso > 0)
    for (i = 0; i < cb; i++)
      {
	mossa = cattura_brutta[i] & 0x7fff;
	FARE (POSIZIONE, mossa);
	chiamata_valu_piena (mossa);
	if (mossa_illegale)
	  {
	    ANNULLA (POSIZIONE, mossa);
	    continue;
	  }
	if (mossa_scacco)
	  v = -tuo_pv_cerca_fine_scacco (POSIZIONE, -ALTO, -BASSO, abisso - 1);
	else
	  v = -tuo_pv_cerca_fine (POSIZIONE, -ALTO, -BASSO, abisso - 1);
	ANNULLA (POSIZIONE, mossa);
	CHECK_HALT ();
	if (v <= valu_migliore)
	  continue;
	valu_migliore = v;
	if (v <= BASSO)
	  continue;
	BASSO = v;
	mossa_buona = mossa;
	if (v >= ALTO)
	  {
	    Zobrist_basso (DINAMICO->Zobrist, mossa, 1, v);
	    RETURN (v);
	  }
      }
  if (abisso >= -2
      && POSIZIONE0->valu >= BASSO - (100 + (12 << (abisso + 5))))
    {
      lista = mio_scacco_zitto (POSIZIONE, LISTA, targa);
      for (i = 0; i < lista - LISTA; i++)
	{
	  mossa = LISTA[i].mossa & 0x7fff;
	  FARE (POSIZIONE, mossa);
	  chiamata_valu_piena (mossa);
	  if (mossa_illegale)
	    {
	      ANNULLA (POSIZIONE, mossa);
	      continue;
	    }
	  v = -tuo_pv_cerca_fine_scacco (POSIZIONE, -ALTO, -BASSO, abisso - 1);
	  ANNULLA (POSIZIONE, mossa);
	  CHECK_HALT ();
	  if (v <= valu_migliore)
	    continue;
	  valu_migliore = v;
	  if (v <= BASSO)
	    continue;
	  BASSO = v;
	  mossa_buona = mossa;
	  if (v >= ALTO)
	    {
	      Zobrist_basso (DINAMICO->Zobrist, mossa, 1, v);
	      RETURN (v);
	    }
	}
      if (abisso >= 0 && BASSO <= POSIZIONE0->valu + 150)
	{
	  lista = mio_guadagno (POSIZIONE, LISTA, BASSO - POSIZIONE0->valu);
	  for (i = 0; i < lista - LISTA; i++)
	    {
	      mossa = LISTA[i].mossa & 0x7fff;
	      FARE (POSIZIONE, mossa);
	      chiamata_valu_piena (mossa);
	      if (-POSIZIONE1->valu < BASSO)
		{
		  ANNULLA (POSIZIONE, mossa);
		  continue;
		}
	      if (mossa_illegale || mossa_scacco)
		{
		  ANNULLA (POSIZIONE, mossa);
		  continue;
		}
	      v = -tuo_pv_cerca_fine (POSIZIONE, -ALTO, -BASSO, 0);
	      ANNULLA (POSIZIONE, mossa);
	      CHECK_HALT ();
	      if (v <= valu_migliore)
		continue;
	      valu_migliore = v;
	      if (v <= BASSO)
		continue;
	      BASSO = v;
	      mossa_buona = mossa;
	      Zobrist_basso (DINAMICO->Zobrist, mossa, 1, v);
	      if (v >= ALTO)
		RETURN (v);
	    }
	}
    }
  if (mossa_buona)
    {
      Zobrist_preciso (POSIZIONE, mossa_buona, 1, valu_migliore, pietra_precisa);
      RETURN (valu_migliore);
    }
  Zobrist_alto (DINAMICO->Zobrist, 1, valu_migliore);
  RETURN (valu_migliore);
}

int
mio_pv_cerca_fine_scacco (tipo_posizione *POSIZIONE, int BASSO, int ALTO, int abisso)
{
  int i;
  uint32 mossa_tras = 0, mossa_buona = 0, mossa, TEMP;
  int valu_migliore, valu;
  uint64 targa;
  tipo_lista_mossa LISTA[256], *lista, *p, *q;
  int k = DINAMICO->Zobrist & elenco_celare;
  int v, tras_abisso, mossa_abisso = 0;
  tipo_dinamico *POSIZIONE0 = DINAMICO;
  tipo_zobrist *tr;
  //YUSUF128 H[1];

  ripetizione_verificare;
  if (ALTO < -VALU_MATTO)
    RETURN (-VALU_MATTO);
  if (BASSO > VALU_MATTO)
    RETURN (VALU_MATTO);
  for (i = 0; i < 4; i++)
    {
      tr = elenco_zobrist + (k + i);
      HYATT_HASH();
      if ((tr->zobrist ^ (DINAMICO->Zobrist >> 32)) == 0)
	{
	  if (zobrist_preciso (tr))
	    {
	      valu = zobrist_valu_alta (tr);
	      RETURN (valu);
	    }
	  if (tr->abisso_basso)
	    {
	      valu = zobrist_valu_bassa (tr);
	      if (valu >= ALTO)
		RETURN (valu);
	    }
	  if (tr->abisso_alto)
	    {
	      valu = zobrist_valu_alta (tr);
	      if (valu <= BASSO)
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
  if (POSIZIONE0->valu + scacco_prugna < BASSO)
    {
      valu_migliore = POSIZIONE0->valu + scacco_prugna;
      v = BASSO - 200;
      targa = tuo_occupato;
      if (v > valu_migliore)
	{
	  targa ^= bitbordo_tuo_pedone;
	  v = BASSO - 500;
	  valu_migliore += 200;
	  if (v > valu_migliore)
	    targa ^= (bitbordo_tuo_cavallo | bitbordo_tuo_alfiere);
	}
    }
  lista = mio_evasione (POSIZIONE, LISTA, targa);
  if ((lista - LISTA) != 1)
    abisso--;
  p = LISTA;
  while (p->mossa)
    {
      if ((p->mossa & 0x7fff) == mossa_tras)
	p->mossa |= 0xfff00000;
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
      mossa &= 0x7fff;
      FARE (POSIZIONE, mossa);
      chiamata_valu_piena (mossa);
      if (mossa_illegale)
	{
	  ANNULLA (POSIZIONE, mossa);
	  continue;
	}
      if (mossa_scacco)
	v = -tuo_pv_cerca_fine_scacco (POSIZIONE, -ALTO, -BASSO, abisso);
      else
	v = -tuo_pv_cerca_fine (POSIZIONE, -ALTO, -BASSO, abisso);
      ANNULLA (POSIZIONE, mossa);
      CHECK_HALT ();
      if (v <= valu_migliore)
	continue;
      valu_migliore = v;
      if (v <= BASSO)
	continue;
      BASSO = v;
      mossa_buona = mossa;
      Zobrist_basso (DINAMICO->Zobrist, mossa, 1, v);
      if (v >= ALTO)
	RETURN (v);
    }
  if (mossa_buona)
    {
      Zobrist_preciso (POSIZIONE, mossa_buona, 1, valu_migliore, pietra_precisa);
      RETURN (valu_migliore);
    }
  Zobrist_alto (DINAMICO->Zobrist, 1, valu_migliore);
  RETURN (valu_migliore);
}
