#include "sed.h"
#include "sceChnnlsv.h"
#include "MemMap.h"
#include <iostream>

//#define DEBUG

using namespace std;

void ShowHelp(char* progName)
{
	cout<<"PSP Savedata Encrypter/Decrypter for PC by hgoel0974 \n";
	cout<<"Usage: \n\tsed -e [Input File] [PARAM.SFO] [Output File] [Game Key File]\n";
	cout<<"\tsed -d [Input File] [Output File] [Game Key File]\n";
}

int main ( int argc, char *argv[] )
{
	if(argc >=4){

		int ret = 0;

		Memory::Init();
		init_chnnlsv();

		unsigned char gameKey[0x11] = {" "};//(unsigned char*)malloc(0x10 * sizeof(unsigned char));

		char out[200] = "";
		strcpy(out, argv[3]);
		strcat(out, argv[4]);

		if(strncmp(argv[1],"-e",2)==0)
		{
			FILE* key = fopen(argv[5], "rb");
			if(key > 0){
				fread((void*)gameKey, 1, 0x10, key);
				fclose(key);
			}else{
				cout<<"Failed to open key file";
				return 0;
			}

			//ARGV 1 = File to Encrypt
			//ARGV 2 = Path to PARAM.SFO
			//ARGV 3 = Output file name
			//ARGV 4 = Game key file
			char fname[30] = "", ext[10] = "";

			_splitpath(argv[4], NULL, NULL, fname, ext);

			strcat(fname, ext);

			ret = encrypt_file(argv[2], argv[4],  fname, argv[3], gameKey); 
		}else if(strncmp(argv[1],"-d",2)==0)
		{
			FILE* key = fopen(argv[4], "rb");
			if(key > 0){
				fread((void*)gameKey, 1, 0x10, key);
				fclose(key);
			}else{
				cout<<"Failed to open key file";
				return 0;
			}

			//ARGV 1 = File to Decrypt
			//ARGV 2 = Output File name
			//ARGV 3 = Game key file
			ret = decrypt_file(argv[3], argv[2], gameKey);
		}else{
			Memory::Dispose();
			//free(gameKey);
			ShowHelp(argv[0]);
			return 0;
		}

		//free(gameKey);
		Memory::Dispose();

		cout<<"Program exited with code : "<<ret;
	}else{
		ShowHelp("sed");
	}
	return 0;
}