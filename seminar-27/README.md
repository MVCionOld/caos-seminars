# Семинар №27
## Filesystem in USErspace

Заголовочные файлы, в которых есть функции для работы с файловой системой:

| Header file | Description |
|-------------|-------------|
| `<fcntl.h>` |	File opening, locking and other operations |
| `<fnmatch.h>` |	Filename matching |
| `<ftw.h>` |	File tree traversal |
| `<sys/stat.h>` |	File information (`stat` et al.) |
| `<sys/statvfs.h>` |	File System information |
| `<dirent.h>` | Directories opening, traversing |


`read`, `write`, `stat`, `fstat` - это все было раньше...

---

### Чтение/запись/... без `FUSE`

![](./pics/ko_module.png)

### Чтение/запись/... при `FUSE`

![](./pics/fuse_module.png)

[FUSE и Python](https://habr.com/ru/post/315654/)

### `libfuse`

[Общий ридинг](https://github.com/victor-yacovlev/mipt-diht-caos/tree/master/practice/fuse#%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA%D0%B0-libfuse)