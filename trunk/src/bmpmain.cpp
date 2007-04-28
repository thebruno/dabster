#define _CRT_SECURE_NO_DEPRECATE
#include "bmp.h"

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <ctime>
using namespace bmptypes;



std::ostream & operator << (std::ostream & out, smap x) {

		out << "name = " << x["name"] << std::endl;
		out << "relativePath = " << x["relativePath"] << std::endl;
		out << "realPath = " << x["realPath"] << std::endl;
		out << "volumeLabel = " << x["volumeLabel"] << std::endl;
		out << "driveFormat = " << x["driveFormat"] << std::endl;
		out << "driveType = " << x["driveType"] << std::endl;

		out << "availableFreeSpace = " << x["availableFreeSpace"] << std::endl;
		out << "totalFreeSpace = " << x["totalFreeSpace"] << std::endl;
		out << "totalSize = " << x["totalSize"] << std::endl;
		out << "isReady = " << x["isReady"] << std::endl;

		out << "creationTimeUtc = " << x["creationTimeUtc"] << std::endl;
		out << "lastWriteTimeUtc = " << x["lastWriteTimeUtc"] << std::endl;
		out << "lastAccessTimeUtc = " << x["lastAccessTimeUtc"] << std::endl;
		out << "length = " << x["length"] << std::endl;

		out << "atrTemporary = " << x["atrTemporary"] << std::endl;
		out << "atrSystem = " << x["atrSystem"] << std::endl;
		out << "atrSparseFile = " << x["atrSparseFile"] << std::endl;
		out << "atrArchive = " << x["atrArchive"] << std::endl;

		out << "atrCompressed = " << x["atrCompressed"] << std::endl;
		out << "atrEncrypted = " << x["atrEncrypted"] << std::endl;
		out << "atrHidden = " << x["atrHidden"] << std::endl;
		out << "atrNotContentIndexed = " << x["atrNotContentIndexed"] << std::endl;

		out << "atrOffline = " << x["atrOffline"] << std::endl;
		out << "atrReadOnly = " << x["atrReadOnly"] << std::endl;
		out << "atrDirectory = " << x["atrDirectory"] << std::endl;
		return out;
	}

std::ostream & operator << (std::ostream & out, vmstring x) {
		unsigned int i;
		for (i = 0; i <x.size(); ++i) {
			out << x[i] << std::endl;
		}
		return out;
	}


int main() {
	std::string nazwa = "1.bmp";

	smap plik1_in, plik2_in, plik3_in;
	smap plik1_inside, plik2_inside, plik3_inside;
	smap plik1_out, plik2_out, plik3_out;

	vmstring sciezki1, sciezki2, sciezki3, sciezki4, wynik;
	plik1_in["name"] = "1.txt"; plik1_in["length"] = "3128"; plik1_in["realPath"]= "";
	sciezki1.push_back(plik1_in);
	plik2_in["name"] = "2.txt"; plik2_in["length"] = "3055"; plik2_in["realPath"]= "";
	sciezki1.push_back(plik2_in);
	plik3_in["name"] = "3.txt"; plik3_in["length"] = "4158"; plik3_in["realPath"]= "";
	//sciezki1.push_back(plik3_in);

	plik1_inside["creationTimeUtc"] = "25.04.2007 08:56:33"; plik1_inside["name"] = "1.txtinbmp";
	plik1_inside["atrReadOnly"] = "true"; plik1_inside["relativePath"] = "";
	//plik1_inside["atrDirectory"] = "true";
	sciezki2.push_back(plik1_inside);

	plik2_inside["creationTimeUtc"] = "01.12.2007 20:56:27"; plik2_inside["name"] = "2.txtinbmp";
	plik2_inside["atrSystem"] = "true"; plik2_inside["relativePath"] = "";
	//plik2_inside["atrDirectory"] = "true";
	sciezki2.push_back(plik2_inside);

	plik3_inside["creationTimeUtc"] = "01.12.2007 20:56:27"; plik3_inside["name"] = "3.txtinbmp";
	plik3_inside["atrHidden"] = "true"; plik3_inside["relativePath"] = "";
	//plik3_inside["atrDirectory"] = "true";
	//sciezki2.push_back(plik3_inside);

	plik1_out["name"] = "wynik1.txt"; plik1_out["realPath"]= "";
	sciezki3.push_back(plik1_out);

	plik2_out["name"] = "wynik2.txt"; plik2_out["realPath"]= "";
	sciezki3.push_back(plik2_out);

	plik3_out["name"] = "wynik3.txt"; plik3_out["realPath"]= "";
	sciezki3.push_back(plik3_out);

	sciezki4.push_back(plik2_inside);



	bmp::BMP * b = 0;
	sfile sf;
	sf.setRealPath(nazwa);
	if (bmp::isBmp(sf)) {

		try {
			b = new bmp::BMP(sf);
			std:: cout << (wynik = b->getContent(string("")));
			if (wynik.empty() && b->getCompression() == 0) {
				b->setCompression(2);
			}
			double t1,t2, t;
			t1 = clock();

			//b->store(sciezki1,sciezki2);
			//b->del(sciezki4);
			 b->extract(sciezki2,sciezki3);
			t2 = clock();
			t = t2-t1;

			if (!t) t = 1;
				std::cout << 0.0 / t<< " KB/s" << std::endl;
			delete b;
		}

		catch(DAB_EXCEPTION& )	{
			std::cout << "BLAD :/";
		}
	}

	// zmienic nazwe buff mode!!!!!!!

/******************************************
kiedy ustawiac bufory i czy robic recznie fill/flush??
napisac funkcje flush i fill dla obu bufforow
przy konstruktorze wyjatki?



*******************************************/


	return 0;
}

/*
name	string

volumeLabel	string

driveFormat	NTFS, FAT32, DMD = DabMailDrive itp.

driveType	CDRom, Fixed, Network, NoRootDirectory, Ram, Removable, Mail, Unknown

availableFreeSpace	liczba [w bajtach]

totalFreeSpace	liczba [w bajtach]

totalSize	liczba [w bajtach]

isReady	true, false

atrTemporary	true, false
atrSystem	true, false
atrSparseFile	true, false
atrArchive	true, false
atrCompressed	true, false
atrEncrypted	true, false
atrHidden	true, false
atrNotContentIndexed	true, false
atrOffline	true, false
atrReadOnly	true, false
atrDirectory	true, false



*/



		/*


name
volumeLabel
driveFormat
driveType
availableFreeSpace
totalFreeSpace
totalSize
isReady

creationTimeUtc
lastWriteTimeUtc
lastAccessTimeUtc
length

atrTemporary
atrSystem
atrSparseFile
atrArchive
atrCompressed

atrEncrypted
atrHidden

atrNotContentIndexed
atrOffline
atrReadOnly

atrDirectory


*/


/*

dodac sprawdzanie bCancel i przerywanie operacji (tych co mozna)

jutro - modify()










*/
