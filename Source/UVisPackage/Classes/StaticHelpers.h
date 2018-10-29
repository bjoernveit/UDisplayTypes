#pragma once

#include "CoreMinimal.h"


struct StaticHelpers
{
	
	static FColor GetInBetweenColor(FColor StartColor, FColor EndColor, double Fraction)
	{
		if (StartColor == EndColor)
			return EndColor;

		uint8 NewA = StartColor.A + ((EndColor.A - StartColor.A) *  Fraction);
		uint8 NewR = StartColor.R + ((EndColor.R - StartColor.R) *  Fraction);
		uint8 NewG = StartColor.G + ((EndColor.G - StartColor.G) *  Fraction);
		uint8 NewB = StartColor.B + ((EndColor.B - StartColor.B) *  Fraction);
		
		return FColor(NewR, NewG, NewB, NewA);
	}


};