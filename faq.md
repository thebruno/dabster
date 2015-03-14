# Często zadawane pytania #
### W jakim formacie ma być wyjątek ###
> Dołączcie do pliku, w którym chcecie zgłosić wyjątek nagłówek err.h (załącznik). Poza tym do projektu dołączcie jeszcze err.cpp, com.h, com.cpp, str.h, str.cpp, set.h, set.cpp i stdMcrs.h - kompilują się pod VC++, powinny kompilować się pod BC++Builderem.

> Jasiowi polecam Windowsa i kompilator z CLR, bo przerabianie tego nie ma sensu zważając, że będziesz z tego korzystał jedynie do testowania - po połączeniu będą dodane te pliki, które Wam przesyłam. W katalogu z exe'kiem stwórzcie folder 'data' i skopiujcie do niego pliki $comPL.dat i $comEN.dat. Gdzieś na początku funkcji main wstawcie taki kod:
```
/* Ustalanie appPath */
str::path appPath;
try {
     appPath = str::fixAppPath();
}
catch (...) {
     /* Niepoprawna sciezka */
     throw 1;
}  //lub return 1; w zależności od tego gdzie to wklejacie.

/* Wczytywanie ustawien */
try {
     set::load(str::fixDelims(appPath.drive + appPath.dir + 
			      "data\\$set.dat"));
}
catch (err) {
     /* Nie mozna zaladowac ustawien */
     throw 2;
}   //lub return 2; w zależności od tego gdzie to wklejacie.

/* Wczytywanie bazy komunikatow i bledow */
try {
     com::load(str::fixDelims(appPath.drive + appPath.dir + "data\\$com" + 
			      set::get("$sLANG").s + ".dat"));
}
catch (err) {
     /* Nie mozna wczytac bazy */
     throw 3;
}   //lub return 3; w zależności od tego gdzie to wklejacie.
```
> Teraz możecie zgłaszać wyjątek tak:
```
throw err("!BMP0");
```
> lub tak:
```
std::vector<std::string> params(1);   //ew. 2, 3, 4, ... 10
params[0] = "parametr";                  //ew. 1, 2, 3, ... 9
throw err("!BMP0", params);
```
> Jak to działa?

> W obu przypadkach zgłaszacie błąd o numerze !BMP0 (numerem jest całość). Treść błędów możecie edytować w pliku '$comPL.dat' i/lub '$comEN.dat' (w zależności od ustawienia $sLANG w pliku '$set.dat - PL lub EN). Numery błędów muszą zaczynać się od '!', później (choć niekoniecznie) powinniście wstawić nazwę swojego modułu ('BMP' lub 'AES') i numer błędu - tak żeby to wszystko było spójne i łatwe do znalezienia po dłuższym czasie. Każdy numer błędu musi występować dwukrotnie - z literą 'd' + krótka treść błędu (d = description) i literą 'h' + dodatkowe informacje na temat błedu (h = help). Treść błędu znajduje się za spacją i ciągnie się do najbliższego entera. Sposób b) przekazuje też parametry (do 10 parametrów o numerach od 0 do 9). W pliku, parametr jest wstawiany w miejsce wszystkich fraz 

&lt;param0&gt;

, 

&lt;param1&gt;

 itd. Może to być np. nazwa pliku, numer czegoś itp. Porównajcie z tym do czego ja to używam.


> Każdy numer błędu musi występować dwukrotnie - z literą 'd' + krótka treść błędu (d = description) i literą 'h' + dodatkowe informacje na temat błedu (h = help). Treść błędu znajduje się za spacją i ciągnie się do najbliższego entera. Sposób b) przekazuje też parametry (do 10 parametrów o numerach od 0 do 9). W pliku, parametr jest wstawiany w miejsce wszystkich fraz 

&lt;param0&gt;

, 

&lt;param1&gt;

 itd. Może to być np. nazwa pliku, numer czegoś itp. Porównajcie z tym do czego ja to używam.


### Mam funkcję która "formatuje" bitmapę - zapisuje kompresję i nagłówek dabstera z wyzerowanymi odpowiednimi wartościami. No i mam problem jak zrobić otwarcie bitmapy, na razie jest tak: BMP(nazwa pliku) zwraca wyjątki (nazwy są oczywiście inne): brak pliku, nieBmp, BMP\_Ok oraz pusta bmp no i na ten ostatni trzeba zareagować ponownym utworzeniem obiektu BMP poleceniem BMP(nazwa pliku, kompresja) to automatycznie odczyta BMP i "sformatuje ją", może tak być? ###

> Opiszę co dla bitmapy. Dla AESa analogicznie:
```
sfile *f = new sfile(sciezka);
//...
if (bmp::isBmp(plik)) {
    bmp *b = new bmp(plik);
    zawartosc = plik.getContent();
}
```

  * Funkcja isBmp() jest w przestrzeni nazw bmp, ale nie jest metodą klasy bmp. Jeśli plik nie istnieje, ma za mało bitów (<54) lub nagłówek bitmapy jest uszkodzony to zwraca 0.
  * Konstruktor o postaci bmp(sfile &p) konwertujący sfile na bmp. Nie musi sprawdzać tego co funkcja isBmp(), bo będzie to sprawdzone wcześniej. Nie powinien też "formatować" ani sprawdzać nagłówka ukrytych w nim plików.
  * getContent() poszukuje plików w istniejącej bitmapie z poprawnym nagłówkiem i zwraca jej zawartość. Zgłasza wyjątek w przypadku błędu w ciele bitmapy (w bitach o numerze większym niż 54), w nagłówkach znalezionych plików itp.

> Generalnie nie zakładajcie czegoś takiego jak "otwarcie" bitmapy czy AESa. Każda funkcja robi co ma robić po czym zamyka strumień. Może też być wywołana zupełnie pojedynczo lub w sekwencji z jakimiś innymi funkcjami (np. isBmp(plik) -> bmp(plik)). Można tylko zakładać, że skoro została wywołana, to to co ma zrobić musi się dać zrobić. Jeśli nie - wyjątek. Z tym, że wyjątków nie ma kiedy np. funkcja isBmp stwierdza, że pliki ukryte w bitmapie są uszkodzone, ale sam plik bitmapy ma poprawny nagłówek. Tak naprawde nie powinna nawet sprawdzać poprawności plików w bitmapie - tylko samą bitmapę.

### Bo przecież nie można chyba wrócić do wykonywanie przerwanej przez wyjątek funkcji i dalej ją wykonywać. ###
> Nie można. Dlatego WYJĄTKI STOSUJEMY W WYJĄTKOWYCH PRZYPADKACH. (patrz odpowiednie pytanie)


### Zauważ, że nie mogę zwrócić dokładnej wolnej przestrzeni bitmapy, ponieważ zależy ona od długości nazwy pliku ###
> Przyjmij długość nazwy pliku = 0. W razie próby spakowania pliku, który zgodnie z tak obliczoną wolną przestrzenią powinien się zmieścić, mimo wszystko by się nie zmieścił, należy zgłosić wyjątek. Nazwy wyjątków będzie trzeba ustalić wspólne, bo PROGRAM GŁÓWNY NIE POWINIEN ROZRÓŻNIAĆ PLIKU BMP OD AES. I jest to zasada generalna i obowiązująca w całości projektu. (Chyba, że to naprawdę konieczne -> np. ustalanie stopnia kompresji itp.)

### Ale wydaje mi się, że jednak wyjątki to powinny być bardziej stosowane do obsługi sytuacji wyjątkowych, a nie jako środek zastępujący wartości zwracane przez funkcje ###
> Tak, to prawda. Wyjątek to wyjątek.

### Nie wiem co zrobić z ograniczeniem długości nazw plików. Na pole określające długość nazwy przeznaczyłem 2 bajty czyli ~64k znaków, ale teraz sprawdziłem, że windows obsługuje wielkości nazw do długości 253(sic!) znaków, przy czym całkowita ścieżka do pliku/folderu też tyle ma wynosić. Z prób wyszło mi nawet krócej ok. 244, czyli utworzysz takiej długości folder na c i już do niego nic nie wrzucisz. Myślę więc, że ograniczę nazwę do 255 znaków. ###
> Sprawdziłem. Udało mi się stworzyć plik o nazwie mającej 255 znaków. Więcej nie chciał pozwolić. Czyli cała ścieżka ma więcej niż 255 znaków, bo ten plik był na dysku C. Zarezerwuj jednak 2 bajty.

### Podczas otwierania plików wypadałoby mieć wyłączność na odczyt/zapis pliku, ale nie wiem jak to się robi i czy da się z poziomu strumienia (fstream) to obsłużyć ###
> Nie wiem, ale sprawdzałem co robi eksplorator podczas próby zakłócenia jego działania. Zacząłem kopiować duży folder na inny dysk i po chwili zacząłem go usuwać z tego dysku, na który był kopiowany. Efekt był do przewidzenia. Użyli prostego rozwiązania, najmniej błędogennego. Dopóki było co usuwać (czyt. kopiowanie nadążało za usuwaniem) wszystko było ok. Jak tylko usunął wszystkie pliki (aktualnie, bo kolejne się wciąż kopiowały) i chciał się zabrać za ostatni, który był dopiero co utworzony i wciąż zapełniany przez funkcję kopiującą, wyskoczył błąd, że nie można usunąć pliku '...'. Czyli on też nie blokuje dostępu do plików, tylko zezwala na korzystanie z nich przez wiele procesów, w tym swoich własnych. Tak jest najprościej i najlepiej - próbujesz robić głupią, niedozwoloną i perfidną rzecz, żeby tylko zawiesić program, to się wal, operacja zostanie przerwana.

### Wspominałeś coś o pasku postępu - jak to wymyśliłeś?? ###
> Korzysta się z klasy prgrss, którą stworzę podczas tworzenia GUI, ale już teraz możecie sobie taką testową klasę zrobić. Wam będzie potrzebna tylko jedna funkcja - show(int value), gdzie value jest wartością postępu dla danej operacji (w procentach od 0 do 100). W klasie są jeszcze inne funckje, ale będą używane tylko przez główny program. Wskaźnik do odpowiedniego obiektu klasy prgrss będzie przechowywany w item w polu: `prgrss _progress;`

> Użycie:
```
 for (i = 1; i <= 100; i++) {
   _progress->show(i);
 }
```

### Ma się rozumieć, że użytkownik powinien mieć możliwość przerwania pakowania/wypakowani, no więc jak to widzisz? ###
> Będzie gdzieś pole 'bool cancel', którego wartość będzie trzeba sprawdzać we wszystkich głównych pętlach metod. Zastanawiałem się nad tym i problem jest dość złożony. Konkretnie dlatego, że nie za bardzo wiem jeszcze, gdzie powinno być to pole. Po drugie dlatego, że nie wiadomo co ma zrobić metoda w razie wykrycia, że użytkownik anulował jej działanie. Na pewno musi zakończyć co trzeba, ale co dalej? Cofnąć ostatnią pod-operację przerwaną w środku? Coś mi się zdaje, że przyda nam się wiedza z wykładu z Baz Danych - będzie potrzebny dziennik operacji i kopia zapasowa plików, na których prowadzone są operacje.

### Wiesz, że można odwołać się do systemu poleceniem system (komenda), poniższe instrukcje utworzą folder i wyświetlą w konsoli zawartość aktualnego katalogu ###
> O komendzie system wiedziałem, ale nie wpadłem na takie jej wykorzystanie.