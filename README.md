В данной ветке представлена реализация бенчмарка модели Блэка и Шоулза для нахождения справедливой цены опциона (https://ru.wikipedia.org/wiki/Модель_Блэка_—_Шоулза) под архитектуру RISC-V с использованием векторных интринсиков.
В данной реализации использовались следующие методы оптимизации алгоритма:
1. Весь алгоритм был векторизован с помощью векторных интринсиков.
2. Алгоритм был расспараллелен  с ручным делением по блоках длины векторного регистра с использованием библиотеки OpenMP.
3. Были написаны векторные реализации  следующих математических функций (дабы свести выгрузки в память из векторных регистров к минимуму):
   1) exp().
   2) erf().
   3) log().
   Для каждой из них был выбран или написан подходящий алгоритм, точности которого хватает для нашей прикладной задачи. Так же для ускорения алгоритма можно уменьшить точность вычисления полиномов, т.к в нашей задаче нам хватит точности до 4 знаков после запятой.
   Для каждой версии были проведены замеры, результаты которых находядтся в папке "Результаты замеров". Помимо результатов в этой папке находятся графики ускорения и эффективности распараллеливания.                                                                                                   
 В папке предоставлены замеры для следующих версий программы:
1) Базовая версия.
2) Версия векторизованная с помощью RISC-V векторных интринсиков.
3) Распараллеленная с неправильным делением по блокам ( без ручной обработки последнего блока).
4) Распараллеленная с правильным делением по блокам (с ручной обработкой размера последнего блока).
5) Распараллеленная с  использованием векторной реализации корня (есть в riscv-vector.h).
6) Распараллеленная с  использованием векторной реализации exp(). Репозиторий с её реализацией и тестом представлен в  моём гитхаб профиле.
7) Распараллеленная с  использованием векторной реализации erf(). Репозиторий с её реализацией и тестом представлен в  моём гитхаб профиле.
8) Распараллеленная с  использованием векторной реализации log(). Репозиторий с её реализацией и тестом представлен в  моём гитхаб профиле. 8 версия на данный момент самая производительная и подсчитывает цену справедливого опциона за 100 млн различных акций за 0,7 сек.
   
