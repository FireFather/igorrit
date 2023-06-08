
#include "RobboLito.h"
#include "sorvegliare.h"

void pedone_reset ()
{
  memset (pedone_zobrist, 0,
	  elenco_pedone_numero * sizeof (tipo_pedone_valu));
}

int
pedone_zobrist_inizializzazione (int mb)
{
  if (mb > 1024)
    mb = 1024;
  elenco_pedone_numero = ((1 << BSR (mb)) << 20) / sizeof (tipo_pedone_valu);
  mb = (elenco_pedone_numero * sizeof (tipo_pedone_valu)) >> 20;
  if (pedone_zobrist)
    free (pedone_zobrist);
  POSIX_MEMALIGN (pedone_zobrist, 64,
		  elenco_pedone_numero * sizeof (tipo_pedone_valu));
  pedone_reset ();
  return (mb);
}

void
guadagno_reset ()
{
  int p, m;
#ifdef MULTI_GUADAGNO
  int cpu;
  for (cpu = 0; cpu < MAX_CPUS; cpu++)
#endif
    for (p = 0; p < 0x10; p++)
      for (m = 0; m < 010000; m++)
#ifdef MULTI_GUADAGNO
	massimo_posizionale_guadagno[cpu][p][m] = 0;
#else
        massimo_posizionale_guadagno[p][m] = 0;
#endif
}

void
storia_reset ()
{
  int pezzo, qu;
#ifdef MULTI_STORIA
  int cpu;
  for (cpu = 0; cpu < MAX_CPUS; cpu++)
#endif
    for (pezzo = 0; pezzo < 0x10; pezzo++)
      for (qu = A1; qu <= H8; qu++)
#ifdef MULTI_STORIA
	storia_valu[cpu][pezzo][qu] = 0x800;
#else
	storia_valu[pezzo][qu] = 0x800;
#endif
}

static void
gonfalone ()
{
  printf ("%s %s\n", EPONYM, VERSION);
#ifdef HAS_PREFETCH
  printf ("compiled with PREFETCH\n");
#endif
#ifdef HAS_POPCNT
  printf ("compiled with POPCNT\n");
#endif
#ifdef YUSUF_MULTICORE
  printf ("Capitalist multicore mode compiled\n");
  printf ("Windows version improved by Sentinel\n");
#endif
  fflush (stdout);
}

int
main ()
{
  gonfalone ();
  pedone_zobrist = NULL;
  memset ( (void*) ROOT_POSIZIONE0, 0, sizeof (tipo_posizione));
  ROOT_POSIZIONE0->din_iniz =
    malloc (abisso_massimo * sizeof(tipo_dinamico));
  ROOT_POSIZIONE0->DIN = ROOT_POSIZIONE0->din_iniz; 
  ROOT_POSIZIONE0->halt = FALSO;
#ifdef WINDOWS
  CPUS_SIZE = check_num_CPU();
#else
  CPUS_SIZE = 1;
#endif
  PONDER = FALSO;
  MPH = 2;
#ifdef YUSUF_MULTICORE
  TITANIC_MODE = VERO;
  rp_init ();
#endif
  pedone_zobrist_inizializzazione (2);
  inizializzazione_zobrist (128);
  storia_reset ();
  inizializzazione_cattura ();
  pila_inizializzazione ();
  pedone_inizializzazione ();
  inizializzazione_valore ();
  statico_inizializzazione ();
  partita_nuova (ROOT_POSIZIONE0, VERO);
#ifdef CON_ROBBO_BUILD
  total_iniz ();
#endif
  while (1)
    ingresso (ROOT_POSIZIONE0);
  return 0;
}
