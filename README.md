# Qt Epson Tm T82ii printer Lib
This is TM-T82II(EPSON) printer LIB,use Qt lib
lib is easy,jsut include header.
the func detail is in header.
p.s. if you want add new commands,you can following url:
https://reference.epson-biz.com/modules/ref_escpos/index.php?content_id=72

and add in *.cpp

# code example:
```
#include "CEpsonEscControl/cepsonesccontrol.h"
...
...

int main(int argc, char *argv[])
{
    ...
    ...
    
    CEpsonEscControl m_ctrl;
    m_ctrl.setPortName("COM1");
    if(!m_ctrl.openSerialPort(QSerialPort::WriteOnly)) {
        qDebug() << "open fails";
        ...
        ...
    }
    int sendBytes = m_ctrl
            .c_init()
            .c_addFont("im mark")
            .c_objLayout('c')
            .c_beginFontSize(2)
            .c_addFont("big font")
            .c_endFontSize()
            .c_lineSpace_b(2)
            .c_addFont("我是繁體中文 hello!!")
            .c_cutPaper(false,1,20)
            .sendBufferToPrinter();
    qDebug() << "actual send = " << sendBytes;


    ...
    ...
}
```
