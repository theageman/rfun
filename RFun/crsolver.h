/*
    RFun - having fun with resistors
    written 2015 by theageman <theageman@gmx.de>

    FILE:      csolver.h
    PURPOSE:   CRSolver class header file
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

#ifndef CRSOLVER_H
#define CRSOLVER_H

#include <QList>
#include <QMutex>
#include "eseries.h"
#include "cresistor.h"

typedef enum ROperation_e {
   roSER, roPAR, roNone
} ROP;

typedef struct {
   CResistor r;
   ROP       op;
}Entity;

typedef Entity* pEntity;

class CRInterface;

class CRSolver
{
public:
   CRSolver();
   ~CRSolver();

   double findCombination(double value, E_Series es, int nResistors);
   int getLength(); // num of elements in list
   int getProgress();
   pEntity getEntity(int index);
   void setProgressFunc(void(CRInterface::*func)(double));
   void setSolutionFunc(void(CRInterface::*func)(QString, bool));
   void setInterface(CRInterface* interface);
   void setMutex(QMutex *mutex);
   void terminate();
   QString printList();

private:
   QList<pEntity> mList;
   int mTogo;
   int mNumTests;
   int mCurTest;
   bool mTerminate;

   void (CRInterface::*mSignalProgress)(double progress);
   void (CRInterface::*mSignalSolution)(QString msg, bool bFinal);
   CRInterface* mInterface;
   QMutex *mMutex;

   void initList(int length);
   void clearList();

   double findCombinationX(double value,
                          E_Series es,
                          int togo,
                          Entity &re,
                           double cerr);
   void calcNumTests(double value, E_Series es);
   QString getMsg(double err, double v);

};

#endif // CRSOLVER_H
