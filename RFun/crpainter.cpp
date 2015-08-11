#include <QPainter>
#include <QPixmap>
#include <QColor>

#include "crpainter.h"
#include "eseries.h"
#include "cresistor.h"
#include "crsolver.h"

CRPainter::~CRPainter()
{
  if (mPMp)
  {
    delete mPMp;
    mPMp=NULL;
  }
}

QPixmap CRPainter::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
   size->setHeight(mHeight);
   size->setWidth(mWidth);

   return mPM;
}

void CRPainter::createImage(CRSolver *slv)
{
   int w, h, x, y, ser, par;
   pEntity e;
   ROP lastOp=roNone;

   QFont font("Arial Black", 10, QFont::Black);

   mSolver = slv;
   if (mSolver)
   {
     // calc dimensions:
     w=60; h=48;
      for (int i=0; i<mSolver->getLength(); i++)
      {
         e=mSolver->getEntity(i);
         if (e->op==roPAR)
            w+=60;
         if (e->op==roSER)
            h+=48;
      }
      w+=20; h+=60;
      mWidth=w; mHeight=h;
      // create pixmap and painter objects:
/*
      QPixmap *pm = new QPixmap(w,h);
      QPainter *pt = new QPainter(pm);
      pt->setRenderHint(QPainter::Antialiasing, true);
      pt->setRenderHint(QPainter::TextAntialiasing, true);
      mPM=pm;
      mPT=pt;
*/
      mPM = mPM.scaled(w, h, Qt::IgnoreAspectRatio, Qt::FastTransformation);
      mPT.begin(&mPM);
      mPT.setRenderHint(QPainter::Antialiasing, true);
      mPT.setRenderHint(QPainter::TextAntialiasing, true);
      // initialize pixmap:
      mPM.fill(QColor(220,220,255,255));
      mPT.setPen(QPen(QBrush(QColor(0,0,0,255)), 2.0)); // black lines 2 px wide
      mPT.setFont(font);
      mPT.setBrush(QBrush(Qt::white));
      // initialize locals:
      ser=par=0;
      x = w-60-10; y=h-48-30;
      for (int i=0; i<mSolver->getLength(); i++)
      {
        e=mSolver->getEntity(i);
        switch (e->op)
        {
          case roPAR:
            par++;
            x-=60;
            // down @ top;
            //pt->drawLine(x+8,y+ser*24,x+8,y-15);
            mPT.drawLine(x+8,y+ser*24,x+8,y);
            // down @ bottom:
            mPT.drawLine(x+8,y+48+ser*24,x+8,y+48+ser*48);
            // right @ bottom:
            mPT.drawLine(x+8,y+48+ser*48,x+8+par*60,y+48+ser*48);
            // right @ top:
            mPT.drawLine(x+8,y,x+68,y);
            if (par>1)
            {
              mPT.setBrush(QBrush(Qt::black));
              mPT.drawEllipse(QPoint(x+68,y+48+ser*48), 3, 3);
              if (lastOp!=roSER)
                mPT.drawEllipse(QPoint(x+68,y), 3, 3);
              mPT.setBrush(QBrush(Qt::white));
            }
            lastOp=roPAR;
            drawResistor(x, y+ser*24, e->r.toString(), &mPT);
            break;
          case roSER:
            ser++;
            y-=48;
            if (lastOp==roPAR)
            {
              mPT.setBrush(QBrush(Qt::black));
              mPT.drawEllipse(QPoint(x+8,y+48), 3, 3);
              mPT.setBrush(QBrush(Qt::white));
            }
            lastOp=roSER;
            drawResistor(x, y, e->r.toString(), &mPT);
            break;
          default:
            drawResistor(x, y, e->r.toString(), &mPT);
        }
      }
      mPT.drawLine(x+8,y,x+8,y-15);
      mPT.drawLine(x+8,h-30,x+8,h-15);
      mPT.drawEllipse(QPoint(x+8,y-17),4,4);
      mPT.drawEllipse(QPoint(x+8,h-13),4,4);
      mPT.setBrush(QBrush(Qt::black));
      if (lastOp!=roSER)
         mPT.drawEllipse(QPoint(x+8,y), 3, 3);
      if (par>0)
         mPT.drawEllipse(QPoint(x+8,h-30), 3, 3);
      mPT.setBrush(QBrush(Qt::white));
   }
   mPT.end();
}

QPixmap *CRPainter::getPixmap()
{
   return &mPM;
}

void CRPainter::drawResistor(int x, int y, QString txt, QPainter *pt)
{
   pt->drawLine(x+8,  y+0,  x+8,  y+9);
   pt->drawLine(x+8,  y+39, x+8,  y+48);
   pt->drawLine(x+1,  y+9,  x+1,  y+39);
   pt->drawLine(x+15, y+9,  x+15, y+39);
   pt->drawLine(x+1,  y+9,  x+15, y+9);
   pt->drawLine(x+1,  y+39, x+15, y+39);
   pt->fillRect(x+2,  y+10, 12, 28, QColor(255,255,255,255));

   pt->drawText(x+18, y+28, txt);
}

