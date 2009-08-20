#include <cassert>

void hueToRgb(float hue, float rgb[3])
{
	assert(hue>=0.0f);
	assert(hue<=1.0f);

	if (hue<=(1.0f/6.0f))
	{
		rgb[0]=1.0f;
		rgb[1]=(hue-(0.0f/6.0f))/(1.0f/6.0f); // ascending
		rgb[2]=0.0f;
	}
	else if (hue<=(2.0f/6.0f))
	{
		rgb[0]=((2.0f/6.0f)-hue)/(1.0f/6.0f); // descending
		rgb[1]=1.0f;
		rgb[2]=0.0f;
	}
	else if (hue<=(3.0/6.0))
	{
		rgb[0]=0.0f;
		rgb[1]=1.0f;
		rgb[2]=(hue-(2.0f/6.0f))/(1.0f/6.0f); // ascending
	}
	else if (hue<=(4.0f/6.0f))
	{
		rgb[0]=0.0f;
		rgb[1]=((4.0f/6.0f)-hue)/(1.0f/6.0f); // descending
		rgb[2]=1.0f;
	}
	else if (hue<=(5.0f/6.0f))
	{
		rgb[0]=(hue-(4.0f/6.0f))/(1.0f/6.0f); // ascending
		rgb[1]=0.0f;
		rgb[2]=1.0f;
	}
	else
	{
		rgb[0]=1.0f;
		rgb[1]=0.0f;
		rgb[2]=((6.0f/6.0f)-hue)/(1.0f/6.0f); // descending
	}
}
