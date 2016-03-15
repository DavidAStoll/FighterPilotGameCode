/*
 * CAndroidFighterPilotThePacificWar.cpp
 *
 *  Created on: May 8, 2012
 *      Author: dstoll
 */
#include "includes/native/CAndroidFighterPilotThePacificWar.h"
#include "includes/core/utility/IPC.h"

//JNI classes
#include "includes/native/graphic/AndroidGraphicsEngine.h"
#include "includes/native/utility/CAndroidAssetDirectory.h"
#include "includes/native/utility/CAndroidFileStream.h"
#include "includes/native/utility/CAndroidImageStore.h"
#include "includes/native/utility/CAndroidSystem.h"
#include "includes/native/utility/CAndroidTCPConnectionEngine.h"
#include "includes/native/utility/CAndroidAmazonGameCircleFunctions.h"

JavaVM* CAndroidFighterPilotThePacificWar::VM = NULL;

CAndroidFighterPilotThePacificWar::CAndroidFighterPilotThePacificWar(JNIEnv* aJniEnv)
:CFighterPilotThePacificWar()
{
	//need to create various java objects here
	CAndroidAssetDirectory::InitJavaAssetDirectoryObject(aJniEnv);
	CAndroidReadStream::InitJavaReadStreamObject(aJniEnv);
	CAndroidWriteStream::InitJavaWriteStreamObject(aJniEnv);
	CAndroidDirectory::InitJavaDirectoryObject(aJniEnv);
	CAndroidGraphicsEngine::InitJavaAndroidGraphicsEngine(aJniEnv);
	CAndroidImageStore::InitJavaImageStoreObject(aJniEnv);
	CAndroidSystem::InitJavaAndroidSystemObject(aJniEnv);
	CAndroidTCPConnectionEngine::InitJavaAndroidTCPConnectionEngineObject(aJniEnv);
	CAndroidAmazonGameCircleFunctions::InitJavaAndroidAmazonGameCircleFunctionsObject(aJniEnv);
}

CAndroidFighterPilotThePacificWar::~CAndroidFighterPilotThePacificWar()
{
	// TODO Auto-generated destructor stub
}

JNIEnv* CAndroidFighterPilotThePacificWar::GetJNI()
{
	JNIEnv* lJniEnv = NULL;
	CAndroidFighterPilotThePacificWar::VM->GetEnv((void**) &lJniEnv, JNI_VERSION_1_6);
	return lJniEnv;
}

//------------------------- NEEDED FOR JAVA NATIVE FUNCTIONS --------------------------//

extern "C"
{
	JNIEXPORT void JNICALL Java_fighterPilot_RisingSunAlpha_AndroidGameThread_ExitGame(JNIEnv* lEnv, jobject obj)
	{
		CGame* lGame = CGame::Game;
		if(lGame->iUserStatsTrackerDaemon != NULL)
			lGame->iUserStatsTrackerDaemon->SetAgreedToUploadStats(false); //little trick, let deamon save stats to disk, since sending it will not work. Android disables sockets

		CGame::Game->ExitGame();
	}

	JNIEXPORT void JNICALL Java_fighterPilot_RisingSunAlpha_AndroidGameThread_InitializeGame(JNIEnv* lEnv, jobject obj)
		{
			CGame* lGame = new CAndroidFighterPilotThePacificWar(lEnv);
			lGame->InitializeGame();
		}

	JNIEXPORT void JNICALL Java_fighterPilot_RisingSunAlpha_AndroidGameThread_InitializeOpenGl(JNIEnv* lEnv, jobject obj)
		{
			CGame* lGame = CGame::Game;
			CGame::Game->iGraphicsEngine->RecreateOpenGlContext();
		}

	JNIEXPORT void JNICALL Java_fighterPilot_RisingSunAlpha_AndroidGameThread_ResumeGame(JNIEnv* lEnv, jobject obj)
		{
			CGame* lGame = CGame::Game;
			lGame->ResumeGame();
		}

	JNIEXPORT bool JNICALL Java_fighterPilot_RisingSunAlpha_AndroidGameThread_SuspendGame(JNIEnv* lEnv, jobject obj)
		{
			if(CGame::Game != NULL)
			{
				CGame::Game->SuspendGame();
				while(CGame::Game->GetIsSuspended() != true)
				{}

				return true;
			}
			else
			{
				return false; //game might not be created yet
			}
		}

	JNIEXPORT void JNICALL Java_fighterPilot_RisingSunAlpha_AndroidGameThread_DestroyOpenGl(JNIEnv* lEnv, jobject obj)
		{
			if(CGame::Game != NULL)
			{
				CGame::Game->SuspendGame();
				while(CGame::Game->GetIsSuspended() != true)
				{}

				//need to remove all mappings since the OpenGL context will be lost next
				CGame::Game->iGraphicsFont->RemoveAllCharacterFontMappings();
				//game is suspended, we also need to delete the current OpenGL context
				CGame::Game->iGraphicsEngine->DestroyOpenGlContext();
			}
		}
}

// JNI OnLoad, needed for the JNI
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* lVm, void* reserved)
{
	CAndroidFighterPilotThePacificWar::VM = lVm;
  return JNI_VERSION_1_6;
}

void CAndroidFighterPilotThePacificWar::CreateJavaObjectForVM(JNIEnv* aEnv, const char* aJavaClassPath, jobject* aObjectPointer)
{
	jclass lJavaClass = aEnv->FindClass(aJavaClassPath);
	jmethodID lJavaObjectMethod = aEnv->GetMethodID(lJavaClass, "<init>", "()V");
	jobject lJavaObject = aEnv->NewObject(lJavaClass, lJavaObjectMethod);

	//make it permament object to avoid recycling
	(*aObjectPointer) = aEnv->NewGlobalRef(lJavaObject);

	//clean up
	aEnv->DeleteLocalRef(lJavaClass);
	aEnv->DeleteLocalRef(lJavaObject); //local reference is not needed anymore
}

