#ifndef ASCEL_PROTOKOLL_H
#define ASCEL_PROTOKOLL_H

#include <QVariant>
#include <QString>
#include <QStringList>

#define code_Frequency 'A'
#define code_Waveform 'B'
#define code_Mode 'C'
#define code_PLLReferenceEnable 'D'
#define code_PLLFactor 'E'
#define code_PLLOffsett 'F'
#define code_StartupWaveform 'G'
#define code_StartupFrequency 'H'
#define code_CallibrationOffset 'I'
#define code_SweepStartFrequency 'J'
#define code_SweepStopFrequency 'K'
#define code_SweepFrequency 'L'
#define code_ModulationType 'M'
#define code_FSKFrequency 'N'
#define code_PSKPhase 'O'
#define code_ModulationSource 'P'
#define code_InternalModulationFrequency 'Q'
#define code_SweepMode 'R'
#define code_Reserved0 'S'
#define code_GetSettings 'T'
#define code_KeepAlive 'U'
#define code_ReturnFromSweep 'V'
#define code_Reserved1 'W'
#define code_HardwareRevision 'X'
#define code_FirmwareRevision 'Y'
#define code_ProduktID 'Z'
#define code_Preset1Frequency '1'
#define code_Preset1Waveform '2'
#define code_Preset2Frequency '3'
#define code_Preset2Waveform '4'
#define code_Preset3Frequency '5'
#define code_Preset3Waveform '6'
#define code_Preset4Frequency '7'
#define code_Preset4Waveform '8'
#define code_Preset5Frequency '9'
#define code_Preset5Waveform '0'

class AscelProtokoll
{
public:
    AscelProtokoll();
    bool CheckCommand(QString command);
    QString GetSendCommandString(char command, int data);
    QString GetSettingsCommand();

    int command;
    int data;

};

#endif // ASCEL_PROTOKOLL_H
