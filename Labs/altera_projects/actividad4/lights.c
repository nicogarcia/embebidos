#define Switches (volatile char *) 0x0003000
#define LEDs (char *) 0x0003010

void main()
{ 
	while (1)
		*LEDs = *Switches;
}