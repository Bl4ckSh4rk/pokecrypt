//    Generation 3 - 7 Pokemon decrypter/encrypter
//    Copyright (C) 2017  BlackShark

//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.

//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *readBytes(char *data, int offset, int count)
{
    char* bytes = 0;
    int x;
    for(x = 0; x < count; x++)
    {
        bytes += ((data[offset + x] & 0xFF) << (8 * x));
    }
    return bytes;
}

int pkmGBAChecksum(char *data)
{
	int checksum = 0, x;
	for(x = 0; x < 24; x++)
        checksum += (data[x * 2] & 0xFF) + ((data[x * 2 + 1] & 0xFF) << 8);

    checksum = (checksum & 0xFFFF);

	return checksum;
}

unsigned int pkmNDSChecksum(char *data)
{
	unsigned int checksum = 0, x;
	for(x = 0; x < 64; x++)
        checksum += (data[x * 2] & 0xFF) + ((data[x * 2 + 1] & 0xFF) << 8);

    checksum = (checksum & 0xFFFF) << 16;

	return checksum;
}

unsigned int pkm3DSChecksum(char *data)
{
	unsigned int checksum = 0, x;
	for(x = 0; x < 112; x++)
        checksum += (data[x * 2] & 0xFF) + ((data[x * 2 + 1] & 0xFF) << 8);

    checksum = (checksum & 0xFFFF) << 16;

	return checksum;
}

int shuffleStructure[24][4] = {
    {0,1,2,3,},
    {0,1,3,2,},
    {0,2,1,3,},
    {0,2,3,1,},
    {0,3,1,2,},
    {0,3,2,1,},
    {1,0,2,3,},
    {1,0,3,2,},
    {1,2,0,3,},
    {1,2,3,0,},
    {1,3,0,2,},
    {1,3,2,0,},
    {2,0,1,3,},
    {2,0,3,1,},
    {2,1,0,3,},
    {2,1,3,0,},
    {2,3,0,1,},
    {2,3,1,0,},
    {3,0,1,2,},
    {3,0,2,1,},
    {3,1,0,2,},
    {3,1,2,0,},
    {3,2,0,1,},
    {3,2,1,0,}
};

int unshuffleStructure[24][4] = {
    {0,1,2,3,},
    {0,1,3,2,},
    {0,2,1,3,},
    {0,3,1,2,},
    {0,2,3,1,},
    {0,3,2,1,},
    {1,0,2,3,},
    {1,0,3,2,},
    {2,0,1,3,},
    {3,0,1,2,},
    {2,0,3,1,},
    {3,0,2,1,},
    {1,2,0,3,},
    {1,3,0,2,},
    {2,1,0,3,},
    {3,1,0,2,},
    {2,3,0,1,},
    {3,2,0,1,},
    {1,2,3,0,},
    {1,3,2,0,},
    {2,1,3,0,},
    {3,1,2,0,},
    {2,3,1,0,},
    {3,2,1,0,}
};

void shufflePkmData(unsigned int key, char *data_in, char *data_out, int sys)
{
    int x, y, algorythm, blockSize;

    if(sys == 0)
    {
        algorythm = key % 24;
        blockSize = 12;
    }
    else if(sys == 1)
    {
        algorythm = ((key & 0x3E000) >> 0xD) % 24;//((key >> 0xD) & 0x1F) % 24;
        blockSize = 32;
    }
    else if(sys == 2)
    {
        algorythm = ((key & 0x3E000) >> 0xD) % 24;//((key >> 0xD) & 0x1F) % 24;
        blockSize = 56;
    }

    for(x = 0; x < 4; x++)
    {
        int pos = shuffleStructure[algorythm][x];
        for(y = 0; y < blockSize; y++)
        {
            data_out[x*blockSize+y] = data_in[pos*blockSize+y]&0xFF;
        }
    }
}

void unshufflePkmData(unsigned int key, char *data_in, char *data_out, int sys)
{
    int x, y, algorythm, blockSize;

    if(sys == 0)
    {
        algorythm = key % 24;
        blockSize = 12;
    }
    else if(sys == 1)
    {
        algorythm = ((key & 0x3E000) >> 0xD) % 24;//((key >> 0xD) & 0x1F) % 24;
        blockSize = 32;
    }
    else if(sys == 2)
    {
        algorythm = ((key & 0x3E000) >> 0xD) % 24;//((key >> 0xD) & 0x1F) % 24;
        blockSize = 56;
    }

    for(x = 0; x < 4; x++)
    {
        int pos = unshuffleStructure[algorythm][x];
        for(y = 0; y < blockSize; y++)
        {
            data_out[x*blockSize+y] = data_in[pos*blockSize+y]&0xFF;
        }
    }
}

void pkmGBAcrypt(unsigned int seed, char *data_in, char *data_out)
{
    int x;
    unsigned int tempData;
    for(x = 0; x < 12; x++)
    {
            tempData = seed ^ (unsigned int)readBytes(data_in, 4*x, 4);

            data_out[4*x +0] = tempData;
            data_out[4*x +1] = tempData>>8;
            data_out[4*x +2] = tempData>>16;
            data_out[4*x +3] = tempData>>24;
    }
}

void pkmNDScrypt(unsigned long seed, char *data_in, char *data_out, int size)
{
    int x;
    unsigned int tempData;
    for(x = 0; x < size/2; x++)
    {
            seed = ((0x41C64E6D * seed) + 0x6073) & 0xFFFFFFFF;
            tempData = (seed >> 16) ^ (unsigned int)readBytes(data_in, 2*x, 2);

            data_out[2*x +0] = tempData;
            data_out[2*x +1] = tempData>>8 ;
    }
}

void exitTool()
{
    printf("\n\nPress ENTER to exit...");
    while(1)
    {
        if (getchar())
            break;
    }
}

int main(int argc, char** argv)
{
    FILE *fp_in, *fp_out;
    char *partData1, *partData2_in, *partData2, *partData2_out, *partData3 = "";
    int fSize = 0;
    unsigned int tid, pid, encryptionKey = 0, checksum = 0;

    printf("\nGeneration 3 - 7 Pokemon decrypter/encrypter\n");
    if(argc != 4)
	{
	    printf("\nUsage:\n");
	    printf("pokecrypt.exe inpkm [-d|-e] outpkm\n");
	    printf("\t-d\tdecrypt Pokemon file\n");
	    printf("\t-e\tencrypt Pokemon file\n");
	    printf("\tinpkm\tPokemon file to decrypt/encrypt\n");
	    printf("\toutpkm\tdecrypted/encrypted Pokemon file\n\n");
	    exitTool();
        return 0;
	}
    if(strcmp(argv[2], "-d") && strcmp(argv[2], "-e"))
    {
        printf("\nArgument error. Third argument should be either -d or -e\n\n");
        exitTool();
        return 0;
    }

    fp_in = fopen(argv[1], "rb");
    if (!fp_in)
    {
        printf("\nPokemon loading failed!\n\n");
        exitTool();
        return 0;
    }

    // get file size and read whole content into memory
    fseek (fp_in, 0, SEEK_END);
    fSize = ftell(fp_in);
    rewind (fp_in);

    if(fSize != 0x50 && fSize != 0x64 && fSize != 0x88 && fSize != 0xDC && fSize != 0xEC && fSize != 0xE8 && fSize != 0x104)
    {
        printf("\nFilesize error.\n"
               "3rd Generation Pokemon data should be:\n"
               "80 Bytes (box, excluding status conditions)\n"
               "100 Bytes (party, including status conditions)\n\n"
               "4th Generation Pokemon data should be:\n"
               "136 Bytes (box, excluding status conditions)\n"
               "236 Bytes (party, including status conditions)\n\n"
               "5th Generation Pokemon data should be:\n"
               "136 Bytes (box, excluding status conditions)\n"
               "220 Bytes (party, including status conditions)\n\n"
               "6th and 7th Generation Pokemon data should be:\n"
               "232 Bytes (box, excluding status conditions)\n"
               "260 Bytes (party, including status conditions)\n\n");
        exitTool();
        return 0;
    }

    int sys = 0, part1Size = 0, part2Size = 0, part3Size = 0;
    if(fSize == 0x50 || fSize == 0x64)
    {
        sys = 0;
        part1Size = 0x20;
        part2Size = 0x30;
        part3Size = 0x14;
    }
    else if(fSize == 0x88 || fSize == 0xDC || fSize == 0xEC)
    {
        sys = 1;
        part1Size = 0x08;
        part2Size = 0x80;
        if(fSize == 0xDC)
            part3Size = 0x54;
        else
            part3Size = 0x64;
    }
    else if(fSize == 0xE8 || fSize == 0x104)
    {
        sys = 2;
        part1Size = 0x08;
        part2Size = 0xE0;
        part3Size = 0x1C;
    }

    //split the pkm data into 3 parts, 2nd part needs decryption/encryption
    partData1 = (char *)malloc(part1Size);
    if(partData1 == NULL)
    {
        printf("Memory allocation error!\nNot enough free memory available.\n\n");
        exitTool();
        return 0;
    }
    fread(partData1, 1, part1Size, fp_in);
    partData2_in = (char *)malloc(part2Size);
    if(partData2_in == NULL)
    {
        printf("Memory allocation error!\nNot enough free memory available.\n\n");
        exitTool();
        return 0;
    }
    fread(partData2_in, 1, part2Size, fp_in);
    if(fSize == 0x64 || fSize == 0xDC || fSize == 0xEC || fSize == 0x104)
    {
        partData3 = (char *)malloc(part3Size);
        if(partData3 == NULL)
        {
            printf("Memory allocation error!\nNot enough free memory available.\n\n");
            exitTool();
            return 0;
        }
        fread(partData3, 1, part3Size, fp_in);
    }
    fclose(fp_in);

    //get pokemon pid
    pid = (unsigned int)readBytes(partData1, 0, 4);
    partData2 = (char *)malloc(part2Size);
    if(partData2 == NULL)
    {
        printf("Memory allocation error!\nNot enough free memory available.\n\n");
        exitTool();
        return 0;
    }
    partData2_out = (char *)malloc(part2Size);
    if(partData2_out == NULL)
    {
        printf("Memory allocation error!\nNot enough free memory available.\n\n");
        exitTool();
        return 0;
    }

    if(sys == 0)
    {
        tid = (unsigned int)readBytes(partData1, 4, 4);
        encryptionKey = pid ^ tid;

        if(!strcmp(argv[2],"-d"))
        {
            unshufflePkmData(pid, partData2_in, partData2, sys);
            pkmGBAcrypt(encryptionKey, partData2, partData2_out);
            checksum = pkmGBAChecksum(partData2_out);
        }
        else if(!strcmp(argv[2],"-e"))
        {
            shufflePkmData(pid, partData2_in, partData2, sys);
            checksum = pkmGBAChecksum(partData2_in);
            pkmGBAcrypt(encryptionKey, partData2, partData2_out);
        }
    }
    else if(sys == 1)
    {
        if(!strcmp(argv[2],"-d"))
        {
            checksum = (unsigned int)readBytes(partData1, 4, 4);
            pkmNDScrypt((checksum >> 16), partData2_in, partData2, 0x80);
            unshufflePkmData(pid, partData2, partData2_out, sys);
            checksum = pkmNDSChecksum(partData2_out);

            if(fSize == 0xDC || fSize == 0xEC)
            {
                pkmNDScrypt(pid, partData3, partData3, part3Size);
            }
        }
        else if(!strcmp(argv[2],"-e"))
        {
            checksum = pkmNDSChecksum(partData2_in);
            shufflePkmData(pid, partData2_in, partData2, sys);
            pkmNDScrypt((checksum >> 16), partData2, partData2_out, 0x80);

            if(fSize == 0xDC || fSize == 0xEC)
            {
                pkmNDScrypt(pid, partData3, partData3, part3Size);
            }
        }
    }
    else if(sys == 2)
    {
        encryptionKey = (unsigned int)readBytes(partData1, 0, 4);
        if(!strcmp(argv[2],"-d"))
        {
            pkmNDScrypt(encryptionKey, partData2_in, partData2, 0xE0);
            unshufflePkmData(encryptionKey, partData2, partData2_out, sys);
            checksum = pkm3DSChecksum(partData2_out);

            if(fSize == 0x104)
            {
                pkmNDScrypt(encryptionKey, partData3, partData3, part3Size);
            }
        }
        else if(!strcmp(argv[2],"-e"))
        {
            checksum = pkm3DSChecksum(partData2_in);
            shufflePkmData(encryptionKey, partData2_in, partData2, sys);
            pkmNDScrypt(encryptionKey, partData2, partData2_out, 0xE0);

            if(fSize == 0x104)
            {
                pkmNDScrypt(encryptionKey, partData3, partData3, part3Size);
            }
        }
    }
    free(partData2_in);
    free(partData2);

    fp_out = fopen(argv[3], "wb+");
    if (!fp_out)
    {
        printf("\nWriting Pokemon file failed!\n\n");
        exitTool();
        return 0;
    }

    // create a decrypted/encrypted pkm file with fixed checksum
    if(sys == 0)
    {
        partData1[0x1D] = (checksum >> 8) & 0xFF;
        partData1[0x1C] = checksum & 0xFF;
    }
    else if(sys == 1 || sys == 2)
    {
        partData1[0x07] = (checksum >> 24) & 0xFF;
        partData1[0x06] = (checksum >> 16) & 0xFF;
        partData1[0x05] = (checksum >> 8) & 0xFF;
        partData1[0x04] = checksum & 0xFF;
    }

    fwrite(partData1, 1, part1Size, fp_out);
    free(partData1);
    fwrite(partData2_out, 1, part2Size, fp_out);
    free(partData2_out);
    if(fSize == 0x64 || fSize == 0xDC || fSize == 0xEC || fSize == 0x104)
    {
        fwrite(partData3, 1, part3Size, fp_out);
        free(partData3);
    }
    fclose(fp_out);

    if(!strcmp(argv[2],"-d"))
        printf("\nPokemon file decrypted!\n\n");

    else if(!strcmp(argv[2],"-e"))
        printf("\nPokemon file encrypted!\n\n");

    exitTool();
    return 0;
}
