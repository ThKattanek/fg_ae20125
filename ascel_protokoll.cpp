#include "ascel_protokoll.h"

AscelProtokoll::AscelProtokoll()
{
}

bool AscelProtokoll::CheckCommand(QString commandline)
{
    QStringList list = commandline.split(':');

    if(list.length() < 3) return false;
    if(list.at(0) != "201") return false;
    if(list.at(1).length() != 1) return false;

    bool ok;

    command = list.at(1).at(0).toAscii();

    switch(command)
    {
        case code_Frequency:
        case code_Waveform:
        case code_Mode:
        case code_PLLReferenceEnable:
        case code_PLLFactor:
        case code_PLLOffsett:
        case code_HardwareRevision:
        case code_FirmwareRevision:
        case code_ProduktID:

            data = list.at(2).toInt(&ok);
            if(!ok) return false;
            return true;
            break;

        case code_KeepAlive:
            return true;
            break;

        default:
            break;
    }

    return false;
}

QString AscelProtokoll::GetSendCommandString(char command, int data)
{
    QString str;
    str = "201:" + QString(command) + ":" + QVariant(data).toString() + ":;";
    return str;
}
