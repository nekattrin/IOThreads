//реализовать программу для работы с
//данными, используя потоки ввода - вывода.Реализовать функций просмотра,
//добавления, удаления, редактирования, сортировки, поиска данных по
//необходимым параметрам.Использовать минимум 5 флагов
//форматирования.Сформировать табличный вывод всех данных.
//Использовать функции setf() и unsetf() для установки и сбрасывания флагов.
//Использовать функции eof(), fail(), good(), bad(), clear() для провеки
//состояния потока.Реализовать в программе возможность работы с данными,
//используя потоки файлового ввода - вывода.Реализовать функций
//1. добавления и удаления данных в / из файл(а),
//2. редактирования и просмотра данных файла,
//3. поиска данных по необходимым параметрам в файле.
//Использовать функции open(), is_open(), bad(), good(), close(), eof(),
//fail() для анализа состояния файла, перегрузить операторы вывода в поток <<
//и считывания из потока >> , использовать функции seekg(), seekp(), tellg()

#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;
const int MAX_ORDERS = 10;

class Item {
protected:
	string name;
	int price;
public:
	Item(string name = "X", int price = 0) { this->name = name; this->price = price; }

	friend class Shop;
	friend class Order;

};
static Item X;


class Shop {
	string ShopName;
public:
	Shop(string name) {
		this->ShopName = name;
	};
	void showData() { cout << setw(20) << this->ShopName; };
	Item itemList[5];
	void showItem(int i) { cout << i + 1 << "." << itemList[i].name << setw(16 - size(itemList[i].name)) << itemList[i].price << " deneg" << endl; }
};



class Order : public Item {
	int number;
public:
	void makeOrder(Item, int);
	friend class Shop;
	friend class Item;
	Order(Item item = X, int number = 0) { this->name = item.name; this->price = item.price;  this->number = number + 1; }
	void showOrder();
};

void Order::makeOrder(Item item, int num) {
	this->name = item.name;
	this->price = item.price;
	this->number = num;
}

void Order::showOrder()
{
	cout << this->number << " - " << this->name << setw(16 - size(this->name)) << this->price;
}



class Busket {
	int numOrders = 0;
public:
	Order* OrderList[];
	friend Order;
	void addOrder(Item item) {
		OrderList[numOrders] = new Order;
		OrderList[numOrders++]->makeOrder(item, numOrders);
	}
	void showBusket();
	void write();
	void read();
	void search();
	void change(int, Item);
	~Busket() {
		for (int i = 0; i < numOrders; i++) {
			delete OrderList[i];
		}
		delete[] OrderList;
		cout << setw(20) << "Busket died\n";
	}
};

void Busket::showBusket() {
	for (int i = 0; i < numOrders; i++) {
		OrderList[i]->showOrder();
		cout << endl;
	}
}

void Busket::write() {
	ofstream file;
	file.open("busket.txt", ios::trunc | ios::binary);
	if (!file) { cout << "file can't be opened(" << endl; return; }
	int size = sizeof(Order);
	for (int i = 0; i < numOrders; i++) {

		file.write((char*)(OrderList[i]), size);
		if (!file) { cout << "data can't be written into the file(" << endl;		return; }
	}


}

void Busket::read() {
	cout << "reading file" << endl;
	int size = sizeof(Order);
	ifstream file;
	file.open("busket.txt", ios::binary);
	if (!file) { cout << "file can't be opened:(" << endl; return; }
	numOrders = 0;
	while (true) {

		file.read((char*)OrderList[numOrders], size);

		if (file.eof()) break;
		if (!file) { cout << "data can't be read from the file:(" << endl;		return; }
		numOrders++;
	}

}

void Busket::search() {

	cout << "Enter order number: " << endl;
	int num;
	cin >> num;
	ifstream file;
	int size = sizeof(Order);
	file.open("busket.txt", ios::binary);
	int position = (num - 1) * size;
	file.seekg(position);
	Order order;
	file.read(reinterpret_cast<char*>(&order), size);
	order.showOrder();

}

void Busket::change(int num, Item item) {
	this->read();
	ofstream file;
	int size = sizeof(Order);
	Order order;
	order.makeOrder(item, num);
	file.open("busket.txt", ios::binary);
	for (int i = 0; i < numOrders; i++)
	{
		if (i == num - 1) {
			file.write((char*)&order, size);
		}
		else {
			file.write((char*)OrderList[i], size);
		}

	}


}


int main() {

	cout << "You entered the electronics store 'Lemon'" << endl << endl;
	Shop shop("Lemon");


	Item item1("Lphone", 1000);
	Item item2("LemonWatch", 150);
	Item item3("LPad", 900);
	Item item4("MagBook", 1200);
	Item item5("WonderPods", 200);

	shop.itemList[0] = item1;
	shop.itemList[1] = item2;
	shop.itemList[2] = item3;
	shop.itemList[3] = item4;
	shop.itemList[4] = item5;


	/*Item itemList[5];
	itemList[0] = item1;
	itemList[1] = item2;
	itemList[2] = item3;
	itemList[3] = item4;
	itemList[4] = item5;*/




	for (int i = 0; i < 5; i++)
		shop.showItem(i);

	Busket busket;

	do {
		cin.clear();
		cout.setf(ios::left);
		cout << "\na - to add new item\n"
			"w - Write all data to a file\n"
			"r - read all data from file\n"
			"n - show data by order number\n"
			"c - change order\n"
			"x - exit\n";
		cout.unsetf(ios::left);
		char ch;
		cin >> ch;
		if (!cin.good()) { cout << "incorrect type entered\n"; break; }

		switch (ch) {
		case 'a':
			cout << "Enter item number: ";
			int item;
			cin >> item;
			if (!cin.good()) { cout << "incorrect type entered\n"; break; }
			busket.addOrder(shop.itemList[item - 1]);
			busket.showBusket();
			break;


		case 'w':
			busket.write();
			break;

		case'r':
			busket.read();
			busket.showBusket();
			break;

		case'n':
			busket.search();
			break;

		case 'c':
			cout << "Enter order number you wonna change: " << endl;
			int num;
			cin >> num;
			cout << "Enter item" << endl;
			int itemnum;
			cin >> itemnum;
			busket.change(num, shop.itemList[itemnum - 1]);
			break;

		case 'x': exit(0);
		default: cout << "\nНеизвестная команда";
		}



	} while (true);

}