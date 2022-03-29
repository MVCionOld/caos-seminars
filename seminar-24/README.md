# Семинар №24
## Протокол HTTP и библиотека cURL

---

### Введение

`HTTP (HyperText Transfer Protocol)` — протокол прикладного/транспортного уровня передачи данных. Изначально был создан как протокол прикладного уровня для передачи документов в html формате (теги и все вот это). Но позже был распробован и сейчас может используется для передачи произвольных данных, что характерно для транспортного уровня.

Протокол HTTP используется браузерами для загрузки и отправки контента. Кроме того, благодаря своей простоте и универсальности, он часто используется как высокоуровневый протокол клиент-серверного взаимодействия.

Большинство серверов работают с версией протокола `HTTP/1.1`, который подразумевает взаимодействие в текстовом виде 
через TCP-сокет.
На семинаре будем рассматривать `HTTP/1.1`, но стоит знать, что текущая версия протокола существенно более эффективна.

`HTTP/1.1` клиент отправляет на сервер текстовый запрос, который содержит:

* Команду запроса
* Заголовки запроса
* Пустую строку - признак окончания заголовков запроса
* Передаваемые данные, если они подразумеваются

В ответ сервер должен отправить:
* Статус обработки запроса
* Заголовки ответа
* Пустую строку - признак окончания заголовков ответа
* Передаваемые данные, если они подразумеваются

[О том как HTTP/2 сделает веб быстрее](https://habr.com/ru/company/nix/blog/304518/)

| HTTP 1.1 | HTTP/2 |
|----------|--------|
| одно соединение - один запрос, <br> как следствие вынужденная конкатенация, встраивание и спрайтинг (spriting) данных | несколько запросов на соединение |
| все нужные заголовки каждый раз отправляются полностью | сжатие заголовков, позволяет не отправлять каждый раз одни и те же заголовки |
| | возможность отправки данных по инициативе сервера |
| текстовый протокол | двоичный протокол |
| | приоритезация потоков - клиент может сообщать, что ему более важно| 

Стандартным портом для HTTP является порт `80`, для HTTPS - порт с номером `443`, но это жёстко не регламентировано, и при необходимости номер порта может быть любым.

---

### Основные команды и заголовки HTTP
 * `GET` - получить содержимое по указанному URL;  
 * `HEAD` - получить только метаинформацию (заголовки) по-указанному URL, но не содержимое;  
 * `POST` - отправить данные на сервер и получить ответ.

"Не основных" очень [много](https://www.tutorialspoint.com/http/http_methods.htm).  

Заголовки - это строки вида `ключ: значение`, определяющие дополнительную метаинформацию запроса или ответа:
```shell
Date:           Mon, 27 Jul 2009 12:28:53 GMT
Server:         Apache/2.2.14 (Win32)
Last-Modified:  Wed, 22 Jul 2009 19:15:56 GMT
ETag:           "34aa387-d-1568eb00"
Vary:           Authorization,Accept
Accept-Ranges:  bytes
Content-Length: 88
Content-Type:   text/html
Connection:     Closed
```

По стандарту `HTTP/1.1`, в любом запросе должен быть как минимум один заголовок - Host, определяющий имя сервера. Это связано с тем, что с одним IP-адресом, на котором работает HTTP-сервер, может быть связано много доменных имен.

Полный список заголовков можно посмотреть в [Википедии](https://en.wikipedia.org/wiki/List_of_HTTP_header_fields).

---

### HTTPS

Протокол `HTTPS` - это реализация протокола `HTTP` поверх дополнительного уровня SSL, который, в свою очередь работает через TCP-сокет. На уровне SSL осуществляется проверка сертификата сервера и обмен ключами шифрования. После этого - начинается обычное взаимодействие по протоколу HTTP в текстовом виде, но это заимодейтвие передается по сети в зашифрованном виде.

---

### Инструменты

* `netcat`
* `telnet`
* `openssl s_client`
* `curl`
* `wget`
* `libcurl`

#### netcat

`netcat` — утилита Unix, позволяющая устанавливать соединения TCP и UDP, принимать оттуда данные и передавать их, а 
также может помочь вам в мониторинге и тестировании сетевых соединений.

* `-h` Справка
* `-v` Вывод информации о процессе работы (verbose)
* `-o <выходной_файл>` Вывод данных в файл
* `-i` <число> Задержка между отправляемыми данными (в секундах)
* `-z` Не посылать данные (сканирование портов)
* `-u` Использовать для подключения UDP протокол
* `-l` Режим прослушивания
* `-p <число>` Локальный номер порта для прослушивания. Используется с опцией -l
* `-s <host>` Использовать заданный локальный («свой») IP-адрес
* `-n` Отключить DNS и поиск номеров портов по /etc/services
* `-w <число>` Задать тайм-аут (в секундах)
* `-q <число>` Задать время ожидания после передачи данных, после истечение которого соединение закрывается

Функции `netcat` не ограничены отправкой TCP и UDP пакетов. Она также может прослушивать порты. Это позволяет создать 
соединение между программами `netcat` на различных компьютерах по принципу «клиент-сервер». Роли сервера и клиента важны
только при первоначальной конфигурации. После установки соединения связь в обоих направлениях будет абсолютно одинаковой.
На одной из машин нужно указать команде netcat прослушивать определенный порт.

#### telnet

`telnet` - это сетевая утилита, которая позволяет соединиться с удаленным портом любого компьютера и установить 
интерактивный канал связи, например, для передачи команд или получения информации. Можно сказать, что это универсальный 
браузер в терминале, который умеет работать со множеством сетевых протоколов.

Например:
```bash
% telnet ejudge.atp-fivt.org 80
Trying 87.251.82.74...
Connected to atp-fivt.org.
Escape character is '^]'.
GET / HTTP/1.1
Host: ejudge.atp-fivt.org

HTTP/1.1 301 Moved Permanently
Server: nginx/1.14.0 (Ubuntu)
Date: Tue, 29 Mar 2022 13:15:27 GMT
Content-Type: text/html
Content-Length: 194
Connection: keep-alive
Location: https://ejudge.atp-fivt.org/

<html>
<head><title>301 Moved Permanently</title></head>
<body bgcolor="white">
<center><h1>301 Moved Permanently</h1></center>
<hr><center>nginx/1.14.0 (Ubuntu)</center>
</body>
</html>
```

#### openssl s_client

Аналогом `telnet` для работы поверх SSL является инструмент `s_client` из состава OpenSSL:

Например:
```bash
% openssl s_client -connect yandex.ru:443
CONNECTED(00000005)
depth=2 C = PL, O = Unizeto Technologies S.A., OU = Certum Certification Authority, CN = Certum Trusted Network CA
verify return:1
depth=1 C = RU, O = Yandex LLC, OU = Yandex Certification Authority, CN = Yandex CA
verify return:1
depth=0 C = RU, L = Moscow, OU = ITO, O = Yandex LLC, CN = yandex.ru
verify return:1
---
Certificate chain
 0 s:/C=RU/L=Moscow/OU=ITO/O=Yandex LLC/CN=yandex.ru
   i:/C=RU/O=Yandex LLC/OU=Yandex Certification Authority/CN=Yandex CA
 1 s:/C=RU/O=Yandex LLC/OU=Yandex Certification Authority/CN=Yandex CA
   i:/C=PL/O=Unizeto Technologies S.A./OU=Certum Certification Authority/CN=Certum Trusted Network CA
 2 s:/C=PL/O=Unizeto Technologies S.A./OU=Certum Certification Authority/CN=Certum Trusted Network CA
   i:/C=PL/O=Unizeto Sp. z o.o./CN=Certum CA
---
Server certificate
-----BEGIN CERTIFICATE-----
MIII...0YSL
-----END CERTIFICATE-----
subject=/C=RU/L=Moscow/OU=ITO/O=Yandex LLC/CN=yandex.ru
issuer=/C=RU/O=Yandex LLC/OU=Yandex Certification Authority/CN=Yandex CA
---
No client certificate CA names sent
Server Temp Key: ECDH, X25519, 253 bits
---
SSL handshake has read 5056 bytes and written 281 bytes
---
New, TLSv1/SSLv3, Cipher is ECDHE-ECDSA-CHACHA20-POLY1305
Server public key is 256 bit
Secure Renegotiation IS supported
Compression: NONE
Expansion: NONE
No ALPN negotiated
SSL-Session:
    Protocol  : TLSv1.2
    Cipher    : ECDHE-ECDSA-CHACHA20-POLY1305
    Session-ID: C61578....D37940919041A
    Session-ID-ctx: 
    Master-Key: 45E...256
    TLS session ticket lifetime hint: 100800 (seconds)
    TLS session ticket:
    0000 - f3 0b 72 1a 65 e7 37 46-f5 fd 90 80 b6 d1 de 63   ..r.e.7F.......c
    ...
    0090 - 23 1e 15 11 61 e7 cf f3-33 ce af a5 15 26 82 10   #...a...3....&..

    Start Time: 1648559771
    Timeout   : 7200 (sec)
    Verify return code: 0 (ok)
---
```

#### curl

Универсальным инструментом для взаимодействия по HTTP в Linux считается `curl`, которая входит в базовый состав всех дистрибутивов. Работает не только по протоколу HTTP, но и HTTPS.

Основные опции `curl`:

 * `-v` - отобразить взаимодействие по протоколу HTTP;
 * `-X КОМАНДА` - отправить вместо `GET` произвольную текстовую команду в запросе;
 * `-I` - ответ до тела с контентом;
 * `-H "Ключ: значение"` - отправить дополнительный заголовок в запросе; таких опций может быть несколько;
 * `--data-binary "какой-то текст"` - отправить строку в качестве данных (например, для `POST`);
 * `--data-binary @имя_файла` - отправить в качестве данных содержимое указанного файла.

Например:
```bash
% curl -I --http2 https://www.keycdn.com
HTTP/2 200 
server: keycdn-engine
date: Tue, 29 Mar 2022 13:31:09 GMT
content-type: text/html
last-modified: Mon, 07 Mar 2022 09:05:17 GMT
vary: Accept-Encoding
etag: W/"6225cacd-fd51"
expires: Tue, 05 Apr 2022 13:31:09 GMT
cache-control: max-age=604800
strict-transport-security: max-age=31536000; includeSubdomains; preload
content-security-policy: default-src 'self' 'unsafe-inline' 'unsafe-eval' https: data:
x-frame-options: SAMEORIGIN
x-xss-protection: 1; mode=block
x-content-type-options: nosniff
referrer-policy: no-referrer-when-downgrade
x-cache: HIT
x-edge-location: defr
access-control-allow-origin: *
```

#### wget

`wget` - в первую очередь предназначен для скачивания файлов. Например, умеет выкачивать страницу рекурсивно.

Например:
```bash
% wget ejudge.atp-fivt.org -O - | head -n 10
--2022-03-29 16:35:26--  http://ejudge.atp-fivt.org/
Распознаётся ejudge.atp-fivt.org (ejudge.atp-fivt.org)… 87.251.82.74
Подключение к ejudge.atp-fivt.org (ejudge.atp-fivt.org)|87.251.82.74|:80... соединение установлено.
HTTP-запрос отправлен. Ожидание ответа… 301 Moved Permanently
Адрес: https://ejudge.atp-fivt.org/ [переход]
--2022-03-29 16:35:26--  https://ejudge.atp-fivt.org/
Подключение к ejudge.atp-fivt.org (ejudge.atp-fivt.org)|87.251.82.74|:443... соединение установлено.
HTTP-запрос отправлен. Ожидание ответа… 200 OK
Длина: 5153 (5,0K) [text/html]
Сохранение в: «STDOUT»

-                     0%[                    ]       0  --.-KB/s               <html>
<head>
    <title>АКОС ФПМИ МФТИ [Operating Systems at MIPT]</title>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="/Material-Icons.css">
    <link rel="stylesheet" href="/material.red-deep_orange.min.css">
    <script defer src="/material.min.js"></script>
</head>
-                   100%[===================>]   5,03K  --.-KB/s    за 0s      

/2022-03-29 16:35:26 (1,20 GB/s) - записан в stdout [5153/5153]
```

#### libcurl

У утилиты `curl` есть программный API, который можно использовать в качестве библиотеки, не запуская отдельный процесс.

API состоит из двух частей: полнофункциональный асинхронный интерфейс (`multi`), и упрощённый с блокирующим вводом-выводом (`easy`).
