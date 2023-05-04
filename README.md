# TokyoNight chat 

TokyoNight chat was developed in about four months thanks to the diligence of a team of two highly skilled developers. In the process of creating this project, such advanced technologies and tools as SQLite3, cJSON, OpenSSL and GTK4 were used, which ensured its high quality and functionality.

After TokyoNight chat was presented as a presentation to an audience of students, this project received the highest marks and well-deserved recognition for its excellent features and technical implementation. This proves the skill of the development team and their ability to create products that meet the highest requirements.

## Developers

- [@iserikov](https://github.com/hicoolasss) - client-part developer.
- [@kyehorov](https://github.com/KyryloYehorov) - server-part developer.


## Demo

Here you can find our demo - https://www.youtube.com/watch?v=fcCUp_We3Ng&ab_channel=criops


- ### Login
![Aerghuierogherioguer](https://i.imgur.com/LLPMT4B.png)

- ### Sign up
![Aerghuierogherioguer](https://i.imgur.com/kdtNhNB.png)

- ### Profile 

In this tab, user can change his avatar.
**note - Name and BIO although you can see them on the screen, these fields do not have any functionality other than visual (for now)!**

![Aerghuierogherioguer](https://i.imgur.com/XNoMIZB.png)

- ### Home 

In this tab, the user has access to the functionality of artificial intelligence based on the chat bot ***chat gpt-3***.

![Aerghuierogherioguer](https://i.imgur.com/Gw5GqeJ.png)

- ### Chats

In this tab, the user can choose to chat with an existing friend or add a new friend.

**note - entry field for search do not have any functionality other than visual (for now)!**
![Aerghuierogherioguer](https://i.imgur.com/zwwD0Ge.png)

Here you can see a fragment of correspondence with a friend, the user can delete and edit his messages.
![Aerghuierogherioguer](https://i.imgur.com/GH10Gwm.png)

- ### Settings

In the settings tab, the user can change the theme of the chat.

**note - In the future, the development team plans to add the functionality of creating custom themes and editing existing themes**
![Aerghuierogherioguer](https://i.imgur.com/1oKnaB1.png)

- ### Achievements

In the achievements tab, the user can see the achievements that he can complete and those achievements that he has already completed.
![Aerghuierogherioguer](https://i.imgur.com/e4B4EPW.png)


- ### Logout

Here user can log out to switch his account.

![Aerghuierogherioguer](https://i.imgur.com/z0UfT0M.png)

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
