#include "RandHelper.h"
#include "mtrand.h"

#include <time.h>

//use time(0) as a seed for now, switch to packet delays later
//haters gonna hate

bool iInit = false;
unsigned long iRand()
{
	if(iInit)
	{
		return MTRand_int32()();
	}
	else
	{
		iInit = true;
		return MTRand_int32((unsigned long(time(0))))();
	}
}

unsigned long iRand(int a_Max)
{
	return int(fRand() * a_Max);
}

unsigned long iRand(int a_Min, int a_Max)
{
	if(a_Min == a_Max)
		return a_Min;
	return a_Min + iRand() % (a_Max - a_Min);
}

bool fInit = false;
double fRand()
{
	if(fInit)
	{
		return MTRand()();
	}
	else
	{
		fInit = true;
		return MTRand((unsigned long(time(0))))();
	}
}

double fRand(float a_Max)
{
	return fRand() * a_Max;
}

double fRand(float a_Min, float a_Max)
{
	if(a_Min == a_Max)
		return a_Min;
	return a_Min + fRand() * (a_Max - a_Min);
}
