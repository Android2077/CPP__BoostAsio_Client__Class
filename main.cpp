#include <iostream>
#include <string>
#include <vector>

#include "BoostAsio__TCP_sender.h" 




int main()
{



   //**********************************************************************************2-BoostAsio__TCP_sender:Начало*******************************************************************************************

    //Класс "отправки сообщений по TCP сокету":


    //Конструктор класса принимает два параметра: 
    //-main_thread   - значит io_context.run() будет вызван в данном потоке в котором создается обьект данного класса. Все ассинхронные колбеки будут вызыватся именно в этом данном потоке.
    //-multi_threads - значит io_context.run() будет вызван в созданном пуле потоков по кол-во равному кол-ву физических потоков в системе. Все ассинхронные колбеки будут вызыватся именно в этом созданном пуле потков.
    //И лямбду колбек, которая будет вызыватся в случае ошибок.


	std::function<void(BoostAsio__TCP_sender* BoostAsio__TCP_sender_p, BoostAsio__TCP_sender::Sockets__class::socket_struct* socket_struct_p, const BoostAsio__TCP_sender::error_enum error_enum_, const boost::system::error_code& boost_error_code, const std::string& error_string)>lamda_error_TCPSender = [](BoostAsio__TCP_sender* BoostAsio__TCP_sender_p, BoostAsio__TCP_sender::Sockets__class::socket_struct* socket_struct_p, const BoostAsio__TCP_sender::error_enum error_enum_, const boost::system::error_code& boost_error_code, const std::string error_string)
		{

			//BoostAsio__TCP_sender_p - указатель на обьект данного класса.
			//socket_struct_p         - указатель на обьект структры сокета.
			//error_enum_             - общий тип ошибки.
			//boost_error_code        - код ошибки boost
			//error_string            - текстовое наименование ошибки

			std::cout << "----------------------Error Callback: start-------------------------" << std::endl;

			std::cout << "Socket_Name Error:" << socket_struct_p->Socket_name <<std::endl;      //Имя сокета, на котором возникла ошибка.

			if (error_enum_ == BoostAsio__TCP_sender::error_enum::boost_error)
			{
				std::cout << "That boost error:" << BoostAsio__TCP_sender_p->get__BoostErrorText(boost_error_code.value()) << ":" << boost_error_code.message() << std::endl;
			}
			else
			{
				if (error_enum_ == BoostAsio__TCP_sender::error_enum::bad_alloc)
				{
					std::cout << "That bad_alloc" << std::endl;
				}
			}

			std::cout << "error_string:" << error_string << std::endl;

			std::cout << "----------------------Error Callback: end-------------------------" << std::endl;
		};


    BoostAsio__TCP_sender BoostAsio__TCP_sender_(BoostAsio__TCP_sender::ThreadType_enum::main_thread, lamda_error_TCPSender);        //Создадим обьект класса.

    //Доступные методы:
    //- add__NewSocket                     - добавляет новый Сокет.
    //- get__Socket_by_Name                - возвращает Сокет по его имени указанном при вызове "add__NewSocket"
    //- Connect_to_IP_adress               - Устанавливает Соединение на Сокете по IP-адрессу.
    //- Connect_to_Web_adress              - Устанавливает Соединение наСокете по Web-адрессу.
    //- send_to_socket                     - Отправляет данные в Сокет.
    //- set__CallbackMode_for_SocketWrite  - Устанавливает режим вызова колбека для метода "set__Callback_for_SocketWrite": будет ли "set__Callback_for_SocketWrite" вызыватся для каждой части отправленных данных в Сокет или будет вызыватся только когда все данные отправлемые данные помещены в Сокет. 
    //- set__Read                          - Устанавливает режим Чтения на Сокете.
    //- set__InternalBuffer_for_Read       - устанвливает опцию использования Внутренего буффера класса для Чтения данных их Сокета.
    //- set__UserBuffer_for_Read           - устанвливает опцию использования Пользовательского буффера для Чтения данных их Сокета.
    //- set__ReadUntil                     - устанавливает режим Чтения "ReadUntil".
    //- set__Callback_Error                - Устанавливает общий колбек для всех Сокетов, который будет вызыватся при Ошибках. 
    //- set__Callback_for_Connect          - Устанавливает общий колбек для всех Сокетов, который будет вызыватся при изменеии статуса Соединения на Сокете.
    //- set__Callback_for_SocketWrite      - Устанавливает общий колбек для всех Сокетов, который будет вызыватся при Записи данных в Сокет.
    //- set__Callback_for_SocketRead       - Устанавливает общий колбек для всех Сокетов, который будет вызыватся при Чтении данных их Сокета.
    //- redefine_socket                    - Переопределяет сущетвующий Сокет.
    //- close_AllSocket                    - закрывает все Сокеты.
    //- delete_AllSocket                   - удаляет все Сокеты.
    //- open_Previously_ClosedSocket       - открывает ранее закрытый Сокет.
    //- close_Socket                       - закрывает Сокет.
    //- delete_Socket                      - удаляет Сокет.
    //- stop                               - останавливает работу класса удаляя и закрывая все атикные Сокеты и Соединения.
    //- wait_here                          - ждем окончания работы класса в этой точке.



	//------------------------------------------------------------------------------------add__NewSocket:Начало-------------------------------------------------------------------------------
	
	//Данный метод создает и добавляет новый сокет.
	//Данный метод потоко-безопасен.


	//-------------------------------------
	std::string Socket_name_1 = "Socket_Name_1";                          //Идентификатор сокета. Если не нужно, то можно передать пустую строку! НУЖЕН ЛИШЬ ДЛЯ УДОБНОГО ПОЛУЧЕНИЯ УКАЗАТЕЛЯ НА СТРУКТУРУ СОКЕТА "socket_struct*" - ПО ЕГО ИМЕНИ, ЕСЛИ ЭТО МОЖЕТ ПОНАДОБИТСЯ.  Но, если имя Сокета все таки указаывалось, то оно НЕ ДОЛЖНО ПОВТОРЯЮТСЯ С ДРУГИМИ ДОБАВЛЕННЫМИ ИМЕНАМИ СОКЕТОВ.

	boost::asio::ip::tcp IP_type_1 = boost::asio::ip::tcp::v4();         //Тип протокола TCP: IPv4 или IPv6.
	//-------------------------------------

	BoostAsio__TCP_sender::Sockets__class::socket_struct* added_socket_1 = BoostAsio__TCP_sender_.get__SocketClass_pointer()->add__NewSocket(Socket_name_1, IP_type_1);       //Возвращает добленный сокет ИЛИ 0 в случе неудачи. Ошибка также придет в колбек.


	if (added_socket_1 == 0)
	{
		std::cout << "Socket NOT ADDED" << std::endl;

		return -1;
	}

	//------------------------------------------------------------------------------------add__NewSocket:Конец-------------------------------------------------------------------------------




	//------------------------------------------------------------------------------------set__Callback_Error:Начало-------------------------------------------------------------------------------
	
	//Данные метод устанавилвает колбек ошибок.
	//Данный метод потоко-безопасен.

	/*
	//----------------------------------------------------
	std::function<void(BoostAsio__TCP_sender* BoostAsio__TCP_sender_p, BoostAsio__TCP_sender::Sockets__class::socket_struct* socket_struct_p, const BoostAsio__TCP_sender::error_enum error_enum_, const boost::system::error_code& boost_error_code, const std::string& error_string)> user_lambda_14 = [](BoostAsio__TCP_sender* BoostAsio__TCP_sender_p, BoostAsio__TCP_sender::Sockets__class::socket_struct* socket_struct_p, const BoostAsio__TCP_sender::error_enum error_enum_, const boost::system::error_code& boost_error_code, const std::string& error_string)
		{
			//Данная лямбда будет вызыватся для всех сокетов из списка добавленных сокетов - на которых произошло данное событие.

			//BoostAsio__TCP_sender_p - указатель на обьект данного класса.
			//socket_struct_p         - указатель на обьект структры сокета.
			//error_enum_             - общий тип ошибки.
			//boost_error_code        - код ошибки boost
			//error_string            - текстовое наименование ошибки

			std::cout << "----------------------Error Callback: start-------------------------" << std::endl;

			std::cout << "Socket_Name Error:" << socket_struct_p->Socket_name <<std::endl;      //Имя сокета, на котором возникла ошибка.

			if (error_enum_ == BoostAsio__TCP_sender::error_enum::boost_error)
			{
				std::cout << "That boost error:" << BoostAsio__TCP_sender_p->get__BoostErrorText(boost_error_code.value()) << ":" << boost_error_code.message() << std::endl;
			}
			else
			{
				if (error_enum_ == BoostAsio__TCP_sender::error_enum::bad_alloc)
				{
					std::cout << "That bad_alloc" << std::endl;
				}
			}

			std::cout << "error_string:" << error_string << std::endl;

			std::cout << "----------------------Error Callback: end-------------------------" << std::endl;


		};
	//----------------------------------------------------


	BoostAsio__TCP_sender_.set__Callback_Error(user_lambda_14);
	*/
	//------------------------------------------------------------------------------------set__Callback_Error:Конец-------------------------------------------------------------------------------




	//------------------------------------------------------------------------------------set__Callback_for_Connect:Начало-------------------------------------------------------------------------------

	//Данные метод устанавливает колбек Установки Соединения или Разрыва Соединения, заменяя предыдущий установленный.
	//Данный метод потоко-безопасен.
	//Данный метод не является обязательным для установки.


	//----------------------------------------------------
	std::function<void(BoostAsio__TCP_sender* BoostAsio__TCP_sender_p, BoostAsio__TCP_sender::Sockets__class::socket_struct* socket_struct_p, const BoostAsio__TCP_sender::Connect_flag Connect_flag_, const boost::system::error_code& boost_error)> user_lambda_15 = [](BoostAsio__TCP_sender* BoostAsio__TCP_sender_p, BoostAsio__TCP_sender::Sockets__class::socket_struct* socket_struct_p, const BoostAsio__TCP_sender::Connect_flag Connect_flag_, const boost::system::error_code& boost_error)
		{
			//Данная лямбда будет вызыватся для всех сокетов из списка добавленных сокетов - на которых произошло данное событие.

			//BoostAsio__TCP_sender_p - указатель на обьект данного класса.
			//socket_struct_p         - указатель на обьект структры сокета.
			//Connect_flag_           - флаг соединения.


			std::cout << "------------------------------------Connect callback: start----------------------------------------------" << std::endl;

			if (Connect_flag_ == BoostAsio__TCP_sender::Connect_flag::Connect)
			{
				std::cout << socket_struct_p->Socket_name << ": Connect:" << socket_struct_p->socket_.remote_endpoint().address().to_string() << ":" << socket_struct_p->socket_.remote_endpoint().port() << std::endl;  //Выводим IP с которым был установлено соеднение.
			}

			if (Connect_flag_ == BoostAsio__TCP_sender::Connect_flag::Disconnect)
			{
				std::cout << socket_struct_p->Socket_name << ": Disconnect:" << BoostAsio__TCP_sender_p->get__BoostErrorText(boost_error.value()) << std::endl;
			}

			std::cout << "------------------------------------Connect callback: end------------------------------------------------" << std::endl;


		};
	//----------------------------------------------------


	BoostAsio__TCP_sender_.set__Callback_for_Connect(user_lambda_15);


	//------------------------------------------------------------------------------------set__Callback_for_Connect:Конец-------------------------------------------------------------------------------


	//------------------------------------------------------------------------------------set__Callback_for_SocketWrite:Начало-------------------------------------------------------------------------------

	//Данные метод устанавливает колбек информарования о том, сколько байт записано в сетевой драйвер для отправи сообщения, заменяя предыдущий установленный.
    //Данный метод потоко-безопасен.
	//Данный метод не является обязательным для установки.


	//----------------------------------------------------
	std::function<void(BoostAsio__TCP_sender* BoostAsio__TCP_sender__class_p, BoostAsio__TCP_sender::Sockets__class::socket_struct* socket_struct_p, const BoostAsio__TCP_sender::CallbackMessage CallbackMessage_, const size_t bytes_send_transferred, const size_t bytes_send_transferred_TOTAL)> user_lambda_16 = [](BoostAsio__TCP_sender* BoostAsio__TCP_sender__class_p, BoostAsio__TCP_sender::Sockets__class::socket_struct* socket_struct_p, const BoostAsio__TCP_sender::CallbackMessage CallbackMessage_, const size_t bytes_send_transferred, const size_t bytes_send_transferred_TOTAL)
		{
			//Данная лямбда будет вызыватся для всех сокетов из списка добавленных сокетов - на которых произошло данное событие.

			//BoostAsio__TCP_sender_p         - указатель на обьект данного класса.
			//socket_struct_p                 - указатель на обьект структры сокета.
			//CallbackMessage_                - флаг: Part - значит это очередная часть передающегося сообщения, Full - значит это полсденяя часть переданного сообщения и сообщение считается полностью переданным.
			//bytes_send_transferred          - размер этой переданной части.
			//bytes_send_transferred_TOTAL    - размер суммарно переданных байт.


			std::cout << "------------------------------------Send callback: start----------------------------------------------" << std::endl;

			if (CallbackMessage_ == BoostAsio__TCP_sender::CallbackMessage::Part)
			{
				std::cout << socket_struct_p->Socket_name << ":" << bytes_send_transferred << " of " << bytes_send_transferred_TOTAL << ":Part" << std::endl;
			}

			if (CallbackMessage_ == BoostAsio__TCP_sender::CallbackMessage::Full)
			{
				std::cout << socket_struct_p->Socket_name << ":" << bytes_send_transferred << " of " << bytes_send_transferred_TOTAL << ":Full" << std::endl;
			}

			std::cout << "------------------------------------Send callback: end------------------------------------------------" << std::endl;


		};
	//----------------------------------------------------


	BoostAsio__TCP_sender_.set__Callback_for_SocketWrite(user_lambda_16);


	//------------------------------------------------------------------------------------set__Callback_for_SocketWrite:Конец-------------------------------------------------------------------------------


	//------------------------------------------------------------------------------------set__Callback_for_SocketRead:Начало-------------------------------------------------------------------------------

	//Данные метод добавляет колбек для приема данных на сокете, то есть он будет вызыватся каждый раз, как приходит какое то сообщение со стороны удаленного IP адресса, заменяя предыдущий установленный.
	//Данный метод потоко-безопасен.
	//Данный метод не является обязательным для установки.


	//----------------------------------------------------
	std::function<void(BoostAsio__TCP_sender* BoostAsio__TCP_sender__class_p, BoostAsio__TCP_sender::Sockets__class::socket_struct* socket_struct_p, const char* pointer_to_DataReceived, const size_t bytes_received)> user_lambda_17 = [](BoostAsio__TCP_sender* BoostAsio__TCP_sender__class_p, BoostAsio__TCP_sender::Sockets__class::socket_struct* socket_struct_p, const char* pointer_to_DataReceived, const size_t bytes_received)
		{
			//Данная лямбда будет вызыватся для всех сокетов из списка добавленных сокетов - на которых произошло данное событие.

			//BoostAsio__TCP_sender_p         - указатель на обьект данного класса.
			//socket_struct_p                 - указатель на обьект структры сокета.
			//pointer_to_DataReceived         - указатель на начало данных - куда записаны принятые данные. Данные по указателю актуальны ТОЛЬКО в момент вызова данного колбека.
			//bytes_received                  - размер данных по указателю, который доступен для считывания.



			std::cout << "------------------------------------Read callback: start----------------------------------------------" << std::endl;

			std::string temp_string;

			temp_string.append(pointer_to_DataReceived, bytes_received);

			std::cout << socket_struct_p->Socket_name << ": Recieve data: " << bytes_received << ":" << temp_string << std::endl;

			std::cout << "------------------------------------Read callback: end------------------------------------------------" << std::endl;


		};
	//----------------------------------------------------


	BoostAsio__TCP_sender_.set__Callback_for_SocketRead(user_lambda_17);

	//------------------------------------------------------------------------------------set__Callback_for_SocketRead:Конец-------------------------------------------------------------------------------






	//-------------------------------------------------------------------------------------get__Socket_by_Name:Начало--------------------------------------------------------------------------------------

	//Возвращает указатель на структуру сокета по имени, которое указывалось при добавлении Сокета "add__NewSocket".

	std::string Socket_name_get = Socket_name_1;

	BoostAsio__TCP_sender::Sockets__class::socket_struct* Socket_p_3_get = BoostAsio__TCP_sender_.get__Socket_by_Name(Socket_name_get);

	//-------------------------------------------------------------------------------------get__Socket_by_Name:Конец--------------------------------------------------------------------------------------





	//------------------------------------------------------------------------------------Connect_to_IP_adress:Начало-------------------------------------------------------------------------------
	
	//Данный метод устанавливает соединение с удаленным IP адрессом на ранее созданном сокете.  Сокет может быть закрыт - функция внутри его откроет заново.
	//Метод Не должен вызыватся для Открытого Сокета!
	//Данный метод потоко-безопасен.


	//--------------------------------------------------
	BoostAsio__TCP_sender::Sockets__class::socket_struct* Socket_p_3 = added_socket_1;                //Указатель на ранее добавленный сокет.

	const char* pointer_to_IP_3 = "127.0.0.1";                                        //Указатель на строку с IP адрессом, к которому нужно подключится.

	int Port_num_3 = 1235;                                                          //Номер порта к которому нужно подключится.

	std::function<void(BoostAsio__TCP_sender* BoostAsio__TCP_sender_p, BoostAsio__TCP_sender::Sockets__class::socket_struct* socket_struct_p, const BoostAsio__TCP_sender::Connect_flag Connect_flag_, const boost::system::error_code& boost_error)> user_lambda_connect = [](BoostAsio__TCP_sender* BoostAsio__TCP_sender_p, BoostAsio__TCP_sender::Sockets__class::socket_struct* socket_struct_p, const BoostAsio__TCP_sender::Connect_flag Connect_flag_, const boost::system::error_code& boost_error)
		{
			//Дання лямбда будет вызыватся только для конкретного Сокета - указанного при вызове "Connect_to_IP_adress".

			//BoostAsio__TCP_sender_p - указатель на обьект данного класса.
			//socket_struct_p         - указатель на обьект структры сокета.
			//Connect_flag_           - флаг соединения.


			std::cout << "------------------------------------Connect callback: start----------------------------------------------" << std::endl;

			if (Connect_flag_ == BoostAsio__TCP_sender::Connect_flag::Connect)
			{
				std::cout << socket_struct_p->Socket_name << ": Connect:" << socket_struct_p->socket_.remote_endpoint().address().to_string() << ":" << socket_struct_p->socket_.remote_endpoint().port() << std::endl;  //Выводим IP с которым был установлено соеднение.
			}

			if (Connect_flag_ == BoostAsio__TCP_sender::Connect_flag::Disconnect)
			{
				std::cout << socket_struct_p->Socket_name << ": Disconnect:" << BoostAsio__TCP_sender_p->get__BoostErrorText(boost_error.value()) << std::endl;
			}

			std::cout << "------------------------------------Connect callback: end------------------------------------------------" << std::endl;


		};
	//--------------------------------------------------


	BoostAsio__TCP_sender_.Connect_to_IP_adress(Socket_p_3, pointer_to_IP_3, Port_num_3, user_lambda_connect);    //ЕСЛИ КОЛБЕК НЕ НУЖЕН, ТО МОЖНО ПЕРЕДАТЬ - 0.

	//Колбек в который придет результат - естественно должен быть установлен заранее с помощью метода "add__Callback_for_Connect". Смотреть описание в соответвующем методе.
	
	//------------------------------------------------------------------------------------Connect_to_IP_adress:Конец-------------------------------------------------------------------------------



	//------------------------------------------------------------------------------------Connect_to_Web_adress:Начало-------------------------------------------------------------------------------
	
	//Данный метод устанавливает соединение с удаленным IP адрессом по его Интернет Web адрессу на ранее созданном сокете. Сокет может быть закрыт - функция внутри его откроет заново.
	//Метод Не должен вызыватся для Открытого Сокета!
	//Данный метод потоко-безопасен.

	/*
	//--------------------------------------------------
	BoostAsio__TCP_sender::Sockets__class::socket_struct* Socket_p_5 = added_socket_1;                //Указатель на ранее добавленный сокет.

	const char* pointer_to_Web_Adress_5 = "google.com";                               //Указатель на строку с Web адрессом, к которому нужно подключится.

	int Port_num_5 = 443;                                                             //Номер порта к которому нужно подключится.
	

	std::function<void(BoostAsio__TCP_sender* BoostAsio__TCP_sender_p, BoostAsio__TCP_sender::Sockets__class::socket_struct* socket_struct_p, const BoostAsio__TCP_sender::Connect_flag Connect_flag_, const boost::system::error_code& boost_error)> user_lambda_connect = [](BoostAsio__TCP_sender* BoostAsio__TCP_sender_p, BoostAsio__TCP_sender::Sockets__class::socket_struct* socket_struct_p, const BoostAsio__TCP_sender::Connect_flag Connect_flag_, const boost::system::error_code& boost_error)
		{
			//Дання лямбда будет вызыватся только для конкретного Сокета - указанного при вызове "Connect_to_IP_adress".

			//BoostAsio__TCP_sender_p - указатель на обьект данного класса.
			//socket_struct_p         - указатель на обьект структры сокета.
			//Connect_flag_           - флаг соединения.


			std::cout << "------------------------------------Connect callback: start----------------------------------------------" << std::endl;

			if (Connect_flag_ == BoostAsio__TCP_sender::Connect_flag::Connect)
			{
				std::cout << socket_struct_p->Socket_name << ": Connect:" << socket_struct_p->socket_.remote_endpoint().address().to_string() << ":" << socket_struct_p->socket_.remote_endpoint().port() << std::endl;  //Выводим IP с которым был установлено соеднение.
			}

			if (Connect_flag_ == BoostAsio__TCP_sender::Connect_flag::Disconnect)
			{
				std::cout << socket_struct_p->Socket_name << ": Disconnect:" << BoostAsio__TCP_sender_p->get__BoostErrorText(boost_error.value()) << std::endl;
			}

			std::cout << "------------------------------------Connect callback: end------------------------------------------------" << std::endl;


		};
	//--------------------------------------------------


	BoostAsio__TCP_sender_.Connect_to_IP_adress(added_socket_1, pointer_to_Web_Adress_5, Port_num_5, user_lambda_connect);       //ЕСЛИ КОЛБЕК НЕ НУЖЕН, ТО МОЖНО ПЕРЕДАТЬ - 0.

	//Колбек в который придет результат - естественно должен быть установлен заранее с помощью метода "add__Callback_for_Connect". Смотреть описание в соответвующем методе.
	*/
	//------------------------------------------------------------------------------------Connect_to_Web_adress:Конец-------------------------------------------------------------------------------



	//------------------------------------------------------------------------------------send_to_socket:Начало-------------------------------------------------------------------------------
	
	//Метод отправляет данные в ранее доавбленный и открытый сокет. 
	//После вызова данного метода - данные отправятся только после того, как будет установленно соединение с удаленным IP. До этого момента все задачи на отправку данных будут помещатся в очередь ожидания - пока не установится соединение.
	//Все данные на сокете будут отправлятся строго последовательно - вызовам данного метода, то есть если в цикле вызвать на одном и том же сокете сразу миллион раз отправку данных - то механизм внутри класса, поставит их в очередь и будет ждать, пока не отправятся все данные по первому вызову данного метода, потом по второму и так далее.
	//Данный метод потоко-безопасен.


	std::string Balvanka_message_7 = "Привет_Hello_6";

	//--------------------------------------------------
	BoostAsio__TCP_sender::Sockets__class::socket_struct* Socket_p_7 = added_socket_1;                //Указатель на ранее добавленный сокет.

	char* pointer_to_SendData_7 = &Balvanka_message_7[0];                             //Указатель на строку c данными, которые нужно отправить. ДАННЫЕ И УКАЗАТЕЛЬ НА НИХ ДОЛЖНЫ БЫТЬ ВАЛИДНЫМИ ВСЁ ВРЕМЯ ОТПРАВКИ ДАННЫХ!!!

	size_t SendData_size_7 = Balvanka_message_7.size();                              //Кол-во байт, которые нужно отправить по указателю "pointer_to_SendData_6".
	
	bool data_copy_flag = false;                                                     //false - значит, что данные по указанному указателю "pointer_to_SendData_7" не будут скопированы во внутренний буффер класса и данные по указанному указателю нужно держать валидными на все время передачи данных; true - в этом случае данные по указанному указателю в размере "SendData_size_7" будут скопированы во внутренний буффер и с данными по указаннмо указателю можно делать все что угодно.

	std::function<void(BoostAsio__TCP_sender* BoostAsio__TCP_sender__class_p, BoostAsio__TCP_sender::Sockets__class::socket_struct* socket_struct_p, const BoostAsio__TCP_sender::CallbackMessage CallbackMessage_, const size_t bytes_send_transferred, const size_t bytes_send_transferred_TOTAL)> user_lambda_send = [](BoostAsio__TCP_sender* BoostAsio__TCP_sender__class_p, BoostAsio__TCP_sender::Sockets__class::socket_struct* socket_struct_p, const BoostAsio__TCP_sender::CallbackMessage CallbackMessage_, const size_t bytes_send_transferred, const size_t bytes_send_transferred_TOTAL)
		{
			//Дання лямбда будет вызыватся только для конкретного Сокета - указанного при вызове "send_to_socket".

			//BoostAsio__TCP_sender_p         - указатель на обьект данного класса.
			//socket_struct_p                 - указатель на обьект структры сокета.
			//CallbackMessage_                - флаг: Part - значит это очередная часть передающегося сообщения, Full - значит это полсденяя часть переданного сообщения и сообщение считается полностью переданным.
			//bytes_send_transferred          - размер этой переданной части.
			//bytes_send_transferred_TOTAL    - размер суммарно переданных байт.


			std::cout << "------------------------------------Send callback: start----------------------------------------------" << std::endl;

			if (CallbackMessage_ == BoostAsio__TCP_sender::CallbackMessage::Part)
			{
				std::cout << socket_struct_p->Socket_name << ":" << bytes_send_transferred << " of " << bytes_send_transferred_TOTAL << ":Part" << std::endl;
			}

			if (CallbackMessage_ == BoostAsio__TCP_sender::CallbackMessage::Full)
			{
				std::cout << socket_struct_p->Socket_name << ":" << bytes_send_transferred << " of " << bytes_send_transferred_TOTAL << ":Full" << std::endl;
			}

			std::cout << "------------------------------------Send callback: end------------------------------------------------" << std::endl;


		};
	//--------------------------------------------------

	BoostAsio__TCP_sender_.send_to_socket(Socket_p_7, pointer_to_SendData_7, SendData_size_7, data_copy_flag, user_lambda_send);     //ЕСЛИ КОЛБЕК НЕ НУЖЕН, ТО МОЖНО ПЕРЕДАТЬ - 0.

	//Колбек в который придет результат - естественно должен быть установлен заранее с помощью метода "add__Callback_for_SocketWrite". Смотреть описание в соответвующем методе.
	
	//------------------------------------------------------------------------------------send_to_socket:Конец-------------------------------------------------------------------------------





	//------------------------------------------------------------------------------------set__CallbackMode_for_SocketWrite:Начало-------------------------------------------------------------------------------
	
	//Данный метод устанавливает флаг колбека для метода "send_to_socket": будет ли колбек вызыватся для всех частей передаваемого сообщения или вызовется только один раз, когда будет переданы все части сообщения.
	//Данный метод потоко-безопасен.


	//--------------------------------------------------
	BoostAsio__TCP_sender::Sockets__class::socket_struct* Socket_p_7_2 = added_socket_1;                //Указатель на ранее добавленный сокет.

	BoostAsio__TCP_sender::CallbackMode CallbackMode_7_2 = BoostAsio__TCP_sender::CallbackMode::Call_OnlyFull_Send;    //Call_AlwaysPart_Send - значит колбек будет вызыватся для все передаваемых частей. Call_OnlyFull_Send - Значит колбек вызовется только один раз для финальной части, когда все сообщение будет передано.
	//--------------------------------------------------


	BoostAsio__TCP_sender_.set__CallbackMode_for_SocketWrite(Socket_p_7_2, CallbackMode_7_2);

	
	//------------------------------------------------------------------------------------set__CallbackMode_for_SocketWrite:Конец-------------------------------------------------------------------------------




	//------------------------------------------------------------------------------------set__InternalBuffer_for_Read:Начало-------------------------------------------------------------------------------
	
	//Данный метод устанавливает буффер определнного размера для приема данных после активации метода "set__Read" - буффер устанавливается внутренний - внутри класса, то есть приемные данные сначала будут записаны в него, а потом в пользовтаельском колбеке - будет прислан указатель на этот внутренний буффер и размер байт, который можно считать их него.
	//Данный метод установлен по умолчанию и размер буффера имеет 1024 байта.
	//Данный метод потоко-безопасен.
	

	//----------------------------------------------------
	BoostAsio__TCP_sender::Sockets__class::socket_struct* Socket_p_11 = added_socket_1;                //Указатель на ранее добавленный сокет.

	size_t InternalBuff_size_11 = 100;                                                //Размер внутреннго буффера для приема данных.
	//----------------------------------------------------



	BoostAsio__TCP_sender_.set__InternalBuffer_for_Read(Socket_p_11, InternalBuff_size_11);
	
	//------------------------------------------------------------------------------------set__InternalBuffer_for_Read:Конец-------------------------------------------------------------------------------


	//------------------------------------------------------------------------------------set__UserBuffer_for_Read:Начало-------------------------------------------------------------------------------
	/*
	//Данный метод устанавливает Пользовательский указатель на буффер и размер по нему для приема данных после активации метода "set__Read" - то есть приемные данные будут записаны по указанному указателю максимально указанного размера - после чего вызовется колбек.
	//Данный метод потоко-безопасен.


	std::string UserBuffer_13;  UserBuffer_13.resize(400);

	//----------------------------------------------------
	BoostAsio__TCP_sender::Sockets__class::socket_struct* Socket_p_13 = added_socket_1;               //Указатель на ранее добавленный сокет.

	char* pointer_to_buff_for_Recieve_13 = &UserBuffer_13[0];                         //Указатель на буффер, куда будут записаны данные при приеме.

	size_t UserBuff_size_13 = UserBuffer_13.size();                                   //Размер буффера для приема данных по указателю.
	//----------------------------------------------------



	BoostAsio__TCP_sender_.set__UserBuffer_for_Read(Socket_p_13, pointer_to_buff_for_Recieve_13, UserBuff_size_13);
	*/
	//------------------------------------------------------------------------------------set__UserBuffer_for_Read:Конец-------------------------------------------------------------------------------




	//---------------------------------------------------------------------------------set__Read:Начало-------------------------------------------------------------------------------
	
	//Данный метод включает на ранее добавленном сокете режим приема данных с удаленного IP адресса.
	//После вызова данного метода режим приема на сокете включится - только после того, как будет установленно соединение с удаленным IP. До этого момента задача на включение приема на сокете будет помещена в очередь ожидания - пока не установится соединение.
	//Данный метод потоко-безопасен.
	//ВНИМАНИЕ: если до вызова этого метода, был вызван метод "set__ReadUntil", то, чтобы вызвать этот метод - сокет нужно закрыть и открыть заново или вызвать "redefine_socket".


	//--------------------------------------------------
	BoostAsio__TCP_sender::Sockets__class::socket_struct* Socket_p_9 = added_socket_1;                //Указатель на ранее добавленный сокет.


	std::function<void(BoostAsio__TCP_sender* BoostAsio__TCP_sender__class_p, BoostAsio__TCP_sender::Sockets__class::socket_struct* socket_struct_p, const char* pointer_to_DataReceived, const size_t bytes_received)> user_lambda_read = [](BoostAsio__TCP_sender* BoostAsio__TCP_sender__class_p, BoostAsio__TCP_sender::Sockets__class::socket_struct* socket_struct_p, const char* pointer_to_DataReceived, const size_t bytes_received)
		{
			//Дання лямбда будет вызыватся только для конкретного Сокета - указанного при вызове "set__Read".

			//BoostAsio__TCP_sender_p         - указатель на обьект данного класса.
			//socket_struct_p                 - указатель на обьект структры сокета.
			//pointer_to_DataReceived         - указатель на начало данных - куда записаны принятые данные.  Данные по указателю актуальны ТОЛЬКО в момент вызова данного колбека.
			//bytes_received                  - размер данных по указателю, который доступен для считывания.



			std::cout << "------------------------------------Read callback: start----------------------------------------------" << std::endl;

			std::string temp_string;

			temp_string.append(pointer_to_DataReceived, bytes_received);

			std::cout << socket_struct_p->Socket_name << ": Recieve data: " << bytes_received << ":" << temp_string << std::endl;

			std::cout << "------------------------------------Read callback: end------------------------------------------------" << std::endl;


		};
	//--------------------------------------------------

	BoostAsio__TCP_sender_.set__Read(Socket_p_9, user_lambda_read);    //ЕСЛИ КОЛБЕК НЕ НУЖЕН, ТО МОЖНО ПЕРЕДАТЬ - 0.

	//Колбек в который придет результат - естественно должен быть установлен заранее с помощью метода "add__Callback_for_SocketRead". Смотреть описание в соответвующем методе.

	//---------------------------------------------------------------------------------set__Read:Конец-------------------------------------------------------------------------------




	//---------------------------------------------------------------------------------set__ReadUntil:Начало-------------------------------------------------------------------------------

	//Данный метод включает на ранее добавленном сокете режим приема данных с удаленного IP адресса в режиме ожадения подстроки разделителя. ТО ЕСТЬ колбек будет вызывается только тогда, когда в Приемных данных появится указанный разделитель.
	//После вызова данного метода режим приема на сокете включится - только после того, как будет установленно соединение с удаленным IP. До этого момента задача на включение приема на сокете будет помещена в очередь ожидания - пока не установится соединение.
	//Данный метод потоко-безопасен.
	//ВНИМАНИЕ: если до вызова этого метода, был вызван метод "set__Read", то, чтобы вызвать этот метод - сокет нужно закрыть и открыть заново или вызвать "redefine_socket".

	//--------------------------------------------------
	BoostAsio__TCP_sender::Sockets__class::socket_struct* Socket_p_8_2 = added_socket_1;                //Указатель на ранее добавленный сокет.

	std::string separator_8_2 = "HUY";                                       //Разделитель, который нужно ожидать в Приемных данных.


	std::function<void(BoostAsio__TCP_sender* BoostAsio__TCP_sender__class_p, BoostAsio__TCP_sender::Sockets__class::socket_struct* socket_struct_p, const char* pointer_to_DataReceived, const size_t bytes_received)> user_lambda_readuntill = [](BoostAsio__TCP_sender* BoostAsio__TCP_sender__class_p, BoostAsio__TCP_sender::Sockets__class::socket_struct* socket_struct_p, const char* pointer_to_DataReceived, const size_t bytes_received)
		{
			//Дання лямбда будет вызыватся только для конкретного Сокета - указанного при вызове "user_lambda_readuntill".

			//BoostAsio__TCP_sender_p         - указатель на обьект данного класса.
			//socket_struct_p                 - указатель на обьект структры сокета.
			//pointer_to_DataReceived         - указатель на начало данных - куда записаны принятые данные.  Данные по указателю актуальны ТОЛЬКО в момент вызова данного колбека.
			//bytes_received                  - размер данных по указателю, который доступен для считывания.



			std::cout << "------------------------------------Read callback: start----------------------------------------------" << std::endl;

			std::string temp_string;

			temp_string.append(pointer_to_DataReceived, bytes_received);

			std::cout << socket_struct_p->Socket_name << ": Recieve data: " << bytes_received << ":" << temp_string << std::endl;

			std::cout << "------------------------------------Read callback: end------------------------------------------------" << std::endl;


		};
	//--------------------------------------------------

	BoostAsio__TCP_sender_.set__ReadUntil(Socket_p_8_2, separator_8_2, user_lambda_readuntill);    //ЕСЛИ КОЛБЕК НЕ НУЖЕН, ТО МОЖНО ПЕРЕДАТЬ - 0.

	//Колбек в который придет результат - естественно должен быть установлен заранее с помощью метода "add__Callback_for_SocketRead". Смотреть описание в соответвующем методе.

	//---------------------------------------------------------------------------------set__ReadUntil:Конец-------------------------------------------------------------------------------




	//------------------------------------------------------------------------------------redefine_socket:Начало-------------------------------------------------------------------------------

	//Данный метод "переопределяет" ранее созданный сокет: то есть просто закрыает сокет, если он открыт и переоткрывает с указанным протоколом IPv4 или IPv6. Если сокет закрыт, то данный метод просто его открывает.
	//Если нужно удалить вообще ВСЕ ЗАДАЧИ - то нужно вызывать метод "close_Socket".
	//Данный метод потоко-безопасен.


	//--------------------------------------------------
	BoostAsio__TCP_sender::Sockets__class::socket_struct* Socket_p_19 = added_socket_1;                //Указатель на ранее добавленный сокет.

	boost::asio::ip::tcp IP_type_19 = boost::asio::ip::tcp::v4();                     //Тип протокола TCP: IPv4 или IPv6.
	//--------------------------------------------------

	BoostAsio__TCP_sender_.get__SocketClass_pointer()->redefine_socket(Socket_p_19, IP_type_19);

	//------------------------------------------------------------------------------------redefine_socket:Конец-------------------------------------------------------------------------------



	//------------------------------------------------------------------------------------close_Socket:Начало-------------------------------------------------------------------------------

	//Данный метод закрывает указанный сокет, отменяя все асинхроные операции и ВСЕ ЗАДАЧИ, который есть в очередях ожидания установки соединения и в очереди последоватльной отправки сообщений.
	//ПОЭТОМУ если нужно закрыть Сокет, только после гарантированной отправки какого либо сообщения, то нужно использовать колбек вместе вызовом "send_to_socket" - и из колбекка уже вызывать закрытие сокета.

	BoostAsio__TCP_sender::Sockets__class::socket_struct* Socket_p_close = added_socket_1;                //Указатель на ранее добавленный сокет.

	//BoostAsio__TCP_sender_.get__SocketClass_pointer()->close_Socket(Socket_p_close);
	//------------------------------------------------------------------------------------close_Socket:Начало-------------------------------------------------------------------------------



	//------------------------------------------------------------------------------------close_AllSocket:Начало-------------------------------------------------------------------------------

	//Данный метод закрывает все сокеты, отменяя все асинхроные операции и ВСЕ ЗАДАЧИ, который есть в очередях ожидания установки соединения и в очереди последоватльной отправки сообщений.
	//Данный метод потоко-безопасен.


	//BoostAsio__TCP_sender_.get__SocketClass_pointer()->close_AllSocket();

	//------------------------------------------------------------------------------------close_AllSocket:Конец-------------------------------------------------------------------------------



	//------------------------------------------------------------------------------------delete_AllSocket:Начало-------------------------------------------------------------------------------

	//Данный метод просто удаляет все ранее созданные сокеты.
	//Данный метод потоко-безопасен.


	//BoostAsio__TCP_sender_.get__SocketClass_pointer()->delete_AllSocket();

	//------------------------------------------------------------------------------------delete_AllSocket:Конец-------------------------------------------------------------------------------


	//------------------------------------------------------------------------------------open_Previously_ClosedSocket:Начало-------------------------------------------------------------------------------

	//Данный метод открывает любой ранее Закрытый сокет, если такой есть в очереди закрытых сокетов. Если же Закрытых Сокетов Нет - то функция возвращает 0 иначе указатель на открытый сокет.
	//Данный метод потоко-безопасен.

	/*
	//--------------------------------------------------
	std::string Socket_name_20 = "Socket_Name_1";                                      //Идентификатор ранее добавленного сокета.

	boost::asio::ip::tcp IP_type_20 = boost::asio::ip::tcp::v4();                     //Тип протокола TCP: IPv4 или IPv6.
	//--------------------------------------------------


	BoostAsio__TCP_sender::Sockets__class::socket_struct* socket_struct_p_20 = BoostAsio__TCP_sender_.get__SocketClass_pointer()->open_Previously_ClosedSocket(IP_type_20);

	if (socket_struct_p_20 != 0)
	{
		std::cout << "Socket Open Name:"<< socket_struct_p_20->Socket_name <<std::endl;
	}
	else
	{
		//Значит закрытых ранее сокетов не было.
	}
	*/
	//------------------------------------------------------------------------------------open_Previously_ClosedSocket:Конец-------------------------------------------------------------------------------




	//------------------------------------------------------------------------------------stop:Начало-------------------------------------------------------------------------------

	//Данный метод ВСЁ полностью останавливает:
	//-Останавливает все задачи "резолвы" на Сокете - если они есть.
	//-Останавливает все задади отсылки/приема на Сокете - если они есть.
	//-Дожидается в Блокирующем режиме завершения всех отмененых задач, которые выполнялись асинхронно. То есть пока все задачи не завершатся этот метод будет полностьб бловировать дальнейшее выполненеи кода.
	//-Отменяет бесконечный цикл io_context, и код достигает вызова io_context.run() и проходит его До вызова "wait_here()" и далее. То есть теперь обьект данного класса не блокирует поток или потоки и Пользовательский код, который находится после "wait_here()" - будет выполнятся дальше, как обычно.
	//Данный метод потоко-безопасен.


	//BoostAsio__TCP_sender_.stop();

	//Вообщем данный метод вызывается, когда нужно полностью завершить работу данного класса.

	//------------------------------------------------------------------------------------stop:Конец-------------------------------------------------------------------------------




	//------------------------------------------------------------------------------------wait_here:Начало-------------------------------------------------------------------------------

	//Данный метод - это аналог метода .join() для потоков. Данный класс будет крутится в условном бесконечом цикле не проходя метод "wait_here()" - пока класс полностью не оставят методом stop().
	//Данный метод всегда вызывается последним и ждет пока класс не завершат.
    //ВНИМАНИЕ!!!! УНИЧТОЖАТЬ ОБЬЕКТ ДАННОГО КЛАССА МОЖНО ТОЛЬКО ПОСЛЕ ТОГО, КАК ЗАВЕРШЕН ДАННЫЙ МЕТОД WAIT_HERE() - ПОТОМУ ЧТО ЛОГИЧНО ЕСЛИ КЛАСС УНИЧТОЖИТЬ РАНЬШЕ, ТО МЕТОД  wait_here() ВЫЗОВЕТСЯ У НУЛЕГО ОБЬЕКТА, ЧТО ПРИВЕДЕТ К НЕМЕДЛЕННОМУ ПАДЕНИЮ ПРОГРАММЫ.
	//Данный метод потоко-безопасен.


	BoostAsio__TCP_sender_.wait_here();

	//------------------------------------------------------------------------------------wait_here:Конец-------------------------------------------------------------------------------


	 //**********************************************************************************2-BoostAsio__TCP_sender:Конец*******************************************************************************************



    return 0;


}
