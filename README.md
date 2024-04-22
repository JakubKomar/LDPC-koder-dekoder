# LDPC koder a dekoder

Tento projekt implementuje jednoduchou konzolovou aplikaci (bms) v jazyce C/C++, která provádí kódování a dekódování vstupní zprávy pomocí Low-density parity-check (LDPC) kódu s poměrem kódování ½.
Parametry

    -m [matica.csv]: Specifikuje soubor obsahující kontrolní matici LDPC kódu.
    -e <<< vstup=[ASCII znaky]: Slouží k zakódování vstupní zprávy.
    -d <<< vstup=[znaky hodnot jednotlivých bitů]: Slouží k dekódování vstupních dat.

#### Spuštění

Program očekává vstup ze standardního vstupu (stdin) a výstup je zobrazen na standardní výstup (stdout).
#### Kódování

Vstupní řetězec je převeden na jednotlivé bajty a zakódován pomocí LDPC.
#### Dekódování

Program očekává pouze symboly [0,1], které reprezentují jednotlivé bity přenášené zprávy. Vstup je dekódován a výsledná zpráva je zobrazena ve formátu ASCII řetězce. Dekódování je schopné úspěšně opravit i vícebitové chyby na různých pozicích.
#### Formát Matice.csv

Soubor matica.csv obsahuje kontrolní matici použitou pro kódování a dekódování. Matice musí být ve formátu .csv, kde sloupce jsou odděleny čárkou a řádky novým řádkem (\n). Pokud není tento parametr specifikován při kódování, matice je automaticky vygenerována a uložena do souboru matica.csv. Při dekódování je specifikace matice povinná a je použita pro proces dekódování.
Architektura

Jako referenční architekturu LDPC kódéra považujeme implementaci v jazyce Python pomocí knihovny pyldpc. Vaše řešení bude testováno a hodnoceno pomocí této implementace. Parametry pyldpc při testování jsou následující:

    Délka codeword = 2 * délka(vstup).
    Dc = délka(vstup).
    Dv = délka(vstup) - 1.

#### Poznámka

Při provádění kódování a dekódování jsou ignorovány veškeré bílé znaky a znaky, které nejsou povoleny. Vstupy jsou očekávány ve formě definované výše.
