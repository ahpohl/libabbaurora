#ifndef ABBAuroraStrings_h
#define ABBAuroraStrings_h

/** @file */ 

/// Aurora strings
class ABBAuroraStrings {
    private:

    public:
       /// Transmission state after receiving a command
       static std::string TransmissionState(unsigned char id);
       /// State of the addressed device
       static std::string GlobalState(unsigned char id);
       /// State of the DC/DC converter
       static std::string DcDcState(unsigned char id);
       /// State of the inverter
       static std::string InverterState(unsigned char id);
       /// Alarm states (alarm codes are shown only on Aurora display)
       static std::string AlarmState(unsigned char id);
       /// Version reading 1: Indoor/Outdoor and type
       static std::string VersionPart1(char id);
       /// Version reading 2: Grid standard
       static std::string VersionPart2(char id);
       /// Version reading 3: Trafo/Non Trafo
       static std::string VersionPart3(char id);
       /// Version reading 4: Wind/PV
       static std::string VersionPart4(char id);
};

#endif
