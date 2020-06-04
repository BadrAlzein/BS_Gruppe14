#ifndef Coroutine_h
#define Coroutine_h

#include "device/CPU.h"

/*
 * Coroutine:
 * Diese Klasse implementiert Coroutinen, welche die Basis
 * f�r alle Arten von Prozessen in Co-Stubs sind.
 *
 *	Anmerkung: wir verwenden ein objektorientiertes
 *	Coroutinen-Modell, daher ist diese Klasse abstrakt.
 *	Eine Coroutine muss sich von dieser Klasse ableiten
 *	und eine "body" Methode definieren.
 *	Diese Methode ist dann gewissermassen die "main" Funktion
 *	fuer die Coroutine.
 *
 */

/* Diese Deklaration verweist auf die von Euch zu
 * implementierende Assemblerprozedur "switchContext".
 */
extern "C" {
	void switchContext(void*& from, void*& to);
}

/* switchContext hat die Aufgabe, die Kontrolle
 * vom Stack der Coroutine "from" auf den Stack der
 * Coroutine "to" zu wechseln und damit einen Kontrolltransfer
 * zu vollziehen.
 *
 * Hinweis: Am einfachsten ist es, einfach die betreffenden Register
 * auf den aktuellen Stack zu retten (pushen) und den Stackpointer (esp)
 * danach an der Adresse, auf die "from" verweist, zu sichern.
 * Nachdem das geschehen ist, wird der Stack gewechselt,
 * indem man den neuen Stackpointerwert von der Adresse
 * auf die "to" zeigt in das Stackpointerregister esp laedt.
 * Dadurch haben wir den Stack gewechselt und wir koennen nun
 * einfach alle auf diesem Stack geretteten Register wiederherstellen
 * (in umgekehrter Reihenfolge poppen). Zum Schluss fuehren
 * wir einfach eine "ret" Instruktion aus, die dazu fuehrt,
 * dass die Coroutine "to" an der Stelle weiterlaeuft,
 * an der sie das letzte mal "switchContext" aufgerufen hat.
 * F�r Coroutinen die zum ersten mal aktiviert werden, muss
 * deshalb ein Stackframe existieren, was gleich aussieht mit
 * dem einer Coroutine die "switchContext" aufgerufen hat.
 *
 * Edit Robert:
 *
 *      1. Register pushen bzw. sichern
 *      2. esp an der Adresse von "from" sichern
 *      3. esp <= to; Wechsel in den neuen Stack
 *      4. Alle gesicherten Register wiederherstellen (pop)
 *      5. ret;
 *         ret nutzt die gegebene Rücksprungadresse, um an einen
 *         Programmpunkt zurückzukehren. Diesen haben wir mit esp <= to
 *         geändert.
 *      6. Die Coroutine läuft an der Stelle weiter, an welcher zuletzt
 *         switchContext() aufgerufen wurde.
 *
 */

class Coroutine {
public:

	/* Aufsetzen einer neuen Coroutine.
	*/
	Coroutine(void* tos = 0)
	{
		setup(tos);
	}

    ~Coroutine()
    {

        // if (sp!=NULL) helper_free(sp);
    }

	/* Kontrolltransfer von dieser Coroutine zu "next"
	 * Die eigentliche Arbeit erledigt "switchContext"
	 */
	void resume(Coroutine* next)
    {
		switchContext(this->sp, next->sp);
	}

	/* Dies ist der Rumpf der Coroutine
	 * und muss in abgeleiteten Klassen definiert werden.
	 */
	virtual void body() = 0;

	/* Diese Methode wird aufgerufen
	 * wenn der Rumpf unserer Coroutine beendet ist
	 * und muss in abgeleiteten Klassen definiert werden.
	 */
	virtual void exit() = 0;


    void* sp; // Der gerettete Stackpointer

private:

    struct SetupFrame {
        unsigned    edi;
        unsigned    esi;
        unsigned    ebx;
        void*       bp;
        void        (*startup)(Coroutine*);
        void*       nirwana;
        Coroutine*  arg;
    };


    /* Diese Funktion hat nur die Aufgabe
     * den Rumpf der uebergebenen Coroutine aufzurufen
     * und nach der Rueckkehr exit() aufzurufen,
     * um die Termination des Rumpfes bekanntzugeben.
     * Diese Prozedur dient so als eine Bruecke zwischen der
     * nicht objektorientierten und der objektorientierten Welt
     * Beachte, das "startup" als "static deklariert ist
     * und deshalb keinen impliziten "this"-Zeiger uebergeben bekommt.
     */
    static void startup(Coroutine* obj) {
        CPU::enableInterrupts();

        obj->body();
        obj->exit();
    };


    /* Aufsetzen einer neuen Coroutine.
     * Der Parameter "tos" (top of stack) ist der
     * initiale Stackpointerwert fuer die neue Coroutine
     * ACHTUNG: tos kann NULL sein (siehe Constructor)!
     */

    void setup(void* tos) {

        if (tos == 0) {
            return;
        }

        tos = (void*) (((SetupFrame*) tos) - 1);

        SetupFrame frame;
        frame.edi       = 0;
        frame.esi       = 0;
        frame.ebx       = 0;
        frame.bp        = (void*) 0;
        frame.arg       = (Coroutine*) this;    // Coroutine zu einem Zeiger casten
        frame.startup   = &startup;
        frame.nirwana   = (void*) 0x0;     // random

        *((SetupFrame*) tos) = frame;

        this->sp = (void*) tos;

        return;

    };

};

#endif