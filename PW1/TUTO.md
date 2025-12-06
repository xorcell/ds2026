# How to make it work

In this directory, type 
```sh
make test
```
It will automaically create folders and binaries (with debug flags, use make all instead if you don't want it)
## Server part
Then go on server test folder 

```sh
cd tests/server_test
```
Create an exemple file

```sh
echo "Salut!" > hello.txt
```
and finally launch the server
```sh
./server 2020
```
There you go for the server

## Client part
In another shell go to the client test folder

then launch the client
```sh
./client 127.0.0.1 2020 hello.txt
``` 
