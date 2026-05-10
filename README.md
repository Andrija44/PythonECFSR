# PythonECFSR

Projekt koji kreira Python sučelje za simboličku regresiju uz ECF biblioteku.

## Instalacija

Instalacija i uporaba ovog projekta je testirana na Linux i Windows operativnim sustavim.\
Za oba operacijska sustava predviđeno je korištenje [Conda Miniforge](https://docs.conda.io/projects/conda/en/latest/index.html) alata za upravljanje paketima Boost i Python. Instalacija bez tog alata je značajnije kompliciranija i teža za reprodukciju, no nije nemoguća.

### Linux

Na Linux operativnom sustavu nije potrebno ništa više od gore navedene Conde, iako je instalacija bez nje relativno jednostavna, ali ovisna o inačici Linuxa.\
Instalacija je zatim moguća pokretanjem skripte [install.sh](install.sh).

### Windows

Na Windows operativnom sustavu uz gore navedenu Condu potreban je i MSVS kompajler za C i C++. Preporučena je inačica 17 (2022), makar je instalacija moguća i sa starijim verzijama, koju je moguće nabaviti preko službene Microsoft stranice ili upisivanjem ove komande u komandnu liniju na Windowsu: `winget install -e --id Microsoft.VisualStudio.2022.BuildTools`. U dobivenom Visual Studio Installeru potrebno je odabrati "Desktop Development with C++" opciju i instalirati ju.\
Instalacija je zatim moguća pokretanjem skripte [install.bat](install.bat).

### Skripta

Nakon što ste nabavili potrebne alate za instalaciju možete pokrenuti navedenu skriptu ovisno o operacijskom sustavu. Moguće je izvoditi i jednu po jednu liniju iz skripte u komandnoj liniji te ću tu ukratko proći kroz svaku komandu.

Kopiranje ovog repozitorija i prebacijevanje u kopirani direktoriji:
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
Kopiranje dobivene datoteke (ecf_sr.so ili ecf_sr.pyd) u pripadajući direktoriji:
```bash
cmake --install build # --config Release # za Windows
```
Instaliranje dobivene biblioteke u sam Python preko alata pip:
```bash
pip install ./wrapper/lib
```

Nakon što ste uspješno instalirali biblioteku _ecf_sr_ možete ju isprobati pokretanjem datoteke [test.py](test.py) (`python test.py`) koja bi trebala ispisati dvije različite formule i veličinu modela.

## Uporaba