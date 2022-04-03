#!/usr/bin/env bash

export MY_PASSWORD=MY_SECRET_PASSWORD
echo -n "Some secret message!" > plain_text.txt
SALT='66AA1122060A0102'

echo "Case 1. Use pass phrase:"
echo "Plain text: '$(cat plain_text.txt)' ($(cat plain_text.txt | wc -c) bytes)"                                        | sed -e 's/^/  /'
# sed -e 's/^/  /' -- просто добавляет отступ в два пробела к каждой выведенной строке
# -p -- опция, чтобы выводить соль, ключ, стартовый вектор
openssl enc -aes-256-ctr -S $SALT -in plain_text.txt -out cipher_text.txt -pass env:MY_PASSWORD -p                      | sed -e 's/^/  /'
echo -e "Ciphertexthexdump: '''\n$(hexdump -C cipher_text.txt)\n''' ($(cat cipher_text.txt | wc -c) bytes)"             | sed -e 's/^/  /'
openssl enc -aes-256-ctr -d -in cipher_text.txt -out recovered_plain_text.txt -pass env:MY_PASSWORD
echo "Recovered plaintext: '$(cat recovered_plain_text.txt)'"                                                           | sed -e 's/^/  /'

echo
echo

IV='E4DEC57ADC9A771DC72A77775A1CF4FF'
KEY='BBC5929AA59B56851391DD723922C2E0F31A2FC873D52D3FBA3FD5391CAD471E'
echo "Case 2. Use explicit key and IV:"
echo "Plain text: '$(cat plain_text.txt)' ($(cat plain_text.txt | wc -c) bytes)"                                        | sed -e 's/^/  /'
openssl enc -aes-256-ctr -in plain_text.txt -out cipher_text.txt -iv $IV -K $KEY -p                                     | sed -e 's/^/  /'
echo -e "Ciphertexthexdump: '''\n$(hexdump -C cipher_text.txt)\n''' ($(cat cipher_text.txt | wc -c) bytes)"             | sed -e 's/^/  /'
openssl enc -aes-256-ctr -d -in cipher_text.txt -out recovered_plain_text.txt -iv $IV -K $KEY
echo "Recovered plaintext: '$(cat recovered_plain_text.txt)'"                                                           | sed -e 's/^/  /'
