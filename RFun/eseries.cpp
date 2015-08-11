/*
    RFun - having fun with resistors
    written 2015 by theageman <theageman@gmx.de>

    FILE:      eseries.cpp
    PURPOSE:   implementation-file for eseries.h
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

#include "eseries.h"
#include "math.h"
//#define DEBUG
#ifdef DEBUG
#include <QtDebug>
#endif

double esGetEValue(int index, E_Series es)
{
   if (index>=0 && index<esSize[es])
   {
      switch(es)
      {
         case esE3:   return esaE3[index];
         case esE6:   return esaE6[index];
         case esE12:  return esaE12[index];
         case esE24:  return esaE24[index];
         case esE48:  return esaE48[index];
         case esE96:  return esaE96[index];
         case esE192: return esaE192[index];
         default: return 0.0;
      }
   }
#ifdef DEBUG
   else
      qDebug() << "esGetEValue: index out of bounds!";
#endif
   return 0.0;
}

double esLogBaseX(double val, double base)
{
   return log(val)/log(base);
}

double esResistorToDouble(esResistor r)
{
#ifdef DEBUG
   if (r.expo<0 || r.expo>12)
      qDebug() << "esResistorToDouble: r.expo out of bounds!";
   if (r.index<0 || r.index>=esSize[r.es])
      qDebug() << "esResistorToDouble: r.index out of bounds!";
#endif
   return esRExpo[r.expo] * esGetEValue(r.index, r.es);
}


esResistor esGetNearestResistor(double value, E_Series es)
{
   esResistor v1, v2, v3;
   double y1, y2, y3;

   if (value<esMINR || value>esMAXR)
   {
      v1.es=esNone;
      v1.expo=0;
      v1.index=-1;
#ifdef DEBUG
      qDebug() << "esGetNearestResistor: value out of limits!";
#endif
      return v1;
   }

   // calc v2:
   v2.es=es;
   v2.expo = (int)(floor(log10(value)+3.0));
   v2.index = (int)(round(esLogBaseX(value/esRExpo[v2.expo], esCFactor[es])));
   if (v2.index>=esSize[es])
   {
      v2.expo++;
      v2.index=0;
   }
   if (v2.index<0)
   {
      v2.expo--;
      v2.index=esSize[es]-1;
   }

   // get predecessor and successor:
   v1 = esGetPrevVal(v2);
   v3 = esGetNextVal(v2);

   // calc errors
   y1 = fabs(value - esResistorToDouble(v1));
   y2 = fabs(value - esResistorToDouble(v2));
   y3 = fabs(value - esResistorToDouble(v3));

   // choose the one with smallest error:
   if (y1<y2)
   {
      if (y1<y3)
         return v1;
      else
         return v3;
   }
   else
   {
      if (y2<y3)
         return v2;
      else
         return v3;
   }
}

esResistor esGetNearestMinResistor(double value, E_Series es)
{
   esResistor result = esGetNearestResistor(value, es);
   if (esResistorToDouble(result)<value-0.000001)
      result = esGetNextVal(result);
   return result;
}

esResistor esGetNearestMaxResistor(double value, E_Series es)
{
   esResistor result = esGetNearestResistor(value, es);
   if (esResistorToDouble(result)>value+0.000001)
      result = esGetPrevVal(result);
   return result;
}

esResistor esGetNextVal(esResistor r)
{
   esResistor result = r;
   result.index++;
   if (result.index>=esSize[result.es])
   {
      result.expo++;
      result.index=0;
   }
#ifdef DEBUG
   if (result.index<0 || result.index>esSize[r.es])
      qDebug() << "esGetNextVal: result.index out of bounds!";
   if (result.expo<0 || result.expo>12)
      qDebug() << "esGetNextVal: result.expo out of bounds!";
#endif
   return result;
}

esResistor esGetPrevVal(esResistor r)
{
   esResistor result = r;
   result.index--;
   if (result.index<0)
   {
      result.expo--;
      result.index = esSize[result.es]-1;
   }
#ifdef DEBUG
   if (result.index<0 || result.index>esSize[r.es])
      qDebug() << "esGetPrevVal: result.index out of bounds!";
   if (result.expo<0 || result.expo>12)
      qDebug() << "esGetPrevVal: result.expo out of bounds!";
#endif
   return result;
}

double ensureRange(double val, double min, double max)
{
   double result = val;
   if (result<min)
      result=min;
   if (result>max)
      result=max;
   return result;
}

double sign(double val)
{
   return val<0.0 ? -1.0 : 1.0;
}

QString esResistorToString(esResistor r)
{
   return esResistorToString(esResistorToDouble(r), r.es);
}

QString esResistorToString(double val, E_Series es)
{
   int idx, dig, pos, precision;
   double sg;
   QString result = "";

   idx = 0;
   precision = 3;
   if (fabs(val)<1e-26)
      result = "0 ";
   else
   {
      sg = sign(val);
      val = fabs(val);
      idx = (int)ensureRange(round((log10(val)-1.5)/3.0), -8.0, 8.0) + 8;
      val = val * powf(10, -3.0*(idx-8));
      dig = fmax(0, precision-abs(round(log10(val)+0.5)));
      result.setNum(val*sg, 'f', dig);
      pos = result.indexOf('.');
      if (pos<0)
         pos = result.indexOf(',');
      if (pos>=0)
         result[pos] = esOOM[idx];
      else
         result += esOOM[idx];
      if (es>=esE3 && es<=esE24)
      {
         // only 2 significant digits...
         pos = result.indexOf(esOOM[idx]);
         while (pos<result.length() &&
                result.length()>3 &&
                result[result.length()-1]=='0')
         {
            result = result.left(result.length()-1);
            pos = result.indexOf(esOOM[idx]);
         }
      }
      else
      {
         // only 3 significant digits...
         pos = result.indexOf(esOOM[idx]);
         while (pos<result.length() &&
                result.length()>4 &&
                result[result.length()-1]=='0')
         {
            result = result.left(result.length()-1);
            pos = result.indexOf(esOOM[idx]);
         }

      }
   }
   // special cases:
   if (result=="1000m")
      result="1R0";
   return result;
}
