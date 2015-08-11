/*
    RFun - having fun with resistors
    written 2015 by theageman <theageman@gmx.de>

    FILE:      cresistor.h
    PURPOSE:   CResistor class header file
    REVISION:  1.0
    BUGS:      haven't found any, yet
    NOTES:     -
    TODOS:     -
    QUESTIONS: (1) We need to investigate the size of enums in gcc. Can we have
                   influence on the size of enums, like in Delphi?
               (2) What about 8/16byte data-alignment? (movups/movaps)
               (3) Is there a "packed"-statement for structs in c++?

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

#ifndef CRESISTOR_H
#define CRESISTOR_H
#include "eseries.h"

#define rEpsilon 0.000001 // 1 micro-ohm

class CResistor
{
public:
   // CONSTRUCTORS:
   CResistor();
   CResistor(const double &value, const E_Series &es=esE12);
   CResistor(const esResistor &r);
   CResistor(const CResistor &r);
   // destructor not needed (no dynamic data)


   // OPERATORS:
   // assignment:
   void operator=(const CResistor &r);

   // unary:
   // NOTE: increment & decrement operators:
   // if mResistor.es is esNone, resistor will be in/decremented by 1.0 ohm (!)
   // In all others cases, resistor will be in/decremented to the next
   // available value within the given e-series!
   CResistor& operator++();    // prefix increment
   CResistor& operator++(int); // postfix increment
   CResistor& operator--();    // prefix decrement
   CResistor& operator--(int); // postfix decrement

   // binary:
   CResistor& operator+=(const CResistor &r); // serial circuit
   CResistor& operator+=(const double    &r); // serial circuit
   CResistor& operator-=(const CResistor &r); // subtracting resistors...
   CResistor& operator-=(const double    &r); // ...somehow strange ;)
   CResistor& operator|=(const CResistor &r); // parallel circuit
   CResistor& operator|=(const double    &r); // parallel circuit
   CResistor& operator*=(const double    &m); // multiply with a scalar
   CResistor& operator/=(const double    &d); // divide by a scalar
   inline CResistor const operator+ (const CResistor &r) { return *this+=r; }
   inline CResistor const operator+ (const double    &r) { return *this+=r; }
   inline CResistor const operator- (const CResistor &r) { return *this-=r; }
   inline CResistor const operator- (const double    &r) { return *this-=r; }
   inline CResistor const operator| (const CResistor &r) { return *this|=r; }
   inline CResistor const operator| (const double    &r) { return *this|=r; }
   inline CResistor const operator||(const CResistor &r) { return *this|=r; }
   inline CResistor const operator||(const double    &r) { return *this|=r; }
   inline CResistor const operator* (const double    &m) { return *this*=m; }
   inline CResistor const operator/ (const double    &d) { return *this/=d; }

   // relational:
   bool operator<(const CResistor &r);
   bool operator>(const CResistor &r);
   bool operator<=(const CResistor &r);
   bool operator>=(const CResistor &r);
   bool operator==(const CResistor &r);
   bool operator!=(const CResistor &r);
   bool operator<(const double &r);
   bool operator>(const double &r);
   bool operator<=(const double &r);
   bool operator>=(const double &r);
   bool operator==(const double &r);
   bool operator!=(const double &r);

   // GETTER:
   QString  toString();
   double   value();
   double   nomValue();
   E_Series getESeries();
   int      getEIndex();
   int      getEExponent();

   // SETTER:
   void setValue(const double &value, const E_Series &es=esE12);

private:
   // data:
   esResistor mResistor; // e-series-related data  (8 bytes + sizeof(E_Series))
   double     mValue;    // actual value           (8 bytes)
   double     mNomValue; // nominal (wanted) value (8 bytes)

   // code:
   void inc();
   void dec();
   void add(const CResistor &r);
   void sub(const CResistor &r);
   void add(const double &r);
   void sub(const double &r);
};


#endif // CRESISTOR_H

