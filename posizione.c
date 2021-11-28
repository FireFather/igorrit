#include "RobboLito.h"

static void
fen_posizionale (tipo_posizione *POSIZIONE, char *I)
{
  int tr = 7, co = 0, c = 0, i, p;
  for (i = A1; i <= H8; i++)
    POSIZIONE->qu[i] = 0;
  while (1)
    {
      if (tr < 0 || co > 8)
	ERRORE_fen ("FEN %s col: %d tra: %d\n", I, co, tr);
      p = I[c++];
      if (p == 0)
	return;
      if (co == 8 && p != '/')
	ERRORE_fen ("FEN %s col: %d tra: %d pez: %d\n", I, co, tr, p);
      switch (p)
	{
	case '/':
	  tr--;
	  co = 0;
	  break;
	case 'p':
	  POSIZIONE->qu[co + 8 * tr] = conto_pedone_nero;
	  co++;
	  break;
	case 'b':
	  if (quadretto_fisso[co + 8 * tr] & SCURO)
	    POSIZIONE->qu[co + 8 * tr] = conto_scuro_nero;
	  else
	    POSIZIONE->qu[co + 8 * tr] = conto_chiaro_nero;
	  co++;
	  break;
	case 'n':
	  POSIZIONE->qu[co + 8 * tr] = conto_cavallo_nero;
	  co++;
	  break;
	case 'r':
	  POSIZIONE->qu[co + 8 * tr] = conto_torre_nero;
	  co++;
	  break;
	case 'q':
	  POSIZIONE->qu[co + 8 * tr] = conto_donna_nera;
	  co++;
	  break;
	case 'k':
	  POSIZIONE->qu[co + 8 * tr] = conto_re_nero;
	  co++;
	  break;
	case 'P':
	  POSIZIONE->qu[co + 8 * tr] = conto_pedone_bianco;
	  co++;
	  break;
	case 'B':
	  if (quadretto_fisso[co + 8 * tr] & SCURO)
	    POSIZIONE->qu[co + 8 * tr] = conto_scuro_bianco;
	  else
	    POSIZIONE->qu[co + 8 * tr] = conto_chiaro_bianco;
	  co++;
	  break;
	case 'N':
	  POSIZIONE->qu[co + 8 * tr] = conto_cavallo_bianco;
	  co++;
	  break;
	case 'R':
	  POSIZIONE->qu[co + 8 * tr] = conto_torre_bianco;
	  co++;
	  break;
	case 'Q':
	  POSIZIONE->qu[co + 8 * tr] = conto_donna_bianca;
	  co++;
	  break;
	case 'K':
	  POSIZIONE->qu[co + 8 * tr] = conto_re_bianco;
	  co++;
	  break;
	case '1':
	  co += 1;
	  break;
	case '2':
	  co += 2;
	  break;
	case '3':
	  co += 3;
	  break;
	case '4':
	  co += 4;
	  break;
	case '5':
	  co += 5;
	  break;
	case '6':
	  co += 6;
	  break;
	case '7':
	  co += 7;
	  break;
	case '8':
	  co += 8;
	  break;
	default:
	  ERRORE_fen ("FEN %s col:%d tra:%d pez:%d\n", I, co, tr, p);
	}
      if ((tr == 0) && (co >= 8))
	break;
    }
}

#include <string.h>
char * legge_fen (tipo_posizione *POSIZIONE, char *I)
{
  char i[1024];
  logico ok;
  int en_passant;
  sscanf (I, "%s", i);
  fen_posizionale (POSIZIONE, i);
  memset (dinamico_inizio, 0, 256 * sizeof (tipo_dinamico));
  DINAMICO = dinamico_inizio;
  I += strlen (i) + 1;
  sscanf (I, "%s", i);
  if (i[0] == 'w')
    POSIZIONE->bianco_en_mossa = VERO;
  else if (i[0] == 'b')
    POSIZIONE->bianco_en_mossa = FALSO;
  else
    ERRORE_fen ("FEN wb %s\n", i);
  I += strlen (i) + 1;
  sscanf (I, "%s", i);
  DINAMICO->arrocco = 16;
  if (!strcmp (i, "KQkq"))
    DINAMICO->arrocco = 15;
  if (!strcmp (i, "Qkq"))
    DINAMICO->arrocco = 14;
  if (!strcmp (i, "Kkq"))
    DINAMICO->arrocco = 13;
  if (!strcmp (i, "kq"))
    DINAMICO->arrocco = 12;
  if (!strcmp (i, "KQq"))
    DINAMICO->arrocco = 11;
  if (!strcmp (i, "Qq"))
    DINAMICO->arrocco = 10;
  if (!strcmp (i, "Kq"))
    DINAMICO->arrocco = 9;
  if (!strcmp (i, "q"))
    DINAMICO->arrocco = 8;
  if (!strcmp (i, "KQk"))
    DINAMICO->arrocco = 7;
  if (!strcmp (i, "Qk"))
    DINAMICO->arrocco = 6;
  if (!strcmp (i, "Kk"))
    DINAMICO->arrocco = 5;
  if (!strcmp (i, "k"))
    DINAMICO->arrocco = 4;
  if (!strcmp (i, "KQ"))
    DINAMICO->arrocco = 3;
  if (!strcmp (i, "Q"))
    DINAMICO->arrocco = 2;
  if (!strcmp (i, "K"))
    DINAMICO->arrocco = 1;
  if (!strcmp (i, "-"))
    DINAMICO->arrocco = 0;
  if (DINAMICO->arrocco == 16)
    ERRORE_fen ("FEN oo %s\n", i);
  I += strlen (i) + 1;
  sscanf (I, "%s", i);
  DINAMICO->en_passant = 0;
  if (!strcmp (i, "-"))
    en_passant = 0;
  else
    {
      en_passant = (i[0] - 'a') + 8 * (i[1] - '1');
      if (en_passant > H8)
	ERRORE_fen ("FEN ep %s\n", i);
      ok = 0;
    }
  if (en_passant)
    {
      if (POSIZIONE->bianco_en_mossa)
	{
	  if (COLONNA (en_passant) != CA
	      && (POSIZIONE->qu[en_passant - 9] == conto_pedone_bianco))
	    ok = VERO;
	  if (COLONNA (en_passant) != CH
	      && (POSIZIONE->qu[en_passant - 7] == conto_pedone_bianco))
	    ok = VERO;
	}
      else
	{
	  if (COLONNA (en_passant) != CA
	      && (POSIZIONE->qu[en_passant + 7] == conto_pedone_nero))
	    ok = VERO;
	  if (COLONNA (en_passant) != CH
	      && (POSIZIONE->qu[en_passant + 9] == conto_pedone_nero))
	    ok = VERO;
	}
      if (ok)
	DINAMICO->en_passant = en_passant;
    }
  I += strlen (i) + 1;
  sscanf (I, "%s", i);
  DINAMICO->mossa50 = (uint8) atoi (i);
  I += strlen (i) + 1;
  sscanf (I, "%s", i);
  I += strlen (i) + 1;
  bitbordo_inizializzazione (POSIZIONE);
  return I;
}

static uint32
mossa_completa (tipo_posizione *POSIZIONE, uint32 x)
{
  int pezzo, ai = AI (x), di = DI (x);
  if (!x)
    return x;
  pezzo = POSIZIONE->qu[di];
  if (pezzo == conto_re_bianco || pezzo == conto_re_nero)
    {
      if (ai - di == 2 || di - ai == 2)
	x |= pietra_arrocco;
    }
  if (AI (x) != 0 && AI (x) == DINAMICO->en_passant
      && (pezzo == conto_pedone_bianco || pezzo == conto_pedone_nero))
    x |= pietra_en_passant;
  return x;
}

#define SCACCO\
  ( POSIZIONE->bianco_en_mossa ?\
    ( DINAMICO->attacco_nero & bitbordo_bianco_re) :\
    ( DINAMICO->attacco_bianco & bitbordo_nero_re ) )

static void
legge_mossas (tipo_posizione *POSIZIONE, char *I)
{
  tipo_lista_mossa LISTA[256], *lista;
  char T[256];
  int i;
  uint32 totale;
  while (I[0])
    {
      mobilita (POSIZIONE);
      if (SCACCO)
	{
	  lista = evasione_mossa (POSIZIONE, LISTA, 0xffffffffffffffff);
	  lista++;
	}
      else
	{
	  lista = cattura_mossa (POSIZIONE, LISTA, POSIZIONE->occupato_nero_bianco);
	  lista = ordinaria_mossa (POSIZIONE, lista);
	}
      totale =
	mossa_completa (POSIZIONE, (I[2] - 'a') + ((I[3] - '1') << 3) +
			((I[0] - 'a') << 6) + ((I[1] - '1') << 9));
      sscanf (I, "%s", T);
      if (strlen (T) == 5)
	{
	  if (I[4] == 'b')
	    totale |= pietra_promozione_alfiere;
	  if (I[4] == 'n')
	    totale |= pietra_promozione_cavallo;
	  if (I[4] == 'r')
	    totale |= pietra_promozione_torre;
	  if (I[4] == 'q')
	    totale |= pietra_promozione_donna;
	}
      for (i = 0; i < lista - LISTA; i++)
	{
	  if (totale == (LISTA[i].mossa & 0x7fff))
	    {
	      fare (POSIZIONE, totale);
	      break;
	    }
	}
      if (i == lista - LISTA)
	{
	  ERRORE ("moves? %s\n", T);
	}
      I += strlen (T) + 1;
      while (I[0] == ' ')
	I++;
    }
}

static char posizione_originale[80] =
  "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
void
posizionale_inizializzazione (tipo_posizione *POSIZIONE, char *I)
{
  char i[1024];
  NODE_CHECK = 0;
  sscanf (I, "%s", i);
  if (!strcmp (i, "startpos"))
    {
      legge_fen (POSIZIONE, posizione_originale);
      I += strlen ("startpos") + 1;
    }
  if (!strcmp (i, "fen"))
    {
      I += strlen ("fen") + 1;
      I = legge_fen (POSIZIONE, I);
    }
  if (I[0])
    {
      sscanf (I, "%s", i);
      if (!strcmp (i, "moves"))
	{
	  I += strlen ("moves") + 1;
	  legge_mossas (POSIZIONE, I);
	}
    }
  POSIZIONE->height = 0;
  if (PARTITA_NUOVA)
    guadagno_reset ();
}
