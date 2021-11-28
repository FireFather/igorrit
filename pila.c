
#include "RobboLito.h"
#ifdef MERSENNE_RAND
#include "mersenne.h"
#endif 

static int sinistra54[64], retto54[64], sinistra09[64];
static int TURNO[64] =
  { 1, 2, 2, 4, 4, 4, 7, 7, 7, 7, 11, 11, 11, 11, 11, 16, 16, 16, 16, 16, 16,
  22, 22, 22, 22, 22, 22, 22, 29, 29, 29, 29, 29, 29, 29, 29,
  37, 37, 37, 37, 37, 37, 37, 44, 44, 44, 44, 44, 44, 50, 50, 50, 50, 50,
  55, 55, 55, 55, 59, 59, 59, 62, 62, 64
};

static int LUNGHEZZA[64], DOVE[64];
static int SALTO[8] = { 6, 10, 15, 17, -6, -10, -15, -17 };

static uint32 randkey = 5489ul;
static uint64 rand0 = 1;
uint16 RAND16()
    {
    rand0 = rand0 * 8765432181103515245 + 1234567891;
    return ((rand0 >> 32) % 65536);
    }

#ifdef MERSENNE_RAND
#define RAND64() mt_llrand()
#define SRAND(rndkey) mt_seed32(rndkey)
#else
static uint64 RAND64()
    {
    return (((uint64)RAND16()) << 48) | (((uint64)RAND16()) << 32) | (((uint64)RAND16()) << 16)
        | (((uint64)RAND16()) << 0);
    }
#endif

void
zob_iniz ()
{
  int i, j;
#ifdef MERSENNE_RAND
	SRAND(randkey);
#endif
  zobrist_mossa_bianca = RAND64 ();
  zobrist_oo[0] = 0;
  zobrist_oo[1] = RAND64 ();
  zobrist_oo[2] = RAND64 ();
  zobrist_oo[4] = RAND64 ();
  zobrist_oo[8] = RAND64 ();
  for (i = 0; i < 16; i++)
    {
      if (POPCNT (i) < 2)
	continue;
      zobrist_oo[i] = 0;
      for (j = 1; j < 16; j <<= 1)
	if (i & j)
	  zobrist_oo[i] ^= zobrist_oo[j];
    }
  for (i = 0; i < 16; i++)
    for (j = A1; j <= H8; j++)
      ZOBRIST[i][j] = RAND64 ();
  for (i = CA; i <= CH; i++)
    zobrist_ep[i] = RAND64 ();
}

void
pila_inizializzazione ()
{
  int qu2, l, w, i, qu = 0, j, u, co, tr, re, dir;
  uint64 T, b, s;
  for (i = A1; i <= H8; i++)
    {
      turno_sinistro45[i] = TURNO[sinistro45[i]];
      turno_retto45[i] = TURNO[retto45[i]];
    }
  for (i = A1; i <= H8; i++)
    {
      turno_normale[i] = 1 + (i & 56);
      turno_sinistro90[i] = 1 + (sinistro90[i] & 56);
    }
  for (i = 1; i <= 8; i++)
    for (j = 1; j <= i; j++)
      {
	LUNGHEZZA[qu] = i;
	DOVE[qu++] = j - 1;
      }
  for (i = 7; i >= 1; i--)
    for (j = 1; j <= i; j++)
      {
	LUNGHEZZA[qu] = i;
	DOVE[qu++] = j - 1;
      }
  for (i = A1; i <= H8; i++)
    {
      sinistra54[sinistro45[i]] = i;
      sinistra09[sinistro90[i]] = i;
      retto54[retto45[i]] = i;
    }
  for (i = A1; i <= H8; i++)
    {
      quadretto_fisso[i] = 0;
      bitFISSO (i, quadretto_fisso[i]);
      quadretto_libero[i] = ~quadretto_fisso[i];
    }

  for (i = A1; i <= H8; i++)
    {
      sinistro90_fisso[i] = 0;
      bitFISSO (sinistro90[i], sinistro90_fisso[i]);
      sinistro90_reset[i] = ~sinistro90_fisso[i];
      sinistro45_fisso[i] = 0;
      bitFISSO (sinistro45[i], sinistro45_fisso[i]);
      sinistro45_reset[i] = ~sinistro45_fisso[i];
      retto45_fisso[i] = 0;
      bitFISSO (retto45[i], retto45_fisso[i]);
      retto45_reset[i] = ~retto45_fisso[i];
    }

  for (i = A1; i <= H8; i++)
    {
      attaco_cavallo[i] = 0;
      for (j = 0; j < 8; j++)
	{
	  qu = i + SALTO[j];
	  if ((qu < A1) || (qu > H8))
	    continue;
	  if ((colonna_distacco (i, qu) > 2)
	      || (traversa_distacco (i, qu) > 2))
	    continue;
	  bitFISSO (qu, attaco_cavallo[i]);
	}
    }

  for (i = A1; i <= H8; i++)
    {
      attaco_re[i] = 0;
      for (j = A1; j <= H8; j++)
	{
	  if (MASSIMO (colonna_distacco (i, j), traversa_distacco (i, j)) ==
	      1)
	    bitFISSO (j, attaco_re[i]);
	}
    }

  for (i = A1; i <= H1; i++)
    {
      attaco_pedone_bianco[i] = 0;
      attaco_pedone_nero[i] = quadretto_fisso[i + 7] | quadretto_fisso[i + 9];
    }
  for (i = A2; i <= H7; i++)
    {
      attaco_pedone_bianco[i] =
	quadretto_fisso[i - 7] | quadretto_fisso[i - 9];
      attaco_pedone_nero[i] = quadretto_fisso[i + 7] | quadretto_fisso[i + 9];
    }
  for (i = A8; i <= H8; i++)
    {
      attaco_pedone_nero[i] = 0;
      attaco_pedone_bianco[i] =
	quadretto_fisso[i - 7] | quadretto_fisso[i - 9];
    }
  for (i = A1; i <= A8; i += 8)
    {
      attaco_pedone_bianco[i] = quadretto_fisso[i - 7];
      attaco_pedone_nero[i] = quadretto_fisso[i + 9];
    }
  for (i = H1; i <= H8; i += 8)
    {
      attaco_pedone_bianco[i] = quadretto_fisso[i - 9];
      attaco_pedone_nero[i] = quadretto_fisso[i + 7];
    }
  attaco_pedone_bianco[A1] = 0;
  attaco_pedone_bianco[A2] = quadretto_fisso[B1];
  attaco_pedone_nero[A7] = quadretto_fisso[B8];
  attaco_pedone_nero[A8] = 0;
  attaco_pedone_bianco[H1] = 0;
  attaco_pedone_bianco[H2] = quadretto_fisso[G1];
  attaco_pedone_nero[H7] = quadretto_fisso[G8];
  attaco_pedone_nero[H8] = 0;

  colonna_isolata[CA] = COLONNAb;
  colonna_isolata[CH] = COLONNAg;
  for (co = CB; co <= CG; co++)
    colonna_isolata[co] = pila_colonna[co - 1] | pila_colonna[co + 1];
  for (qu = A1; qu <= H8; qu++)
    {
      pila_isolato2_bianco[qu] = 0;
      pila_isolato2_nero[qu] = 0;
      co = COLONNA (qu);
      tr = TRAVERSA (qu);
      if (tr < T8)
	pila_isolato2_bianco[qu] |=
	  colonna_isolata[co] & pila_traversa[tr + 1];
      if (tr < T7)
	pila_isolato2_bianco[qu] |=
	  colonna_isolata[co] & pila_traversa[tr + 2];
      if (tr > T1)
	pila_isolato2_nero[qu] |= colonna_isolata[co] & pila_traversa[tr - 1];
      if (tr > T2)
	pila_isolato2_nero[qu] |= colonna_isolata[co] & pila_traversa[tr - 2];
      pedone_connesso[qu] =
	pila_isolato2_bianco[qu] | pila_isolato2_nero[qu] |
	(pila_traversa[tr] & colonna_isolata [co]);
    }
  for (tr = T1; tr <= T8; tr++)
    {
      anteriore_bianco[tr] = 0;
      for (j = tr + 1; j <= T8; j++)
	anteriore_bianco[tr] |= pila_traversa[j];
      non_anteriore_bianco[tr] = ~anteriore_bianco[tr];
    }
  for (tr = T8; tr >= T1; tr--)
    {
      anteriore_nero[tr] = 0;
      for (j = tr - 1; j >= T1; j--)
	anteriore_nero[tr] |= pila_traversa[j];
      non_anteriore_nero[tr] = ~anteriore_nero[tr];
    }
  for (u = 0; u < 128; u += 2)
    for (co = CA; co <= CH; co++)
      {
	T = 0;
	if (co < 7)
	  {
	    s = 1 << (co + 1);
	    while (s < 256)
	      {
		T |= s;
		if (u & s)
		  break;
		s <<= 1;
	      }
	  }
	if (co > 0)
	  {
	    s = 1 << (co - 1);
	    while (s > 0)
	      {
		T |= s;
		if (u & s)
		  break;
		s >>= 1;
	      }
	  }
	for (i = 0; i < 8; i++)
	  attaco_normale[co + 8 * i][u >> 1] = T << (8 * i);
      }

  for (qu = A1; qu <= H8; qu++)
    {
      pedone_libero_bianco[qu] =
	(colonna_isolata[COLONNA (qu)] | pila_colonna[COLONNA (qu)]) &
	anteriore_bianco[TRAVERSA (qu)];
      pedone_libero_nero[qu] =
	(colonna_isolata[COLONNA (qu)] | pila_colonna[COLONNA (qu)]) &
	anteriore_nero[TRAVERSA (qu)];
    }

  for (qu = A1; qu <= H8; qu++)
    {
      if (COLONNA (qu) >= CC)
	ovest2[qu] = quadretto_fisso[qu - 2];
      else
	ovest2[qu] = 0;
      if (COLONNA (qu) <= CF)
	est2[qu] = quadretto_fisso[qu + 2];
      else
	est2[qu] = 0;
      if (COLONNA (qu) >= CB)
	ovest1[qu] = quadretto_fisso[qu - 1];
      else
	ovest1[qu] = 0;
      if (COLONNA (qu) <= CG)
	est1[qu] = quadretto_fisso[qu + 1];
      else
	est1[qu] = 0;
      adiacente[qu] = ovest1[qu] | est1[qu];
    }

  for (qu = A1; qu <= H8; qu++)
    {
      pedone_protetto_bianco[qu] =
	(colonna_isolata[COLONNA (qu)]) & non_anteriore_bianco[TRAVERSA (qu)];
      pedone_protetto_nero[qu] =
	(colonna_isolata[COLONNA (qu)]) & non_anteriore_nero[TRAVERSA (qu)];
    }

  for (qu = A1; qu <= H8; qu++)
    {
      co = COLONNA (qu);
      tr = TRAVERSA (qu);
      diagonale_lungo[qu] = 0;
      if (co <= CD)
	{
	  while (co < CH && tr < T8)
	    {
	      co++;
	      tr++;
	      diagonale_lungo[qu] |= quadretto_fisso[8 * tr + co];
	    }
	  co = COLONNA (qu);
	  tr = TRAVERSA (qu);
	  while (co < CH && tr > T1)
	    {
	      co++;
	      tr--;
	      diagonale_lungo[qu] |= quadretto_fisso[8 * tr + co];
	    }
	}
      else
	{
	  while (co > CA && tr < T8)
	    {
	      co--;
	      tr++;
	      diagonale_lungo[qu] |= quadretto_fisso[8 * tr + co];
	    }
	  co = COLONNA (qu);
	  tr = TRAVERSA (qu);
	  while (co > CA && tr > T1)
	    {
	      co--;
	      tr--;
	      diagonale_lungo[qu] |= quadretto_fisso[8 * tr + co];
	    }
	}
    }

  for (qu = A1; qu <= H8; qu++)
    colonna_aperta_bianca[qu] =
      pila_colonna[COLONNA (qu)] & anteriore_bianco[TRAVERSA (qu)];
  for (qu = A1; qu <= H8; qu++)
    colonna_aperta_nera[qu] =
      pila_colonna[COLONNA (qu)] & anteriore_nero[TRAVERSA (qu)];
  for (qu = A1; qu <= H8; qu++)
    DOPPIO[qu] = pila_colonna[COLONNA (qu)] ^ (((uint64) 1) << qu);

  for (qu = A1; qu <= H8; qu++)
    for (i = 0; i < 64; i++)
      {
	T = attaco_normale[sinistro90[qu]][i];
	attaco_sinistro90[qu][i] = 0;
	while (T)
	  {
	    b = BSF (T);
	    attaco_sinistro90[qu][i] |= quadretto_fisso[sinistra09[b]];
	    bitLIBERO (b, T);
	  }
      }

  for (u = 0; u < 128; u += 2)
    for (qu = A1; qu <= H8; qu++)
      {
	T = 0;
	l = LUNGHEZZA[qu];
	w = DOVE[qu];
	attaco_retto45[retto54[qu]][u >> 1] = 0;
	if (w < l)
	  {
	    s = 1 << (w + 1);
	    while (s < (1 << l))
	      {
		T |= s;
		if (u & s)
		  break;
		s <<= 1;
	      }
	  }
	if (w > 0)
	  {
	    s = 1 << (w - 1);
	    while (s > 0)
	      {
		T |= s;
		if (u & s)
		  break;
		s >>= 1;
	      }
	  }
	T <<= (qu - w);
	while (T)
	  {
	    b = BSF (T);
	    attaco_retto45[retto54[qu]][u >> 1] |=
	      quadretto_fisso[retto54[b]];
	    bitLIBERO (b, T);
	  }
      }

  for (u = 0; u < 128; u += 2)
    for (qu = A1; qu <= H8; qu++)
      {
	T = 0;
	l = LUNGHEZZA[qu];
	w = DOVE[qu];
	attaco_sinistro45[sinistra54[qu]][u >> 1] = 0;
	if (w < l)
	  {
	    s = 1 << (w + 1);
	    while (s < (1 << l))
	      {
		T |= s;
		if (u & s)
		  break;
		s <<= 1;
	      }
	  }
	if (w > 0)
	  {
	    s = 1 << (w - 1);
	    while (s > 0)
	      {
		T |= s;
		if (u & s)
		  break;
		s >>= 1;
	      }
	  }
	T <<= (qu - w);
	while (T)
	  {
	    b = BSF (T);
	    attaco_sinistro45[sinistra54[qu]][u >> 1] |=
	      quadretto_fisso[sinistra54[b]];
	    bitLIBERO (b, T);
	  }
      }

#define DISTANZA(i, j)\
 ( MASSIMO (colonna_distacco (i, j), traversa_distacco (i, j)) )
  for (qu = A1; qu <= H8; qu++)
    {
      quadrante_nero_mossa_bianca[qu] = quadrante_nero_mossa_nera[qu] = 0;
      j = (qu & 7) + 56;
      if (TRAVERSA (qu) == T2)
	qu2 = qu + 8;
      else
	qu2 = qu;
      for (i = A1; i <= H8; i++)
	{
	  if (DISTANZA (qu2, j) < DISTANZA (j, i) - 1)
	    bitFISSO (i, quadrante_nero_mossa_nera[qu]);
	  if (DISTANZA (qu2, j) < DISTANZA (j, i))
	    bitFISSO (i, quadrante_nero_mossa_bianca[qu]);
	}
    }
  for (qu = A1; qu <= H8; qu++)
    {
      quadrante_bianco_mossa_bianca[qu] = quadrante_bianco_mossa_nera[qu] = 0;
      j = (qu & 7);
      if (TRAVERSA (qu) == T7)
	qu2 = qu - 8;
      else
	qu2 = qu;
      for (i = A1; i <= H8; i++)
	{
	  if (DISTANZA (qu2, j) < DISTANZA (j, i) - 1)
	    bitFISSO (i, quadrante_bianco_mossa_bianca[qu]);
	  if (DISTANZA (qu2, j) < DISTANZA (j, i))
	    bitFISSO (i, quadrante_bianco_mossa_nera[qu]);
	}
    }

  for (qu = A1; qu <= H8; qu++)
    {
      bianco_guido[qu] = nero_guido[qu] = 0;
      co = COLONNA (qu);
      if (co == CA || co == CH)
	T = colonna_isolata[co];
      else
	T = colonna_isolata[co] | pila_colonna[co];
      if (TRAVERSA (qu) >= T6)
	bianco_guido[qu] |= (T & TRAVERSA8);
      if (TRAVERSA (qu) >= T5)
	bianco_guido[qu] |= (T & TRAVERSA7);
      if (TRAVERSA (qu) <= T3)
	nero_guido[qu] |= (T & TRAVERSA1);
      if (TRAVERSA (qu) <= T4)
	nero_guido[qu] |= (T & TRAVERSA2);
    }

  for (qu = A1; qu <= H8; qu++)
    {
      nord_ovest[qu] = (TRAVERSA (qu) != T8
			&& COLONNA (qu) != CA) ? quadretto_fisso[qu + 7] : 0;
      nord_est[qu] = (TRAVERSA (qu) != T8
		      && COLONNA (qu) != CH) ? quadretto_fisso[qu + 9] : 0;
      sud_ovest[qu] = (TRAVERSA (qu) != T1
		       && COLONNA (qu) != CA) ? quadretto_fisso[qu - 9] : 0;
      sud_est[qu] = (TRAVERSA (qu) != T1
		     && COLONNA (qu) != CH) ? quadretto_fisso[qu - 7] : 0;
    }

  for (qu = A1; qu <= H8; qu++)
    for (re = A1; re <= H8; re++)
      {
	EVASIONE[re][qu] = attaco_re[re];
	if (TRAVERSA (re) == TRAVERSA (qu))
	  {
	    if (COLONNA (re) != CA)
	      EVASIONE[re][qu] ^= quadretto_fisso[re - 1];
	    if (COLONNA (re) != CH)
	      EVASIONE[re][qu] ^= quadretto_fisso[re + 1];
	  }
	if (COLONNA (re) == COLONNA (qu))
	  {
	    if (TRAVERSA (re) != T1)
	      EVASIONE[re][qu] ^= quadretto_fisso[re - 8];
	    if (TRAVERSA (re) != T8)
	      EVASIONE[re][qu] ^= quadretto_fisso[re + 8];
	  }
	if ((TRAVERSA (re) - TRAVERSA (qu)) == (COLONNA (re) - COLONNA (qu)))
	  {
	    if (TRAVERSA (re) != T8 && COLONNA (re) != CH)
	      EVASIONE[re][qu] ^= quadretto_fisso[re + 9];
	    if (TRAVERSA (re) != T1 && COLONNA (re) != CA)
	      EVASIONE[re][qu] ^= quadretto_fisso[re - 9];
	  }
	if ((TRAVERSA (re) - TRAVERSA (qu)) == (COLONNA (qu) - COLONNA (re)))
	  {
	    if (TRAVERSA (re) != T8 && COLONNA (re) != CA)
	      EVASIONE[re][qu] ^= quadretto_fisso[re + 7];
	    if (TRAVERSA (re) != T1 && COLONNA (re) != CH)
	      EVASIONE[re][qu] ^= quadretto_fisso[re - 7];
	  }
	if (attaco_re[re] & quadretto_fisso[qu])
	  EVASIONE[re][qu] |= quadretto_fisso[qu];
      }

  for (co = CA; co <= CH; co++)
    {
      colonnas_sinistras[co] = colonnas_rettas[co] = 0;
      for (i = CA; i < co; i++)
	colonnas_sinistras[co] |= pila_colonna[i];
      for (i = co + 1; i <= CH; i++)
	colonnas_rettas[co] |= pila_colonna[i];
    }

  for (qu = A1; qu <= H8; qu++)
    for (re = A1; re <= H8; re++)
      {
	INTERPOSIZIONE[re][qu] = quadretto_fisso[qu];
	dir = 0;
	if (TRAVERSA (re) == TRAVERSA (qu))
	  {
	    if (re > qu)
	      dir = 1;
	    else
	      dir = -1;
	  }
	if (COLONNA (re) == COLONNA (qu))
	  {
	    if (re > qu)
	      dir = 8;
	    else
	      dir = -8;
	  }
	if ((TRAVERSA (re) - TRAVERSA (qu)) == (COLONNA (re) - COLONNA (qu)))
	  {
	    if (re > qu)
	      dir = 9;
	    else
	      dir = -9;
	  }
	if ((TRAVERSA (re) - TRAVERSA (qu)) == (COLONNA (qu) - COLONNA (re)))
	  {
	    if (re > qu)
	      dir = 7;
	    else
	      dir = -7;
	  }
	if (dir)
	  for (i = qu; i != re; i += dir)
	    bitFISSO (i, INTERPOSIZIONE[re][qu]);
      }

  for (qu = A1; qu <= H8; qu++)
    {
      ortogonale[qu] =
	pila_traversa[TRAVERSA (qu)] | pila_colonna[COLONNA (qu)];
      diagonale[qu] = 0;
      for (co = COLONNA (qu), tr = TRAVERSA (qu); co <= CH && tr <= T8;
	   co++, tr++)
	bitFISSO (8 * tr + co, diagonale[qu]);
      for (co = COLONNA (qu), tr = TRAVERSA (qu); co <= CH && tr >= T1;
	   co++, tr--)
	bitFISSO (8 * tr + co, diagonale[qu]);
      for (co = COLONNA (qu), tr = TRAVERSA (qu); co >= CA && tr <= T8;
	   co--, tr++)
	bitFISSO (8 * tr + co, diagonale[qu]);
      for (co = COLONNA (qu), tr = TRAVERSA (qu); co >= CA && tr >= T1;
	   co--, tr--)
	bitFISSO (8 * tr + co, diagonale[qu]);
      ortogonale[qu] &= quadretto_libero[qu];
      diagonale[qu] &= quadretto_libero[qu];
      non_ortogonale[qu] = ~ortogonale[qu];
      non_diagonale[qu] = ~diagonale[qu];
      ortodiagonale[qu] = ortogonale[qu] | diagonale[qu];
    }

  for (j = A1; j <= H8; j++)
    for (i = A1; i <= H8; i++)
      {
	indice_linea[i][j] = direzione_malato;
	if (i == j)
	  continue;
	if (TRAVERSA (j) == TRAVERSA (i))
	  indice_linea[i][j] = direzione_orizzontale;
	if (COLONNA (j) == COLONNA (i))
	  indice_linea[i][j] = direzione_verticale;
	if ((COLONNA (i) - COLONNA (j)) == (TRAVERSA (i) - TRAVERSA (j)))
	  indice_linea[i][j] = direzione_a1h8;
	if ((COLONNA (j) - COLONNA (i)) == (TRAVERSA (i) - TRAVERSA (j)))
	  indice_linea[i][j] = direzione_h1a8;
      }

  zob_iniz ();
}
