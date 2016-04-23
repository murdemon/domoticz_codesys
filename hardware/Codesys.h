#pragma once

#include "DomoticzHardware.h"
#include "GpioPin.h"
#include "../main/RFXtrx.h"

class CCodesys : public CDomoticzHardwareBase
{
public:
	explicit CCodesys(const int ID);
	~CCodesys();

	bool WriteToHardware(const char *pdata, const unsigned char length);
	
	static bool InitPins();
	static std::vector<CGpioPin> GetPinList();
	static CGpioPin* GetPPinById(int id);

private:
	bool StartHardware();
	bool StopHardware();

	void Do_Work();
	void ProcessInterrupt(int gpioId, int housecode);
	
	// List of GPIO pin numbers, ordered as listed
	static std::vector<CGpioPin> pins;
	
	boost::shared_ptr<boost::thread> m_thread;
	volatile bool m_stoprequested;
	int m_waitcntr;
	tRBUF IOPinStatusPacket;

};
