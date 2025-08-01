# SILENCIONS - generator obudów s|HP|

## Funkcjonalność

Aplikacja ma za zadanie dobranie odpowiedniej obudowy akustycznej s|HP| firmy Silencions
do pompy ciepła użytkownika. Generator składa się z trzech kroków, na których trzeba podać 
następujące dane:

1. Skrajne wymiary pompy ciepła (szerokość, głębokość i wysokosć),
2. Odległości do najbliższych przeszkód (z lewej, prawej i od tylnej strony urządzenia),
3. Minimalne odstępy między pompą ciepła a wewnętrznymi ściankami obudowy.

Aplikacja następnie analizuje podane dane i wyświetla wynik, którym może być:

- Standardowa obudowa s|HP| (model, rysunki techniczne oraz wymiary),
- Personalizowana obudowa s|HP| (przykładowe realizacje oraz komunikat o niepowodzeniu).

Ostatecznie oferowana jest możliwość kontatku. Użytkownik może wysłać wiadomość email
z wynikami generatora bezpośrednio z poziomu aplikacji albo skopiować automatycznie
stworzoną treść wiadomości i wysłać ją ręcznie. 

Przykłady aplikacji w trakcie działania:
<p align="middle">
  <img src="/ex/coverapp_ex1.png" width="400" />
  <img src="/ex/coverapp_ex2.png" width="400" />
  <img src="/ex/coverapp_ex3.png" width="400" /> 
</p>

## Implementacja

Aplikacja została napisana w języku C++ z użyciem bibliotek Qt. Bazowo jest to zwykła 
aplikacja okienkowa, ale została przetransformowana do działania w przeglądarkach za
pomocą narzędzia Emscripten. Jest to zestaw kompilatorów, które przetwarzają kod C++
na WebAssembly. Qt współpracuje z tym oprogramowaniem, umożliwiając integrację z Qt Creator.

## Instalacja i uruchomienie

Przed rozpoczęciem instalacji należy pobrać następujące elementy:

- Qt software - dostępne do pobrania pod: https://www.qt.io/download-dev
- Emscripten - poradnik instalacji dostępny pod: https://emscripten.org/docs/getting_started/downloads.html
- make - narzędzie do generowania plików wykonywalnych na podstawie zasad zdefiniowanych w pliku ```Makefile```

Przy instalacji Qt oraz Emscripten należy pobrać pasujące wersje. Więcej informacji bezpośrednio w 
poradniku integracji Qt z WebAssembly dostępnych pod linkiem: https://doc.qt.io/qt-6/wasm.html.

Narzędzie ```make``` można pobrać dla systemu Linux Debian/Ubuntu bezpośrednio z terminala:
   ```bash
   sudo apt-get update
   sudo apt install build-essential
   ```
lub dla systemu Windows ze strony: https://gnuwin32.sourceforge.net/packages/make.htm.

### Pobranie repozytorium

Sklonuj repozytorium:
   ```bash
   git clone https://github.com/Silencions-dev/CoverGeneratorApp.git
   cd CoverGeneratorApp
   ```
lub pobierz jako archiwum i rozpakuj:
  ```bash
  unzip CoverGeneratorApp-main.zip
  cd CoverGeneratorApp-main
  ```

### Kompilacja ręczna

Zaktualizuj ścieżki i zmienne środowiskowe Emscripten w aktywnej sesji terminala:
   ```bash
   source /path/to/emsdk/emsdk_env.sh
   ```
Utwórz folder na wyniki kompilacji:
   ```bash
   mkdir build
   cd build
   ```
Przygotuj plik Makefile na podstawie pliku projektu (```HP_CoverDesigner.pro```):
   ```bash
   /path/to/Qt/x.x.x/wasm_singlethread/bin/qmake ../HP_CoverDesigner.pro
   ```
Fragment ```x.x.x``` zastąp swoją wersją Qt, np. ```Qt/6.9.0```.

Rozpocznij kompilację z wykorzystaniem wszystkich dostępnych rdzeni CPU (aby przyspieszyć pracę):
   ```bash
   make -j$(nproc)
   ```
### Kompilacja w Qt Creator

Do kompilacji można też wykorzystać graficzne środowisko - Qt Creator.

Uruchomienie IDE:
```bash
/path/to/Qt/Tools/QtCreator/bin/qtcreator
```
Następnie otworzyć nowy projekt oraz wybrać plik ```HP_CoverDesigner.pro```. Potem z listy rozwijanej pod ```Edit```
przejść do ```Preferences``` i wybrać opcję ```SDKs```. Wcisnąć zakładkę ```WebAssembly``` i ustawić ścieżkę do
kompilatorów Emscripten przyciskiem ```Browse...```. Ustawienie ścieżek do przechowywania wyników kompilacji 
można ustawić w zakładce ```Projects``` w lewej części okna. Dalej wybrać odpowiedni zestaw narzędzi z ```Kit Selector```
w lewym dolnym rogu a następnie przycisk ```Build``` i ```Run```. Więcej o konfiguracji Qt Creator pod WebAssembly
znajduje się w poradniku: https://doc.qt.io/qtcreator/creator-setup-webassembly.html.

### Uruchomienie

Uruchomienie aplikacji w przeglądarce za pomocą Emscripten (lub automatycznie przez ```Run``` w Qt Creator):
   ```bash
   /path/to/emscripten/emrun --browser=<your_broswer> HP_CoverDesigner.html
   ```
Argument ```--browser``` przyjmuje dosłownie nazwę używanej przeglądarki, np. ```--browser=firefox```.

Alternatywnie można wystartować Python'owy serwer http w folderze, który zawiera pliki wykonywalne aplikacji (w formie WebAssembly):
   ```bash
   cd /path/to/build
   python -m http.server 
   ```
Serwer domyślnie nasłuchuje na porcie ```8000```, zatem w przeglądarce należy wpisać link:
  ```
  http://localhost:8000
  ```
Przykładowo, bardziej sprecyzowane ustawienie sewera:
```bash
python -m http.server -b 0.0.0.0 8080
```
W tym wypadku serwer nasłuchuje na porcie ```8080``` oraz pozwala na połączenie dowolnemu urządzeniu z lokalnej
sieci. Po wejściu w link przeglądarka pokaże zawartość folderu, z którego został uruchomiony serwer (pliki
aplikacji). Aby uruchomić aplikację należy wybrać z listy plik ```HP_CoverDesigner.html```.

### Licencja

Aplikacja objęta jest licencją GPLv2.0, co czyni ją projektem open source. Dokładne informacje oraz warunki wykorzystania kodu źródłowego można znaleźć
w pliku LICENSE dostępnym w repozytorium.
   






