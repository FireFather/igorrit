
#include "RobboLito.h"
#ifdef YUSUF_MULTICORE
#define DO_LOCKED(x) { LOCK (SMP_IVAN); (x); UNLOCK (SMP_IVAN); }
#define MAX_SPLIT 8

volatile int SMP_FREE;
tipo_posizione* volatile WORKING[MAX_CPUS];
logico volatile PTHREAD_EXIT[MAX_CPUS];
void thread_halt (tipo_posizione *POS);
logico INIT[MAX_CPUS];

typedef struct
{
  int cpu;
}
t_args;

t_args ARGS[MAX_CPUS];

volatile int init_threads;
//volatile logico IVAN_END_SMP;
//jmp_buf DEC_JMP[MAX_CPUS]; /* Decembrist bugfix */
volatile int active_threads;

void thread_stall (tipo_posizione *PARENT, int cpu);
void thread_init (int cpu)
{
  DO_LOCKED (init_threads++);
  while (init_threads < CPUS_SIZE)
    {
    }
  WaitForSingleObject(WAKEUP[cpu], INFINITE);
  DO_LOCKED (active_threads++);
  thread_stall (NULL_PARENT, cpu);
}

void ivan_end_smp()
{
  int cpu;
  int rp;
  int sp;

  LOCK (SMP_IVAN);
  for (cpu = 1; cpu < CPUS_SIZE; cpu++)
    PTHREAD_EXIT[cpu] = VERO;
  UNLOCK (SMP_IVAN);
  for (cpu = 1; cpu < CPUS_SIZE; cpu++)
    {
      PTHREAD_EXIT[cpu] = VERO;
      SetEvent(WAIT_EVENT[cpu]);
    }
  for (cpu = 0; cpu < CPUS_SIZE; cpu++)
    for (rp = 0; rp < RP_PER_CPU ; rp++)
      ROOT_POSIZIONE[cpu][rp].halt = VERO;

  /* Here we try to put other threads to sleep, we send sleep signal and
     wait for 10ms, if some are still awake we reset WAKEUP event and 
	 signal WAIT_EVENT (to release if some thread is stuck somewhere.
	 Then we wait another 10ms. Finally, if some threads are still awake,
	 we wait final 10ms and then give up, reseting WAIT_EVENT back. */
  if (active_threads > 1)
    {
      for (cpu = 0; cpu < CPUS_SIZE; cpu++)
	    for (rp = 0; rp < RP_PER_CPU ; rp++)
	      ROOT_POSIZIONE[cpu][rp].halt = VERO;
	  WaitForSingleObject(WAKEUP[0], 5);
      if (active_threads > 1)
        {
	      for (cpu = 1; cpu < CPUS_SIZE; cpu++)
		    {
			  ResetEvent(WAKEUP[cpu]);
		      SetEvent(WAIT_EVENT[cpu]);
		    }
		  //printf("info time 0 pv e2e4 !!! FIRST SLEEP WAIT !!!\n");
	      //fflush(stdout);
		  WaitForSingleObject(WAKEUP[0], 10);
          if (active_threads > 1)
            {
	        //printf("info time 0 pv e2e4 !!! THREAD WANT SLEEP !!!\n");
		    //fflush(stdout);
			WaitForSingleObject(WAKEUP[0], 10);
	        for (cpu = 1; cpu < CPUS_SIZE; cpu++)
	          ResetEvent(WAIT_EVENT[cpu]);
            }
	    }
    }
  for (cpu = 0; cpu < CPUS_SIZE; cpu++)
    for (rp = 0; rp < RP_PER_CPU ; rp++)
      ROOT_POSIZIONE[cpu][rp].used = FALSO;
  for (sp = 0; sp < MAX_SP; sp++)
    ROOT_SP[sp].aktiv = FALSO;
}

IVAN_THREAD(A)
{
  t_args *AA;
  AA = (t_args*) A;
  INIT[AA->cpu] = VERO;
  thread_init (AA->cpu);
  return (DWORD)NULL;
}

static logico INIT0 = FALSO;
void ivan_init_smp()
{
  int cpu;
  int rp;
  int sp;
  tipo_posizione *RP00;
  int h;
  
  SMP_FREE = 0;
  for (cpu = 0; cpu < CPUS_SIZE; cpu++)
    for (rp = 0; rp < RP_PER_CPU ; rp++)
      {
	ROOT_POSIZIONE[cpu][rp].used = FALSO;
	ROOT_POSIZIONE[cpu][rp].nodes = 0;
      }
  for (sp = 0; sp < MAX_SP; sp++)
    ROOT_SP[sp].aktiv = FALSO;
  for (cpu = 0; cpu < CPUS_SIZE; cpu++)
    {
      WORKING[cpu] = NULL;
      PTHREAD_EXIT[cpu] = FALSO;
    }
  NULL_PARENT->child_count = 123;
  if (!INIT0)
    {
	  INIT0 = VERO;
      for (cpu = 1; cpu < CPUS_SIZE; cpu++) /* MAX_CPUS ? */
	   {
	     ARGS[cpu].cpu = cpu;
	     PTHREAD_CREATE (&PTHREAD[cpu], NULL, ivan_thread, &ARGS[cpu]);
	   }
	  while (init_threads < CPUS_SIZE)
	    {
	    }
    }
  if (active_threads < CPUS_SIZE) 
    for (cpu = 1; cpu < CPUS_SIZE; cpu++)
	  SetEvent(WAKEUP[cpu]);

  RP00 = &ROOT_POSIZIONE[0][0];
  RP00->used = VERO;
  RP00->halt = FALSO;
  memcpy (RP00, ROOT_POSIZIONE0, 356);
  memcpy (RP00->din_iniz, ROOT_POSIZIONE0->din_iniz, 2 * sizeof (tipo_dinamico));
  RP00->DIN = RP00->din_iniz + 1;
#undef mucchio_altezzo
#undef MUCCHIO
  h = ROOT_POSIZIONE0->mucchio_altezzo;
  memcpy (RP00->MUCCHIO, ROOT_POSIZIONE0->MUCCHIO, h * sizeof (uint64));
  RP00->mucchio_altezzo = h;
  RP00->child_count = 0;
  RP00->parent = NULL;
  for (cpu = 0; cpu < CPUS_SIZE; cpu++)
    RP00->children[cpu] = NULL;
  RP00->splitpunkt = NULL;
  WORKING[0] = RP00;
}

static void SMP_GOOD_HISTORY (tipo_posizione *POS, uint32 m, SPLITPUNKT *sp)
{
  int sv = STORIA_VALU (POS, m);
  STORIA_VALU (POS, m) = sv + (( (0xff00 - sv) * sp->abisso) >> 8);
}

void ivan_fail_high (SPLITPUNKT *sp, tipo_posizione *POS, uint32 m)
{
  int cpu;
  LOCK (sp->splock);
  if (sp->tot || POS->halt)
    {
      UNLOCK (sp->splock);
      return;
    }
  sp->tot = VERO;
  sp->mossa = m;
  sp->value = sp->beta;
  UNLOCK (sp->splock);
  if (POS->qu[AI (m)] == 0 && mossa_storia (m))
    SMP_GOOD_HISTORY (POS, m, sp);
  if (sp->node_type == NODE_TYPE_ALL)
    Zobrist_basso_totale (POS, m, sp->abisso, sp->beta);
  else
    Zobrist_basso (POS->DIN->Zobrist, m, sp->abisso, sp->beta);
  LOCK (SMP_IVAN);
  LOCK (POS->parent->padlock);
  if (!POS->halt)
    {
      for (cpu = 0; cpu < CPUS_SIZE; cpu++)
	if (POS->parent->children[cpu] && cpu != POS->cpu)
	  thread_halt (POS->parent->children[cpu]);
    }
  UNLOCK (POS->parent->padlock);
  UNLOCK (SMP_IVAN);
}

static INLINE void SMP_BAD_HISTORY (tipo_posizione *POS, uint32 m, SPLITPUNKT *sp)
{
  if ((POS->DIN + 1)->cattura == 0 && mossa_storia (m))
    {
      int sv = STORIA_VALU (POS, m);
      if (POS->DIN->valu > sp->alpha - 50)
	STORIA_VALU (POS, m) = sv - ((sv * sp->abisso) >> 8);
    }
}

static void ivan_search_cut_node (tipo_posizione *POS)
{
  SPLITPUNKT *sp;
  sp = POS->splitpunkt;
  LOCK (sp->splock);
  sp->childs++;
  UNLOCK (sp->splock);
  POS->bianco_en_mossa ? ivan_smp_cut_node_white (POS) : ivan_smp_cut_node_black (POS);
  LOCK (sp->splock);
  sp->childs--;
  if (!sp->tot && !sp->childs && !POS->halt)
    {
      Zobrist_alto_tagliato (POS, sp->abisso, sp->value);
    }
  UNLOCK (sp->splock);
}

static void ivan_search_all_node (tipo_posizione *POS)
{
  SPLITPUNKT *sp;
  sp = POS->splitpunkt;
  LOCK (sp->splock);
  sp->childs++;
  UNLOCK (sp->splock);
  POS->bianco_en_mossa ? ivan_smp_all_node_white (POS) : ivan_smp_all_node_black (POS);
  LOCK (sp->splock);
  sp->childs--;
  if (!sp->tot && !sp->childs && !POS->halt)
    {
      Zobrist_alto (POS->DIN->Zobrist, sp->abisso, sp->value);
    }
  UNLOCK (sp->splock);
}


void ivan_search (tipo_posizione *POS)
{
  SPLITPUNKT *sp;
  sp = POS->splitpunkt;
  if (sp->node_type == NODE_TYPE_ALL)
    {
      ivan_search_all_node (POS);
      return;
    }
  if (sp->node_type == NODE_TYPE_CUT)
    {
      ivan_search_cut_node (POS);
      return;
    }
  LOCK (sp->splock);
  sp->childs++;
  UNLOCK (sp->splock);
  POS->bianco_en_mossa ? ivan_smp_pv_node_white (POS) : ivan_smp_pv_node_black (POS);
  LOCK (sp->splock);
  sp->childs--;
  if (!sp->tot && !sp->childs && !POS->halt)
    {
      uint32 m = sp->mossa_buona;
      if (m)
	{
	  Zobrist_preciso (POS, m, sp->abisso, sp->value, pietra_precisa);
	  if (POS->qu[AI (m)] == 0 && mossa_storia (m))
	    SMP_GOOD_HISTORY (POS, m, sp);
	}
      else
	  Zobrist_alto (POS->DIN->Zobrist, sp->abisso, sp->value);
    }
  UNLOCK (sp->splock);
}

static void COPY_FROM_CHILD (tipo_posizione *PARENT, tipo_posizione *CHILD)
{
  if (CHILD->splitpunkt->value >= CHILD->splitpunkt->beta)
    PARENT->DIN->mossa = CHILD->splitpunkt->mossa;
  else
    PARENT->DIN->mossa = 0;
}

/* #include <signal.h> */
void thread_stall (tipo_posizione *PARENT, int cpu)
{
  tipo_posizione *W;
  while (VERO)
    {
      DO_LOCKED (SMP_FREE++);
      while (!WORKING[cpu] && PARENT->child_count && !PTHREAD_EXIT[cpu])
	    {
          WaitForSingleObject(WAIT_EVENT[cpu], INFINITE);
	    }

      if (PTHREAD_EXIT[cpu])
	    {
	      WORKING[cpu] = NULL;
	      PARENT = NULL_PARENT;
	      DO_LOCKED (active_threads--);
	      DO_LOCKED (SMP_FREE--);
	      WaitForSingleObject(WAKEUP[cpu], INFINITE);
		  PTHREAD_EXIT[cpu] = FALSO;
		  DO_LOCKED (active_threads++);
	      continue;
	    }

      LOCK (SMP_IVAN);
      DO_LOCKED (SMP_FREE--);
      W = WORKING[cpu];
      if (!W)
	    {
	    WORKING[cpu] = PARENT;
	    SetEvent (WAIT_EVENT[cpu]);
	    UNLOCK (SMP_IVAN); 
	    return;
	  }
      UNLOCK (SMP_IVAN);

      ivan_search (W);
      LOCK (SMP_IVAN);
      LOCK (W->parent->padlock);
      COPY_FROM_CHILD (W->parent, W);
      W->parent->child_count--;

      if (W->parent->child_count == 0)
	    {
	      int icpu = W->parent->cpu;
	      SetEvent(WAIT_EVENT[icpu]);
	    }

      W->parent->children[cpu] = NULL;
      UNLOCK (W->parent->padlock);
      WORKING[cpu] = NULL;
      W->used = FALSO;
      UNLOCK (SMP_IVAN);
    }
}

void thread_halt (tipo_posizione *POS)
{
  int n;  
  LOCK (POS->padlock);
  POS->halt = VERO;
  for (n = 0; n < CPUS_SIZE; n++)
    {
      if (POS->children[n] != NULL)
	thread_halt (POS->children[n]);
    }
  UNLOCK (POS->padlock);
}

static void sp_init()
{
  int sp;
  for (sp = 0; sp < MAX_SP; sp++)
    {
      ROOT_SP[sp].aktiv = FALSO;
      LOCK_INIT (ROOT_SP[sp].splock);
    }
}

void rp_init()
{
  int cpu;
  int rp;
  INIT0 = FALSO;
  LOCK_INIT (SMP_IVAN);
  for (cpu = 0; cpu < MAX_CPUS; cpu++)
    for (rp = 0; rp < RP_PER_CPU ; rp++)
     {
        memset( (void*) &ROOT_POSIZIONE[cpu][rp], 0, sizeof (tipo_posizione));
        ROOT_POSIZIONE[cpu][rp].din_iniz = malloc (abisso_massimo * sizeof(tipo_dinamico));
        ROOT_POSIZIONE[cpu][rp].used =  FALSO;
        ROOT_POSIZIONE[cpu][rp].parent = NULL;
        ROOT_POSIZIONE[cpu][rp].DIN = ROOT_POSIZIONE[cpu][rp].din_iniz;
        ROOT_POSIZIONE[cpu][rp].cpu = cpu;
        LOCK_INIT (ROOT_POSIZIONE[cpu][rp].padlock);
      }
  for (cpu = 0; cpu < MAX_CPUS; cpu++)
    WAKEUP[cpu] = CreateEvent(0, FALSO, FALSO, 0);
  for (cpu = 0; cpu < MAX_CPUS; cpu++)
    WAIT_EVENT[cpu] = CreateEvent(0, FALSO, FALSO, 0);
  init_threads = 1;
  active_threads = 1;
  sp_init ();
}

static tipo_posizione* get_posizione (int cpu)
{
  int u;
  for (u = 0; u < RP_PER_CPU; u++)
    if (!ROOT_POSIZIONE[cpu][u].used)
      break;
  if (u == RP_PER_CPU)
    return NULL;
  ROOT_POSIZIONE[cpu][u].used = VERO;
  ROOT_POSIZIONE[cpu][u].halt = FALSO;
  return &ROOT_POSIZIONE[cpu][u];
}

static tipo_posizione* COPY_TO_CHILD (int icpu, tipo_posizione *PARENT)
{
  tipo_posizione *CHILD;
  int cpu;
  int h;
  CHILD = get_posizione (icpu);
  if (!CHILD)
    return NULL;
  for (cpu = 0; cpu < CPUS_SIZE; cpu++)
    CHILD->children[cpu] = NULL;
  memcpy (CHILD, PARENT, 356);
  memcpy (CHILD->din_iniz, PARENT->DIN - 1, 2 * sizeof (tipo_dinamico));
  CHILD->DIN = CHILD->din_iniz + 1;
#undef mucchio_altezzo
#undef MUCCHIO
  h = PARENT->mucchio_altezzo;
  memcpy (CHILD->MUCCHIO, PARENT->MUCCHIO, h * sizeof (uint64));
  CHILD->mucchio_altezzo = h;
  return CHILD;
}

static void end_splitpunkt (SPLITPUNKT *sp) /* LOCKed ? */
{
  sp->aktiv = FALSO;
}

static SPLITPUNKT* new_splitpunkt()
{
  int sp;
  for (sp = 0; sp < MAX_SP; sp++)
    if (!ROOT_SP[sp].aktiv)
      return &ROOT_SP[sp];
  return NULL;
}

logico IVAN_SPLIT (tipo_posizione *POSIZIONE, tipo_prossimo *PROSSIMO,
		   int abisso, int beta, int alpha, int NODE_TYPE, int *r)
{
  int cpu;
  int split;
  tipo_posizione *CHILD;
  SPLITPUNKT *sp;

/* If one or more threads are still sleeping, wait 5 more ms, if they don't wake up,
   send wakeup singnal again, and wait another 5ms. If some is not awake, it's lost
   for this move and continue since we can't wait any more. Resets events also. */
  if (active_threads < CPUS_SIZE) 
    {
	  WaitForSingleObject(WAKEUP[0], 5);
	  //printf("info time 0 pv e2e4 !!! FIRST WAIT !!!\n");
	  //fflush(stdout);
    }
  if (active_threads < CPUS_SIZE)
    {
	  for (cpu = 1; cpu < CPUS_SIZE; cpu++)
	    SetEvent(WAKEUP[cpu]);
	  WaitForSingleObject(WAKEUP[0], 5);
      if (active_threads < CPUS_SIZE)
        {
	      //printf("info time 0 pe2e4 !!! THREAD LOST !!!\n");
		  //fflush(stdout);
	      for (cpu = 1; cpu < CPUS_SIZE; cpu++)
	        ResetEvent(WAKEUP[cpu]);
        }
    }

  LOCK (SMP_IVAN);
  for (cpu = 0; cpu < CPUS_SIZE; cpu++)
    if (!WORKING[cpu])
      break;
  if (POSIZIONE->halt || cpu == CPUS_SIZE)
    {
      UNLOCK (SMP_IVAN);
      return FALSO;
    }
  WORKING[POSIZIONE->cpu] = NULL;
  POSIZIONE->child_count = 0;
  sp = new_splitpunkt ();
  if (sp == NULL)
    {
      WORKING[POSIZIONE->cpu] = POSIZIONE;      
      UNLOCK (SMP_IVAN);
      return FALSO;
    }
  LOCK (sp->splock); /* ? */
  sp->alpha = alpha;
  sp->beta = beta;
  sp->abisso = abisso;
  sp->node_type = NODE_TYPE;
  if (NODE_TYPE != NODE_TYPE_PV)
    sp->value = sp->beta - 1;
  else
    sp->value = sp->alpha;
  sp->mossa = 0;
  sp->mossa_buona = 0;
  sp->childs = 0;
  sp->MOVE_PICK = PROSSIMO;
  sp->tot = FALSO;
  sp->aktiv = VERO;
  UNLOCK (sp->splock); /* ? */
  split = 0;
  for (cpu = 0; cpu < CPUS_SIZE && split < MAX_SPLIT; cpu++)
    {
      POSIZIONE->children[cpu] = NULL;
      if (WORKING[cpu] == NULL)
	    {
	      CHILD = COPY_TO_CHILD (cpu, POSIZIONE);
	      if (!CHILD)
	        continue;
	      split++;
	      POSIZIONE->children[cpu] = CHILD;
	      CHILD->cpu = cpu;
	      CHILD->parent = POSIZIONE;
	      CHILD->halt = FALSO;
	      CHILD->splitpunkt = sp;
	      POSIZIONE->child_count++;
	    }
    }
  if (split == 0)
    {
      WORKING[POSIZIONE->cpu] = POSIZIONE;
      LOCK (sp->splock);
      end_splitpunkt (sp);
      UNLOCK (sp->splock);
      UNLOCK (SMP_IVAN);
      return FALSO;
    }
  /* cuando "split" es uno ? */
  for (cpu = 0; cpu < CPUS_SIZE; cpu++)
    {
      if (POSIZIONE->children[cpu])
	    {
	      WORKING[cpu] = POSIZIONE->children[cpu];
	      SetEvent(WAIT_EVENT[cpu]);
	    }
    }
  UNLOCK (SMP_IVAN);
  thread_stall (POSIZIONE, POSIZIONE->cpu);
  LOCK (SMP_IVAN);
  LOCK (sp->splock); /* ? */
  *r = sp->value;
  end_splitpunkt (sp);
  UNLOCK (sp->splock); /* ? */
  UNLOCK (SMP_IVAN);
  return VERO;
}
#endif

