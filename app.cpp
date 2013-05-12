#include "app.h"

App::App(QObject *parent) : QObject(parent) {
    init();
}

void App::init() {
    QGst::init();
    m_pipelineOut = QGst::Pipeline::create();
    m_pipelineIn = QGst::Pipeline::create();
}

void App::run() {

//    startVoice();
    startListen(m_pipelineIn, 5000); // TODO settings

    m_pipelineOut->setState(QGst::StatePlaying);
    m_pipelineIn->setState(QGst::StatePlaying);

    QTimer::singleShot(2000, this, SLOT(testSlot()));
    QTimer::singleShot(4000, this, SLOT(testSlot2()));
}

void App::startVoice() {
    rtpbin = QGst::ElementFactory::make("gstrtpbin");
    if (!rtpbin) {
        qFatal("Failed to create gstrtpbin");
    }
    qDebug() << "initn create";

    m_pipelineOut->add(rtpbin);

    //audio content
    //sending side
    QGst::ElementPtr audiosrc;

    try {
        audiosrc = QGst::Bin::fromDescription(
//            "alsasrc device=plughw:0 ! queue ! audioconvert ! audiorate ! audio/x-raw-int,rate=8000 "
//            "! speexenc ! rtpspeexpay"
//        );
            "audiotestsrc ! queue ! audioconvert ! audiorate ! audio/x-raw-int,rate=8000 ! audioconvert"
        );
//        audiosrc = QGst::Bin::fromDescription(
//             "audiotestsrc ! audioconvert ! level ! audioconvert ! twolame  ! rtpmpapay"
//        );
    } catch (const QGlib::Error & error) {
        qCritical() << error;
        qFatal("One ore more required elements are missing. Aborting...");
    }
    m_pipelineOut->add(audiosrc);

    volumeOut = QGst::ElementFactory::make("volume");
    m_pipelineOut->add(volumeOut); // TODO settings

    audiosrc->link(volumeOut);

    QGst::ElementPtr decoder;
    try {
        decoder = QGst::Bin::fromDescription(
            "speexenc vad=false ! rtpspeexpay"
        );
    } catch (const QGlib::Error & error) {
        qCritical() << error;
        qFatal("One ore more required elements are missing. Aborting...");
    }
    m_pipelineOut->add(decoder);
    volumeOut->link(decoder);



    decoder->link(rtpbin, "send_rtp_sink_1");

    rtpudpsink =  QGst::ElementFactory::make("udpsink");
    if (!rtpudpsink) {
        qFatal("Failed to create udpsink. Aborting...");
    }


    rtpudpsink->setProperty("host", "127.0.0.1"); // desttination 192.168.0.102
    rtpudpsink->setProperty("port", 5000);        // port
    m_pipelineOut->add(rtpudpsink);
    rtpbin->link("send_rtp_src_1", rtpudpsink);

}

void App::startListen(QGst::PipelinePtr pipe, int port) {
    QGst::ElementPtr rtcpudpsink = QGst::ElementFactory::make("udpsrc");
    rtcpudpsink->setProperty("host", "127.0.0.1"); // TODO settings
    rtcpudpsink->setProperty("port", port);        // source
//    rtcpudpsink->setProperty("sync", false);
//    rtcpudpsink->setProperty("async", false);
    rtcpudpsink->setProperty("caps", QGst::Caps::fromString("application/x-rtp,media=(string)audio, clock-rate=(int)8000, encoding-name=(string)SPEEX,payload=(int)110"));
    pipe->add(rtcpudpsink);


    QGst::ElementPtr bin;

    try {
        bin = QGst::Bin::fromDescription(
            "rtpspeexdepay ! speexdec ! audioconvert"
        );
    } catch (const QGlib::Error & error) {
        qCritical() << error;
        qFatal("One ore more required elements are missing. Aborting...");
    }
    pipe->add(bin);
    rtcpudpsink->link(bin);

    volumeIn = QGst::ElementFactory::make("volume"); // TODO settings
    pipe->add(volumeIn);
    bin->link(volumeIn);


    QGst::ElementPtr audioSynk = QGst::ElementFactory::make("autoaudiosink");
    pipe->add(audioSynk);
    volumeIn->link(audioSynk);
}


void App::testSlot() {
    qDebug() << "remove!";
    volumeIn->setProperty("mute", true);
}

void App::testSlot2() {
    qDebug() << "add again!";
    volumeIn->setProperty("mute", false);
}
