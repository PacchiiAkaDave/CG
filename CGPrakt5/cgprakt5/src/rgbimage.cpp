#include "rgbimage.h"
#include "color.h"
#include "assert.h"

RGBImage::RGBImage( unsigned int Width, unsigned int Height)
{
	m_Width = Width;
	m_Height = Height;
	m_Image = new Color[m_Width * m_Height];
	// TODO: add your code
}

RGBImage::~RGBImage()
{
	delete m_Image;
	// TODO: add your code
}

void RGBImage::setPixelColor( unsigned int x, unsigned int y, const Color& c)
{
	if (y * m_Width + x <= m_Width * m_Height) {
		m_Image[y * m_Width + x] = c;
	}
	// TODO: add your code

}

const Color& RGBImage::getPixelColor( unsigned int x, unsigned int y) const
{
	if (y * m_Width + x <= m_Width * m_Height) {
		return m_Image[y * m_Width + x];
	}
}

unsigned int RGBImage::width() const
{
	// TODO: add your code
	return m_Width;
}

unsigned int RGBImage::height() const
{
	// TODO: add your code
	return m_Height;
}

unsigned char RGBImage::convertColorChannel( float v)
{
	char c;
	if (v <= 1.0f && v >= 0.0f) {
		c = v * 255;
	}
	if (v < 0.0f) {
		c = 0;
	}
	if (v > 1.0f) {
		c = 255;
	}
	// TODO: add your code
	return c;
}


bool RGBImage::saveToDisk( const char* Filename)
{
	FILE* f;
	errno_t err = fopen_s(&f,Filename,"wb");

	unsigned char bmpheader[54];

	int iSize = 3 * m_Width * m_Height;
	int fileSize = iSize + 54;

	for (int i = 0; i < 54; i++) {
		bmpheader[i] = 0;
	}
	//erste 14 Bytes BitmapFileheader
	//ASCII Zeichenkette "BM"
	bmpheader[0] = 'B';
	bmpheader[1] = 'M';
	//Filesize
	bmpheader[2] = (unsigned char)(fileSize);
	bmpheader[3] = (unsigned char)(fileSize >> 8);
	bmpheader[4] = (unsigned char)(fileSize >> 16);
	bmpheader[5] = (unsigned char)(fileSize >> 24);

	bmpheader[10] = 54; //aufgrund 14 Fileblocks und 40 Infoblocks

	//BitmapInfoheader
	bmpheader[14] = 40; //Info header size

	//Breite der Bitmap
	bmpheader[18] = (unsigned char)(m_Width);
	bmpheader[19] = (unsigned char)(m_Width >> 8);
	bmpheader[20] = (unsigned char)(m_Width >> 16);
	bmpheader[21] = (unsigned char)(m_Width >> 24);

	//Hoehe der Bitmap (negativer Wert damit Top-Down) 
	bmpheader[22] = (unsigned char)(m_Height*-1);
	bmpheader[23] = (unsigned char)(m_Height*-1 >> 8);
	bmpheader[24] = (unsigned char)(m_Height*-1 >> 16);
	bmpheader[25] = (unsigned char)(m_Height*-1 >> 24);

	bmpheader[26] = 1; //biplanes Wert 1

	//Farbtiefe
	bmpheader[28] = 24;

	//biImageSize
	bmpheader[34] = (unsigned char)(iSize);
	bmpheader[35] = (unsigned char)(iSize >> 8);
	bmpheader[36] = (unsigned char)(iSize >> 16);
	bmpheader[37] = (unsigned char)(iSize >> 24);

	fwrite(bmpheader, 1, 54, f);

	for (int y = 0; y < m_Height; y++) {
		for (int x = 0; x < m_Width; x++) {
			Color c = m_Image[(y * m_Width) + x];
			unsigned char R = convertColorChannel(c.R);
			unsigned char G = convertColorChannel(c.G);
			unsigned char B = convertColorChannel(c.B);
			fwrite(&B, 1, 1, f);
			fwrite(&G, 1, 1, f);
			fwrite(&R, 1, 1, f);
		}
	}

	fclose(f);
	// TODO: add your code
	return true;
}
