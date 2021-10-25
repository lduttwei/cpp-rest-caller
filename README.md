#CPP-CALLER

1. Run
Das executable des Projektes heisst ./cpp-caller und befindet sich im Build verzeichniss. 

Es ist eine Konsolenapplikation mit einem Kommandozeilenparser.
Das projekt laeuft zusammen mit meinem m223-punchclock-quarkus.

Zur bedienung:

Das Programm verfuegt ueber eine Hilfe. Einfach immer ein Kommando eingeben und es wird eine Antwort kommen, was fuer Optionen offen sind. 

Wenn man nicht eingelogt ist wird man automatisch dazu aufgefordert.
Im ausgeloggten Zustand verhaltet sich das Programm so:

./cpp-caller login/register
- login: Einloggen
- register: Registieren 

Das Login wird durch ein File: session.json gespeichert. Das einzige was cpp-caller nicht behandlen kann ist, dass das Token ablaeuft. Also ist man evt. noch eingeloggt aber bekommt 400er Errors weil das Token abgeflaufen ist. Dann einfach -> ausloggen -> einloggen.


Eingeloggt:
 
./cpp-caller project/user/logout

- project & user:
 
Beim Projekt und User sind die weiteren Optionen DELETE/POST/PUT und LIST verfuegbar.
z.B ./cpp-caller project DELETE 1 - gibt alle Projekte aus.
Wenn ich wissen will, wie sich ein Kommando verhaltet, kann ich einfach eingeben:
./cpp-caller project und es wird angezeigt, wie es funktioniert. 

- logout: Man wird wieder ausgeloggt und kann sich registieren oder einloggen.
 


 

2. Build 

Das programm wird mit CMake gebuilded:
- cd build 
- cmake --build .
Die einzige dependency ist cmake und openssl. Falls openssl nicht vorhanden ist koennte es Probleme geben beim builden.



