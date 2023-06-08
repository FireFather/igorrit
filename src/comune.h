
#include <string.h>
#define valu_tempo 5
#define valu_tempo2 5
#define pedone_prugna 160
#define minore_prugna 500
#define torre_prugna 800
#define scacco_prugna 10
#define valu_pigro 150
#define valu_pigro2 300
#define partita_prima ((DINAMICO->materiale & 0xff) >= 18)

#ifdef MULTI_GUADAGNO
#define MASSIMO_POSIZIONALE(x)\
 ((int) massimo_posizionale_guadagno[POSIZIONE->cpu][POSIZIONE->qu[DI(x)]][x & 07777])
#else
#define MASSIMO_POSIZIONALE(x)\
 ((int) massimo_posizionale_guadagno[POSIZIONE->qu[DI(x)]][x & 07777])
#endif

#define ripetizione_interrogativo(x)\
  (VALU > x && POSIZIONE0->mossa50 >= 2 &&\
   ((AI(mossa) << 6) | DI(mossa)) == (POSIZIONE0 -1 )->mossa\
   && POSIZIONE->qu[AI(mossa)] == 0)

#define INTERROGATIVO_INTERPOSIZIONE(x) (x & (1 << 15))
#define SEE_facile(x) (x & 0x300000)

#define pedone_libero_mossa(ai, x)\
  (POSIZIONE->qu[ai] == conto_mio_pedone && x &&\
   (bitbordo_tuo_pedone & pedone_libero[ai]) == 0)

#define PEZZO_BIANCO(pezzo) (pezzo <= 7)
#define PEZZO_NERO(pezzo) (pezzo >= 8)
