
#define TURNO 8
#define STORIA_BUONA(mossa, abisso) \
 { int sv = STORIA_VALU (POSIZIONE , mossa); \
   STORIA_VALU (POSIZIONE, mossa) = \
                 sv + (( (0xff00 - sv) * abisso) >> TURNO); \
   if (mossa != DINAMICO->uccisore_uno) \
   { DINAMICO->uccisore_due = DINAMICO->uccisore_uno; \
     DINAMICO->uccisore_uno = mossa; } }

#define STORIA_BRUTTA(mossa, abisso) \
 { int sv = STORIA_VALU (POSIZIONE, mossa); \
   if (POSIZIONE0->valu > VALU - 50) \
      STORIA_VALU (POSIZIONE, mossa) = sv - ((sv * abisso) >> TURNO); }

#define STORIA_BRUTTA_pv(mossa, abisso) \
 { int sv = STORIA_VALU (POSIZIONE, mossa); \
   if (POSIZIONE0->valu > BASSO - 50) \
      STORIA_VALU (POSIZIONE, mossa) = sv - ((sv * abisso) >> TURNO); }
