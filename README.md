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

## Implementacja

Aplikacja została napisana w języku C++ z użyciem bibliotek Qt. Bazowo jest to zwykła 
aplikacja okienkowa, ale została przetransformowana do działania w przeglądarkach za
pomocą narzędzia Emscripten. Jest to zestaw kompilatorów, które przetwarzają kod C++
na WebAssembly. Qt współpracuje z tym oprogramowaniem, umożliwiając integrację z Qt Creator.

## Instalacja i uruchomienie

Przed rozpoczęciem instalacji należy pobrać następujące elementy:

- Qt software - dostępne do pobrania pod: https://www.qt.io/download-dev
- Emscripten - poradnik instalacji dostępny pod: https://emscripten.org/docs/getting_started/downloads.html

Przy instalacji Qt oraz Emscripten należy pobrać pasujące wersje. Więcej informacji bezpośrednio w 
poradniku integracji Qt z WebAssembly dostępnych pod linkiem: https://doc.qt.io/qt-6/wasm.html.

Zainstaluj Qt oraz Emscripten SDK.

Sklonuj repozytorium:
   ```bash
   git clone https://github.com/Silencions-dev/CoverGeneratorApp.git
   cd CoverGeneratorApp-main
   ```
lub pobierz jako archiwum i rozpakuj:
  ```bash
  unzip CoverGeneratorApp-main.zip
  cd CoverGeneratorApp-main
  ```

Kompilacja:
   ```bash
   source /path/to/emsdk/emsdk_env.sh
   /path/to/Qt/x.x.x/6.9.0/wasm_singlethread/bin/qt-cmake -B build -S .
   cmake --build build
   ```
Można też wykorzystać środowisko Qt Creator do kompilacji. Gotowe pliki konfiguracyjne zamieszczone są
w repozytorium. Uruchomienie IDE:
```bash
./path/to/Qt/Tools/QtCreator/bin/qtcreator
```
Następnie otworzyć nowy projekt oraz wybrać plik ```HP_CoverDesigner.pro```. Potem z listy rozwijanej pod ```Edit```
przejść do ```Preferences``` i wybrać opcję ```SDKs```. Wcisnąć zakładkę ```WebAssembly``` i ustawić ścieżkę do
kompilatorów Emscripten przyciskiem ```Browse...```. Ustawienie ścieżek do przechowywania wyników kompilacji 
można ustawić w zakładce ```Projects``` w lewej części okna. 

Uruchomienie aplikacji w przeglądarce za pomocą Emscripten (lub automatycznie przez ```Run``` w Qt Creator):
   ```bash
   /path/to/emscripten/emrun --browser=my_broswer HP_CoverDesigner.html
   ```
Alternatywnie można wystartować Python'owy serwer http w folderze z przekompilowaną aplikacją w formie WebAssembly:
   ```bash
   cd /path/to/webassembly-build
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

   
   






