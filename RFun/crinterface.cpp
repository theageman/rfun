#include <math.h>
#include "crinterface.h"
#include "eseries.h"
#include "cresistor.h"
#include "crsolver.h"

CRInterface::CRInterface(QObject *parent) : QObject(parent)
{
  mPainter=NULL;
  mSolver=NULL;
  mThread=NULL;
}

CRInterface::~CRInterface()
{
  deleteThreadObjects();
}

void CRInterface::newTask(double val, int ESeries, int numResistors)
{
  deleteThreadObjects();
  if (mSolver)
  {
    WorkerThread *workerThread = new WorkerThread;
    connect(workerThread, &WorkerThread::resultReady, this, &CRInterface::finishedSolution);
    connect(workerThread, &WorkerThread::finished, workerThread, &QObject::deleteLater);
    mThread=workerThread;
    workerThread->mSolver=mSolver;
    workerThread->mVal=val;
    workerThread->mES=(E_Series)ESeries;
    workerThread->mNum=numResistors;
    workerThread->start();
  }
}

int CRInterface::getProgress()
{
  int result=0;
  if (mSolver)
    result = mSolver->getProgress();
  return result;
}

void CRInterface::stopTask()
{
  if (mSolver)
    mSolver->terminate();
}

QString CRInterface::getFormular()
{
  if (mSolver)
    return "R = "+mSolver->printList();
  else
    return "R = ";
}

CRSolver *CRInterface::getSolver()
{
  return mSolver;
}

CRPainter *CRInterface::getPainter()
{
  return mPainter;
}

void CRInterface::setSolver(CRSolver *so)
{
  mSolver = so;
  if (mSolver)
  {
    mSolver->setMutex(&mMutex);
    mSolver->setInterface(this);
    mSolver->setProgressFunc(&CRInterface::progressSlot);
    mSolver->setSolutionFunc(&CRInterface::solutionSlot);
  }
}

void CRInterface::setPainter(CRPainter *painter)
{
  mPainter = painter;
}

void CRInterface::finishedSolution(const QString &msg)
{
  mThread=NULL;
}

// function callbacks:

void CRInterface::progressSlot(double p)
{
  /*
   mMutex.lock();
   emit progress((int)fmin(p*1000, 1000));
   mMutex.unlock();
*/
}

void CRInterface::solutionSlot(QString msg, bool bFinal)
{
  //   mMutex.lock();
  if (mPainter)
    mPainter->createImage(mSolver);
  emit newImage(msg, bFinal);
  //   mMutex.unlock();
}

void CRInterface::deleteThreadObjects()
{
  if (mThread)
  {
    if (mThread->isRunning())
      mThread->terminate();
    while (mThread->isRunning());
    delete mThread;
    mThread=NULL;
  }
}



