/*
    RFun - having fun with resistors
    written 2015 by theageman <theageman@gmx.de>

    FILE:      csolver.h
    PURPOSE:   CRSolver class implementation file
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

#include <math.h>
#include <QDebug>
#include <QMutex>
#include "crsolver.h"
#include "eseries.h"
#include "cresistor.h"

CRSolver::CRSolver()
{
   mSignalProgress=NULL;
   mSignalSolution=NULL;
   mMutex=NULL;
   mNumTests=100;
}

CRSolver::~CRSolver()
{
   clearList();
}

void CRSolver::clearList()
{
   for (int i=0; i<mList.length(); i++)
      delete mList[i];
   mList.clear();
}

void CRSolver::initList(int length)
{
   clearList();
   for (int i=0; i<length; i++)
   {
      pEntity e = new Entity;
      e->op=roNone;
      mList.append(e);
   }
}

void CRSolver::calcNumTests(double value, E_Series es)
{
   mNumTests=0;
   esResistor r1 = esGetNearestResistor(value, es);
   esResistor r2 = esGetNearestMaxResistor(esMAXR, es);

   // count parallel combinations to tested:
   while (r1.expo!=r2.expo || r1.index!=r2.index)
   {
      mNumTests++;
      r1 = esGetNextVal(r1);
   }

   // count serial combinations to tested:
   r1 = esGetNearestMinResistor(esMINR, es);
   while (r1.expo!=r2.expo || r1.index!=r2.index)
   {
      mNumTests++;
      r1 = esGetNextVal(r1);
   }
   mCurTest = 0;
}

QString CRSolver::getMsg(double err, double v)
{
   QString result;
   result = "Value: "+QString::number(v, 'g', 12)+"; deviation: "+QString::number(err*100.0, 'g', 6)+"%";
   return result;
}

double CRSolver::findCombination(double value, E_Series es, int nResistors)
{
   initList(nResistors);
   mTogo=nResistors-1;
   Entity e;
   calcNumTests(value, es);
   mTerminate=false;
   double val = findCombinationX(value, es, nResistors-1, e, 1e30);
#ifdef DEBUG
   qDebug() << "Final solution: R=" << QString::number(val, 'f', 20);
#endif
   printList();
   mCurTest=0;
   double err=val/value-1.0;
   (mInterface->*mSignalSolution)(getMsg(err, val), true);
   return val;
}

int CRSolver::getLength()
{
   return mList.length();
}

int CRSolver::getProgress()
{
   int result = (1000*mCurTest) / mNumTests;
   return result;
}

pEntity CRSolver::getEntity(int index)
{
   if (index>=0 && index<mList.length())
      return mList[index];
   else
      return NULL;
}

void CRSolver::setProgressFunc(void (CRInterface::*func)(double))
{
   mSignalProgress=func;
}

void CRSolver::setSolutionFunc(void (CRInterface::*func)(QString, bool))
{
   mSignalSolution=func;
}

void CRSolver::setInterface(CRInterface *interface)
{
   mInterface=interface;
}

void CRSolver::setMutex(QMutex *mutex)
{
   mMutex=mutex;
}

void CRSolver::terminate()
{
   mTerminate=true;
}

double CRSolver::findCombinationX(double value, E_Series es, int togo, Entity &re, double cerr)
{
   double result = 1e30;
   pEntity e = mList[togo];
   double err, lerr;

   double v1, v, v2;
   Entity r1, r2, rb;
   lerr=cerr;

   if (togo==0)
   {
      r1.r.setValue(value, es);
      r1.op=roNone;
      v = r1.r.value();
      re=r1;
      err=v/value-1.0;
      if (mMutex)
         mMutex->lock();
      if (fabs(err)<fabs(lerr))
         *e=r1;
      if (mMutex)
         mMutex->unlock();
      return v;
   }

   // init resistors
   r1.op = roNone;
   rb.op = roNone;
   rb.r.setValue(esMINR, es);

   // check out serial combinations:
   r1.r.setValue(esMINR, es);
   v1=r1.r.value();
   while (v1<esMAXR)
   {
      // feedback
      if (togo==mTogo)
         mCurTest++;
      if (mTerminate)
         return 0;

      // calc ideal serial value
      v = value-v1;

      if (v<esMINR || v>esMAXR)
         goto skips;

      // try to find that value via recursion:
      v2 = findCombinationX(v, es, togo-1, r2, lerr);
      if (v2<0.0)
         goto skips;

      // check result:
      v = v1+v2;
      err = v/value-1.0;
      if (fabs(err)<fabs(lerr))
      {
         // found a better solution:
         lerr = err;
         result=v;
         rb=r1;
         rb.op=roSER;

         re=rb; // return best fit
         if (mMutex)
            mMutex->lock();
         *e=rb; // store best fit
         if (mMutex)
            mMutex->unlock();
         if (togo==mTogo)
         {
#ifdef DEBUG
            qDebug() << "Solution: " << QString::number(result, 'f', 20) << "  error: " << QString::number(lerr, 'f', 20);
            printList();
#endif
            if (mSignalProgress && mInterface && mMutex)
               (mInterface->*mSignalSolution)(getMsg(err, v), false);
            if (err==0.0)
               return result;
         }
      }
skips:
      r1.r++;
      v1=r1.r.value();
   }

   // check out parallel combinations:
   r1.r.setValue(value, es);
   v1=r1.r.value();
   while (v1<esMAXR)
   {
      // feedback
      if (togo==mTogo)
         mCurTest++;
      if (mTerminate)
         return 0;

      // calc ideal parallel value
      if (v1==value)
         goto skipp; // avoid div by zero
      v = (v1*value)/(v1-value);

      if (v<esMINR || v>esMAXR)
         goto skipp;

      // try to find that value via recursion:
      v2 = findCombinationX(v, es, togo-1, r2, lerr);
      if (v2<0.0)
         goto skipp;
      // check result:
      v = (v1*v2)/(v1+v2);
      err = v/value-1.0;
      if (fabs(err)<fabs(lerr))
      {
         // found a better solution:
         lerr = err;
         result=v;
         rb=r1;
         rb.op=roPAR;

         re=rb; // return best fit
         if (mMutex)
            mMutex->lock();
         *e=rb; // store best fit
         if (mMutex)
            mMutex->unlock();
         if (togo==mTogo)
         {
#ifdef DEBUG
            qDebug() << "Solution: " << QString::number(result, 'f', 20) << "  error: " << QString::number(lerr, 'f', 20);
            printList();
#endif
            if (mSignalProgress && mInterface && mMutex)
               (mInterface->*mSignalSolution)(getMsg(err, v), false);
            if (err==0.0)
               return result;
         }
      }
skipp:
      r1.r++;
      v1=r1.r.value();
   }

   return result;
}

QString CRSolver::printList()
{
   pEntity e;
   QString s="";
   int toClose = 0;
   ROP lastOp=mList[mTogo]->op;

   for (int i=mTogo; i>=0; i--)
   {
      e = mList[i];
      if (i<mTogo && i>0 && lastOp!=e->op)
      {
         s += "(";
         lastOp=e->op;
         toClose++;
      }
      if (e->op==roPAR)
         s += e->r.toString() + " || ";
      else if (e->op==roSER)
         s += e->r.toString() + " + ";
      else
         s += e->r.toString();
   }
   for (int i=0; i<toClose; i++)
         s += ")";
#ifdef DEBUG
   qDebug() << s;
#endif
   return s;
}

