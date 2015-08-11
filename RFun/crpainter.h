#ifndef CRPAINTER_H
#define CRPAINTER_H

#include <QObject>
#include <QPainter>
#include <QPixmap>
#include <QQuickImageProvider>

#include "eseries.h"
#include "cresistor.h"
#include "crsolver.h"

#define crpWidth 40
#define crpHeight 40


class CRPainter : public QQuickImageProvider

{
public:
    CRPainter() : QQuickImageProvider(QQuickImageProvider::Pixmap)
    {
      mPMp = new QPixmap(20,20);
      mPM = *mPMp;
    }
    ~CRPainter();
    QPixmap requestPixmap(const QString& id, QSize* size, const QSize& requestedSize);

   void createImage(CRSolver *slv);
   QPixmap *getPixmap();


signals:

public slots:

private:
   CRSolver *mSolver;
   QPixmap  mPM;
   QPixmap  *mPMp;
   QPainter mPT;
   int mWidth, mHeight;

   void drawResistor(int x, int y, QString txt, QPainter *pt);
};

#endif // CRPAINTER_H
