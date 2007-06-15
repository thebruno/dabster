#define _CRT_SECURE_NO_DEPRECATE
#define _POSIX_SOURCE 
#include "bmp.h"

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <ctime>
#include "err.h"
#include "com.h"
#include "str.h"
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
/*
	std::vector<int> a;
	std::vector<int>::reverse_iterator ri;
	a.push_back(1);
	a.push_back(2);
	a.push_back(3);
	a.push_back(4);


	ri = a.rbegin();
	std::cout << *ri;
	ri++;
	std::cout << *ri;
*/
	/* Wczytywanie bazy komunikatow i bledow */


	std::string nazwa = "test.bmp";
	
	smap plik1_in, plik2_in, plik3_in, plik4_in, plik5_in, plik6_in ;
	smap folder1_in, folder2_in, folder3_in, folder4_in;

	smap plik1_inside, plik2_inside, plik3_inside, plik4_inside, plik5_inside, plik6_inside;
	smap folder1_inside, folder2_inside, folder3_inside, folder4_inside;

	smap new_plik1_inside, new_plik2_inside, new_plik3_inside, new_plik4_inside, 
		new_plik5_inside, new_plik6_inside;
	smap new_folder1_inside, new_folder2_inside, new_folder3_inside, new_folder4_inside, 
		new_folder5_inside, new_folder6_inside;

	smap plik1_out, plik2_out, plik3_out, plik4_out, plik5_out, plik6_out;

	vmstring sciezki1, sciezki2, sciezki3, sciezki4, sciezki5, wynik;
	// deklaracja plikow rzeczywistych


	// testowanie wydajnosci:
	plik1_in["name"] = "1.rar"; plik1_in["length"] = "11282348"; plik1_in["realPath"]= "1.rar";
	sciezki1.push_back(plik1_in);

	plik1_inside["creationTimeUtc"] = "25.04.2007 08:56:33"; plik1_inside["name"] = "11.rarinbmp";
	plik1_inside["atrReadOnly"] = "true"; plik1_inside["relativePath"] = "11.txtinbmp";
	sciezki2.push_back(plik1_inside);

	plik1_out["name"] = "wynik1.rar"; plik1_out["realPath"]= "wynik1.rar";
	sciezki3.push_back(plik1_out);


	/*plik1_in["name"] = "1.txt"; plik1_in["length"] = "20237"; plik1_in["realPath"]= "1.txt";
	sciezki1.push_back(plik1_in);
	plik2_in["name"] = "2.txt"; plik2_in["length"] = "26033"; plik2_in["realPath"]= "2.txt";
	sciezki1.push_back(plik2_in);
	plik3_in["name"] = "3.txt"; plik3_in["length"] = "278"; plik3_in["realPath"]= "3.txt";
	sciezki1.push_back(plik3_in);
	plik4_in["name"] = "4.txt"; plik4_in["length"] = "1056"; plik4_in["realPath"]= "4.txt";
	sciezki1.push_back(plik4_in);
	plik5_in["name"] = "5.txt"; plik5_in["length"] = "31107"; plik5_in["realPath"]= "5.txt";
	sciezki1.push_back(plik5_in);
	plik6_in["name"] = "6.txt"; plik6_in["length"] = "880"; plik6_in["realPath"]= "6.txt";
	sciezki1.push_back(plik6_in);

	// deklaracja folderow rzeczywistych
	folder1_in["name"] = "f1\\"; folder1_in["realPath"]= "f1\\";
	sciezki1.push_back(folder1_in);
	folder2_in["name"] = "f2\\"; folder2_in["realPath"]= "f2\\";
	sciezki1.push_back(folder2_in);
	folder3_in["name"] = "f3\\"; folder3_in["realPath"]= "f3\\";
	sciezki1.push_back(folder3_in);
	folder4_in["name"] = "f4\\"; folder4_in["realPath"]= "f4\\";
	sciezki1.push_back(folder4_in);

	// atrybuty dla plikow w bmp
	plik1_inside["creationTimeUtc"] = "25.04.2007 08:56:33"; plik1_inside["name"] = "11.txtinbmp";
	plik1_inside["atrReadOnly"] = "true"; plik1_inside["relativePath"] = "11.txtinbmp";
	sciezki2.push_back(plik1_inside);

	plik2_inside["creationTimeUtc"] = "01.12.2007 20:56:27"; plik2_inside["name"] = "22.txtinbmp";
	plik2_inside["atrSystem"] = "true"; plik2_inside["relativePath"] = "22.txtinbmp";
	sciezki2.push_back(plik2_inside);

	plik3_inside["creationTimeUtc"] = "01.12.2007 20:56:27"; plik3_inside["name"] = "33.txtinbmp";
	plik3_inside["atrHidden"] = "true"; plik3_inside["relativePath"] = "fold1\\33.txtinbmp";
	sciezki2.push_back(plik3_inside);

	plik4_inside["creationTimeUtc"] = "11.12.2007 11:56:27"; plik4_inside["name"] = "44.txtinbmp";
	plik4_inside["atrHidden"] = "true"; plik4_inside["relativePath"] = "fold1\\44.txtinbmp";
	sciezki2.push_back(plik4_inside);

	plik5_inside["creationTimeUtc"] = "10.12.2000 18:56:27"; plik5_inside["name"] = "55.txtinbmp";
	plik5_inside["atrHidden"] = "true"; plik5_inside["relativePath"] = "fold1\\fold3\\55.txtinbmp";
	sciezki2.push_back(plik5_inside);

	plik6_inside["creationTimeUtc"] = "01.01.1997 00:56:45"; plik6_inside["name"] = "66.txtinbmp";
	plik6_inside["atrHidden"] = "true"; plik6_inside["relativePath"] = "fold1\\fold3\\fold4\\66.txtinbmp";
	sciezki2.push_back(plik6_inside);

	// atrybuty dla folderow w bmp:
	folder1_inside["creationTimeUtc"] = "01.01.1998 00:86:35"; folder1_inside["name"] = "fold1\\";
	folder1_inside["atrHidden"] = "true"; folder1_inside["relativePath"] = "fold1\\";
	folder1_inside["atrDirectory"] = "true";
	sciezki2.push_back(folder1_inside);

	folder2_inside["creationTimeUtc"] = "02.03.1999 00:46:15"; folder2_inside["name"] = "fold2\\";
	folder2_inside["atrHidden"] = "true"; folder2_inside["relativePath"] = "fold2\\";
	folder2_inside["atrDirectory"] = "true";
	sciezki2.push_back(folder2_inside);

	folder3_inside["creationTimeUtc"] = "03.08.1993 10:36:15"; folder3_inside["name"] = "fold3\\";
	folder3_inside["atrHidden"] = "true"; folder3_inside["relativePath"] = "fold1\\fold3\\";
	folder3_inside["atrDirectory"] = "true";
	sciezki2.push_back(folder3_inside);

	folder4_inside["creationTimeUtc"] = "04.04.1986 03:56:45"; folder4_inside["name"] = "fold4\\";
	folder4_inside["atrHidden"] = "true"; folder4_inside["relativePath"] = "fold1\\fold3\\fold4\\";
	folder4_inside["atrDirectory"] = "true";
	sciezki2.push_back(folder4_inside);

	// przy wypakowaniu rzeczywiste pliki wynikowe
	plik1_out["name"] = "wynik1.txt"; plik1_out["realPath"]= "wynik1.txt";
	sciezki3.push_back(plik1_out);

	plik2_out["name"] = "wynik2.txt"; plik2_out["realPath"]= "wynik2.txt";
	sciezki3.push_back(plik2_out);

	plik3_out["name"] = "wynik3.txt"; plik3_out["realPath"]= "wynik3.txt";
	sciezki3.push_back(plik3_out);

	plik4_out["name"] = "wynik4.txt"; plik4_out["realPath"]= "wynik4.txt";
	sciezki3.push_back(plik4_out);

	plik5_out["name"] = "wynik5.txt"; plik5_out["realPath"]= "wynik5.txt";
	sciezki3.push_back(plik5_out);

	plik6_out["name"] = "wynik6.txt"; plik6_out["realPath"]= "wynik6.txt";
	sciezki3.push_back(plik6_out);

	// zmiany nazwy flików
	new_plik1_inside["name"] = "1ab";	new_plik1_inside["relativePath"] = "1ab";
	new_plik2_inside["name"] = "2cd"; 	new_plik2_inside["relativePath"] = "2cd";
	new_plik3_inside["name"] = "33ef"; 	new_plik3_inside["relativePath"] = "33ef";
	new_plik4_inside["name"] = "4gh"; 	new_plik4_inside["relativePath"] = "4gh";
	new_plik5_inside["name"] = "55ef"; 	new_plik5_inside["relativePath"] = "55ef";
	new_plik6_inside["name"] = "66ef"; 	new_plik6_inside["relativePath"] = "66ef";

	// zmiany nazwy folderów
	new_folder1_inside["name"] = "f1ab";		new_folder1_inside["relativePath"] = "f1ab";
	new_folder2_inside["name"] = "f2cd"; 	new_folder2_inside["relativePath"] = "f2cd";
	new_folder3_inside["name"] = "f33ef"; 	new_folder3_inside["relativePath"] = "f33ef";
	new_folder4_inside["name"] = "f4gh"; 	new_folder4_inside["relativePath"] = "f4gh";
	// wrzucanie plikow
	sciezki4.push_back(plik1_inside);
	sciezki5.push_back(new_plik1_inside);

	sciezki4.push_back(plik2_inside);
	sciezki5.push_back(new_plik2_inside);

	sciezki4.push_back(plik3_inside);
	sciezki5.push_back(new_plik3_inside);

	sciezki4.push_back(plik4_inside);
	sciezki5.push_back(new_plik4_inside);

	sciezki4.push_back(plik5_inside);
	sciezki5.push_back(new_plik5_inside);

	sciezki4.push_back(plik6_inside);
	sciezki5.push_back(new_plik6_inside);

	// wrzucanie folderow
	sciezki4.push_back(folder1_inside);
	sciezki5.push_back(new_folder1_inside);

	sciezki4.push_back(folder2_inside);
	sciezki5.push_back(new_folder2_inside);

	sciezki4.push_back(folder3_inside);
	sciezki5.push_back(new_folder3_inside);

	sciezki4.push_back(folder4_inside);
	sciezki5.push_back(new_folder4_inside);

*/
	bmp::BMP * b = 0;
	sfile sf;
	sf.setRealPath(nazwa);
	file * f = &sf;
	if (bmp::isBmp(f)) {

		try {
			b = new bmp::BMP();
			b->setRealPath(nazwa);
			std:: cout << (wynik = b->getContent(""));
 			if (wynik.empty() && b->getCompression() == 0) {
				b->setCompression(1);
			}
			double t1,t2, t;
			t1 = clock();

			b->store(sciezki1,sciezki2);
			//b->del(sciezki4);
			//b->extract(sciezki2,sciezki3);
			//b->modify(sciezki2, sciezki5);
			//b->copyInside(sciezki4, sciezki5);
			t2 = clock();
			t = t2-t1;

			if (!t) t = 1;
			t /= 1000 ;
				std::cout << (11282348/1024) / t<< " KB/s" << std::endl;
			delete b;
		}

		catch(err& a)	{
			std::cout << a.sDescription << std::endl;
			std::cout << a.sHelp << std::endl;
			std::cout << a.sNumber << std::endl;
			std::cout << "BLAD :/";
		}
	}
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
dodac zmienna reprezentujaca slash '\\' lub '/' w zaleznosci od systemu


*/
