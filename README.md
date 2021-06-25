# Simple server && client for unreal engine.

Super simple `async-single thread` server and client using  `asio`  one of boost library module. This server support transaction with `mySQL` database due to `mySQL C connector`. 



### 1. Overvall process structure

![img](https://user-images.githubusercontent.com/30077320/123431189-f6469980-d603-11eb-942a-a5c7751e7cee.png)

### 2. Packet(message) structure

![packet](https://user-images.githubusercontent.com/30077320/123431183-f5ae0300-d603-11eb-91a0-d078ad1d3fcf.png)

*Header* only can be replaced by limit values below. 

```
1. ACK    : Test usage.
2. LOGIN  : Basically, first packet when a client access the server.
3. LOGOUT : Sent from 'EndPlay()' function of cilent object.
4. ITEM   : For transcation with mySQL.
```

In addition, *payload* should include the required data which header you have chosen.

### 3. Log in/out

![login out](C:\Users\나\Desktop\취업포트폴리오\lumberJack\login out.png)

All clients have a unique `UID` that is composed of integers and it use for all operations.

### 4. Item transaction

Notice that required `UID` front of *payload* which it allows accessing private user item table.

![ItemTrans](C:\Users\나\Desktop\취업포트폴리오\lumberJack\ItemTrans.png)

```
1. ITEMOUT  : Subtract item count value in the database.
2. ITMEIN   : Add item count value in the database.
3. ITEMSYNC : Make the same data between client item data and database data.
```
