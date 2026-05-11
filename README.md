# PythonECFSR

Projekt koji kreira Python sučelje za simboličku regresiju uz ECF biblioteku.

## Instalacija

Instalacija i uporaba ovog projekta je testirana na Linux i Windows operacijskim sustavima.\
Za oba operacijska sustava predviđeno je korištenje [Conda Miniforge](https://docs.conda.io/projects/conda/en/latest/index.html) alata za upravljanje paketima Boost i Python. Instalacija bez tog alata je značajnije kompliciranija i teža za reprodukciju, no nije nemoguća.

### Linux

Na Linux operativnom sustavu nije potrebno ništa više od gore navedene Conde, iako je instalacija bez nje relativno jednostavna, ali ovisna o inačici Linuxa.\
Instalacija je zatim moguća pokretanjem skripte [install.sh](install.sh).

### Windows

Na Windows operativnom sustavu uz gore navedenu Condu potreban je i MSVS kompajler za C i C++. Preporučena je inačica 17 (2022), iako je instalacija moguća i sa starijim verzijama, koju je moguće nabaviti preko službene Microsoft stranice ili upisivanjem ove komande u komandnu liniju na Windowsu: `winget install -e --id Microsoft.VisualStudio.2022.BuildTools`. U dobivenom Visual Studio Installeru potrebno je odabrati "Desktop Development with C++" opciju i instalirati ju.\
Instalacija je zatim moguća pokretanjem skripte [install.bat](install.bat).

### Skripta

Nakon što ste nabavili potrebne alate za instalaciju možete pokrenuti navedenu skriptu ovisno o operacijskom sustavu. Moguće je izvoditi i jednu po jednu liniju iz skripte u komandnoj liniji (__preporučeni način za prvu instalaciju__) te su komande ukratko objašnjene ovdje.

Kopiranje ovog repozitorija i prebacijevanje u kopirani direktorij:
```bash
git clone https://github.com/Andrija44/PythonECFSR.git
cd PythonECFSR
```
Kreiranje i aktiviranje Conda okruženja na bazi [environment.yml](environment.yml) datoteke:
```bash
conda env create -f environment.yml
conda init # ovu komandu je potrebno samo ako prvi put pokrećete Condu
conda activate ecf
```
Kopiranje ECF repozitorija (ako imate već skinuti ECF, ovaj korak je i dalje potreban jer će ova verzija biti kompajlirana s posebnim zastavicama):
```bash
git clone https://github.com/djakobovic/ECF.git
```
Pokretanje cmake naredbi, odnosno kompajliranje projekata:
```bash
cmake -S . -B build
cmake --build build # --config Release # za Windows
```
Kopiranje dobivene datoteke (ecf_sr.so ili ecf_sr.pyd) u pripadajući direktorij:
```bash
cmake --install build # --config Release # za Windows
```
Instaliranje dobivene biblioteke u sam Python preko alata pip:
```bash
pip install ./wrapper/lib
```

Nakon što ste uspješno instalirali biblioteku _ecf_sr_, možete ju isprobati pokretanjem datoteke [test.py](test.py) (`python test.py`) koja bi trebala ispisati dvije različite formule i veličinu modela.

Ako radite ikakve promjene na projektu, na raspolaganju je i skripta [build.sh](wrapper/build.sh)/[build.bat](wrapper/build.bat) koja se može upotrijebiti za ponovno instaliranje paketa bez ponovnog povlačenja ECF biblioteke.

## Uporaba

Biblioteku _ecf_sr_ je moguće koristiti kroz interaktivni terminal Python-a ili kreiranjem i pokretanjem .py datoteke.\
Biblioteka se u Pythonu može koristiti korištenjem naredbe `import ecf_sr` ili se mogu uvesti zasebne klase naredbom `from ecf_sr import ECFBaseRegressor, ECFTree, ECFCGP, ModelType`.\
Navedene klase su ujedno i jedine klase trenutno na raspolaganju u biblioteci.

### ECFBaseRegressor

ECFBaseRegressor([ECFBaseRegressor.py](wrapper/lib/ecf_sr/ECFBaseRegressor.py)) je glavna klasa u paketu te je preko nje moguće slati vlastite datoteke parametara. Klasa ujedno prima `model_type` koja označava koji model/genotip u ECFu će se koristiti. Taj parametar se mora poklapati s onim navedenim u datoteci parametara inače će program baciti grešku. Još je potreban i parametar `linear_scaling` koji označava hoće li se koristiti proces linearnog skaliranja pri korištenju modela.
```py
ECFBaseRegressor(parameters: str, model_type=ModelType.TREE_MODEL: ModelType, linear_scaling=True: bool)
```
Od funkcija su na raspolaganju:
* Funkcija `fit(X, y)` koja trenira model na danim podacima
* Funkcija `predict(X)` koja vraća pretpostavljenu vrijednost za X na bazi treniranog modela
* Funkcija `get_model()` koja vraća trenirani model u skladu sa sympy paketom
* Funkcija `get_model_type()` koja vraća tip modela/genotip koji se koristi
* Funkcija `get_n_nodes()` koja vraća veličinu modela

### ECFTree

ECFTree([ECFTree.py](wrapper/lib/ecf_sr/ECFTree.py)) je klasa koja nasljeđuje klasu [ECFBaseRegressor](#ecfbaseregressor) za Tree genotip u ECFu. U sebi već ima ugrađenu datoteku parametara koju je moguće prilagođavati preko konstruktora klase (ako se želi koristiti neka vlastita datoteka parametara, treba se koristiti ECFBaseRegressor klasa). Ova klasa ujedno ima i prilagođenu `fit(X, y)` funkciju za dinamičko kreiranje terminala potrebnih u ECFu.
```py
ECFTree(functionset="+ - * / min max", maxdepth=6, terminalset="1 [-1 1]",
        mutation_indprob=0.3, population_size=500, term_stagnation=20,
        term_fitnessval=0, linear_scaling=True, random_state=0)
```

### ECFCGP

ECFCGP([ECFCGP.py](wrapper/lib/ecf_sr/ECFCGP.py)) je klasa koja nasljeđuje klasu [ECFBaseRegressor](#ecfbaseregressor) za CGP genotip u ECFu. Od ECFTree klase se razlikuje samo po tipu genotipa, parametrima i svojoj implementaciji `fit(X, y)` funkcije.
```py
ECFCGP(numvariables=3, numoutputs=1, numrows=5, numcols=10, levelsback=4,
        functionset="+ * - / min max", mutation_indprob=0.7, population_size=1000,
        term_maxgen=50, term_fitnessval=0, linear_scaling=True, random_state=0)
```

## Vlastite klase i modeli

Za dodavanje novih modela i paket potrebno je na svim mjestima u datotekama [library.cpp](wrapper/library.cpp), [Model.cpp](wrapper/Model.cpp) i [Model.hpp](wrapper/Model.hpp) gdje se spominje _TEMPLATE_ model unijeti podatke za novi model.\
U [library.cpp](wrapper/library.cpp) to je na tri mjesta povezana s ModelType enumom.\
U [Model.hpp](wrapper/Model.hpp) treba kreirati definiciju nove klase modela koja nasljeđuje Model klasu i definirati sve potrebne funkcije.\
U [Model.cpp](wrapper/Model.cpp) treba implementirati funkcije navedene u Model.hpp.

Nakon dodavanja model u C++ dijelu koda (i ponovne instalacije), može mu se pristupiti preko [ECFBaseRegressor](#ecfbaseregressor) klase na način naveden u [uporabi](#uporaba).\
No može se i kreirati zasebna klasa za taj model, po uzoru na klase ECFTree i ECFCGP. Ta klasa se može staviti u direktorij [wrapper/lib/ecf_sr/](wrapper/lib/ecf_sr/) ako želite da bude dio paketa (nakon čega se paket ponovno treba instalirati).

U direktoriju [examples/](examples/) postoji nekoliko primjera vlastitih Python klasa koje nisu dio paketa, no definiraju svoju datoteku parametara.

U datoteci [ECFCustomParams.py](examples/ECFCustomParams.py) je prikazano kako se datoteka parametara može učitati direktno iz datoteke.