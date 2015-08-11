/*
    RFun - having fun with resistors
    written 2015 by theageman <theageman@gmx.de>

    FILE:      cresistor.cpp
    PURPOSE:   CResistor class implementation file
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

#include "cresistor.h"
#include "eseries.h"
#include <math.h>


// constructors:

CResistor::CResistor()
{
   mResistor.expo=8;
   mResistor.index=0;
   mResistor.es=esE12;
   mValue = esResistorToDouble(mResistor);
   mNomValue = mValue;
}

CResistor::CResistor(const double &value, const E_Series &es)
{
   CResistor();
   setValue(value, es);
}

CResistor::CResistor(const esResistor &r)
{
   CResistor();
   mResistor = r;
   mValue = esResistorToDouble(mResistor);
   mNomValue = mValue;
}

CResistor::CResistor(const CResistor &r)
{
   mResistor = r.mResistor;
   mValue=r.mValue;
   mNomValue=r.mNomValue;
}


// overloaded operators:

void CResistor::operator=(const CResistor &r)
{
   mResistor = r.mResistor;
   mValue = r.mValue;
   mNomValue = r.mNomValue;
}

CResistor& CResistor::operator++() // prefix increment
{
   // increment
   inc();

   // return object
   return *this;
}

CResistor& CResistor::operator++(int) // postfix increment
{
   CResistor& r(*this); // save old resistor (make a copy)
   inc(); // inc this
   return r; // return old resistor
}

CResistor& CResistor::operator--() // prefix decrement
{
   dec();
   return *this;
}

CResistor& CResistor::operator--(int) // postfix decrement
{
   CResistor& r(*this);
   dec();
   return r;
}

CResistor& CResistor::operator+=(const CResistor &r)
{
   add(r);
   return *this;
}

CResistor& CResistor::operator+=(const double &r)
{
   add(r);
   return *this;
}

CResistor& CResistor::operator-=(const CResistor &r)
{
   sub(r);
   return *this;
}

CResistor& CResistor::operator-=(const double &r)
{
   sub(r);
   return *this;
}

CResistor& CResistor::operator|=(const CResistor &r)
{
   mNomValue = (mNomValue * r.mNomValue) / (mNomValue + r.mNomValue);
   mResistor = esGetNearestResistor(mNomValue, mResistor.es);
   mValue = esResistorToDouble(mResistor);
   return *this;
}

CResistor& CResistor::operator|=(const double &r)
{
   mNomValue = (mNomValue * r) / (mNomValue + r);
   mResistor = esGetNearestResistor(mNomValue, mResistor.es);
   mValue = esResistorToDouble(mResistor);
   return *this;
}

CResistor& CResistor::operator*=(const double &m)
{
   mNomValue = mNomValue * m;
   mResistor = esGetNearestResistor(mNomValue, mResistor.es);
   mValue = esResistorToDouble(mResistor);
   return *this;
}

CResistor& CResistor::operator/=(const double &d)
{
   double divisor = fmax(rEpsilon, fabs(d));
   mNomValue = mNomValue / divisor;
   mResistor = esGetNearestResistor(mNomValue, mResistor.es);
   mValue = esResistorToDouble(mResistor);
   return *this;
}

bool CResistor::operator<(const CResistor &r)
{
   return mValue<r.mValue;
}

bool CResistor::operator>(const CResistor &r)
{
   return mValue>r.mValue;
}

bool CResistor::operator<=(const CResistor &r)
{
   return mValue<=r.mValue;
}

bool CResistor::operator>=(const CResistor &r)
{
   return mValue>=r.mValue;
}

bool CResistor::operator==(const CResistor &r)
{
   return fabs(mValue-r.mValue)<rEpsilon;
}

bool CResistor::operator!=(const CResistor &r)
{
   return fabs(mValue-r.mValue)>=rEpsilon;
}

bool CResistor::operator<(const double &r)
{
   return mValue<r;
}

bool CResistor::operator>(const double &r)
{
   return mValue>r;
}

bool CResistor::operator<=(const double &r)
{
   return mValue<=r;
}

bool CResistor::operator>=(const double &r)
{
   return mValue>=r;
}

bool CResistor::operator==(const double &r)
{
   return fabs(mValue-r)<rEpsilon;
}

bool CResistor::operator!=(const double &r)
{
   return fabs(mValue-r)>=rEpsilon;
}



// getter

QString CResistor::toString()
{
   return esResistorToString(mValue, mResistor.es);
}

double CResistor::value()
{
   return mValue;
}

double CResistor::nomValue()
{
   return mNomValue;
}

E_Series CResistor::getESeries()
{
   return mResistor.es;
}

int CResistor::getEIndex()
{
   return mResistor.index;
}

int CResistor::getEExponent()
{
   return mResistor.expo;
}


// setter

void CResistor::setValue(const double &value, const E_Series &es)
{
   mNomValue = value;
   mResistor.es=es;
   if (es==esNone)
   {
      mResistor.index=-1;
      mResistor.expo=0;
      mValue=value;
   }
   else
   {
      mResistor = esGetNearestResistor(value, es);
      mValue = esResistorToDouble(mResistor);
   }
}


// private methods

void CResistor::inc()
{
   if (mResistor.es!=esNone)
   {
      // get next available value:
      mResistor = esGetNextVal(mResistor);
      mValue = esResistorToDouble(mResistor);
      // what to do with nominal val?
      mNomValue = mValue;
   }
   else
   {
      // increment by 1.0
      mNomValue += 1.0;
      mValue = mNomValue;
   }
}

void CResistor::dec()
{
   if (mResistor.es!=esNone)
   {
      // get previous available value:
      mResistor = esGetPrevVal(mResistor);
      mValue = esResistorToDouble(mResistor);
      // what to do with nominal val?
      mNomValue = mValue;
   }
   else
   {
      // decrement by 1.0
      mNomValue -= 1.0;
      mValue = mNomValue;
   }
}

void CResistor::add(const CResistor &r)
{
   mNomValue += r.mNomValue;
   mResistor = esGetNearestResistor(mNomValue, mResistor.es);
   mValue = esResistorToDouble(mResistor);
}

void CResistor::sub(const CResistor &r)
{
   if (mNomValue>r.mNomValue)
      mNomValue -= r.mNomValue;
   else
      mNomValue = esMINR;
   mResistor = esGetNearestResistor(mNomValue, mResistor.es);
   mValue = esResistorToDouble(mResistor);
}

void CResistor::add(const double &r)
{
   mNomValue += r;
   mResistor = esGetNearestResistor(mNomValue, mResistor.es);
   mValue = esResistorToDouble(mResistor);
}

void CResistor::sub(const double &r)
{
   if (mNomValue>r)
      mNomValue -= r;
   else
      mNomValue = esMINR;
   mResistor = esGetNearestResistor(mNomValue, mResistor.es);
   mValue = esResistorToDouble(mResistor);
}





