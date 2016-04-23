#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#ifdef WITH_GPIO
#include "Codesys.h"
#include "GpioPin.h"
#ifndef WIN32
	#include <wiringPi.h>
#endif
#include "../main/Helper.h"
#include "../main/Logger.h"
#include "hardwaretypes.h"
#include "../main/RFXtrx.h"
#include "../main/localtime_r.h"
#include "../main/mainworker.h"

#define NO_INTERRUPT	-1
#define MAX_GPIO	31

bool m_bIsInitGPIOPins_c=false;

char Old_Mem[1024];
char Act_Mem[1024];
void *pMemory;

// List of GPIO pin numbers, ordered as listed
std::vector<CGpioPin> CCodesys::pins;

// Queue of GPIO numbers which have triggered at least an interrupt to be processed.
std::vector<int> gpioInterruptQueue_c;

boost::mutex interruptQueueMutex_c;
boost::condition_variable interruptCondition_c;


/*
 * Direct GPIO implementation, inspired by other hardware implementations such as PiFace and EnOcean
 */
CCodesys::CCodesys(const int ID)
{
	m_stoprequested=false;
	m_HwdID=ID;

	//Prepare a generic packet info for LIGHTING1 packet, so we do not have to do it for every packet
	IOPinStatusPacket.LIGHTING1.packetlength = sizeof(IOPinStatusPacket.LIGHTING1) -1;
	IOPinStatusPacket.LIGHTING1.housecode = 65;
	IOPinStatusPacket.LIGHTING1.packettype = pTypeLighting1;
	IOPinStatusPacket.LIGHTING1.subtype = sTypeIMPULS;
	IOPinStatusPacket.LIGHTING1.rssi = 12;
	IOPinStatusPacket.LIGHTING1.seqnbr = 0;

	if (!m_bIsInitGPIOPins_c)
	{
		InitPins();
		m_bIsInitGPIOPins_c=true;
	}
}

CCodesys::~CCodesys(void)
{
}

/*
 * interrupt handlers:
 *********************************************************************************
 */


 void pushInterrupt_c(int gpioId) {
	boost::mutex::scoped_lock lock(interruptQueueMutex_c);

	if(std::find(gpioInterruptQueue_c.begin(), gpioInterruptQueue_c.end(), gpioId) != gpioInterruptQueue_c.end()) {
		_log.Log(LOG_NORM, "GPIO: Interrupt for GPIO %d already queued. Ignoring...", gpioId);
		interruptCondition_c.notify_one();
	}
	else {
		// Queue interrupt. Note that as we make sure it contains only unique numbers, it can never "overflow".
		_log.Log(LOG_NORM, "GPIO: Queuing interrupt for GPIO %d.", gpioId);
		gpioInterruptQueue_c.push_back(gpioId);
		interruptCondition_c.notify_one();
	}
	_log.Log(LOG_NORM, "GPIO: %d interrupts in queue.", gpioInterruptQueue_c.size());
}



void interruptHandler0_c(void) { pushInterrupt_c(0); }
void interruptHandler1_c(void) { pushInterrupt_c(1); }
void interruptHandler2_c(void) { pushInterrupt_c(2); }
void interruptHandler3_c(void) { pushInterrupt_c(3); }
void interruptHandler4_c(void) { pushInterrupt_c(4); }

void interruptHandler7_c(void) { pushInterrupt_c(7); }
void interruptHandler8_c(void) { pushInterrupt_c(8); }
void interruptHandler9_c(void) { pushInterrupt_c(9); }
void interruptHandler10_c(void) { pushInterrupt_c(10); }
void interruptHandler11_c(void) { pushInterrupt_c(11); }

void interruptHandler14_c(void) { pushInterrupt_c(14); }
void interruptHandler15_c(void) { pushInterrupt_c(15); }

void interruptHandler17_c(void) { pushInterrupt_c(17); }
void interruptHandler18_c(void) { pushInterrupt_c(18); }

void interruptHandler20_c(void) { pushInterrupt_c(20); }
void interruptHandler21_c(void) { pushInterrupt_c(21); }
void interruptHandler22_c(void) { pushInterrupt_c(22); }
void interruptHandler23_c(void) { pushInterrupt_c(23); }
void interruptHandler24_c(void) { pushInterrupt_c(24); }
void interruptHandler25_c(void) { pushInterrupt_c(25); }

void interruptHandler27_c(void) { pushInterrupt_c(27); }
void interruptHandler28_c(void) { pushInterrupt_c(28); }
void interruptHandler29_c(void) { pushInterrupt_c(29); }
void interruptHandler30_c(void) { pushInterrupt_c(30); }
void interruptHandler31_c(void) { pushInterrupt_c(31); }



bool CCodesys::StartHardware()
{
#ifndef WIN32
	// TODO make sure the WIRINGPI_CODES environment variable is set, otherwise WiringPi makes the program exit upon error
	// Note : We're using the wiringPiSetupSys variant as it does not require root privilege
	if (wiringPiSetupSys() != 0) {
		_log.Log(LOG_ERROR, "GPIO: Error initializing wiringPi !");
		return false;
	}
#endif
	m_stoprequested=false;

	//Start worker thread that will be responsible for interrupt handling
	m_thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&CCodesys::Do_Work, this)));

	m_bIsStarted=true;
#ifndef WIN32
	//Hook up interrupt call-backs for each input GPIO
	for(std::vector<CGpioPin>::iterator it = pins.begin(); it != pins.end(); ++it) {
		if (it->GetIsExported() && it->GetIsInput()) {
			_log.Log(LOG_NORM, "GPIO: Hooking interrupt handler for GPIO %d.", it->GetId());
			switch (it->GetId()) {
				case 0:	wiringPiISR(0, INT_EDGE_SETUP, &interruptHandler0_c); break;
				case 1: wiringPiISR(1, INT_EDGE_SETUP, &interruptHandler1_c); break;
				case 2: wiringPiISR(2, INT_EDGE_SETUP, &interruptHandler2_c); break;
				case 3: wiringPiISR(3, INT_EDGE_SETUP, &interruptHandler3_c); break;	
				case 4: wiringPiISR(4, INT_EDGE_SETUP, &interruptHandler4_c); break;

				case 7: wiringPiISR(7, INT_EDGE_SETUP, &interruptHandler7_c); break;
				case 8: wiringPiISR(8, INT_EDGE_SETUP, &interruptHandler8_c); break;
				case 9: wiringPiISR(9, INT_EDGE_SETUP, &interruptHandler9_c); break;
				case 10: wiringPiISR(10, INT_EDGE_SETUP, &interruptHandler10_c); break;
				case 11: wiringPiISR(11, INT_EDGE_SETUP, &interruptHandler11_c); break;

				case 14: wiringPiISR(14, INT_EDGE_SETUP, &interruptHandler14_c); break;
				case 15: wiringPiISR(15, INT_EDGE_SETUP, &interruptHandler15_c); break;

				case 17: wiringPiISR(17, INT_EDGE_SETUP, &interruptHandler17_c); break;
				case 18: wiringPiISR(18, INT_EDGE_SETUP, &interruptHandler18_c); break;

				case 20: wiringPiISR(20, INT_EDGE_SETUP, &interruptHandler20_c); break;
				case 21: wiringPiISR(21, INT_EDGE_SETUP, &interruptHandler21_c); break;
				case 22: wiringPiISR(22, INT_EDGE_SETUP, &interruptHandler22_c); break;
				case 23: wiringPiISR(23, INT_EDGE_SETUP, &interruptHandler23_c); break;
				case 24: wiringPiISR(24, INT_EDGE_SETUP, &interruptHandler24_c); break;
				case 25: wiringPiISR(25, INT_EDGE_SETUP, &interruptHandler25_c); break;

				case 27: wiringPiISR(27, INT_EDGE_SETUP, &interruptHandler27_c); break;
				case 28: wiringPiISR(28, INT_EDGE_SETUP, &interruptHandler28_c); break;
				case 29: wiringPiISR(29, INT_EDGE_SETUP, &interruptHandler29_c); break;
				case 30: wiringPiISR(30, INT_EDGE_SETUP, &interruptHandler30_c); break;
				case 31: wiringPiISR(31, INT_EDGE_SETUP, &interruptHandler31_c); break;

				default:
					_log.Log(LOG_ERROR, "GPIO: Error hooking interrupt handler for unknown GPIO %d.", it->GetId());
			}
		}
	}
	_log.Log(LOG_NORM, "GPIO: WiringPi is now initialized");
#endif
	sOnConnected(this);

	return (m_thread!=NULL);
}


bool CCodesys::StopHardware()
{
	if (m_thread != NULL) {
		m_stoprequested=true;
		interruptCondition_c.notify_one();
		m_thread->join();
	}

	m_bIsStarted=false;

	return true;
}


bool CCodesys::WriteToHardware(const char *pdata, const unsigned char length)
{
#ifndef WIN32
	const tRBUF *pCmd = reinterpret_cast<const tRBUF*>(pdata);

	if ((pCmd->LIGHTING1.packettype == pTypeLighting1) && (pCmd->LIGHTING1.subtype == sTypeIMPULS)) {
		unsigned char housecode = (pCmd->LIGHTING1.housecode);
			int gpioId = pCmd->LIGHTING1.unitcode;
			int newValue = static_cast<int>(pCmd->LIGHTING1.cmnd);
			_log.Log(LOG_NORM,"Codesys: pin #%d state new %d", gpioId, newValue);
			memset(pMemory + (gpioId - 1)+(housecode-65)*64,newValue,1);
			}
	else {
		_log.Log(LOG_NORM,"GPIO: WriteToHardware packet type %d or subtype %d unknown", pCmd->LIGHTING1.packettype, pCmd->LIGHTING1.subtype);
		return false;
	}
	return true;
#else
	return false;
#endif
}


void CCodesys::ProcessInterrupt(int gpioId, int housecode) {
#ifndef WIN32
	_log.Log(LOG_NORM, "Codesys: Processing interrupt for sensor %d...", gpioId);

	
	int value = Act_Mem[15+gpioId+(housecode-65)*64];

	IOPinStatusPacket.LIGHTING1.housecode=housecode;

	if (value != 0) {
		IOPinStatusPacket.LIGHTING1.cmnd = light1_sOn;
		 _log.Log(LOG_NORM, "Codesys: Set to On");
	}
	else {
		IOPinStatusPacket.LIGHTING1.cmnd = light1_sOff;
		_log.Log(LOG_NORM, "Codesys: Set to Off");
	}

	unsigned char seqnr = IOPinStatusPacket.LIGHTING1.seqnbr;
	seqnr++;
	IOPinStatusPacket.LIGHTING1.seqnbr = seqnr;
	IOPinStatusPacket.LIGHTING1.unitcode = gpioId;

	sDecodeRXMessage(this, (const unsigned char *)&IOPinStatusPacket, NULL, 255);

	_log.Log(LOG_NORM, "Codesys: Done processing interrupt for sensor %d.", gpioId);
#endif
}

void CCodesys::Do_Work()
{
/*	int interruptNumber = NO_INTERRUPT;
	boost::posix_time::milliseconds duration(12000);
	std::vector<int> triggers;
*/
	_log.Log(LOG_NORM,"Codesys: Worker started...");


	while (!m_stoprequested) {
/*		boost::mutex::scoped_lock lock(interruptQueueMutex_c);
		if (!interruptCondition_c.timed_wait(lock, duration)) {
			//_log.Log(LOG_NORM, "GPIO: Updating heartbeat");
			mytime(&m_LastHeartbeat);
		} else {
			while (!gpioInterruptQueue_c.empty()) {
				interruptNumber = gpioInterruptQueue_c.front();
				triggers.push_back(interruptNumber);
				gpioInterruptQueue_c.erase(gpioInterruptQueue_c.begin());
				_log.Log(LOG_NORM, "GPIO: Acknowledging interrupt for GPIO %d.", interruptNumber);
			}
		}
		lock.unlock();

		if (m_stoprequested) {
			break;
		}

    	while (!triggers.empty()) {
		interruptNumber = triggers.front();
		triggers.erase(triggers.begin());
		CCodesys::ProcessInterrupt(interruptNumber);
		}
#else*/
		mytime(&m_LastHeartbeat);
	 	sleep_milliseconds(1000);
		memcpy(Act_Mem,pMemory,1024);

		//_log.Log(LOG_NORM, "Codesys: val = %i",Act_Mem[16]);
		for (int j=0;j<16;j++) {
		for (int i=0;i<16;i++) {if (Old_Mem[(16+i)+64*j] != Act_Mem[(16+i)+64*j]) {
			_log.Log(LOG_NORM, "Codesys: Changing on sensor %i detected", i+1);
			CCodesys::ProcessInterrupt(i+1,65+j);}
			} 
			}
		memcpy(Old_Mem,Act_Mem,1024);

	}
	_log.Log(LOG_NORM,"Codesys: Worker stopped...");
	munmap(pMemory,1024);
   	shm_unlink("mymemory");
}

/*
 * static
 * One-shot method to initialize pins
 *
 */
bool CCodesys::InitPins()
{
   char buf[128]={0};
   int fd = shm_open("mymemory", O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);
   ftruncate(fd, 1024);
   pMemory = mmap(0, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
   close(fd);
   memset(pMemory,0 ,1024);
   memset(Old_Mem,0, 1024);
return true;
}

/* static */
std::vector<CGpioPin> CCodesys::GetPinList()
{
	return pins;
}

/* static */
CGpioPin* CCodesys::GetPPinById(int id)
{
	for(std::vector<CGpioPin>::iterator it = pins.begin(); it != pins.end(); ++it) {
		if (it->GetId() == id) {
			return &(*it);
		}
	}
	return NULL;
}

#endif // WITH_GPIO
