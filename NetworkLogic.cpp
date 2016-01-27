#include "NetworkLogic.h"

#if defined _EG_MARMALADE_PLATFORM
#	if defined I3D_ARCH_X86
#		if(defined _MSC_VER && !defined __clang__ && !defined __gcc__)
#			define PLAYER_NAME L"Marmalade X86 Windows"
#		else
#			define PLAYER_NAME L"Marmalade X86 OS X"
#		endif
#	elif defined I3D_ARCH_ARM
#		define PLAYER_NAME L"Marmalade ARM"
#	else
#		define PLAYER_NAME L"unknown Marmalade platform"
#	endif
#elif defined _EG_WINDOWS_PLATFORM
#	define PLAYER_NAME L"Win32"
#elif defined _EG_IPHONE_PLATFORM
#	define PLAYER_NAME L"iOS"
#elif defined _EG_IMAC_PLATFORM
#	define PLAYER_NAME L"OS X"
#elif defined _EG_ANDROID_PLATFORM
#	define PLAYER_NAME L"Android"
#elif defined _EG_BLACKBERRY_PLATFORM
#	define PLAYER_NAME L"Blackberry"
#elif defined _EG_PS3_PLATFORM
#	define PLAYER_NAME L"PS3"
#elif defined _EG_LINUX_PLATFORM
#	define PLAYER_NAME L"Linux"
#else
#	define PLAYER_NAME L"unknown platform"
#endif

ExitGames::Common::JString& NetworkLogicListener::toString(ExitGames::Common::JString& retStr, bool /*withTypes*/) const
{
	return retStr;
}

State StateAccessor::getState(void) const
{
	return mState;
}

void StateAccessor::setState(State newState)
{
	mState = newState;
	for(unsigned int i=0; i<mStateUpdateListeners.getSize(); i++)
		mStateUpdateListeners[i]->stateUpdate(newState);
}

void StateAccessor::registerForStateUpdates(NetworkLogicListener* listener)
{
	mStateUpdateListeners.addElement(listener);
}

Input NetworkLogic::getLastInput(void) const
{
	return mLastInput;
}

void NetworkLogic::setLastInput(Input newInput)
{
	mLastInput = newInput;
}

State NetworkLogic::getState(void) const
{
	return mStateAccessor.getState();
}

// functions
NetworkLogic::NetworkLogic(OutputListener* listener, const wchar_t* appVersion)
#ifdef _EG_MS_COMPILER
#	pragma warning(push)
#	pragma warning(disable:4355)
#endif
	: mLoadBalancingClient(*this, L"2f6b07c8-8892-4f5f-bd39-976c9c07cb82", appVersion, PLAYER_NAME)
	, mLastInput(INPUT_NON)
	, mOutputListener(listener)
#ifdef _EG_MS_COMPILER
#	pragma warning(pop)
#endif
{
	mStateAccessor.setState(STATE_INITIALIZED);
	mLoadBalancingClient.setDebugOutputLevel(ExitGames::Common::DebugLevel::ALL);
	mLogger.setListener(*this);
	mLogger.setDebugOutputLevel(ExitGames::Common::DebugLevel::INFO);
    mGameID = L"";
}

void NetworkLogic::registerForStateUpdates(NetworkLogicListener* listener)
{
	mStateAccessor.registerForStateUpdates(listener);
}

void NetworkLogic::connect(void)
{
	mLoadBalancingClient.connect(); // specify the ip and port of your local masterserver here; call the parameterless overload instead, if you want to connect to the  exitgamescloud
	mStateAccessor.setState(STATE_CONNECTING);
}

void NetworkLogic::disconnect(void)
{
	mLoadBalancingClient.disconnect();
}

void NetworkLogic::opCreateRoom(void)   
{
	// if last digits are always nearly the same, this is because of the timer calling this function being triggered every x ms with x being a factor of 10
	ExitGames::Common::JString tmp;
	mLoadBalancingClient.opCreateRoom(tmp=(int)GETTIMEMS(), true, true, MAX_NUMBER_OF_PLAYERS);
    
	mStateAccessor.setState(STATE_JOINING);
	mOutputListener->writeLine(ExitGames::Common::JString(L"creating game \"") + tmp + L"\"");
}

void NetworkLogic::opJoinRoom(std::string roomNumber)
{
    mLoadBalancingClient.opJoinRoom(ExitGames::Common::JString(roomNumber.c_str()));
}

void NetworkLogic::opJoinRandomRoom(void)
{

    int count = mLoadBalancingClient.getRoomList().getSize();
    
    bool wasCreated = count > 0;
    
    if (wasCreated)
    {
        mLoadBalancingClient.opJoinRandomRoom();
        mOutputListener->writeLine(L"opJoinRandomRoom: Joining  random room");
    }
    else
    {
        opCreateRoom();
        mOutputListener->writeLine(L"opJoinRandomRoom: creating room");
    }
}

void NetworkLogic::run(void)
{
	if(mLastInput == INPUT_EXIT && mStateAccessor.getState() != STATE_DISCONNECTING && mStateAccessor.getState() != STATE_DISCONNECTED)
	{
		disconnect();
		mStateAccessor.setState(STATE_DISCONNECTING);
		mOutputListener->writeLine(L"run() -- terminating application");
	}
    else if (mLastInput == INPUT_RECONNECT)
    {
        connect();
        mStateAccessor.setState(STATE_CONNECTING);
        mOutputListener->writeLine(L"run() -- re-connecting");
    }
	else
	{
		State state = mStateAccessor.getState();
		switch(state)
		{
		case STATE_INITIALIZED:
            connect();
            mStateAccessor.setState(STATE_CONNECTING);
            mOutputListener->writeLine(L"run() -- connecting");
                
			break;
		case STATE_CONNECTING:
			break; // wait for callback
		case STATE_CONNECTED:
			switch(mLastInput)
			{
			case INPUT_CREATE_GAME: // create Game
				opCreateRoom();
				break;
			case INPUT_JOIN_RANDOM_GAME: // join Game
                    if (L"" == mGameID)
                    {
                        opJoinRandomRoom();
                    }
                    else
                    {
                        mLoadBalancingClient.opJoinRoom(mGameID);
                        mOutputListener->writeLine(L"run() -- joining room game:" + mGameID);
                    }
				mStateAccessor.setState(STATE_JOINING);
				mOutputListener->writeLine(L"run() -- joining random game");
				break;
            
			default: // no or illegal input -> stay waiting for legal input
				break;
			}
			break;
		case STATE_JOINING:
			break; // wait for callback
            
        case STATE_CREATED_JOINED:
        case STATE_JOINED:
			
			switch(mLastInput)
			{
			case INPUT_LEAVE_GAME: // leave Game
				mLoadBalancingClient.opLeaveRoom();
				mStateAccessor.setState(STATE_LEAVING);
				mOutputListener->writeLine(L"run() -- leaving game room");
				break;
			default: // no or illegal input -> stay waiting for legal input
				break;
			}
			break;
		case STATE_LEAVING:
			break; // wait for callback
		case STATE_LEFT:
			mStateAccessor.setState(STATE_CONNECTED);
			break;
		case STATE_DISCONNECTING:
			break; // wait for callback
		default:
			break;
		}
	}
	mLastInput = INPUT_NON;
	mLoadBalancingClient.service();
}

void NetworkLogic::sendEvent(ExitGames::Common::Hashtable data, EventType eventType)
{
    mOutputListener->write("Sending event");
    mOutputListener->writeLine(data.toString());
	mLoadBalancingClient.opRaiseEvent(true, data, eventType);

}

void NetworkLogic::customEventAction(int /*playerNr*/, nByte eventCode, const ExitGames::Common::Object& eventContent)
{
    mOutputListener->writeLine("Event received");
    mOutputListener->writeLine(eventContent.toString());
    
    
    ExitGames::Common::Hashtable data = ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>(eventContent).getDataCopy() ;
    
    mOutputListener->callForEvent(data, (EventType)eventCode);
}

void NetworkLogic::connectionErrorReturn(int errorCode)
{
	EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"code: %d", errorCode);
	mOutputListener->writeLine(ExitGames::Common::JString(L"connection failed with error ") + errorCode);
	mStateAccessor.setState(STATE_DISCONNECTED);
}

void NetworkLogic::clientErrorReturn(int errorCode)
{
	EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"code: %d", errorCode);
	mOutputListener->writeLine(ExitGames::Common::JString(L"received error ") + errorCode + L" from client");
}

void NetworkLogic::warningReturn(int warningCode)
{
	EGLOG(ExitGames::Common::DebugLevel::WARNINGS, L"code: %d", warningCode);
	mOutputListener->writeLine(ExitGames::Common::JString(L"received warning ") + warningCode + L" from client");
}

void NetworkLogic::serverErrorReturn(int errorCode)
{
	EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"code: %d", errorCode);
	mOutputListener->writeLine(ExitGames::Common::JString(L"received error ") + errorCode + " from server");
}

void NetworkLogic::joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player)
{
	mOutputListener->writeLine(ExitGames::Common::JString(L"player ") + playerNr + L" " + player.getName() + L" has joined the game");
    
    int noOfPlayers = mLoadBalancingClient.getCurrentlyJoinedRoom().getPlayers().getSize();
    
    ExitGames::LoadBalancing::MutableRoom room = mLoadBalancingClient.getCurrentlyJoinedRoom();
    
    
    char ff[100] = {0};
    sprintf(ff, "players in room %d", noOfPlayers);
        
    mOutputListener->writeLine(ff);
   
    
    if (noOfPlayers >= MAX_NUMBER_OF_PLAYERS)
    {
        mOutputListener->writeLine(L"Game full");
        
        ConnectionRelatedData data;
        data.PlayerName = player.getName().UTF8Representation().cstr();
        data.RoomName = mLoadBalancingClient.getCurrentlyJoinedRoom().getName().UTF8Representation().cstr();
        mGameID = mLoadBalancingClient.getCurrentlyJoinedRoom().getName().UTF8Representation().cstr();
        mOutputListener->broadcastRoomStateChanged(evtPlayerConnected, data);
    }
    mOutputListener->writeLine(room.toString());
}

void NetworkLogic::leaveRoomEventAction(int playerNr)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	mOutputListener->writeLine(L"");
	mOutputListener->writeLine(ExitGames::Common::JString(L"player ") + playerNr + " has left the game");
    int noOfPlayers = mLoadBalancingClient.getCurrentlyJoinedRoom().getPlayerCount();
    char ff[100] = {0};
    sprintf(ff, "players in room %d", noOfPlayers);
    
    //if (mLoadBalancingClient.getCurrentlyJoinedRoom().getPlayers().getSize() < MAX_NUMBER_OF_PLAYERS)
    {
        mOutputListener->writeLine(L"Game incomplete");
        ConnectionRelatedData data;
        data.OpponentIsDisconnected = true;
        
        mOutputListener->broadcastRoomStateChanged(evtPlayerDisconnected, data);
    }

    
    char buf[100] = {0};
    sprintf(buf, "players in room %d", mLoadBalancingClient.getCurrentlyJoinedRoom().getPlayers().getSize());
    
    mOutputListener->writeLine( ExitGames::Common::JString(buf));

}

void NetworkLogic::connectReturn(int errorCode, const ExitGames::Common::JString& errorString)
{
	if(errorCode)
	{
		EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mStateAccessor.setState(STATE_DISCONNECTING);
        
		return;
	}
	mOutputListener->writeLine(L"connected");
	mStateAccessor.setState(STATE_CONNECTED);
}

void NetworkLogic::disconnectReturn(void)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	mOutputListener->writeLine(L"disconnectReturn() -- disconnected");
	mStateAccessor.setState(STATE_DISCONNECTED);
}

void NetworkLogic::createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	if(errorCode)
	{
		mOutputListener->writeLine(L"opCreateRoom() failed: " + errorString);
		mStateAccessor.setState(STATE_CONNECTED);
		return;
	}

	mOutputListener->writeLine(L"createRoomReturn -> game room \"" + mLoadBalancingClient.getCurrentlyJoinedRoom().toString() + "\" has been created");
	
	mStateAccessor.setState(STATE_CREATED_JOINED);
}

void NetworkLogic::joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	if(errorCode)
	{
		EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mOutputListener->writeLine(L"opJoinRoom() failed: " + errorString);
		mStateAccessor.setState(STATE_CONNECTED);
		return;
	}
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"localPlayerNr: %d", localPlayerNr);
	mOutputListener->writeLine(L"game room \"" + mLoadBalancingClient.getCurrentlyJoinedRoom().getName() + "\" has been successfully joined");
	mOutputListener->writeLine(L"regularly sending dummy events now");
	mStateAccessor.setState(STATE_JOINED);
}

void NetworkLogic::joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	if(errorCode)
	{
		EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mOutputListener->writeLine(L"opJoinRandomRoom() failed: " + errorString);
		mStateAccessor.setState(STATE_CONNECTED);
		return;
	}
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"localPlayerNr: %d", localPlayerNr);
	mOutputListener->writeLine(L"game room \"" + mLoadBalancingClient.getCurrentlyJoinedRoom().getName() + "\" has been successfully joined");
	mOutputListener->writeLine(L"regularly sending dummy events now");
	mStateAccessor.setState(STATE_JOINED);
}

void NetworkLogic::leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	if(errorCode)
	{
		EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mOutputListener->writeLine(L"opLeaveRoom() failed: " + errorString);
		mStateAccessor.setState(STATE_DISCONNECTING);
		return;
	}
	mOutputListener->writeLine(L"game room has been successfully left");
	mStateAccessor.setState(STATE_LEFT);
}

void NetworkLogic::gotQueuedReturn(void)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
    mOutputListener->writeLine(L"returned to qued");
}

void NetworkLogic::joinLobbyReturn(void)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	mOutputListener->writeLine(L"joined lobby");
}

void NetworkLogic::leaveLobbyReturn(void)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	mOutputListener->writeLine(L"left lobby");
}

// protocol implementations
void NetworkLogic::debugReturn(const ExitGames::Common::JString& string)
{
	mOutputListener->debugReturn(string);
}

void NetworkLogic::debugReturn(ExitGames::Common::DebugLevel::DebugLevel /*debugLevel*/, const ExitGames::Common::JString& string)
{
	debugReturn(string);
}
