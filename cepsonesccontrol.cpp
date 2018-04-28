#include "cepsonesccontrol.h"

CEpsonEscControl::CEpsonEscControl(QObject *parent) : QObject(parent)
{
    m_script = new QByteArray();
    *m_script = "";
    m_serialPort.setBaudRate(QSerialPort::Baud9600);
    m_serialPort.setDataBits(QSerialPort::Data8);
    m_serialPort.setStopBits(QSerialPort::OneStop);
    m_serialPort.setParity(QSerialPort::NoParity);
    m_fontCode = QTextCodec::codecForName("big5");
}

CEpsonEscControl::~CEpsonEscControl() {
    delete m_script;
}

CEpsonEscControl& CEpsonEscControl::c_addFont(QByteArray text) {
    (*m_script) += m_fontCode->fromUnicode(text + '\n');
    return (*this);
}

CEpsonEscControl& CEpsonEscControl::c_clearBuffer() {
    (*m_script).clear();
    return (*this);
}

CEpsonEscControl& CEpsonEscControl::c_beginFontSize(uchar sz) {
    c_beginFontSize(sz,sz);
    return (*this);
}

CEpsonEscControl& CEpsonEscControl::c_beginFontSize(uchar width,uchar height) {
    if(width > 7 || height > 7) {
        qDebug() << "font size can't big than 7";
        return (*this);
    }
    (*m_script) += QByteArray::fromRawData("\x1d\x21",2);
    width <<= 4;
    char _byte = (width | height);
    *m_script += QByteArray::fromRawData(&_byte,1);
    return (*this);
}

CEpsonEscControl& CEpsonEscControl::c_endFontSize() {
    c_beginFontSize(0);
    return (*this);
}

QByteArray CEpsonEscControl::getCurBufferContent() {
    return *m_script;
}

CEpsonEscControl& CEpsonEscControl::c_cutPaper(bool fullcut,uchar mode,uchar pos) {
    char _c = 0;
    *m_script += QByteArray::fromRawData("\x1d\x56",2);
    switch(mode) {

    case 0: { //func A
        _c = (fullcut?'0':'1');
        *m_script += QByteArray::fromRawData(&_c,1);
        return (*this);
    }

    case 1:{ //func B
        _c = (char)(fullcut?65:66);
        break;
    }

    case 2:{ //func C
        _c = (char)(fullcut?97:98);
        break;
    }

    case 3:{ //func D
        _c = (char)(fullcut?103:104);
        break;
    }
    default:
        return (*this);
    }
    *m_script += QByteArray::fromRawData(&_c,1);
    *m_script += QByteArray::fromRawData((char*)&pos,1);
    qDebug() << (*m_script);
    return (*this);
}

CEpsonEscControl& CEpsonEscControl::c_lineSpace(uchar count) {
    *m_script += QByteArray::fromRawData("\x1B\x33",2);
    *m_script += QByteArray::fromRawData((char*)&count,1);
    return (*this);
}

CEpsonEscControl& CEpsonEscControl::c_objLayout(uchar layout) {
    char _c = (layout=='l'?'0':
                           (layout=='r'?'2':
                                        (layout=='c'?'1':'3')));
    if(_c == '3')
        return (*this);
    *m_script += QByteArray::fromRawData("\x1B\x61",2);
    *m_script += QByteArray::fromRawData(&_c,1);
    return (*this);
}
CEpsonEscControl& CEpsonEscControl::c_init() {
    *m_script += QByteArray::fromRawData("\x1B\x40",2);
    return (*this);
}

CEpsonEscControl& CEpsonEscControl::c_initLineSpace() {
    *m_script += QByteArray::fromRawData("\x1B\x32",2);
    return (*this);
}

CEpsonEscControl& CEpsonEscControl::c_feedLine(uchar count) {
    *m_script += QByteArray::fromRawData("\x1B\x4A",2);
    *m_script += QByteArray::fromRawData((char*)&count,1);
    return (*this);
}

CEpsonEscControl& CEpsonEscControl::c_lineSpace_b(uchar count) {
    *m_script += QByteArray::fromRawData("\x1B\x64",2);
    *m_script += QByteArray::fromRawData((char*)&count,1);
    return (*this);
}

CEpsonEscControl& CEpsonEscControl::c_sendPlus(uchar pin,uchar plusT,uchar spaceT) {
    if(pin != '0' || pin != '1')
        return (*this);
    *m_script += QByteArray::fromRawData("\x1B\x70",2);
    *m_script += QByteArray::fromRawData((char*)&pin,1);
    *m_script += QByteArray::fromRawData((char*)&plusT,1);
    *m_script += QByteArray::fromRawData((char*)&spaceT,1);
    return (*this);
}

uint CEpsonEscControl::getDataSize() {
    return m_script->length();
}

QSerialPort& CEpsonEscControl::getSerialPort() {
    return m_serialPort;
}

void CEpsonEscControl::setPortName(const QString& name) {
    m_serialPort.setPortName(name);
}

bool CEpsonEscControl::openSerialPort(QSerialPort::OpenMode mode) {
    return m_serialPort.open(mode);
}

uint CEpsonEscControl::sendBufferToPrinter(bool clearBufr) {
    uint sendSz = m_serialPort.write(*m_script);
    if(clearBufr)
        m_script->clear();
    return sendSz;
}
