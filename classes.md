# Klasy #
![http://dabster.googlecode.com/svn/trunk/doc/classes.png](http://dabster.googlecode.com/svn/trunk/doc/classes.png)

Schemat najważniejszych klas używanych w Dabsterze przedstawiono na powyższym rysunku. Ramki oznaczają klasy. Pogrubiona ramka oznacza klasę wirtualną. Szerokie strzałki oznaczają wskaźnik. Grot wskazuje element, którego adres jest zapamiętywany w obiekcie, z którego prowadzi strzałka. Przerywana linia oznacza, że wskaźnik nie musi istnieć. Pozostałe strzałki oznaczają dziedziczenie.

W projekcie przyjęto zasadę, że klasy wewnętrzne nie powinny odwoływać się do żadnych kontrolek znajdujących się w graficznym interfejsie użytkownika GUI. Ma to maksymalnie uniezależnić wewnętrzne klasy od systemu operacyjnego i od samego GUI. Wyjątek mogą stanowić komponenty umożliwiające np. łączenie się z Internetem czy obsługę systemu plików, z których niektóre klasy korzystają.

Ponadto do niezbędnego minimum ograniczono komunikację między najniżej położonymi klasami powyższej hierarchii z jakimikolwiek innymi klasami. Są one projektowane niezależnie, dlatego taka komunikacja stwarzałaby wiele problemów i mogłaby wpływać na sposób w jaki musiałyby działać inne klasy z najniższego poziomu, co podważałoby ich niezależność. To niezbędne minimum stanowi klasa prgrss, do której wskaźnik jest przechowywany w klasie item. Odwołują się do niej funkcje najniżej położonych klas.


## MDI Explorer ##
### mdiXplrr ###

Bezpośrednio pośredniczy pomiędzy użytkownikiem, a klasami wewnętrznymi. Żadna inna klasa nie powinna korzystać z klas wewnętrznych.

Klasa połączona z obiektem. Zawiera okienko z eksploratorem i umieszczonymi na nim kontrolkami typu label (m.in. do wyświetlania nazw plików i innych danych) i picture (do wyświetlania ikonek itp.) oraz ewentualnie innymi.

Zarządza zdarzeniami takimi jak kliknięcia obiektu w eksploratorze prawym (menu kontekstowe z opcjami typu kopiuj, edytuj, właściwości itp.) i lewym przyciskiem, zarówno pojedyncze (podświetlenie itp.) jak i podwójne (otwieranie).


## Progress ##
### prgrss ###

Pośredniczy w procesie przesyłania informacji między klasami wewnętrznymi, a paskiem postępu _prgProgressBar i/lub polem_lblLabel. Umożliwia ustawianie i pobieranie wartości paska postępu (getValue), wyświetlanego przy pasku tekstu (setText, getText) oraz wartości początkowej i końcowej paska (setStart, setStop, getStart, getStop). Możliwe jest również odświeżanie widoku paska postępu (show).


## Item ##
### item ###

Klasa wirtualna, korzeń hierarchii klas. Po item dziedziczą klasy folder i file. Umożliwia pobranie i ustawienie nazwy i rzeczywistej ścieżki obiektu (getName, getRealPath, setName, setRealPath). Zawiera wskaźnik do pojedynczego obiektu prgrss.


## Folder ##
### folder ###

Klasa wirtualna dziedzicząca po klasie item. Zawiera operacje pozwalające na kopiowanie obiektów z zewnątrz do folderu (store) i wypakowywanie poza folder (extract), przenoszenie obiektów wewnątrz folderu lub zmianę ich nazwy (modify), kopiowanie obiektów wewnątrz folderu (copyInside), usuwanie obiektów (del) oraz zwracanie listy obiektów wewnątrz folderu wraz z dodatkowymi informacjami o nich (getContent).

Wszystkie te operacje mogą zostać wykonane nie tyko dla FB, ale również dla całego DFB, po podaniu w odpowiednich parametrach ich adresów zapisanych względem folderu bazowego.


## File ##
### file ###

Klasa wirtualna dziedzicząca po klasie item. Umożliwia edycję plików (edit).


## Drive ##
### drive ###

Klasa wirtualna dziedzicząca po klasie folder. Pozwala otrzymać informacje o pojemności dysku (getCapacity).


## Simple Folder, Simple File i Simple Drive ##
### odpowiednio sfolder, sfile, sdrive ###

Klasy dziedziczące odpowiednio po klasach folder, file i drive. Reprezentują foldery, pliki i dyski proste. W ich implementacji mogą zostać użyte komponenty lub funkcje interfejsu systemu operacyjnego (np. WinAPI).


## Bitmap ##
### bmp ###

Klasa dziedzicząca po klasach file i drive. Ukrywa dane w obrazku zapisanym w formacie bitmapy. Opis formatu znajduje się w angielskojęzycznej Wikipedii, pod adresem http://en.wikipedia.org/wiki/Windows_and_OS/2_bitmap. Umożliwia dodatkowo ustawienie i pobranie stopnia kompresji (setCompression, getCompression).


## Wave ##
### wv ###

Klasa dziedzicząca po klasach file i drive. Ukrywa dane w pliku muzycznym zapisanym w formacie wave. Opis formatu znajduje się na stronach Centrum Badań Komputerowych nad Muzyką i Akustyką Uniwersytetu Stanforda (CCRMA) pod adresem http://ccrma.stanford.edu/courses/422/projects/WaveFormat/. Umożliwia dodatkowo ustawienie i pobranie stopnia kompresji (setCompression, getCompression).


## Advanced Encryption Standard ##
### aes ###

patrz [opis klasy](AdvancedEncryptionStandard.md)

Klasa dziedzicząca po klasach folder i file. Szyfruje dane przy pomocy algorytmu blokowego Rijndael opisanego w publikacji numer 197 amerykańskiego Narodowego Instytutu Standardów i Technologii (NIST), znajdującej się pod adresem http://csrc.nist.gov/publications.


## Twofish ##
### twfsh ###

Klasa dziedzicząca po klasach folder i file. Szyfruje dane przy pomocy algorytmu Twofish pobieżnie opisanego w książce „Kryptografia w praktyce”, Nielsa Fergusona i Bruce’a Schneiera, wydanej w 2004 przez wydawnictwo Helion. Bardziej szczegółowy opis znajduje się na stronie jednego z autorów szyfru Bruce’a Schneiera http://www.schneier.com/.

Umożliwia dodatkowo ustalenie i pobranie klucza (setKey, getKey), oraz wielkości szyfrowanego bloku (setBlockSize, getBlockSize).


## Burrows-Wheeler Transform ##
### bwt ###

Klasa dziedzicząca po klasach folder i file. Kompresuje dane przy użyciu algorytmu wykorzystującego transformatę Burrowsa-Wheelera. Szczegółowy opis algorytmu znajduje się w pracy doktorskiej Sebastiana Deorowicza „Universal lossless data compression algorithms”, która jest dostępna pod adresem www-zo.iinf.polsl.gliwice.pl/~sdeor.




## File Transfer Protocol ##
### ftp ###

Klasa dziedzicząca po klasie drive. Umożliwia komunikację z serwerami przy pomocy Protokołu Transferu Plików (FTP). W jej implementacji mogą zostać użyte komponenty lub funkcje interfejsu systemu operacyjnego (np. WinAPI).


## Mail Drive ##
### mldrv ###

Klasa dziedzicząca po klasie drive. Umożliwia komunikację z serwerami poczty przy pomocy protokołów POP3 i SMTP. Tworzy wirtualne systemy plików na tych serwerach. W jej implementacji mogą zostać użyte komponenty lub funkcje interfejsu systemu operacyjnego (np. WinAPI).
















