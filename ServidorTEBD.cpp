// Mubbo.cpp : main project file.

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>

using namespace System;
using namespace System::Threading;
using namespace System::IO;
using namespace System::Net;
using namespace System::Net::Sockets;
using namespace System::Text;
using namespace System::Xml;
using namespace System::Xml::Schema;
using namespace System::Xml::XPath;
using namespace System::Xml::Xsl;


#define BUFFER_SIZE 4096
int FLAG_XSL;


static void ValidationEventHandlerOne(Object^ sender, ValidationEventArgs^ e)
{
	switch (e->Severity)
	{
	case XmlSeverityType::Error:
		Console::WriteLine("Error: {0}", e->Message);
		break;
	case XmlSeverityType::Warning:
		Console::WriteLine("Warning {0}", e->Message);
		break;
	default:
		Console::WriteLine("Sucesso!");
		break;
	}
}

void boletimValidation(String^ boletimXML, Socket^ serverSocket) {
	Console::WriteLine("\nBoletim recebido:\n" + boletimXML + "\n");


	if (FLAG_XSL == 1) {

		String^ stylesheet = "arquivos/boletim.xsl";

		XslTransform^ xslt = gcnew XslTransform;
		xslt->Load(stylesheet);

		XmlReader^ reader = XmlReader::Create(gcnew StringReader(boletimXML));
		XmlDocument^ doc = gcnew XmlDocument();
		doc->Load(reader);

		StringWriter^ writer = gcnew StringWriter();

		xslt->Transform(doc, nullptr, writer, nullptr);

		////

		String^ out = writer->ToString();
		Console::WriteLine(out);



		XmlReaderSettings^ settings = gcnew XmlReaderSettings();
		settings->Schemas->Add("https://www.w3schools.com", "arquivos/boletim.xsd");
		settings->ValidationType = ValidationType::Schema;

		XmlReader^ readerXsl = XmlReader::Create(gcnew StringReader(out), settings);
		XmlDocument^ document = gcnew XmlDocument();
		document->Load(readerXsl);

		ValidationEventHandler^ eventHandler = gcnew ValidationEventHandler(ValidationEventHandlerOne);

		document->Validate(eventHandler);

		Console::Write("Boletim validado com sucesso\n");
		String^ stringRetorno;
		stringRetorno = "<?xml version='1.0' encoding='utf-8'?><response_boletim><status_submeter>" + "0" + "</status_submeter></response_boletim>";
		Console::WriteLine("Mensagem retornada:\n" + stringRetorno);
		array<Byte>^ data = System::Text::Encoding::UTF8->GetBytes(stringRetorno);
		serverSocket->Send(data);
	}
	else if (FLAG_XSL == 0) {
		XmlReaderSettings^ settings = gcnew XmlReaderSettings();
		settings->Schemas->Add("https://www.w3schools.com", "arquivos/boletim.xsd");
		settings->ValidationType = ValidationType::Schema;

		XmlReader^ reader = XmlReader::Create(gcnew StringReader(boletimXML), settings);
		XmlDocument^ document = gcnew XmlDocument();
		document->Load(reader);

		ValidationEventHandler^ eventHandler = gcnew ValidationEventHandler(ValidationEventHandlerOne);

		document->Validate(eventHandler);
		Console::Write("Boletim validado com sucesso\n");
		String^ stringRetorno;
		stringRetorno = "<?xml version='1.0' encoding='utf-8'?><response_boletim><status_submeter>" + "0" + "</status_submeter></response_boletim>";
		Console::WriteLine("Mensagem retornada:\n" + stringRetorno);
		array<Byte>^ data = System::Text::Encoding::UTF8->GetBytes(stringRetorno);
		serverSocket->Send(data);
	}

}

void consultaValidation(String^ consultaXML, Socket^ serverSocket) {
	Console::WriteLine("\nConsulta recebida:\n" + consultaXML + "\n");
	XmlReaderSettings^ settings = gcnew XmlReaderSettings();
	settings->Schemas->Add("https://www.w3schools.com", "arquivos/consulta.xsd");
	settings->ValidationType = ValidationType::Schema;

	XmlReader^ reader = XmlReader::Create(gcnew StringReader(consultaXML), settings);
	XmlDocument^ document = gcnew XmlDocument();
	document->Load(reader);

	ValidationEventHandler^ eventHandler = gcnew ValidationEventHandler(ValidationEventHandlerOne);


	document->Validate(eventHandler);

	String^ tempCPF = document->ChildNodes[1]->ChildNodes[0]->InnerText;
	String^ stringRetorno;
	if (tempCPF == "0000000000") {
		stringRetorno = "<?xml version = '1.0' encoding = 'utf-8' ?><response_consulta_status><status_consulta_status>" + "0" + "</status_consulta_status></response_consulta_status>\0";
	}
	else if (tempCPF == "0000000001") {
		stringRetorno = "<?xml version = '1.0' encoding = 'utf-8' ?><response_consulta_status><status_consulta_status>" + "1" + "</status_consulta_status></response_consulta_status>\0";
	}
	else if (tempCPF == "0000000002") {
		stringRetorno = "<?xml version = '1.0' encoding = 'utf-8' ?><response_consulta_status><status_consulta_status>" + "2" + "</status_consulta_status></response_consulta_status>\0";
	}
	else if (tempCPF == "0000000003") {
		stringRetorno = "<?xml version = '1.0' encoding = 'utf-8' ?><response_consulta_status><status_consulta_status>" + "3" + "</status_consulta_status></response_consulta_status>\0";
	}
	else if (tempCPF == "0000000004") {
		stringRetorno = "<?xml version = '1.0' encoding = 'utf-8' ?><response_consulta_status><status_consulta_status>" + "4" + "</status_consulta_status></response_consulta_status>\0";
	}
	else {
		stringRetorno = "<?xml version = '1.0' encoding = 'utf-8' ?><response_consulta_status><status_consulta_status>" + "Erro interno" + "</status_consulta_status></response_consulta_status>\0";
	}
	Console::WriteLine("Mensagem retornada:\n" + stringRetorno);
	array<Byte>^ data = System::Text::Encoding::UTF8->GetBytes(stringRetorno);
	serverSocket->Send(data);
}

void requestValidation(Socket^ serverSocket) {
	array<Byte>^ receiveBuffer = gcnew array<Byte>(BUFFER_SIZE);
	StringBuilder^ stringReceive = gcnew StringBuilder();
	int rc;

	do
	{
		rc = serverSocket->Receive(receiveBuffer);
		if (receiveBuffer[BUFFER_SIZE - 1] == '\0')
			rc = BUFFER_SIZE - 1;
		stringReceive->Append(ASCIIEncoding::ASCII->GetString(receiveBuffer)->TrimEnd('\0'));
		Array::Clear(receiveBuffer, 0, BUFFER_SIZE);
	} while (rc == BUFFER_SIZE);

	Console::WriteLine("Requisicao recebida:\n" + stringReceive->ToString());

	XmlReaderSettings^ settings = gcnew XmlReaderSettings();
	settings->Schemas->Add("https://www.w3schools.com", "arquivos/request.xsd");
	settings->ValidationType = ValidationType::Schema;



	XmlReader^ reader = XmlReader::Create(gcnew StringReader(stringReceive->ToString()), settings);
	XmlDocument^ document = gcnew XmlDocument();
	document->Load(reader);

	ValidationEventHandler^ eventHandler = gcnew ValidationEventHandler(ValidationEventHandlerOne);
	document->Validate(eventHandler);
	Console::WriteLine("Request validado com sucesso!");

	String^ metodo = document->ChildNodes[1]->ChildNodes[0]->InnerText;
	if (metodo == "submeter") {
		String^ boletimXML = document->ChildNodes[1]->ChildNodes[1]->InnerText;
		boletimValidation(boletimXML, serverSocket);
	}
	else if (metodo == "consulta") {
		String^ consultaXML = document->ChildNodes[1]->ChildNodes[1]->InnerText;
		consultaValidation(consultaXML, serverSocket);
	}
}

void threadReceive(Object^ argsSocket) {
	auto clientSocket = safe_cast<Socket^>(argsSocket);

	for (int n = 0;; n++) {
		//Recebendo XML REQUEST
		try
		{
			requestValidation(clientSocket);
		}
		catch (SocketException^ e) {
			Console::WriteLine("Conexão perdida");
			clientSocket->Shutdown(SocketShutdown::Both);
			clientSocket->Close();
			Console::WriteLine("Shutting down server");
		}
		catch (XmlSchemaValidationException^ e) {
			String^ stringRetorno;
			stringRetorno = "<?xml version = '1.0' encoding = 'utf-8' ?><response_boletim><status_submeter>" + "1" + "</status_submeter></response_boletim>\0";
			Console::WriteLine("Mensagem retornada:\n" + stringRetorno);
			array<Byte>^ data = System::Text::Encoding::UTF8->GetBytes(stringRetorno);
			clientSocket->Send(data);
			//Console::WriteLine("XML Invalido");
		}
		catch (XmlException^ e) {
			String^ stringRetorno;
			stringRetorno = "<?xml version = '1.0' encoding = 'utf-8' ?><response_boletim><status_submeter>" + "2" + "</status_submeter></response_boletim>\0";
			Console::WriteLine("Mensagem retornada:\n" + stringRetorno);
			array<Byte>^ data = System::Text::Encoding::UTF8->GetBytes(stringRetorno);
			clientSocket->Send(data);
			//Console::WriteLine("XML mal-formado");
		}
		catch (ObjectDisposedException^ e) {
			Console::WriteLine("Cliente desconectado");
			clientSocket->Close();
			break;
		}

		Console::WriteLine("Mensagem: " + n + "\n\n");
	}
}



int main(array<String ^> ^args)
{
	int port;
	if (args->Length == 2) {
		port = Int32::Parse(args[0]);
		FLAG_XSL = Int32::Parse(args[1]);
	}
	else {
		Console::WriteLine("Sem parâmetros");
		exit(1);
	}
		

	


	IPAddress^ ipAddress;

	for each (auto addr in Dns::GetHostEntry(Dns::GetHostName())->AddressList) {
		if (addr->AddressFamily == AddressFamily::InterNetwork) {
			ipAddress = addr;
			break;
		}
	}

	Console::WriteLine("Meu ip: {0}", ipAddress);
	IPEndPoint^ ipEndpoint = gcnew IPEndPoint(ipAddress, port);
	Socket^ listenerSocket = gcnew Socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp);
	listenerSocket->Bind(ipEndpoint);
	Console::WriteLine("Porta {0} em uso", port);


	while (true) {

		listenerSocket->Listen(1);

		Console::WriteLine("Aguardando conexão");
		Socket^ serverSocket = listenerSocket->Accept();
		Console::WriteLine("Nova conexão aceita\n\n");

		Thread^ newThread = gcnew Thread(gcnew ParameterizedThreadStart(&threadReceive));
		newThread->Start(serverSocket);
	}


	Console::WriteLine("Shutting down server");
}