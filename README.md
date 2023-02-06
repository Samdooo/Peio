# Dimension Viewer

[<img src="https://i.imgur.com/W6jntv2.png" width=50% height=50%>](https://www.youtube.com/watch?v=Ntglvek07Us)

Dimension Viewer är ett projekt vars syfte är att generalisera en typ av realistisk ljussimulering, så kallad global illumination, till högre dimensioner.
(Klicka på bilden ovan för ett exempel)

Simuleringen är skriven i språken C++ och HLSL och finns under [src/Peio/Dimension Viewer](<src/Peio/Dimension Viewer>). Den är byggd ovanpå några andra egenskrivna projekt:

* [Graphics](src/Peio/Graphics) som ger grundläggande funktionalitet för användande av grafikkort. Det använder sig av biblioteket [Direct3D 12](https://learn.microsoft.com/en-us/windows/win32/direct3d12/direct3d-12-graphics).
* [Windows](src/Peio/Windows) som underlättar vid hantering av fönster och input. Det använder sig av [Windows API](https://learn.microsoft.com/en-us/windows/win32/apiindex/windows-api-list).
* [Media](src/Peio/Media) som förenklar hantering av media (i nuläget endast video). Det använder sig av biblioteket [FFmpeg](https://ffmpeg.org/).

## **Programmet**
En exempelvideo då programmet körs finns att se [här](https://www.youtube.com/watch?v=Ntglvek07Us).

Programmet kan endast köras på Windows-datorer.

### **Kloning**
[Microsoft Visual Studio 2022](https://visualstudio.microsoft.com/vs/) bör ha installerats innan projektet klonas.
Klona sedan hela projektet ("Peio") till en tom mapp och öppna `Peio.sln`. Filen under `Dimension Viewer/Source Files/main.cpp` är till största del fri att leka runt med.
I menyn, gå sedan till `Build -> Build Solution`. Innan programmet kan köras behöver några DLL-filer placeras i mappen `bin`. Dessa finns att ladda ner [här](https://drive.google.com/uc?id=1AOwn3nTKK3aPo04PS9NkOiHt3LPdHix0&export=download). Kör sedan filen `Peio-Dimension Viewer.exe` i mappen `bin` för att starta programmet.

### **Installation**
Det rekommenderas att klona projektet för att kunna ändra inställningar, men om endast programmet önskas finns en zip-fil med de nödvändiga filerna att ladda ner [här](https://drive.google.com/uc?id=1zPyBVQ80Q-mrt42WHN7JoLv94LSl0lSV&export=download).
<br> Extrahera zip-filen och kör filen `Peio-Dimension Viewer.exe` i mappen `bin`.

### **Navigering**
Default-världen är fyrdimensionell och består av 20 st slumpmässiga objekt: tio hyperrektanglar och tio hypersfärer.

Tryck på `esc` för aktivera eller inaktivera kontrollerna.

Kamerans rörelse styrs med `W`, `S`, `D`, `A`, `Mellanslag`, `Shift`. 
<br> Tryck på `Uppåtpil` eller `Nedåtpil` för att ändra vilka axlar, i förhållande till kameran, ovannämnda tangenter flyttar kameran längs.

Kamerans rotation styrs med musrörelser.
<br> `Scrolla` för att ändra vilka plan kameran roterar längs.

## **Rapport**
Rapporten med alla relevanta formler samt härledningar finns att ladda ner [här](https://drive.google.com/uc?id=1T5tn9xtHR01OGPzWYCWezdt1KJ_7sYWl&export=download).

