#ifndef APP_H
#define APP_H

#include <QString>
#include <QTimer>
#include <QtCore>


#include <QGlib/Connect>
#include <QGlib/Error>
#include <QGst/Init>
#include <QGst/Pipeline>
#include <QGst/ElementFactory>
#include <QGst/Pad>
#include <QGst/Structure>
#include <QGst/Bus>
#include <QGst/Message>

class App: public QObject {
    Q_OBJECT
public:
    explicit App(QObject *parent = 0);
    void run();
public slots:
   void testSlot();
   void testSlot2();
private:
    void init();
    void startVoice();
    void startListen(QGst::PipelinePtr, int port);
    QGst::PipelinePtr m_pipelineOut;
    QGst::PipelinePtr m_pipelineIn;

    QGst::ElementPtr rtpbin;
    QGst::ElementPtr rtpudpsink;

    QGst::ElementPtr volumeOut;
    QGst::ElementPtr volumeIn;
};

#endif // APP_H
