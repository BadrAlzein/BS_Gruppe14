#include "thread/Activity.h"
#include "thread/ActivityScheduler.h"
#include "io/PrintStream.h"
/* Aufsetzen eines Threads, der initiale Zustand ist "Blocked",
	 * da der Thread erst laufen darf, wenn der spezielle Konstruktor
	 * der abgeleiteten Klasse abgearbeitet ist. Die Aktivierung
	 * erfolgt von der abgeleiteten Klasse mittels "wakeup".
	*/
/**
initialisinglist from the activity.h class
the initail state is blocked and waiting for a
 task to take place

**/
Activity::Activity(void *tos) : Coroutine(tos)
{

    this->state = BLOCKED;
    // waiti
    // the thread will be activated with the help of class Activity
}

/* Verpacken des aktuellen Kontrollflusses als Thread.
	 * Wird nur f�r den Hauptkontrollfluss "main" ben�tigt.
	 * Es wird hier kein Stack initialisiert.
	 * Beachte das Activity wegen der Vererbungsbeziehung von
	 * Coroutine abstrakt ist. Bei Bedarf muss "body" direkt
	 * aufgerufen werden.
	 */
// after creating the activity it should be activated
Activity::Activity()
{
    this->state = BLOCKED;
    scheduler.start(this);
}
/* Im Destruktor muss ein explizites Terminieren dieser Aktivitaet erfolgen.
	 * Das muss geschehen, da aufgrund der Aufrufreihenfolge von
	 * Destruktoren bereits der abgeleitete Teil des Activity-Objekts zerstoert
	 * wurde und diese Aktivitaet daher keine Laufzeit mehr erhalten darf.
	 * Das Warten auf die Beendigung (mittels join()) muss im Destruktor der
	 * von Activity am weitesten abgeleiteten Klasse erfolgen.
	 */

Activity::~Activity()
{
    //as soon as a process executes the last instruction , this function will be called
    scheduler.kill(this);
}
/* Veranlasst den Scheduler, diese Aktivitaet zu suspendieren.
	 */
void Activity::sleep()
{
    //suspend the active process
    //sleep can be called from and active process
    scheduler.suspend();
}
/* Veranlasst den Scheduler, diese Aktivitaet aufzuwecken.
	 */
void Activity::wakeup()
{
    if (this->state == READY)
    {
        // queue the Process
        scheduler.schedule(this);
    }
}
/* Diese Aktivitaet gibt die CPU vorruebergehend ab.
	 */
void Activity::yield()
{
    //the process gives the processor the first element from the ready list
    //and will queue again at the end of the list
    changeTo(READY); //(Pending->)
    scheduler.reschedule();
}
/* Diese Aktivitaet wird terminiert. Hier muss eine eventuell
	 * auf die Beendigung wartende Aktivit�t geweckt werden.
	 */
void Activity::exit()
// to terminate the process we set it to zombie states according to the ctivity.h clss

{
    //
    if (this->state == ZOMBIE)
    {
        scheduler.exit();
    }
}
/* Der aktuelle Prozess wird solange schlafen gelegt, bis der
	 * Prozess auf dem join aufgerufen wird beendet ist. Das
	 * Wecken des wartenden Prozesses �bernimmt exit.
	 */
void Activity::join()
{

    scheduler.getCurrentActivity()->sleep();

}   
/* �ndern des Ausf�hrungszustandes. Diese Methode sollte nur vom
	 * Scheduler verwendet werden.
	 */
void Activity::changeTo(State state)
{
    this->state = state;
}