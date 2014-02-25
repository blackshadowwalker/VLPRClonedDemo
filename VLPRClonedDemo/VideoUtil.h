#pragma once

class VideoUtil
{
public:
	VideoUtil(void);
	~VideoUtil(void);

	static int write24BitBmpFile(char *filename, unsigned int width, unsigned int height, unsigned char *image,bool isBGR= true , int widthstep=0 ) ;
};
