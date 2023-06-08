
#include "RobboLito.h"
#include <string.h>
#include "sorvegliare.h"

static sint64 INVIO;
static sint64 ORA_ASSOLUTA, ORA_DESIDERATA, INCREMENTO;
static int ABISSO;
static logico ALT;

void
cerca_alt (int tr)
{
  ALT = VERO;
  if (salto_ok)
    longjmp (J, 1);
}

void
notizia (sint64 x)
{
  uint64 t, nps, NODOS = 0;
  int cpu, rp;
  clock_t u;
#ifdef YUSUF_MULTICORE
  if (TITANIC_MODE)
    for (cpu = 0; cpu < CPUS_SIZE; cpu++)
      for (rp = 0; rp < RP_PER_CPU ; rp++)
        NODOS += ROOT_POSIZIONE[cpu][rp].nodes;
  else
#endif
    NODOS = ROOT_POSIZIONE0->nodes;
  u = clock ();
  INVIO = x;
  t = x / 1000;
  if (t == 0)
    nps = 0;
  else
    nps = NODOS / t;
  u = clock () - MOMENTO;
  TRASMISSIONE ("info time %I64u nodes %I64u nps %I64u cpuload %d\n",
		t, NODOS, nps * 1000,
		(int) ((double) u / (double) x * 1000.0));
}

static sint64 ORA_BATTAGLIA, ORA_FACILE, ORA_ORDINARIA;

void
risolvere_termine (tipo_posizione *POSIZIONE, int d)
{
  uint64 x;
  if (!salto_ok)
    return;
  x = orologio () - OROLOGIO;
  if (PONDERING)
	goto FINALE;
  if (d == ABISSO)
    cerca_alt (1);
  if (x - INVIO > 1000000)
    notizia (x);
  if (x > ORA_ASSOLUTA)
    cerca_alt (1);
  if (d >= 0 && d < 8)
    goto FINALE;
  if (!MOSSA_BRUTTA && x >= ORA_BATTAGLIA)
    cerca_alt (2);
  if (FACILE && x >= ORA_FACILE)
    cerca_alt (3);
  if (!MOSSA_BATTAGLIA && x >= ORA_ORDINARIA && !MOSSA_BRUTTA)
    cerca_alt (4);
 FINALE:
  while (interrogativo_ingresso ())
    ingresso (POSIZIONE);
}

#define INFINITO 0xffffffffffffffff
#define STRTOK(p) p = strtok (NULL, " ")

void
cerca_inizializzazione (tipo_posizione *POSIZIONE, char *str)
{
  char *p;
  sint64 ora_bianca = INFINITO, ora_nera = INFINITO,
    incremento_bianco = 0, incremento_nero = 0, ORA, movestogo = 0;
  int PTF = 0;
  ABISSO = 255;
  ORA_ASSOLUTA = ORA_DESIDERATA = INFINITO;
  ALT = FALSO;
  INVIO = 0;
  PONDERING = FALSO;
  p = strtok (str, " ");
  for (STRTOK (p); p != NULL; STRTOK (p))
    {
      if (!strcmp (p, "depth"))
	{
	  STRTOK (p);
	  ABISSO = MASSIMO (1, atoll (p));
	}
      else if (!strcmp (p, "movetime"))
	{
	  STRTOK (p);
	  ORA_ASSOLUTA = MASSIMO (1, atoll (p)) * 1000 - 10000;
	}
      else if (!strcmp (p, "wtime"))
	{
	  STRTOK (p);
	  ora_bianca = atoll (p) * 1000;
	}
      else if (!strcmp (p, "winc"))
	{
	  STRTOK (p);
	  incremento_bianco = atoll (p) * 1000;
	}
      else if (!strcmp (p, "btime"))
	{
	  STRTOK (p);
	  ora_nera = atoll (p) * 1000;
	}
      else if (!strcmp (p, "binc"))
	{
	  STRTOK (p);
	  incremento_nero = atoll (p) * 1000;
	}
      else if (!strcmp (p, "movestogo"))
	{
	  STRTOK (p);
	  movestogo = atoll (p);
	}
	  else if (!strcmp(p, "ponder") && PONDER)
	{
		PONDERING = TRUE;
		continue;
	}
      else if (!strcmp (p, "infinite"))
	continue;
      else
	ERRORE ("go string: %s", p);
    }

  	if (PONDER)
		if( MPH == 3 )
			PTF = 8;
		else if( MPH == 2 )
			PTF = 6;

  ORA_BATTAGLIA = INFINITO;
  ORA_ORDINARIA = INFINITO;
  ORA_FACILE = INFINITO;
  ORA = POSIZIONE->bianco_en_mossa ? ora_bianca : ora_nera;
  if (ORA == INFINITO)
    goto FINALE;

  ORA = MASSIMO(ORA - 500000, 9 * ORA / 10);
  if (ORA < 0)
	ORA = 1;
  INCREMENTO = POSIZIONE->bianco_en_mossa ? incremento_bianco : incremento_nero;

  if (movestogo)
    {
      ORA_DESIDERATA = ORA / movestogo + INCREMENTO;
	  if (movestogo > 3)
		ORA_ASSOLUTA = MINIMO(ORA * (movestogo + 2) / (movestogo * 3), 6 * ORA_DESIDERATA);
	  else
		ORA_ASSOLUTA = ORA * (movestogo + 1) / (movestogo * 2);
      if (ORA_ASSOLUTA < 10000)
	    ORA_ASSOLUTA = 10000;
    }
  else
    {
	  if ((ORA / 40) > INCREMENTO)
		{
			ORA_DESIDERATA = ORA / (((48 - PTF) * ORA / 40 - INCREMENTO * 18) / (ORA / 40)) + INCREMENTO;
			ORA_ASSOLUTA = ORA / ((5 * ORA / 40 - INCREMENTO * 3) / (ORA / 40));
		}
		else
		{
			ORA_DESIDERATA = (ORA / (30 - PTF / 2)) + INCREMENTO;
			ORA_ASSOLUTA = ORA / 2;
		}

		if (ORA < 500000)
		{
			ORA_DESIDERATA = 5000;
			ORA_ASSOLUTA = 10000;
		}
		else if (ORA < 1000000 && INCREMENTO < 500000)
		{
			ORA_DESIDERATA = ORA / 80;
			ORA_ASSOLUTA = ORA / 20;
		}
		else if (ORA < 2000000 && INCREMENTO < 500000)
		{
			ORA_ASSOLUTA = ORA / 10;
		}
    }

  if (ORA_DESIDERATA < 5000)
	ORA_DESIDERATA = 5000;

  ORA_FACILE = ORA_DESIDERATA / 4;
  ORA_BATTAGLIA = ORA_DESIDERATA;
  ORA_ORDINARIA = (3 * ORA_DESIDERATA) / 4;

 FINALE:
  if (ORA_ASSOLUTA == INFINITO)
    ANALISI = VERO;
  else
    ANALISI = FALSO;
}
