
#include <iostream>
#include <string>
#include <vector>
#include <Net_Core.h>
#include <Net_Server.h>
#include <mysql.h>

#pragma comment(lib, "libmysql.lib")

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif


class MyServer : public net::Server_interface {
public:
	MyServer(uint16_t nPort) 
		: net::Server_interface(nPort) {
		DBconn = mysql_init(NULL);
		if (mysql_real_connect(DBconn, "localhost", "root", "mysql365!!",
			"lumberjack", 0, NULL, 0) != NULL) {
			std::cout << "[DB]: Connected." << std::endl;
		}
	}
	~MyServer() {}

protected:
	virtual bool onClientConnect(std::shared_ptr<net::Connection<MessageType>> client) {
		return true;
	}

	virtual void onClientDisconnect(std::shared_ptr<net::Connection<MessageType>> client) {
		std::cout << "Removing client [" << client->getID() << "]\n";
	}

	int retrieveItemData(const char* out, std::string& uid) {
		std::cout << "\n";

		int size{};
		const char query[100]{};
		std::vector<size_t> poses;
		std::string raw = "SELECT * FROM inventory_[U_ID] WHERE inventory_[U_ID].uid = [U_ID]";
		
		size_t pos = raw.find("[U_ID]");
		while (pos != std::string::npos) {
			poses.emplace_back(pos);
			raw.replace(pos, 6, uid);
			pos = raw.find("[U_ID]", pos + 6);
		}

		std::memcpy((char*)query, raw.c_str(), raw.length());
		
		if (mysql_query(DBconn, query)) {
			std::cout << "[QueryError]: query" << std::endl;
			return -1;
		}

		MYSQL_RES* result = mysql_store_result(DBconn);
		MYSQL_ROW row;
		int num_fields = mysql_num_fields(result);
		
		std::string val(3,'0');
		val.reserve(80);
		
		int cnt{};
		while ((row = mysql_fetch_row(result))) {
			size++;
			for (int i = 0; i < num_fields - 1; i++) {
				const char* ele = row[i] ? row[i] : "NULL";
				cnt = std::strlen(ele);

				for (int j = 0; j < 4 - cnt; j++) {
					val.push_back('0');
				}

				for (int j = 0; j < std::strlen(ele); j++) {
					val.push_back(ele[j]);
				}
			}
		}
		std::string num = std::to_string(size);
		val.replace(3 - num.length(), num.length(), num);

		std::memcpy((char*)out, val.c_str(), val.length());
		std::cout << "\n";

		return size;
	}

	bool isInventoryTableExist(const std::string& Player_uid) {
		const char* query[100]{};
		std::string raw = "SHOW TABLES LIKE 'T_NAME'";
		
		auto iter = raw.find("T_NAME");
		raw.replace(iter, 6, Player_uid);
		std::memcpy(query, raw.c_str(), raw.length());

		if (mysql_query(DBconn, (char*)query)) {
			std::cout << "Error: query.. in isItemTableExist()\n";
			return false;
		}

		MYSQL_ROW row;
		MYSQL_RES* result = mysql_store_result(DBconn);
		if (result == NULL) {
			std::cout << "Error: result.. in isItemTableExist()\n";
			return false;
		}

		if (row = mysql_fetch_row(result))
			return true;
		return false;
	}

	bool createInventoryTable(const std::string& Player_uid) {
		const char* query[100]{};
		std::string raw = "CREATE TABLE Inventory_[uid]"
			"(item_code int PRIMARY KEY,"
			"count int,"
			"uid int)";

		auto iter = raw.find("[uid]");
		raw.replace(iter, 5, Player_uid);
		std::memcpy(query, raw.c_str(), raw.length());

		if (mysql_query(DBconn, (char*)query)) {
			std::cout<<"Error: query.. in createInventotyTable()\n";
			return false;
		}

		return true;
	}

	bool addItemToDB(std::string& m_data) {
		std::cout << "\n";
		if (m_data.length() != 13) return false;

		const char query[100]{};
		std::string raw = "UPDATE stacked_belonging SET count = count + 1 "
							"WHERE uid= AND item_code=";
		const char* target = "uid=";
		// 66, 81
		std::string t_uid = m_data.substr(0, 4);
		std::string t_code = m_data.substr(5, 4);
		std::string t_count = m_data.substr(9, 4);

		const char* m_uid = t_uid.c_str();
		const char* m_code = t_code.c_str();
		const char* m_count = t_count.c_str();

		char c_uid[10]{'\''};
		char c_code[10]{'\''};
		char c_count[10]{};
		std::memcpy((char*)c_uid + 1, m_uid, std::strlen(m_uid));
		std::memcpy((char*)c_code + 1, m_code, std::strlen(m_code));
		std::memcpy((char*)c_count, m_count, std::strlen(m_count));
		c_uid[std::strlen(c_uid)] = '\'\0';
		c_code[std::strlen(c_code)] = '\'\0';
		c_count[std::strlen(c_count)] = '\0';


		auto iter = raw.find(" AND");
		raw.insert(iter, c_uid);
		raw.insert(raw.length(), c_code);
		iter = raw.find("1");
		raw.replace(iter, 1, c_count);

		std::memcpy((char*)query, raw.data(), raw.length());
		std::cout <<"query: "<< query << std::endl;
		
		if (isInventoryTableExist(t_uid)) {
			if (mysql_query(DBconn, query)) {
				std::cout << "ERROR: query" << "\n";
				return false;
			}
			std::cout << "Query success!\n\n";
		}
		else {

		}

		return true;
	}

	bool itemTransaction(std::shared_ptr<net::Connection<MessageType>>& client, net::Message<MessageType>& msg) {
		std::string m_data(msg.Body.begin(), msg.Body.end());
		
		std::string m_uid(m_data.substr(0, 4));
		int _op = stoi(m_data.substr(4, 1));

		std::cout << "[SERVER]: Item transaction request from.... " << m_uid << std::endl;
		std::cout << "Original m_data: " << m_data << std::endl;

		switch (_op) {
			case ItemOperation::ITEMOUT: 
			{
				break;
			}
			case ItemOperation::ITEMIN:
			{
				std::cout << "Process type: ITEM_IN\n";
				std::cout << "item_code: " << m_data.substr(5, 4) << std::endl;
				std::cout << "item_count: " << m_data.substr(9, 4) << std::endl;
				
				if (addItemToDB(m_data)) {
					std::cout << "Item transaction success!\n";
				}

				break;
			}
			case ItemOperation::ITEMSYNC:
			{
				std::cout << "Process type: ITEM_SYNC" << std::endl;
				net::Message<MessageType> msg;
				msg.Header.id = MessageType::ITEM;

				const char reply[80]{};
				retrieveItemData(reply, m_uid);

				std::cout << "[SERVER] item data :";
				std::cout << reply << std::endl;

				msg.gets(reply, std::strlen(reply));
				client->send(msg);

				break;
			}
			default:
			{
				std::cout << "Unvalid operation: " << _op << std::endl;
				break;
			}
		}



		return false;
	}

	bool login(net::Message<MessageType>& msg) {
		std::cout << "[SERVER]: Player Login..  uid=";
		std::string m_uid(msg.Body.begin(), msg.Body.end());
		std::cout << m_uid << "\n";
		
		// 1. User table.
		char* out = nullptr;
		const char query[100]{};
		std::string raw = "SELECT EXISTS (SELECT * FROM user WHERE uid=[U_ID]) as SUCCESS";
		auto iter = raw.find("[U_ID]");
		raw.replace(iter, 6, m_uid);
		std::memcpy((char*)query, raw.c_str(), raw.length());

		if (mysql_query(DBconn, query)) {
			std::cout << "Error: query.. login()\n";
			return false;
		}

		MYSQL_RES* result = mysql_store_result(DBconn);
		MYSQL_ROW row;

		if (row = mysql_fetch_row(result)) {
			out = row[0];
		}

		if (out!=nullptr && out[0] - '0' == 1) {
			return true;
		}

		return false;
	}

	bool update_login_history(net::Message<MessageType>& msg) {
		std::cout << "[SERVER]: Update Login history..  uid=";
		std::string m_uid(msg.Body.begin(), msg.Body.end());
		std::cout << m_uid << "\n";

		char* out = nullptr;
		const char query[100]{};
		std::string raw = "SELECT EXISTS (SELECT * FROM login_history WHERE uid=[U_ID]) as SUCCESS";
		auto iter = raw.find("[U_ID]");
		raw.replace(iter, 6, m_uid);
		std:memcpy((char*)query, raw.c_str(), raw.length());

		// Check query.
		if (mysql_query(DBconn, query)) {
			std::cout << "Error: query.. update_login_history()\n";
			return false;
		}

		MYSQL_RES* result = mysql_store_result(DBconn);
		MYSQL_ROW row;

		if (row = mysql_fetch_row(result)) {
			out = row[0];
		}

		if (out != nullptr && out[0] - '0' == 0) {
			return false;
		}

		raw = "UPDATE login_history SET login_time = NOW() "
			"WHERE login_history.uid=[U_ID]";

		iter = raw.find("[U_ID]");
		raw.replace(iter, 6, m_uid);
		std::memset((char*)query, 0, 100);
		std::memcpy((char*)query, raw.c_str(), raw.length());

		// Update query.
		if (mysql_query(DBconn, query)) {
			std::cout << "Error: query.. update_login_history()\n";
			return false;
		}

		return true;
	}

	virtual void onMessage(std::shared_ptr<net::Connection<MessageType>> client, net::Message<MessageType>& msg) {
		switch (msg.Header.id) {
		case MessageType::ACK: {
			std::cout << "[SERVER]: ACK\n";

			break;
		}
		
		case MessageType::LOGIN: {
			std::cout << "\n";
			
			const char reply[1]{'1'};

			if (!login(msg)) {
				std::cout << "[SERVER] inValid user..\n";
				std::memcpy((char*)reply, "0", 1);
			}
			if (!update_login_history(msg)) {
				std::cout << "[SERVER] failed to update login history..\n";
				std::memcpy((char*)reply, "0", 1);
			}

			net::Message<MessageType> msg;
			msg.gets(reply, 1);
			client->send(msg);

			std::cout << "\n";
			break;
		}

		case MessageType::ITEM: {
			if (itemTransaction(client, msg)) {}
		}
		}
	}

	MYSQL* DBconn = NULL;
};


int main() {
	MyServer server(999);
	server.start();

	while (1) {
		server.update(-1, true);
	}


	return 0;

}

