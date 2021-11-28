#include "RobboLito.h"

#include "sorvegliare.h"
#include "mossa_nullo.i"
#include <string.h>

#define LEGALE (POSIZIONE->bianco_en_mossa ? !nero_in_scacco : !bianco_in_scacco)
#define SCACCO (POSIZIONE->bianco_en_mossa ? bianco_in_scacco: nero_in_scacco)

jmp_buf J;

void
uscita_mossa_migliore (tipo_posizione *POSIZIONE)
{
  if (!radice_mossa)
    {
      TRASMISSIONE ("bestmove NULL\n");
      return;
    }
  fare (POSIZIONE, radice_mossa);
  chiamata_valu_piena (0);
#if 0
  TRASMISSIONE ("hash %lld\n", POSIZIONE->DIN->Zobrist); /* %llx ? */
#endif
  annullare (POSIZIONE, radice_mossa);
  TRASMISSIONE ("bestmove %s ponder %s\n", mossa_notazione (radice_mossa, STRING1), mossa_notazione (ponder_move, STRING2));
}

static char *
modificatore (int BASSO, int valu, int ALTO, char *s)
{
  s[0] = 0;
  if (valu <= BASSO)
    strcpy (s, "upperbound");
  else if (valu >= ALTO)
    strcpy (s, "lowerbound");
  else
    strcpy(s, "");
  return s;
}

static char *
cp_mate (int valu, char *s)
{
  if (valu > VALU_MATTO - abisso_massimo)
    sprintf (s, "mate %d", (VALU_MATTO + 1 - valu) / 2);
  else if (valu < -VALU_MATTO + abisso_massimo)
    sprintf (s, "mate %d", (-VALU_MATTO - valu) / 2);
  else
    sprintf (s, "cp %d", valu);
  return s;
}

void
informazione (tipo_posizione *POSIZIONE, uint64 x, int BASSO, int valu, int ALTO)
{
  uint64 t, nps, NODOS = 0;
  int cpu, rp;
  int sd, k, mossa;
  tipo_dinamico *p;
  char pv[256 * 6], *q;
  tipo_zobrist_pv *tr;
  tipo_zobrist *tz;
  uint64 zobrist_mucchio[256];
  int i;
  int cnt = 0;
  int pv_move = 0;
  logico B;
  //YUSUF128 H[1];
#ifdef YUSUF_MULTICORE
  if (TITANIC_MODE)
    for (cpu = 0; cpu < CPUS_SIZE; cpu++)
      for (rp = 0; rp < RP_PER_CPU ; rp++)
	NODOS += ROOT_POSIZIONE[cpu][rp].nodes;
  else
#endif
    NODOS = ROOT_POSIZIONE0->nodes;
#if 1
  for (p = dinamico_inizio + 1; p->Zobrist; p++);
  sd = p - (dinamico_inizio + 1);
  if (sd < 5)
	sd = 0;
#else
  sd = 0;
#endif
  memset (zobrist_mucchio, 0, 256 * sizeof (uint64));
  t = x / 1000;
  if (t == 0)
    nps = 0;
  else
    nps = NODOS / t;
  q = pv;
  zobrist_mucchio[cnt++] = DINAMICO->Zobrist;
  mossa_notazione (radice_mossa, STRING1);
  strcpy (q, STRING1);
  q += strlen (STRING1);
  strcpy (q, " ");
  q++;
  mossa = radice_mossa;
  while (mossa)
    {
      fare (POSIZIONE, mossa);
		pv_move++;
		if (pv_move == 2)
			ponder_move = mossa;
      chiamata_valu_piena (0);
      B = FALSO;
      for (i = 0; i < cnt; i++)
	if (zobrist_mucchio[i] == DINAMICO->Zobrist)
	  B = VERO;
      if (B)
	break;
      zobrist_mucchio[cnt++] = DINAMICO->Zobrist;
      mossa = 0;
      k = DINAMICO->Zobrist & elenco_celare_pv;
      for (i = 0; i < 4; i++)
	{
	  tr = elenco_zobrist_pv + (k + i);
	  HYATT_HASH();
	  if (tr->zobrist == DINAMICO->Zobrist)
	    {
	      mossa = tr->mossa;
	      break;
	    }
	}
#if 1
      if (!mossa)
	{      
	  k = DINAMICO->Zobrist & elenco_celare;
	  for (i = 0; i < 4; i++)
	    {
	      tz = elenco_zobrist + (k + i);
	      HYATT_HASH();
	      if (tz->zobrist == DINAMICO->Zobrist)
		{
		  mossa = tz->mossa;
		  break;
		}
	    }
	}
#endif
      if (!mossa
	  || (POSIZIONE->bianco_en_mossa ?
	      !garantire_bianco (POSIZIONE, mossa) : !garantire_nero (POSIZIONE, mossa)))
	break;
      if (cnt > 250)
	break;
      mossa_notazione (mossa, STRING1);
      strcpy (q, STRING1);
      q += strlen (STRING1);
      strcpy (q, " ");
      q++;
    }
  q--;
  *q = 0;
  while (DINAMICO != (dinamico_inizio + 1))
    {
      if (!DINAMICO->mossa)
	nullo_annulla (POSIZIONE);
      else
	annullare (POSIZIONE, DINAMICO->mossa);
    }
  TRASMISSIONE
    ("info depth %d seldepth %d time %I64u nodes %I64u nps %I64u score %s %s pv %s\n",
     radice_abisso / 2, sd, t, NODOS, nps * 1000, cp_mate (valu, STRING2), modificatore (BASSO, valu, ALTO, STRING3), pv);
}

#include <string.h>
void
cerca (tipo_posizione *POSIZIONE)
{
  int z;
  tipo_dinamico *p, *q;
  tipo_posizione* POS;
  PARTITA_NUOVA = FALSO;
  OROLOGIO = orologio ();
  MOMENTO = clock ();
  mucchio_altezzo = -1;
  radice_mossa = ponder_move = radice_abisso = radice_valu = 0;
  for (p = dinamico_inizio; p <= DINAMICO; p++)
    MUCCHIO[++mucchio_altezzo] = p->Zobrist;
  NODE_CHECK = 0;
  ROOT_POSIZIONE0->nodes = 0;
  if (ANALISI)
    {
      logico RIPETIZIONE;
      for (p = dinamico_inizio; p < DINAMICO; p++)
	{
	  RIPETIZIONE = FALSO;
	  for (q = p + 2; q < DINAMICO; q += 2)
	    if (p->Zobrist == q->Zobrist)
	      {
		RIPETIZIONE = VERO;
		break;
	      }
	  if (!RIPETIZIONE)
	    MUCCHIO[p - dinamico_inizio] = 0;
	  (p + 1)->mossa = 0;
	}
    }
  memcpy (dinamico_inizio + 1, DINAMICO, sizeof (tipo_dinamico));
  memset (dinamico_inizio + 2, 0, 254 * sizeof (tipo_dinamico));
  memset (dinamico_inizio, 0, sizeof (tipo_dinamico));
  DINAMICO = dinamico_inizio + 1;
  POSIZIONE->height = 0;
  anno_incremento ();
#ifdef CON_ROBBO_BUILD
  if (RobboFare (POSIZIONE)) return;
#endif
  radice_precedente = -VALU_MATTO;
  FACILE = FALSO;
  salto_ok = VERO;
  POS = POSIZIONE;
#ifdef YUSUF_MULTICORE
  if (TITANIC_MODE)
    {
      ivan_init_smp();
      POS = &ROOT_POSIZIONE[0][0];
    }
#endif
  z = setjmp (J);
  if (!z)
    {
      if (POS->bianco_en_mossa)
	cerca_alto_bianco (POS);
      else
	cerca_alto_nero (POS);
    }
#ifdef YUSUF_MULTICORE
  if (TITANIC_MODE)
    ivan_end_smp();
#endif
  salto_ok = FALSO;
  abisso_precedente = radice_abisso;
  if (POS == POSIZIONE)
    {
      while (POS->DIN != (POS->din_iniz + 1))
	{
	  if (!POS->DIN->mossa)
	    nullo_annulla (POS);
	  else
	    annullare (POS, POS->DIN->mossa);
	}
    }
 informazione (POSIZIONE, orologio () - OROLOGIO, -32767, radice_valu, 32767);
 uscita_mossa_migliore (POSIZIONE);
}
