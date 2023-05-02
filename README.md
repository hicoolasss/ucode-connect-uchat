# TokyoNight chat 

This is our implementation of the real-time messaging app for the ucode-connect-uchat project.


## Developers

- [@iserikov](https://github.com/hicoolasss) - client-part developer.
- [@kyehorov](https://github.com/KyryloYehorov) - server-part developer.


## Demo

Here you can find our denmo - https://www.youtube.com/watch?v=fcCUp_We3Ng&ab_channel=criops



## Installation

### For linux


***IMPORTANT*** - version of **TokyoNight** chat below tested only on Kali linux!

***IMPORTANT*** - to compile **TokyoNight** chat you need to install : *sqlite3, cjson, openssl, gtk4.*

- Firstly you need to clone our project with this command - ``` git clone -b uchat-after-presentation git@github.com:hicooolasss/ucode-connect-uchat.git ```.

- After you have done that go to the project folder and type : ```make all```.

-  To run the server, type ```./uchat_server PORT``` where ```PORT``` is some number starting from *1000*, for example - *1234*.

After that, the terminal will display the PID of our server (this is in case you want to find the server in the task manager).


- In order to connect to the server from the client, you need to open a new terminal in the root folder of the repository and write - ```./uchat 127.0.0.1 PORT```, **note - the PORT for the client must be the same as for the server, otherwise the client will not connect** ,after that the client will automatically connect to the server and you can enjoy using our program.

### For MacOS


***IMPORTANT*** - to compile **TokyoNight** chat you need to install : *brew.*

- Firstly you need to clone our project with this command - ``` git clone -b uchat-after-presentation git@github.com:hicooolasss/ucode-connect-uchat.git ```.

- After you have done that go to the project folder and type : ```make all```.

- To run the server, type ```./uchat_server PORT``` where ```PORT``` is some number starting from *1000*, for example - *1234*.

After that, the terminal will display the PID of our server (this is in case you want to find the server in the task manager).

- In order to connect to the server from the client, you need to open a new terminal in the root folder of the repository and write - ```./uchat 127.0.0.1 PORT```, **note - the PORT for the client must be the same as for the server, otherwise the client will not connect** ,after that the client will automatically connect to the server and you can enjoy using our program.
