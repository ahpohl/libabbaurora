#ifndef ABBAuroraStrings_h
#define ABBAuroraStrings_h

class ABBAuroraStrings {
    private:

    public:
       static std::string TransmissionState(unsigned char id);
       static std::string GlobalState(unsigned char id);
       static std::string DcDcState(unsigned char id);
       static std::string InverterState(unsigned char id);
       static std::string AlarmState(unsigned char id);
       static std::string VersionPart1(char id);
       static std::string VersionPart2(char id);
       static std::string VersionPart3(char id);
       static std::string VersionPart4(char id);
};

#endif
