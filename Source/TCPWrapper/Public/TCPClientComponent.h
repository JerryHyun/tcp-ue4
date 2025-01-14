#pragma once

#include "Components/ActorComponent.h"
#include "Networking.h"
#include "IPAddress.h"
#include "TCPServerComponent.h"
#include "TCPClientComponent.generated.h"


UCLASS(ClassGroup = "Networking", meta = (BlueprintSpawnableComponent))
class TCPWRAPPER_API UTCPClientComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()
public:

	//Async events

	/** On message received on the receiving socket. */
	UPROPERTY(BlueprintAssignable, Category = "TCP Events")
	FTCPMessageSignature OnReceivedBytes;

	/** Callback when we start listening on the TCP receive socket*/
	UPROPERTY(BlueprintAssignable, Category = "TCP Events")
	FTCPEventSignature OnConnected;

	/** Default sending socket IP string in form e.g. 127.0.0.1. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TCP Connection Properties")
	FString ConnectionIP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TCP Connection Properties")
	int32 ConnectionPort;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TCP Connection Properties")
	FString ClientSocketName;

	/** in bytes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TCP Connection Properties")
	int32 BufferMaxSize;

	/** If true will auto-connect on begin play to IP/port specified as a client. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TCP Connection Properties")
	bool bShouldAutoConnectOnBeginPlay;

	/** Whether we should process our data on the game thread or the TCP thread. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TCP Connection Properties")
	bool bReceiveDataOnGameThread;


	/**
	* Connect to a TCP endpoint, optional method if auto-connect is set to true.
	* Emit function will then work as long the network is reachable. By default
	* it will attempt this setup for this socket on beginplay.
	*
	* @param InIP the ip4 you wish to connect to
	* @param InPort the TCP port you wish to connect to
	*/
	UFUNCTION(BlueprintCallable, Category = "TCP Functions")
	void ConnectToSocketAsClient(const FString& InIP = TEXT("127.0.0.1"), const int32 InPort = 3000);

	/**
	* Close the sending socket. This is usually automatically done on endplay.
	*/
	UFUNCTION(BlueprintCallable, Category = "TCP Functions")
	void CloseSocket();

	/**
	* Emit specified bytes to the TCP channel.
	*
	* @param Message	Bytes
	*/
	UFUNCTION(BlueprintCallable, Category = "TCP Functions")
	bool Emit(const TArray<uint8>& Bytes);
	
	UFUNCTION(BlueprintCallable, Category = "TCP Functions")
	bool IsConnected();

	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
protected:
	FSocket* ClientSocket;
	FThreadSafeBool bShouldReceiveData;
	TFuture<void> ClientConnectionFinishedFuture;

	//FTCPSocketReceiver* TCPReceiver;
	FString SocketDescription;
	TSharedPtr<FInternetAddr> RemoteAdress;
};
