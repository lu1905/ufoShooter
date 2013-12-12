#include "Texture.h"

void Texture::load(const char * filename)
{
    CImg<uint8_t> tex(filename);

        data = new uint8_t[tex.width() * tex.height() * tex.spectrum()];
        
    int pos = 0;
        for(int i = 0; i < tex.height(); i++)
    {
        for(int j = 0; j < tex.width(); j++)
        {
            for(int k = 0; k < tex.spectrum(); k++)
            {
                data[pos++] = tex(j, i, 0, k);
            }
            }
        }
    
    height = tex.height();
    width = tex.width();
    spectrum = tex.spectrum();
}
