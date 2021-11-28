
#include "RobboLito.h"
#include "pedone_valu.v"

typedef struct
{
  uint64 BORDO, MEZZO, CENTRO;
  uint8 rifugio_bordo[8], rifugio_mezzo[8], rifugio_centro[8];
  uint8 uragano_bordo[8], uragano_mezzo[8], uragano_centro[8],
    RIFdiag[8], zero, valu_zero;
} tipo_rifugio_uragano;

tipo_rifugio_uragano RIF_URA[8];

const uint8 RIFaa[8] = { 30, 0, 5, 15, 20, 25, 25, 25 };
const uint8 RIFab[8] = { 55, 0, 15, 40, 50, 55, 55, 55 };
const uint8 RIFac[8] = { 30, 0, 10, 20, 25, 30, 30, 30 };
const uint8 URAaa[8] = { 5, 0, 35, 15, 5, 0, 0, 0 };
const uint8 URAab[8] = { 10, 0, 50, 20, 10, 0, 0, 0 };
const uint8 URAac[8] = { 10, 0, 50, 20, 10, 0, 0, 0 };

const uint8 RIFba[8] = { 30, 0, 5, 15, 20, 25, 25, 25 };
const uint8 RIFbb[8] = { 55, 0, 15, 40, 50, 55, 55, 55 };
const uint8 RIFbc[8] = { 30, 0, 10, 20, 25, 30, 30, 30 };
const uint8 URAba[8] = { 5, 0, 35, 15, 5, 0, 0, 0 };
const uint8 URAbb[8] = { 10, 0, 50, 20, 10, 0, 0, 0 };
const uint8 URAbc[8] = { 10, 0, 50, 20, 10, 0, 0, 0 };

const uint8 RIFcb[8] = { 30, 0, 5, 15, 20, 25, 25, 25 };
const uint8 RIFcc[8] = { 55, 0, 15, 40, 50, 55, 55, 55 };
const uint8 RIFcd[8] = { 30, 0, 10, 20, 25, 30, 30, 30 };
const uint8 URAcb[8] = { 5, 0, 35, 15, 5, 0, 0, 0 };
const uint8 URAcc[8] = { 10, 0, 50, 20, 10, 0, 0, 0 };
const uint8 URAcd[8] = { 10, 0, 50, 20, 10, 0, 0, 0 };

const uint8 RIFdc[8] = { 30, 0, 5, 15, 20, 25, 25, 25 };
const uint8 RIFdd[8] = { 55, 0, 15, 40, 50, 55, 55, 55 };
const uint8 RIFde[8] = { 30, 0, 10, 20, 25, 30, 30, 30 };
const uint8 URAdc[8] = { 5, 0, 35, 15, 5, 0, 0, 0 };
const uint8 URAdd[8] = { 10, 0, 50, 20, 10, 0, 0, 0 };
const uint8 URAde[8] = { 10, 0, 50, 20, 10, 0, 0, 0 };

const uint8 RIFaDIAG[8] = { 10, 0, 2, 4, 6, 8, 10, 10 };
const uint8 RIFbDIAG[8] = { 8, 0, 2, 4, 6, 7, 8, 8 };
const uint8 RIFcDIAG[8] = { 6, 0, 2, 3, 4, 5, 6, 6 };
const uint8 RIFdDIAG[8] = { 4, 0, 1, 2, 3, 4, 4, 4 };

void
pedone_inizializzazione ()
{
  int co, tr;
  int targa[8] = { CB, CB, CC, CD, CE, CF, CG, CG };
  int cambio[8] = { 1, 1, 1, 1, -1, -1, -1, -1 };
  for (co = CA; co <= CH; co++)
    {
      RIF_URA[co].BORDO = pila_colonna[targa[co] - cambio[co]];
      RIF_URA[co].MEZZO = pila_colonna[targa[co]];
      RIF_URA[co].CENTRO = pila_colonna[targa[co] + cambio[co]];
    }
  for (tr = T1; tr <= T8; tr++)
    {
      RIF_URA[CA].rifugio_bordo[tr] = RIFaa[tr];
      RIF_URA[CA].uragano_bordo[tr] = URAaa[tr];
      RIF_URA[CA].rifugio_mezzo[tr] = RIFab[tr];
      RIF_URA[CA].uragano_mezzo[tr] = URAab[tr];
      RIF_URA[CA].rifugio_centro[tr] = RIFac[tr];
      RIF_URA[CA].uragano_centro[tr] = URAac[tr];
      RIF_URA[CH].rifugio_bordo[tr] = RIFaa[tr];
      RIF_URA[CH].uragano_bordo[tr] = URAaa[tr];
      RIF_URA[CH].rifugio_mezzo[tr] = RIFab[tr];
      RIF_URA[CH].uragano_mezzo[tr] = URAab[tr];
      RIF_URA[CH].rifugio_centro[tr] = RIFac[tr];
      RIF_URA[CH].uragano_centro[tr] = URAac[tr];
      RIF_URA[CA].RIFdiag[tr] = RIFaDIAG[tr];
      RIF_URA[CH].RIFdiag[tr] = RIFaDIAG[tr];
    }
  for (tr = T1; tr <= T8; tr++)
    {
      RIF_URA[CB].rifugio_bordo[tr] = RIFba[tr];
      RIF_URA[CB].uragano_bordo[tr] = URAba[tr];
      RIF_URA[CB].rifugio_mezzo[tr] = RIFbb[tr];
      RIF_URA[CB].uragano_mezzo[tr] = URAbb[tr];
      RIF_URA[CB].rifugio_centro[tr] = RIFbc[tr];
      RIF_URA[CB].uragano_centro[tr] = URAbc[tr];
      RIF_URA[CG].rifugio_bordo[tr] = RIFba[tr];
      RIF_URA[CG].uragano_bordo[tr] = URAba[tr];
      RIF_URA[CG].rifugio_mezzo[tr] = RIFbb[tr];
      RIF_URA[CG].uragano_mezzo[tr] = URAbb[tr];
      RIF_URA[CG].rifugio_centro[tr] = RIFbc[tr];
      RIF_URA[CG].uragano_centro[tr] = URAbc[tr];
      RIF_URA[CB].RIFdiag[tr] = RIFbDIAG[tr];
      RIF_URA[CG].RIFdiag[tr] = RIFbDIAG[tr];
    }
  for (tr = T1; tr <= T8; tr++)
    {
      RIF_URA[CC].rifugio_bordo[tr] = RIFcb[tr];
      RIF_URA[CC].uragano_bordo[tr] = URAcb[tr];
      RIF_URA[CC].rifugio_mezzo[tr] = RIFcc[tr];
      RIF_URA[CC].uragano_mezzo[tr] = URAcc[tr];
      RIF_URA[CC].rifugio_centro[tr] = RIFcd[tr];
      RIF_URA[CC].uragano_centro[tr] = URAcd[tr];
      RIF_URA[CF].rifugio_bordo[tr] = RIFcb[tr];
      RIF_URA[CF].uragano_bordo[tr] = URAcb[tr];
      RIF_URA[CF].rifugio_mezzo[tr] = RIFcc[tr];
      RIF_URA[CF].uragano_mezzo[tr] = URAcc[tr];
      RIF_URA[CF].rifugio_centro[tr] = RIFcd[tr];
      RIF_URA[CF].uragano_centro[tr] = URAcd[tr];
      RIF_URA[CC].RIFdiag[tr] = RIFcDIAG[tr];
      RIF_URA[CF].RIFdiag[tr] = RIFcDIAG[tr];
    }
  for (tr = T1; tr <= T8; tr++)
    {
      RIF_URA[CD].rifugio_bordo[tr] = RIFdc[tr];
      RIF_URA[CD].uragano_bordo[tr] = URAdc[tr];
      RIF_URA[CD].rifugio_mezzo[tr] = RIFdd[tr];
      RIF_URA[CD].uragano_mezzo[tr] = URAdd[tr];
      RIF_URA[CD].rifugio_centro[tr] = RIFde[tr];
      RIF_URA[CD].uragano_centro[tr] = URAde[tr];
      RIF_URA[CE].rifugio_bordo[tr] = RIFdc[tr];
      RIF_URA[CE].uragano_bordo[tr] = URAdc[tr];
      RIF_URA[CE].rifugio_mezzo[tr] = RIFdd[tr];
      RIF_URA[CE].uragano_mezzo[tr] = URAdd[tr];
      RIF_URA[CE].rifugio_centro[tr] = RIFde[tr];
      RIF_URA[CE].uragano_centro[tr] = URAde[tr];
      RIF_URA[CD].RIFdiag[tr] = RIFdDIAG[tr];
      RIF_URA[CE].RIFdiag[tr] = RIFdDIAG[tr];
    }
  for (co = CA; co <= CH; co++)
    {
      RIF_URA[co].zero =
	RIF_URA[co].rifugio_bordo[T2] + RIF_URA[co].rifugio_mezzo[T2] +
	RIF_URA[co].rifugio_centro[T2];
      RIF_URA[co].valu_zero = 10;
    }
}

static int
re_bianco_periglio (tipo_posizione *POSIZIONE, int re_bianco_qu)
{
  int e, traversa_bianca_A, traversa_bianca_B, traversa_bianca_C,
    traversa_nera_A, traversa_nera_B, traversa_nera_C,
    v, tr = TRAVERSA (re_bianco_qu);
  uint64 T, A = bitbordo_bianco_pedone & non_anteriore_nero[tr];
  tipo_rifugio_uragano Z = RIF_URA[COLONNA (re_bianco_qu)];
  T = A & Z.BORDO;
  traversa_bianca_A = BSF (T);
  if (!T)
    traversa_bianca_A = 0;
  traversa_bianca_A >>= 3;
  T = A & Z.MEZZO;
  traversa_bianca_B = BSF (T);
  if (!T)
    traversa_bianca_B = 0;
  traversa_bianca_B >>= 3;
  T = A & Z.CENTRO;
  traversa_bianca_C = BSF (T);
  if (!T)
    traversa_bianca_C = 0;
  traversa_bianca_C >>= 3;
  T = bitbordo_nero_pedone & Z.BORDO;
  traversa_nera_A = BSF (T);
  if (!T)
    traversa_nera_A = 0;
  traversa_nera_A >>= 3;
  T = bitbordo_nero_pedone & Z.MEZZO;
  traversa_nera_B = BSF (T);
  if (!T)
    traversa_nera_B = 0;
  traversa_nera_B >>= 3;
  T = bitbordo_nero_pedone & Z.CENTRO;
  traversa_nera_C = BSF (T);
  if (!T)
    traversa_nera_C = 0;
  traversa_nera_C >>= 3;
  v =
    (Z.rifugio_bordo)[traversa_bianca_A] +
    (Z.rifugio_mezzo)[traversa_bianca_B] +
    (Z.rifugio_centro)[traversa_bianca_C];
  if (v == Z.zero)
    v = Z.valu_zero;
  T = A & diagonale_lungo[re_bianco_qu];
  e = BSF (T);
  if (!T)
    e = 0;
  e >>= 3;
  v += (Z.RIFdiag)[e];
  e = (Z.uragano_bordo)[traversa_nera_A];
  if (traversa_nera_A == (traversa_bianca_A + 1))
    e >>= 1;
  v += e;
  e = (Z.uragano_mezzo)[traversa_nera_B];
  if (traversa_nera_B == (traversa_bianca_B + 1))
    e >>= 1;
  v += e;
  e = (Z.uragano_centro)[traversa_nera_C];
  if (traversa_nera_C == (traversa_bianca_C + 1))
    e >>= 1;
  v += e;
  return v;
}

static int
re_nero_periglio (tipo_posizione *POSIZIONE, int re_nero_qu)
{
  int e, traversa_bianca_A, traversa_bianca_B, traversa_bianca_C,
    traversa_nera_A, traversa_nera_B, traversa_nera_C,
    v, tr = TRAVERSA (re_nero_qu);
  uint64 T, A = bitbordo_nero_pedone & non_anteriore_bianco[tr];
  tipo_rifugio_uragano Z = RIF_URA[COLONNA (re_nero_qu)];
  T = A & Z.BORDO;
  traversa_nera_A = BSR (T);
  if (!T)
    traversa_nera_A = 56;
  traversa_nera_A >>= 3;
  traversa_nera_A = 7 - traversa_nera_A;
  T = A & Z.MEZZO;
  traversa_nera_B = BSR (T);
  if (!T)
    traversa_nera_B = 56;
  traversa_nera_B >>= 3;
  traversa_nera_B = 7 - traversa_nera_B;
  T = A & Z.CENTRO;
  traversa_nera_C = BSR (T);
  if (!T)
    traversa_nera_C = 56;
  traversa_nera_C >>= 3;
  traversa_nera_C = 7 - traversa_nera_C;
  T = bitbordo_bianco_pedone & Z.BORDO;
  traversa_bianca_A = BSR (T);
  if (!T)
    traversa_bianca_A = 56;
  traversa_bianca_A >>= 3;
  traversa_bianca_A = 7 - traversa_bianca_A;
  T = bitbordo_bianco_pedone & Z.MEZZO;
  traversa_bianca_B = BSR (T);
  if (!T)
    traversa_bianca_B = 56;
  traversa_bianca_B >>= 3;
  traversa_bianca_B = 7 - traversa_bianca_B;
  T = bitbordo_bianco_pedone & Z.CENTRO;
  traversa_bianca_C = BSR (T);
  if (!T)
    traversa_bianca_C = 56;
  traversa_bianca_C >>= 3;
  traversa_bianca_C = 7 - traversa_bianca_C;
  v =
    (Z.rifugio_bordo)[traversa_nera_A] + (Z.rifugio_mezzo)[traversa_nera_B] +
    (Z.rifugio_centro)[traversa_nera_C];
  if (v == Z.zero)
    v = Z.valu_zero;
  T = A & diagonale_lungo[re_nero_qu];
  e = BSR (T);
  if (!T)
    e = 56;
  e >>= 3;
  e = 7 - e;
  v += (Z.RIFdiag)[e];
  e = (Z.uragano_bordo)[traversa_bianca_A];
  if (traversa_bianca_A == (traversa_nera_A + 1))
    e >>= 1;
  v += e;
  e = (Z.uragano_mezzo)[traversa_bianca_B];
  if (traversa_bianca_B == (traversa_nera_B + 1))
    e >>= 1;
  v += e;
  e = (Z.uragano_centro)[traversa_bianca_C];
  if (traversa_bianca_C == (traversa_nera_C + 1))
    e >>= 1;
  v += e;
  return v;
}

void
valutazione_pedone (tipo_posizione* POSIZIONE, tipo_pedone_valu * RISU)
{
  int c, valu = 0, B,
    distanza_re_bianco, distanza_re_nero,
    distanza_re_bianco_primo, distanza_re_nero_primo;
  int re_bianco_qu = POSIZIONE->bianco_re_quadretto,
    re_nero_qu = POSIZIONE->nero_re_quadretto;
  int b, tr, co, v, valu_pedone_libero;
  uint64 T, U, V, CONNESSO;
  tipo_pedone_valu *ptr;

  RISU->bianco_pedone_chiaro = RISU->nero_pedone_chiaro =
    RISU->bianco_pedone_scuro = RISU->nero_pedone_scuro = 0;
  RISU->bianco_re_periglio = RISU->nero_re_periglio = 0;
  RISU->bianco_pedone_libero_linea = RISU->nero_pedone_libero_linea = 0;
  distanza_re_nero_primo = distanza_re_bianco_primo = 30000;
  CONNESSO = 0;

  c = 0;
  for (co = CA; co <= CH; co++)
    {
      if ((bitbordo_bianco_pedone & pila_colonna[co]) == 0)
	c = 0;
      else
	{
	  if (c == 0)
	    {
	      valu -= isola;
	    }
	  c = 1;
	}
    }

  T = bitbordo_bianco_pedone;
  while (T)
    {
      b = BSF (T);
      bitLIBERO (b, T);
      tr = TRAVERSA (b);
      co = COLONNA (b);

      distanza_re_bianco = distanza_re_pedone_bianco (b, re_bianco_qu);
      if (distanza_re_bianco < distanza_re_bianco_primo)
	distanza_re_bianco_primo = distanza_re_bianco;
      distanza_re_nero = distanza_re_pedone_bianco (b, re_nero_qu);
      if (distanza_re_nero < distanza_re_nero_primo)
	distanza_re_nero_primo = distanza_re_nero;

      if (quadretto_fisso[b] & CHIARO)
	{
	  RISU->bianco_pedone_chiaro += pedone_bloccato_valu[b];
	  if (POSIZIONE->qu[b + 8] == conto_pedone_nero)
	    RISU->bianco_pedone_chiaro += pedone_bloccato_valu[b];
	}
      else
	{
	  RISU->bianco_pedone_scuro += pedone_bloccato_valu[b];
	  if (POSIZIONE->qu[b + 8] == conto_pedone_nero)
	    RISU->bianco_pedone_scuro += pedone_bloccato_valu[b];
	}

      if (bitbordo_bianco_pedone & ovest2[b]
	  && (bitbordo_bianco_pedone & anteriore_bianco[tr - 1] &
	      pila_colonna[co - 1]) == 0)
	{
	  valu -= foro;
	}

      if ((bitbordo_bianco_pedone | bitbordo_nero_pedone) &
	  colonna_aperta_bianca[b])
	{
	  if (bitbordo_bianco_pedone & DOPPIO[b])
	    {
	      valu -= doppio_chiuso;
	      if ((bitbordo_bianco_pedone & colonna_isolata[co]) == 0)
		{
		  valu -= doppio_chiuso_isolato;
		}
	    }
	  if ((bitbordo_bianco_pedone & colonna_isolata[co]) == 0)
	    {
	      valu -= isolato_chiuso;
	      continue;
	    }
	  if ((bitbordo_bianco_pedone & pedone_protetto_bianco[b]) == 0)
	    {
	      B = b + 8;
	      if ((bitbordo_bianco_pedone & attaco_pedone_nero[b]) == 0)
		{
		  B += 8;
		  if ((bitbordo_bianco_pedone & attaco_pedone_nero[b + 8]) == 0)
		    B += 8;
		}
	      if (bitbordo_nero_pedone & attaco_pedone_nero[B])
		{
		  valu -= ritardato_chiuso;
		}
	    }
	  continue;
	}

      if (bitbordo_bianco_pedone & DOPPIO[b])
	{
	  valu -= doppio_aperto;
	  if ((bitbordo_bianco_pedone & colonna_isolata[co]) == 0)
	    {
	      valu -= doppio_aperto_isolato;
	    }
	}
      if ((bitbordo_bianco_pedone & colonna_isolata[co]) == 0)
	{
	  valu -= isolato_aperto;
	}
      else
	{
	  if ((bitbordo_bianco_pedone & pedone_protetto_bianco[b]) == 0)
	    {
	      B = b + 8;
	      if ((bitbordo_bianco_pedone & attaco_pedone_nero[b]) == 0)
		{
		  B += 8;
		  if ((bitbordo_bianco_pedone & attaco_pedone_nero[b + 8]) == 0)
		    B += 8;
		}
	      if (bitbordo_nero_pedone & attaco_pedone_nero[B])
		{
		  valu -= ritardato_aperto;
		}
	    }
	}

      if ((bitbordo_nero_pedone & pedone_libero_bianco[b]) == 0)
	goto LIBERO_BIANCO;
      if (bitbordo_nero_pedone & pedone_libero_bianco[b] &
	  ~attaco_pedone_nero[b])
	{
	  valu += pedone_candidato_valu[tr];
	  continue;
	}
      if (POPCNT (attaco_pedone_nero[b] & bitbordo_nero_pedone) >
	  POPCNT (attaco_pedone_bianco[b] & bitbordo_bianco_pedone))
	{
	  valu += pedone_candidato_valu[tr];
	  continue;
	}
    LIBERO_BIANCO:
      valu_pedone_libero = pedone_libero_valu[tr];
      if (bitbordo_bianco_pedone & attaco_pedone_bianco[b])
	valu_pedone_libero += pedone_libero_valu_protetto[tr];
      if ((bitbordo_nero_pedone & colonnas_sinistras[co]) == 0
	  || (bitbordo_nero_pedone & colonnas_rettas[co]) == 0)
	valu_pedone_libero += pedone_libero_valu_ampio[tr];

      V = pedone_connesso[b] & CONNESSO;
      CONNESSO |= quadretto_fisso[b];
      if (V)
	{
	  valu_pedone_libero +=
	    pedone_libero_valu_connesso[tr] +
	    pedone_libero_valu_connesso[TRAVERSA (BSF (V))];
	  bitLIBERO (0, V);
	  if (V)
	    valu_pedone_libero +=
	      pedone_libero_valu_connesso[tr] +
	      pedone_libero_valu_connesso[TRAVERSA (BSF (V))];
	}
      valu += valu_pedone_libero;
      RISU->bianco_pedone_libero_linea |= (uint8) (1 << co);
      if (b <= H3)
	{
	  continue;
	}
      valu +=
	(distanza_re_pedone_bianco (b + 8, re_nero_qu) *
	 mitu_re_pedone_distanza[TRAVERSA (b)]);
      valu -=
	(distanza_re_pedone_bianco (b + 8, re_bianco_qu) *
	 mio_re_pedone_distanza[TRAVERSA (b)]);
    }

  c = 0;
  for (co = CA; co <= CH; co++)
    {
      if ((bitbordo_nero_pedone & pila_colonna[co]) == 0)
	c = 0;
      else
	{
	  if (c == 0)
	    {
	      valu += isola;
	    }
	  c = 1;
	}
    }

  CONNESSO = 0;
  T = bitbordo_nero_pedone;
  while (T)
    {
      b = BSF (T);
      bitLIBERO (b, T);
      tr = TRAVERSA (b);
      co = COLONNA (b);

      distanza_re_nero = distanza_re_pedone_nero (b, re_nero_qu);
      if (distanza_re_nero < distanza_re_nero_primo)
	distanza_re_nero_primo = distanza_re_nero;
      distanza_re_bianco = distanza_re_pedone_nero (b, re_bianco_qu);
      if (distanza_re_bianco < distanza_re_bianco_primo)
	distanza_re_bianco_primo = distanza_re_bianco;

      if (quadretto_fisso[b] & CHIARO)
	{
	  RISU->nero_pedone_chiaro += pedone_bloccato_valu[b];
	  if (POSIZIONE->qu[b - 8] == conto_pedone_bianco)
	    RISU->nero_pedone_chiaro += pedone_bloccato_valu[b];
	}
      else
	{
	  RISU->nero_pedone_scuro += pedone_bloccato_valu[b];
	  if (POSIZIONE->qu[b - 8] == conto_pedone_bianco)
	    RISU->nero_pedone_scuro += pedone_bloccato_valu[b];
	}

      if (bitbordo_nero_pedone & ovest2[b]
	  && (bitbordo_nero_pedone & anteriore_nero[tr + 1] &
	      pila_colonna[co - 1]) == 0)
	{
	  valu += foro;
	}

      if ((bitbordo_bianco_pedone | bitbordo_nero_pedone) &
	  colonna_aperta_nera[b])
	{
	  if (bitbordo_nero_pedone & DOPPIO[b])
	    {
	      valu += doppio_chiuso;
	      if ((bitbordo_nero_pedone & colonna_isolata[co]) == 0)
		{
		  valu += doppio_chiuso_isolato;
		}
	    }
	  if ((bitbordo_nero_pedone & colonna_isolata[co]) == 0)
	    {
	      valu += isolato_chiuso;
	      continue;
	    }
	  if ((bitbordo_nero_pedone & pedone_protetto_nero[b]) == 0)
	    {
	      B = b - 8;
	      if ((bitbordo_nero_pedone & attaco_pedone_bianco[b]) == 0)
		{
		  B -= 8;
		  if ((bitbordo_nero_pedone & attaco_pedone_bianco[b - 8]) ==
		      0)
		    B -= 8;
		}
	      if (bitbordo_bianco_pedone & attaco_pedone_bianco[B])
		{
		  valu += ritardato_chiuso;
		}
	    }
	  continue;
	}

      if (bitbordo_nero_pedone & DOPPIO[b])
	{
	  valu += doppio_aperto;
	  if ((bitbordo_nero_pedone & colonna_isolata[co]) == 0)
	    {
	      valu += doppio_aperto_isolato;
	    }
	}
      if ((bitbordo_nero_pedone & colonna_isolata[co]) == 0)
	{
	  valu += isolato_aperto;
	}
      else
	{
	  if ((bitbordo_nero_pedone & pedone_protetto_nero[b]) == 0)
	    {
	      B = b - 8;
	      if ((bitbordo_nero_pedone & attaco_pedone_bianco[b]) == 0)
		{
		  B -= 8;
		  if ((bitbordo_nero_pedone & attaco_pedone_bianco[b - 8]) == 0)
		    B -= 8;
		}
	      if (bitbordo_bianco_pedone & attaco_pedone_bianco[B])
		{
		  valu += ritardato_aperto;
		}
	    }
	}

      if ((bitbordo_bianco_pedone & pedone_libero_nero[b]) == 0)
	goto LIBERO_NERO;
      if (bitbordo_bianco_pedone & pedone_libero_nero[b] &
	  ~attaco_pedone_bianco[b])
	{
	  valu -= pedone_candidato_valu[7 - tr];
	  continue;
	}
      if (POPCNT (attaco_pedone_bianco[b] & bitbordo_bianco_pedone) >
	  POPCNT (attaco_pedone_nero[b] & bitbordo_nero_pedone))
	{
	  valu -= pedone_candidato_valu[7 - tr];
	  continue;
	}
    LIBERO_NERO:
      valu_pedone_libero = pedone_libero_valu[7 - tr];
      if (bitbordo_nero_pedone & attaco_pedone_nero[b])
	valu_pedone_libero += pedone_libero_valu_protetto[7 - tr];
      if ((bitbordo_bianco_pedone & colonnas_sinistras[co]) == 0
	  || (bitbordo_bianco_pedone & colonnas_rettas[co]) == 0)
	valu_pedone_libero += pedone_libero_valu_ampio[7 - tr];

      V = pedone_connesso[b] & CONNESSO;
      CONNESSO |= quadretto_fisso[b];
      if (V)
	{
	  valu_pedone_libero +=
	    pedone_libero_valu_connesso[7 - tr] +
	    pedone_libero_valu_connesso[7 - (BSF (V) >> 3)];
	  bitLIBERO (0, V);
	  if (V)
	    valu_pedone_libero +=
	      pedone_libero_valu_connesso[7 - tr] +
	      pedone_libero_valu_connesso[7 - (BSF (V) >> 3)];
	}

      valu -= valu_pedone_libero;
      RISU->nero_pedone_libero_linea |= (uint8) (1 << co);
      if (b >= A6)
	{
	  continue;
	}
      valu -=
	(distanza_re_pedone_nero (b - 8, re_bianco_qu) *
	 mitu_re_pedone_distanza[T8 - TRAVERSA (b)]);
      valu +=
	(distanza_re_pedone_nero (b - 8, re_nero_qu) *
	 mio_re_pedone_distanza[T8 - TRAVERSA (b)]);
    }


  T = 0;
  for (tr = T2; tr <= T7; tr++)
    T |= ((bitbordo_bianco_pedone >> (8 * tr)) & 0xff);
  U = 0;
  for (tr = T2; tr <= T7; tr++)
    U |= ((bitbordo_nero_pedone >> (8 * tr)) & 0xff);
  RISU->bianco_pedone_linea_conteggio = POPCNT (T);
  RISU->nero_pedone_linea_conteggio = POPCNT (U);
  RISU->aperto_colonna_conteggio = 8 - POPCNT (T | U);
  RISU->bianco_patta_numero =
    opposto_moltiplicatore[POPCNT (T & ~U)] *
    conto_moltiplicatore[RISU->bianco_pedone_linea_conteggio];
  RISU->nero_patta_numero =
    opposto_moltiplicatore[POPCNT (U & ~T)] *
    conto_moltiplicatore[RISU->nero_pedone_linea_conteggio];
  if (bitbordo_bianco_pedone | bitbordo_nero_pedone)
    {
      valu += distanza_re_nero_primo - distanza_re_bianco_primo;
    }

  T =
    ((bitbordo_nero_pedone & (~COLONNAa)) >> 9) |
    ((bitbordo_nero_pedone & (~COLONNAh)) >> 7);
  if ((~T) & attaco_re[re_bianco_qu] & bitbordo_nero_pedone)
    {
      valu += re_attacco_pedone;
    }
  if (DINAMICO->arrocco & 1)
    {
      valu += re_oo;
    }
  if (DINAMICO->arrocco & 2)
    {
      valu += re_ooo;
    }

  T =
    ((bitbordo_bianco_pedone & (~COLONNAa)) << 7) |
    ((bitbordo_bianco_pedone & (~COLONNAh)) << 9);
  if ((~T) & attaco_re[re_nero_qu] & bitbordo_bianco_pedone)
    {
      valu -= re_attacco_pedone;
    }
  if (DINAMICO->arrocco & 4)
    {
      valu -= re_oo;
    }
  if (DINAMICO->arrocco & 8)
    {
      valu -= re_ooo;
    }

  RISU->puntos = valu;
  v = re_bianco_periglio (POSIZIONE, re_bianco_qu);
  if (arrocco_bianco_oo)
    v = MINIMO (v, 5 + re_bianco_periglio (POSIZIONE, G1));
  if (arrocco_bianco_ooo)
    v = MINIMO (v, 5 + re_bianco_periglio (POSIZIONE, C1));
  RISU->bianco_re_periglio = puntos (v, 0);
  v = re_nero_periglio (POSIZIONE, re_nero_qu);
  if (arrocco_nero_oo)
    v = MINIMO (v, 5 + re_nero_periglio (POSIZIONE, G8));
  if (arrocco_nero_ooo)
    v = MINIMO (v, 5 + re_nero_periglio (POSIZIONE, C8));
  RISU->nero_re_periglio = puntos (v, 0);
  RISU->ZobristPedone = DINAMICO->ZobristPedone;
#if 1
  RISU->ZobristPedone ^= (((uint64*) (RISU)) + 0x1)[0];
  RISU->ZobristPedone ^= (((uint64*) (RISU)) + 0x2)[0];
  RISU->ZobristPedone ^= (((uint64*) (RISU)) + 0x3)[0];
#endif
  ptr = pedone_zobrist + (POSIZIONE->DIN->ZobristPedone & (elenco_pedone_numero - 1));
  memcpy (ptr, RISU, sizeof(tipo_pedone_valu));
#if 1
  RISU->ZobristPedone ^= (((uint64*) (RISU)) + 0x1)[0];
  RISU->ZobristPedone ^= (((uint64*) (RISU)) + 0x2)[0];
  RISU->ZobristPedone ^= (((uint64*) (RISU)) + 0x3)[0];
#endif
  return;
}
