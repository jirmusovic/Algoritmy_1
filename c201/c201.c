/* c201.c **********************************************************************
** Téma: Jednosměrný lineární seznam
**
**                     Návrh a referenční implementace: Petr Přikryl, říjen 1994
**                                          Úpravy: Andrea Němcová listopad 1996
**                                                   Petr Přikryl, listopad 1997
**                                Přepracované zadání: Petr Přikryl, březen 1998
**                                  Přepis do jazyka C: Martin Tuček, říjen 2004
**                                              Úpravy: Kamil Jeřábek, září 2020
**                                                    Daniel Dolejška, září 2021
**                                                    Daniel Dolejška, září 2022
**
** Implementujte abstraktní datový typ jednosměrný lineární seznam.
** Užitečným obsahem prvku seznamu je celé číslo typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou typu List.
** Definici konstant a typů naleznete v hlavičkovém souboru c201.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ List:
**
**      List_Dispose ....... zrušení všech prvků seznamu,
**      List_Init .......... inicializace seznamu před prvním použitím,
**      List_InsertFirst ... vložení prvku na začátek seznamu,
**      List_First ......... nastavení aktivity na první prvek,
**      List_GetFirst ...... vrací hodnotu prvního prvku,
**      List_DeleteFirst ... zruší první prvek seznamu,
**      List_DeleteAfter ... ruší prvek za aktivním prvkem,
**      List_InsertAfter ... vloží nový prvek za aktivní prvek seznamu,
**      List_GetValue ...... vrací hodnotu aktivního prvku,
**      List_SetValue ...... přepíše obsah aktivního prvku novou hodnotou,
**      List_Next .......... posune aktivitu na další prvek seznamu,
**      List_IsActive ...... zjišťuje aktivitu seznamu.
**
** Při implementaci funkcí nevolejte žádnou z funkcí implementovaných v rámci
** tohoto příkladu, není-li u dané funkce explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam předá
** někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c201.h"

#include <stdio.h> // printf
#include <stdlib.h> // malloc, free

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě. Nastaví error_flag na logickou 1.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void List_Error() {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = TRUE;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným
 * seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
 * že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu jednosměrně vázaného seznamu
 */
void List_Init( List *list ) {
	list->activeElement = NULL;		
	list->firstElement = NULL;		// Inicializace prvního a aktivního elementu - vložení nulové hodnoty
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam list do stavu, v jakém se nacházel
 * po inicializaci. Veškerá paměť používaná prvky seznamu list bude korektně
 * uvolněna voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu jednosměrně vázaného seznamu
 **/
void List_Dispose( List *list ) {
	for(; list->firstElement != NULL; list->firstElement = list->activeElement){    // Dokud nebudeme na posledním prvku (ten odkazuje na null), 
		list->activeElement = list->firstElement->nextElement;						// 					přidělujeme prvnímu prvku aktivní prvek
		free(list->firstElement);													// Mazání prvků od prvního prvku
	}
	list->activeElement = NULL;														// Vrací do stavu po inicializaci
}

/**
 * Vloží prvek s hodnotou data na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci List_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu jednosměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void List_InsertFirst( List *list, int data ) {
	ListElementPtr tmp = malloc(sizeof(struct ListElement));	// Alokace paměti pro nový prvek
		if(tmp == NULL){										// Nutné ověření alokace paměti
			List_Error();
		} 
	tmp->data = data;											// Vložení hodnoty data do dočasné peoměnné
	tmp->nextElement = list->firstElement;						// Přesunutí prvního prvku na další - děláme místo pro nový prvek
	list->firstElement = tmp;									// Vložení hodnoty data na místo nového prvního prvku
}

/**
 * Nastaví aktivitu seznamu list na jeho první prvek.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 * @param list Ukazatel na inicializovanou strukturu jednosměrně vázaného seznamu
 */
void List_First( List *list ) {
	list->activeElement = list->firstElement;	// Nastavení aktivity seznamu na první prvek
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci List_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu jednosměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void List_GetFirst( List *list, int *dataPtr ) {
	if(list->firstElement != NULL){			   // Ověření, zda je prvek aktivní
		*dataPtr = list->firstElement->data;   // Dereferenční přiřazení hodnoty dat z prvního prvku
	} else {								   // List je prázdný
		List_Error();
	}
}

/**
 * Zruší první prvek seznamu list a uvolní jím používanou paměť.
 * Pokud byl rušený prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu jednosměrně vázaného seznamu
 */
void List_DeleteFirst( List *list ) {
	if(list->firstElement != NULL){								// Pouze aktivní seznam
		ListElementPtr tmp;								
		tmp = list->firstElement;								// Uložení prvního prvku do dočasné proměnné
		list->firstElement = list->firstElement->nextElement;   // Posunutí prvního prvku na další prvek
		free(tmp);												// Smazání původního prvního prvku
	}
	
}

/**
 * Zruší prvek seznamu list za aktivním prvkem a uvolní jím používanou paměť.
 * Pokud není seznam list aktivní nebo pokud je aktivní poslední prvek seznamu list,
 * nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu jednosměrně vázaného seznamu
 */
void List_DeleteAfter( List *list ) {
	struct ListElement *tmp;
	if(list->activeElement != NULL){								// Ověření, zda je seznam aktivní
		if(list->activeElement->nextElement != NULL){				// Zjistíme, zda je co rušit
			tmp = list->activeElement->nextElement;					// Nastavení ukazatele na prvek, který chceme zrušit
			list->activeElement->nextElement = tmp->nextElement;	// Přesunutí na další prvek
			free(tmp);												// Uvolnění paměti užívanou rušeným prvkem
		}
	}
}

/**
 * Vloží prvek s hodnotou data za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje!
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * zavolá funkci List_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu jednosměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void List_InsertAfter( List *list, int data ) {
	if(list->activeElement != NULL){									// Ověření, zda je seznam aktivní
		struct ListElement *tmp = malloc(sizeof(struct ListElement));	// Alokace paměti pro nový prvek
			if(tmp == NULL){											// Nutné ověření správné alokace
				List_Error();
			}
		tmp->data = data;												// Vložení hodnoty data do dočasné peoměnné
		tmp->nextElement = list->activeElement->nextElement;			
		list->activeElement->nextElement = tmp;							// Vložení nového prvku za aktivní prvek
	}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam není aktivní, zavolá funkci List_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu jednosměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void List_GetValue( List *list, int *dataPtr ) {
	if(list->activeElement != NULL){			// Pouze pro aktivní seznam
		*dataPtr = list->activeElement->data;   // Dereferenční vložení hodnoty aktivního prvku do parametru
	} else {
		List_Error();
	}
}

/**
 * Přepíše data aktivního prvku seznamu list hodnotou data.
 * Pokud seznam list není aktivní, nedělá nic!
 *
 * @param list Ukazatel na inicializovanou strukturu jednosměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void List_SetValue( List *list, int data ) {
	if(list->activeElement != NULL){		// Pouze pro aktivní seznam
		list->activeElement->data = data;   // Přepsání dat aktivního prvku
	}
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Všimněte si, že touto operací se může aktivní seznam stát neaktivním.
 * Pokud není předaný seznam list aktivní, nedělá funkce nic.
 *
 * @param list Ukazatel na inicializovanou strukturu jednosměrně vázaného seznamu
 */
void List_Next( List *list ) {
	if(list->activeElement != NULL){								// Pouze pro aktivní seznam
		list->activeElement = list->activeElement->nextElement;		// Posunutí aktivity na další prvek
	}
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Tuto funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu jednosměrně vázaného seznamu
 */
int List_IsActive( List *list ) {
	return list->activeElement != NULL;  // Porovnání s nulovou hodnotou, vrací 1, pokud je list aktivní, jinak 0
}

/* Konec c201.c */
