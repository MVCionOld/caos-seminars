[Общий ридинг](https://github.com/victor-yacovlev/mipt-diht-caos/tree/master/practice/file_io#%D0%BA%D0%BE%D0%BC%D0%BF%D0%B8%D0%BB%D1%8F%D1%86%D0%B8%D1%8F-%D0%B8-%D0%B7%D0%B0%D0%BF%D1%83%D1%81%D0%BA-windows-%D0%BF%D1%80%D0%BE%D0%B3%D1%80%D0%B0%D0%BC%D0%BC-%D0%B8%D0%B7-linux)
  
Небольшое резюме:
 * Вместо файловых дескрипторов - `HANDLE` (вроде это просто `void*`)
 * Много алиасов для типов вроде `HANDLE`, `DWORD`, `BOOL`, `LPTSTR`, `LPWSTR`.  
 * Очень много аргументов у всех функций
 * Плохая документация, гуглится все плохо
 * Надо установить `wine` и `mingw-w64`
   * `sudo apt install gcc-mingw-w64`
   * `sudo apt install wine64`