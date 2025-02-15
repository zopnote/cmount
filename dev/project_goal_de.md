Promts an ChatGPT:
> Erstelle aus der Mindmap eine Art Zielprodukts definition. 
> Diese soll in roher Markdown geschrieben sein.
> Schreibe ein Dokument über den Roten faden sozusagen.
> Setze neben der Mindmap noch folgende Informationen, wenn auch nur indirekt mit um:

Cmount ist eine Anwendung geschrieben in C, die erweitert werden kann in C und von Haus aus, mit mehreren Sprachen kommen soll.

Diese Sprachen sind Dart, Python und C. Diese sollen miteinander Interagieren können, 
wobei es dann selbstverständlich wichtig ist eine gute Struktur für Projekte zu haben die in der Cmount Umgebung sind
und eine noch bessere und sinnvolle struktur für die Binärdateien die aus dem Prozess am Ende entstehen. 

C wird direkt in Cmount integriert, wobei die anderen Sprachen über extensions implementiert werden. 
Extensions sollen untereinander kommunizieren und voneinander abhängen können.

Doch eines der wichtigsten Sachen ist, 
dass Cmount Hot reload bereitstellt und für extensions auch ein interface und events benötigt, 
da diese Sprachen mit eben diesem Feature integrieren sollen.

Es gibt also verschiedene Phasen: 
Development (In dieser Phase sollen Programmiersprachen Hot reload unterstützen und trotzdem noch miteinander interagieren),
Release (Alle binaries werden gebaut und können in AOT Kompilierung immer noch miteinander Interagieren) und zusätzlich noch die Runtime. 
Eine mit Cmount gebaute Anwendung soll das Einheitsprotokoll, 
also das Interface für die zusammenarbeit zwischen extensions mitliefern und durch Cmount extensions jederzeit erweiter bar sein. 
Es gibt also im Einheitsprotokoll Laufzeit und Buildtime events und abfragen. 
Eine mit Cmount gebaute anwendung kann selbst auf das einheitsprotokoll, 
also auf die extensions zugreifen und so eine modding api bereitstellen, 
die über das einheitsprotokoll verfügbar ist. 
Man kann dann die mit Cmount gebaute Anwendung mit argumenten starten, um extensions zu installieren. 
Ich wiederhole also, extensions können immer zum einheitsprotokoll installiert werden und damit interagieren. 
Dieses Konzept besteht beim Buildprogramm Cmount und bei Programmen die mit Cmount gebaut sind. 
Die mit Cmount gebauten Programme können, wie Cmount und die extensions auf ein gemeinsames Protokoll zugreifen, 
um miteinander zu kommunizieren.

Über das Protokoll ist der gesamte build prozess organisiert um im Development JIT und Hot reload zu ermöglichen (Für C, Python und Dart sowie für alle anderen Sprachen die man noch hinzufügen könnte) und im Release mode wird AOT verwendet und optimiert und zur Laufzeit des mit Cmount erstellten Programmes können weitere Extensions installiert werden


# Cmount

---

## 1. Überblick und Motivation

- **Cmount** ist eine Anwendung, geschrieben in C.  
- Sie soll von Haus aus mehrere Sprachen unterstützen:  
  - **C** (direkte Integration)  
  - **Dart** (über Extensions)  
  - **Python** (über Extensions)  
- Die Erweiterung weiterer Sprachen ist vorgesehen.  
- Hauptmotivationen:  
  - **Hohe Modularität** und **Flexibilität** in der Entwicklung.  
  - **Hot Reload**-Fähigkeiten während der Entwicklungsphase.  
  - Gemeinsame **Schnittstelle** (Einheitsprotokoll) für alle Erweiterungen.  
  - **AOT**-Kompilierung (Ahead Of Time) für Release-Builds, um Performance und Kompatibilität zu steigern.  

---

## 2. Ziele

1. **Erweiterbare Entwicklungsumgebung**  
   - Ermöglicht es, **Extensions** hinzuzufügen, zu aktualisieren und zu entfernen.  
   - Extensions können untereinander interagieren und voneinander abhängen.  

2. **Interaktion mehrerer Sprachen**  
   - **C**, **Dart** und **Python** sollen in der Entwicklungsphase (Development) Hot Reload ermöglichen.  
   - Gemeinsame **APIs** und **Events** für die Kommunikation zwischen den Sprachen.  

3. **Einheitsprotokoll**  
   - Ein zentrales Protokoll, das den **Buildprozess** und die **Laufzeit** steuert.  
   - Standardisierte Schnittstelle, um das Zusammenspiel aller Extensions zu ermöglichen (inkl. **Modding-API**).  

4. **Konsistente Build- und Release-Phasen**  
   - **Development-Phase**: Fokussiert auf Hot Reload, JIT, schnelle Iteration.  
   - **Release-Phase**: Fokussiert auf AOT-Kompilierung, Performance, Stabilität.  
   - **Runtime**: Ermöglicht das Nachladen und Installieren neuer Extensions im laufenden Betrieb.  

5. **Klar strukturierte Projekt- und Binary-Verwaltung**  
   - **Ordner- und Dateikonventionen** für Projekte in Cmount.  
   - Einheitliche **Ausgabeformate** für Binaries, unabhängig von der verwendeten Sprache.  

---

## 3. Aufbau und Komponenten

### 3.1 Cmount-Kern
- In **C** implementiert.
- Enthält die grundlegende **Build-Logik**, **Hot Reload**-Mechanismen sowie das **Einheitsprotokoll**.  
- Ermöglicht die direkte Integration von C-Code (ohne zusätzliche Extension).

### 3.2 Extensions
- **Dart** und **Python** werden als **Extensions** angebunden.  
- Können untereinander kommunizieren und voneinander abhängen (z.B. eine Dart-Extension, die eine Python-Extension nutzt).  
- Greifen über ein gemeinsames **Interface** auf Cmount zu:
  - **Events**: Laufzeit- und Buildtime-Events.  
  - **API**: Zugriff auf das Einheitsprotokoll und aufeinander.  

### 3.3 Einheitsprotokoll
- Zentrale Schnittstelle für **Kommunikation** und **Koordination**:  
  - **Build-Prozess** (z.B. Hot Reload, JIT, AOT-Kompilierung).  
  - **Runtime** (z.B. Events, Installation weiterer Extensions).  
- Definiert, wie Extensions ihre **Funktionen**, **Konfigurationen** und **Abhängigkeiten** registrieren.  
- Sorgt für eine konsistente **Modding-API**, sodass auch externe Programme, die mit Cmount gebaut sind, Erweiterungen zulassen können.

---

## 4. Entwicklungsphasen

### 4.1 Development-Phase
- Fokus auf **schnelle Iteration** durch **Hot Reload** und **JIT**.  
- **C**, **Dart** und **Python** sollen gemeinsam lauffähig sein und miteinander interagieren können.  
- Event-Handling während der Entwicklung:  
  - **Buildtime-Events** für den Kompilierungsvorgang.  
  - **Laufzeit-Events** für dynamische Änderungen (Hot Reload).  

### 4.2 Release-Phase
- **AOT-Kompilierung** für alle beteiligten Sprachen, sofern möglich (z.B. Dart, C).  
- Erstellung optimierter **Binaries**, die weiterhin über das Einheitsprotokoll miteinander interagieren können.  
- **Erweiterbarkeit** bleibt erhalten: Auch im Release-Betrieb können Extensions installiert oder aktualisiert werden, sofern das Anwendungskonzept dies vorsieht.

### 4.3 Runtime
- Laufende Anwendung (entweder Cmount selbst oder ein mit Cmount erstelltes Programm).  
- Kann mittels **Argumenten** oder **Befehlen** neue Extensions installieren, entfernen oder aktualisieren.  
- Greift ebenfalls auf das **Einheitsprotokoll** zu, um interne Funktionen anzubieten (z.B. Modding-API für Nutzer).

---

## 5. Mögliche Einsatzgebiete

1. **Toolchains**  
   - Cmount kann als **Build-Umgebung** für komplexe Projekte dienen, in denen mehrere Sprachen parallel genutzt werden.  
   - Gemeinsames Protokoll vereinfacht das Erstellen von **Automatisierungsskripten** und CI/CD-Pipelines.

2. **Erweiterbare Anwendungen**  
   - Applikationen, die im Laufe der Zeit um neue Module oder **Plugins** erweitert werden sollen.  
   - **Modding-Szenarien** für Spiele oder Tools, bei denen Endnutzer über das Einheitsprotokoll zusätzliche Funktionen hinzufügen.

3. **Rapid Prototyping**  
   - Durch Hot Reload in **C**, **Dart** und **Python** lassen sich Ideen schnell umsetzen.  
   - Flexibler Wechsel zwischen Sprachen in einer gemeinsamen Umgebung.

4. **Integrationsplattform**  
   - Nutzung als Middleware, um verschiedene Sprach-Ökosysteme unter einem Dach zusammenzuführen.  
   - Bietet einheitliche Schnittstellen und ein zentrales Event-System.

---

## 6. Struktur und Projektorganisation

- **Projektordner**:
  - Enthält **Quellcode** für C, Dart und Python (nach Möglichkeit in getrennten, klar benannten Ordnern).  
  - **Extensions-Ordner**: Alle externen Module/Plugins, die per Einheitsprotokoll eingebunden werden.  
- **Build-Artefakte**:
  - Getrennte **Build-Ordner** für Development- und Release-Builds.  
  - **Konfigurationsdateien** (z.B. cmount_config, extension_config), die Abhängigkeiten und Events definieren.  
- **Ausführbare Dateien**:
  - **Cmount-Binary** (z.B. `cmount` oder `cmount.exe`).  
  - **Extensions**: Kompilierte oder interpretierte Module (abhängig von der Sprache).  
  - **Finale Binaries** für das Release, in denen sämtliche benötigten Komponenten eingebettet sind.

---

## 7. Einheitsprotokoll im Detail

1. **Registrierung**  
   - Jede Extension meldet sich beim Einheitsprotokoll an und deklariert ihre **Fähigkeiten** (z.B. Event-Listener, Funktionen).  
2. **Event-System**  
   - **Buildtime-Events**: Trigger für Kompilierung, Tests, Packaging.  
   - **Laufzeit-Events**: Trigger für Hot Reload, Installation neuer Extensions, Interaktionen zwischen Modulen.  
3. **API-Zugriff**  
   - Gemeinsame Funktionen zum **Senden und Empfangen** von Daten.  
   - Möglichkeit, **Abhängigkeiten** zu deklarieren und von anderen Extensions bereitgestellte Funktionen aufzurufen.  
4. **Modding-API**  
   - Anwendbar auf Cmount selbst und auf Programme, die mit Cmount gebaut wurden.  
   - Anwender können über Startargumente oder Konfigurationsdateien neue Extensions installieren und aktivieren.  

---

## 8. Fazit und Ausblick

Mit **Cmount** soll eine **flexible**, **modulare** und **spracheübergreifende** Plattform entstehen, die sowohl den Entwicklungs- als auch den Release-Prozess abdeckt.  
Die **Kernpunkte** sind:

- **Hot Reload** in der Development-Phase für schnelle Iterationen.  
- **AOT-Kompilierung** im Release-Modus für Leistung und Stabilität.  
- Ein **einheitliches Protokoll** (Einheitsprotokoll) für die Koordination zwischen C, Dart, Python und möglichen zukünftigen Sprachen.  
- **Extensions** und **Modding-API**, um die Plattform über die gesamte Lebensdauer hinweg erweitern zu können.  

Diese Zielproduktsdefinition soll als **roter Faden** für die Weiterentwicklung von Cmount dienen und die Basis für die konkrete technische Umsetzung bilden. Sie zeigt, wie die einzelnen Bausteine (Cmount-Kern, Extensions, Einheitsprotokoll, Build-Prozess, Runtime) zusammenspielen und welche Einsatzszenarien sich daraus ergeben.

