#ifndef SORTED_LIST_H
#define SORTED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ListNode {
	struct ListNode *urmator;
	struct ListNode *anterior;
	int prioritate;
	char *nume;
} ListNode;

typedef struct List {
	ListNode *primul;
	ListNode *ultimul;
} List;

typedef struct Coada {
	ListNode *front;
	ListNode *rear;
} Coada;

int deschise = 0, nr_deschise;
char s[100];

void adauga_persoana(List ***ghisee, char *nume, int prioritate, 
		int numar_ghiseu);

void sterge_persoana(List ***ghisee, char *nume, int prioritate, 
		int numar_ghiseu);

int gaseste_pozitie_persoana (List **ghisee, char *nume, int prioritate, 
		int numar_ghiseu);

int calculeaza_numar_total_oameni ( List **ghisee );

int primul_eliminat ( List **ghisee ) {

	int i = 0, j, k;
	ListNode* nodemax;

	while ( (*( ghisee + i ))->primul == NULL )
		i ++;
	k = i;
	nodemax = (*( ghisee + i ))->primul;

	for ( j = i; j < nr_deschise; j ++ )
		if ( (*( ghisee + j ))->primul != NULL ) {
			if ( (*( ghisee + j ))->primul->prioritate > nodemax->prioritate ) {
				nodemax = (*( ghisee + j ))->primul;
				k = j;
			}
			if ( (*( ghisee + j ))->primul->prioritate == nodemax->prioritate )
				if ( strcmp ( nodemax->nume, (*( ghisee + j ))->primul->nume ) > 0 ) {
					nodemax = (*( ghisee + j ))->primul;
					k = j;
				}
		}

	return k;

}

void sterge_prima_persoana ( List ***ghisee, int pozitie_eliminare ) {

	ListNode* sterge_ma;

	if ( (*( *ghisee + pozitie_eliminare ))->primul->urmator == NULL )
		(*( *ghisee + pozitie_eliminare ))->primul = NULL;
	else {
		sterge_ma = (*( *ghisee + pozitie_eliminare ))->primul;
		(*( *ghisee + pozitie_eliminare ))->primul =
		(*( *ghisee + pozitie_eliminare ))->primul->urmator;
		(*( *ghisee + pozitie_eliminare ))->primul->anterior = NULL;
		free ( sterge_ma );
	}

}

void deschide_ghisee( List ***ghisee, int N ) {

	int i, j = 0, nr_total_oameni = calculeaza_numar_total_oameni ( *ghisee ), pozitie_eliminare;
	ListNode* coada = ( ListNode* ) malloc ( sizeof ( ListNode ) ), *revenire_primul = coada;

	if ( N < 1 )
		return;

	if ( deschise == 0 ) {
		*ghisee = ( List** ) malloc ( N * sizeof ( List* ) );
		for ( i = 0; i < N; i ++ ) {
			*( *ghisee + i ) = ( List* ) malloc ( sizeof ( List ) );
			( *( *ghisee + i ))->primul = NULL;
			( *( *ghisee + i ))->ultimul = NULL;
		}
		nr_deschise = N;
		deschise = 1;
		return;
	}

	if ( nr_total_oameni != 0 )
		for ( i = 0; i < nr_total_oameni; i ++ ) {

			pozitie_eliminare = primul_eliminat ( *ghisee );
			coada->prioritate = (*( *ghisee + pozitie_eliminare ))->primul->prioritate;
			coada->nume = (*( *ghisee + pozitie_eliminare ))->primul->nume;
			//printf ( "%d ", coada->prioritate );
			if ( i < nr_total_oameni - 1 ) {
				coada->urmator = ( ListNode* ) malloc ( sizeof ( ListNode ) );
				coada = coada->urmator;
			}
			sterge_prima_persoana ( ghisee, pozitie_eliminare );

		}
	coada = revenire_primul;

//re-adaugare oameni la ghisee

	*ghisee = ( List** ) malloc ( sizeof ( List* ) );
	for ( i = 0; i < N; i ++ )
		*( *ghisee + i ) = ( List* ) malloc ( sizeof ( List ) );

	if ( N > nr_total_oameni ) {

		for ( i = 0; i < nr_total_oameni; i ++ ) {
			(*( *ghisee + i ))->ultimul = ( ListNode* ) malloc ( sizeof ( ListNode ) );
			(*( *ghisee + i ))->primul = (*( *ghisee + i ))->ultimul;
		}

		for ( i = 0; i < nr_total_oameni; i ++ ) {
			(*( *ghisee + i ))->ultimul->prioritate = coada->prioritate;
			(*( *ghisee + i ))->ultimul->nume = coada->nume;
			coada = coada->urmator;
		}
		nr_deschise = N;
		return;

	}

	if ( N <= nr_total_oameni ) {

		for ( i = 0; i < N; i ++ ) {
			(*( *ghisee + i ))->ultimul = ( ListNode* ) malloc ( sizeof ( ListNode ) );
			(*( *ghisee + i ))->primul = (*( *ghisee + i ))->ultimul;
		}

		for ( i = 0; i < nr_total_oameni; i ++ ) {
			if ( i >= N ) {
				(*( *ghisee + i % N ))->ultimul->urmator =
				( ListNode* ) malloc ( sizeof ( ListNode ) );
				(*( *ghisee + i % N ))->ultimul = (*( *ghisee + i % N ))->ultimul->urmator;
				(*( *ghisee + i % N ))->ultimul->urmator =
				( ListNode* ) malloc ( sizeof ( ListNode ) );
				(*( *ghisee + i % N ))->ultimul->urmator->anterior = (*( *ghisee + i % N ))->ultimul;
			}
			(*( *ghisee + i % N ))->ultimul->prioritate = coada->prioritate;
			(*( *ghisee + i % N ))->ultimul->nume = coada->nume;
			coada = coada->urmator;
		}

		nr_deschise = N;
		return;
	}

}

void inchide_ghisee(List ***ghisee) {

	int i;
	ListNode* free_n;

	for ( i = 0; i < nr_deschise; i ++ )
		while ( (*( *ghisee + i ))->primul != NULL ) {
			free_n = (*( *ghisee + i ))->primul;
			(*( *ghisee + i ))->primul = (*( *ghisee + i ))->primul->urmator;
			free ( free_n );
		}

	free ( *ghisee );
	deschise = 0;

}

void adauga_persoana(List ***ghisee, char *nume, int prioritate, 
		int numar_ghiseu) {

	ListNode* node = ( ListNode* ) malloc ( sizeof ( ListNode ) ), *revenire_primul, *revenire_ultimul;
	node->prioritate = prioritate;
	node->nume = nume;

	if ( (*( *ghisee + numar_ghiseu ))->primul == NULL ) {
		(*( *ghisee + numar_ghiseu ))->primul = node;
		(*( *ghisee + numar_ghiseu ))->ultimul = node;
		return;
	}//(1)

	if ( (*( *ghisee + numar_ghiseu ))->primul == (*( *ghisee + numar_ghiseu ))->ultimul ) {
		if ( prioritate > (*( *ghisee + numar_ghiseu ))->primul->prioritate ||
		( prioritate == (*( *ghisee + numar_ghiseu ))->primul->prioritate &&
		strcmp ( nume, (*( *ghisee + numar_ghiseu ))->primul->nume ) <= 0 ) ) {
			node->urmator = (*( *ghisee + numar_ghiseu ))->primul;
			node->anterior = NULL;
			(*( *ghisee + numar_ghiseu ))->primul->anterior = node;
			(*( *ghisee + numar_ghiseu ))->primul = node;
			return;
		}//(2.1)(2.2)

		if ( prioritate < (*( *ghisee + numar_ghiseu ))->primul->prioritate ||
		( prioritate == (*( *ghisee + numar_ghiseu ))->primul->prioritate &&
		strcmp ( nume, (*( *ghisee + numar_ghiseu ))->primul->nume ) >= 0 ) ) {
			node->urmator = NULL;
			node->anterior = (*( *ghisee + numar_ghiseu ))->ultimul;
			(*( *ghisee + numar_ghiseu ))->ultimul->urmator = node;
			(*( *ghisee + numar_ghiseu ))->ultimul = node;
			return;
		}//(2.3)(2.4)
	}//(2)

	if ( (*( *ghisee + numar_ghiseu ))->primul != (*( *ghisee + numar_ghiseu ))->ultimul ) {
		if ( (*( *ghisee + numar_ghiseu ))->primul->prioritate ==
		(*( *ghisee + numar_ghiseu ))->ultimul->prioritate ) {
			if ( (*( *ghisee + numar_ghiseu ))->primul->prioritate < prioritate ) {
				node->urmator = (*( *ghisee + numar_ghiseu ))->primul;
				node->anterior = NULL;
				(*( *ghisee + numar_ghiseu ))->primul->anterior = node;
				(*( *ghisee + numar_ghiseu ))->primul = node;
				return;
			}//(3.1.1)
			if ( (*( *ghisee + numar_ghiseu ))->ultimul->prioritate > prioritate ) {
				node->urmator = NULL;
				node->anterior = (*( *ghisee + numar_ghiseu ))->ultimul;
				(*( *ghisee + numar_ghiseu ))->ultimul->urmator = node;
				(*( *ghisee + numar_ghiseu ))->ultimul = node;
				return;
			}//(3.1.2)
			if ( prioritate == (*( *ghisee + numar_ghiseu ))->primul->prioritate ) {
				if ( strcmp ( nume, (*( *ghisee + numar_ghiseu ))->primul->nume ) <= 0 ) {
					node->urmator = (*( *ghisee + numar_ghiseu ))->primul;
					node->anterior = NULL;
					(*( *ghisee + numar_ghiseu ))->primul->anterior = node;
					(*( *ghisee + numar_ghiseu ))->primul = node;
					return;
				}//(3.1.3.1)
				if ( strcmp ( nume, (*( *ghisee + numar_ghiseu ))->ultimul->nume ) >= 0 ) {
					node->urmator = NULL;
					node->anterior = (*( *ghisee + numar_ghiseu ))->ultimul;
					(*( *ghisee + numar_ghiseu ))->ultimul->urmator = node;
					(*( *ghisee + numar_ghiseu ))->ultimul = node;
					return;
				}//(3.1.3.2)
				if ( strcmp ( (*( *ghisee + numar_ghiseu ))->primul->nume, nume ) <= 0 &&
				strcmp ( nume, (*( *ghisee + numar_ghiseu ))->ultimul->nume ) <= 0 ) {
					revenire_primul = (*( *ghisee + numar_ghiseu ))->primul;
					while ( strcmp ( nume, (*( *ghisee + numar_ghiseu ))->primul->nume ) >=
					0 )
						(*( *ghisee + numar_ghiseu ))->primul =
						(*( *ghisee + numar_ghiseu ))->primul->urmator;

					node->urmator = (*( *ghisee + numar_ghiseu ))->primul;
					node->anterior = (*( *ghisee + numar_ghiseu ))->primul->anterior;
					(*( *ghisee + numar_ghiseu ))->primul->anterior->urmator = node;
					(*( *ghisee + numar_ghiseu ))->primul->anterior = node;
					(*( *ghisee + numar_ghiseu ))->primul = revenire_primul;
					return;
				}//(3.1.3.3)
			}//(3.1.3)
		}//(3.1)
		if ( (*( *ghisee + numar_ghiseu ))->primul->prioritate >
		(*( *ghisee + numar_ghiseu ))->ultimul->prioritate ) {
			if ( prioritate > (*( *ghisee + numar_ghiseu ))->primul->prioritate ||
			( prioritate == (*( *ghisee + numar_ghiseu ))->primul->prioritate &&
			strcmp ( nume, (*( *ghisee + numar_ghiseu ))->primul->nume ) <= 0 ) ) {
				node->urmator = (*( *ghisee + numar_ghiseu ))->primul;
				node->anterior = NULL;
				(*( *ghisee + numar_ghiseu ))->primul->anterior = node;
				(*( *ghisee + numar_ghiseu ))->primul = node;
				return;
			}//(3.2.1)(3.2.2)
			if ( prioritate < (*( *ghisee + numar_ghiseu ))->ultimul->prioritate ||
			( prioritate == (*( *ghisee + numar_ghiseu ))->ultimul->prioritate &&
			strcmp ( nume, (*( *ghisee + numar_ghiseu ))->ultimul->nume ) >= 0 ) ) {
				node->urmator = NULL;
				node->anterior = (*( *ghisee + numar_ghiseu ))->ultimul;
				(*( *ghisee + numar_ghiseu ))->ultimul->urmator = node;
				(*( *ghisee + numar_ghiseu ))->ultimul = node;
				return;
			}//(3.2.3)(3.2.4)
			if ( prioritate == (*( *ghisee + numar_ghiseu ))->primul->prioritate &&
			strcmp ( nume, (*( *ghisee + numar_ghiseu ))->primul->nume ) >= 0 ) {
				revenire_primul = (*( *ghisee + numar_ghiseu ))->primul;
				while ( prioritate == (*( *ghisee + numar_ghiseu ))->primul->prioritate &&
				strcmp ( nume, (*( *ghisee + numar_ghiseu ))->primul->nume ) >= 0 )
					(*( *ghisee + numar_ghiseu ))->primul =
					(*( *ghisee + numar_ghiseu ))->primul->urmator;
				node->urmator = (*( *ghisee + numar_ghiseu ))->primul;
				node->anterior = (*( *ghisee + numar_ghiseu ))->primul->anterior;
				(*( *ghisee + numar_ghiseu ))->primul->anterior->urmator = node;
				(*( *ghisee + numar_ghiseu ))->primul->anterior = node;
				(*( *ghisee + numar_ghiseu ))->primul = revenire_primul;
				return;
			}//(3.2.5)
			if ( prioritate == (*( *ghisee + numar_ghiseu ))->ultimul->prioritate &&
			strcmp ( nume, (*( *ghisee + numar_ghiseu ))->ultimul->nume ) <= 0 ) {
				revenire_ultimul = (*( *ghisee + numar_ghiseu ))->ultimul;
				while ( prioritate == (*( *ghisee + numar_ghiseu ))->ultimul->prioritate &&
				strcmp ( nume, (*( *ghisee + numar_ghiseu ))->ultimul->nume ) <= 0 )
					(*( *ghisee + numar_ghiseu ))->ultimul =
					(*( *ghisee + numar_ghiseu ))->ultimul->anterior;
				node->urmator = (*( *ghisee + numar_ghiseu ))->ultimul->urmator;
				node->anterior = (*( *ghisee + numar_ghiseu ))->ultimul;
				(*( *ghisee + numar_ghiseu ))->ultimul->anterior = node;
				(*( *ghisee + numar_ghiseu ))->ultimul = node;
				(*( *ghisee + numar_ghiseu ))->ultimul = revenire_ultimul;
				return;
			}//(3.2.6)
			if ( (*( *ghisee + numar_ghiseu ))->primul->prioritate > prioritate &&
			prioritate > (*( *ghisee + numar_ghiseu ))->ultimul->prioritate ) {
				revenire_primul = (*( *ghisee + numar_ghiseu ))->primul;
				while ( (*( *ghisee + numar_ghiseu ))->primul->prioritate > prioritate )
					(*( *ghisee + numar_ghiseu ))->primul =
					(*( *ghisee + numar_ghiseu ))->primul->urmator;
				if ( (*( *ghisee + numar_ghiseu ))->primul->prioritate < prioritate ) {
					node->urmator = (*( *ghisee + numar_ghiseu ))->primul;
					node->anterior = (*( *ghisee + numar_ghiseu ))->primul->anterior;
					(*( *ghisee + numar_ghiseu ))->primul->anterior->urmator = node;
					(*( *ghisee + numar_ghiseu ))->primul->anterior = node;
					(*( *ghisee + numar_ghiseu ))->primul = revenire_primul;
					return;
				}//(3.2.7.1)
				if ( (*( *ghisee + numar_ghiseu ))->primul->prioritate == prioritate ) {
					while ( (*( *ghisee + numar_ghiseu ))->primul->prioritate ==
					prioritate &&
					strcmp ( nume, (*( *ghisee + numar_ghiseu ))->primul->nume ) >= 0 )
						(*( *ghisee + numar_ghiseu ))->primul =
						(*( *ghisee + numar_ghiseu ))->primul->urmator;
					node->urmator = (*( *ghisee + numar_ghiseu ))->primul->urmator;
					node->anterior = (*( *ghisee + numar_ghiseu ))->primul;
					(*( *ghisee + numar_ghiseu ))->primul->urmator->anterior = node;
					(*( *ghisee + numar_ghiseu ))->primul->urmator = node;
					(*( *ghisee + numar_ghiseu ))->primul = revenire_primul;
					return;
				}//(3.2.7.2)
			}//(3.2.7)
		}//(3.2)
	}//(3)

}

void sterge_persoana(List ***ghisee, char *nume, int prioritate, 
		int numar_ghiseu) {

	int i, poz;
	ListNode* free_poz, *revenire_primul = (*( *ghisee + numar_ghiseu ))->primul, *node = NULL;

	if ( (*( *ghisee + numar_ghiseu ))->primul != NULL )

		if ( (*( *ghisee + numar_ghiseu ))->primul == (*( *ghisee + numar_ghiseu ))->ultimul ) {
			if ( (*( *ghisee + numar_ghiseu ))->primul->prioritate == prioritate )
				if ( strcmp ( (*( *ghisee + numar_ghiseu ))->primul->nume, nume ) == 0 ) {
					(*( *ghisee + numar_ghiseu ))->primul = NULL;
					(*( *ghisee + numar_ghiseu ))->ultimul = NULL;
				}
		}

		else {
			poz = gaseste_pozitie_persoana ( *ghisee, nume, prioritate, numar_ghiseu );
			if ( poz >= 0 ) {
				for ( i = 0; i < poz; i ++ )
					(*( *ghisee + numar_ghiseu ))->primul = (*( *ghisee + numar_ghiseu ))->primul->urmator;
				if ( poz == 0 ) {
					free_poz = (*( *ghisee + numar_ghiseu ))->primul;
					(*( *ghisee + numar_ghiseu ))->primul = (*( *ghisee + numar_ghiseu ))->primul->urmator;
					(*( *ghisee + numar_ghiseu ))->primul->anterior = NULL;
					free ( free_poz );
				}

				if ( poz > 0 && (*( *ghisee + numar_ghiseu ))->primul != (*( *ghisee + numar_ghiseu ))->ultimul ) {
					free_poz = (*( *ghisee + numar_ghiseu ))->primul;
					(*( *ghisee + numar_ghiseu ))->primul->anterior->urmator = (*( *ghisee + numar_ghiseu ))->primul->urmator;
					(*( *ghisee + numar_ghiseu ))->primul->urmator->anterior = (*( *ghisee + numar_ghiseu ))->primul->anterior;
					free ( free_poz );

					(*( *ghisee + numar_ghiseu ))->primul = revenire_primul;
				}

				if ( poz > 0 && (*( *ghisee + numar_ghiseu ))->primul == (*( *ghisee + numar_ghiseu ))->ultimul ) {
					free_poz = (*( *ghisee + numar_ghiseu ))->ultimul;
					(*( *ghisee + numar_ghiseu ))->ultimul = (*( *ghisee + numar_ghiseu ))->ultimul->anterior;
					(*( *ghisee + numar_ghiseu ))->ultimul->urmator = NULL;
					free ( free_poz );

					(*( *ghisee + numar_ghiseu ))->primul = revenire_primul;
				}
			}
		}

}

void sterge_primii_oameni(List ***ghisee) {

	int i;

	for ( i = 0; i < nr_deschise; i ++ )
		sterge_prima_persoana ( ghisee, i );

}

int calculeaza_numar_oameni_ghiseu(List **ghisee, int numar_ghiseu) {

	int nr_persoane_ghiseu = 0;
	ListNode* revenire_primul = (*( ghisee + numar_ghiseu ))->primul;

	while ( (*( ghisee + numar_ghiseu ))->primul != NULL ) {
		nr_persoane_ghiseu ++;
		(*( ghisee + numar_ghiseu ))->primul = (*( ghisee + numar_ghiseu ))->primul->urmator;
	}
	(*( ghisee + numar_ghiseu ))->primul = revenire_primul;

	return nr_persoane_ghiseu;

}

int calculeaza_numar_total_oameni(List **ghisee) {
	
	int nr_total_persoane = 0, i;
	ListNode* revenire_primul;

	if ( nr_deschise != 0 ) {
		for ( i = 0; i < nr_deschise; i ++ ) {
			revenire_primul = (*( ghisee + i ))->primul;
			while ( (*( ghisee + i ))->primul != NULL ) {
				nr_total_persoane ++;
				(*( ghisee + i ))->primul = (*( ghisee + i ))->primul->urmator;
			}
			(*( ghisee + i ))->primul = revenire_primul;
		}
		return nr_total_persoane;
	}

	return 0;

}

int gaseste_pozitie_persoana (List **ghisee, char *nume, int prioritate, 
		int numar_ghiseu) {

	int poz = 0;
	ListNode* revenire_primul = (*( ghisee + numar_ghiseu ))->primul;

	while ( (*( ghisee + numar_ghiseu ))->primul != (*( ghisee + numar_ghiseu ))->ultimul &&
	(*( ghisee + numar_ghiseu ))->primul->prioritate != prioritate ) {
			(*( ghisee + numar_ghiseu ))->primul = (*( ghisee + numar_ghiseu ))->primul->urmator;
			poz ++;
	}

	if ( (*( ghisee + numar_ghiseu ))->primul->prioritate == prioritate )
		while ( (*( ghisee + numar_ghiseu ))->primul != NULL )
			if ( (*( ghisee + numar_ghiseu ))->primul->prioritate == prioritate ) {

				if ( strcmp ( (*( ghisee + numar_ghiseu ))->primul->nume, nume ) == 0 ) {
					(*( ghisee + numar_ghiseu ))->primul = revenire_primul;
					return poz;
				}
				(*( ghisee + numar_ghiseu ))->primul = (*( ghisee + numar_ghiseu ))->primul->urmator;
				poz ++;
			}
	if ( (*( ghisee + numar_ghiseu ))->primul == (*( ghisee + numar_ghiseu ))->ultimul &&
	(*( ghisee + numar_ghiseu ))->ultimul->prioritate == prioritate &&
	strcmp ( (*( ghisee + numar_ghiseu ))->ultimul->nume, nume ) == 0 ) {

		(*( ghisee + numar_ghiseu ))->primul = revenire_primul;
		return ++poz;

	}

	return -1;
}

char* gaseste_persoana_din_fata(List **ghisee, char *nume, int prioritate, 
		int numar_ghiseu) {

	int poz = gaseste_pozitie_persoana ( ghisee, nume, prioritate, numar_ghiseu ), i, Prioritate, nr_cifre, Prioritate1;
	ListNode* revenire_primul = (*( ghisee + numar_ghiseu ))->primul;

	if ( poz == 0 || poz == -1 )
		return "Nu exista.";
	
	for ( i = 0; i < poz - 1; i ++ )
		(*( ghisee + numar_ghiseu ))->primul = (*( ghisee + numar_ghiseu ))->primul->urmator;

	Prioritate1 = Prioritate = (*( ghisee + numar_ghiseu ))->primul->prioritate;

	nr_cifre = 0;

	while ( Prioritate1 != 0 ) {
		Prioritate1 = Prioritate1 / 10;
		nr_cifre ++;
	}

	for ( i = nr_cifre - 1; i >= 0; i -- ) {
		s[i] = Prioritate % 10 + 48;
		Prioritate = Prioritate / 10;
	}

	strcat ( s, (*( ghisee + numar_ghiseu ))->primul->nume );

	return s;

}

void afiseaza_rand_ghiseu(FILE *f, List **ghisee, int numar_ghiseu) {

	char s[1000], i;

	if ( (*( ghisee + numar_ghiseu ))->primul == NULL ) {
		fprintf ( f, "Ghiseul %d nu are nicio persoana in asteptare.", numar_ghiseu );
		return;
	}

	ListNode* revenire_primul = (*( ghisee + numar_ghiseu ))->primul;

	fseek ( f, 0, SEEK_SET );

	while ( (*( ghisee + numar_ghiseu ))->primul->urmator != NULL ) {
		fseek ( f, 0, SEEK_CUR );
		i = 0;
		while ( s[i] != '\0' ) {
			s[i] = '\0';
			i ++;
		}
		strcpy ( s, (*( ghisee + numar_ghiseu ))->primul->nume );
		printf ( "%s\n", s );
		fprintf ( f, "%s", s );
		fprintf ( f, "-%d;", (*( ghisee + numar_ghiseu ))->primul->prioritate );
		(*( ghisee + numar_ghiseu ))->primul = (*( ghisee + numar_ghiseu ))->primul->urmator;
	}

	(*( ghisee + numar_ghiseu ))->primul = revenire_primul;

}

#endif
