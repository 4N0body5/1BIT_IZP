## Projekt 1 - Práca s textom


Popis projektu
Cieľom projektu bolo vytvoriť program, který bude implementovat základné operácie tabulkových procesorov. Vstupem programu sú textové dáta, operacie sa zadavajú prostredníctvom argumentov na príkazovom riadku a svoj výsledok vypisuje na výstup.

Preklad:

`$ gcc -std=c99 -Wall -Wextra -Werror sheet.c -o sheet`

Syntax spustenia:

`./sheet [-d DELIM] [Príkazy pre úpravu tabuľky]`

`./sheet [-d DELIM] [Selekcia riadkov] [Príkaz pre zpracovanie dát]`

Príkazy pre úpravu tabuľky:
* `irow R` - insert row before R
* `arow` - append row
* `drow R` - delete row R
* `drows N M` - delete rows N - M
* `icol C` - insert column before C
* `acol` - append column
* `dcol C` - delete column C
* `dcols N M` - delete columns N - M

Príkazy pre zpracovanie dát:
* `cset C STR` - in column C set STR
* `tolower C` - in column C set to lower case
* `toupper C` - in column C set to upper case
* `round C` - in column C round the number
* `int C` - in column C make integer from the number
* `copy N M` - copy from column N and overwrite column M
* `swap N M` - swap betwee N and M
* `move N M` - move N before M.
* `csum C N M` - sum from columns N - M and save in column C
* `cavg C N M` - average from columns N - M and save in column C
* `cmin C N M` - min from columns N - M and save in column C
* `cmax C N M` - max from columns N - M and save in column C
* `ccount C N M` - sum from columns N - M and save in column C of non-empty cells
* `cseq N M B` - sequence of numbers beginning with B in columns N - M

* `rseq C N M B` - sequence of numbers beginning with B in rows N - M for column C
* `rsum C N M` - sum from column C and rows N - M and save in column C row M+1
* `ravg C N M` - average from column C and rows N - M and save in column C row M+1
* `rmin C N M` - min from column C and rows N - M and save in column C row M+1
* `rmax C N M` - max from column C and rows N - M and save in column C row M+1
* `rcount C N M` - sum from column C and rows N - M and save in column C row M+1 of non-empty cells
