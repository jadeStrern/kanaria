kanaria - station
* libqtgstreamer-dev
* libgstreamer-plugins-base0.10-dev


1) destination/source port
2) volume in/out
3) QGst::Caps::fromString



gst params
      - audiotestsrc
      - gst-launch -v alsasrc ! audioconvert ! level ! audioconvert ! twolame  ! rtpmpapay  ! udpsink host=192.168.0.100 port=5004
      - gst-launch -v alsasrc device=plughw:0  ! audioconvert ! audiorate ! audio/x-raw-int,rate=8000 ! speexenc ! rtpspeexpay  ! udpsink host=192.168.0.100 port=5004
      - gst-launch -v filesrc location= ~/Music/death\ in\ vegas/Death\ In\ Vegas\ -\ Satan\'s\ Circus\ \(2005\)/Sons\ of\ Rother\ vs\ Shout.mp3  ! mad ! audioconvert ! audiorate !  volume volume=1 ! speexenc vad=false  ! rtpspeexpay ! udpsink host=192.168.0.101 port=5000   // sender
      - gst-launch udpsrc port=5000 caps="application/x-rtp,media=(string)audio, clock-rate=(int)8000, encoding-name=(string)SPEEX,payload=(int)110" ! rtpspeexdepay ! speexdec ! autoaudiosink             // listen
