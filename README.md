V první domácí úloze, kterou najdete v URL odkaze zadání (Moodle kurzu IAL > Domácí úlohy > 1. domácí úloha), na Vás čekají následující tři příklady:

C201: jednosměrně vázaný lineární seznam (až 3 body),
C203: fronta znaků v poli (až 3 body),
C206: implementace dvousměrně vázaného lineárního seznamu (až 4 body).
Vaším úkolem je doplnit těla funkcí v souborech c201.c, c203.c a c206.c a tyto tři soubory následně odevzdat prostřednictvím STUDISu (nezabalené a v kódování v UTF-8).

Správnost Vašich implementací si můžete částečně ověřit pomocí základních testovacích aplikací (např. c201-test.c), pro jejichž překlad máte předpřipraveny i soubory Makefile (make all, make clean, make run). Pozn.: do hlaviček funkcí ani do předpřipravených hlavičkových souborů, prosím, nezasahujte!

Jestliže Vaše implementace daného příkladu generuje správné výsledky pro základní testovací aplikaci, při překladu a spuštění na serveru eva.fit.vutbr.cz (správné výsledky, které najdete např. v souboru c201-test.output, jež můžete s výstupy vaší aktuální implementace snadno porovnat spuštěním make run), získáváte za příklad 1 bod. Jestliže Vaše implementace vyhoví i pokročilým testům (ty v zadání domácí úlohy nenajdete), získáváte za příklad i zbývající body (tedy 2 body za příklad C201 a C203 a 3 body za příklad C206).

Doporučuji Vám proto, abyste si zkusili základní testovací aplikaci rozšířit a odhalit tak co nejvíce chyb ve své implementaci. Případnou upravenou testovací aplikaci ale do STUDISu neodevzdávejte.

Pokud se dostanete do časové tísně, nesnažte se řešit všechny příklady najednou v naději, že za částečné řešení něco dostanete. V takovém případě je výhodnější dokončit alespoň jeden či dva příklady pořádně.

Důležitá upozornění!

Domácí úlohy budou překládány, spouštěny a hodnoceny automatickými skripty na serveru eva.fit.vutbr.cz. Nedodržení zadání (např. zásahy do kostry programu) bude mít za následek ztrátu všech bodů za domácí úlohu!

Stejně tak budou 0 body hodnoceny příklady nepřeložitelné a ty, které se nekonečně zacyklí - příklad se se zlou potáže (přesněji řečeno s kill -9), pokud bude potřebovat k řešení více jak 15s.
