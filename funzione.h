
int pedone_zobrist_inizializzazione (int);
void pila_inizializzazione ();

void cerca_alt (int);
void risolvere_termine (tipo_posizione *, int);
void cerca_inizializzazione (tipo_posizione *, char *);
void notizia (sint64);

void valutazione (tipo_posizione *, int, int, int);
void mobilita (tipo_posizione *);
void valu_zobrist_libero ();

tipo_lista_mossa *cattura_mossa (tipo_posizione *, tipo_lista_mossa *, uint64);
tipo_lista_mossa *ordinaria_mossa (tipo_posizione *, tipo_lista_mossa *);
tipo_lista_mossa *evasione_mossa (tipo_posizione *, tipo_lista_mossa *, uint64);
void inizializzazione_cattura ();

tipo_lista_mossa *cattura_bianca (tipo_posizione *, tipo_lista_mossa *, uint64);
tipo_lista_mossa *cattura_nera (tipo_posizione *, tipo_lista_mossa *, uint64);
tipo_lista_mossa *ordinaria_bianca (tipo_posizione *, tipo_lista_mossa *);
tipo_lista_mossa *ordinaria_nera (tipo_posizione *, tipo_lista_mossa *);
void sorta_ordinaria
(tipo_lista_mossa *, tipo_lista_mossa *, uint32, uint32, uint32);

tipo_lista_mossa *scacco_zitto_bianco (tipo_posizione *, tipo_lista_mossa *, uint64);
tipo_lista_mossa *scacco_zitto_nero (tipo_posizione *, tipo_lista_mossa *, uint64);

tipo_lista_mossa *guadagno_bianco (tipo_posizione *, tipo_lista_mossa *, int);
tipo_lista_mossa *guadagno_nero (tipo_posizione *, tipo_lista_mossa *, int);
tipo_lista_mossa *evasione_nero (tipo_posizione *, tipo_lista_mossa *, uint64);
tipo_lista_mossa *evasione_bianco (tipo_posizione *, tipo_lista_mossa *, uint64);

void anno_incremento ();
void zobrist_libero ();
int inizializzazione_zobrist (int);

void Zobrist_basso_totale (tipo_posizione *, int, int, int);
void Zobrist_alto_tagliato (tipo_posizione *, int, int);
void Zobrist_basso (uint64, int, int, int);
void Zobrist_alto (uint64, int, int);
void Zobrist_preciso (tipo_posizione *, int, int, int, int);

void ingresso (tipo_posizione *);

void storia_reset ();
void guadagno_reset ();

void fare (tipo_posizione *, uint32);
void annullare (tipo_posizione *, uint32);
void fare_bianco (tipo_posizione *, uint32);
void annullare_bianco (tipo_posizione *, uint32);
void fare_nero (tipo_posizione *, uint32);
void annullare_nero (tipo_posizione *, uint32);

void inizializzazione_valore ();

uint32 prossimo_bianco (tipo_posizione *,tipo_prossimo *);
uint32 prossimo_nero (tipo_posizione *,tipo_prossimo *);

logico garantire_bianco (tipo_posizione *,uint32);
logico garantire_nero (tipo_posizione *,uint32);

void pedone_inizializzazione ();
void valutazione_pedone (tipo_posizione *, tipo_pedone_valu *);

int pv_cerca_fine_bianco (tipo_posizione *,int, int, int);
int pv_cerca_fine_scacco_bianco (tipo_posizione *,int, int, int);
int pv_cerca_fine_nero (tipo_posizione *,int, int, int);
int pv_cerca_fine_scacco_nero (tipo_posizione *,int, int, int);

void cerca_alto_bianco (tipo_posizione *);
void cerca_alto_nero (tipo_posizione *);
int nodo_superiore_bianco (tipo_posizione *, int, int, int);
int nodo_superiore_nero (tipo_posizione *, int, int, int);

int nodo_pv_bianco (tipo_posizione *, int, int, int, int);
int nodo_pv_nero (tipo_posizione *, int, int, int, int);

int escludo_bianco (tipo_posizione *, int, int, uint32);
int escludo_scacco_bianco (tipo_posizione *, int, int, uint32);
int escludo_nero (tipo_posizione *, int, int, uint32);
int escludo_scacco_nero (tipo_posizione *, int, int, uint32);

int nodo_tagliato_bianco (tipo_posizione *, int, int);
int nodo_tagliato_nero (tipo_posizione *, int, int);
int nodo_tagliato_scacco_bianco (tipo_posizione *, int, int);
int nodo_tagliato_scacco_nero (tipo_posizione *, int, int);

int nodo_totale_bianco (tipo_posizione *, int, int);
int nodo_totale_nero (tipo_posizione *, int, int);
int nodo_totale_scacco_bianco (tipo_posizione *, int, int);
int nodo_totale_scacco_nero (tipo_posizione *, int, int);

int cerca_mezzo_bianco (tipo_posizione *, int, int);
int cerca_mezzo_nero (tipo_posizione *, int, int);
int cerca_mezzo_scacco_bianco (tipo_posizione *, int, int);
int cerca_mezzo_scacco_nero (tipo_posizione *, int, int);

int cerca_fine_bianco (tipo_posizione *, int, int);
int cerca_fine_nero (tipo_posizione *, int, int);
int cerca_fine_scacco_bianco (tipo_posizione *, int, int);
int cerca_fine_scacco_nero (tipo_posizione *, int, int);

void uscita_mossa_migliore ();
void cerca (tipo_posizione *);
void informazione (tipo_posizione *, uint64, int, int, int);

logico SEE_bianca (tipo_posizione *, uint32);
logico SEE_nera (tipo_posizione *, uint32);

char *legge_fen (tipo_posizione *, char *);
void posizionale_inizializzazione (tipo_posizione *, char *);

void statico_inizializzazione ();
void pedone_reset ();

#ifdef YUSUF_MULTICORE
void rp_init();
void ivan_init_smp();
void ivan_end_smp();
void sp_init();
#endif

void ERRORE (char *, ...);
void ERRORE_fen (char *, ...);
void TRASMISSIONE (char *, ...);
char *mossa_notazione (uint32, char *);
uint64 orologio ();
void bitbordo_inizializzazione (tipo_posizione *);
void partita_nuova (tipo_posizione *, logico);
logico interrogativo_ingresso ();

logico RobboFare (tipo_posizione *);
void iniz_robbo_utilita (logico);
void total_iniz ();
logico TRIPLE_VALU (tipo_posizione *, int *);
void INIZ_ROBBO_TRIPLE ();
void triple_statistica ();

logico IVAN_SPLIT (tipo_posizione *, tipo_prossimo *, int, int, int, int, int *);
void ivan_fail_high(SPLITPUNKT *, tipo_posizione *, uint32);

void ivan_smp_pv_node_white (tipo_posizione *);
void ivan_smp_pv_node_black (tipo_posizione *);
void ivan_smp_all_node_white (tipo_posizione *);
void ivan_smp_all_node_black (tipo_posizione *);
void ivan_smp_cut_node_white (tipo_posizione *);
void ivan_smp_cut_node_black (tipo_posizione *);

int check_num_CPU();
