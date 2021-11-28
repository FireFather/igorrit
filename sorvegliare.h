
uint32 radice_mossa;
uint32 ponder_move;
int radice_valu, radice_precedente;
int radice_abisso, abisso_precedente, radice_rapido;

#include <time.h>
extern jmp_buf J;
logico salto_ok;
logico FACILE, MOSSA_BRUTTA, MOSSA_BATTAGLIA;
uint64 OROLOGIO, MOMENTO;
logico ANALISI;
logico PONDER;
logico PONDERING;
int MPH;
int numCPU;