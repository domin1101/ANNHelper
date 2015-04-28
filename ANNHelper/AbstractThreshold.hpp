#pragma once

#ifndef _ABSTRACTTHRESHOLD_H_
#define _ABSTRACTTHRESHOLD_H_

// A abstract threshold
class AbstractThreshold
{
private:
public:
	virtual ~AbstractThreshold() {}
	virtual AbstractThreshold* getCopy() = 0;
};

#endif