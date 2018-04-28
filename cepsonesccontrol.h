#ifndef CEPSONESCCONTROL_H
#define CEPSONESCCONTROL_H

#include <QObject>
#include <QDebug>
#include <QSerialPort>
#include <QTextCodec>

class CEpsonEscControl : public QObject
{
    Q_OBJECT
public:

    explicit CEpsonEscControl(QObject *parent = nullptr);
    ~CEpsonEscControl();
    CEpsonEscControl& c_addFont(QByteArray text);  //add text ((auto append '\n'
    CEpsonEscControl& c_clearBuffer();  //clear m_script buffer
    CEpsonEscControl& c_beginFontSize(uchar sz); // 0 ~ 7
    CEpsonEscControl& c_beginFontSize(uchar width,uchar height); // 0~7
    CEpsonEscControl& c_endFontSize();  //set font sz to origin size
    CEpsonEscControl& c_cutPaper(bool fullcut = false,uchar mode = 0,uchar pos = 0);  //mode=1 => funcA,mode=2 => funcB.... pos is n = 0-255
    /*<Function A>	Cuts the paper
    <Function B>	Feeds paper and cuts the paper
    <Function C>	Sets the paper cutting position
    <Function D>	Feeds paper and cuts the paper, and feeds paper to print starting position

    The printer operation of the each function is as follows:
    <Function A>	Executes paper cut
    <Function B>	Feeds paper to [cutting position + (n × vertical motion unit)] and executes paper cut
    <Function C>	Preset [cutting position + (n × vertical motion unit)] to the paper cutting position, and executes paper cut when it reaches the autocutter position after printing and feeding
    <Function D>	Feeds paper to [cutting position + (n × vertical motion unit)] and executes paper cut, then moves paper to the print start position by reverse feeding.*/

    CEpsonEscControl& c_init(); //clear printer buffer and resets the printer
    CEpsonEscControl& c_lineSpace(uchar count); // line space
    CEpsonEscControl& c_lineSpace_b(uchar count); // big than c_lineSpace
    CEpsonEscControl& c_objLayout(uchar layout); // 'l' = left,'r' = right , 'c' = center
    CEpsonEscControl& c_initLineSpace(); //init line space to origin
    CEpsonEscControl& c_feedLine(uchar count = 1); // feed paper
    CEpsonEscControl& c_sendPlus(uchar pin,uchar plusT,uchar spaceT); //send plus|| pin='0'(pin 2),'1'(pin 5)  plusT(ms) spaceT(ms)  ex: open cash drawer

    QByteArray getCurBufferContent(); //get buffer bytes data
    uint getDataSize(); //get cur m_script bytes
    //serial default is in construct func
    QSerialPort& getSerialPort(); //return m_serialPort
    void setPortName(const QString&);  //set serial port ex: com1(or COM1) com2...
    bool openSerialPort(QSerialPort::OpenMode);
    uint sendBufferToPrinter(bool clearBufr = true); //send m_script to serial port(printer) return actual send bytes
signals:

public slots:

private:
    QByteArray *m_script;
    QSerialPort m_serialPort;
    QTextCodec *m_fontCode;
};

#endif // CEPSONESCCONTROL_H
