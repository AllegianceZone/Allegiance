#pragma once

#using <Microsoft.WindowsAzure.Configuration.dll>
#using <Microsoft.ServiceBus.dll>
#using <Newtonsoft.Json.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Diagnostics;
using namespace Newtonsoft::Json;
using namespace Microsoft::ServiceBus::Messaging;
using namespace Microsoft::WindowsAzure;
using namespace Microsoft::Win32;

namespace Lobby {

	/// <summary>
	/// Summary for MyComponent
	/// </summary>
	public ref class MyComponent 
	{
	public:
		MyComponent(void)
		{
			RegistryKey^ key = Microsoft::Win32::Registry::LocalMachine->OpenSubKey("Software\\Microsoft\\Microsoft Games\\Allegiance\\1.3\\AllLobby");
			System::String^ _topicConnectionString = "Endpoint=sb://azstream.servicebus.windows.net/;SharedAccessKeyName=azlobbyinstance;SharedAccessKey=4JwtkNOQeWi0mko/HezOvxr5aaVPmOZ7km4N2hj1OVY=";
			String^ conn = Convert::ToString(key->GetValue("lobbygames.azurestream.connectionstring"));
			if (!String::IsNullOrEmpty(conn))
				tc = TopicClient::CreateFromConnectionString(_topicConnectionString, "lobbygames");
		}
		void UpdateWebsite(){
			BrokeredMessage^ m = gcnew BrokeredMessage("{'somejson'=true}");
			tc->SendAsync(m);
		}
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyComponent()
		{
		}

	private:
		TopicClient^ tc;
	};
}
