/*
    RFun - having fun with resistors
    written 2015 by theageman <theageman@gmx.de>

    FILE:      eseries.h
    PURPOSE:   types/consts etc. regarding E-Series (E3..E192)
    REVISION:  1.0
    BUGS:      haven't found any, yet
    NOTES:     -
    TODOS:     -
    QUESTIONS: -
    HISTORY:   07/25/2015: first version

    Please note:
    This is my first C++-project since ~22 years (i'm a Delphi/Asm-Coder).
    Every experienced C++ programmer will probably find unefficient/unelegant/
    unreliable/ugly/dangerous or whatever code in these files.
    As this is open source, please feel free to enhance this project.
    Have fun,
    Olli - <theageman@gmx.de>

    Disclaimer of Warranty
    THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE
    LAW. EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR
    OTHER PARTIES PROVIDE THE PROGRAM “AS IS” WITHOUT WARRANTY OF ANY KIND,
    EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE
    ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.
    SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY
    SERVICING, REPAIR OR CORRECTION.

    Limitation of Liability
    IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING WILL
    ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MODIFIES AND/OR CONVEYS THE
    PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY
    GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE
    USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED TO LOSS OF
    DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD
    PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER PROGRAMS),
    EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGES.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ESERIES
#define ESERIES

#include <QString>

// definition of existing e-series (as of wikipedia):
typedef enum E_Series_e {
   esE3,
   esE6,
   esE12,
   esE24,
   esE48,
   esE96,
   esE192,
   esNone
} E_Series;

// name-strings of the above definition:
const QString E_SeriesName[] = {
   "E3",
   "E6",
   "E12",
   "E24",
   "E48",
   "E96",
   "E192",
   ""
};

// array-of-number-of-elements of the above definition:
const int esSize[] = {
   3,
   6,
   12,
   24,
   48,
   96,
   192,
   0
};

// the actual e-series:
// source DE : https://de.wikipedia.org/wiki/E-Reihe
// source EN : https://en.wikipedia.org/wiki/Preferred_number#E_series:_Capacitors_and_resistors
const double esaE3[] = {
   1.0 , 2.2 , 4.7
};

const double esaE6[] = {
   1.0 , 1.5 , 2.2 , 3.3 , 4.7 , 6.8
};

const double esaE12[] = {
   1.0 , 1.2 , 1.5 , 1.8 , 2.2 , 2.7 , 3.3 , 3.9 , 4.7 , 5.6 , 6.8 , 8.2
};

const double esaE24[] = {
   1.0 , 1.1 , 1.2 , 1.3 , 1.5 , 1.6 , 1.8 , 2.0 , 2.2 , 2.4 , 2.7 , 3.0 ,
   3.3 , 3.6 , 3.9 , 4.3 , 4.7 , 5.1 , 5.6 , 6.2 , 6.8 , 7.5 , 8.2 , 9.1
};

const double esaE48[] = {
   1.00, 1.05, 1.10, 1.15, 1.21, 1.27, 1.33, 1.40, 1.47, 1.54, 1.62, 1.69,
   1.78, 1.87, 1.96, 2.05, 2.15, 2.26, 2.37, 2.49, 2.61, 2.74, 2.87, 3.01,
   3.16, 3.32, 3.48, 3.65, 3.83, 4.02, 4.22, 4.42, 4.64, 4.87, 5.11, 5.36,
   5.62, 5.90, 6.19, 6.49, 6.81, 7.15, 7.50, 7.87, 8.25, 8.66, 9.09, 9.53
};

const double esaE96[] = {
   1.00, 1.02, 1.05, 1.07, 1.10, 1.13, 1.15, 1.18, 1.21, 1.24, 1.27, 1.30,
   1.33, 1.37, 1.40, 1.43, 1.47, 1.50, 1.54, 1.58, 1.62, 1.65, 1.69, 1.74,
   1.78, 1.82, 1.87, 1.91, 1.96, 2.00, 2.05, 2.10, 2.15, 2.21, 2.26, 2.32,
   2.37, 2.43, 2.49, 2.55, 2.61, 2.67, 2.74, 2.80, 2.87, 2.94, 3.01, 3.09,
   3.16, 3.24, 3.32, 3.40, 3.48, 3.57, 3.65, 3.74, 3.83, 3.92, 4.02, 4.12,
   4.22, 4.32, 4.42, 4.53, 4.64, 4.75, 4.87, 4.99, 5.11, 5.23, 5.36, 5.49,
   5.62, 5.76, 5.90, 6.04, 6.19, 6.34, 6.49, 6.65, 6.81, 6.98, 7.15, 7.32,
   7.50, 7.68, 7.87, 8.06, 8.25, 8.45, 8.66, 8.87, 9.09, 9.31, 9.53, 9.76
};

const double esaE192[] = {
   1.00, 1.01, 1.02, 1.04, 1.05, 1.06, 1.07, 1.09, 1.10, 1.11, 1.13, 1.14,
   1.15, 1.17, 1.18, 1.20, 1.21, 1.23, 1.24, 1.26, 1.27, 1.29, 1.30, 1.32,
   1.33, 1.35, 1.37, 1.38, 1.40, 1.42, 1.43, 1.45, 1.47, 1.49, 1.50, 1.52,
   1.54, 1.56, 1.58, 1.60, 1.62, 1.64, 1.65, 1.67, 1.69, 1.72, 1.74, 1.76,
   1.78, 1.80, 1.82, 1.84, 1.87, 1.89, 1.91, 1.93, 1.96, 1.98, 2.00, 2.03,
   2.05, 2.08, 2.10, 2.13, 2.15, 2.18, 2.21, 2.23, 2.26, 2.29, 2.32, 2.34,
   2.37, 2.40, 2.43, 2.46, 2.49, 2.52, 2.55, 2.58, 2.61, 2.64, 2.67, 2.71,
   2.74, 2.77, 2.80, 2.84, 2.87, 2.91, 2.94, 2.98, 3.01, 3.05, 3.09, 3.12,
   3.16, 3.20, 3.24, 3.28, 3.32, 3.36, 3.40, 3.44, 3.48, 3.52, 3.57, 3.61,
   3.65, 3.70, 3.74, 3.79, 3.83, 3.88, 3.92, 3.97, 4.02, 4.07, 4.12, 4.17,
   4.22, 4.27, 4.32, 4.37, 4.42, 4.48, 4.53, 4.59, 4.64, 4.70, 4.75, 4.81,
   4.87, 4.93, 4.99, 5.05, 5.11, 5.17, 5.23, 5.30, 5.36, 5.42, 5.49, 5.56,
   5.62, 5.69, 5.76, 5.83, 5.90, 5.97, 6.04, 6.12, 6.19, 6.26, 6.34, 6.42,
   6.49, 6.57, 6.65, 6.73, 6.81, 6.90, 6.98, 7.06, 7.15, 7.23, 7.32, 7.41,
   7.50, 7.59, 7.68, 7.77, 7.87, 7.96, 8.06, 8.16, 8.25, 8.35, 8.45, 8.56,
   8.66, 8.76, 8.87, 8.98, 9.09, 9.20, 9.31, 9.42, 9.53, 9.65, 9.76, 9.88
};

// valid exponents for resistors:
const double esRExpo[] = {
   0.001,
   0.01,
   0.1,
   1.0,
   10.0,
   100.0,
   1000.0,
   10000.0,
   100000.0,
   1000000.0,
   10000000.0,
   100000000.0,
   1000000000.0
};

// some pre-calculated values - neccessary in:
// QString esResistorToString(double val, E_Series es):
const double esCFactor[] = {
   2.1544346900318837217592935665194,  // 10^(1/3)
   1.4677992676220695409205171148169,  // 10^(1/6)
   1.2115276586285884463586029333229,  // 10^(1/12)
   1.1006941712522095691624196247996,  // 10^(1/24)
   1.0491397291363098356862621744709,  // 10^(1/48)
   1.0242752213815922608073370159966,  // 10^(1/96)
   1.0120648306218294140409291743841   // 10^(1/192)
};

// "Order Of Magnitude"-constants:
const QChar esOOM[] = {
   'y',   // 10^-24  yocto
   'z',   // 10^-21  zepto
   'a',   // 10^-18  atto
   'f',   // 10^-15  femto
   'p',   // 10^-12  pico
   'n',   // 10^-9   nano
   'u',   // 10^-6   micro
   'm',   // 10^-3   milli
   'R',   // 10^0
   'k',   // 10^3    kilo
   'M',   // 10^6    mega
   'G',   // 10^9    giga
   'T',   // 10^12   tera
   'P',   // 10^15   peta
   'E',   // 10^18   exa
   'Z',   // 10^21   zetta
   'Y'    // 10^24   yotta
};

// complete definition of a resistor:
typedef struct {
   int expo;
   int index;
   E_Series es;
} esResistor;

// some "real-world"-constraints:
const double esMINR     = 1.0;       // 1R0 (1 milli-ohm)
const double esMAXR     = 1000000000.0;   // 1,00 G-ohm
const double esINFINITY = 1e30;           // esINFINITY >> esMAXR ;)

// some useful routines:
double     esGetEValue            (int        index, E_Series es);
double     esLogBaseX             (double     val,   double   base);
double     esResistorToDouble     (esResistor r);
esResistor esGetNearestResistor   (double     value, E_Series es);
esResistor esGetNearestMinResistor(double     value, E_Series es);
esResistor esGetNearestMaxResistor(double     value, E_Series es);
esResistor esGetNextVal           (esResistor r);
esResistor esGetPrevVal           (esResistor r);
QString    esResistorToString     (esResistor r);
QString    esResistorToString     (double     val,   E_Series es);

#endif // ESERIES

