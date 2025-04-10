RTC - projekt
Jméno: Vojtěch Beran
Třída: C1
Jméno projektu: RTC

O čem projekt je?

	Jedná se amatérskou 2d simulaci světelných paprsků. Více času jsem strávil grafickou částí projektu (ale stejně to moc dobře nevypadá).

K čemu projekt slouží a proč jsem ho dělal?

	Projekt jsem nedělal s úmyslem vytvořit něco co by někomu nebo něčemu pomohlo, ale spíše jako způsob jak se seznámit se základy c++ a dokázat že něco málo umím.

Co bych chtěl dělat dál?

	Zatím nápad na další projekt nemám, ale rozhodně bych chtěl pokračovat v c++ a zaměřit se více na práci s pamětí, algoritmy, atd..

V čem je projekt udělán ?

	Projekt je napsán v C++17 a jako grafickou knihovnu používá SDL2 a SDL2_ image. Pro sestavení projektu se používají Makefiles.

Co je potřeba pro sestavení projektu ? 

	MacOS:
		- Xcode developer tools (obsahuje g++, makefile support, atd.)
		- v terminálu napište “brew install sdl” a “brew install sdl2_image”			
	Windows:
		- MinGW-w64 g++ compiler (musí podporovat C++17 a verze g++ by měla být alespoň 8 (g++ —version), aby plně podporovala std::filesystem)

		- MSYS2 prostředí (poté zadat příkaz “pacman -S make” pro stáhnutí makefile systému)
			- (https://www.msys2.org)
		- nebo stáhnout přímo GNU Make (https://www.gnu.org/software/make/) 

		- SDL (https://github.com/libsdl-org/SDL/releases/tag/release-3.2.10)
		- SDL_Image (https://github.com/libsdl-org/SDL_image/releases)
			- u obou stačí verze 2.3.2 a dále
		- taky by se dat stáhnout příkazem “pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-make \
             mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image”, ale nemuzete si určit kam se SDL2 a SDL2_Image uloží

! Potom upravte Makefile a dejte to INCLUDE_PATH a LIB_PATH path k SDL2 A SDL2_Image (include a lib). !




Jak projekt sestavit ?
	
	1) Dejte si složku na plochu (Desktop)
	2) Přejděte v terminálu(MacOS) nebo do MSYS2(Windows) do složky kde je projekt uložen (cd Desktop/RTC/)
	3) napište “make”

Pokud se při kompilaci něco nepovedlo nejspíše je problém buď v tom že je složka s projektem na špatném místě nebo nějaké soubory chybý (mělo by být zobrazeno jako error v terminálu) 

Jak projekt ovládat ?

Pro přidávání a odebírání objektů jsou tlačítka s ikonkami daného objektu.
Pokud chcete změnit počet paprsků tak stačí jít do definitions.cpp a změnit RAY_AMOUNT
Pro jakékoliv jiné úpravy nejsou tlačítka nebo textové příkazy v terminálu, takže musí být změněny pří v kódu 
