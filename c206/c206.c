/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
** uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error() {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = TRUE;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {		// Inicializace seznamu vložením nulové hodnoty do prvního, posledního a aktivního prvku
	list->activeElement = NULL;
	list->firstElement = NULL;
	list->lastElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
	while(list->lastElement != NULL){								// Dokud není poslední prvek prázdný, prováděj:
		list->activeElement = list->lastElement;					// - nastav poslední prvek na aktivní
		list->lastElement = list->lastElement->previousElement;		// - z posledního prvku udělej předposlední
		free(list->activeElement);									// - smaž aktivní prvek (dosud byl poslední)
	}
	list->firstElement = NULL;										// Navrácení do stavu po inicializaci
	list->activeElement = NULL;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {
	DLLElementPtr tmp = malloc(sizeof(struct DLLElement));		// Alokace místa pro nový prvek
	if (tmp == NULL){											// Nutné ověření korektnosti alokace
		DLL_Error();
		return;
	}
	tmp->data = data;
	tmp->nextElement = list->firstElement;						// Následující prvek za novým nastavit na první
	tmp->previousElement = NULL;								// Předchozí nového prvku musí ukazovat na NULL
	if(list->firstElement != NULL){								// Pokud seznam již prvního měl, 
		list->firstElement->previousElement = tmp;				//	první bude ukazovat na nový prvek
	}else{
		list->lastElement = tmp;								// Jinak vložení do prázdného seznamu
	}
	list->firstElement = tmp;									// Úprava ukazatele na nový prvek
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {
	DLLElementPtr tmp = malloc(sizeof(struct DLLElement));		// Alokace místa pro nový prvek
	if(tmp == NULL){											// Nutné ověření korektnosti alokace
		DLL_Error();
		return;
	}
	tmp->data = data;
	tmp->nextElement = NULL;									// Následující nového prvku ukazuje na NULL
	tmp->previousElement = list->lastElement;					// Předchozí nového nastavit na poslední
	if(list->lastElement != NULL){								// Pokud seznam již posledního měl,
		list->lastElement->nextElement = tmp;					//	poslední bude ukazovat na nový
	}else{
		list->firstElement = tmp;								// Vložení do prázdného seznamu
	}
	list->lastElement = tmp;									// Úprava ukazatele

}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
	list->activeElement = list->firstElement;	// První prvek == aktivní prvek
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
	list->activeElement = list->lastElement;	// Poslední prvek == aktivní prvek
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {
	if(list->firstElement != NULL){				// Seznam není prázdný
		*dataPtr = list->firstElement->data;	// Do parametru vkládáme hodnotu prvního prvku
	}else{
		DLL_Error();							// Seznam je prázdný
	}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {
	if(list->lastElement != NULL){				// Seznam není prázdný
		*dataPtr = list->lastElement->data;		//  Do parametru vkládáme hodnotu posledního prvku
	}else{
		DLL_Error();							// Seznam je prázdný
	}
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
	DLLElementPtr ptr;
	if(list->firstElement != NULL){									// Seznam není prázdný
		ptr = list->firstElement;									
		if(list->activeElement == list->firstElement){				// První prvek je aktivní
			list->activeElement = NULL;								// Ruší se aktivita
		}
		if(list->firstElement == list->lastElement){				// První prvek je zároveň poslední (seznam má jediný prvek)
			list->lastElement = NULL;
			list->firstElement = NULL;
		}
		else{
			list->firstElement = list->firstElement->nextElement;	// Aktualizace začátku seznamu
			list->firstElement->previousElement = NULL;				// Předchozí prvního ukazuje na NULL
		}
		free(ptr);													// Uvolnění paměti
	}
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
	DLLElementPtr ptr;
	if(list->lastElement != NULL){										// Seznam není prázdný
		ptr = list->lastElement;
		if(list->activeElement == list->lastElement){					// Poslední prvek je aktivní
			list->activeElement = NULL;									// Ruší se aktivita
		}
		if(list->firstElement == list->lastElement){					// První prvek je zároveň poslední (seznam má jediný prvek)
			list->lastElement = NULL;
			list->firstElement = NULL;
		}
		else{
			list->lastElement = list->lastElement->previousElement;		// Aktualizace konce seznamu
			list->lastElement->nextElement = NULL;						// Následující za posledním ukazuje na NULL
		}
		free(ptr);														// Uvolnění paměti
	}
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
	if(list->activeElement != NULL){											// Pozor, ať nerušíme poslední prvek
		if(list->activeElement->nextElement != NULL){							// Je co zrušit
			DLLElementPtr tmp = list->activeElement->nextElement;				// Ukazatel na rušený prvek
			list->activeElement->nextElement = tmp->nextElement;				// Swap rušeného prvku
			if(tmp == list->lastElement){										// Rušení posledního prvku
				list->lastElement = list->activeElement;						// Z posledního udělat aktivní
			}else{																// Následující prvek za tím, co se ruší, ukazuje na aktivní prvek
				tmp->nextElement->previousElement = list->activeElement;
			}
			free(tmp);															// Uvolnění paměti
		}																		
	}
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
	if(list->activeElement != NULL || list->firstElement != list->activeElement){ // Pozor, ať nerušíme poslední prvek
		if(list->activeElement->previousElement != NULL){						  // Je co zrušit
			DLLElementPtr tmp = list->activeElement->previousElement;			  // Ukazatel na rušený prvek
			list->activeElement->previousElement = tmp->previousElement;		  // Ukazatel na rušený prvek
			list->activeElement->nextElement = tmp->nextElement;				  // Swap rušeného prvku
			if(tmp == list->firstElement){										  // Rušení prvního prvku
				list->firstElement = list->activeElement;						  // Z prvního udělat aktivní
			}else{																  // Následující prvek před tím, co se ruší, ukazuje na aktivní prvek
				tmp->previousElement->nextElement = list->activeElement;
			}
			free(tmp);															  // Uvolnění paměti	
		}																		
	}
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {
	if(list->activeElement != NULL){									
		DLLElementPtr tmp =malloc(sizeof(struct DLLElement));			// Alokace místa pro nový prvek
			if(tmp == NULL){											// Nutné ověření akolace paměti
				DLL_Error();
				return;
			}															
		tmp->data = data;
		tmp->nextElement = list->activeElement->nextElement;			
		tmp->previousElement = list->activeElement;						// Předchozí nastavit na aktivní
		list->activeElement->nextElement = tmp;							// Navázání zleva na nový prvek
		if(list->activeElement == list->lastElement){					// Vkládání za posledního
			list->lastElement = tmp;									// Oprava ukazatele posledního
		}else{
			tmp->nextElement->previousElement = tmp;					// Navázání zprava na nového
		}
	}
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {
	if(list->activeElement != NULL){									
		DLLElementPtr tmp =malloc(sizeof(struct DLLElement));			// Alokace místa pro nový prvek
			if(tmp == NULL){											// Nutné ověření akolace paměti
				DLL_Error();
				return;
			}															
		tmp->data = data;
		tmp->nextElement = list->activeElement;							// Následující nastavit na aktivní
		tmp->previousElement = list->activeElement->previousElement;
		list->activeElement->previousElement = tmp;
		if(list->activeElement == list->firstElement){					// Vkládání před prvního
			list->firstElement = tmp;									// Oprava ukazatele prvního
		}else{
			tmp->previousElement->nextElement = tmp;					// Navázání zleva na nového
		}
	}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {
	if(list->activeElement != NULL){			// Je seznam aktivní?
		*dataPtr = list->activeElement->data;
	}else{
		DLL_Error();							// Není aktivní
	}
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {
	if(list->activeElement != NULL){		// Pouze pro aktivní seznam
		list->activeElement->data = data;	// Uložení nové hodnoty na místo původní hodnoty
	}
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
	if(list->activeElement != NULL){								// Pouze pro aktivní seznam
		list->activeElement = list->activeElement->nextElement;		// Přesunutí aktivity na následující prvek
	}
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
	if(list->activeElement != NULL){									// Pouze pro aktivní seznam
		list->activeElement = list->activeElement->previousElement;		// Přesunutí aktivity na předchozí prvek
	}
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
	return list->activeElement != NULL;		// Porovnání s NULL, vrací 1, pokud je aktivní, jinak 0
}

/* Konec c206.c */
