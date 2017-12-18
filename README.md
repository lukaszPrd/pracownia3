Temat: Problem synchronizacyjny (rozwinięcie problemu producenta-konsumenta)

Specyfikacja.
Celem pracowni jest stworzenia symulatora średniowiecznej wioski. Każda z postaci to osobny wątek i każda z nich wykonuje dziennie określone czynności, wchodząc w interakcję z innymi osobami oraz zasobami.

Wątki.
- rycerz (Odwiedza damę serca jeśli jest ona akurat w domu, walczy z innymi rycerzem jeśli spotyka go u damy serca, przesiaduje w karczmie) 
- oberżysta (Kupuje towary u handlarza, obsługuje rycerzy w karczmie)
- handlarz (Sprzedaje towary)
- dama serca (Kupuje towary u handlarza, spotyka się z rycerzem)

Powiązania.
- Każdy z rycerzy wybiera losowo jedną damę serca i codziennie ją odwiedza, jeśli akurat jest w domu. Gdy 2 spotka się z tą samą, walczą na śmierć i życie. O wyniku walki decyduje liczba wyrzuconych oczek na kostce.
- Rycerz może odejść z wioski, jeśli odejdzie jego dama serca.
- Dama serca może odejśc z wioski, jeśli braknie dla niej towarów.
- Oberżysta może odejść z wioski, jeśli braknie dla niego towarów lub braknie rycerzy.
- Handlarz może odejść, jeśli nic nie sprzeda.
